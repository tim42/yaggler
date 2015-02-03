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
#include <type_traits>
#include <mutex>

#include <GLEW/glew.h>

#include <string.h>
#include <tools/enable_if.hpp>
#include <tools/ownership.hpp>

#include "shader/shader_base.hpp"
#include "shader/shader_options.hpp"
#include "shader/common_shader_loader.hpp"
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
      //        ShaderSource:           neam::yaggler::shader::shader_loader<...> (see common_shader_loader.hpp)
      //        ShaderOption:           neam::embed::shader::option (see shader/shader_options.hpp)
      template<typename ShaderType, typename ShaderSource, typename ShaderOption>
      class shader<type::opengl, ShaderType, ShaderSource, ShaderOption>
      {
        public:
          shader()
            : shader_id(0), source()
          {
            ownership = true;

            // the ressource could be shared for file / constexpr strings. (yes, I can do that :D )
            // NOTE: this is probably not what you want.
            if (ShaderOption::value == shader_option::shared_instance)
            {
              static GLuint static_shader_id = 0;

              ownership = false;

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

            ShaderSource::has_source_changed();      // init this func;
            changed = true;
          }

          shader(GLuint _id)
            : shader_id(_id), source(), ownership(false)
          {
            ShaderSource::has_source_changed();      // init this func;
            changed = true;
          }

          shader(GLuint _id, assume_ownership_t)
            : shader_id(_id), source(), ownership(true)
          {
            ShaderSource::has_source_changed();      // init this func;
            changed = true;
          }

          // we keep the same ShaderType
          shader(const shader &o)
          : shader_id(o.get_id()), source(o.source), additional_str(o.get_additional_strings()), ownership(false)
          {
            ShaderSource::has_source_changed();      // init this func;
            changed = false;
          }
          template<typename OShaderSourceType, typename OShaderSource, typename OShaderOption>
          shader(const shader<ShaderType, OShaderSourceType, OShaderSource, OShaderOption> &o)
            : shader_id(o.get_id()), source(), additional_str(o.get_additional_strings()), ownership(true)
          {
            ShaderSource::has_source_changed();      // init this func;
            changed = true;
          }

          shader(shader &o, stole_ownership_t)
          : shader_id(o.get_id()), source(o.source), additional_str(o.get_additional_strings()), ownership(o.has_ownership())
          {
            o.give_up_ownership();
            ShaderSource::has_source_changed();      // init this func;
            changed = false;
          }
          template<typename OShaderSourceType, typename OShaderSource, typename OShaderOption>
          shader(shader<ShaderType, OShaderSourceType, OShaderSource, OShaderOption> &o, stole_ownership_t)
            : shader_id(o.get_id()), source(), additional_str(o.get_additional_strings()), ownership(o.has_ownership())
          {
            o.give_up_ownership();
            ShaderSource::has_source_changed();      // init this func;
            changed = true;
          }

          shader(shader &&o)
          : shader_id(o.shader_id), source(std::move(o.source)), additional_str(std::move(o.additional_str)), ownership(o.ownership)
          {
            o.give_up_ownership();
            ShaderSource::has_source_changed();      // init this func;
            changed = false;
          }
          template<typename OShaderSourceType, typename OShaderSource, typename OShaderOption>
          shader(shader<ShaderType, OShaderSourceType, OShaderSource, OShaderOption> &&o)
            : shader_id(o.get_id()), source(), additional_str(o.get_additional_strings()), ownership(o.has_ownership())
          {
            o.give_up_ownership();
            ShaderSource::has_source_changed();      // init this func;
            changed = true;
          }

          ~shader()
          {
            if (shader_id && ownership && ShaderOption::value != shader_option::shared_instance)
            {
              glDeleteShader(shader_id);
            }
          }

          shader &give_up_ownership()
          {
            ownership = false;
            return *this;
          }

          shader &assume_ownership()
          {
            // no ownership on shareds. (there is a workaround this limitation,
            // but this mean that you explicitly ask YägGler to break this
            // (by assigning a 'shared' shader to a 'non-shared' one).
            if (ShaderOption::value != shader_option::shared_instance)
              ownership = true;
            return *this;
          }

          bool has_ownership() const
          {
            return ownership;
          }

          const std::string &get_source() const
          {
            return source;
          }

          constexpr static GLenum shader_type = ShaderType::value;

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
            if (changed || ShaderSource::has_source_changed())
              unlocked_recompile();
          }

          // return true if the shader has failed to build
          bool has_failed() const
          {
            return failed;
          }

          // return the shader id
          GLuint get_id() const
          {
            return shader_id;
          }

          // return the full #version line
          // (you could not use get_preprocessor_value("", "version") for this value as the line is 'removed')
          const std::string &get_version_string() const
          {
            return version_str;
          }

          // 'parse' (what a big word here...) the source to get a preprocessor token and its value.
          // return an empty string if nothing was found, the 'value' of the first occurence if anything was found
          // (if there isn't values, it simply return "\n")
          //
          // to get something like
          // #define TOTO       42
          // 'simply' do:
          // my_shader.get_preprocessor_value("TODO");
          // or
          // my_shader.get_preprocessor_value("TODO", "define");
          // and the function will return std::string("42")
          //
          // to get the first #ifdef, you could do:
          // my_shader.get_preprocessor_value("", "ifdef")
          std::string get_preprocessor_value(const std::string &name, const std::string &preprocessor_token = "define") const
          {
            std::string token = '#' + preprocessor_token;

            // get the version string
            for (size_t i = 0; source.size() > token.size() && i < source.size() - token.size(); ++i)
            {
              if (source[i] == '\n' && source[i + 1] == '#')
              {
                // we got our #[preprocessor_token]
                if (!strncmp(source.data() + i + 1, token.data(), token.size()) && isspace(source[i + 1 + token.size()]))
                {
                  // skip whitespaces
                  size_t ws = 0;
                  size_t pos = i + 1 + token.size();

                  while (isspace(source[pos + ws]) && source[pos + ws] != '\n')
                    ++ws;

                  pos += ws;
                  // cmp the value
                  if (name.empty() || (!strncmp(source.data() + pos, name.data(), name.size()) && isspace(source[pos + name.size()])))
                  {
                    // skip whitespaces
                    size_t sec_ws = 0;
                    pos += name.size();

                    while (isspace(source[pos + sec_ws]) && source[pos + sec_ws] != '\n')
                      ++sec_ws;

                    pos += sec_ws;

                    size_t j = pos;
                    for (; j < source.size() && source.data()[j] != '\n'; ++j);

                    if (j != pos)
                      return source.substr(pos, j - pos);
                    return std::string("\n");
                  }
                }
              }
            }
            return std::string("");
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

          const std::string &get_additional_strings() const
          {
            return additional_str;
          }

          // return the source name (filename) or type (ex.: "<dynamic string>")
          static constexpr inline const char *get_source_name()
          {
            return ShaderSource::get_source_name();
          }

          void _preload()
          {
            source = ShaderSource::get_source_string();
          }

          using shader_source = ShaderSource;

        private:
          inline void unlocked_recompile()
          {
            source = ShaderSource::get_source_string();

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

                  version_str = source.substr(i + 1, j - i);

                  // comment the original #version string
                  source[i + 1] = '/';
                  source[i + 2] = '/';

                  break;
                }
              }
            }

            // build
            const GLchar *source_array[] =
            {
              version_str.data(),
              additional_str.data() ? additional_str.data() : "",
              source.data(),
            };

            glShaderSource(shader_id, sizeof(source_array) / sizeof(source_array[0]), source_array, nullptr);
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
              const char *header = "could not compile shader";
              strcpy(message, header);

              strcat(message, " '");
              strcat(message, ShaderSource::get_source_name());
              strcat(message, "'");

              if (::opengl_version::debug)
              {
                strcat(message, ":\n");
                glGetShaderInfoLog(shader_id, max_len - strlen(message), &status, message + strlen(message));
              }
              throw shader_exception(message, true);
            }
#ifndef YAGGLER_NO_MESSAGES
            if (::opengl_version::debug)
            {
              neam::cr::out.debug() << LOGGER_INFO << "compiled shader  '" << ShaderSource::get_source_name() << "'" << std::endl;
            }
#endif
            failed = false;
          }

        private:
          GLuint shader_id;
          std::string source;
          std::string version_str;
          std::string additional_str;

          bool ownership;

          bool failed = false;
          bool changed = false;
      };
    } // namespace shader
  } // namespace yaggler
} // namespace neam

#endif /*__N_707724487786268404_1508345494__PIXEL_SHADER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

