//
// file : opengl_object.hpp
// in : file:///home/tim/projects/yaggler/yaggler/geometry/opengl_buffer.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 18/01/2014 12:18:58
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


#ifndef __N_19241511841048067712_64997576__OPENGL_BUFFER_HPP__
# define __N_19241511841048067712_64997576__OPENGL_BUFFER_HPP__

#include <yaggler_type.hpp>
#include <GLEW/glew.h>

#include <geometry/buffer_base.hpp>
#include <geometry/options.hpp>

namespace neam
{
  namespace yaggler
  {
    namespace geometry
    {
      // GeomType is an GLenum embed.
      template<typename GeomType, typename... Args>
      class buffer<type::opengl, GeomType, Args...>
      {
        public:
          // constructors
          buffer()
          : id(0), link(false)
          {
            glGenBuffers(1, &id);
          }

          explicit buffer(GLuint _id)
            : id(_id), link(true)
          {
          }

          buffer(GLuint _id, assume_ownership_t)
            : id(_id), link(false)
          {
          }

          // destructor
          ~buffer()
          {
            if (!link)
              glDeleteBuffers(1, &id);
          }

          // getters
          GLuint get_id() const
          {
            return id;
          }

          bool is_link() const
          {
            return link;
          }

          // bind the buffer
          void bind() const
          {
            glBindBuffer(GeomType::value, id);
          }

        private:
          GLuint id;
          bool link;
      };
    } // namespace geometry
  } // namespace yaggler
} // namespace neam

#endif /*__N_19241511841048067712_64997576__OPENGL_BUFFER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

