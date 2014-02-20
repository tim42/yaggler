//
// file : material_usings.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/material_usings.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 31/01/2014 17:57:46
//
//
// Copyright (C) 2014 Timothée Feuillet
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//

#ifndef __N_214703487977944557_959918726__MATERIAL_USINGS_HPP__
# define __N_214703487977944557_959918726__MATERIAL_USINGS_HPP__

#include <klmb/tools/ct_texture_list.hpp>
#include <klmb/tools/ct_shader_list.hpp>
#include <klmb/tools/ct_shader_type.hpp>

#include <shader/shader_base.hpp>
#include <shader/shader_options.hpp>

#include <texture/texture_base.hpp>
#include <texture/opengl_texture.hpp>
#include <texture/png_loader.hpp>

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      // /// some useful usings for materials

      // /// /// texture usings

      // empty texture list
      using no_textures = texture_list<>;

      // png textures only, no mipmaps from images
      // it takes only the filename of the textures (as a ct string)
      template<const char *... Strings>
      using rgba_png_2d_texture_list =  neam::klmb::yaggler::texture_list
      <
        neam::yaggler::texture::texture
        <
          neam::yaggler::type::opengl, neam::embed::GLenum<GL_TEXTURE_2D>,
          neam::yaggler::texture::options::png_texture_init<GL_RGBA, neam::embed::string<Strings> >
        >...
      >;

      template<const char * Filename>
      using rgba_png_2d_texture = neam::yaggler::texture::texture
      <
        neam::yaggler::type::opengl, neam::embed::GLenum<GL_TEXTURE_2D>,
        neam::yaggler::texture::options::png_texture_init<GL_RGBA, neam::embed::string<Filename> >
      >;

      // /// /// shaders usings

      // shader from a file, compiled only one time (no FS watcher)
      template<GLenum ShaderType, const char *Filename>
      using file_shader = neam::yaggler::shader::shader
      <
        neam::yaggler::type::opengl, neam::embed::GLenum<ShaderType>,
        neam::yaggler::shader::opengl::file, neam::embed::string<Filename>,
        neam::embed::shader::option<neam::yaggler::shader::shader_option::one_shot_compilation>
      >;
      // shader from a constexpr string
      template<GLenum ShaderType, const char *CTString>
      using ct_string_shader = neam::yaggler::shader::shader
      <
        neam::yaggler::type::opengl, neam::embed::GLenum<ShaderType>,
        neam::yaggler::shader::opengl::constexpr_string, neam::embed::string<CTString>,
        neam::embed::shader::option<neam::yaggler::shader::shader_option::one_shot_compilation>
      >;
      // shader from a string
      template<GLenum ShaderType, const char *CTString>
      using string_shader = neam::yaggler::shader::shader
      <
        neam::yaggler::type::opengl, neam::embed::GLenum<ShaderType>,
        neam::yaggler::shader::opengl::dyn_string, neam::embed::string<CTString>,
        neam::embed::shader::option<neam::yaggler::shader::shader_option::one_shot_compilation>
      >;
      // shader from a function call
      template<GLenum ShaderType, const char *(*Function)()>
      using shader_from_function_call_char = neam::yaggler::shader::shader
      <
        neam::yaggler::type::opengl, neam::embed::GLenum<ShaderType>,
        neam::yaggler::shader::opengl::function, neam::embed::shader::function_ptr_char<Function>,
        neam::embed::shader::option<neam::yaggler::shader::shader_option::one_shot_compilation>
      >;
      template<GLenum ShaderType, std::string (*Function)()>
      using shader_from_function_call_string = neam::yaggler::shader::shader
      <
        neam::yaggler::type::opengl, neam::embed::GLenum<ShaderType>,
        neam::yaggler::shader::opengl::function, neam::embed::shader::function_ptr_string<Function>,
        neam::embed::shader::option<neam::yaggler::shader::shader_option::one_shot_compilation>
      >;

      // shader from a file, compiled only one time (no FS watcher)
      // shader type is deduced from file extension at compile time.
      //
      // default YägGLer shader extensions are:
      //
      // .frag        --> fragment shader
      // .geom        --> geometry shader
      // .tess-eval   --> tesselation evaluation shader
      // .tess-ctrl   --> tesselation control shader
      // .vert        --> vertex shader
      //
      // .compute     --> compute shader
      //
      template<const char *Filename>
      using auto_file_shader = file_shader<internal::shader_type_from_filename<Filename>::shader_type, Filename>;

      // some default shaders
      constexpr neam::string_t klmb_default_vert = "data/klmb-framework/defaults/default.vert";
      constexpr neam::string_t klmb_default_frag = "data/klmb-framework/defaults/default.frag";
      using default_shaders = shader_list
      <
        file_shader<GL_FRAGMENT_SHADER, klmb_default_frag>,
        file_shader<GL_VERTEX_SHADER, klmb_default_vert>
      >;

    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_214703487977944557_959918726__MATERIAL_USINGS_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

