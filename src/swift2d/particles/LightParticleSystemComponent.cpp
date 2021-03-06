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
#include <swift2d/particles/LightParticleSystemComponent.hpp>

#include <swift2d/particles/LightParticleShader.hpp>
#include <swift2d/particles/ParticleSystem.hpp>
#include <swift2d/graphics/RendererPool.hpp>
#include <swift2d/textures/DefaultTexture.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

LightParticleSystemComponent::LightParticleSystemComponent()
  : MidLife(0.5f)
  , StartScale(1.f),               MidScale(1.f),                 EndScale(1.f)
  , StartColor(Color(1, 1, 1, 1)), MidColor(Color(1, 1, 1, 0.5)), EndColor(Color(1, 1, 1, 0)) {}

////////////////////////////////////////////////////////////////////////////////

void LightParticleSystemComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;

  ParticleSystemComponent::serialize(s);

  s.MidLife = MidLife();
  s.StartScale = StartScale();
  s.MidScale = MidScale();
  s.EndScale = EndScale();
  s.StartColor = StartColor().vec4();
  s.MidColor = MidColor().vec4();
  s.EndColor = EndColor().vec4();
  s.Texture = Texture();

  scene->renderers().light_particle_systems.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void LightParticleSystemComponent::accept(SavableObjectVisitor& visitor) {
  ParticleSystemComponent::accept(visitor);
  visitor.add_member("MidLife",    MidLife);
  visitor.add_member("StartScale", StartScale);
  visitor.add_member("MidScale",   MidScale);
  visitor.add_member("EndScale",   EndScale);
  visitor.add_member("StartColor", StartColor);
  visitor.add_member("MidColor",   MidColor);
  visitor.add_member("EndColor",   EndColor);
  visitor.add_object_property("Texture",     Texture);
}

////////////////////////////////////////////////////////////////////////////////

void LightParticleSystemComponent::Renderer::predraw(RenderContext const& ctx) {
  for (auto& object : objects) {
    SWIFT_PUSH_GL_RANGE("Update LightParticleSystem");
    object.System->update_particles(object, ctx);
    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

void LightParticleSystemComponent::Renderer::draw(RenderContext const& ctx, int start, int end) {
  for (int i(start); i<end; ++i) {
    auto& o(objects[i]);

    SWIFT_PUSH_GL_RANGE("Draw LightParticleSystem");

    if (o.System->get_particle_count() > 0) {

      if (o.Texture) {
        o.Texture->bind(ctx, 3);
      } else {
        DefaultTexture::get().bind(3);
      }

      ogl::Context::BlendFunc(
        oglplus::BlendFunction::One,
        oglplus::BlendFunction::One
      );

      auto& shader(LightParticleShader::get());
      shader.use();
      shader.projection.       Set(ctx.projection_matrix);
      shader.diffuse.          Set(3);
      shader.scale_mid_life.   Set(math::vec4(o.StartScale, o.MidScale, o.EndScale, o.MidLife));
      shader.start_color.      Set(o.StartColor);
      shader.mid_color.        Set(o.MidColor);
      shader.end_color.        Set(o.EndColor);
      shader.screen_size.      Set(ctx.g_buffer_size/(ctx.light_sub_sampling ? 2 : 1));
      shader.g_buffer_normal.  Set(1);
      shader.g_buffer_light.   Set(2);

      o.System->draw_particles(ctx);
    }

    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

}
