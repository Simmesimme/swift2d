////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_RENDERER_HPP
#define SWIFT2D_RENDERER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/FPSCounter.hpp>
#include <swift2d/graphics/RenderClient.hpp>
#include <swift2d/scene/SerializedScene.hpp>
#include <vector>
#include <string>
#include <memory>

namespace swift {

// forward declares ------------------------------------------------------------
class Pipeline;
typedef std::shared_ptr<Pipeline> PipelinePtr;

class SceneObject;
typedef std::shared_ptr<SceneObject>       SceneObjectPtr;

////////////////////////////////////////////////////////////////////////////////
// Manages the rendering on multiple contexts.                                //
// This class is used to provide a renderer frontend interface to the user.   //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class Renderer {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  Renderer(PipelinePtr const& pipeline);
  virtual ~Renderer();

  void process(SceneObjectPtr const& scene);

  void stop();

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  RenderClient<ConstSerializedScenePtr>* render_client_;
  FPSCounter                             application_fps_;
};

}

#endif  // SWIFT2D_RENDERER_HPP
