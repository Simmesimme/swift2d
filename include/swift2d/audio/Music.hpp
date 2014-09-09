////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_MUSIC_HPP
#define SWIFT2D_MUSIC_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/audio/AudioBuffer.hpp>
#include <swift2d/properties.hpp>

#include <mutex>
#include <queue>
#include <thread>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Music;
typedef std::shared_ptr<Music>       MusicPtr;
typedef std::shared_ptr<const Music> ConstMusicPtr;
typedef Property<MusicPtr>           MusicProperty;

// -----------------------------------------------------------------------------
class Music : public AudioBuffer {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  static MusicPtr create() {
    return std::make_shared<Music>();
  }

  static MusicPtr create_from_file(std::string const& file_name) {
    auto music(std::make_shared<Music>());
    music->load_from_file(file_name);
    return music;
  }

  Music();
  virtual ~Music();

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "Music"; }

  std::string const& get_title();
  std::string const& get_artist();
  std::string const& get_album();
  std::string const& get_year();

  // internal interface --------------------------------------------------------
  void load(oalplus::Source* source);
  void unload(oalplus::Source* source);
  void update(oalplus::Source* source, double time);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 protected:
  void load_from_file(std::string const& file_name);

 private:
  std::string file_;

  std::string title_;
  std::string artist_;
  std::string album_;
  std::string year_;
  int sample_rate_;
  int channels_;

  std::vector<ALuint> buffers_;
  unsigned long max_frames_;

  std::mutex load_mutex_;
  std::thread load_thread_;
  std::queue<std::vector<short>> buffer_queue_;
  bool playing_;
};

}

#endif // SWIFT2D_MUSIC_HPP
