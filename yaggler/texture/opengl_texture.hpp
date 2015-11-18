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
#include <texture/texture_base.hpp>

namespace neam
{
  namespace yaggler
  {
    namespace texture
    {
      /// \brief A wrapper for an openGL Texture object.
      /// \param TextureType is the dimensionality (an GLenum embed with those possible values:
      ///    GL_TEXTURE_{1,2,3}D,
      ///    GL_TEXTURE_{1,2}D_ARRAY,
      ///    GL_TEXTURE_RECTANGLE,
      ///    GL_TEXTURE_CUBE_MAP{,_ARRAY},
      ///    GL_TEXTURE_BUFFER,
      ///    GL_TEXTURE_2D_MULTISAMPLE{,_ARRAY})
      /// \param Args None, one or multiple initializers for the texture and its mipmap levels
      /// \see png_texture_init
      /// \see empty_texture_init
      /// \see ct_texture_init
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
          /// \brief default constructor for a texture. Create a new openGL Texture object and intialize it
          /// accordingly to the Args... class template parameter
          /// \param _spl_idx is the sampler index for the texture (used when binding the texture).
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

          /// \brief Create a wrapper linked to an openGL texture ID, allowing the GL texture object to be managed for a short time
          /// by this class. When the created instance has ended its life, the GL object won't be destructed
          /// \param _id the openGL texture object ID
          /// \param _spl_idx is the sampler index for the texture (used when binding the texture).
          texture(GLuint _id, size_t _spl_idx)
            : ownership(false), id(_id), spl_idx(_spl_idx)
          {
            __tpl_init();
          }

          /// \brief Create a wrapper of an openGL texture ID, allowing the GL texture object to be managed
          /// by this class. When the created instance has ended its life, the GL object will be destructed
          /// \param _id the openGL texture object ID
          /// \param _spl_idx is the sampler index for the texture (used when binding the texture).
          texture(GLuint _id, size_t _spl_idx, assume_ownership_t)
            : ownership(true), id(_id), spl_idx(_spl_idx)
          {
            __tpl_init();
          }

          /// \brief create a wrapper of the same GL texture object in another texture instance, allowing the GL texture object to be managed for a short time
          /// by this class. When the created instance has ended its life, the GL object won't be destructed
          /// \param o the other texture class to copy the information from
          texture(const texture &o)
            : ownership(false), id(o.get_id()), spl_idx(o.get_texture_sampler()), image_texture(o.image_texture),
              image_format(o.image_format), image_access_mode(o.image_access_mode), handle(o.handle)
          {
            __tpl_init();
          }

          /// \brief create a wrapper of the same GL texture object in another texture instance, allowing the GL texture object to be managed for a short time
          /// by this class. When the created instance has ended its life, the GL object won't be destructed
          /// \param o the other texture class to copy the information from
          template<typename... TArgs>
          texture(const texture< type::opengl, TextureType, TArgs... > &o)
            : ownership(false), id(o.get_id()), spl_idx(o.get_texture_sampler()), image_texture(o.is_image_texture()),
              image_format(o.get_image_format()), image_access_mode(o.get_image_access_mode()), handle(o.get_image_handle())
          {
            __tpl_init();
          }

          /// \brief create a wrapper of the same GL texture object in another texture instance, allowing the GL texture object to be managed
          /// by this class. When the created instance has ended its life, the GL object will be destructed (ONLY IF THE \p o HAS THE OWNERSHIP)
          /// \param o the other texture class to copy the information from
          texture(texture &o, stole_ownership_t)
            : ownership(o.has_ownership()), id(o.get_id()), spl_idx(o.get_texture_sampler()), image_texture(o.image_texture),
              image_format(o.image_format), image_access_mode(o.image_access_mode), handle(o.handle)
          {
            o.give_up_ownership();
            __tpl_init();
          }

          /// \brief create a wrapper of the same GL texture object in another texture instance, allowing the GL texture object to be managed
          /// by this class. When the created instance has ended its life, the GL object will be destructed (ONLY IF THE \p o HAS THE OWNERSHIP)
          /// \param o the other texture class to copy the information from
          template<typename... TArgs>
          texture(texture< type::opengl, TextureType, TArgs... > &o, stole_ownership_t)
            : ownership(o.has_ownership()), id(o.get_id()), spl_idx(o.get_texture_sampler()), image_texture(o.is_image_texture()),
              image_format(o.get_image_format()), image_access_mode(o.get_image_access_mode()), handle(o.get_image_handle())
          {
            o.give_up_ownership();
            __tpl_init();
          }

          /// \brief create a wrapper of the same GL texture object in another texture instance, allowing the GL texture object to be managed
          /// by this class. When the created instance has ended its life, the GL object will be destructed (ONLY IF THE \p o HAS THE OWNERSHIP)
          /// \param o the other texture class to copy the information from
          texture(texture &&o)
            : ownership(o.has_ownership()), id(o.get_id()), spl_idx(o.get_texture_sampler()), image_texture(o.image_texture),
              image_format(o.image_format), image_access_mode(o.image_access_mode), handle(o.handle)
          {
            o.give_up_ownership();
            __tpl_init();
          }

          /// \brief create a wrapper of the same GL texture object in another texture instance, allowing the GL texture object to be managed
          /// by this class. When the created instance has ended its life, the GL object will be destructed (ONLY IF THE \p o HAS THE OWNERSHIP)
          /// \param o the other texture class to copy the information from
          template<typename... TArgs>
          texture(texture< type::opengl, TextureType, TArgs... > &&o)
            : ownership(o.has_ownership()), id(o.get_id()), spl_idx(o.get_texture_sampler()), image_texture(o.is_image_texture()),
              image_format(o.get_image_format()), image_access_mode(o.get_image_access_mode()), handle(o.get_image_handle())
          {
            o.give_up_ownership();
            __tpl_init();
          }

          /// \brief destructor
          ~texture()
          {
            if (ownership)
              glDeleteTextures(1, &id);
          }

          /// \brief give up the texture GL object ownership WITHOUT DELETING IT
          /// (When the texture<> instance has ended its life, the GL object won't be destructed)
          texture &give_up_ownership()
          {
            ownership = false;
            return *this;
          }

          /// \brief assume the texture GL object ownership
          /// (When the texture<> instance has ended its life, the GL object WILL be destructed)
          texture &assume_ownership()
          {
            ownership = true;
            return *this;
          }

          /// \brief create a wrapper of the same GL texture object in another texture instance, allowing the GL texture object to be managed
          /// by this class. When the created instance has ended its life, the GL object will be destructed (ONLY IF THE \p o HAS THE OWNERSHIP)
          /// \param o the other texture class to copy the information from
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

          /// \brief create a wrapper of the same GL texture object in another texture instance, allowing the GL texture object to be managed
          /// by this class. When the created instance has ended its life, the GL object will be destructed (ONLY IF THE \p o HAS THE OWNERSHIP)
          /// \param o the other texture class to copy the information from
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

          /// \brief return the GL object id for the texture
          GLuint get_id() const
          {
            return id;
          }

          /// \brief return true if the instance has the ownership of the GL object, false otherwise
          bool has_ownership() const
          {
            return ownership;
          }

          /// \brief set the texture sampler index
          void set_texture_sampler(size_t _spl_idx)
          {
            spl_idx = _spl_idx;
          }

          /// \brief the return value is used to bind samplers in shaders
          size_t get_texture_sampler() const
          {
            return spl_idx;
          }

          /// \brief return the same thing than get_texture_sampler, to be used directly in uniform var affectation
          /// FIXME is that OK (does this mean what it's intended to mean ?)
          /// \warning Should not be used. Please instead use get_texture_sampler()
          operator GLint() const
          {
            return spl_idx;
          }

          /// \brief make bindless the current GL texture
          /// \warning HACK # TODO: change this. (and make a ct init for this.)
          /// \note you should call make_resident() after
          void set_image_texture(bool _image_texture, GLenum format, GLenum access_mode)
          {
            image_texture = _image_texture;
            image_format = format;
            image_access_mode = access_mode;
          }

          /// \brief return the image handle (the bindless texture handle)
          /// \note you must call set_image_texture() before this one
          uint64_t get_image_handle()
          {
            if (image_texture)
            {
              handle = glGetImageHandleARB(id, 0, GL_TRUE, 0, image_format);
              return handle;
            }
            return 0;
          }

          /// \brief return the image handle (the bindless texture handle)
          /// \note you must call set_image_texture() before this one
          uint64_t get_image_handle() const
          {
            if (image_texture)
              return handle;
            return 0;
          }

          /// \brief Check if the current texture is a bindless one
          bool is_image_texture() const
          {
            return image_texture;
          }

          /// \brief Return the image texture format.
          /// \note Only use this when is_image_texture() is true
          /// \see get_texture_type()
          GLenum get_image_format() const
          {
            return image_format;
          }

          /// \brief Return the image texture access mode.
          /// \note Only use this when is_image_texture() is true
          GLenum get_image_access_mode() const
          {
            return image_access_mode;
          }

          /// \brief see glMakeImageHandleResidentARB and glMakeImageHandleNonResidentARB
          void make_resident(bool resident = true)
          {
            if (resident)
              glMakeImageHandleResidentARB(handle, image_access_mode);
            else
              glMakeImageHandleNonResidentARB(handle);
          }

          /// \brief use the texture (bind it to the T.U.)
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

          /// \brief undo a previous use() operation
          void unuse() const
          {
            if (!image_texture)
              glBindTexture(TextureType::value, 0);
            else
              glBindImageTexture(spl_idx, 0, 0, GL_TRUE, 0, image_access_mode, image_format);
          }

          /// \brief same thing as use() but do not bind it to a TU
          void bind() const
          {
            glBindTexture(TextureType::value, id);
          }

          /// \brief undo a previous bind() operation
          void unbind() const
          {
            glBindTexture(TextureType::value, 0);
          }

          /// \brief return the texture type
          static constexpr GLenum get_texture_type()
          {
            return TextureType::value;
          }

          /// \brief generate mipmaps for the current texture
          /// \note after this operation the texture is still bound
          void generate_mipmaps()
          {
            bind();
            glGenerateMipmap(TextureType::value);
          }

          /// \brief set the texture data from a initializer source
          /// \param it the initializer source that will be used to set the texture data
          /// \see png_texture_init
          /// \see empty_texture_init
          /// \see ct_texture_init
          template<typename InitType>
          auto set_texture_data(const InitType &it)
          -> NCR_ENABLE_IF(TextureType::value != GL_TEXTURE_RECTANGLE && TextureType::value != GL_TEXTURE_CUBE_MAP, void)
          {
            set_texture_data(it.internal_format, it.size, it.format, it.type, it.data, it.level);
          }

          /// \brief set the texture data from a initializer source (for cubemaps)
          /// \param face is the cubemap face to set
          /// \param it the initializer source that will be used to set the texture data
          /// \see png_texture_init
          /// \see empty_texture_init
          /// \see ct_texture_init
          template<typename InitType>
          auto set_texture_data(cubemap_face face, const InitType &it)
          -> NCR_ENABLE_IF(TextureType::value == GL_TEXTURE_CUBE_MAP, void)
          {
            set_texture_data(face, it.internal_format, it.size, it.format, it.type, it.data, it.level);
          }

          /// \brief set a region of the texture data from a initializer source
          /// \param offset the offset of the data to set in the texture
          /// \param it the initializer source that will be used to set the texture data
          /// \see png_texture_init
          /// \see empty_texture_init
          /// \see ct_texture_init
          template<typename InitType>
          auto set_sub_texture_data(const decltype(InitType::size) &offset, const InitType &it)
          -> NCR_ENABLE_IF(TextureType::value != GL_TEXTURE_RECTANGLE && TextureType::value != GL_TEXTURE_CUBE_MAP, void)
          {
            set_sub_texture_data(offset, it.size, it.format, it.type, it.data, it.level);
          }

          /// \brief set a region of the texture data from a initializer source
          /// \param face is the cubemap face to set
          /// \param offset the offset of the data to set in the texture
          /// \param it the initializer source that will be used to set the texture data
          /// \see png_texture_init
          /// \see empty_texture_init
          /// \see ct_texture_init
          template<typename InitType>
          auto set_sub_texture_data(cubemap_face face, const decltype(InitType::size) &offset, const InitType &it)
          -> NCR_ENABLE_IF(TextureType::value == GL_TEXTURE_CUBE_MAP, void)
          {
            set_sub_texture_data(face, offset, it.size, it.format, it.type, it.data, it.level);
          }


          /// \brief set 1D texture data
          ///
          /// \param format and \param type is the couple that define 'data' format and type
          /// \param internal_format define the way data will be stored by openGL
          /// \param size is the x size of the image data. (ATTENTION: not in fixed pos, but as integers)
          ///
          /// \link https://www.opengl.org/sdk/docs/man/xhtml/glTexImage1D.xml
          auto set_texture_data(GLint internal_format, ct::fixed_t size, GLenum format, GLenum type, GLvoid *data, GLint level = 0)
          -> NCR_ENABLE_IF((TextureType::value == GL_TEXTURE_1D), void)
          {
#ifdef YAGGLER_USE_DSA_IF_AVAILABLE
#define Y_DSA_COND  (!!glTextureImage1DEXT)
#else
#define Y_DSA_COND  true
#endif
            if (::opengl_version::useDSA && Y_DSA_COND) // NOTE: as useDSA is constexpr, the dead-code elimination pass will remove that branching
            {
              glTextureImage1DEXT(id, TextureType::value, level, internal_format, size, 0, format, type, data);
            }
            else
            {
              bind();
              glTexImage1D(TextureType::value, level, internal_format, size, 0, format, type, data);
            }
#undef Y_DSA_COND
          }

          /// set 2D texture data
          ///
          /// \param format and \param type is the couple that define 'data' format and type
          /// \param internal_format define the way data will be stored by openGL
          /// \param size is the x/y size of the image data. (ATTENTION: not in fixed pos, but as integers)
          ///
          /// \link https://www.opengl.org/sdk/docs/man/xhtml/glTexImage2D.xml
          auto set_texture_data(GLint internal_format, const ct::vector2 &size, GLenum format, GLenum type, GLvoid *data, GLint level = 0)
          -> NCR_ENABLE_IF((TextureType::value == GL_TEXTURE_2D || TextureType::value == GL_TEXTURE_1D_ARRAY), void)
          {
#ifdef YAGGLER_USE_DSA_IF_AVAILABLE
#define Y_DSA_COND  (!!glTextureImage2DEXT)
#else
#define Y_DSA_COND  true
#endif
            if (::opengl_version::useDSA && Y_DSA_COND) // NOTE: as useDSA is constexpr, the dead-code elimination pass will remove that branching
            {
              glTextureImage2DEXT(id, TextureType::value, level, internal_format, size.x, size.y, 0, format, type, data);
            }
            else
            {
              bind();
              glTexImage2D(TextureType::value, level, internal_format, size.x, size.y, 0, format, type, data);
            }
#undef Y_DSA_COND
          }

          /// set 2D texture data (rectangle textures)
          ///
          /// \param format and \param type is the couple that define 'data' format and type
          /// \param internal_format define the way data will be stored by openGL
          /// \param size is the x/y size of the image data. (ATTENTION: not in fixed pos, but as integers)
          ///
          /// \link https://www.opengl.org/sdk/docs/man/xhtml/glTexImage2D.xml
          /// \note for rectangle texture, as the specs says that level MUST be 0.
          auto set_texture_data(GLint internal_format, const ct::vector2 &size, GLenum format, GLenum type, GLvoid *data)
          -> NCR_ENABLE_IF(TextureType::value == GL_TEXTURE_RECTANGLE, void)
          {
#ifdef YAGGLER_USE_DSA_IF_AVAILABLE
#define Y_DSA_COND  (!!glTextureImage2DEXT)
#else
#define Y_DSA_COND  true
#endif
            if (::opengl_version::useDSA && Y_DSA_COND) // NOTE: as useDSA is constexpr, the dead-code elimination pass will remove that branching
            {
              glTextureImage2DEXT(id, TextureType::value, 0, internal_format, size.x, size.y, 0, format, type, data);
            }
            else
            {
              bind();
              glTexImage2D(TextureType::value, 0, internal_format, size.x, size.y, 0, format, type, data);
            }
#undef Y_DSA_COND
          }

          /// set 2D texture data (cubemaps)
          ///
          /// \param format and \param type is the couple that define 'data' format and type
          /// \param internal_format define the way data will be stored by openGL
          /// \param size is the x/y size of the image data. (ATTENTION: not in fixed pos, but as integers)
          ///
          /// \link https://www.opengl.org/sdk/docs/man/xhtml/glTexImage2D.xml
          /// \note the same as the GL_TEXTURE_2D one, but you also had to specify the cubemap face
          auto set_texture_data(cubemap_face face, GLint internal_format, const ct::vector2 &size, GLenum format, GLenum type, GLvoid *data, GLint level = 0)
          -> NCR_ENABLE_IF(TextureType::value == GL_TEXTURE_CUBE_MAP, void)
          {
#ifdef YAGGLER_USE_DSA_IF_AVAILABLE
#define Y_DSA_COND  (!!glTextureImage2DEXT)
#else
#define Y_DSA_COND  true
#endif
            if (::opengl_version::useDSA && Y_DSA_COND) // NOTE: as useDSA is constexpr, the dead-code elimination pass will remove that branching
            {
              glTextureImage2DEXT(id, static_cast<GLenum>(face), level, internal_format, size.x, size.y, 0, format, type, data);
            }
            else
            {
              bind();
              glTexImage2D(static_cast<GLenum>(face), level, internal_format, size.x, size.y, 0, format, type, data);
            }
#undef Y_DSA_COND
          }

          /// \brief set 3D texture data
          ///
          /// \param format and \param type is the couple that define 'data' format and type
          /// \param internal_format define the way data will be stored by openGL
          /// \param size is the x/y/z size of the image data. (ATTENTION: not in fixed pos, but as integers)
          ///
          /// \link https://www.opengl.org/sdk/docs/man/xhtml/glTexImage3D.xml
          auto set_texture_data(GLint internal_format, const ct::vector3 &size, GLenum format, GLenum type, GLvoid *data, GLint level = 0)
          -> NCR_ENABLE_IF((TextureType::value == GL_TEXTURE_3D || TextureType::value == GL_TEXTURE_2D_ARRAY), void)
          {
#ifdef YAGGLER_USE_DSA_IF_AVAILABLE
#define Y_DSA_COND  (!!glTextureImage3DEXT)
#else
#define Y_DSA_COND  true
#endif
            if (::opengl_version::useDSA && Y_DSA_COND) // NOTE: as useDSA is constexpr, the dead-code elimination pass will remove that branching
            {
              glTextureImage3DEXT(id, TextureType::value, level, internal_format, size.x, size.y, size.z, 0, format, type, data);
            }
            else
            {
              bind();
              glTexImage3D(TextureType::value, level, internal_format, size.x, size.y, size.z, 0, format, type, data);
            }
#undef Y_DSA_COND
          }

          /// \brief set 1D texture data of a particular region of the texture
          ///
          /// \param format and \param type is the couple that define 'data' format and type
          /// \param internal_format define the way data will be stored by openGL
          /// \param size is the x size of the image data. (ATTENTION: not in fixed pos, but as integers)
          ///
          /// \link https://www.opengl.org/sdk/docs/man/xhtml/glTexSubImage1D.xml
          auto set_sub_texture_data(ct::fixed_t offset, ct::fixed_t size, GLenum format, GLenum type, GLvoid *data, GLint level = 0)
          -> NCR_ENABLE_IF((TextureType::value == GL_TEXTURE_1D), void)
          {
#ifdef YAGGLER_USE_DSA_IF_AVAILABLE
#define Y_DSA_COND  (!!glTextureSubImage1DEXT)
#else
#define Y_DSA_COND  true
#endif
            if (::opengl_version::useDSA && Y_DSA_COND) // NOTE: as useDSA is constexpr, the dead-code elimination pass will remove that branching
            {
              glTextureSubImage1DEXT(id, TextureType::value, level, offset, size, format, type, data);
            }
            else
            {
              bind();
              glTexSubImage1D(TextureType::value, level, offset, size, format, type, data);
            }
#undef Y_DSA_COND
          }

          /// set 2D texture data of a particular region of the texture
          ///
          /// \param format and \param type is the couple that define 'data' format and type
          /// \param internal_format define the way data will be stored by openGL
          /// \param size is the x/y size of the image data. (ATTENTION: not in fixed pos, but as integers)
          ///
          /// \link https://www.opengl.org/sdk/docs/man/xhtml/glTexSubImage2D.xml
          auto set_sub_texture_data(const ct::vector2 &offset, const ct::vector2 &size, GLenum format, GLenum type, GLvoid *data, GLint level = 0)
          -> NCR_ENABLE_IF((TextureType::value == GL_TEXTURE_2D || TextureType::value == GL_TEXTURE_1D_ARRAY), void)
          {
#ifdef YAGGLER_USE_DSA_IF_AVAILABLE
#define Y_DSA_COND  (!!glTextureSubImage2DEXT)
#else
#define Y_DSA_COND  true
#endif
            if (::opengl_version::useDSA && Y_DSA_COND) // NOTE: as useDSA is constexpr, the dead-code elimination pass will remove that branching
            {
              glTextureSubImage2DEXT(id, TextureType::value, level, offset.x, offset.y, size.x, size.y, format, type, data);
            }
            else
            {
              bind();
              glTexSubImage2D(TextureType::value, level, offset.x, offset.y, size.x, size.y, format, type, data);
            }
#undef Y_DSA_COND
          }

          /// set 2D texture data of a particular region of the texture (rectangle)
          ///
          /// \param format and \param type is the couple that define 'data' format and type
          /// \param internal_format define the way data will be stored by openGL
          /// \param size is the x/y size of the image data. (ATTENTION: not in fixed pos, but as integers)
          ///
          /// \link https://www.opengl.org/sdk/docs/man/xhtml/glTexSubImage2D.xml
          /// \note for rectangle, as the specs says that level MUST be 0.
          auto set_sub_texture_data(const ct::vector2 &offset, const ct::vector2 &size, GLenum format, GLenum type, GLvoid *data)
          -> NCR_ENABLE_IF(TextureType::value == GL_TEXTURE_RECTANGLE, void)
          {
#ifdef YAGGLER_USE_DSA_IF_AVAILABLE
#define Y_DSA_COND  (!!glTextureSubImage2DEXT)
#else
#define Y_DSA_COND  true
#endif
            if (::opengl_version::useDSA && Y_DSA_COND) // NOTE: as useDSA is constexpr, the dead-code elimination pass will remove that branching
            {
              glTextureSubImage2DEXT(id, TextureType::value, 0, offset.x, offset.y, size.x, size.y, format, type, data);
            }
            else
            {
              bind();
              glTexSubImage2D(TextureType::value, 0, offset.x, offset.y, size.x, size.y, format, type, data);
            }
#undef Y_DSA_COND
          }

          /// set 2D texture data of a particular region of the texture (cubemap)
          ///
          /// \param format and \param type is the couple that define 'data' format and type
          /// \param internal_format define the way data will be stored by openGL
          /// \param size is the x/y size of the image data. (ATTENTION: not in fixed pos, but as integers)
          ///
          /// \link https://www.opengl.org/sdk/docs/man/xhtml/glTexSubImage2D.xml
          /// \note for rectangle, as the specs says that level MUST be 0.
          /// \note the same as the GL_TEXTURE_2D one, but you also had to specify the cubemap face
          auto set_sub_texture_data(cubemap_face face, const ct::vector2 &offset, const ct::vector2 &size, GLenum format, GLenum type, GLvoid *data, GLint level = 0)
          -> NCR_ENABLE_IF(TextureType::value == GL_TEXTURE_CUBE_MAP, void)
          {
#ifdef YAGGLER_USE_DSA_IF_AVAILABLE
#define Y_DSA_COND  (!!glTextureSubImage2DEXT)
#else
#define Y_DSA_COND  true
#endif
            if (::opengl_version::useDSA && Y_DSA_COND) // NOTE: as useDSA is constexpr, the dead-code elimination pass will remove that branching
            {
              glTextureSubImage2DEXT(id, static_cast<GLenum>(face), level, offset.x, offset.y, size.x, size.y, format, type, data);
            }
            else
            {
              bind();
              glTexSubImage2D(static_cast<GLenum>(face), level, offset.x, offset.y, size.x, size.y, format, type, data);
            }
#undef Y_DSA_COND
          }

          /// \brief set 3D texture data
          ///
          /// \param format and \param type is the couple that define 'data' format and type
          /// \param internal_format define the way data will be stored by openGL
          /// \param size is the x/y/z size of the image data. (ATTENTION: not in fixed pos, but as integers)
          ///
          /// \link https://www.opengl.org/sdk/docs/man/xhtml/glTexSubImage3D.xml
          auto set_sub_texture_data(const ct::vector3 &offset, const ct::vector3 &size, GLenum format, GLenum type, GLvoid *data, GLint level = 0)
          -> NCR_ENABLE_IF((TextureType::value == GL_TEXTURE_3D || TextureType::value == GL_TEXTURE_2D_ARRAY), void)
          {
#ifdef YAGGLER_USE_DSA_IF_AVAILABLE
#define Y_DSA_COND  (!!glTextureSubImage3DEXT)
#else
#define Y_DSA_COND  true
#endif
            if (::opengl_version::useDSA && Y_DSA_COND) // NOTE: as useDSA is constexpr, the dead-code elimination pass will remove that branching
            {
              glTextureSubImage3DEXT(id, TextureType::value, level, offset.x, offset.y, offset.z, size.x, size.y, size.z, format, type, data);
            }
            else
            {
              bind();
              glTexSubImage3D(TextureType::value, level, offset.x, offset.y, offset.z, size.x, size.y, size.z, format, type, data);
            }
#undef Y_DSA_COND
          }

          /// \brief set tex parameters (see openGL doc for pname / param @ https://www.opengl.org/sdk/docs/man/xhtml/glTexParameter.xml)
          void set_gl_parameter(GLenum pname, float param)
          {
#ifdef YAGGLER_USE_DSA_IF_AVAILABLE
#define Y_DSA_COND  (!!glTextureParameterfEXT)
#else
#define Y_DSA_COND  true
#endif
            if (::opengl_version::useDSA && Y_DSA_COND) // NOTE: as useDSA is constexpr, the dead-code elimination pass will remove that branching
            {
              glTextureParameterfEXT(id, TextureType::value, pname, param);
            }
            else
            {
              bind();
              glTexParameterf(TextureType::value, pname, param);
            }
#undef Y_DSA_COND
          }

          /// \brief set tex parameters (see openGL doc for pname / param @ https://www.opengl.org/sdk/docs/man/xhtml/glTexParameter.xml)
          void set_gl_parameter(GLenum pname, GLint param)
          {
#ifdef YAGGLER_USE_DSA_IF_AVAILABLE
#define Y_DSA_COND  (!!glTextureParameteriEXT)
#else
#define Y_DSA_COND  true
#endif
            if (::opengl_version::useDSA && Y_DSA_COND) // NOTE: as useDSA is constexpr, the dead-code elimination pass will remove that branching
            {
              glTextureParameteriEXT(id, TextureType::value, pname, param);
            }
            else
            {
              bind();
              glTexParameteri(TextureType::value, pname, param);
            }
#undef Y_DSA_COND
          }

          /// \brief set tex parameters (see openGL doc for pname / param @ https://www.opengl.org/sdk/docs/man/xhtml/glTexParameter.xml)
          void set_gl_parameter(GLenum pname, float *params)
          {
#ifdef YAGGLER_USE_DSA_IF_AVAILABLE
#define Y_DSA_COND  (!!glTextureParameterfvEXT)
#else
#define Y_DSA_COND  true
#endif
            if (::opengl_version::useDSA && Y_DSA_COND) // NOTE: as useDSA is constexpr, the dead-code elimination pass will remove that branching
            {
              glTextureParameterfvEXT(id, TextureType::value, pname, params);
            }
            else
            {
              bind();
              glTexParameterfv(TextureType::value, pname, params);
            }
#undef Y_DSA_COND
          }

          /// \brief set tex parameters (see openGL doc for pname / param @ https://www.opengl.org/sdk/docs/man/xhtml/glTexParameter.xml)
          void set_gl_parameter(GLenum pname, GLint *params)
          {
#ifdef YAGGLER_USE_DSA_IF_AVAILABLE
#define Y_DSA_COND  (!!glTextureParameterivEXT)
#else
#define Y_DSA_COND  true
#endif
            if (::opengl_version::useDSA && Y_DSA_COND) // NOTE: as useDSA is constexpr, the dead-code elimination pass will remove that branching
            {
              glTextureParameterivEXT(id, TextureType::value, pname, params);
            }
            else
            {
              bind();
              glTexParameteriv(TextureType::value, pname, params);
            }
#undef Y_DSA_COND
          }

          /// \brief set tex parameters (see openGL doc for pname / param @ https://www.opengl.org/sdk/docs/man/xhtml/glTexParameter.xml)
          void set_gl_parameterI(GLenum pname, GLint *params)
          {
#ifdef YAGGLER_USE_DSA_IF_AVAILABLE
#define Y_DSA_COND  (!!glTextureParameterIivEXT)
#else
#define Y_DSA_COND  true
#endif
            if (::opengl_version::useDSA && Y_DSA_COND) // NOTE: as useDSA is constexpr, the dead-code elimination pass will remove that branching
            {
              glTextureParameterIivEXT(id, TextureType::value, pname, params);
            }
            else
            {
            bind();
            glTexParameterIiv(TextureType::value, pname, params);
            }
#undef Y_DSA_COND
          }

          /// \brief set tex parameters (see openGL doc for pname / param @ https://www.opengl.org/sdk/docs/man/xhtml/glTexParameter.xml)
          void set_gl_parameterI(GLenum pname, GLuint *params)
          {
#ifdef YAGGLER_USE_DSA_IF_AVAILABLE
#define Y_DSA_COND  (!!glTextureParameterIuivEXT)
#else
#define Y_DSA_COND  true
#endif
            if (::opengl_version::useDSA && Y_DSA_COND) // NOTE: as useDSA is constexpr, the dead-code elimination pass will remove that branching
            {
              glTextureParameterIuivEXT(id, TextureType::value, pname, params);
            }
            else
            {
              bind();
              glTexParameterIuiv(TextureType::value, pname, params);
            }
#undef Y_DSA_COND
          }

          /// \brief create a link to a more generic texture. (removing from the type any extra Args template parameter)
          /// no inheritance involved. This cast will create a \e link to the GL texture object.
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

