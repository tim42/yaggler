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
          texture(size_t _spl_idx = 0)
            : ownership(true), id(0), spl_idx(_spl_idx)
          {
            glGenTextures(1, &id);
            bind();

            // default filters
            set_gl_parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            set_gl_parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            // nvidia debug flood :)
            set_gl_parameter(GL_TEXTURE_MAX_LEVEL, 0);

            __tpl_init();
          }

          texture(GLuint _id, size_t _spl_idx)
            : ownership(false), id(_id), spl_idx(_spl_idx)
          {
            __tpl_init();
          }

          texture(GLuint _id, size_t _spl_idx, assume_ownership_t)
            : ownership(true), id(_id), spl_idx(_spl_idx)
          {
            __tpl_init();
          }

          texture(const texture &o)
            : ownership(false), id(o.get_id()), spl_idx(o.get_texture_sampler()), image_texture(o.image_texture),
              image_format(o.image_format), image_access_mode(o.image_access_mode), handle(o.handle)
          {
            __tpl_init();
          }
          template<typename... TArgs>
          texture(const texture< type::opengl, TextureType, TArgs... > &o)
            : ownership(false), id(o.get_id()), spl_idx(o.get_texture_sampler()), image_texture(o.is_image_texture()),
              image_format(o.get_image_format()), image_access_mode(o.get_image_access_mode()), handle(o.get_image_handle())
          {
            __tpl_init();
          }

          texture(texture &o, stole_ownership_t)
            : ownership(o.has_ownership()), id(o.get_id()), spl_idx(o.get_texture_sampler()), image_texture(o.image_texture),
              image_format(o.image_format), image_access_mode(o.image_access_mode), handle(o.handle)
          {
            o.give_up_ownership();
            __tpl_init();
          }
          template<typename... TArgs>
          texture(texture< type::opengl, TextureType, TArgs... > &o, stole_ownership_t)
            : ownership(o.has_ownership()), id(o.get_id()), spl_idx(o.get_texture_sampler()), image_texture(o.is_image_texture()),
              image_format(o.get_image_format()), image_access_mode(o.get_image_access_mode()), handle(o.get_image_handle())
          {
            o.give_up_ownership();
            __tpl_init();
          }

          texture(texture &&o)
            : ownership(o.has_ownership()), id(o.get_id()), spl_idx(o.get_texture_sampler()), image_texture(o.image_texture),
              image_format(o.image_format), image_access_mode(o.image_access_mode), handle(o.handle)
          {
            o.give_up_ownership();
            __tpl_init();
          }
          template<typename... TArgs>
          texture(texture< type::opengl, TextureType, TArgs... > &&o)
            : ownership(o.has_ownership()), id(o.get_id()), spl_idx(o.get_texture_sampler()), image_texture(o.is_image_texture()),
              image_format(o.get_image_format()), image_access_mode(o.get_image_access_mode()), handle(o.get_image_handle())
          {
            o.give_up_ownership();
            __tpl_init();
          }

          ~texture()
          {
            if (ownership)
              glDeleteTextures(1, &id);
          }

          // give up the buffer WITHOUT DELETING IT
          // (simply become a link)
          texture &give_up_ownership()
          {
            ownership = false;
            return *this;
          }

          texture &assume_ownership()
          {
            ownership = true;
            return *this;
          }

          // see stole_ownership_t
          template<typename... BArgs>
          texture &stole(texture<type::opengl, TextureType, BArgs...> &o)
          {
            if (&o != this)
            {
              if (ownership)
                glDeleteTextures(1, &id);

              ownership = o.has_ownership();
              id = o.get_id();
              spl_idx = o.get_texture_sampler();
              image_texture = o.image_texture;
              image_format = (o.image_format);
              image_access_mode = (o.image_access_mode);
              handle = (o.handle);
              o.give_up();
            }
            return *this;
          }

          // create a simple link
          template<typename... BArgs>
          texture &link_to(const texture<type::opengl, TextureType, BArgs...> &o)
          {
            if (&o != this)
            {
              if (ownership)
                glDeleteTextures(1, &id);

              ownership = false;
              id = o.get_id();
              spl_idx = o.get_texture_sampler();
              image_texture = o.image_texture;
              image_format = (o.image_format);
              image_access_mode = (o.image_access_mode);
              handle = (o.handle);
            }
            return *this;
          }

          // this won't do what you want.
          texture &operator = (const texture &) = delete;

          GLuint get_id() const
          {
            return id;
          }

          bool has_ownership() const
          {
            return ownership;
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

          // HACK # TODO: change this. (and make a ct init for this.)
          void set_image_texture(bool _image_texture, GLenum format, GLenum access_mode)
          {
            image_texture = _image_texture;
            image_format = format;
            image_access_mode = access_mode;
          }

          // NOTE: you must call set_image_texture
          // NOTE: THIS IS A HACK.
          // HACK # TODO: change this.
          uint64_t get_image_handle()
          {
            if (image_texture)
            {
              handle = glGetImageHandleARB(id, 0, GL_TRUE, 0, image_format);
              return handle;
            }
            return 0;
          }
          uint64_t get_image_handle() const
          {
            if (image_texture)
              return handle;
            return 0;
          }
          bool is_image_texture() const
          {
            return image_texture;
          }
          GLenum get_image_format() const
          {
            return image_format;
          }
          GLenum get_image_access_mode() const
          {
            return image_access_mode;
          }
          void make_resident(bool resident = true)
          {
            if (resident)
              glMakeImageHandleResidentARB(handle, image_access_mode);
            else
              glMakeImageHandleNonResidentARB(handle);
          }

          // use the texture (bind it to the T.U.)
          void use() const
          {
            if (!image_texture)
            {
              glActiveTexture(GL_TEXTURE0 + spl_idx);
              glBindTexture(TextureType::value, id);
            }
            else
              glBindImageTexture(spl_idx, id, 0, GL_TRUE, 0, image_access_mode, image_format);
          }

          void unuse() const
          {
            if (!image_texture)
              glBindTexture(TextureType::value, 0);
            else
              glBindImageTexture(spl_idx, 0, 0, GL_TRUE, 0, image_access_mode, image_format);
          }

          // same thing than 'use' but do not bind it to a TU
          void bind() const
          {
            glBindTexture(TextureType::value, id);
          }

          void unbind() const
          {
            glBindTexture(TextureType::value, 0);
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

//           template<typename InitType>
//           auto set_texture_data(const InitType &it)
//           -> NCR_ENABLE_IF(TextureType::value == GL_TEXTURE_RECTANGLE, void)
//           {
//             set_texture_data(it.internal_format, it.size, it.format, it.type, it.data);
//           }
          template<typename InitType>
          auto set_texture_data(const InitType &it)
          -> NCR_ENABLE_IF(TextureType::value != GL_TEXTURE_RECTANGLE && TextureType::value != GL_TEXTURE_CUBE_MAP, void)
          {
            set_texture_data(it.internal_format, it.size, it.format, it.type, it.data, it.level);
          }
          template<typename InitType>
          auto set_texture_data(cubemap_face face, const InitType &it)
          -> NCR_ENABLE_IF(TextureType::value == GL_TEXTURE_CUBE_MAP, void)
          {
            set_texture_data(face, it.internal_format, it.size, it.format, it.type, it.data, it.level);
          }

          //           template<typename InitType>
//           auto set_sub_texture_data(const decltype(InitType::size) &offset, const InitType &it)
//           -> NCR_ENABLE_IF(TextureType::value == GL_TEXTURE_RECTANGLE, void)
//           {
//             set_sub_texture_data(offset, it.size, it.format, it.type, it.data);
//           }
          template<typename InitType>
          auto set_sub_texture_data(const decltype(InitType::size) &offset, const InitType &it)
          -> NCR_ENABLE_IF(TextureType::value != GL_TEXTURE_RECTANGLE && TextureType::value != GL_TEXTURE_CUBE_MAP, void)
          {
            set_sub_texture_data(offset, it.size, it.format, it.type, it.data, it.level);
          }
          template<typename InitType>
          auto set_sub_texture_data(cubemap_face face, const decltype(InitType::size) &offset, const InitType &it)
          -> NCR_ENABLE_IF(TextureType::value == GL_TEXTURE_CUBE_MAP, void)
          {
            set_sub_texture_data(face, offset, it.size, it.format, it.type, it.data, it.level);
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

          // set 1D texture data
          // I now this is a bit messy, but it's the best I can do at this hour of the night.
          //
          // format / type is the couple that define 'data' format and type
          // internal_format define the way data will be stored by openGL
          // size is the x size of the image data. (ATTENTION: not in fixed pos, but as integers)
          //
          // doc: https://www.opengl.org/sdk/docs/man/xhtml/glTexImage1D.xml
          auto set_sub_texture_data(ct::fixed_t offset, ct::fixed_t size, GLenum format, GLenum type, GLvoid *data, GLint level = 0)
          -> NCR_ENABLE_IF((TextureType::value == GL_TEXTURE_1D), void)
          {
            bind();
            glTexSubImage1D(TextureType::value, level, offset, size, format, type, data);
          }

          // set 2D texture data
          // I now this is a bit messy, but it's the best I can do at this hour of the night.
          //
          // format / type is the couple that define 'data' format and type
          // internal_format define the way data will be stored by openGL
          // size is the x/y size of the image data. (ATTENTION: not in fixed pos, but as integers)
          //
          // doc: https://www.opengl.org/sdk/docs/man/xhtml/glTexImage2D.xml
          auto set_sub_texture_data(const ct::vector2 &offset, const ct::vector2 &size, GLenum format, GLenum type, GLvoid *data, GLint level = 0)
          -> NCR_ENABLE_IF((TextureType::value == GL_TEXTURE_2D || TextureType::value == GL_TEXTURE_1D_ARRAY), void)
          {
            bind();
            glTexSubImage2D(TextureType::value, level, offset.x, offset.y, size.x, size.y, format, type, data);
          }
          // for rectangle, as the specs says that level MUST be 0.
          auto set_sub_texture_data(const ct::vector2 &offset, const ct::vector2 &size, GLenum format, GLenum type, GLvoid *data)
          -> NCR_ENABLE_IF(TextureType::value == GL_TEXTURE_RECTANGLE, void)
          {
            bind();
            glTexSubImage2D(TextureType::value, 0, offset.x, offset.y, size.x, size.y, format, type, data);
          }

          // the same as the GL_TEXTURE_2D, but you also had to specify the cubemap face
          auto set_sub_texture_data(cubemap_face face, const ct::vector2 &offset, const ct::vector2 &size, GLenum format, GLenum type, GLvoid *data, GLint level = 0)
          -> NCR_ENABLE_IF(TextureType::value == GL_TEXTURE_CUBE_MAP, void)
          {
            bind();
            glTexSubImage2D(static_cast<GLenum>(face), level, offset.x, offset.y, size.x, size.y, format, type, data);
          }

          // set 3D texture data
          // I now this is a bit messy, but it's the best I can do at this hour of the night.
          //
          // format / type is the couple that define 'data' format and type
          // internal_format define the way data will be stored by openGL
          // size is the x/y/z size of the image data. (ATTENTION: not in fixed pos, but as integers)
          //
          // doc: https://www.opengl.org/sdk/docs/man/xhtml/glTexImage3D.xml
          auto set_sub_texture_data(const ct::vector3 &offset, const ct::vector3 &size, GLenum format, GLenum type, GLvoid *data, GLint level = 0)
          -> NCR_ENABLE_IF((TextureType::value == GL_TEXTURE_3D || TextureType::value == GL_TEXTURE_2D_ARRAY), void)
          {
            bind();
            glTexSubImage3D(TextureType::value, level, offset.x, offset.y, offset.z, size.x, size.y, size.z, format, type, data);
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

          // create a link to a more generic texture.
          // no inheritance involved. This cast will create a 'link' program shader object.
          operator texture<type::opengl, TextureType> ()
          {
            return texture<type::opengl, TextureType> (*this);
          }

        private:
          bool ownership;
          GLuint id;
          size_t spl_idx;
          bool image_texture = false;
          GLenum image_format;
          GLenum image_access_mode;
          uint64_t handle = 0;
      };
    } // namespace texture
  } // namespace yaggler
} // namespace neam

#endif /*__N_470500441990277920_992406037__OPENGL_TEXTURE_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

