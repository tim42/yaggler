//
// file : generic_material.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/material/generic_material.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 17/02/2014 15:54:08
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

#ifndef __N_6208373671893816553_11894195__GENERIC_MATERIAL_HPP__
# define __N_6208373671893816553_11894195__GENERIC_MATERIAL_HPP__

#include <glm/glm.hpp>
#include <yaggler_except.hpp>
#include <tools/ownership.hpp>

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      /// \brief wraps the complicated material class in a more generic one (speaking of C++ types)
      class material_wrapper
      {
        public:
          /// \brief destructor
          ~material_wrapper()
          {
            delete wrapper;
          }

          /// \brief default constructor, initialize with an empty material
          material_wrapper() : wrapper(nullptr) {}

          /// \brief Copy another material_wrapper but don't manage the lifetime of the wrapped material
          material_wrapper(const material_wrapper &mw) : wrapper(mw.wrapper ? mw.wrapper->clone() : nullptr) {}

          /// \brief Move another material_wrapper AND manage the lifetime of the wrapped material
          /// \note The moved material_wraper will be emptied
          material_wrapper(material_wrapper &&mw) : wrapper(mw.wrapper)
          {
            mw.wrapper = nullptr;
          }

          /// \brief wrap a material, but don't manage its lifetime
          template<typename MaterialType>
          material_wrapper(MaterialType &mt) : wrapper(new spec_wrapper<MaterialType>(&mt)) {}

          /// \brief wrap a material, but don't manage its lifetime
          template<typename MaterialType>
          material_wrapper(MaterialType *mt) : wrapper(new spec_wrapper<MaterialType>(mt)) {}

          /// \brief wrap a material and manage its lifetime
          template<typename MaterialType>
          material_wrapper(MaterialType *mt, assume_ownership_t) : wrapper(new spec_wrapper<MaterialType>(mt, neam::assume_ownership)) {}

          /// \brief wrap a material and manage its lifetime
          template<typename MaterialType>
          material_wrapper(MaterialType &&mt) : wrapper(new spec_wrapper<MaterialType>(new MaterialType(std::move(mt)), neam::assume_ownership)) {}

          /// \brief Copy another material_wrapper but don't manage the lifetime of the wrapped material
          material_wrapper &operator = (const material_wrapper &mw)
          {
            if (&mw != this)
            {
              if (wrapper)
                delete wrapper;
              wrapper = (mw.wrapper ? mw.wrapper->clone() : nullptr);
            }
            return *this;
          }

          /// \brief Move another material_wrapper AND manage the lifetime of the wrapped material
          /// \note The moved material_wraper will be emptied
          material_wrapper &operator = (material_wrapper &&mw)
          {
            if (&mw != this)
            {
              if (wrapper)
                delete wrapper;

              wrapper = mw.wrapper;
              mw.wrapper = nullptr;
            }

            return *this;
          }

          // std ownership funcs
          material_wrapper &give_up_ownership()
          {
            if (wrapper)
              wrapper->ownership = false;
            return *this;
          }

          material_wrapper &assume_ownership()
          {
            if (wrapper)
              wrapper->ownership = true;
            return *this;
          }

          /// \brief check whether the wrapper wrap a material or not
          bool is_empty() const
          {
            return !wrapper;
          }

          /// \brief bind the material
          void use() const
          {
            if (wrapper)
              wrapper->use();
          }

          /// \brief relink the shader program
          void link_shader()
          {
            if (wrapper)
              wrapper->link_shader();
          }

          /// \brief Set the varyings to be captured by the transform feedback
          /// \note Well, you'll have to (re ?)link your shader after this. (Except with the \e NV_transform_feedback extension if activated)
          /// \link https://www.opengl.org/wiki/Transform_Feedback
          void set_transform_feedback_varyings(std::initializer_list<std::string> varyings, GLenum buffer_mode)
          {
            if (wrapper)
              wrapper->set_transform_feedback_varyings(varyings, buffer_mode);
          }

          /// \brief Return a reference to the pointer of the view-projection matrix
          glm::mat4 *& get_vp_matrix()
          {
            if (!wrapper)
              throw neam::yaggler::yaggler_exception("get_vp_matrix on empty material wrapper", __FILE__, __LINE__);
            return wrapper->get_vp_matrix();
          }

          /// \brief Return a reference to the pointer of the world matrix
          glm::mat4 *& get_object_matrix()
          {
            if (!wrapper)
              throw neam::yaggler::yaggler_exception("get_object_matrix on empty material wrapper", __FILE__, __LINE__);
            return wrapper->get_object_matrix();
          }

        private:
          struct base_wrapper
          {
            virtual ~base_wrapper() {}

            virtual base_wrapper *clone() const = 0;

            virtual void use() const = 0;
            virtual void link_shader() = 0;
            virtual void set_transform_feedback_varyings(std::initializer_list<std::string> varyings, GLenum buffer_mode) = 0;


            bool ownership = false;

            // see material.hpp --> variable_indexes
            virtual glm::mat4 *& get_vp_matrix() = 0;
            virtual glm::mat4 *& get_object_matrix() = 0;
          } *wrapper;

          template<typename MaterialType>
          struct spec_wrapper : base_wrapper
          {
            spec_wrapper(MaterialType *_matptr) : matptr(_matptr) {}
            spec_wrapper(MaterialType *_matptr, assume_ownership_t) : matptr(_matptr)
            {
              ownership = (true);
            }

            virtual ~spec_wrapper()
            {
              if (ownership)
                delete matptr;
            }

            MaterialType *matptr;

            virtual base_wrapper *clone() const final
            {
              return new spec_wrapper {matptr};
            }

            virtual void use() const final
            {
              matptr->use();
            }
            virtual void link_shader() final
            {
              matptr->link_shader();
            }

            virtual void set_transform_feedback_varyings(std::initializer_list<std::string> varyings, GLenum buffer_mode) final
            {
              matptr->set_transform_feedback_varyings(varyings, buffer_mode);
            }

            virtual glm::mat4 *&get_object_matrix() final
            {
              return matptr->template get_variable<variable_indexes::object_matrix>();
            }

            virtual glm::mat4 *&get_vp_matrix() final
            {
              return matptr->template get_variable<variable_indexes::vp_matrix>();
            }
          };
      };

    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_6208373671893816553_11894195__GENERIC_MATERIAL_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

