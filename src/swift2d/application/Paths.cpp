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

// class header
#include <swift2d/application/Paths.hpp>

#include <swift2d/utils/Logger.hpp>

#include <boost/filesystem.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Paths::Paths()
  : executable_() {}

////////////////////////////////////////////////////////////////////////////////

void Paths::init(int argc, char** argv) {

  executable_ = boost::filesystem::system_complete(argv[0]).normalize().remove_filename().string();

  if (executable_ == "") {
    LOG_ERROR << "Failed to get executable path!" << std::endl;
    return;
  }

  // create tmp directory ------------------------------------------------------
  boost::filesystem::create_directory(executable_ + "/tmp");
}

////////////////////////////////////////////////////////////////////////////////

void Paths::clean_up() {

  // delete tmp directory ------------------------------------------------------
  if (!boost::filesystem::remove_all(executable_ + "/tmp")) {
    LOG_ERROR << "Failed to delete temporary directory!" << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////

std::string Paths::tmp_file(std::string const& suffix) const {
  std::string file(boost::filesystem::unique_path().string());
  boost::filesystem::path p(executable_ + "/tmp/" + file + "." + suffix);
  return p.normalize().string();
}

////////////////////////////////////////////////////////////////////////////////

std::string Paths::resource(std::string const& type, std::string const& file) const {
  boost::filesystem::path p(executable_ + "/resources/" + type + "/" + file);
  return p.normalize().string();
}

////////////////////////////////////////////////////////////////////////////////

std::string Paths::make_absolute(std::string const& file) const {
  auto p(boost::filesystem::absolute(file, executable_));
  return p.normalize().string();
}

////////////////////////////////////////////////////////////////////////////////

std::string Paths::get_extension(std::string const& file) const {
  boost::filesystem::path p(file);
  return p.extension().string();
}

////////////////////////////////////////////////////////////////////////////////

std::set<std::string> Paths::list_files(std::string const& directory) const {
  boost::filesystem::path dir(directory);
  boost::filesystem::directory_iterator end_iter;

  std::set<std::string> result;

  if (boost::filesystem::exists(dir) && boost::filesystem::is_directory(dir)) {
    for(boost::filesystem::directory_iterator dir_iter(dir); dir_iter != end_iter; ++dir_iter) {
      if (boost::filesystem::is_regular_file(dir_iter->status())) {
        result.insert(boost::filesystem::path(*dir_iter).normalize().string());
      }
    }
  }

  return result;
}

////////////////////////////////////////////////////////////////////////////////

}
