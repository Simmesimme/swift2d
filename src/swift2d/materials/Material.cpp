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
#include <swift2d/materials/Material.hpp>

#include <swift2d/geometries/Quad.hpp>
#include <swift2d/databases/TextureDatabase.hpp>
#include <swift2d/materials/MaterialShaderFactory.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Material::Material()
  : AnimatedDiffuseTexture(nullptr)
  , DiffuseTexture(nullptr)
  , AnimatedNormalTexture(nullptr)
  , NormalTexture(nullptr)
  , AnimatedEmitTexture(nullptr)
  , EmitTexture(nullptr)
  , AnimatedGlowTexture(nullptr)
  , GlowTexture(nullptr)
  , AnimatedShinynessTexture(nullptr)
  , ShinynessTexture(nullptr)
  , Diffuse(Color(1, 1, 1))
  , Emit(1.0)
  , Glow(0.0)
  , Shinyness(1.0)
  , BlendAdditive(false)
  , TexcoordOffset(math::vec2(0.f,0.f))
  , TexcoordScale(math::vec2(1.f,1.f))
  , current_shader_dirty_(true)
  , current_shader_(nullptr) {

  AnimatedDiffuseTexture.on_change().connect([this](TexturePtr const& val){
    current_shader_dirty_ = true;
    return true;
  });

  DiffuseTexture.on_change().connect([this](TexturePtr const& val){
    current_shader_dirty_ = true;
    return true;
  });

  NormalTexture.on_change().connect([this](TexturePtr const& val){
    current_shader_dirty_ = true;
    return true;
  });

  EmitTexture.on_change().connect([this](TexturePtr const& val){
    current_shader_dirty_ = true;
    return true;
  });

  GlowTexture.on_change().connect([this](TexturePtr const& val){
    current_shader_dirty_ = true;
    return true;
  });

  ShinynessTexture.on_change().connect([this](TexturePtr const& val){
    current_shader_dirty_ = true;
    return true;
  });

  DiffuseTextureName.on_change().connect([this](std::string const& val) {
    DiffuseTexture = TextureDatabase::get().lookup_or_load(val);
    return true;
  });
  NormalTextureName.on_change().connect([this](std::string const& val) {
    NormalTexture = TextureDatabase::get().lookup_or_load(val);
    return true;
  });
  EmitTextureName.on_change().connect([this](std::string const& val) {
    EmitTexture = TextureDatabase::get().lookup_or_load(val);
    return true;
  });
  GlowTextureName.on_change().connect([this](std::string const& val) {
    GlowTexture = TextureDatabase::get().lookup_or_load(val);
    return true;
  });
  ShinynessTextureName.on_change().connect([this](std::string const& val) {
    ShinynessTexture = TextureDatabase::get().lookup_or_load(val);
    return true;
  });
}

////////////////////////////////////////////////////////////////////////////////

void Material::draw_quad(RenderContext const& ctx, math::mat3 const& transform,
                         float depth, float time) {
  draw_quad_impl(ctx, {transform}, ctx.projection_matrix, depth, {time});
}

////////////////////////////////////////////////////////////////////////////////

void Material::draw_quads(RenderContext const& ctx, std::vector<math::mat3> const& transforms,
                         float depth, std::vector<float> const& times) {
  draw_quad_impl(ctx, transforms, ctx.projection_matrix, depth, times);
}

////////////////////////////////////////////////////////////////////////////////

void Material::draw_fullscreen_quad(RenderContext const& ctx, float time) {
  draw_quad_impl(ctx, {math::mat3()}, math::mat3(), 1.0f, {time});
}

////////////////////////////////////////////////////////////////////////////////

void Material::accept(SavableObjectVisitor& visitor) {
  visitor.add_object_property("AnimatedDiffuseTexture", AnimatedDiffuseTexture);
  visitor.add_object_property("DiffuseTexture", DiffuseTexture);
  visitor.add_member("DiffuseTextureName", DiffuseTextureName);
  visitor.add_member("Diffuse", Diffuse);
  visitor.add_member("NormalTextureName", NormalTextureName);
  visitor.add_object_property("NormalTexture", NormalTexture);
  visitor.add_object_property("EmitTexture", EmitTexture);
  visitor.add_member("EmitTextureName", EmitTextureName);
  visitor.add_member("Emit", Emit);
  visitor.add_object_property("GlowTexture", GlowTexture);
  visitor.add_member("GlowTextureName", GlowTextureName);
  visitor.add_member("Glow", Glow);
  visitor.add_object_property("ShinynessTexture", ShinynessTexture);
  visitor.add_member("ShinynessTextureName", ShinynessTextureName);
  visitor.add_member("Shinyness", Shinyness);
  visitor.add_member("BlendAdditive", BlendAdditive);
  visitor.add_member("TexcoordOffset", TexcoordOffset);
  visitor.add_member("TexcoordScale", TexcoordScale);
}

////////////////////////////////////////////////////////////////////////////////

void Material::draw_quad_impl(RenderContext const& ctx,
                              std::vector<math::mat3> const& transforms,
                              math::mat3 const& projection, float depth,
                              std::vector<float> const& times) {

  if (current_shader_dirty_) {
    current_shader_dirty_ = false;

    int capabilities(0);

    if (AnimatedDiffuseTexture()) {
      capabilities |= MaterialShaderFactory::ANIMATED_DIFFUSE_TEX;
    }

    if (DiffuseTexture()) {
      capabilities |= MaterialShaderFactory::DIFFUSE_TEX;
    }

    if (AnimatedNormalTexture()) {
      capabilities |= MaterialShaderFactory::ANIMATED_NORMAL_TEX;
    }

    if (NormalTexture()) {
      capabilities |= MaterialShaderFactory::NORMAL_TEX;
    }

    if (AnimatedEmitTexture()) {
      capabilities |= MaterialShaderFactory::ANIMATED_EMIT_TEX;
    }

    if (EmitTexture()) {
      capabilities |= MaterialShaderFactory::EMIT_TEX;
    }

    if (AnimatedGlowTexture()) {
      capabilities |= MaterialShaderFactory::ANIMATED_GLOW_TEX;
    }

    if (GlowTexture()) {
      capabilities |= MaterialShaderFactory::GLOW_TEX;
    }

    if (AnimatedShinynessTexture()) {
      capabilities |= MaterialShaderFactory::ANIMATED_SHINYNESS_TEX;
    }

    if (ShinynessTexture()) {
      capabilities |= MaterialShaderFactory::SHINYNESS_TEX;
    }

    current_shader_ = MaterialShaderFactory::get().get_shader(capabilities);
  }

  current_shader_->use();
  current_shader_->projection.Set(projection);
  current_shader_->depth.Set(depth);
  current_shader_->parallax.Set(ctx.projection_parallax);

  bool needs_time(false);
  bool needs_texcoords(false);

  if (AnimatedDiffuseTexture()) {
    AnimatedDiffuseTexture()->bind(ctx, 0);
    current_shader_->diffuse_tex.Set(0);
    needs_time = true;
    needs_texcoords = true;

  } else if (DiffuseTexture()) {
    DiffuseTexture()->bind(ctx, 0);
    current_shader_->diffuse_tex.Set(0);
    needs_texcoords = true;
  }
  current_shader_->diffuse.Set(Diffuse().vec4());


  if (AnimatedNormalTexture()) {
    AnimatedNormalTexture()->bind(ctx, 1);
    current_shader_->normal_tex.Set(1);
    current_shader_->normal_transform.Set(math::make_rotation(math::get_rotation(transforms[0])));
    needs_time = true;
    needs_texcoords = true;

  } else if (NormalTexture()) {
    NormalTexture()->bind(ctx, 1);
    current_shader_->normal_tex.Set(1);
    current_shader_->normal_transform.Set(math::make_rotation(math::get_rotation(transforms[0])));
    needs_texcoords = true;
  }

  if (AnimatedEmitTexture()) {
    AnimatedEmitTexture()->bind(ctx, 2);
    current_shader_->emit_tex.Set(2);
    needs_time = true;
    needs_texcoords = true;

  } else if (EmitTexture()) {
    EmitTexture()->bind(ctx, 2);
    current_shader_->emit_tex.Set(2);
    needs_texcoords = true;
  }
  current_shader_->emit.Set(Emit());

  if (AnimatedGlowTexture()) {
    AnimatedGlowTexture()->bind(ctx, 3);
    current_shader_->glow_tex.Set(3);
    needs_time = true;
    needs_texcoords = true;

  } else if (GlowTexture()) {
    GlowTexture()->bind(ctx, 3);
    current_shader_->glow_tex.Set(3);
    needs_texcoords = true;
  }
  current_shader_->glow.Set(Glow());

  if (AnimatedShinynessTexture()) {
    AnimatedShinynessTexture()->bind(ctx, 4);
    current_shader_->shinyness_tex.Set(4);
    needs_time = true;
    needs_texcoords = true;

  } else if (ShinynessTexture()) {
    ShinynessTexture()->bind(ctx, 4);
    current_shader_->shinyness_tex.Set(4);
  }
  current_shader_->shinyness.Set(Shinyness());

  if (needs_texcoords) {
    current_shader_->texcoord_offset_scale.Set(math::vec4(
      TexcoordOffset().x(), TexcoordOffset().y(),
      TexcoordScale().x(), TexcoordScale().y()
    ));
  }

  bool blend(BlendAdditive());

  if (blend) {
    ogl::Context::BlendFunc(oglplus::BlendFn::SrcAlpha, oglplus::BlendFn::One);
  }

  int index(0);

  while (index < transforms.size()) {
    std::size_t count(std::min(100, (int)transforms.size()-index));

    current_shader_->transform.Set(count, &transforms[index]);
    if (needs_time) {
      current_shader_->time.Set(count, &times[index]);
    }

    Quad::get().draw(count);
    index += count;
  }


  if (blend) {
    ogl::Context::BlendFunc(oglplus::BlendFn::SrcAlpha, oglplus::BlendFn::OneMinusSrcAlpha);
  }
}

////////////////////////////////////////////////////////////////////////////////

}
