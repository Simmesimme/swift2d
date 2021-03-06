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

#ifndef SWIFT2D_DEAL_DAMAGE_COMPONENT_HPP
#define SWIFT2D_DEAL_DAMAGE_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>
#include <swift2d/math.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class DealDamageComponent;
typedef std::shared_ptr<DealDamageComponent>       DealDamageComponentPtr;
typedef std::shared_ptr<const DealDamageComponent> ConstDealDamageComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL DealDamageComponent : public Component {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Float  Amount;
  UInt64 DamageSourceID;

  // ----------------------------------------------------- contruction interface
  DealDamageComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static DealDamageComponentPtr create(Args&& ... a) {
    return std::make_shared<DealDamageComponent>(a...);
  }

  // creates a copy from this
  DealDamageComponentPtr create_copy() const {
    return std::make_shared<DealDamageComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "DealDamageComponent"; }

  virtual void on_init();

  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
};

}

#endif  // SWIFT2D_DEAL_DAMAGE_COMPONENT_HPP
