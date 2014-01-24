//
// file : context.hpp
// in : file:///home/tim/projects/yaggler/yaggler/shader/context.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 16/12/2013 11:36:32
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


#ifndef __N_17818792821539412134_1983288025__CONTEXT_HPP__
# define __N_17818792821539412134_1983288025__CONTEXT_HPP__
#include <initializer_list>
#include "opengl_uniform_var.hpp"
#include <tools/tuple.hpp>
#include <tools/enable_if.hpp>
#include <tools/function.hpp>
#include <tools/array.hpp>

namespace neam
{
  namespace yaggler
  {
    namespace shader
    {
      // context types
      namespace contexts
      {
        class dynamic {};
        class fixed {};
        class none {};
      } // namespace contexts

      // aim to create a context for vars (and avoid to set them again and again)
      template<typename Type, typename... Args> class variable_context
      {
          // create an error message
//         static_assert(!(sizeof...(Args) + 1), "This context type doesn't exist or is not valid.");

          // construct a new static context (this use arg deduction)
          // "disable" this class (to kill even more the user under endless error messages).
          variable_context() = delete;
          ~variable_context() = delete;

        public:
          template<typename CTXType, typename... NArgs, typename... Vars>
          constexpr static variable_context<CTXType, NArgs...> create(neam::cr::tuple<NArgs...> && _values, Vars && ... _vars)
          {
            static_assert((sizeof...(NArgs)) == (sizeof...(Vars)), "mismatching values number and uniform_variable number.");
            return variable_context<CTXType, NArgs...>(_values, _vars...);
          }
      };

      // context impl

      // the context will could not be changed.
      // Args... are 'embeds' (see neam::embed::embed) or functions
      // (embeded functions will be called to get the result, each time 'use()' is called)
      //
      // This class could be seen as a 'type saver'
      template<typename... Args>
      class variable_context<contexts::fixed, Args...>
      {
        public:
          template<typename... Vars>
          constexpr variable_context(const neam::cr::tuple<Args...> &_values, Vars &&... _vars) : vars{{_vars...}}, values(_values)
          {
            static_assert((sizeof...(Args)) == (sizeof...(Vars)), "mismatching values number and uniform_variable number.");
          }

          // do the binding
          void use()
          {
            binder(cr::gen_seq<sizeof...(Args)>());
          }

        private:
          template<size_t... Idxs>
          void binder(neam::cr::seq<Idxs...>&&)
          {
            void((char []){_binder<Idxs, Args>(0)...}); // who knows how this'll be optimised out ?
            // (and which compiler supports it...)
//             char _[] __attribute__((unused)) = {_binder<Idxs, Args>(0)...};
          }

          // on functions/functors
          template<size_t Idx, typename Arg>
          char _binder(typename neam::cr::enable_if < neam::cr::is_function_pointer<Arg>::value || std::is_member_function_pointer<Arg>::value, __LINE__, size_t >::type)
          {
            vars[Idx] = (values.template get<Idx>())();
            return 0;
          }

          // on a classic var (use affectation directly)
          template<size_t Idx, typename Arg>
          char _binder(typename neam::cr::enable_if < !neam::cr::is_function_pointer<Arg>::value && !std::is_member_function_pointer<Arg>::value, __LINE__, size_t >::type)
          {
            vars[Idx] = (values.template get<Idx>());
            return 0;
          }

        private:
          neam::cr::array<uniform_variable, sizeof...(Args)> vars;
          neam::cr::tuple<Args...> values;
      };
    } // namespace shader
  } // namespace yaggler
} // namespace neam

#endif /*__N_17818792821539412134_1983288025__CONTEXT_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

