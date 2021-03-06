//
// file : klmb_context_helper.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/klmb_context_helper.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 31/01/2014 14:23:18
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

#ifndef __N_10235510111851740554_136762483__KLMB_CONTEXT_HELPER_HPP__
# define __N_10235510111851740554_136762483__KLMB_CONTEXT_HELPER_HPP__
#include <string>
#include <tools/ref.hpp>

// some types/helpers for shader contexts, mostly used in K:LMB materials
namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      /// \brief the material's shader context key/value 'pair'
      /// \see make_ctx_pair()
      template<typename Value>
      struct material_ctx_pair
      {
        using value_t = Value; ///< \brief the value type

        /// \brief Initialize the ctx_pair
        material_ctx_pair(const std::string &name, const Value &val, GLenum _resource_type = GL_UNIFORM) : variable_name(name), value(val), resource_type(_resource_type) {}

        /// \brief Initialize the ctx_pair
        material_ctx_pair(std::string &&name, const Value &val, GLenum _resource_type = GL_UNIFORM) : variable_name(name), value(val), resource_type(_resource_type) {}

        /// \brief Initialize the ctx_pair
        material_ctx_pair(const std::string &name, Value &&val, GLenum _resource_type = GL_UNIFORM) : variable_name(name), value(val), resource_type(_resource_type) {}

        /// \brief Initialize the ctx_pair
        material_ctx_pair(std::string &&name, Value &&val, GLenum _resource_type = GL_UNIFORM) : variable_name(name), value(val), resource_type(_resource_type) {}

        /// \brief Initialize by copy the ctx_pair
        template<typename OtherValue>
        material_ctx_pair(const material_ctx_pair<OtherValue> &o) : variable_name(o.variable_name), value(o.value), resource_type(o.resource_type) {}

        /// \brief Initialize by move the ctx_pair
        template<typename OtherValue>
        material_ctx_pair(material_ctx_pair<OtherValue> &&o) : variable_name(std::move(o.variable_name)), value(std::move(o.value)), resource_type(o.resource_type) {}

        std::string variable_name;
        value_t value;
        GLenum resource_type;
      };

      /// \brief a material_ctx_pair maker for uniform variable
      template<typename Value>
      material_ctx_pair<Value> make_ctx_pair(const std::string &name, Value val)
      {
        return material_ctx_pair<Value>(name, std::move(val), GL_UNIFORM);
      }

      /// \brief a material_ctx_pair maker for uniform variable
      template<typename Value>
      material_ctx_pair<Value> make_ctx_pair(std::string &&name, Value val)
      {
        return material_ctx_pair<Value>(std::move(name), std::move(val), GL_UNIFORM);
      }

      /// \brief a material_ctx_pair maker for uniform buffer
      template<typename Value>
      material_ctx_pair<Value> make_ctx_buffer_pair(const std::string &name, Value val, GLenum resource_type = GL_UNIFORM_BLOCK)
      {
        return material_ctx_pair<Value>(name, std::move(val), resource_type);
      }

      /// \brief a material_ctx_pair maker for uniform buffer
      template<typename Value>
      material_ctx_pair<Value> make_ctx_buffer_pair(std::string &&name, Value val, GLenum resource_type = GL_UNIFORM_BLOCK)
      {
        return material_ctx_pair<Value>(std::move(name), std::move(val), resource_type);
      }


      /// \brief reference a texture in the material's shader context
      /// will be replaced by a real (C++) reference to the texture.
      template<size_t Index>
      struct reference_to_texture
      {
      };

      /// \brief create a variable and reference it in the context
      /// you can always change its value later in the material.
      /// \see material::get_variable()
      template<typename VariableType>
      struct variable
      {
        /// \brief Initialize the variable from its default value
        variable(const VariableType &_default_value)
          : value(_default_value)
        {
        }

        using reference_t = cr::ref<VariableType>;

        VariableType value;
      };

      /// \brief create a reference to a variable<>
      /// \see class variable
      template<typename VariableType, size_t Index>
      struct indexed_variable
      {
        /// \brief Initialize the indexed_variable from another variable
        indexed_variable(const variable<VariableType> &_default_value)
          : value(_default_value.value)
        {
        }

        using reference_t = cr::ref<VariableType>;
        using variable_t = VariableType;
        static constexpr size_t index = Index;

        // FIXME shouldn't this be a cr::ref<> ?
        //       (combined with an operator VariableType () {return value.value;} )
        VariableType value;
      };
    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_10235510111851740554_136762483__KLMB_CONTEXT_HELPER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

