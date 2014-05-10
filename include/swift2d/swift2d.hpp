////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_HPP
#define SWIFT2D_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/events.hpp>
#include <swift2d/math.hpp>
#include <swift2d/scene.hpp>
#include <swift2d/renderer.hpp>
#include <swift2d/input/keys.hpp>

namespace swift {

/**
 * Initialize swift2d.
 *
 * This should be called once at the beginning of every application
 * using swift2d.
 */

void init(int argc, char** argv);

}

#endif  // SWIFT2D_HPP

