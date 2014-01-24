//
// file : opengl_texture.hpp
// in : file:///home/tim/projects/yaggler/yaggler/texture/opengl_texture.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 17/12/2013 18:07:26
//
//
// Copyright (C) 2013-2014 Timothée Feuillet
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


#ifndef __N_470500441990277920_992406037__OPENGL_TEXTURE_HPP__
# define __N_470500441990277920_992406037__OPENGL_TEXTURE_HPP__

#include <GLEW/glew.h>
// #include <compiler_fixs.hpp>
#include <tools/enable_if.hpp>
#include <yaggler_type.hpp>

#include <texture/options.hpp>

namespace neam
{
  namespace yaggler
  {
    namespace texture
    {
      // TextureType is the dimensionality (an GLenum embed with those possible values:
      //    GL_TEXTURE_1D,
      //    GL_TEXTURE_2D,
      //    GL_TEXTURE_3D,
      //    GL_TEXTURE_1D_ARRAY,
      //    GL_TEXTURE_2D_ARRAY,
      //    GL_TEXTURE_RECTANGLE,
      //    GL_TEXTURE_CUBE_MAP,
      //    GL_TEXTURE_CUBE_MAP_ARRAY,
      //    GL_TEXTURE_BUFFER,
      //    GL_TEXTURE_2D_MULTISAMPLE or
      //    GL_TEXTURE_2D_MULTISAMPLE_ARRAY)
      template<typename TextureType, typename... Args>
      class texture<type::opengl, TextureType, Args...>
      {
        private:
#ifndef YAGGLER_NO_FUCKING_TESTS
          static_assert(TextureType::value == GL_TEXTURE_1D ||
                        TextureType::value == GL_TEXTURE_2D ||
                        TextureType::value == GL_TEXTURE_3D ||
                        TextureType::value == GL_TEXTURE_1D_ARRAY ||
                        TextureType::value == GL_TEXTURE_2D_ARRAY ||
                        TextureType::value == GL_TEXTURE_RECTANGLE ||
                        TextureType::value == GL_TEXTURE_CUBE_MAP ||
                        TextureType::value == GL_TEXTURE_CUBE_MAP_ARRAY ||
                        TextureType::value == GL_TEXTURE_BUFFER ||
                        TextureType::value == GL_TEXTURE_2D_MULTISAMPLE ||
                        TextureType::value == GL_TEXTURE_2D_MULTISAMPLE_ARRAY ||
                        TextureType::value == GL_TEXTURE_CUBE_MAP_ARRAY,
                        "Invalid texture type");
#endif

          // empty/end
          template<size_t>
          void __rec_init()
          {
          }

          // some init, not a cubemap, not a rectangle
          template<size_t, typename _Opt, typename... _Args>
          auto __rec_init()
          -> NCR_ENABLE_IF((TextureType::value != GL_TEXTURE_CUBE_MAP && TextureType::value != GL_TEXTURE_RECTANGLE), void)
          {
            const _Opt opt;
            set_texture_data(_Opt::internal_format, opt.size, opt.format, opt.type, opt.data, opt.level);
            __rec_init<0, _Args...>();
          }

          // some init, is a rectangle, not a cubemap
          template<size_t, typename _Face, typename _Opt, typename... _Args>
          auto __rec_init()
          -> NCR_ENABLE_IF((TextureType::value == GL_TEXTURE_RECTANGLE), void)
          {
            const _Opt opt;
            set_texture_data(opt.internal_format, opt.size, opt.format, opt.type, opt.data);
            __rec_init<0, _Args...>();
          }

          // some init, not a rectangle, is a cubemap
          template<size_t, typename _Face, typename _Opt, typename... _Args>
          auto __rec_init()
          -> NCR_ENABLE_IF((TextureType::value == GL_TEXTURE_CUBE_MAP), void)
          {
            const _Opt opt;
            set_texture_data(_Face::value, opt.internal_format, opt.size, opt.format, opt.type, opt.data, opt.level);
            __rec_init<0, _Args...>();
          }
          template<size_t, typename _Face, typename _Opt>
          auto __rec_init()
          -> NCR_ENABLE_IF((TextureType::value == GL_TEXTURE_CUBE_MAP), void)
          {
            const _Opt opt;
            set_texture_data(_Face::value, opt.internal_format, opt.size, opt.format, opt.type, opt.data, opt.level);
          }

          // init from template parameters
          void __tpl_init()
          {
            __rec_init<0, Args...>();
          }

        public:
          texture(size_t _spl_idx = 0) : link(false), id(0), spl_idx(_spl_idx)
          {
            glGenTextures(1, &id);
            bind();

            // default filters
            set_gl_parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            set_gl_parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            __tpl_init();
          }

          texture(GLuint _id, size_t _spl_idx) : link(true), id(_id), spl_idx(_spl_idx)
          {
          }

          texture(GLuint _id, size_t _spl_idx, assume_ownership_t) : link(false), id(_id), spl_idx(_spl_idx)
          {
          }

          template<typename... TArgs>
          texture(const texture< type::opengl, TextureType, TArgs... > &o)
            : link(true), id(o.id), spl_idx(o.spl_idx)
          {
#ifndef YAGGLER_NO_FUCKING_TESTS // even if this quite like a sanity test...
            static_assert(!sizeof...(Args), "uh... You've created a shallow clone of a texture, but you've also given it the template args to initialise it...");
#endif
          }

          ~texture()
          {
            if (!link)
              glDeleteTextures(1, &id);
          }

          // this won't do what you want.
          texture(const texture &) = delete;
          texture &operator = (const texture &) = delete;

          GLuint get_id() const
          {
            return id;
          }

          bool is_link() const
          {
            return link;
          }

          // the active texture
          void set_texture_sampler(size_t _spl_idx)
          {
            spl_idx = _spl_idx;
          }

          // the return value is used to bind samplers in shaders
          size_t get_texture_sampler() const
          {
            return spl_idx;
          }

          // FIXME: is that OK (does this mean what it's intended to mean ?)
          // return the same thing than get_texture_sampler, to be used directly in uniform var affectation
          operator GLint() const
          {
            return spl_idx;
          }

          // use the texture (bind it to the T.U.)
          void use() const
          {
            glActiveTexture(GL_TEXTURE0 + spl_idx);
            glBindTexture(TextureType::value, id);
          }

          // same thing than 'use' but do not bind it to a TU
          void bind() const
          {
            glBindTexture(TextureType::value, id);
          }

          // return the texture type
          static constexpr GLenum get_texture_type()
          {
            return TextureType::value;
          }

          void generate_mipmaps()
          {
            bind();
            glGenerateMipmap(TextureType::value);
          }

          // set 1D texture data
          // I now this is a bit messy, but it's the best I can do at this hour of the night.
          //
          // format / type is the couple that define 'data' format and type
          // internal_format define the way data will be stored by openGL
          // size is the x size of the image data. (ATTENTION: not in fixed pos, but as integers)
          //
          // doc: https://www.opengl.org/sdk/docs/man/xhtml/glTexImage1D.xml
          auto set_texture_data(GLint internal_format, ct::fixed_t size, GLenum format, GLenum type, GLvoid *data, GLint level = 0)
          -> NCR_ENABLE_IF((TextureType::value == GL_TEXTURE_1D), void)
          {
            bind();
            glTexImage1D(TextureType::value, level, internal_format, size, 0, format, type, data);
          }

          // set 2D texture data
          // I now this is a bit messy, but it's the best I can do at this hour of the night.
          //
          // format / type is the couple that define 'data' format and type
          // internal_format define the way data will be stored by openGL
          // size is the x/y size of the image data. (ATTENTION: not in fixed pos, but as integers)
          //
          // doc: https://www.opengl.org/sdk/docs/man/xhtml/glTexImage2D.xml
          auto set_texture_data(GLint internal_format, const ct::vector2 &size, GLenum format, GLenum type, GLvoid *data, GLint level = 0)
          -> NCR_ENABLE_IF((TextureType::value == GL_TEXTURE_2D || TextureType::value == GL_TEXTURE_1D_ARRAY), void)
          {
            bind();
            glTexImage2D(TextureType::value, level, internal_format, size.x, size.y, 0, format, type, data);
          }
          // for rectangle, as the specs says that level MUST be 0.
          auto set_texture_data(GLint internal_format, const ct::vector2 &size, GLenum format, GLenum type, GLvoid *data)
          -> NCR_ENABLE_IF(TextureType::value == GL_TEXTURE_RECTANGLE, void)
          {
            bind();
            glTexImage2D(TextureType::value, 0, internal_format, size.x, size.y, 0, format, type, data);
          }

          // the same as the GL_TEXTURE_2D, but you also had to specify the cubemap face
          auto set_texture_data(cubemap_face face, GLint internal_format, const ct::vector2 &size, GLenum format, GLenum type, GLvoid *data, GLint level = 0)
          -> NCR_ENABLE_IF(TextureType::value == GL_TEXTURE_CUBE_MAP, void)
          {
            bind();
            glTexImage2D(static_cast<GLenum>(face), level, internal_format, size.x, size.y, 0, format, type, data);
          }

          // set 3D texture data
          // I now this is a bit messy, but it's the best I can do at this hour of the night.
          //
          // format / type is the couple that define 'data' format and type
          // internal_format define the way data will be stored by openGL
          // size is the x/y/z size of the image data. (ATTENTION: not in fixed pos, but as integers)
          //
          // doc: https://www.opengl.org/sdk/docs/man/xhtml/glTexImage3D.xml
          auto set_texture_data(GLint internal_format, const ct::vector3 &size, GLenum format, GLenum type, GLvoid *data, GLint level = 0)
          -> NCR_ENABLE_IF((TextureType::value == GL_TEXTURE_3D || TextureType::value == GL_TEXTURE_2D_ARRAY), void)
          {
            bind();
            glTexImage3D(TextureType::value, level, internal_format, size.x, size.y, size.z, 0, format, type, data);
          }

          // set tex parameters (see openGL doc for pname / param @ https://www.opengl.org/sdk/docs/man/xhtml/glTexParameter.xml)
          void set_gl_parameter(GLenum pname, float param)
          {
            bind();
            glTexParameterf(TextureType::value, pname, param);
          }
          void set_gl_parameter(GLenum pname, GLint param)
          {
            bind();
            glTexParameteri(TextureType::value, pname, param);
          }
          void set_gl_parameter(GLenum pname, float *params)
          {
            bind();
            glTexParameterfv(TextureType::value, pname, params);
          }
          void set_gl_parameter(GLenum pname, GLint *params)
          {
            bind();
            glTexParameteriv(TextureType::value, pname, params);
          }

          // border specific ones
          void set_gl_parameterI(GLenum pname, GLint *params)
          {
            bind();
            glTexParameterIiv(TextureType::value, pname, params);
          }
          void set_gl_parameterI(GLenum pname, GLuint *params)
          {
            bind();
            glTexParameterIuiv(TextureType::value, pname, params);
          }


        private:
          bool link;
          GLuint id;
          size_t spl_idx;
      };
    } // namespace texture
  } // namespace yaggler
} // namespace neam

#endif /*__N_470500441990277920_992406037__OPENGL_TEXTURE_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

