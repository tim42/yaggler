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
#include "shader/except.hpp"

#include "../yaggler_type.hpp"

namespace neam
{
  namespace yaggler
  {
    namespace shader
    {
      /// \brief represent a single shader unit
      /// \param shadertype {type: neam::embed::GLenum} The shader type (vertex shader, fragment shader, ...). This is directly an openGL value
      /// \param ShaderSource {type: neam::yaggler::shader::shader_loader<...>} the loader for this shader \see common_shader_loader.hpp
      /// \param ShaderOption {type: neam::embed::shader::option} \see shader/shader_options.hpp
      ///                     Some shader options (like one shot compilation, shared shader instance, reloaded on change shader)
      /// \param ShaderEnv The kind of environment to use for the shader (whether or not to support #include, or absolutly nothing at all).
      ///                  \see common_shader_env.hpp
      /// \note for an "easy" shader creation, you should take a look at klmb/material_usings.hpp
      template<typename ShaderType, typename ShaderSource, typename ShaderOption, typename ShaderEnv>
      class shader<type::opengl, ShaderType, ShaderSource, ShaderOption, ShaderEnv>
      {
        public:
          /// \brief create and initialize a new shader
          /// \exception shader_exception if openGL refuse to create a new shader
          shader()
            : shader_id(0)
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
            _preload();
          }

          /// \brief initialize using an existing openGL shader id
          /// \note the newly created shader won't have the ownership of the underlying openGL shader:
          ///       destructing it won't destroy the openGL shader
          explicit shader(GLuint _id)
            : shader_id(_id), ownership(false)
          {
            ShaderSource::has_source_changed();      // init this func;
            _preload();
          }

          /// \brief initialize using an existing openGL shader id
          /// \note the newly created shader will have the ownership of the underlying openGL shader:
          ///       destructing it will destroy the openGL shader
          shader(GLuint _id, assume_ownership_t)
            : shader_id(_id), ownership(true)
          {
            ShaderSource::has_source_changed();      // init this func;
            _preload();
          }

          /// \brief initialize using an existing shader
          /// \note the newly created shader won't have the ownership of the underlying openGL shader:
          ///       destructing it won't destroy the openGL shader
          shader(const shader &o)
           : shader_id(o.get_id()), ownership(false)
          {
            ShaderSource::has_source_changed();      // init this func;
            _preload();
          }

          /// \brief initialize using an existing shader of a different type / with a different set of option / loader / ...
          /// \note the newly created shader won't have the ownership of the underlying openGL shader:
          ///       destructing it won't destroy the openGL shader
          template<typename OShaderSourceType, typename OShaderSource, typename OShaderOption>
          shader(const shader<ShaderType, OShaderSourceType, OShaderSource, OShaderOption> &o)
            : shader_id(o.get_id()), ownership(true)
          {
            ShaderSource::has_source_changed();      // init this func;
            _preload();
          }

          /// \brief initialize using an existing shader
          /// \note the newly created shader will acquire the ownership of the underlying openGL shader only if the original shader has the ownership
          ///       if the shader acquire the ownership, destructing it will destroy the openGL shader
          shader(shader &o, stole_ownership_t)
          : shader_id(o.get_id()), ownership(o.has_ownership())
          {
            o.give_up_ownership();
            ShaderSource::has_source_changed();      // init this func;
            _preload();
          }

          /// \brief initialize using an existing shader of a different type / with a different set of option / loader / ...
          /// \note the newly created shader will acquire the ownership of the underlying openGL shader only if the original shader has the ownership
          ///       if the shader acquire the ownership, destructing it will destroy the openGL shader
          template<typename OShaderSourceType, typename OShaderSource, typename OShaderOption>
          shader(shader<ShaderType, OShaderSourceType, OShaderSource, OShaderOption> &o, stole_ownership_t)
            : shader_id(o.get_id()), ownership(o.has_ownership())
          {
            o.give_up_ownership();
            ShaderSource::has_source_changed();      // init this func;
            _preload();
          }

          /// \brief initialize using an existing shader
          /// \note the newly created shader will acquire the ownership of the underlying openGL shader only if the original shader has the ownership
          ///       if the shader acquire the ownership, destructing it will destroy the openGL shader
          shader(shader &&o)
          : shader_id(o.shader_id), ownership(o.ownership)
          {
            o.give_up_ownership();
            ShaderSource::has_source_changed();      // init this func;
            _preload();
          }

          /// \brief initialize using an existing shader of a different type / with a different set of option / loader / ...
          /// \note the newly created shader will acquire the ownership of the underlying openGL shader only if the original shader has the ownership
          ///       if the shader acquire the ownership, destructing it will destroy the openGL shader
          template<typename OShaderSourceType, typename OShaderSource, typename OShaderOption>
          shader(shader<ShaderType, OShaderSourceType, OShaderSource, OShaderOption> &&o)
            : shader_id(o.get_id()), ownership(o.has_ownership())
          {
            o.give_up_ownership();
            ShaderSource::has_source_changed();      // init this func;
            _preload();
          }

          ~shader()
          {
            if (shader_id && ownership && ShaderOption::value != shader_option::shared_instance)
            {
              glDeleteShader(shader_id);
            }
          }

          /// \brief abandon the overniship of a openGL shader
          /// This mostly mean that destructing the instance won't destruct the shader
          shader &give_up_ownership()
          {
            ownership = false;
            return *this;
          }

          /// \brief forcibly acquire the overniship of a openGL shader
          /// This mostly mean that destructing the instance will destruct the shader
          shader &assume_ownership()
          {
            // no ownership on shareds. (there is a workaround for this limitation,
            // but this mean that you explicitly ask YägGler to break this rule:
            //  assigning a 'shared' shader to a 'non-shared' one
            if (ShaderOption::value != shader_option::shared_instance)
              ownership = true;
            return *this;
          }

          /// \brief return true if the instance ohas the ownership of the underlying openGL shader
          bool has_ownership() const
          {
            return ownership;
          }

          /// \brief return the current source string of the shader (could be different from the source of the actual shader)
          std::string get_source() const
          {
            return env.get_source();
          }

          constexpr static GLenum shader_type = ShaderType::value;

          /// \brief rebuild the shader, re-get the sources, ...
          /// \note this could be slow.
          /// \see recompile_if_changed()
          void recompile()
          {
            _preload();
            unlocked_recompile();
          }

          /// \brief rebuild the shader only if it has changed since the last build
          /// faster than a direct call to recompile()
          /// \note the ShaderOption one_shot_compilation disable the source watch for this method
          bool recompile_if_changed()
          {
            bool changed = false;
            if (ShaderOption::value != one_shot_compilation && ShaderSource::has_source_changed())
            {
              _preload();
              changed = true;
            }
            if (changed || env.has_changed())
            {
              unlocked_recompile();
              return true;
            }
            return false;
          }

          /// \brief return true if the shader has failed to build
          bool has_failed() const
          {
            return failed;
          }

          /// \brief return the shader openGL id
          GLuint get_id() const
          {
            return shader_id;
          }

          /// \brief return the source name (ex. the filename) or type (ex. \code "<dynamic string>" \endcode)
          static inline std::string get_source_name()
          {
            return ShaderSource::get_source_name();
          }

          /// \brief preload the source code of the shader
          /// \note marked as advanced
          void _preload()
          {
            env.on_change(ShaderSource::get_source_string());
          }

          using shader_source = ShaderSource;
          static constexpr shader_option shader_config_option = ShaderOption::value;
          using shader_env = ShaderEnv;

        private:
          inline void unlocked_recompile()
          {
            std::string source = env.get_source();
            env.clear_changed();

            // build
            const GLchar *source_array[] =
            {
              source.data(),
            };

            glGetError();
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

          ShaderEnv env;
        public:
          decltype(ShaderEnv::api) &environment = env.api;
        private:

          bool ownership;

          bool failed = false;
      };
    } // namespace shader
  } // namespace yaggler
} // namespace neam

#endif /*__N_707724487786268404_1508345494__PIXEL_SHADER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

