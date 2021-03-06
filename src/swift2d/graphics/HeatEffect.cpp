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
#include <swift2d/graphics/HeatEffect.hpp>

#include <swift2d/components/DrawableComponent.hpp>
#include <swift2d/graphics/RendererPool.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

HeatEffect::HeatEffect(RenderContext const& ctx) {
  if (ctx.heat_effect) {

    auto create_texture = [&](
      oglplus::Texture& tex, int width, int height,
      oglplus::enums::PixelDataInternalFormat i_format,
      oglplus::enums::PixelDataFormat         p_format) {

      ogl::Context::Bound(oglplus::Texture::Target::_2D, tex)
        .Image2D(0, i_format, width, height,
          0, p_format, oglplus::PixelDataType::Float, nullptr)
        .MaxLevel(0)
        .MinFilter(oglplus::TextureMinFilter::Linear)
        .MagFilter(oglplus::TextureMagFilter::Linear)
        .WrapS(oglplus::TextureWrap::MirroredRepeat)
        .WrapT(oglplus::TextureWrap::MirroredRepeat);
    };

    create_texture(
      heat_buffer_, ctx.g_buffer_size.x()/4, ctx.g_buffer_size.y()/4,
      oglplus::PixelDataInternalFormat::RG,
      oglplus::PixelDataFormat::RG
    );

    heat_fbo_.Bind(oglplus::Framebuffer::Target::Draw);
    oglplus::Framebuffer::AttachColorTexture(
      oglplus::Framebuffer::Target::Draw, 0, heat_buffer_, 0
    );

    GLfloat clear[2] = {0.5f, 0.5f};
    ogl::Context::ClearColorBuffer(0, clear);
  }
}

////////////////////////////////////////////////////////////////////////////////

void HeatEffect::process(ConstSerializedScenePtr const& scene,
                         RenderContext const& ctx) {

  if (!scene->renderers().heat_particle_systems.empty()) {

    ogl::Context::BlendFunc(
      oglplus::BlendFunction::SrcAlpha,
      oglplus::BlendFunction::OneMinusSrcAlpha
    );

    ogl::Context::Viewport(ctx.g_buffer_size.x()/4, ctx.g_buffer_size.y()/4);

    heat_fbo_.Bind(oglplus::Framebuffer::Target::Draw);
    ogl::Context::DrawBuffer(oglplus::FramebufferColorAttachment::_0);

    GLfloat clear[2] = {0.5f, 0.5f};
    ogl::Context::ClearColorBuffer(0, clear);

    scene->renderers().process_heat(ctx);
  }
}

////////////////////////////////////////////////////////////////////////////////

int HeatEffect::bind_buffers(int start, RenderContext const& ctx) {
  oglplus::Texture::Active(start);
  ogl::Context::Bind(ose::_2D(), heat_buffer_);
  return start + 1;
}

////////////////////////////////////////////////////////////////////////////////

}
