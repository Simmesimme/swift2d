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

#ifndef SWIFT2D_SAVABLE_OBJECT_HPP
#define SWIFT2D_SAVABLE_OBJECT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/objects/Object.hpp>

#include <memory>
#include <functional>
#include <unordered_map>

namespace swift {

// forward declares ------------------------------------------------------------
class SavableObjectVisitor;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class SavableObject;
typedef std::shared_ptr<SavableObject>       SavableObjectPtr;
typedef std::shared_ptr<const SavableObject> ConstSavableObjectPtr;
// typedef Property<SavableObjectPtr>           SavableObjectProperty;

// -----------------------------------------------------------------------------
class SWIFT_DLL SavableObject : public Object {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  void          save_to_file(std::string const& path);
  std::string   save_to_buffer();

  void          load_from_file(std::string const& path);
  void          load_from_buffer(std::string const& buffer);

  static SavableObjectPtr create_from_file(std::string const& path);
  static SavableObjectPtr create_from_buffer(std::string const& buffer);

  virtual void accept(SavableObjectVisitor& to) {};
  virtual void on_loaded() {};
};

}

#endif  // SWIFT2D_SAVABLE_OBJECT_HPP
