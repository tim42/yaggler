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

#ifndef _WIN32
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
#endif

#include <GLEW/glew.h>
// #include <GL/gl.h>
#include <string.h>
#include <tools/enable_if.hpp>
#include <tools/ownership.hpp>

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
      //        ShaderSource:           neam::embed::string or neam::yaggler::embed::shader::function_ptr_* (see shader/shader_type.hpp)
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
            // NOTE: this is not what you want.
/*            if (std::is_same<ShaderSourceType, opengl::constexpr_string>::value || std::is_same<ShaderSourceType, opengl::file>::value)
            {
              static GLuint static_shader_id = 0;

              link = true;

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
            else */
            std::cout << "shader: construct" << std::endl;

            if (!(shader_id = glCreateShader(ShaderType::value)))
            {
              failed = true;
              throw_on_glerror<shader_exception>("Unable to create the shader (glCreateShader): ");
              failed = false;
            }

            has_source_changed(0);      // init this func;
            changed = true;
          }

          shader(GLuint _id)
            : shader_id(_id), source(), link(true)
          {
            has_source_changed(0);      // init this func;
            changed = true;
          }

          shader(GLuint _id, assume_ownership_t)
            : shader_id(_id), source(), link(false)
          {
            has_source_changed(0);      // init this func;
            changed = true;
          }

          // we keep the same ShaderType
          shader(const shader &o)
          : shader_id(o.get_id()), source(o.source), additional_str(o.get_additional_strings()), link(true)
          {
            has_source_changed(0);      // init this func;
            changed = false;
          }
          template<typename OShaderSourceType, typename OShaderSource, typename OShaderOption>
          shader(const shader<ShaderType, OShaderSourceType, OShaderSource, OShaderOption> &o)
            : shader_id(o.get_id()), source(), additional_str(o.get_additional_strings()), link(true)
          {
            has_source_changed(0);      // init this func;
            changed = true;
          }

          shader(shader &o, stole_ownership_t)
          : shader_id(o.get_id()), source(o.source), additional_str(o.get_additional_strings()), link(o.is_link())
          {
            o.give_up_ownership();
            has_source_changed(0);      // init this func;
            changed = false;
          }
          template<typename OShaderSourceType, typename OShaderSource, typename OShaderOption>
          shader(shader<ShaderType, OShaderSourceType, OShaderSource, OShaderOption> &o, stole_ownership_t)
            : shader_id(o.get_id()), source(), additional_str(o.get_additional_strings()), link(o.is_link())
          {
            o.give_up_ownership();
            has_source_changed(0);      // init this func;
            changed = true;
          }

          shader(shader &&o)
          : shader_id(o.shader_id), source(std::move(o.source)), additional_str(std::move(o.additional_str)), link(o.link)
          {
            o.give_up_ownership();
            has_source_changed(0);      // init this func;
            changed = false;
          }
          template<typename OShaderSourceType, typename OShaderSource, typename OShaderOption>
          shader(shader<ShaderType, OShaderSourceType, OShaderSource, OShaderOption> &&o)
            : shader_id(o.get_id()), source(), additional_str(o.get_additional_strings()), link(o.is_link())
          {
            o.give_up_ownership();
            has_source_changed(0);      // init this func;
            changed = true;
          }

          ~shader()
          {
            if (shader_id && !link)
            {
              glDeleteShader(shader_id);
            }
          }

          shader &give_up_ownership()
          {
            link = true;
            return *this;
          }

          shader &assume_ownership()
          {
            link = false;
            return *this;
          }

          bool is_link() const
          {
            return link;
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

          constexpr int set_source(NCR_ENABLE_IF((!std::is_same<ShaderSourceType, opengl::dyn_string>::value), const std::string &) = "") const
          {
            static_assert(!(sizeof(ShaderSourceType) + 1), "shader<type::opengl>::set_source is only possible with ShaderSourceType set to opengl::dyn_string");
            return 0;
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

          void _preload()
          {
            if (!std::is_same<ShaderSourceType, opengl::function>::value)
              source = get_source_string(0);
          }

          // return the source name (filename) or type (ex.: "<dynamic string>")
          static constexpr inline const char *get_source_name()
          {
            return _get_source_name(0);
          }

        private:
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
              strcat(message, get_source_name());
              strcat(message, "'");

              strcat(message, ":\n");
              glGetShaderInfoLog(shader_id, max_len - strlen(message), &status, message + strlen(message));
              throw shader_exception(message, true);
            }
#ifndef YAGGLER_NO_MESSAGES
            std::cout << "YAGGLER: compiled shader  '" << get_source_name() << "'" << std::endl;
#endif
            failed = false;
          }

          // return the name (filename/typename)
          static constexpr inline const char *_get_source_name(NCR_ENABLE_IF((std::is_same<ShaderSourceType, opengl::constexpr_string>::value), int) = 0)
          {
            return "<constexpr  string>";
          }

          static constexpr inline const char *_get_source_name(NCR_ENABLE_IF((std::is_same<ShaderSourceType, opengl::file>::value), int) = 0)
          {
            return ShaderSource::value;
          }

          static constexpr inline const char *_get_source_name(NCR_ENABLE_IF((std::is_same<ShaderSourceType, opengl::dyn_string>::value), int) = 0)
          {
            return "<dynamic string>";
          }

          static constexpr inline const char *_get_source_name(NCR_ENABLE_IF((std::is_same<ShaderSourceType, opengl::function>::value), int) = 0)
          {
            return "<function return>";
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

          bool link = false;

          bool failed = false;
          bool changed = false;
          float old_time = 0.0f;
      };
    } // namespace shader
  } // namespace yaggler
} // namespace neam

#endif /*__N_707724487786268404_1508345494__PIXEL_SHADER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

