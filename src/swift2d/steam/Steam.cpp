////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
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
      what = "entered the room";
    }

    if (result->m_rgfChatMemberStateChange == k_EChatMemberStateChangeLeft) {
      what = "left the room";
    }

    if (result->m_rgfChatMemberStateChange == k_EChatMemberStateChangeDisconnected) {
      what = "disconnected";
    }

    if (result->m_rgfChatMemberStateChange == k_EChatMemberStateChangeKicked) {
      what = "was kicked";
    }

    if (result->m_rgfChatMemberStateChange == k_EChatMemberStateChangeBanned) {
      what = "was banned";
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
    current_room_ = result->m_ulSteamIDLobby;
    std::string name(SteamMatchmaking()->GetLobbyData(current_room_, "name"));

    on_message.emit(MessageType::MSG_JOIN, get_user_id(), "joined " + name);

    int user_count = SteamMatchmaking()->GetNumLobbyMembers(current_room_);
    for (int i(0); i<user_count; ++i) {
      auto user = SteamMatchmaking()->GetLobbyMemberByIndex(current_room_, i);
      on_message.emit(MessageType::MSG_CHAT_UPDATE, user.ConvertToUint64(), "is in this room");
    }
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
        set_room_data("owner_network_id", std::to_string(Network::get().get_own_id()));
      } else {
        LOG_WARNING << "failed to create lobby" << std::endl;
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

std::string Steam::get_room_data(std::string const& key) {
  return SteamMatchmaking()->GetLobbyData(current_room_, key.c_str());
}

////////////////////////////////////////////////////////////////////////////////

math::uint64 Steam::get_room_owner_id() {
  return SteamMatchmaking()->GetLobbyOwner(current_room_).ConvertToUint64();
}

////////////////////////////////////////////////////////////////////////////////

math::uint64 Steam::get_room_owner_network_id() {
  std::string owner_network_id(get_room_data("owner_network_id"));

  if (owner_network_id == "") {
    return 0;
  }

  return std::from_string<math::uint64>(owner_network_id);
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

    std::string name(SteamMatchmaking()->GetLobbyData(current_room_, "name"));
    on_message.emit(MessageType::MSG_LEAVE, get_user_id(), "left " + name);

    SteamMatchmaking()->LeaveLobby(current_room_);
    current_room_ = 0;

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
