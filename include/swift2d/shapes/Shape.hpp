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

#ifndef SWIFT2D_SHAPE_HPP
#define SWIFT2D_SHAPE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/TransformableComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Shape;
typedef std::shared_ptr<Shape>       ShapePtr;
typedef std::shared_ptr<const Shape> ConstShapePtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL Shape : public TransformableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------ public methods
  virtual bool contains(ShapePtr const& other) = 0;
  virtual bool intersects(ShapePtr const& other) = 0;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_SHAPE_HPP
