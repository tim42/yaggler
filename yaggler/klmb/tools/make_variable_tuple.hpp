//
// file : make_material_tuple.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/tools/make_material_tuple.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 31/01/2014 15:22:07
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

#ifndef __N_322291351346493037_1997316684__MAKE_VARIABLE_TUPLE_HPP__
# define __N_322291351346493037_1997316684__MAKE_VARIABLE_TUPLE_HPP__

#include <tools/ref.hpp>
#include <tools/tuple.hpp>
#include <tools/ct_list.hpp>
#include <tools/merge_pack.hpp>
#include <shader/context.hpp>
#include <klmb/material/klmb_context_helper.hpp>

/// \file make_material_tuple.hpp
/// \brief work with variable reference of the form variable<1> in the variable list
///

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      namespace internal
      {
        // Thanks to : stackoverflow.com/q/18366398/
        template<typename...>
        struct variable_filter
        {
          variable_filter() : instance() {}

          using tuple = cr::tuple<>;

          tuple instance;
        };
        template<typename Current, typename... Next>
        struct variable_filter<Current, Next...>
        {
          variable_filter(const Current &, const Next & ... n) : instance(variable_filter<Next...>(n...).instance) {}

          using tuple = typename variable_filter<Next...>::tuple;
          tuple instance;
        };
        template<typename VariableType, typename... Types>
        struct variable_filter<material_ctx_pair<variable<VariableType>>, Types...>
        {
          template<size_t... Idxs>
          variable_filter(const variable<VariableType> &p, cr::seq<Idxs...>, const Types & ... n)
            : instance(p.value, variable_filter<Types...>(n...).instance.template get<Idxs>()...) {}

          variable_filter(const variable<VariableType> &p, const Types & ... n) : variable_filter(p, cr::gen_seq<variable_filter<Types...>::tuple::size()>(), n...) {}

          using tuple =  typename ct::merger<cr::tuple<VariableType>, typename variable_filter<Types...>::tuple>::tuple;
          tuple instance;
        };
        template<typename VariableType, size_t Index, typename... Types>
        struct variable_filter<indexed_variable<VariableType, Index>, Types...>
        {
          template<size_t... Idxs>
          variable_filter(const indexed_variable<VariableType, Index> &p, cr::seq<Idxs...>, const Types & ... n)
          : instance(p.value, variable_filter<Types...>(n...).instance.template get<Idxs>()...) {}

          variable_filter(const indexed_variable<VariableType, Index> &p, const Types & ... n) : variable_filter(p, cr::gen_seq<variable_filter<Types...>::tuple::size()>(), n...) {}

          using tuple = typename ct::merger<cr::tuple<VariableType>, typename variable_filter<Types...>::tuple>::tuple;
          tuple instance;
        };

        // create indexed variables from ctx pairs
        template<typename... Values>
        struct variable_indexer
        {
          private:
            template<size_t Index, typename... Next>
            struct _indexer
            {
              using tuple = cr::tuple<>;
            };

            template<size_t Index, typename Current, typename... Next>
            struct _indexer<Index, Current, Next...>
            {
              using tuple = typename ct::merger<cr::tuple<Current>, typename _indexer<Index, Next...>::tuple>::tuple;
            };

            template<size_t Index, typename VariableType, typename... Next>
            struct _indexer<Index, material_ctx_pair<variable<VariableType>>, Next...>
            {
              using tuple = typename ct::merger<cr::tuple<material_ctx_pair<indexed_variable<VariableType, Index>>>, typename _indexer<Index + 1, Next...>::tuple>::tuple;
            };

          public:
            using tuple = typename _indexer<0, Values...>::tuple;
        };

      } // namespace internal
    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_322291351346493037_1997316684__MAKE_VARIABLE_TUPLE_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

