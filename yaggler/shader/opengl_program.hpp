//
// file : program.hpp
// in : file:///home/tim/projects/yaggler/yaggler/shader/program.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 17/10/2013 22:32:47
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


#ifndef __N_184189058577424383_273887087__PROGRAM_HPP__
# define __N_184189058577424383_273887087__PROGRAM_HPP__

#include <cstring>

#include <GLEW/glew.h>
// #include <GL/gl.h>

#include <shader/program_base.hpp>
#include <shader/except.hpp>
#include <shader/opengl_uniform_var.hpp>

#include <tools/bad_type.hpp>

#include <tools/execute_pack.hpp>
#include <tools/tuple.hpp>
#include <tools/type_at_index.hpp>
#include <yaggler_type.hpp>

namespace neam
{
  namespace yaggler
  {
    namespace shader
    {
      /// \brief a shader program (a set of shaders of at least the vertex and fragment stage)
      /// \note shaders could be dynamically attached/detached, but the ones that are in CTShaders are attached at creation.
      /// \note shaders in CTShaders are removed/deleted when the program is destructed, not the other attached ones.
      ///        (a solution to this is to attach locally created shaders to the program, they will be kept in memory
      ///         until the program is destructed / relinked (this is a std openGL behavior))
      /// \note there's no 'shared instances' (as with shaders) here.
      /// \param CTShaders is a list of shader to link within the program
      template<typename... CTShaders>
      class program<type::opengl, CTShaders...>
      {
        private: // iterate over ct shaders. [helper]
          template<size_t... Idx>
          inline void it_over_cts_attach(cr::seq<Idx...>)
          {
            NEAM_EXECUTE_PACK((glAttachShader(pg_id, shaders.template get<Idx>().get_id())));
          }

          template<typename Shader>
          inline void _it_recompile_cts_inner(Shader &shader)
          {
            shader.recompile();
            failed |= shader.has_failed();
          }

          template<size_t... Idx>
          inline void it_over_cts_recompile(cr::seq<Idx...>)
          {
            failed = false;
            NEAM_EXECUTE_PACK((_it_recompile_cts_inner(shaders.template get<Idx>())));
          }

        public:
          /// \brief create a link to the original shader program
          /// \param _pg_id The program openGL ID
          explicit program(GLuint _pg_id)
          : shaders(), pg_id(_pg_id), ownership(false)
          {
            it_over_cts_attach(cr::gen_seq<sizeof...(CTShaders)>());
          }

          /// \brief encapsulate the original shader program
          /// \param _pg_id The program openGL ID
          program(GLuint _pg_id, assume_ownership_t)
          : shaders(), pg_id(_pg_id), ownership(true)
          {
            it_over_cts_attach(cr::gen_seq<sizeof...(CTShaders)>());
          }

          /// \brief Create a new program
          /// \throw shader_exception if openGL refuse to allocate an ID for the program
          program()
          : shaders(), pg_id(0), ownership(true)
          {
            if (!(pg_id = glCreateProgram()))
            {
              glGetError(); // discard error
              throw shader_exception("Unable to create the shader (glCreateShader)");
            }

            it_over_cts_attach(cr::gen_seq<sizeof...(CTShaders)>());
          }

          /// \brief create a link to the program \p p
          /// \param p The program to create a link from
          program(const program &p)
            : shaders(p.shaders), pg_id(p.get_id()), ownership(false)
          {
          }

          /// \brief create a link to the program \p p (that has a different type)
          /// \param p The program to create a link from
          template<typename... OCTShaders>
          program(const program<type::opengl, OCTShaders...> &p)
            : shaders(), pg_id(p.get_id()), ownership(false)
          {
            it_over_cts_attach(cr::gen_seq<sizeof...(CTShaders)>());
          }

          /// \brief Stole ownership of a openGL shader program (if \p p is an owner)
          /// \note If \p p doesn't have ownership, it will still create a link to it
          /// \param p The program to stole ownership from
          /// \note \p p will be a link to the program
          ///     (meaning that when its life come to an end, it won't touch the openGL shader program)
          program(program &p, stole_ownership_t)
          : shaders(p.shaders), pg_id(p.get_id()), ownership(p.has_ownership())
          {
            p.give_up_ownership();
          }

          /// \brief Stole ownership of a openGL shader program (if \p p is the owner)
          /// \note If \p p doesn't have ownership, it will still create a link to it
          /// \param p The program to stole ownership from
          /// \note \p p will be a link to the program
          ///     (meaning that when its life come to an end, it won't touch the openGL shader program)
          template<typename... OCTShaders>
          program(program<type::opengl, OCTShaders...> &p, stole_ownership_t)
            : shaders(), pg_id(p.get_id()), ownership(p.has_ownership())
          {
            p.give_up_ownership();
            it_over_cts_attach(cr::gen_seq<sizeof...(CTShaders)>());
          }

          /// \brief Stole ownership of a openGL shader program (it \p p has it)
          /// \note If \p p doesn't have ownership, it will still create a link to it
          /// \param p The program to stole ownership from
          /// \note \p p will be a link to the program
          ///     (meaning that when its life come to an end, it won't touch the openGL shader program)
          program(program &&p)
            : shaders(std::move(p.shaders)), pg_id(p.get_id()), ownership(p.has_ownership())
          {
            p.give_up_ownership();
          }

          /// \brief Stole ownership of a openGL shader program (it \p p has it)
          /// \note If \p p doesn't have ownership, it will still create a link to it
          /// \param p The program to stole ownership from
          /// \note \p p will be a link to the program
          ///     (meaning that when its life come to an end, it won't touch the openGL shader program)
          template<typename... OCTShaders>
          program(program<type::opengl, OCTShaders...> && p)
            : shaders(), pg_id(p.get_id()), ownership(p.has_ownership())
          {
            p.give_up_ownership();
            it_over_cts_attach(cr::gen_seq<sizeof...(CTShaders)>());
          }

          /// \brief destructor
          ~program()
          {
            if (pg_id && ownership)
              glDeleteProgram(pg_id);
          }

          /// \brief abandon ownership of the underlying openGL resource
          /// The resource life-scope will no more be related with the current object lifetime.
          program &give_up_ownership()
          {
            ownership = false;
            return *this;
          }

          /// \brief forcibly acquire the ownership of the underlying openGL resource
          /// The resource life-scope will be related with the current object lifetime.
          program &assume_ownership()
          {
            ownership = true;
            return *this;
          }

          /// \brief Copy + stole the ownership (if any) of another shader program object.
          /// The acquired (if acquired) resource life-scope will be related with the current object lifetime.
          /// \see stole_ownership_t
          template<typename... OInit>
          program &stole(program<type::opengl, OInit...> &t)
          {
            if (&t != this)
            {
              if (ownership)
                glDeleteProgram(pg_id);

              ownership = t.has_ownership();
              pg_id = t.get_id();
              t.give_up();
            }
            return *this;
          }

          /// \brief link to another shader program object. (it could be associated as a pointer copy)
          /// The openGL resource life-scope will not be related with the current object lifetime.
          template<typename... OInit>
          program &link_to(program<type::opengl, OInit...> &t)
          {
            if (&t != this)
            {
              if (ownership)
                glDeleteProgram(pg_id);

              ownership = false;
              pg_id = t.get_id();
            }
            return *this;
          }

          /// \brief return the number of shaders in CTShaders
          static constexpr size_t get_ct_shaders_number()
          {
            return sizeof...(CTShaders);
          }

          /// \brief return the openGL shader ID
          GLuint get_id() const
          {
            return pg_id;
          }

          /// \brief return true if this instance has the ownership of the resource
          bool has_ownership() const
          {
            return ownership;
          }

          /// \brief recompile shaders in the CTShaders template parameter and relink the program
          void recompile_cts_shader_if_changed()
          {
            try
            {
              it_over_cts_recompile(cr::gen_seq<sizeof...(CTShaders)>());
            }
            catch(const shader_exception &e)
            {
              neam::cr::out.error() << LOGGER_INFO << "program<type::opengl>::recompile_cts_shader_if_changed: shader_exception: " << e.what() << std::endl;
              return;
            }
            link();
          }

          /// \brief link the program
          void link()
          {
            try
            {
              it_over_cts_recompile(cr::gen_seq<sizeof...(CTShaders)>());
            }
            catch(const shader_exception &e)
            {
              neam::cr::out.error() << LOGGER_INFO << "program<type::opengl>::link: shader_exception: " << e.what() << std::endl;
              return;
            }

            if (failed)
              return;

            glLinkProgram(pg_id);

            GLint ret;
            glGetProgramiv(pg_id, GL_LINK_STATUS, &ret);

            if (!ret)
            {
              constexpr size_t max_len = 8192;
              char *message = new char[max_len];
              const char *header = "could not link program:\n";
              strcpy(message, header);
              glGetProgramInfoLog(pg_id, max_len - strlen(header), &ret, message + strlen(header));
              throw shader_exception(message, true);
            }
          }

          /// \brief return the uniform variable associated with a given \p name
          /// \note the variable will be invalid if the program is relinked
          uniform_variable get_uniform_variable(const std::string &name) const
          {
            return glGetUniformLocation(pg_id, name.data());
          }

          /// \brief return the uniform block associated with a given \p name
          /// \note the variable will be invalid if the program is relinked
          uniform_variable get_uniform_block(const std::string &name) const
          {
            return uniform_variable(glGetUniformBlockIndex(pg_id, name.data()), pg_id);
          }

          /// \brief return the uniform variable/block associated with a given \p name
          /// switch based on a boolean parameter (mostly used in materials)
          /// \note the variable will be invalid if the program is relinked
          uniform_variable get_uniform(const std::string &name, bool block)
          {
            if (block)
              return get_uniform_block(name);
            return get_uniform_variable(name);
          }

          /// \brief Associates a generic vertex attribute index with a named attribute variable (gl doc for glBindAttribLocation)
          /// \note don't forget to call link() after !
          void bind_attribute_location(const std::string &attribute_name, GLuint location)
          {
            glBindAttribLocation(pg_id, location, attribute_name.data());
          }

          /// \brief bind the program for the current rendering context
          /// \note relink
          void relink_and_use()
          {
            try
            {
              it_over_cts_recompile(cr::gen_seq<sizeof...(CTShaders)>());
            }
            catch(const shader_exception &e)
            {
              neam::cr::out.error() << LOGGER_INFO << "program<type::opengl>::use: shader_exception: " << e.what() << std::endl;
              return;
            }
            link();
            glUseProgram(pg_id);
          }

          /// \brief bind the program for the current rendering context
          /// const version: don't relink
          void use() const
          {
            glUseProgram(pg_id);
          }

          /// \brief unbind the program for the current context
          void unuse() const
          {
            glUseProgram(0);
          }

          /// \brief attach a shader to the program
          /// \note you may want to relink the program
          template<typename Shader>
          void attach_shader(const Shader &shader)
          {
            glAttachShader(pg_id, shader.get_id());
          }

          /// \brief detach a shader from the program
          /// \note you may want to relink the program
          template<typename Shader>
          void detach_shader(const Shader &shader)
          {
            glDetachShader(pg_id, shader.get_id());
          }

          /// \brief return true if the program has failed to build
          bool has_failed() const
          {
            return failed;
          }


          /// \brief create a link to a more generic shader.
          /// no inheritance is involved. This cast will create a 'link' program shader object.
          /// the 'link' (a program object that refer to the same openGL program) won't destroy the
          /// openGL program when it's destructor is called
          operator program<type::opengl> ()
          {
            return program<type::opengl> (pg_id);
          }

          /// \brief return the yaggler shader object that's in the CTShaders template parameter at index \p Index
          template<size_t Index>
          auto get_shader_at_index()
          -> typename ct::type_at_index<Index, CTShaders..., cr::bad_type>::type &
          {
            return shaders.template get<Index>();
          }

          /// \brief return the yaggler shader object that's in the CTShaders template parameter at index \p Index
          template<size_t Index>
          auto get_shader_at_index() const
          -> const typename ct::type_at_index<Index, CTShaders..., cr::bad_type>::type &
          {
            return shaders.template get<Index>();
          }

        private: // vars.
          cr::tuple<CTShaders...> shaders; // ct shaders
          GLuint pg_id;
          bool ownership;
          bool failed = false;
      };
    } // namespace shader
  } // namespace yaggler
} // namespace neam

#endif /*__N_184189058577424383_273887087__PROGRAM_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

