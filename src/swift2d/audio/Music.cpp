////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/audio/Music.hpp>
#include <sndfile.h>
#include <vector>
#include <array>

#define BUFFER_COUNT 3

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Music::Music()
  : buffers_(BUFFER_COUNT)
  , playing_id_(0) {}

////////////////////////////////////////////////////////////////////////////////

Music::~Music() {
  alDeleteBuffers(BUFFER_COUNT, buffers_.data());
}

////////////////////////////////////////////////////////////////////////////////

void Music::load_from_file(std::string const& file_name) {

  file_ = file_name;

  SF_INFO info;
  SNDFILE* file = sf_open(file_.c_str(), SFM_READ, &info);

  auto tmp(sf_get_string(file, SF_STR_TITLE));
  if (tmp) title_  = std::string(tmp);
  tmp = sf_get_string(file, SF_STR_ARTIST);
  if (tmp) artist_ = std::string(tmp);
  tmp = sf_get_string(file, SF_STR_ALBUM);
  if (tmp) album_  = std::string(tmp);
  tmp = sf_get_string(file, SF_STR_DATE);
  if (tmp) year_   = std::string(tmp);

  sample_rate_ = info.samplerate;
  channels_ = info.channels;

  int error(sf_error(file));
  if (error) {
    Logger::LOG_WARNING << "Error loading audio file \"" << file_ << "\": "
                        << sf_error_number(error) << std::endl;
  }

  max_frames_ = info.frames;

  sf_close(file);

  alGenBuffers(BUFFER_COUNT, buffers_.data());
}

////////////////////////////////////////////////////////////////////////////////

void Music::load(oalplus::Source* source) {

  int playing_id = playing_id_;

  std::thread load_thread([this, playing_id](){

    unsigned long current_frame = 0;

    while (playing_id == playing_id_) {
      if (buffer_queue_.size() < BUFFER_COUNT && current_frame < max_frames_) {
        SF_INFO info;
        SNDFILE* file = sf_open(file_.c_str(), SFM_READ, &info);

        // read one second
        int read_amount = std::min(max_frames_, channels_ * sample_rate_ + current_frame) - current_frame;
        current_frame += read_amount;

        std::vector<short> data(channels_ * read_amount);

        sf_seek(file, current_frame - read_amount, SEEK_SET);
        sf_readf_short(file, data.data(), read_amount);

        sf_close(file);

        std::unique_lock<std::mutex> lock(load_mutex_); {
          buffer_queue_.push(data);
        }
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
  });

  load_thread.detach();
}

////////////////////////////////////////////////////////////////////////////////

void Music::unload(oalplus::Source* source) {
  ++playing_id_;

  while (!buffer_queue_.empty()) {
    buffer_queue_.pop();
  }

  source->DetachBuffers();
}

////////////////////////////////////////////////////////////////////////////////

void Music::update(oalplus::Source* source, double time) {
  int processed(0);
  int queued(0);
  alGetSourcei(oalplus::GetALName(*source), AL_BUFFERS_PROCESSED, &processed);
  alGetSourcei(oalplus::GetALName(*source), AL_BUFFERS_QUEUED,    &queued);

  while ((processed > 0 || queued < BUFFER_COUNT) && buffer_queue_.size() > 0) {

    auto load_buffer = [this](ALuint buffer){
      std::unique_lock<std::mutex> lock(load_mutex_);
      auto f = channels_ == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
      alBufferData(buffer, f, &buffer_queue_.front().front(),
                   buffer_queue_.front().size() * sizeof(short), sample_rate_);
      buffer_queue_.pop();
    };

    if (queued < BUFFER_COUNT) {
      queued += 1;
      load_buffer(buffers_[queued-1]);
      alSourceQueueBuffers(oalplus::GetALName(*source), 1, &buffers_[queued-1]);

    } else if (processed > 0) {
      // reuse already used buffers
      processed -= 1;

      // unqueue oldest buffer
      ALuint name;
      alSourceUnqueueBuffers(oalplus::GetALName(*source), 1, &name);

      // load & queue next
      load_buffer(name);
      alSourceQueueBuffers(oalplus::GetALName(*source), 1, &name);
    }

    // there was an underflow --- start playing again!
    if (queued == 1) {
      source->Play();
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

std::string const& Music::get_title() {
  return title_;
}
std::string const& Music::get_artist() {
  return artist_;
}
std::string const& Music::get_album() {
  return album_;
}
std::string const& Music::get_year() {
  return year_;
}

////////////////////////////////////////////////////////////////////////////////

}
