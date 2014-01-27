//
// file : pixel_shader.hpp
// in : file:///home/tim/projects/yaggler/yaggler/shader/pixel_shader.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 17/10/2013 04:42:10
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


#ifndef __N_707724487786268404_1508345494__PIXEL_SHADER_HPP__
# define __N_707724487786268404_1508345494__PIXEL_SHADER_HPP__

#include <string>
#include <fstream>
#include <sstream>
#include <type_traits>
#include <mutex>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <GLEW/glew.h>
#include <GL/gl.h>
#include <string.h>
#include <tools/enable_if.hpp>

#include "shader/shader_base.hpp"
#include "shader/shader_options.hpp"
#include "shader/except.hpp"

// #include "threads/spinlock.hpp"

namespace neam
{
  namespace yaggler
  {
    namespace shader
    {
      // template types:
      //        shadertype:             neam::embed::GLenum
      //        ShaderSourceType:       [...] (see shader/shader_type.hpp)
      //        ShaderSource:           neam::embed::string or neam::yaggler::shader::opengl::function_ptr (see shader/shader_type.hpp)
      //        ShaderOption:           neam::embed::shader::option
      //
      // (not) thread safe !!! (no spinlock & cpu burns)
      template<typename ShaderType, typename ShaderSourceType, typename ShaderSource, typename ShaderOption>
      class shader<type::opengl, ShaderType, ShaderSourceType, ShaderSource, ShaderOption>
      {
        public:
          shader()
            : shader_id(0), source()
          {
            // the ressource will be shared for file / constexpr strings. (yes, I can do that :D )
            if (std::is_same<ShaderSourceType, opengl::constexpr_string>::value || std::is_same<ShaderSourceType, opengl::file>::value)
            {
              static GLuint static_shader_id = 0;

              if (static_shader_id)
                shader_id = static_shader_id;
              else
              {
                if (!(shader_id = glCreateShader(ShaderType::value)))
                {
                  failed = true;
                  throw_on_glerror<shader_exception>("Unable to create the shader (glCreateShader): ");
                  failed = false;
                }
                static_shader_id = shader_id;
              }
            }
            else if (!(shader_id = glCreateShader(ShaderType::value)))
            {
              failed = true;
              throw_on_glerror<shader_exception>("Unable to create the shader (glCreateShader): ");
              failed = false;
            }

            has_source_changed(0);      // init this func;
            recompile();                // REEEEEBUUUILLLLL:D
          }

          ~shader()
          {
            if (shader_id)
            {
              glDeleteShader(shader_id);
            }
          }

          const std::string &get_source() const
          {
            return source;
          }

          // rebuild the shader. re-get the sources, ...
          // NOTE: this could be slow.
          void recompile()
          {
            unlocked_recompile();
          }

          // rebuild the shader only if it has changed since the last build
          // possibly faster than a direct call to 'recompile()'
          void recompile_if_changed()
          {
            if (changed || has_source_changed(0))
              unlocked_recompile();
          }

          // return true if the shader has failed to build
          bool has_failed() const
          {
            return failed;
          }

          // set/change the internal source string
          // only existing for ShaderSourceType === opengl::dyn_string
          // an empty string will reset the source to its default (ct) value
          // you may need to call recompile() (and link() in the program) after...
          void set_source(NCR_ENABLE_IF((std::is_same<ShaderSourceType, opengl::dyn_string>::value), const std::string &) _source = "")
          {
            source = _source;
            changed = true;
          }

          constexpr int set_source(NCR_ENABLE_IF((!std::is_same<ShaderSourceType, opengl::dyn_string>::value), const std::string &) = "")
          {
            static_assert(!(sizeof(ShaderSourceType) + 1), "shader<type::opengl>::set_source is only possible with ShaderSourceType set to opengl::dyn_string");
            return 0;
          }

          // return the shader id
          GLuint get_id() const
          {
            return shader_id;
          }

          shader &clear_additional_strings()
          {
            additional_str.clear();
            return *this;
          }

          // add some strings to the code of the shader
          // (right after the #version token)
          // you may want to recompile and link the program after this...
          shader &append_to_additional_strings(const std::string &to_add)
          {
            additional_str = (additional_str + to_add) + std::string("\n");
            changed = true;
            return *this;
          }

        private: // unlocked functions (they must be called with their lock held)
          inline void unlocked_recompile()
          {
            if (!std::is_same<ShaderSourceType, opengl::function>::value)
              source = get_source_string(0);

            // get the version string
            for (size_t i = 0; source.size() > 9 && i < source.size() - 9; ++i)
            {
              if (source[i] == '\n' && source[i + 1] == '#')
              {
                // we got our #version
                if (!strncmp(source.data() + i + 1, "#version ", 9))
                {
                  size_t j = i + 10;
                  for (; j < source.size() && source.data()[j] != '\n'; ++j);

                  char *str = strndup(source.data() + i + 1, (j - i));

                  version_str = std::string(str);

                  // comment the original #version string
                  source[i + 1] = '/';
                  source[i + 2] = '/';

                  break;
                }
              }
            }

            // build
            GLint length_array[] =
            {
              (GLint)version_str.size(),
              (GLint)additional_str.size(),
              (GLint)source.size(),
            };
            const GLchar *source_array[] =
            {
              version_str.data(),
              additional_str.data() ? additional_str.data() : "",
              source.data(),
            };

            glShaderSource(shader_id, sizeof(length_array) / sizeof(length_array[0]), source_array, length_array);
            failed = true;
            throw_on_glerror<shader_exception>("unable to set the shader source (glShaderSource): ");
            failed = false;

            glCompileShader(shader_id);

            GLint status;
            glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);

            if (status == GL_FALSE)
            {
              failed = true;
              constexpr size_t max_len = 8192;
              char *message = new char[max_len];
              const char *header = "could not compile shader:\n";
              strcpy(message, header);
              glGetShaderInfoLog(shader_id, max_len - strlen(header), &status, message + strlen(header));
              throw shader_exception(message, true);
            }
            failed = false;
          }

        private: // source helpers

          // this one return directly a const char * to avoid the creation of a temporary std::string (and also because it's possible :D )
          constexpr inline const char *get_source_string(NCR_ENABLE_IF((std::is_same<ShaderSourceType, opengl::constexpr_string>::value), int)) const
          {
            return ShaderSource::value;
          }

          inline std::string get_source_string(NCR_ENABLE_IF((std::is_same<ShaderSourceType, opengl::file>::value), int) = 0) const
          {
            // if anybody says there's a shorter way to load a file into a string in C++, perhaps he is lying ;)
            return static_cast<std::ostringstream&>(std::ostringstream() << (std::ifstream(ShaderSource::value).rdbuf())).str();
          }

          inline std::string get_source_string(NCR_ENABLE_IF((std::is_same<ShaderSourceType, opengl::dyn_string>::value), int) = 0) const
          {
            if (source.empty())
              source = ShaderSource::value;
            return source;
          }

          inline std::string get_source_string(NCR_ENABLE_IF((std::is_same<ShaderSourceType, opengl::function>::value), int) = 0) const
          {
            return ShaderSource::value();
          }

        private: // watcher helpers
          inline bool has_source_changed(NCR_ENABLE_IF((std::is_same<ShaderSourceType, opengl::file>::value && ShaderOption::value == shader_option::reload_on_change), int) = 0)
          {
            // only on linux: ( :D )
            //
            // http://stackoverflow.com/q/9376975
            float old_old_time = old_time;
            struct stat file_stat;

            int err = stat(ShaderSource::value, &file_stat);
            if (err != 0)
              return true;

            // ~0.76 day, it may be enought ^^ (another big point is that if the filesystem support it, the < 1s time is also used)
            old_time = static_cast<float>(file_stat.st_mtim.tv_sec & 0xFFFF) + (static_cast<float>(file_stat.st_mtim.tv_nsec) * 1e-9f);

            if (!old_old_time)
              return false;

            return old_old_time != old_time;
          }

          inline bool has_source_changed(NCR_ENABLE_IF((std::is_same<ShaderSourceType, opengl::dyn_string>::value && ShaderOption::value == shader_option::reload_on_change), int) = 0)
          {
            // check the string:
            bool old_changed = changed;
            changed = false;
            return old_changed;
          }

          // recheck the source generated by the function.
          // NOTE: could be slow !!!
          inline bool has_source_changed(NCR_ENABLE_IF((std::is_same<ShaderSourceType, opengl::function>::value && ShaderOption::value == shader_option::reload_on_change), int) = 0) const
          {
            // check the string:
            std::string tstr = get_source_string(0);
            if (!(tstr == source))
            {
              source = tstr;
              return true;
            }
            return false;
          }

          // can't change (constexpr string)
          // this is a bit stupid to user opengl::constexpr_string and shader_option::reload_on_change
          constexpr inline bool has_source_changed(NCR_ENABLE_IF((std::is_same<ShaderSourceType, opengl::constexpr_string>::value && ShaderOption::value == shader_option::reload_on_change), int) = 0) const
          {
            return false;
          }

          // always return false
          constexpr inline bool has_source_changed(NCR_ENABLE_IF((ShaderOption::value != shader_option::reload_on_change), int) = 0) const
          {
            return false;
          }

        private:
          GLuint shader_id;
          std::string source;
          std::string version_str;
          std::string additional_str;

          bool failed = false;
          bool changed = false;
          float old_time = 0.0f;
      };
    } // namespace shader
  } // namespace yaggler
} // namespace neam

#endif /*__N_707724487786268404_1508345494__PIXEL_SHADER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

