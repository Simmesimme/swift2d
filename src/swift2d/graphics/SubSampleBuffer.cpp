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
#include <swift2d/graphics/SubSampleBuffer.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

SubSampleBuffer::SubSampleBuffer(RenderContext const& ctx, int sub_sample_level)
  : sub_sample_level_(sub_sample_level) {

  auto create_texture = [&](
    oglplus::Texture& tex, int width, int height,
    oglplus::enums::PixelDataInternalFormat i_format,
    oglplus::enums::PixelDataFormat         p_format) {

    ogl::Context::Bound(oglplus::Texture::Target::_2D, tex)
      .Image2D(0, i_format, width, height,
        0, p_format, oglplus::PixelDataType::UnsignedByte, nullptr)
      .MaxLevel(0)
      .MinFilter((ctx.sub_sampling || sub_sample_level_ > 1) ? oglplus::TextureMinFilter::Linear : oglplus::TextureMinFilter::Nearest)
      .MagFilter((ctx.sub_sampling || sub_sample_level_ > 1) ? oglplus::TextureMagFilter::Linear : oglplus::TextureMagFilter::Nearest)
      .WrapS(oglplus::TextureWrap::MirroredRepeat)
      .WrapT(oglplus::TextureWrap::MirroredRepeat);
  };

  // create textures -----------------------------------------------------------
  auto size(ctx.g_buffer_size/sub_sample_level);

  create_texture(
    diffuse_, size.x(), size.y(),
    oglplus::PixelDataInternalFormat::RGBA8,
    oglplus::PixelDataFormat::RGBA
  );

  if (ctx.dynamic_lighting) {
    create_texture(
      normal_, size.x(), size.y(),
      oglplus::PixelDataInternalFormat::RGB8,
      oglplus::PixelDataFormat::RGB
    );
    create_texture(
      light_, size.x(), size.y(),
      oglplus::PixelDataInternalFormat::RGB8,
      oglplus::PixelDataFormat::RGB
    );
  }

  // create framebuffer object -------------------------------------------------
  fbo_.Bind(oglplus::Framebuffer::Target::Draw);

  oglplus::Framebuffer::AttachColorTexture(
    oglplus::Framebuffer::Target::Draw, 0, diffuse_, 0
  );

  if (ctx.dynamic_lighting) {
    oglplus::Framebuffer::AttachColorTexture(
      oglplus::Framebuffer::Target::Draw, 1, normal_, 0
    );
    oglplus::Framebuffer::AttachColorTexture(
      oglplus::Framebuffer::Target::Draw, 2, light_, 0
    );
  }
}

////////////////////////////////////////////////////////////////////////////////

void SubSampleBuffer::bind_for_drawing(RenderContext const& ctx) {

  auto size(ctx.g_buffer_size/sub_sample_level_);
  ogl::Context::Viewport(size.x(), size.y());

  if (ctx.dynamic_lighting) {

    fbo_.Bind(oglplus::Framebuffer::Target::Draw);

    oglplus::Context::ColorBuffer draw_buffs[3] =  {
      oglplus::FramebufferColorAttachment::_0,
      oglplus::FramebufferColorAttachment::_1,
      oglplus::FramebufferColorAttachment::_2
    };
    ogl::Context::DrawBuffers(draw_buffs);
    ogl::Context::ClearColor(0.f, 0.f, 0.f, 0.f);
    ogl::Context::Clear().ColorBuffer();

  } else {
    fbo_.Bind(oglplus::Framebuffer::Target::Draw);

    oglplus::Context::ColorBuffer draw_buffs[1] =  {
      oglplus::FramebufferColorAttachment::_0
    };
    ogl::Context::DrawBuffers(draw_buffs);
    ogl::Context::ClearColor(0.f, 0.f, 0.f, 0.f);
    ogl::Context::Clear().ColorBuffer();
  }
}

////////////////////////////////////////////////////////////////////////////////

void SubSampleBuffer::bind_diffuse(int location) {
  oglplus::Texture::Active(location);
  diffuse_.Bind(oglplus::Texture::Target::_2D);
}

////////////////////////////////////////////////////////////////////////////////

void SubSampleBuffer::bind_normal(int location) {
  oglplus::Texture::Active(location);
  normal_.Bind(oglplus::Texture::Target::_2D);
}

////////////////////////////////////////////////////////////////////////////////

void SubSampleBuffer::bind_light(int location) {
  oglplus::Texture::Active(location);
  light_.Bind(oglplus::Texture::Target::_2D);
}

////////////////////////////////////////////////////////////////////////////////

}
