//
// file : object.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/object/object.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 13/02/2014 16:40:11
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

#ifndef __N_115111457392076053_1707192587__OBJECT_HPP__
# define __N_115111457392076053_1707192587__OBJECT_HPP__

#include <deque>

#include <geometry/opengl_vao.hpp>
#include <geometry/opengl_draw_state.hpp>
#include <geometry/buffer_base.hpp>

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      // an object simply holds a VAO + buffers + drawer
      // there's no material here/position/... . It simply holds the geom / drawer
      template<GLenum... CTBufferTypes>
      struct object
      {
        public:
          // the vao
          neam::yaggler::geometry::vao<neam::yaggler::type::opengl> vao;

          // the drawer
          neam::yaggler::geometry::draw_state<neam::yaggler::type::opengl> drawer;

          // the ct buffers
          cr::tuple<neam::yaggler::geometry::buffer<neam::yaggler::type::opengl, embed::GLenum<CTBufferTypes>>...> ct_buffers;

          template<GLenum BufferType>
          using _sub_container_type = std::deque<neam::yaggler::geometry::buffer<neam::yaggler::type::opengl, embed::GLenum<BufferType>>>;

          template<GLenum... BufferTypes>
          using _container_tuple = cr::tuple<_sub_container_type<BufferTypes>...>;

          // some other buffers, possibly added at runtime.
          // even if for some of them, it makes no sense in using them in an object ;)
          // 0:  GL_ARRAY_BUFFER,
          // 1:  GL_ATOMIC_COUNTER_BUFFER,
          // 2:  GL_COPY_READ_BUFFER,
          // 3:  GL_COPY_WRITE_BUFFER,
          // 4:  GL_DRAW_INDIRECT_BUFFER,
          // 5:  GL_DISPATCH_INDIRECT_BUFFER,
          // 6:  GL_ELEMENT_ARRAY_BUFFER,
          // 7:  GL_PIXEL_PACK_BUFFER,
          // 8:  GL_PIXEL_UNPACK_BUFFER,
          // 9:  GL_QUERY_BUFFER,
          // 10: GL_SHADER_STORAGE_BUFFER,
          // 11: GL_TEXTURE_BUFFER,
          // 12: GL_TRANSFORM_FEEDBACK_BUFFER,
          // 13: GL_UNIFORM_BUFFER
          _container_tuple<GL_ARRAY_BUFFER, GL_ATOMIC_COUNTER_BUFFER, GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, GL_DRAW_INDIRECT_BUFFER, GL_DISPATCH_INDIRECT_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_PIXEL_PACK_BUFFER, GL_PIXEL_UNPACK_BUFFER, GL_QUERY_BUFFER, GL_SHADER_STORAGE_BUFFER, GL_TEXTURE_BUFFER, GL_TRANSFORM_FEEDBACK_BUFFER, GL_UNIFORM_BUFFER> buffers;

          static constexpr GLenum __indexes_container[] = {GL_ARRAY_BUFFER, GL_ATOMIC_COUNTER_BUFFER, GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, GL_DRAW_INDIRECT_BUFFER, GL_DISPATCH_INDIRECT_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_PIXEL_PACK_BUFFER, GL_PIXEL_UNPACK_BUFFER, GL_QUERY_BUFFER, GL_SHADER_STORAGE_BUFFER, GL_TEXTURE_BUFFER, GL_TRANSFORM_FEEDBACK_BUFFER, GL_UNIFORM_BUFFER};

          // to help with indexes
          static constexpr size_t get_index_for_enum(GLenum val)
          {
            return _rec_get_index_for_enum(val, 0, sizeof(__indexes_container) / sizeof(__indexes_container[0]));
          }

        public:
          using rt_idxs_gen_seq = cr::gen_seq<14>;

          // constructors
          object() {}

          // do not use those two.
          explicit object(neam::yaggler::geometry::vao<neam::yaggler::type::opengl> &&_vao) : vao(std::move(_vao)) {}
          explicit object(const neam::yaggler::geometry::vao<neam::yaggler::type::opengl> &_vao) : vao((_vao)) {}

          // create a link
          object(const object &o)
            : vao(o.vao), drawer(o.drawer), ct_buffers(o.ct_buffers), buffers(o.buffers)
          {
          }
          // stole ownership
          object(object &o, stole_ownership_t)
            : object(std::move(o), cr::gen_seq<sizeof...(CTBufferTypes)>(), rt_idxs_gen_seq())
          {
          }
          object(object && o)
            : object(std::move(o), cr::gen_seq<sizeof...(CTBufferTypes)>(), rt_idxs_gen_seq())
          {
          }

          // std ownership management
          object &give_up_ownership()
          {
            vao.give_up_ownership();
            ct_buffers_give_up_ownership(cr::gen_seq<sizeof...(CTBufferTypes)>());
            buffers_give_up_ownership(rt_idxs_gen_seq());
            return *this;
          }
          object &assume_ownership()
          {
            vao.assume_ownership();
            ct_buffers_assume_ownership(cr::gen_seq<sizeof...(CTBufferTypes)>());
            buffers_assume_ownership(rt_idxs_gen_seq());
            return *this;
          }

          object &stole(object &o)
          {
            vao.stole(o.vao);
            ct_buffers_stole(o, cr::gen_seq<sizeof...(CTBufferTypes)>());
            buffers_stole(o, rt_idxs_gen_seq());
            return *this;
          }

          object &link_to(const object &o)
          {
            vao.link_to(o.vao);
            ct_buffers_link_to(o, cr::gen_seq<sizeof...(CTBufferTypes)>());
            buffers_link_to(o, rt_idxs_gen_seq());
            return *this;
          }

          // draw the object
          void draw() const
          {
            vao.use();
            drawer.draw();
          }

          // this function is soooo cool... :)
          // NOTE: the returned object will be the owner of the vao and buffers
          object<> convert_to_generic()
          {
            return _convert_to_generic(cr::gen_seq<sizeof...(CTBufferTypes)>());
          }

          // NOTE: the returned object will be the owner of the vao and buffers
          object<> create_generic_link() const
          {
            return _create_generic_link(cr::gen_seq<sizeof...(CTBufferTypes)>());
          }

        private: // helpers
          static constexpr size_t _rec_get_index_for_enum(GLenum val, size_t idx, size_t max_idx)
          {
            return __indexes_container[idx] == val ? idx : (idx + 1 < max_idx ? _rec_get_index_for_enum(val, idx + 1, max_idx) : max_idx + 1);
          }

          template<size_t... CTIdxs>
          object<> _convert_to_generic(cr::seq<CTIdxs...>)
          {
            object<> gen(std::move(vao));

            gen.drawer = drawer;

            // copy 'runtime' buffers
            gen.___gen_buffers_stole(*this, rt_idxs_gen_seq());

            // move 'ct' buffers to 'runtime' (non-ct) buffers
            void((int []){(gen.buffers.template get_ref<get_index_for_enum(CTBufferTypes)>().emplace_back( std::move(ct_buffers.template get_ref<CTIdxs>())), 5)...});

            return gen;
          }
          template<size_t... CTIdxs>
          object<> _create_generic_link(cr::seq<CTIdxs...>) const
          {
            object<> gen((vao));

            gen.drawer = drawer;

            // 'link' 'runtime' buffers
            gen.___gen_buffers_link(*this, rt_idxs_gen_seq());

            // 'link' 'ct' buffers as 'runtime' (non-ct) buffers
            void((int []){(gen.buffers.template get_ref<get_index_for_enum(CTBufferTypes)>().emplace_back((ct_buffers.template get<CTIdxs>())), 5)...});

            return gen;
          }

        public:
          template<GLenum... CTArgs, size_t... Idxs>
          void ___gen_buffers_stole(object<CTArgs...> &o, cr::seq<Idxs...>)
          {
            void((int []){((buffers.template get_ref<Idxs>() = std::move(o.buffers.template get_ref<Idxs>())), 5)...}); // who knows how this'll be optimised out ?
            // (and which compiler supports it...)
          }
          template<GLenum... CTArgs, size_t... Idxs>
          void ___gen_buffers_link(const object<CTArgs...> &o, cr::seq<Idxs...>)
          {
            void((int []){((___gen_buffers_link_sub<Idxs>(o)), 5)...}); // who knows how this'll be optimised out ?
            // (and which compiler supports it...)
          }

        private:
          template<size_t Idx, GLenum... CTArgs>
          void ___gen_buffers_link_sub(const object<CTArgs...> &o)
          {
            auto &ref = buffers.get_ref<Idx>();
            const auto &oref = o.buffers.template get<Idx>();

            ref.clear();
            // ref.reserve(oref.size());
            for (const auto &it : oref)
            {
              ref.push_back(it);
            }
          }


          // ownership thief
          template<size_t... CTIdxs, size_t... RTIdxs>
          object(object && o, cr::seq<CTIdxs...>, cr::seq<RTIdxs...>)
            : vao(o.vao, stole_ownership), drawer(o.drawer),
              ct_buffers(cr::make_move((o.ct_buffers.template get_ref<CTIdxs>()))...),
              buffers(cr::make_move(o.buffers.template get_ref<RTIdxs>())...)
          {
          }

          template<size_t... Idxs>
          void ct_buffers_give_up_ownership(cr::seq<Idxs...>)
          {
            void((int []){(ct_buffers.template get_ref<Idxs>().give_up_ownership(), 5)...}); // who knows how this'll be optimised out ?
            // (and which compiler supports it...)
          }
          template<size_t... Idxs>
          void ct_buffers_assume_ownership(cr::seq<Idxs...>)
          {
            void((int []){(ct_buffers.template get_ref<Idxs>().assume_ownership(), 5)...}); // who knows how this'll be optimised out ?
            // (and which compiler supports it...)
          }
          template<size_t... Idxs>
          void ct_buffers_stole(object &o, cr::seq<Idxs...>)
          {
            void((int []){(ct_buffers.template get_ref<Idxs>().stole(o.ct_buffers.template get_ref<Idxs>()), 5)...}); // who knows how this'll be optimised out ?
            // (and which compiler supports it...)
          }
          template<size_t... Idxs>
          void ct_buffers_link_to(const object &o, cr::seq<Idxs...>)
          {
            void((int []){(ct_buffers.template get_ref<Idxs>().link_to(o.ct_buffers.template get<Idxs>()), 5)...}); // who knows how this'll be optimised out ?
            // (and which compiler supports it...)
          }

          template<size_t... Idxs>
          void buffers_give_up_ownership(cr::seq<Idxs...>)
          {
            void((int []){(sub_buffers_give_up_ownership<Idxs>(), 5)...}); // who knows how this'll be optimised out ?
            // (and which compiler supports it...)
          }
          template<size_t Idx>
          void sub_buffers_give_up_ownership()
          {
            auto &ref = buffers.get_ref<Idx>();
            for (auto &it : ref)
              it.give_up_ownership();
          }

          template<size_t... Idxs>
          void buffers_assume_ownership(cr::seq<Idxs...>)
          {
            void((int []){(sub_buffers_assume_ownership<Idxs>(), 5)...}); // who knows how this'll be optimised out ?
            // (and which compiler supports it...)
          }
          template<size_t Idx>
          void sub_buffers_assume_ownership()
          {
            auto &ref = buffers.get_ref<Idx>();
            for (auto &it : ref)
              it.assume_ownership();
          }

          template<size_t... Idxs>
          void buffers_stole(object &o, cr::seq<Idxs...>)
          {
            void((int []){((buffers.template get_ref<Idxs>() = std::move(o.buffers.template get_ref<Idxs>())), 5)...}); // who knows how this'll be optimised out ?
            // (and which compiler supports it...)
          }
          template<size_t... Idxs>
          void buffers_link_to(const object &o, cr::seq<Idxs...>)
          {
            void((int []){((sub_buffers_link_to<Idxs>(o)), 5)...}); // who knows how this'll be optimised out ?
            // (and which compiler supports it...)
          }
          template<size_t Idx>
          void sub_buffers_link_to(const object &o)
          {
            auto &ref = buffers.get_ref<Idx>();
            const auto &oref = o.buffers.template get<Idx>();

            ref.clear();
            // ref.reserve(oref.size());
            for (const auto &it : oref)
            {
              ref.push_back(it);
            }
          }
      };

    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_115111457392076053_1707192587__OBJECT_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

