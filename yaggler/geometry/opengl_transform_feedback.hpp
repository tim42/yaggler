//
// file : opengl_transform_feedback.hpp
// in : file:///home/tim/projects/yaggler/yaggler/geometry/opengl_transform_feedback.hpp
//
// created by : Timothée Feuillet on linux-vnd3.site
// date: 20/11/2015 16:27:10
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

#ifndef __N_18597228691000876858_68461380__OPENGL_TRANSFORM_FEEDBACK_HPP__
# define __N_18597228691000876858_68461380__OPENGL_TRANSFORM_FEEDBACK_HPP__

#include <yaggler_type.hpp>

#include <GLEW/glew.h>

#include <geometry/transform_feedback_base.hpp>
#include <geometry/opengl_buffer.hpp>
#include <geometry/opengl_draw_state.hpp>
#include <yaggler_except.hpp>

namespace neam
{
  namespace yaggler
  {
    namespace geometry
    {
      /// \brief this class represent a transform feedback.
      template<>
      class transform_feedback<type::opengl>
      {
        public:
          /// \brief create a link to the original transform feedback
          /// \param _pg_id The program openGL ID
          explicit transform_feedback(GLuint _id)
          : id(_id), ownership(false)
          {
          }

          /// \brief encapsulate the original transform feedback
          /// \param _pg_id The program openGL ID
          transform_feedback(GLuint _id, assume_ownership_t)
          : id(_id), ownership(true)
          {
          }

          /// \brief Create a new program
          /// \throw shader_exception if openGL refuse to allocate an ID for the program
          transform_feedback()
          : id(0), ownership(true)
          {
            glGenTransformFeedbacks(1, &id);
            if (!id)
            {
              glGetError(); // discard error
              throw yaggler_exception("Unable to create the transform_feedback (glCreateProgram)", __FILE__, __LINE__);
            }
          }

          /// \brief create a link to the program \p p
          /// \param p The program to create a link from
          transform_feedback(const transform_feedback &tf)
            : id(tf.get_id()), ownership(false)
          {
          }

          /// \brief Stole ownership of a openGL transform feedback (if \p p is an owner)
          /// \note If \p p doesn't have ownership, it will still create a link to it
          /// \param p The program to stole ownership from
          /// \note \p p will be a link to the program
          ///     (meaning that when its life come to an end, it won't touch the openGL transform feedback)
          transform_feedback(transform_feedback &tf, stole_ownership_t)
          : id(tf.get_id()), ownership(tf.has_ownership())
          {
            tf.give_up_ownership();
          }

          /// \brief Stole ownership of a openGL transform feedback (it \p p has it)
          /// \note If \p p doesn't have ownership, it will still create a link to it
          /// \param p The program to stole ownership from
          /// \note \p p will be a link to the program
          ///     (meaning that when its life come to an end, it won't touch the openGL transform feedback)
          transform_feedback(transform_feedback &&tf)
            : id(tf.get_id()), ownership(tf.has_ownership())
          {
            tf.give_up_ownership();
          }

          /// \brief destructor
          ~transform_feedback()
          {
            if (ownership)
                glDeleteTransformFeedbacks(1, &id);
          }

          /// \brief abandon ownership of the underlying openGL resource
          /// The resource life-scope will no more be related with the current object lifetime.
          transform_feedback &give_up_ownership()
          {
            ownership = false;
            return *this;
          }

          /// \brief forcibly acquire the ownership of the underlying openGL resource
          /// The resource life-scope will be related with the current object lifetime.
          transform_feedback &assume_ownership()
          {
            ownership = true;
            return *this;
          }

          /// \brief Copy + stole the ownership (if any) of another transform feedback object.
          /// The acquired (if acquired) resource life-scope will be related with the current object lifetime.
          /// \see stole_ownership_t
          transform_feedback &stole(transform_feedback &tf)
          {
            if (&tf != this)
            {
              if (ownership)
                glDeleteTransformFeedbacks(1, &id);

              ownership = tf.has_ownership();
              id = tf.get_id();
              tf.give_up_ownership();
            }
            return *this;
          }

          /// \brief Copy + stole the ownership (if any) of another transform feedback object.
          /// The acquired (if acquired) resource life-scope will be related with the current object lifetime.
          /// \see stole_ownership_t
          transform_feedback &operator = (transform_feedback &&tf)
          {
            return stole(tf);
          }

          /// \brief link to another transform feedback object. (it could be associated as a pointer copy)
          /// The openGL resource life-scope will not be related with the current object lifetime.
          transform_feedback &link_to(const transform_feedback &tf)
          {
            if (&tf != this)
            {
              if (ownership)
                glDeleteTransformFeedbacks(1, &id);

              ownership = false;
              id = tf.get_id();
            }
            return *this;
          }

          /// \brief link to another transform feedback object. (it could be associated as a pointer copy)
          /// The openGL resource life-scope will not be related with the current object lifetime.
          transform_feedback &operator = (const transform_feedback &tf)
          {
            return link_to(tf);
          }

          /// \brief return the openGL transform feedback ID
          GLuint get_id() const
          {
            return id;
          }

          /// \brief return true if this instance has the ownership of the resource
          bool has_ownership() const
          {
            return ownership;
          }

          /// \brief Pause the transform feedback
          void pause()
          {
            glPauseTransformFeedback();
          }

          /// \brief Resume the transform feedback
          void resume()
          {
            glResumeTransformFeedback();
          }

          /// \brief Bind the transform feedback object
          void bind()
          {
            glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, id);
          }

          /// \brief Unbind the transform feedback object
          void unbind()
          {
            glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
          }

          /// \brief Begin the transform feedback process
          /// \note automatically bind the transform_feedback object
          void begin(GLenum primitive_mode)
          {
            bind();
            glBeginTransformFeedback(primitive_mode);
          }

          /// \brief End the transform feedback process
          void end()
          {
            glEndTransformFeedback();
          }

          /// \brief Draw the transform_feedback buffer
          template<typename Type>
          void draw(const draw_state<Type> &ds)
          {
            ds.set_transform_feedback(*this);
          }

        private:
          GLuint id;
          bool ownership = true;
      };


      /// \brief scope ->begin() ->end() helper class (mostly used for transform_feedback)
      template<typename Class>
      class scoped_begin
      {
        public:
          scoped_begin(Class &_cl) : cl(_cl)
          {
            cl.begin();
          }

          scoped_begin(Class &_cl, neam::assume_ownership_t) : cl(_cl)
          {
          }

          ~scoped_begin()
          {
            cl.end();
          }

          Class &cl;
      };
      using scoped_transform_feedback = scoped_begin<transform_feedback<type::opengl>>;
    } // namespace geometry
  } // namespace yaggler
} // namespace neam

#endif /*__N_18597228691000876858_68461380__OPENGL_TRANSFORM_FEEDBACK_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

