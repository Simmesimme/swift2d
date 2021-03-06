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

#ifndef SWIFT2D_CIRCULAR_SHAPE_HPP
#define SWIFT2D_CIRCULAR_SHAPE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/shapes/Shape.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// A screen core represents a rectangular, virtual screen. Combined with a    //
// view core it defines the viewing frustum.                                  //
////////////////////////////////////////////////////////////////////////////////

// forward declares ------------------------------------------------------------
class RectangularShape;
typedef std::shared_ptr<RectangularShape>       RectangularShapePtr;

// shared pointer type definition ----------------------------------------------
class CircularShape;
typedef std::shared_ptr<CircularShape>       CircularShapePtr;
typedef std::shared_ptr<const CircularShape> ConstCircularShapePtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL CircularShape : public TransformableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static CircularShapePtr create(Args&& ... a) {
    return std::make_shared<CircularShape>(a...);
  }

  // creates a copy from this
  CircularShapePtr create_copy() const {
    return std::make_shared<CircularShape>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "CircularShape"; }


  /*virtual*/ void update(double time) {
    TransformableComponent::update(time);
    center_ = (WorldTransform() * math::vec3(0, 0, 1)).xy();
    auto diameter((WorldTransform() * math::vec3(1, 0, 0)).xy());
    square_radius_ = diameter.x()*diameter.x() + diameter.y()*diameter.y();
  }

  /*virtual*/ bool contains(CircularShapePtr const& other) {
    auto dist(center_ - other->center_);
    return dist.x()*dist.x() + dist.y()*dist.y() < other->square_radius_ - square_radius_;
  };

  /*virtual*/ bool intersects(CircularShapePtr const& other) {
    auto dist(center_ - other->center_);
    return dist.x()*dist.x() + dist.y()*dist.y() < square_radius_ + other->square_radius_;
  };

  /*virtual*/ bool contains(RectangularShapePtr const& other) {
    return false;
  };

  /*virtual*/ bool intersects(RectangularShapePtr const& other) {
    return false;
  };

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  math::vec2  center_;
  float       square_radius_;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_CIRCULAR_SHAPE_HPP
