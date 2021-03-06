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
#include <swift2d/settings/SettingsWrapper.hpp>

#include <swift2d/objects/SavableObjectVisitor.hpp>
#include <swift2d/steam/Steam.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

void SettingsWrapper::set_settings_type(std::string const& type) {
  settings_type_ = type;

  Settings = std::dynamic_pointer_cast<EngineSettings>(Object::create(settings_type_));
}

////////////////////////////////////////////////////////////////////////////////

SettingsWrapper::SettingsWrapper()
  : settings_type_("EngineSettings") {

  Settings = EngineSettings::create();
}

////////////////////////////////////////////////////////////////////////////////

SettingsWrapper::~SettingsWrapper() {}

////////////////////////////////////////////////////////////////////////////////

void SettingsWrapper::load_from_file(std::string const& file_name) {
  auto d(EngineSettings::create_from_file(file_name));
  if (d) {
    Settings = std::dynamic_pointer_cast<EngineSettings>(d);
  } else {
    Settings = std::dynamic_pointer_cast<EngineSettings>(Object::create(settings_type_));
  }
}

////////////////////////////////////////////////////////////////////////////////

void SettingsWrapper::save_to_file(std::string const& file_name) {
  Settings->save_to_file(file_name);
}

////////////////////////////////////////////////////////////////////////////////

void SettingsWrapper::load_from_steam_file(std::string const& file_name) {
  auto buffer = Steam::get().load_file_from_cloud(file_name);
  auto d(EngineSettings::create_from_buffer(buffer));
  if (d) {
    Settings = std::dynamic_pointer_cast<EngineSettings>(d);
  } else {
    Settings = std::dynamic_pointer_cast<EngineSettings>(Object::create(settings_type_));
  }
}

////////////////////////////////////////////////////////////////////////////////

void SettingsWrapper::save_to_steam_file(std::string const& file_name) {

  Steam::get().save_file_to_cloud(file_name, Settings->save_to_buffer());
}


////////////////////////////////////////////////////////////////////////////////

}
