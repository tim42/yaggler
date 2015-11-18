//
// file : common_shader_env.hpp
// in : file:///home/tim/projects/yaggler/yaggler/shader/common_shader_env.hpp
//
// created by : Timothée Feuillet on linux-vnd3.site
// date: 17/10/2015 00:38:42
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

#ifndef __N_11405993361271642551_656833314__COMMON_SHADER_ENV_HPP__
# define __N_11405993361271642551_656833314__COMMON_SHADER_ENV_HPP__

#include "../yaggler_type.hpp"
#include "shader_env.hpp"
#include "shader/shader_preprocessor.hpp"

namespace neam
{
  namespace yaggler
  {
    namespace shader
    {
      /// \brief This is a shader env absolutely empty
      /// \note does not work with KLMB/bleunw stages.
      template<>
      struct shader_env<no_env>
      {
        public:
          /// \brief Called each time the source has changed
          void on_change(const std::string &s)
          {
            source = s;
            changed = true;
          }

          /// \brief Called to query for env inner changes
          bool has_changed() const
          {
            return changed;
          }

          /// \brief Reset inner change flag
          void clear_changed()
          {
            changed = false;
          }

          /// \brief Called to retrieve the source code of the shader
          const std::string &get_source() const
          {
            return source;
          }

          /// \brief The empty API of the env
          struct {} api;

        private:
          std::string source;
          bool changed = false;
      };

      /// \brief This is the base class for shader envs with preprocessor usage
      /// The crtp pattern is a bit strange, but it allow code-reuse.
      /// \note you have to define the on_change method in a child class !
      template<typename EnvAPI>
      class preprocessor_shader_env
      {
        public:
          preprocessor_shader_env()
          {
            api.env = static_cast<decltype(api.env)>(this);
          }

          /// \brief Called to query for env inner changes
          bool has_changed() const
          {
            return changed;
          }

          /// \brief Reset inner change flag
          void clear_changed()
          {
            changed = false;
          }

          /// \brief Called to retrieve the source code of the shader
          std::string get_source() const
          {
            return preprocessor.get_string();
          }

        public:
          EnvAPI api;

        protected:
          shader_preprocessor preprocessor;
          bool changed = true;
      };

      /// \brief A base API for env with preprocessor
      /// \note it supports all that is needed by KLMB
      /// \note it's up to the holder to setup insert_before_token and initial_addstr_token.
      /// \note you have to define the void process_change() method in the holder class.
      template<typename Env>
      class preprocessor_shader_env_api
      {
        public:
          preprocessor_shader_env_api()
            : insert_before_token {0, shader_preprocessor::e_token_type::none, 0, "", {}},
              initial_addstr_token {0, shader_preprocessor::e_token_type::none, 0, "", {}}
          {
          }

          /// \brief 'parse' (what a big word here...) the source to get a preprocessor token and its value.
          /// return an empty string if nothing was found, the 'value' of the first occurrence if anything was found
          /// (if there isn't values, it simply return \code "\n" \endcode)
          ///
          /// to get something like
          /// \code #define TOTO       42 \endcode
          /// do:
          /// \code my_shader.environment.get_preprocessor_value("TOTO"); \endcode
          /// or
          /// \code my_shader.environment.get_preprocessor_value("TOTO", "define"); \endcode
          /// and the function will return \code std::string("42") \endcode
          ///
          /// to get the very first \code #ifdef \endcode, one could do:
          /// \code my_shader.environment.get_preprocessor_value("", "ifdef") \endcode
          /// to get the version of a shader, one could do:
          /// \code my_shader.environment.get_preprocessor_value("", "version") \endcode
          /// \note it will not expand values
          /// \note it will not cache results (so this could be ~slow !)
          std::string get_preprocessor_value(const std::string &name, const std::string &preprocessor_token = "define") const
          {
            auto token = env->preprocessor.get_token(preprocessor_token, name);
            if (token.type == shader_preprocessor::e_token_type::none)
              return std::string("");
            std::string value;
            if (!token.subtokens.size())
              return std::string("\n");
            auto it = token.subtokens.begin();
            ++it; // skip the prep. token
            if (name.size())
              ++it; // skip the name token
            while (it != token.subtokens.end() && it->type != shader_preprocessor::e_token_type::preprocessor_token)
              ++it; // skip args
            // build the return string
            shader_preprocessor::e_token_type last_type = shader_preprocessor::e_token_type::none;
            for (; it != token.subtokens.end(); ++it)
            {
              if (last_type != shader_preprocessor::e_token_type::preprocessor_token_args &&
                  it->type == shader_preprocessor::e_token_type::preprocessor_token_args)
                value += '(';
              if (last_type == shader_preprocessor::e_token_type::preprocessor_token_args)
              {
                if (it->type == shader_preprocessor::e_token_type::preprocessor_token_args)
                  value += ',';
                else
                  value += ')';
              }
              if (it->type == shader_preprocessor::e_token_type::preprocessor_token_string)
                  value += '"';
              value += it->str;
              if (it->type == shader_preprocessor::e_token_type::preprocessor_token_string)
                  value += '"';
            }
            if (last_type == shader_preprocessor::e_token_type::preprocessor_token_args)
              value += ')';
            if (!value.size())
              return std::string("\n");
            return value;
          }

          /// \brief Append a given string after the #version line
          /// you may want to recompile and link the program after this.
          void append_to_additional_strings(const std::string &to_add)
          {
            additional_strings = (additional_strings + to_add) + std::string("\n");
            if (no_additional_string)
              return;
            // push the change at the end of the additional strings
            env->preprocessor.insert_before(insert_before_token, to_add);
            env->changed = true;
            env->process_change();
          }

          /// \brief return the string that is added on the top of the shader file
          const std::string &get_additional_strings() const
          {
            return additional_strings;
          }

          /// \brief clear the set of strings that is added on the top of the shader file
          void clear_additional_strings()
          {
            additional_strings.clear();
            if (no_additional_string)
              return;
            env->preprocessor.remove_token_range(initial_addstr_token, insert_before_token);
            env->changed = true;
            env->process_change();
          }

        protected:
          std::string additional_strings;
          bool no_additional_string = true;
          shader_preprocessor::token_entry insert_before_token;
          shader_preprocessor::token_entry initial_addstr_token;

        protected:
          Env *env;
          friend Env;
          template<typename EnvAPI> friend class preprocessor_shader_env;
      };

      /// \brief This is a shader env with a simple preprocessor parser
      /// It provides just enough functionality to work with KLMB and some bleunw shaders
      template<>
      struct shader_env<simple_preprocessor> : public preprocessor_shader_env<preprocessor_shader_env_api<shader_env<simple_preprocessor>>>
      {
        public:
          /// \brief Called each time the source has changed
          void on_change(const std::string &s)
          {
            preprocessor.clear();
            preprocessor.feed(s);

            //insert additional_strings comments, save those tokens
            auto version_token = preprocessor.get_first_token("version");
            api.no_additional_string = true;
            if (version_token.type == shader_preprocessor::e_token_type::none)
              return;
            auto token_list = preprocessor.insert_after(version_token, "//< YAGGLER: shader_env<simple_preprocessor> additional strings: end >");
            if (!token_list.size())
              return;
            api.insert_before_token = token_list.front();

            token_list = preprocessor.insert_after(version_token, "//< YAGGLER: shader_env<simple_preprocessor> additional strings >");
            if (!token_list.size())
              return;
            api.initial_addstr_token = token_list.front();

            api.no_additional_string = false;

            preprocessor.insert_before(api.insert_before_token, api.additional_strings);

            changed = false;
          }

        protected:
          /// \brief called when the source of the shader is changed by the API
          void process_change() const
          {
          }

        protected:
          friend class preprocessor_shader_env_api<shader_env<simple_preprocessor>>;
      };

      /// \brief This is a shader env with a preprocessor parser
      /// It provides support for #include
      template<>
      struct shader_env<advanced_preprocessor> : public preprocessor_shader_env<preprocessor_shader_env_api<shader_env<advanced_preprocessor>>>
      {
        public:
          /// \brief Called each time the source has changed
          void on_change(const std::string &s)
          {
            preprocessor.clear();
            preprocessor.feed(s);

            //insert additional_strings comments, save those tokens
            auto version_token = preprocessor.get_first_token("version");
            api.no_additional_string = true;
            if (version_token.type == shader_preprocessor::e_token_type::none)
              return;
            auto token_list = preprocessor.insert_after(version_token, "//< YAGGLER: shader_env<advanced_preprocessor> additional strings: end >");
            if (!token_list.size())
              return;
            api.insert_before_token = token_list.front();

            token_list = preprocessor.insert_after(version_token, "//< YAGGLER: shader_env<advanced_preprocessor> additional strings >");
            if (!token_list.size())
              return;
            api.initial_addstr_token = token_list.front();

            api.no_additional_string = false;

            preprocessor.insert_before(api.insert_before_token, api.additional_strings);

            process_includes();

            changed = false;
          }

        protected:
          /// \brief process includes (non-recursively), and replace them by the file content
          /// \todo handle more complicated things like a double inclusion guard, ...
          void process_includes()
          {
            size_t count = 0;
            auto token = preprocessor.get_first_token("include");
            while (token.type != shader_preprocessor::e_token_type::none)
            {
              // Oops, too many included files !
              if (count++ > 100)
              {
                neam::cr::out.error() << LOGGER_INFO << "Too Many file included !!!" << std::endl;
                preprocessor.clear();
                return;
              }

              std::string include_file = (++token.subtokens.begin())->str;
              std::string content = "";

              std::ifstream file(include_file);

              if (!file)
                neam::cr::out.error() << LOGGER_INFO << "could not find included file '" << include_file << "'" << std::endl;
              else
                content = static_cast<const std::ostringstream &>(std::ostringstream() << (file.rdbuf())).str();
              preprocessor.replace_token(token, "// include: " + include_file + " \n" + content + "\n// end include: " + include_file + "\n");

              // next token:
              token = preprocessor.get_first_token("include");
            }
          }

          /// \brief called when the source of the shader is changed by the API
          void process_change()
          {
            process_includes();
          }

        protected:
          friend class preprocessor_shader_env_api<shader_env<advanced_preprocessor>>;
      };
    }
  }
} // namespace neam

#endif /*__N_11405993361271642551_656833314__COMMON_SHADER_ENV_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

