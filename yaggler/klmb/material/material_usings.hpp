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
#include <shader/common_shader_loader.hpp>
#include <shader/common_shader_env.hpp>

#include <texture/texture_base.hpp>
#include <texture/opengl_texture.hpp>
#include <texture/png_loader.hpp>

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      /// \file material_usings.hpp
      /// \brief some useful usings for materials and textures

      // /// /// texture usings

      /// \brief empty texture list
      using no_textures = texture_list<>;

      /// \brief Create a texture list from a list of filename
      /// for png textures only, no mipmaps from images
      /// it takes only the filename of the textures (as a ct string)
      template<const char *... Strings>
      using rgba_png_2d_texture_list =  neam::klmb::yaggler::texture_list
      <
        neam::yaggler::texture::texture
        <
          neam::yaggler::type::opengl, neam::embed::GLenum<GL_TEXTURE_2D>,
          neam::yaggler::texture::options::png_texture_init<GL_RGBA, neam::embed::string<Strings> >
        >...
      >;

      /// \brief Create a texture from its filename
      template<const char * Filename>
      using rgba_png_2d_texture = neam::yaggler::texture::texture
      <
        neam::yaggler::type::opengl, neam::embed::GLenum<GL_TEXTURE_2D>,
        neam::yaggler::texture::options::png_texture_init<GL_RGBA, neam::embed::string<Filename> >
      >;

      // /// /// shaders usings
      //
      // NOTE:  please note that shared instances also share the source code. If you change the defines in a ct_string and you rebuild the shader,
      //        every other instances of the class will share the changes.

      /// \brief shader from a file
      template<GLenum ShaderType, const char *Filename, neam::yaggler::shader::shader_option Flags = neam::yaggler::shader::shader_option::one_shot_compilation>
      using file_shader = neam::yaggler::shader::shader
      <
        neam::yaggler::type::opengl, neam::embed::GLenum<ShaderType>,
        neam::yaggler::shader::shader_loader<neam::yaggler::shader::file, neam::embed::string<Filename>>,
        neam::embed::shader::option<Flags>,
        neam::yaggler::shader::shader_env<neam::yaggler::shader::advanced_preprocessor>
      >;

      /// \brief shared shader from a file
      /// \note please note that shared instances also share the source code. If you change the defines in a ct_string and you rebuild the shader,
      ///        every other instances of the class will share the changes.
      template<GLenum ShaderType, const char *Filename, neam::yaggler::shader::shader_option Flags = neam::yaggler::shader::shader_option::none>
      using shared_file_shader = file_shader<ShaderType, Filename, static_cast<neam::yaggler::shader::shader_option>(neam::yaggler::shader::shader_option::shared_instance | Flags)>;

      /// \brief shader from a constexpr string
      template<GLenum ShaderType, const char *CTString>
      using ct_string_shader = neam::yaggler::shader::shader
      <
        neam::yaggler::type::opengl, neam::embed::GLenum<ShaderType>,
        neam::yaggler::shader::shader_loader<neam::yaggler::shader::constexpr_string, neam::embed::string<CTString>>,
        neam::embed::shader::option<neam::yaggler::shader::shader_option::one_shot_compilation>,
        neam::yaggler::shader::shader_env<neam::yaggler::shader::simple_preprocessor>
      >;

      /// \brief shared shader from a constexpr string
      /// \note please note that shared instances also share the source code. If you change the defines in a ct_string and you rebuild the shader,
      ///        every other instances of the class will share the changes.
      template<GLenum ShaderType, const char *CTString>
      using shared_ct_string_shader = neam::yaggler::shader::shader
      <
        neam::yaggler::type::opengl, neam::embed::GLenum<ShaderType>,
        neam::yaggler::shader::shader_loader<neam::yaggler::shader::constexpr_string, neam::embed::string<CTString>>,
        neam::embed::shader::option<neam::yaggler::shader::shader_option::shared_instance>,
        neam::yaggler::shader::shader_env<neam::yaggler::shader::simple_preprocessor>
      >;

      /// \brief shader from a string
      template<GLenum ShaderType, const char *CTString>
      using string_shader = neam::yaggler::shader::shader
      <
        neam::yaggler::type::opengl, neam::embed::GLenum<ShaderType>,
        neam::yaggler::shader::shader_loader<neam::yaggler::shader::dyn_string, neam::embed::string<CTString>>,
        neam::embed::shader::option<neam::yaggler::shader::shader_option::one_shot_compilation>,
        neam::yaggler::shader::shader_env<neam::yaggler::shader::simple_preprocessor>
      >;

      /// \brief shared shader from a string
      /// \note please note that shared instances also share the source code. If you change the defines in a ct_string and you rebuild the shader,
      ///        every other instances of the class will share the changes.
      template<GLenum ShaderType, const char *CTString>
      using shared_string_shader = neam::yaggler::shader::shader
      <
        neam::yaggler::type::opengl, neam::embed::GLenum<ShaderType>,
        neam::yaggler::shader::shader_loader<neam::yaggler::shader::dyn_string, neam::embed::string<CTString>>,
        neam::embed::shader::option<neam::yaggler::shader::shader_option::shared_instance>,
        neam::yaggler::shader::shader_env<neam::yaggler::shader::simple_preprocessor>
      >;


      /// \brief shader from a file, compiled only one time (no FS watcher)
      /// shader type is deduced from file extension at compile time.
      ///
      /// default YägGLer shader extensions are:
      ///
      /// .frag        --> fragment shader
      /// .geom        --> geometry shader
      /// .tess-eval   --> tesselation evaluation shader
      /// .tess-ctrl   --> tesselation control shader
      /// .vert        --> vertex shader
      ///
      /// .compute     --> compute shader
      ///
      template<const char *Filename, neam::yaggler::shader::shader_option Flags = neam::yaggler::shader::shader_option::none>
      using auto_file_shader = file_shader<internal::shader_type_from_filename<Filename>::shader_type, Filename, Flags>;

      /// \brief shader from a file, compiled only one time (no FS watcher)
      /// shader type is deduced from file extension at compile time.
      ///
      /// default YägGLer shader extensions are:
      ///
      /// .frag        --> fragment shader
      /// .geom        --> geometry shader
      /// .tess-eval   --> tesselation evaluation shader
      /// .tess-ctrl   --> tesselation control shader
      /// .vert        --> vertex shader
      ///
      /// .compute     --> compute shader
      ///
      /// \note please note that shared instances also share the source code. If you change the defines in a ct_string and you rebuild the shader,
      ///        every other instances of the class will share the changes.
      template<const char *Filename>
      using auto_shared_file_shader = shared_file_shader<internal::shader_type_from_filename<Filename>::shader_type, Filename>;

      // some default shaders
      extern constexpr neam::string_t klmb_default_vert_file = "data/klmb-framework/defaults/default.vert";
      extern constexpr neam::string_t klmb_default_frag_file = "data/klmb-framework/defaults/default.frag";

      using klmb_default_vert = auto_shared_file_shader<klmb_default_vert_file>;
      using klmb_default_frag = auto_shared_file_shader<klmb_default_frag_file>;

      using default_shaders = shader_list
      <
        klmb_default_frag,
        klmb_default_vert
      >;

      namespace special_shaders
      {
        extern constexpr neam::string_t klmb_fsquad_vert_files = "data/shaders/fsquad/fsquad.vert";

        using klmb_fsquad_vert = auto_shared_file_shader<klmb_fsquad_vert_files>;
      } // namespace special_shaders

    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_214703487977944557_959918726__MATERIAL_USINGS_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

