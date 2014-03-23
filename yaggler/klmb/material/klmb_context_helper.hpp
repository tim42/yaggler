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
      // the material's shader context key/value 'pair'
      template<typename Value>
      struct material_ctx_pair
      {
        using value_t = Value;

        material_ctx_pair(const std::string &name, const Value &val, bool buffer = false) : variable_name(name), value(val), buffer_storage(buffer) {}
        material_ctx_pair(std::string &&name, const Value &val, bool buffer = false) : variable_name(name), value(val), buffer_storage(buffer) {}
        material_ctx_pair(const std::string &name, Value &&val, bool buffer = false) : variable_name(name), value(val), buffer_storage(buffer) {}
        material_ctx_pair(std::string &&name, Value &&val, bool buffer = false) : variable_name(name), value(val), buffer_storage(buffer) {}

        template<typename OtherValue>
        material_ctx_pair(const material_ctx_pair<OtherValue> &o) : variable_name(o.variable_name), value(o.value), buffer_storage(o.buffer_storage) {}

        template<typename OtherValue>
        material_ctx_pair(material_ctx_pair<OtherValue> &&o) : variable_name(std::move(o.variable_name)), value(std::move(o.value)), buffer_storage(o.buffer_storage) {}

        std::string variable_name;
        value_t value;
        bool buffer_storage;
      };

      // a maker (for uniform variable)
      template<typename Value>
      material_ctx_pair<Value> make_ctx_pair(const std::string &name, Value val)
      {
        return material_ctx_pair<Value>(name, std::move(val), false);
      }
      template<typename Value>
      material_ctx_pair<Value> make_ctx_pair(std::string &&name, Value val)
      {
        return material_ctx_pair<Value>(std::move(name), std::move(val), false);
      }
      // for uniform buffer
      template<typename Value>
      material_ctx_pair<Value> make_ctx_buffer_pair(const std::string &name, Value val)
      {
        return material_ctx_pair<Value>(name, std::move(val), true);
      }
      template<typename Value>
      material_ctx_pair<Value> make_ctx_buffer_pair(std::string &&name, Value val)
      {
        return material_ctx_pair<Value>(std::move(name), std::move(val), true);
      }


      // reference a texture in the material's shader context
      // will be replaced by a real (C++) reference to the texture.
      template<size_t Index>
      struct reference_to_texture
      {
      };

      // create a variable a reference it in the context
      // you can always change its value later in the material.
      template<typename VariableType>
      struct variable
      {
        variable(const VariableType &_default_value)
          : value(_default_value)
        {
        }

        using reference_t = cr::ref<VariableType>;

        VariableType value;
      };

      template<typename VariableType, size_t Index>
      struct indexed_variable
      {
        indexed_variable(const variable<VariableType> &_default_value)
          : value(_default_value.value)
        {
        }

        using reference_t = cr::ref<VariableType>;
        using variable_t = VariableType;
        static constexpr size_t index = Index;

        VariableType value;
      };
    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_10235510111851740554_136762483__KLMB_CONTEXT_HELPER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

