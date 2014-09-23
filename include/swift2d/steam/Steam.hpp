////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_STEAM_HPP
#define SWIFT2D_STEAM_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Singleton.hpp>
#include <swift2d/properties.hpp>
#include <unordered_map>

struct LobbyChatUpdate_t;
struct LobbyChatMsg_t;
struct LobbyEnter_t;
struct PersonaStateChange_t;

namespace swift {

template<typename T> class SteamCallback;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class Steam : public Singleton<Steam> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  enum MessageType {
    CHAT          = 0,
    CHAT_UPDATE   = 1,
    LEAVE         = 2,
    JOIN          = 3,
    MESSAGE       = 4,
    ERROR         = 5
  };

  struct RoomData {
    std::string name;
    int         max_players;
    int         current_players;
  };

  Signal<std::unordered_map<uint64_t, RoomData>> on_updated_room_list;
  Signal<MessageType, uint64_t, std::string>     on_message;

  bool init();
  void update();

  void        update_room_list();
  uint64_t    get_room_id();

  void        create_room(std::string const& name);
  void        join_room(uint64_t room_id);
  void        leave_room();

  void        set_room_data(std::string const& key, std::string const& value);
  std::string get_room_data(std::string const& key);
  uint64_t    get_room_owner();

  void        send_chat_message(std::string const& message);

  uint64_t    get_user_id();
  std::string get_user_name();
  std::string get_user_name(uint64_t steam_id);
  std::string get_user_avatar(uint64_t steam_id);

  friend class Singleton<Steam>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  Steam();
  ~Steam();

  void save_avatar(uint64_t steam_id);

  uint64_t current_room_;

  SteamCallback<LobbyChatUpdate_t>*     chat_update_;
  SteamCallback<LobbyChatMsg_t>*        chat_message_;
  SteamCallback<LobbyEnter_t>*          lobby_enter_;
  SteamCallback<PersonaStateChange_t>*  persona_change_;

  std::unordered_map<uint64_t, std::string> avatar_cache_;
};

}

#endif  // SWIFT2D_STEAM_HPP
