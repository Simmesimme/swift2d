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

#ifndef SWIFT2D_CIRCLE_COLLISION_SHAPE_HPP
#define SWIFT2D_CIRCLE_COLLISION_SHAPE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/physics/CollisionShape.hpp>

// forward declares ------------------------------------------------------------
class b2Shape;

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class CircleCollisionShape;
typedef std::shared_ptr<CircleCollisionShape>       CircleCollisionShapePtr;
typedef std::shared_ptr<const CircleCollisionShape> ConstCircleCollisionShapePtr;
typedef Property<CircleCollisionShapePtr>           CircleCollisionShapeProperty;

// -----------------------------------------------------------------------------
class SWIFT_DLL CircleCollisionShape : public CollisionShape {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  float Radius;

  // ----------------------------------------------------- contruction interface
  static CircleCollisionShapePtr create() {
    return std::make_shared<CircleCollisionShape>();
  }

  CircleCollisionShape();

  // ------------------------------------------------------------ public methods
  b2Shape* get_shape(math::mat3 const& body_transform) const;

  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "CircleCollisionShape"; }
  virtual void accept(SavableObjectVisitor& visitor);
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_CIRCLE_COLLISION_SHAPE_HPP
