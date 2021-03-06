////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/steam/Steam.hpp>

#include <swift2d/network/Network.hpp>
#include <swift2d/application/Paths.hpp>
#include <swift2d/steam/SteamOnceCallback.hpp>
#include <swift2d/steam/SteamCallback.hpp>
#include <swift2d/utils/Logger.hpp>
#include <swift2d/utils/stl_helpers.hpp>

#include <stb_image/stb_image_write.h>

#include <array>
#include <iostream>
#include <steam/steam_api.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Steam::Steam()
  : current_room_(0)
  , chat_update_(nullptr)
  , chat_message_(nullptr)
  , lobby_enter_(nullptr)
  , persona_change_(nullptr) {

  // on chat update ------------------------------------------------------------
  chat_update_ = new SteamCallback<LobbyChatUpdate_t>([this](LobbyChatUpdate_t* result) {

    std::string what;

    if (result->m_rgfChatMemberStateChange == k_EChatMemberStateChangeEntered) {
      what = "join";
    }

    if (result->m_rgfChatMemberStateChange == k_EChatMemberStateChangeLeft) {
      what = "leave";
    }

    if (result->m_rgfChatMemberStateChange == k_EChatMemberStateChangeDisconnected) {
      what = "disconnect";
    }

    if (result->m_rgfChatMemberStateChange == k_EChatMemberStateChangeKicked) {
      what = "kick";
    }

    if (result->m_rgfChatMemberStateChange == k_EChatMemberStateChangeBanned) {
      what = "bann";
    }

    on_message.emit(MessageType::MSG_CHAT_UPDATE, result->m_ulSteamIDUserChanged, what);

  });

  // on chat message -----------------------------------------------------------
  chat_message_ = new SteamCallback<LobbyChatMsg_t>([this](LobbyChatMsg_t* result) {
    CSteamID        speaker;
    EChatEntryType  type;
    char            data[2048];
    int             length = SteamMatchmaking()->GetLobbyChatEntry(
                                result->m_ulSteamIDLobby, result->m_iChatID,
                                &speaker, data, sizeof(data), &type);

    if (type == k_EChatEntryTypeChatMsg) {
      auto message = std::string(data, length);
      on_message.emit(MessageType::MSG_CHAT, speaker.ConvertToUint64(), message);
    }
  });

  // on lobby enter ------------------------------------------------------------
  lobby_enter_ = new SteamCallback<LobbyEnter_t>([this](LobbyEnter_t* result) {

    set_user_data("internal_address", Network::get().get_internal_address());
    set_user_data("external_address", Network::get().get_external_address());
    set_user_data("network_id", std::to_string(Network::get().get_network_id()));

    std::vector<math::uint64> users;

    current_room_ = result->m_ulSteamIDLobby;
    int user_count = SteamMatchmaking()->GetNumLobbyMembers(current_room_);
    for (int i(0); i<user_count; ++i) {
      auto user = SteamMatchmaking()->GetLobbyMemberByIndex(current_room_, i).ConvertToUint64();
      if (user != get_user_id()) {
        users.push_back(user);
      }
    }

    on_joined_room.emit(current_room_, users);
  });

  // persona state change ------------------------------------------------------
  persona_change_ = new SteamCallback<PersonaStateChange_t>([this](PersonaStateChange_t* result) {

    // save avatar
    if (result->m_nChangeFlags == k_EPersonaChangeAvatar) {

      auto avatar(avatar_cache_.find(result->m_ulSteamID));
      std::string file;

      if (avatar == avatar_cache_.end()) {
        file = Paths::get().tmp_file("png");
        avatar_cache_[result->m_ulSteamID] = file;
      } else {
        file = avatar->second;
      }

      save_avatar(result->m_ulSteamID);
    }
  });
}

////////////////////////////////////////////////////////////////////////////////

Steam::~Steam() {
  SteamAPI_Shutdown();
  if(chat_update_)    delete chat_update_;
  if(chat_message_)   delete chat_message_;
  if(lobby_enter_)    delete lobby_enter_;
  if(persona_change_) delete persona_change_;
}

////////////////////////////////////////////////////////////////////////////////

bool Steam::init() {
  if (!SteamAPI_IsSteamRunning()) {
    LOG_WARNING << "Steam is not running." << std::endl;
    LOG_WARNING << "Please start Steam before running the game." << std::endl;
    return false;
  }

  if (!SteamAPI_Init()) {
    LOG_WARNING << "Failed to initialized Steam!" << std::endl;
    return false;
  }

  SteamUtils()->SetOverlayNotificationPosition(k_EPositionBottomLeft);

  return true;
}

////////////////////////////////////////////////////////////////////////////////

void Steam::update() {
  SteamAPI_RunCallbacks();
}

////////////////////////////////////////////////////////////////////////////////

math::uint64 Steam::get_room_id() {
  return current_room_;
}

////////////////////////////////////////////////////////////////////////////////

void Steam::update_room_list() {
  SteamOnceCallback<LobbyMatchList_t>::set(
    SteamMatchmaking()->RequestLobbyList(),
    [](LobbyMatchList_t *result, bool f) {

    std::unordered_map<math::uint64, RoomData> rooms;
    for (int i(0); i<result->m_nLobbiesMatching; ++i) {
      CSteamID id = SteamMatchmaking()->GetLobbyByIndex(i);

      RoomData data;
      data.name             = std::string(SteamMatchmaking()->GetLobbyData(id, "name"));
      data.max_players      = SteamMatchmaking()->GetLobbyMemberLimit(id);
      data.current_players  = SteamMatchmaking()->GetNumLobbyMembers(id);

      rooms[id.ConvertToUint64()] = data;
    }
    Steam::get().on_updated_room_list.emit(rooms);
  });
}

////////////////////////////////////////////////////////////////////////////////

void Steam::create_room(std::string const& name) {
  if (current_room_ == 0) {

    SteamOnceCallback<LobbyCreated_t>::set(
      SteamMatchmaking()->CreateLobby(k_ELobbyTypePublic, 8),
      [this, name](LobbyCreated_t *result, bool f) {

      if (result->m_eResult == k_EResultOK) {
        current_room_ = result->m_ulSteamIDLobby;

        set_room_data("name", name.c_str());

      } else {
        LOG_WARNING << "Failed to create lobby" << std::endl;
      }
    });

  } else {
    LOG_WARNING << "Already in a room" << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////

void Steam::set_room_data(std::string const& key, std::string const& value) {
  SteamMatchmaking()->SetLobbyData(current_room_, key.c_str(), value.c_str());
}

////////////////////////////////////////////////////////////////////////////////

void Steam::set_user_data(std::string const& key, std::string const& value) {
  SteamMatchmaking()->SetLobbyMemberData(current_room_, key.c_str(), value.c_str());
}

////////////////////////////////////////////////////////////////////////////////

std::string Steam::get_room_data(std::string const& key) {
  return SteamMatchmaking()->GetLobbyData(current_room_, key.c_str());
}

////////////////////////////////////////////////////////////////////////////////

std::string Steam::get_user_data(std::string const& key, math::uint64 user) {
  return SteamMatchmaking()->GetLobbyMemberData(current_room_, user, key.c_str());
}

////////////////////////////////////////////////////////////////////////////////

math::uint64 Steam::get_room_owner_id() {
  return SteamMatchmaking()->GetLobbyOwner(current_room_).ConvertToUint64();
}

////////////////////////////////////////////////////////////////////////////////

math::uint64 Steam::get_user_with_address(std::string const& address) {
  int user_count = SteamMatchmaking()->GetNumLobbyMembers(current_room_);
  for (int i(0); i<user_count; ++i) {
    auto user = SteamMatchmaking()->GetLobbyMemberByIndex(current_room_, i).ConvertToUint64();
    if (get_external_address(user) == address || get_internal_address(user) == address) {
      return user;
    }
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////

std::string Steam::get_internal_address(math::uint64 user) {
  return get_user_data("internal_address", user);
}

////////////////////////////////////////////////////////////////////////////////

std::string Steam::get_external_address(math::uint64 user) {
  return get_user_data("external_address", user);
}

////////////////////////////////////////////////////////////////////////////////

math::uint64 Steam::get_network_id(math::uint64 user) {
  return from_string<math::uint64>(get_user_data("network_id", user));
}

////////////////////////////////////////////////////////////////////////////////

void Steam::join_room(math::uint64 id) {
  if (current_room_ != 0) {
    leave_room();
  }
  SteamMatchmaking()->JoinLobby(id);
}

////////////////////////////////////////////////////////////////////////////////

void Steam::leave_room() {
  if (current_room_ != 0) {
    SteamMatchmaking()->LeaveLobby(current_room_);

    math::uint64 last_room(current_room_);

    current_room_ = 0;

    on_left_room.emit(last_room);

  } else {
    LOG_WARNING << "Not in a room" << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////

void Steam::send_chat_message(std::string const& message) {
  if (current_room_ != 0) {

    if (!SteamMatchmaking()->SendLobbyChatMsg(current_room_, message.c_str(), message.length()+1)) {
      LOG_WARNING << "failed to send message" << std::endl;
    }

  } else {
    LOG_WARNING << "Not in a room" << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////

math::uint64 Steam::get_user_id() {
  return SteamUser()->GetSteamID().ConvertToUint64();
}

////////////////////////////////////////////////////////////////////////////////

std::string Steam::get_user_name() {
  return SteamFriends()->GetPersonaName();
}

////////////////////////////////////////////////////////////////////////////////

std::string Steam::get_user_name(math::uint64  steam_id) {
  return SteamFriends()->GetFriendPersonaName(steam_id);
}

////////////////////////////////////////////////////////////////////////////////

std::string Steam::get_user_avatar(math::uint64 steam_id) {
  auto avatar(avatar_cache_.find(steam_id));
  if (avatar != avatar_cache_.end()) {
    return avatar->second;
  }

  SteamFriends()->RequestUserInformation(steam_id, false);

  std::string file(Paths::get().tmp_file("png"));
  avatar_cache_[steam_id] = file;

  save_avatar(steam_id);

  return file;
}

////////////////////////////////////////////////////////////////////////////////

void Steam::save_file_to_cloud(std::string const& file_name, std::string const& file_data) {
  SteamRemoteStorage()->FileWrite(file_name.c_str(), &file_data[0], file_data.size());
}

////////////////////////////////////////////////////////////////////////////////

std::string Steam::load_file_from_cloud(std::string const& file_name) {
  if (SteamRemoteStorage()->FileExists(file_name.c_str())) {
    auto file_size(SteamRemoteStorage()->GetFileSize(file_name.c_str()));
    std::string data(file_size, ' ');
    SteamRemoteStorage()->FileRead(file_name.c_str(), &data[0], file_size);
    return data;
  }

  LOG_WARNING << "Unable to load file " << file_name
              << " from Steam cloud: File doesn't exist" << std::endl;

  return std::string();
}


////////////////////////////////////////////////////////////////////////////////

void Steam::save_avatar(math::uint64 steam_id) {
  int avatar_id = SteamFriends()->GetSmallFriendAvatar(steam_id);

  math::uint32 width;
  math::uint32 height;

  SteamUtils()->GetImageSize(avatar_id, &width, &height);

  int data_length = width*height * 4 * sizeof(math::uint8);
  std::vector<math::uint8> data(data_length);

  if (!SteamUtils()->GetImageRGBA(avatar_id, &data[0], data_length)) {
    LOG_WARNING << "Failed to get avatar image!" << std::endl;
  } else {
    stbi_write_png(avatar_cache_[steam_id].c_str(), width, height, 4, &data[0], width * 4 * sizeof(math::uint8));
  }
}

////////////////////////////////////////////////////////////////////////////////

}
