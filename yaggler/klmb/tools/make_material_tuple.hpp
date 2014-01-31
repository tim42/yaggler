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

#ifndef __N_322290351346493037_1997316684__MAKE_MATERIAL_TUPLE_HPP__
# define __N_322290351346493037_1997316684__MAKE_MATERIAL_TUPLE_HPP__

#include <tools/ref.hpp>
#include <tools/tuple.hpp>
#include <tools/ct_list.hpp>
#include <shader/context.hpp>
#include <klmb/klmb_context_helper.hpp>

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      namespace internal
      {
        // make ctx, return 'the tuple' type
        // This is needed to make liaison between textures/shader uniforms
        // -- Thanks to : stackoverflow.com/q/18366398
        template <typename Textures, typename Cr, typename Type> struct make_type;

        // not a texture reference
        template <typename Textures, typename Cr, typename ...Others>
        struct make_type<Textures, Cr, cr::tuple<Others...>>
        {
          using type = cr::tuple<typename std::remove_reference<Cr>::type, Others...>;
        };

        // a texture reference
        template <typename Textures, size_t TextureIndex, typename ...Others>
        struct make_type<Textures, texture_reference<TextureIndex>, cr::tuple<Others...>>
        {
          using _texture_ref_type = cr::ref<typename Textures::template get_type<TextureIndex>>;

          using type = cr::tuple<typename std::remove_reference<_texture_ref_type>::type, Others...>;
        };

        template<typename Textures, typename...>
        struct make_tuple
        {
          using type = cr::tuple<>;
        };
        template<typename Textures, typename Current, typename... Types>
        struct make_tuple<Textures, Current, Types...>
        {
          using type = typename make_type<Textures, Current, typename make_tuple<Textures, Types...>::type>::type;
        };

        // create the values for the tuple

        // not a texture reference
        template<typename Textures, typename Type>
        struct get_type_instance
        {
          static const Type &return_instance(const Textures &, const Type &instance)
          {
            return instance;
          }
        };

        // a texture reference
        template<typename Textures, size_t TextureIndex>
        struct get_type_instance<Textures, texture_reference<TextureIndex>>
        {
          using _texture_ref_type = cr::ref<typename Textures::template get_type<TextureIndex>>;

          static _texture_ref_type return_instance(Textures &textures, const texture_reference<TextureIndex> &)
          {
            return _texture_ref_type(textures.instance.template get_ref<TextureIndex>());
          }
        };

        template<typename... Ts>
        using contex_base_t = neam::yaggler::shader::variable_context<neam::yaggler::shader::contexts::fixed, Ts...>;

        template<typename Textures, typename... TupleArgs>
        using context_t = typename ct::extract_types<contex_base_t, typename make_tuple<Textures, typename TupleArgs::value_t...>::type>::type;
      } // namespace internal
    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_322290351346493037_1997316684__MAKE_MATERIAL_TUPLE_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

