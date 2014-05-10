////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_GEOMETRY_RESOURCE_HPP
#define SWIFT2D_GEOMETRY_RESOURCE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/math/types.hpp>

#include <string>
#include <vector>

namespace swift {

// forward declares ------------------------------------------------------------
struct RenderContext;

////////////////////////////////////////////////////////////////////////////////
// Base class for different geometries.                                       //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class GeometryResource {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // Draws the GeometryResource.
  virtual void draw(RenderContext const& context, math::mat3 const& transform) const = 0;

  // Interface to implement pre-draw tasks (occlusion queries, LOD etc.)
  virtual void predraw(RenderContext const& context) const {}

 ///////////////////////////////////////////////////////////////////////////////
 // -------------------------------------------------------- protected interface
 protected:

};

}

#endif  // SWIFT2D_GEOMETRY_RESOURCE_HPP
