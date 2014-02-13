//
// file : opengl_uniform_var.hpp
// in : file:///home/tim/projects/yaggler/yaggler/shader/opengl_uniform_var.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 26/10/2013 16:45:39
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


#ifndef __N_4228199451929725216_1925374995__OPENGL_UNIFORM_VAR_HPP__
# define __N_4228199451929725216_1925374995__OPENGL_UNIFORM_VAR_HPP__

#include <GLEW/glew.h>
#include <glm/glm.hpp>

#include <vector>
#include <array>

#include <tools/array_wrapper.hpp>
#include <tools/ref.hpp>
#include <ct_point.hpp>

namespace neam
{
  namespace yaggler
  {
    namespace shader
    {
#define CHECK_ID        if (id == -1)return *this
      class uniform_variable
      {
        public:
          uniform_variable(GLint _id = -1) : id(_id) {}
          ~uniform_variable() {}

          // this probably wont do what you want... so I delete it.
          uniform_variable &operator = (const uniform_variable &o) = delete;

          // return the id of the var
          GLint get_id() const
          {
            return id;
          }

          // could be dangerous.
          void _set_id(GLint _id)
          {
            id = _id;
          }

          bool is_valid() const
          {
            return id != -1;
          }

          /// switch for refs (avoid a g++ error) ///
          template<typename Etype>
          inline uniform_variable &operator = (const neam::cr::ref<Etype> &t)
          {
            *this = t.value;
            return *this;
          }

          /// FLOAT 1V ///

          // set the value
          inline uniform_variable &operator = (double value)
          {
            CHECK_ID;
            glUniform1f(id, value);
            return *this;
          }
          inline uniform_variable &operator = (float value)
          {
            CHECK_ID;
            glUniform1f(id, value);
            return *this;
          }

          // for fixed size arrays
          template<size_t Size>
          inline uniform_variable &operator = (const float (&value)[Size])
          {
            CHECK_ID;
            glUniform1fv(id, Size, value);
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const std::array<float, Size> &value)
          {
            CHECK_ID;
            glUniform1fv(id, Size, value.data());
            return *this;
          }

          // for vectors
          inline uniform_variable &operator = (const std::vector<float> &value)
          {
            CHECK_ID;
            glUniform1fv(id, value.size(), value.data());
            return *this;
          }

          // for array wrappers
          inline uniform_variable &operator = (const array_wrapper<float> &value)
          {
            CHECK_ID;
            glUniform1fv(id, value.size, value.array);
            return *this;
          }

          /// INT 1V ///

          // set the value
          inline uniform_variable &operator = (GLint value)
          {
            CHECK_ID;
            glUniform1i(id, value);
            return *this;
          }
          inline uniform_variable &operator = (long value)
          {
            CHECK_ID;
            glUniform1i(id, value);
            return *this;
          }
          inline uniform_variable &operator = (unsigned long value)
          {
            CHECK_ID;
            glUniform1ui(id, value);
            return *this;
          }

          // for fixed size arrays
          template<size_t Size>
          inline uniform_variable &operator = (const GLint (&value)[Size])
          {
            CHECK_ID;
            glUniform1iv(id, Size, value);
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const std::array<GLint, Size> &value)
          {
            CHECK_ID;
            glUniform1iv(id, Size, value.data());
            return *this;
          }

          // for vectors
          inline uniform_variable &operator = (const std::vector<GLint> &value)
          {
            CHECK_ID;
            glUniform1iv(id, value.size(), value.data());
            return *this;
          }

          // for array wrappers
          inline uniform_variable &operator = (const array_wrapper<GLint> &value)
          {
            CHECK_ID;
            glUniform1iv(id, value.size, value.array);
            return *this;
          }

          /// UNSIGNED INT 1V ///

          // set the value
          inline uniform_variable &operator = (GLuint value)
          {
            CHECK_ID;
            glUniform1ui(id, value);
            return *this;
          }

          // for fixed size arrays
          template<size_t Size>
          inline uniform_variable &operator = (const GLuint (&value)[Size])
          {
            CHECK_ID;
            glUniform1uiv(id, Size, value);
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const std::array<GLuint, Size> &value)
          {
            CHECK_ID;
            glUniform1uiv(id, Size, value.data());
            return *this;
          }

          // for vectors
          inline uniform_variable &operator = (const std::vector<GLuint> &value)
          {
            CHECK_ID;
            glUniform1uiv(id, value.size(), value.data());
            return *this;
          }

          // for array wrappers
          inline uniform_variable &operator = (const array_wrapper<GLuint> &value)
          {
            CHECK_ID;
            glUniform1uiv(id, value.size, value.array);
            return *this;
          }

          /// FLOAT 2V ///

          // set the value
          inline uniform_variable &operator = (const ct::vector2 &value)
          {
            CHECK_ID;
            glUniform2f(id, neam::ct::conversion::to<float>(value.x), neam::ct::conversion::to<float>(value.y));
            return *this;
          }
          inline uniform_variable &operator = (const glm::vec2 &value)
          {
            CHECK_ID;
            glUniform2f(id, value.x, value.y);
            return *this;
          }

          // for fixed size arrays
          template<size_t Size>
          inline uniform_variable &operator = (const float (&value)[Size][2])
          {
            CHECK_ID;
            glUniform2fv(id, Size, value);
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const ct::vector2 (&value)[Size])
          {
            CHECK_ID;
            GLfloat *tmp_value = new GLfloat[Size * 2];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 2 + 0] = neam::ct::conversion::to<float>(value[i].x);
              tmp_value[i * 2 + 1] = neam::ct::conversion::to<float>(value[i].y);
            }

            glUniform2fv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const glm::vec2 (&value)[Size])
          {
            CHECK_ID;
            GLfloat *tmp_value = new GLfloat[Size * 2];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 2 + 0] = value[i].x;
              tmp_value[i * 2 + 1] = value[i].y;
            }

            glUniform2fv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const std::array<float[2], Size> &value)
          {
            CHECK_ID;
            glUniform2fv(id, Size, value.data());
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const std::array<ct::vector2, Size> &value)
          {
            CHECK_ID;
            GLfloat *tmp_value = new GLfloat[Size * 2];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 2 + 0] = neam::ct::conversion::to<float>(value[i].x);
              tmp_value[i * 2 + 1] = neam::ct::conversion::to<float>(value[i].y);
            }

            glUniform2fv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const std::array<glm::vec2, Size> &value)
          {
            CHECK_ID;
            GLfloat *tmp_value = new GLfloat[Size * 2];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 2 + 0] = (value[i].x);
              tmp_value[i * 2 + 1] = (value[i].y);
            }

            glUniform2fv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          // for vectors
          inline uniform_variable &operator = (const std::vector<float[2]> &value)
          {
            CHECK_ID;
            glUniform2fv(id, value.size(), reinterpret_cast<const GLfloat *>(value.data()));
            return *this;
          }
          inline uniform_variable &operator = (const std::vector<ct::vector2> &value)
          {
            CHECK_ID;

            const size_t size = value.size();
            GLfloat *tmp_value = new GLfloat[size * 2];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 2 + 0] = neam::ct::conversion::to<float>(value[i].x);
              tmp_value[i * 2 + 1] = neam::ct::conversion::to<float>(value[i].y);
            }

            glUniform2fv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &operator = (const std::vector<glm::vec2> &value)
          {
            CHECK_ID;

            const size_t size = value.size();
            GLfloat *tmp_value = new GLfloat[size * 2];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 2 + 0] = (value[i].x);
              tmp_value[i * 2 + 1] = (value[i].y);
            }

            glUniform2fv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          // for array wrappers
          inline uniform_variable &operator = (const array_wrapper<float[2]> &value)
          {
            CHECK_ID;
            glUniform2fv(id, value.size, reinterpret_cast<const GLfloat *>(value.array));
            return *this;
          }
          inline uniform_variable &operator = (const array_wrapper<ct::vector2> &value)
          {
            CHECK_ID;
            const size_t size = value.size;
            GLfloat *tmp_value = new GLfloat[size * 2];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 2 + 0] = neam::ct::conversion::to<float>(value.array[i].x);
              tmp_value[i * 2 + 1] = neam::ct::conversion::to<float>(value.array[i].y);
            }

            glUniform2fv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &operator = (const array_wrapper<glm::vec2> &value)
          {
            CHECK_ID;
            const size_t size = value.size;
            GLfloat *tmp_value = new GLfloat[size * 2];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 2 + 0] = (value.array[i].x);
              tmp_value[i * 2 + 1] = (value.array[i].y);
            }

            glUniform2fv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          /// INT 2V ///

          // set the value
          inline uniform_variable &operator = (const glm::ivec2 &value)
          {
            CHECK_ID;
            glUniform2i(id, value.x, value.y);
            return *this;
          }
          inline uniform_variable &set_as_int(const ct::vector2 &value)
          {
            CHECK_ID;
            glUniform2i(id, neam::ct::conversion::to<GLint>(value.x), neam::ct::conversion::to<GLint>(value.y));
            return *this;
          }
          inline uniform_variable &set_as_int(const glm::vec2 &value)
          {
            CHECK_ID;
            glUniform2i(id, (value.x), (value.y));
            return *this;
          }

          // for fixed size arrays
          template<size_t Size>
          inline uniform_variable &operator = (const GLint (&value)[Size][2])
          {
            CHECK_ID;
            glUniform2iv(id, Size, value);
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &set_as_int(const ct::vector2 (&value)[Size])
          {
            CHECK_ID;
            GLint *tmp_value = new GLint[Size * 2];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 2 + 0] = neam::ct::conversion::to<GLint>(value[i].x);
              tmp_value[i * 2 + 1] = neam::ct::conversion::to<GLint>(value[i].y);
            }

            glUniform2iv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &set_as_int(const glm::vec2 (&value)[Size])
          {
            CHECK_ID;
            GLint *tmp_value = new GLint[Size * 2];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 2 + 0] = (value[i].x);
              tmp_value[i * 2 + 1] = (value[i].y);
            }

            glUniform2iv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator =(const glm::ivec2 (&value)[Size])
          {
            CHECK_ID;
            GLint *tmp_value = new GLint[Size * 2];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 2 + 0] = (value[i].x);
              tmp_value[i * 2 + 1] = (value[i].y);
            }

            glUniform2iv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const std::array<GLint[2], Size> &value)
          {
            CHECK_ID;
            glUniform2iv(id, Size, value.data());
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &set_as_int(const std::array<ct::vector2, Size> &value)
          {
            CHECK_ID;
            GLint *tmp_value = new GLint[Size * 2];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 2 + 0] = neam::ct::conversion::to<GLint>(value[i].x);
              tmp_value[i * 2 + 1] = neam::ct::conversion::to<GLint>(value[i].y);
            }

            glUniform2iv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &set_as_int(const std::array<glm::vec2, Size> &value)
          {
            CHECK_ID;
            GLint *tmp_value = new GLint[Size * 2];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 2 + 0] = (value[i].x);
              tmp_value[i * 2 + 1] = (value[i].y);
            }

            glUniform2iv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const std::array<glm::ivec2, Size> &value)
          {
            CHECK_ID;
            GLint *tmp_value = new GLint[Size * 2];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 2 + 0] = (value[i].x);
              tmp_value[i * 2 + 1] = (value[i].y);
            }

            glUniform2iv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          // for vectors
          inline uniform_variable &operator = (const std::vector<GLint[2]> &value)
          {
            CHECK_ID;
            glUniform2iv(id, value.size(), reinterpret_cast<const GLint *>(value.data()));
            return *this;
          }
          inline uniform_variable &set_as_int(const std::vector<ct::vector2> &value)
          {
            CHECK_ID;

            const size_t size = value.size();
            GLint *tmp_value = new GLint[size * 2];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 2 + 0] = neam::ct::conversion::to<GLint>(value[i].x);
              tmp_value[i * 2 + 1] = neam::ct::conversion::to<GLint>(value[i].y);
            }

            glUniform2iv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &set_as_int(const std::vector<glm::vec2> &value)
          {
            CHECK_ID;

            const size_t size = value.size();
            GLint *tmp_value = new GLint[size * 2];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 2 + 0] = (value[i].x);
              tmp_value[i * 2 + 1] = (value[i].y);
            }

            glUniform2iv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &operator = (const std::vector<glm::ivec2> &value)
          {
            CHECK_ID;

            const size_t size = value.size();
            GLint *tmp_value = new GLint[size * 2];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 2 + 0] = (value[i].x);
              tmp_value[i * 2 + 1] = (value[i].y);
            }

            glUniform2iv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          // for array wrappers
          inline uniform_variable &operator = (const array_wrapper<GLint[2]> &value)
          {
            CHECK_ID;
            glUniform2iv(id, value.size, reinterpret_cast<const GLint *>(value.array));
            return *this;
          }
          inline uniform_variable &set_as_int(const array_wrapper<ct::vector2> &value)
          {
            CHECK_ID;
            const size_t size = value.size;
            GLint *tmp_value = new GLint[size * 2];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 2 + 0] = neam::ct::conversion::to<GLint>(value.array[i].x);
              tmp_value[i * 2 + 1] = neam::ct::conversion::to<GLint>(value.array[i].y);
            }

            glUniform2iv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &set_as_int(const array_wrapper<glm::vec2> &value)
          {
            CHECK_ID;
            const size_t size = value.size;
            GLint *tmp_value = new GLint[size * 2];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 2 + 0] = (value.array[i].x);
              tmp_value[i * 2 + 1] = (value.array[i].y);
            }

            glUniform2iv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &operator = (const array_wrapper<glm::ivec2> &value)
          {
            CHECK_ID;
            const size_t size = value.size;
            GLint *tmp_value = new GLint[size * 2];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 2 + 0] = (value.array[i].x);
              tmp_value[i * 2 + 1] = (value.array[i].y);
            }

            glUniform2iv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          /// UNSIGNED INT 2V ///

          // set the value
          inline uniform_variable &set_as_uint(const ct::vector2 &value)
          {
            CHECK_ID;
            glUniform2ui(id, neam::ct::conversion::to<GLuint>(value.x), neam::ct::conversion::to<GLuint>(value.y));
            return *this;
          }
          inline uniform_variable &set_as_uint(const glm::vec2 &value)
          {
            CHECK_ID;
            glUniform2ui(id, (value.x), (value.y));
            return *this;
          }
          inline uniform_variable &operator = (const glm::uvec2 &value)
          {
            CHECK_ID;
            glUniform2ui(id, (value.x), (value.y));
            return *this;
          }

          // for fixed size arrays
          template<size_t Size>
          inline uniform_variable &operator = (const GLuint (&value)[Size][2])
          {
            CHECK_ID;
            glUniform2uiv(id, Size, value);
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &set_as_uint(const ct::vector2 (&value)[Size])
          {
            CHECK_ID;
            GLuint *tmp_value = new GLuint[Size * 2];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 2 + 0] = neam::ct::conversion::to<GLuint>(value[i].x);
              tmp_value[i * 2 + 1] = neam::ct::conversion::to<GLuint>(value[i].y);
            }

            glUniform2uiv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &set_as_uint(const glm::vec2 (&value)[Size])
          {
            CHECK_ID;
            GLuint *tmp_value = new GLuint[Size * 2];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 2 + 0] = (value[i].x);
              tmp_value[i * 2 + 1] = (value[i].y);
            }

            glUniform2uiv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const glm::uvec2 (&value)[Size])
          {
            CHECK_ID;
            GLuint *tmp_value = new GLuint[Size * 2];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 2 + 0] = (value[i].x);
              tmp_value[i * 2 + 1] = (value[i].y);
            }

            glUniform2uiv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const std::array<GLuint[2], Size> &value)
          {
            CHECK_ID;
            glUniform2uiv(id, Size, value.data());
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &set_as_uint(const std::array<ct::vector2, Size> &value)
          {
            CHECK_ID;
            GLuint *tmp_value = new GLuint[Size * 2];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 2 + 0] = neam::ct::conversion::to<GLuint>(value[i].x);
              tmp_value[i * 2 + 1] = neam::ct::conversion::to<GLuint>(value[i].y);
            }

            glUniform2uiv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &set_as_uint(const std::array<glm::vec2, Size> &value)
          {
            CHECK_ID;
            GLuint *tmp_value = new GLuint[Size * 2];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 2 + 0] = (value[i].x);
              tmp_value[i * 2 + 1] = (value[i].y);
            }

            glUniform2uiv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const std::array<glm::uvec2, Size> &value)
          {
            CHECK_ID;
            GLuint *tmp_value = new GLuint[Size * 2];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 2 + 0] = (value[i].x);
              tmp_value[i * 2 + 1] = (value[i].y);
            }

            glUniform2uiv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          // for vectors
          inline uniform_variable &operator = (const std::vector<GLuint[2]> &value)
          {
            CHECK_ID;
            glUniform2uiv(id, value.size(), reinterpret_cast<const GLuint *>(value.data()));
            return *this;
          }
          inline uniform_variable &set_as_uint(const std::vector<ct::vector2> &value)
          {
            CHECK_ID;

            const size_t size = value.size();
            GLuint *tmp_value = new GLuint[size * 2];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 2 + 0] = neam::ct::conversion::to<GLuint>(value[i].x);
              tmp_value[i * 2 + 1] = neam::ct::conversion::to<GLuint>(value[i].y);
            }

            glUniform2uiv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &set_as_uint(const std::vector<glm::vec2> &value)
          {
            CHECK_ID;

            const size_t size = value.size();
            GLuint *tmp_value = new GLuint[size * 2];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 2 + 0] = (value[i].x);
              tmp_value[i * 2 + 1] = (value[i].y);
            }

            glUniform2uiv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &operator = (const std::vector<glm::uvec2> &value)
          {
            CHECK_ID;

            const size_t size = value.size();
            GLuint *tmp_value = new GLuint[size * 2];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 2 + 0] = (value[i].x);
              tmp_value[i * 2 + 1] = (value[i].y);
            }

            glUniform2uiv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          // for array wrappers
          inline uniform_variable &operator = (const array_wrapper<GLuint[2]> &value)
          {
            CHECK_ID;
            glUniform2uiv(id, value.size, reinterpret_cast<const GLuint *>(value.array));
            return *this;
          }
          inline uniform_variable &set_as_uint(const array_wrapper<ct::vector2> &value)
          {
            CHECK_ID;
            const size_t size = value.size;
            GLuint *tmp_value = new GLuint[size * 2];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 2 + 0] = neam::ct::conversion::to<GLuint>(value.array[i].x);
              tmp_value[i * 2 + 1] = neam::ct::conversion::to<GLuint>(value.array[i].y);
            }

            glUniform2uiv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &set_as_uint(const array_wrapper<glm::vec2> &value)
          {
            CHECK_ID;
            const size_t size = value.size;
            GLuint *tmp_value = new GLuint[size * 2];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 2 + 0] = (value.array[i].x);
              tmp_value[i * 2 + 1] = (value.array[i].y);
            }

            glUniform2uiv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &operator = (const array_wrapper<glm::uvec2> &value)
          {
            CHECK_ID;
            const size_t size = value.size;
            GLuint *tmp_value = new GLuint[size * 2];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 2 + 0] = (value.array[i].x);
              tmp_value[i * 2 + 1] = (value.array[i].y);
            }

            glUniform2uiv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          /// FLOAT 3V /// // (yes, I'll do this fucking copy-pasting for EVERY
          ///          /// //  FUCKING TYPES THAT OPENGL HAS. FUCK.)

          // set the value
          inline uniform_variable &operator = (const ct::vector3 &value)
          {
            CHECK_ID;
            glUniform3f(id, neam::ct::conversion::to<float>(value.x), neam::ct::conversion::to<float>(value.y), neam::ct::conversion::to<float>(value.z));
            return *this;
          }
          inline uniform_variable &operator = (const glm::vec3 &value)
          {
            CHECK_ID;
            glUniform3f(id, (value.x), (value.y), (value.z));
            return *this;
          }

          // for fixed size arrays
          template<size_t Size>
          inline uniform_variable &operator = (const float (&value)[Size][3])
          {
            CHECK_ID;
            glUniform3fv(id, Size, value);
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const ct::vector3 (&value)[Size])
          {
            CHECK_ID;
            GLfloat *tmp_value = new GLfloat[Size * 3];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 3 + 0] = neam::ct::conversion::to<float>(value[i].x);
              tmp_value[i * 3 + 1] = neam::ct::conversion::to<float>(value[i].y);
              tmp_value[i * 3 + 2] = neam::ct::conversion::to<float>(value[i].z);
            }

            glUniform3fv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const glm::vec3 (&value)[Size])
          {
            CHECK_ID;
            GLfloat *tmp_value = new GLfloat[Size * 3];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 3 + 0] = (value[i].x);
              tmp_value[i * 3 + 1] = (value[i].y);
              tmp_value[i * 3 + 2] = (value[i].z);
            }

            glUniform3fv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const std::array<float[3], Size> &value)
          {
            CHECK_ID;
            glUniform3fv(id, Size, value.data());
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const std::array<ct::vector3, Size> &value)
          {
            CHECK_ID;
            GLfloat *tmp_value = new GLfloat[Size * 3];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 3 + 0] = neam::ct::conversion::to<float>(value[i].x);
              tmp_value[i * 3 + 1] = neam::ct::conversion::to<float>(value[i].y);
              tmp_value[i * 3 + 2] = neam::ct::conversion::to<float>(value[i].z);
            }

            glUniform3fv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const std::array<glm::vec3, Size> &value)
          {
            CHECK_ID;
            GLfloat *tmp_value = new GLfloat[Size * 3];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 3 + 0] = (value[i].x);
              tmp_value[i * 3 + 1] = (value[i].y);
              tmp_value[i * 3 + 2] = (value[i].z);
            }

            glUniform3fv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          // for vectors
          inline uniform_variable &operator = (const std::vector<float[3]> &value)
          {
            CHECK_ID;
            glUniform3fv(id, value.size(), reinterpret_cast<const GLfloat *>(value.data()));
            return *this;
          }
          inline uniform_variable &operator = (const std::vector<ct::vector3> &value)
          {
            CHECK_ID;
            const size_t size = value.size();
            GLfloat *tmp_value = new GLfloat[size * 3];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 3 + 0] = neam::ct::conversion::to<float>(value[i].x);
              tmp_value[i * 3 + 1] = neam::ct::conversion::to<float>(value[i].y);
              tmp_value[i * 3 + 2] = neam::ct::conversion::to<float>(value[i].z);
            }

            glUniform3fv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &operator = (const std::vector<glm::vec3> &value)
          {
            CHECK_ID;
            const size_t size = value.size();
            GLfloat *tmp_value = new GLfloat[size * 3];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 3 + 0] = (value[i].x);
              tmp_value[i * 3 + 1] = (value[i].y);
              tmp_value[i * 3 + 2] = (value[i].z);
            }

            glUniform3fv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          // for array wrappers
          inline uniform_variable &operator = (const array_wrapper<float[3]> &value)
          {
            CHECK_ID;
            glUniform3fv(id, value.size, reinterpret_cast<const GLfloat *>(value.array));
            return *this;
          }
          inline uniform_variable &operator = (const array_wrapper<ct::vector3> &value)
          {
            CHECK_ID;

            const size_t size = value.size;
            GLfloat *tmp_value = new GLfloat[size * 3];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 3 + 0] = neam::ct::conversion::to<float>(value.array[i].x);
              tmp_value[i * 3 + 1] = neam::ct::conversion::to<float>(value.array[i].y);
              tmp_value[i * 3 + 2] = neam::ct::conversion::to<float>(value.array[i].z);
            }

            glUniform3fv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &operator = (const array_wrapper<glm::vec3> &value)
          {
            CHECK_ID;

            const size_t size = value.size;
            GLfloat *tmp_value = new GLfloat[size * 3];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 3 + 0] = (value.array[i].x);
              tmp_value[i * 3 + 1] = (value.array[i].y);
              tmp_value[i * 3 + 2] = (value.array[i].z);
            }

            glUniform3fv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          /// INT 3V ///

          // set the value
          inline uniform_variable &set_as_int(const ct::vector3 &value)
          {
            CHECK_ID;
            glUniform3i(id, neam::ct::conversion::to<GLint>(value.x), neam::ct::conversion::to<GLint>(value.y), neam::ct::conversion::to<GLint>(value.z));
            return *this;
          }
          inline uniform_variable &set_as_int(const glm::vec3 &value)
          {
            CHECK_ID;
            glUniform3i(id, (value.x), (value.y), (value.z));
            return *this;
          }
          inline uniform_variable &operator = (const glm::ivec3 &value)
          {
            CHECK_ID;
            glUniform3i(id, (value.x), (value.y), (value.z));
            return *this;
          }

          // for fixed size arrays
          template<size_t Size>
          inline uniform_variable &operator = (const GLint (&value)[Size][3])
          {
            CHECK_ID;
            glUniform3iv(id, Size, value);
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &set_as_int(const ct::vector3 (&value)[Size])
          {
            CHECK_ID;
            GLint *tmp_value = new GLint[Size * 3];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 3 + 0] = neam::ct::conversion::to<GLint>(value[i].x);
              tmp_value[i * 3 + 1] = neam::ct::conversion::to<GLint>(value[i].y);
              tmp_value[i * 3 + 2] = neam::ct::conversion::to<GLint>(value[i].z);
            }

            glUniform3iv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &set_as_int(const glm::vec3 (&value)[Size])
          {
            CHECK_ID;
            GLint *tmp_value = new GLint[Size * 3];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 3 + 0] = (value[i].x);
              tmp_value[i * 3 + 1] = (value[i].y);
              tmp_value[i * 3 + 2] = (value[i].z);
            }

            glUniform3iv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const glm::ivec3 (&value)[Size])
          {
            CHECK_ID;
            GLint *tmp_value = new GLint[Size * 3];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 3 + 0] = (value[i].x);
              tmp_value[i * 3 + 1] = (value[i].y);
              tmp_value[i * 3 + 2] = (value[i].z);
            }

            glUniform3iv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const std::array<GLint[3], Size> &value)
          {
            CHECK_ID;
            glUniform3iv(id, Size, value.data());
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &set_as_int(const std::array<ct::vector3, Size> &value)
          {
            CHECK_ID;
            GLint *tmp_value = new GLint[Size * 3];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 3 + 0] = neam::ct::conversion::to<GLint>(value[i].x);
              tmp_value[i * 3 + 1] = neam::ct::conversion::to<GLint>(value[i].y);
              tmp_value[i * 3 + 2] = neam::ct::conversion::to<GLint>(value[i].z);
            }

            glUniform3iv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &set_as_int(const std::array<glm::vec3, Size> &value)
          {
            CHECK_ID;
            GLint *tmp_value = new GLint[Size * 3];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 3 + 0] = (value[i].x);
              tmp_value[i * 3 + 1] = (value[i].y);
              tmp_value[i * 3 + 2] = (value[i].z);
            }

            glUniform3iv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const std::array<glm::ivec3, Size> &value)
          {
            CHECK_ID;
            GLint *tmp_value = new GLint[Size * 3];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 3 + 0] = (value[i].x);
              tmp_value[i * 3 + 1] = (value[i].y);
              tmp_value[i * 3 + 2] = (value[i].z);
            }

            glUniform3iv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          // for vectors
          inline uniform_variable &operator = (const std::vector<GLint[3]> &value)
          {
            CHECK_ID;
            glUniform3iv(id, value.size(), reinterpret_cast<const GLint *>(value.data()));
            return *this;
          }
          inline uniform_variable &set_as_int(const std::vector<ct::vector3> &value)
          {
            CHECK_ID;
            const size_t size = value.size();
            GLint *tmp_value = new GLint[size * 3];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 3 + 0] = neam::ct::conversion::to<GLint>(value[i].x);
              tmp_value[i * 3 + 1] = neam::ct::conversion::to<GLint>(value[i].y);
              tmp_value[i * 3 + 2] = neam::ct::conversion::to<GLint>(value[i].z);
            }

            glUniform3iv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &set_as_int(const std::vector<glm::vec3> &value)
          {
            CHECK_ID;
            const size_t size = value.size();
            GLint *tmp_value = new GLint[size * 3];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 3 + 0] = (value[i].x);
              tmp_value[i * 3 + 1] = (value[i].y);
              tmp_value[i * 3 + 2] = (value[i].z);
            }

            glUniform3iv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &operator = (const std::vector<glm::ivec3> &value)
          {
            CHECK_ID;
            const size_t size = value.size();
            GLint *tmp_value = new GLint[size * 3];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 3 + 0] = (value[i].x);
              tmp_value[i * 3 + 1] = (value[i].y);
              tmp_value[i * 3 + 2] = (value[i].z);
            }

            glUniform3iv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          // for array wrappers
          inline uniform_variable &operator = (const array_wrapper<GLint[3]> &value)
          {
            CHECK_ID;
            glUniform3iv(id, value.size, reinterpret_cast<const GLint *>(value.array));
            return *this;
          }
          inline uniform_variable &set_as_int(const array_wrapper<ct::vector3> &value)
          {
            CHECK_ID;
            const size_t size = value.size;
            GLint *tmp_value = new GLint[size * 3];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 3 + 0] = neam::ct::conversion::to<GLint>(value.array[i].x);
              tmp_value[i * 3 + 1] = neam::ct::conversion::to<GLint>(value.array[i].y);
              tmp_value[i * 3 + 2] = neam::ct::conversion::to<GLint>(value.array[i].z);
            }

            glUniform3iv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &set_as_int(const array_wrapper<glm::vec3> &value)
          {
            CHECK_ID;
            const size_t size = value.size;
            GLint *tmp_value = new GLint[size * 3];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 3 + 0] = (value.array[i].x);
              tmp_value[i * 3 + 1] = (value.array[i].y);
              tmp_value[i * 3 + 2] = (value.array[i].z);
            }

            glUniform3iv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &operator = (const array_wrapper<glm::ivec3> &value)
          {
            CHECK_ID;
            const size_t size = value.size;
            GLint *tmp_value = new GLint[size * 3];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 3 + 0] = (value.array[i].x);
              tmp_value[i * 3 + 1] = (value.array[i].y);
              tmp_value[i * 3 + 2] = (value.array[i].z);
            }

            glUniform3iv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          /// UNSIGNED INT 3V ///

          // set the value
          inline uniform_variable &set_as_uint(const ct::vector3 &value)
          {
            CHECK_ID;
            glUniform3ui(id, neam::ct::conversion::to<GLuint>(value.x), neam::ct::conversion::to<GLuint>(value.y), neam::ct::conversion::to<GLuint>(value.z));
            return *this;
          }
          inline uniform_variable &set_as_uint(const glm::vec3 &value)
          {
            CHECK_ID;
            glUniform3ui(id, (value.x), (value.y), (value.z));
            return *this;
          }
          inline uniform_variable &operator = (const glm::uvec3 &value)
          {
            CHECK_ID;
            glUniform3ui(id, (value.x), (value.y), (value.z));
            return *this;
          }

          // for fixed size arrays
          template<size_t Size>
          inline uniform_variable &operator = (const GLuint (&value)[Size][3])
          {
            CHECK_ID;
            glUniform3uiv(id, Size, value);
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &set_as_uint(const ct::vector3 (&value)[Size])
          {
            CHECK_ID;
            GLuint *tmp_value = new GLuint[Size * 3];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 3 + 0] = neam::ct::conversion::to<GLuint>(value[i].x);
              tmp_value[i * 3 + 1] = neam::ct::conversion::to<GLuint>(value[i].y);
              tmp_value[i * 3 + 2] = neam::ct::conversion::to<GLuint>(value[i].z);
            }

            glUniform3uiv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &set_as_uint(const glm::vec3 (&value)[Size])
          {
            CHECK_ID;
            GLuint *tmp_value = new GLuint[Size * 3];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 3 + 0] = (value[i].x);
              tmp_value[i * 3 + 1] = (value[i].y);
              tmp_value[i * 3 + 2] = (value[i].z);
            }

            glUniform3uiv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const glm::uvec3 (&value)[Size])
          {
            CHECK_ID;
            GLuint *tmp_value = new GLuint[Size * 3];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 3 + 0] = (value[i].x);
              tmp_value[i * 3 + 1] = (value[i].y);
              tmp_value[i * 3 + 2] = (value[i].z);
            }

            glUniform3uiv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const std::array<GLuint[3], Size> &value)
          {
            CHECK_ID;
            glUniform3uiv(id, Size, value.data());
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &set_as_uint(const std::array<ct::vector3, Size> &value)
          {
            CHECK_ID;
            GLuint *tmp_value = new GLuint[Size * 3];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 3 + 0] = neam::ct::conversion::to<GLuint>(value[i].x);
              tmp_value[i * 3 + 1] = neam::ct::conversion::to<GLuint>(value[i].y);
              tmp_value[i * 3 + 2] = neam::ct::conversion::to<GLuint>(value[i].z);
            }

            glUniform3uiv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &set_as_uint(const std::array<glm::vec3, Size> &value)
          {
            CHECK_ID;
            GLuint *tmp_value = new GLuint[Size * 3];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 3 + 0] = (value[i].x);
              tmp_value[i * 3 + 1] = (value[i].y);
              tmp_value[i * 3 + 2] = (value[i].z);
            }

            glUniform3uiv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const std::array<glm::uvec3, Size> &value)
          {
            CHECK_ID;
            GLuint *tmp_value = new GLuint[Size * 3];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 3 + 0] = (value[i].x);
              tmp_value[i * 3 + 1] = (value[i].y);
              tmp_value[i * 3 + 2] = (value[i].z);
            }

            glUniform3uiv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          // for vectors
          inline uniform_variable &operator = (const std::vector<GLuint[3]> &value)
          {
            CHECK_ID;
            glUniform3uiv(id, value.size(), reinterpret_cast<const GLuint *>(value.data()));
            return *this;
          }
          inline uniform_variable &set_as_uint(const std::vector<ct::vector3> &value)
          {
            CHECK_ID;
            const size_t size = value.size();
            GLuint *tmp_value = new GLuint[size * 3];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 3 + 0] = neam::ct::conversion::to<GLuint>(value[i].x);
              tmp_value[i * 3 + 1] = neam::ct::conversion::to<GLuint>(value[i].y);
              tmp_value[i * 3 + 2] = neam::ct::conversion::to<GLuint>(value[i].z);
            }

            glUniform3uiv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &set_as_uint(const std::vector<glm::vec3> &value)
          {
            CHECK_ID;
            const size_t size = value.size();
            GLuint *tmp_value = new GLuint[size * 3];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 3 + 0] = (value[i].x);
              tmp_value[i * 3 + 1] = (value[i].y);
              tmp_value[i * 3 + 2] = (value[i].z);
            }

            glUniform3uiv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &operator = (const std::vector<glm::uvec3> &value)
          {
            CHECK_ID;
            const size_t size = value.size();
            GLuint *tmp_value = new GLuint[size * 3];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 3 + 0] = (value[i].x);
              tmp_value[i * 3 + 1] = (value[i].y);
              tmp_value[i * 3 + 2] = (value[i].z);
            }

            glUniform3uiv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          // for array wrappers
          inline uniform_variable &operator = (const array_wrapper<GLuint[3]> &value)
          {
            CHECK_ID;
            glUniform3uiv(id, value.size, reinterpret_cast<const GLuint *>(value.array));
            return *this;
          }
          inline uniform_variable &set_as_uint(const array_wrapper<ct::vector3> &value)
          {
            CHECK_ID;

            const size_t size = value.size;
            GLuint *tmp_value = new GLuint[size * 3];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 3 + 0] = neam::ct::conversion::to<GLuint>(value.array[i].x);
              tmp_value[i * 3 + 1] = neam::ct::conversion::to<GLuint>(value.array[i].y);
              tmp_value[i * 3 + 2] = neam::ct::conversion::to<GLuint>(value.array[i].z);
            }

            glUniform3uiv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &set_as_uint(const array_wrapper<glm::vec3> &value)
          {
            CHECK_ID;

            const size_t size = value.size;
            GLuint *tmp_value = new GLuint[size * 3];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 3 + 0] = (value.array[i].x);
              tmp_value[i * 3 + 1] = (value.array[i].y);
              tmp_value[i * 3 + 2] = (value.array[i].z);
            }

            glUniform3uiv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &operator = (const array_wrapper<glm::uvec3> &value)
          {
            CHECK_ID;

            const size_t size = value.size;
            GLuint *tmp_value = new GLuint[size * 3];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 3 + 0] = (value.array[i].x);
              tmp_value[i * 3 + 1] = (value.array[i].y);
              tmp_value[i * 3 + 2] = (value.array[i].z);
            }

            glUniform3uiv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          /// FLOAT 4V /// // (yes, I'll do this fucking copy-pasting for EVERY
          ///          /// //  FUCKING TYPES THAT OPENGL HAS. FUCK.)

          // set the value
          inline uniform_variable &operator = (const ct::vector4 &value)
          {
            CHECK_ID;
            glUniform4f(id, neam::ct::conversion::to<float>(value.x), neam::ct::conversion::to<float>(value.y), neam::ct::conversion::to<float>(value.z), neam::ct::conversion::to<float>(value.w));
            return *this;
          }
          inline uniform_variable &operator = (const glm::vec4 &value)
          {
            CHECK_ID;
            glUniform4f(id, (value.x), (value.y), (value.z), (value.w));
            return *this;
          }

          // for fixed size arrays
          template<size_t Size>
          inline uniform_variable &operator = (const float (&value)[Size][4])
          {
            CHECK_ID;
            glUniform4fv(id, Size, value);
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const ct::vector4 (&value)[Size])
          {
            CHECK_ID;
            GLfloat *tmp_value = new GLfloat[Size * 4];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 4 + 0] = neam::ct::conversion::to<float>(value[i].x);
              tmp_value[i * 4 + 1] = neam::ct::conversion::to<float>(value[i].y);
              tmp_value[i * 4 + 2] = neam::ct::conversion::to<float>(value[i].z);
              tmp_value[i * 4 + 2] = neam::ct::conversion::to<float>(value[i].z);
            }

            glUniform4fv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const glm::vec4 (&value)[Size])
          {
            CHECK_ID;
            GLfloat *tmp_value = new GLfloat[Size * 4];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 4 + 0] = (value[i].x);
              tmp_value[i * 4 + 1] = (value[i].y);
              tmp_value[i * 4 + 2] = (value[i].z);
              tmp_value[i * 4 + 3] = (value[i].w);
            }

            glUniform4fv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const std::array<float[4], Size> &value)
          {
            CHECK_ID;
            glUniform4fv(id, Size, value.data());
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const std::array<ct::vector4, Size> &value)
          {
            CHECK_ID;
            GLfloat *tmp_value = new GLfloat[Size * 4];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 4 + 0] = neam::ct::conversion::to<float>(value[i].x);
              tmp_value[i * 4 + 1] = neam::ct::conversion::to<float>(value[i].y);
              tmp_value[i * 4 + 2] = neam::ct::conversion::to<float>(value[i].z);
              tmp_value[i * 4 + 3] = neam::ct::conversion::to<float>(value[i].w);
            }

            glUniform4fv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const std::array<glm::vec4, Size> &value)
          {
            CHECK_ID;
            GLfloat *tmp_value = new GLfloat[Size * 4];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 4 + 0] = (value[i].x);
              tmp_value[i * 4 + 1] = (value[i].y);
              tmp_value[i * 4 + 2] = (value[i].z);
              tmp_value[i * 4 + 3] = (value[i].w);
            }

            glUniform4fv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          // for vectors
          inline uniform_variable &operator = (const std::vector<float[4]> &value)
          {
            CHECK_ID;
            glUniform4fv(id, value.size(), reinterpret_cast<const GLfloat *>(value.data()));
            return *this;
          }
          inline uniform_variable &operator = (const std::vector<ct::vector4> &value)
          {
            CHECK_ID;
            const size_t size = value.size();
            GLfloat *tmp_value = new GLfloat[size * 4];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 4 + 0] = neam::ct::conversion::to<float>(value[i].x);
              tmp_value[i * 4 + 1] = neam::ct::conversion::to<float>(value[i].y);
              tmp_value[i * 4 + 2] = neam::ct::conversion::to<float>(value[i].z);
              tmp_value[i * 4 + 3] = neam::ct::conversion::to<float>(value[i].w);
            }

            glUniform4fv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &operator = (const std::vector<glm::vec4> &value)
          {
            CHECK_ID;
            const size_t size = value.size();
            GLfloat *tmp_value = new GLfloat[size * 4];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 4 + 0] = (value[i].x);
              tmp_value[i * 4 + 1] = (value[i].y);
              tmp_value[i * 4 + 2] = (value[i].z);
              tmp_value[i * 4 + 3] = (value[i].w);
            }

            glUniform4fv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          // for array wrappers
          inline uniform_variable &operator = (const array_wrapper<float[4]> &value)
          {
            CHECK_ID;
            glUniform4fv(id, value.size, reinterpret_cast<const GLfloat *>(value.array));
            return *this;
          }
          inline uniform_variable &operator = (const array_wrapper<ct::vector4> &value)
          {
            CHECK_ID;

            const size_t size = value.size;
            GLfloat *tmp_value = new GLfloat[size * 4];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 4 + 0] = neam::ct::conversion::to<float>(value.array[i].x);
              tmp_value[i * 4 + 1] = neam::ct::conversion::to<float>(value.array[i].y);
              tmp_value[i * 4 + 2] = neam::ct::conversion::to<float>(value.array[i].z);
              tmp_value[i * 4 + 3] = neam::ct::conversion::to<float>(value.array[i].w);
            }

            glUniform4fv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &operator = (const array_wrapper<glm::vec4> &value)
          {
            CHECK_ID;

            const size_t size = value.size;
            GLfloat *tmp_value = new GLfloat[size * 4];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 4 + 0] = (value.array[i].x);
              tmp_value[i * 4 + 1] = (value.array[i].y);
              tmp_value[i * 4 + 2] = (value.array[i].z);
              tmp_value[i * 4 + 3] = (value.array[i].w);
            }

            glUniform4fv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          /// INT 3V ///

          // set the value
          inline uniform_variable &set_as_int(const ct::vector4 &value)
          {
            CHECK_ID;
            glUniform4i(id, neam::ct::conversion::to<GLint>(value.x), neam::ct::conversion::to<GLint>(value.y), neam::ct::conversion::to<GLint>(value.z), neam::ct::conversion::to<GLint>(value.w));
            return *this;
          }
          inline uniform_variable &set_as_int(const glm::vec4 &value)
          {
            CHECK_ID;
            glUniform4i(id, (value.x), (value.y), (value.z), (value.w));
            return *this;
          }
          inline uniform_variable &operator = (const glm::ivec4 &value)
          {
            CHECK_ID;
            glUniform4i(id, (value.x), (value.y), (value.z), (value.w));
            return *this;
          }

          // for fixed size arrays
          template<size_t Size>
          inline uniform_variable &operator = (const GLint (&value)[Size][4])
          {
            CHECK_ID;
            glUniform4iv(id, Size, value);
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &set_as_int(const ct::vector4 (&value)[Size])
          {
            CHECK_ID;
            GLint *tmp_value = new GLint[Size * 4];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 4 + 0] = neam::ct::conversion::to<GLint>(value[i].x);
              tmp_value[i * 4 + 1] = neam::ct::conversion::to<GLint>(value[i].y);
              tmp_value[i * 4 + 2] = neam::ct::conversion::to<GLint>(value[i].z);
              tmp_value[i * 4 + 3] = neam::ct::conversion::to<GLint>(value[i].w);
            }

            glUniform4iv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &set_as_int(const glm::vec4 (&value)[Size])
          {
            CHECK_ID;
            GLint *tmp_value = new GLint[Size * 4];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 4 + 0] = (value[i].x);
              tmp_value[i * 4 + 1] = (value[i].y);
              tmp_value[i * 4 + 2] = (value[i].z);
              tmp_value[i * 4 + 3] = (value[i].w);
            }

            glUniform4iv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const glm::ivec4 (&value)[Size])
          {
            CHECK_ID;
            GLint *tmp_value = new GLint[Size * 4];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 4 + 0] = (value[i].x);
              tmp_value[i * 4 + 1] = (value[i].y);
              tmp_value[i * 4 + 2] = (value[i].z);
              tmp_value[i * 4 + 3] = (value[i].w);
            }

            glUniform4iv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const std::array<GLint[4], Size> &value)
          {
            CHECK_ID;
            glUniform4iv(id, Size, value.data());
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &set_as_int(const std::array<ct::vector4, Size> &value)
          {
            CHECK_ID;
            GLint *tmp_value = new GLint[Size * 4];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 4 + 0] = neam::ct::conversion::to<GLint>(value[i].x);
              tmp_value[i * 4 + 1] = neam::ct::conversion::to<GLint>(value[i].y);
              tmp_value[i * 4 + 2] = neam::ct::conversion::to<GLint>(value[i].z);
              tmp_value[i * 4 + 3] = neam::ct::conversion::to<GLint>(value[i].w);
            }

            glUniform4iv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &set_as_int(const std::array<glm::vec4, Size> &value)
          {
            CHECK_ID;
            GLint *tmp_value = new GLint[Size * 4];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 4 + 0] = (value[i].x);
              tmp_value[i * 4 + 1] = (value[i].y);
              tmp_value[i * 4 + 2] = (value[i].z);
              tmp_value[i * 4 + 3] = (value[i].w);
            }

            glUniform4iv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const std::array<glm::ivec4, Size> &value)
          {
            CHECK_ID;
            GLint *tmp_value = new GLint[Size * 4];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 4 + 0] = (value[i].x);
              tmp_value[i * 4 + 1] = (value[i].y);
              tmp_value[i * 4 + 2] = (value[i].z);
              tmp_value[i * 4 + 3] = (value[i].w);
            }

            glUniform4iv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          // for vectors
          inline uniform_variable &operator = (const std::vector<GLint[4]> &value)
          {
            CHECK_ID;
            glUniform4iv(id, value.size(), reinterpret_cast<const GLint *>(value.data()));
            return *this;
          }
          inline uniform_variable &set_as_int(const std::vector<ct::vector4> &value)
          {
            CHECK_ID;
            const size_t size = value.size();
            GLint *tmp_value = new GLint[size * 4];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 4 + 0] = neam::ct::conversion::to<GLint>(value[i].x);
              tmp_value[i * 4 + 1] = neam::ct::conversion::to<GLint>(value[i].y);
              tmp_value[i * 4 + 2] = neam::ct::conversion::to<GLint>(value[i].z);
              tmp_value[i * 4 + 3] = neam::ct::conversion::to<GLint>(value[i].w);
            }

            glUniform4iv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &set_as_int(const std::vector<glm::vec4> &value)
          {
            CHECK_ID;
            const size_t size = value.size();
            GLint *tmp_value = new GLint[size * 4];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 4 + 0] = (value[i].x);
              tmp_value[i * 4 + 1] = (value[i].y);
              tmp_value[i * 4 + 2] = (value[i].z);
              tmp_value[i * 4 + 3] = (value[i].w);
            }

            glUniform4iv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &operator = (const std::vector<glm::ivec4> &value)
          {
            CHECK_ID;
            const size_t size = value.size();
            GLint *tmp_value = new GLint[size * 4];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 4 + 0] = (value[i].x);
              tmp_value[i * 4 + 1] = (value[i].y);
              tmp_value[i * 4 + 2] = (value[i].z);
              tmp_value[i * 4 + 3] = (value[i].w);
            }

            glUniform4iv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          // for array wrappers
          inline uniform_variable &operator = (const array_wrapper<GLint[4]> &value)
          {
            CHECK_ID;
            glUniform4iv(id, value.size, reinterpret_cast<const GLint *>(value.array));
            return *this;
          }
          inline uniform_variable &set_as_int(const array_wrapper<ct::vector4> &value)
          {
            CHECK_ID;
            const size_t size = value.size;
            GLint *tmp_value = new GLint[size * 4];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 4 + 0] = neam::ct::conversion::to<GLint>(value.array[i].x);
              tmp_value[i * 4 + 1] = neam::ct::conversion::to<GLint>(value.array[i].y);
              tmp_value[i * 4 + 2] = neam::ct::conversion::to<GLint>(value.array[i].z);
              tmp_value[i * 4 + 3] = neam::ct::conversion::to<GLint>(value.array[i].w);
            }

            glUniform4iv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &set_as_int(const array_wrapper<glm::vec4> &value)
          {
            CHECK_ID;
            const size_t size = value.size;
            GLint *tmp_value = new GLint[size * 4];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 4 + 0] = (value.array[i].x);
              tmp_value[i * 4 + 1] = (value.array[i].y);
              tmp_value[i * 4 + 2] = (value.array[i].z);
              tmp_value[i * 4 + 3] = (value.array[i].w);
            }

            glUniform4iv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &operator = (const array_wrapper<glm::ivec4> &value)
          {
            CHECK_ID;
            const size_t size = value.size;
            GLint *tmp_value = new GLint[size * 4];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 4 + 0] = (value.array[i].x);
              tmp_value[i * 4 + 1] = (value.array[i].y);
              tmp_value[i * 4 + 2] = (value.array[i].z);
              tmp_value[i * 4 + 3] = (value.array[i].w);
            }

            glUniform4iv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          /// UNSIGNED INT 3V ///

          // set the value
          inline uniform_variable &set_as_uint(const ct::vector4 &value)
          {
            CHECK_ID;
            glUniform4ui(id, neam::ct::conversion::to<GLuint>(value.x), neam::ct::conversion::to<GLuint>(value.y), neam::ct::conversion::to<GLuint>(value.z), neam::ct::conversion::to<GLuint>(value.w));
            return *this;
          }
          inline uniform_variable &set_as_uint(const glm::vec4 &value)
          {
            CHECK_ID;
            glUniform4ui(id, (value.x), (value.y), (value.z), (value.w));
            return *this;
          }
          inline uniform_variable &operator = (const glm::uvec4 &value)
          {
            CHECK_ID;
            glUniform4ui(id, (value.x), (value.y), (value.z), (value.w));
            return *this;
          }

          // for fixed size arrays
          template<size_t Size>
          inline uniform_variable &operator = (const GLuint (&value)[Size][4])
          {
            CHECK_ID;
            glUniform4uiv(id, Size, value);
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &set_as_uint(const ct::vector4 (&value)[Size])
          {
            CHECK_ID;
            GLuint *tmp_value = new GLuint[Size * 4];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 4 + 0] = neam::ct::conversion::to<GLuint>(value[i].x);
              tmp_value[i * 4 + 1] = neam::ct::conversion::to<GLuint>(value[i].y);
              tmp_value[i * 4 + 2] = neam::ct::conversion::to<GLuint>(value[i].z);
              tmp_value[i * 4 + 3] = neam::ct::conversion::to<GLuint>(value[i].w);
            }

            glUniform4uiv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &set_as_uint(const glm::vec4 (&value)[Size])
          {
            CHECK_ID;
            GLuint *tmp_value = new GLuint[Size * 4];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 4 + 0] = (value[i].x);
              tmp_value[i * 4 + 1] = (value[i].y);
              tmp_value[i * 4 + 2] = (value[i].z);
              tmp_value[i * 4 + 3] = (value[i].w);
            }

            glUniform4uiv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const glm::uvec4 (&value)[Size])
          {
            CHECK_ID;
            GLuint *tmp_value = new GLuint[Size * 4];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 4 + 0] = (value[i].x);
              tmp_value[i * 4 + 1] = (value[i].y);
              tmp_value[i * 4 + 2] = (value[i].z);
              tmp_value[i * 4 + 3] = (value[i].w);
            }

            glUniform4uiv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const std::array<GLuint[4], Size> &value)
          {
            CHECK_ID;
            glUniform4uiv(id, Size, value.data());
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &set_as_uint(const std::array<ct::vector4, Size> &value)
          {
            CHECK_ID;
            GLuint *tmp_value = new GLuint[Size * 4];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 4 + 0] = neam::ct::conversion::to<GLuint>(value[i].x);
              tmp_value[i * 4 + 1] = neam::ct::conversion::to<GLuint>(value[i].y);
              tmp_value[i * 4 + 2] = neam::ct::conversion::to<GLuint>(value[i].z);
              tmp_value[i * 4 + 3] = neam::ct::conversion::to<GLuint>(value[i].w);
            }

            glUniform4uiv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &set_as_uint(const std::array<glm::vec4, Size> &value)
          {
            CHECK_ID;
            GLuint *tmp_value = new GLuint[Size * 4];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 4 + 0] = (value[i].x);
              tmp_value[i * 4 + 1] = (value[i].y);
              tmp_value[i * 4 + 2] = (value[i].z);
              tmp_value[i * 4 + 3] = (value[i].w);
            }

            glUniform4uiv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          template<size_t Size>
          inline uniform_variable &operator = (const std::array<glm::uvec4, Size> &value)
          {
            CHECK_ID;
            GLuint *tmp_value = new GLuint[Size * 4];

            for (unsigned int i = 0; i < Size; ++i)
            {
              tmp_value[i * 4 + 0] = (value[i].x);
              tmp_value[i * 4 + 1] = (value[i].y);
              tmp_value[i * 4 + 2] = (value[i].z);
              tmp_value[i * 4 + 3] = (value[i].w);
            }

            glUniform4uiv(id, Size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          // for vectors
          inline uniform_variable &operator = (const std::vector<GLuint[4]> &value)
          {
            CHECK_ID;
            glUniform4uiv(id, value.size(), reinterpret_cast<const GLuint *>(value.data()));
            return *this;
          }
          inline uniform_variable &set_as_uint(const std::vector<ct::vector4> &value)
          {
            CHECK_ID;
            const size_t size = value.size();
            GLuint *tmp_value = new GLuint[size * 4];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 4 + 0] = neam::ct::conversion::to<GLuint>(value[i].x);
              tmp_value[i * 4 + 1] = neam::ct::conversion::to<GLuint>(value[i].y);
              tmp_value[i * 4 + 2] = neam::ct::conversion::to<GLuint>(value[i].z);
              tmp_value[i * 4 + 3] = neam::ct::conversion::to<GLuint>(value[i].w);
            }

            glUniform4uiv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &set_as_uint(const std::vector<glm::vec4> &value)
          {
            CHECK_ID;
            const size_t size = value.size();
            GLuint *tmp_value = new GLuint[size * 4];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 4 + 0] = (value[i].x);
              tmp_value[i * 4 + 1] = (value[i].y);
              tmp_value[i * 4 + 2] = (value[i].z);
              tmp_value[i * 4 + 3] = (value[i].w);
            }

            glUniform4uiv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &operator = (const std::vector<glm::uvec4> &value)
          {
            CHECK_ID;
            const size_t size = value.size();
            GLuint *tmp_value = new GLuint[size * 4];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 4 + 0] = (value[i].x);
              tmp_value[i * 4 + 1] = (value[i].y);
              tmp_value[i * 4 + 2] = (value[i].z);
              tmp_value[i * 4 + 3] = (value[i].w);
            }

            glUniform4uiv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          // for array wrappers
          inline uniform_variable &operator = (const array_wrapper<GLuint[4]> &value)
          {
            CHECK_ID;
            glUniform4uiv(id, value.size, reinterpret_cast<const GLuint *>(value.array));
            return *this;
          }
          inline uniform_variable &set_as_uint(const array_wrapper<ct::vector4> &value)
          {
            CHECK_ID;

            const size_t size = value.size;
            GLuint *tmp_value = new GLuint[size * 4];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 4 + 0] = neam::ct::conversion::to<GLuint>(value.array[i].x);
              tmp_value[i * 4 + 1] = neam::ct::conversion::to<GLuint>(value.array[i].y);
              tmp_value[i * 4 + 2] = neam::ct::conversion::to<GLuint>(value.array[i].z);
              tmp_value[i * 4 + 3] = neam::ct::conversion::to<GLuint>(value.array[i].w);
            }

            glUniform4uiv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &set_as_uint(const array_wrapper<glm::vec4> &value)
          {
            CHECK_ID;

            const size_t size = value.size;
            GLuint *tmp_value = new GLuint[size * 4];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 4 + 0] = (value.array[i].x);
              tmp_value[i * 4 + 1] = (value.array[i].y);
              tmp_value[i * 4 + 2] = (value.array[i].z);
              tmp_value[i * 4 + 3] = (value.array[i].w);
            }

            glUniform4uiv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }
          inline uniform_variable &operator = (const array_wrapper<glm::uvec4> &value)
          {
            CHECK_ID;

            const size_t size = value.size;
            GLuint *tmp_value = new GLuint[size * 4];

            for (unsigned int i = 0; i < size; ++i)
            {
              tmp_value[i * 4 + 0] = (value.array[i].x);
              tmp_value[i * 4 + 1] = (value.array[i].y);
              tmp_value[i * 4 + 2] = (value.array[i].z);
              tmp_value[i * 4 + 3] = (value.array[i].w);
            }

            glUniform4uiv(id, size, tmp_value);
            delete [] tmp_value;
            return *this;
          }

          // TODO: mat2 mat3 mat4
          // TODO: mat2x3 mat3x2 mat3x2 mat2x3 mat2x4 mat4x2 mat3x4 mat4x3

          /// OPERATORS ///
          /// NOTE: **NOT** a value based comparison.

          // just in case
          inline bool operator == (const uniform_variable &o) const
          {
            return id == o.id;
          }

          inline bool operator != (const uniform_variable &o) const
          {
            return id != o.id;
          }

          // to store in maps (jic)
          inline bool operator < (const uniform_variable &o) const
          {
            return id < o.id;
          }

        private:
          GLint id;
      };
#undef CHECK_ID
    } // namespace shader
  } // namespace yaggler
} // namespace neam

#endif /*__N_4228199451929725216_1925374995__OPENGL_UNIFORM_VAR_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

