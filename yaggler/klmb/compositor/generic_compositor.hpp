//
// file : generic_compositor.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/compositor/generic_compositor.hpp
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

#ifndef __N_6208373671893816553_11894195__GENERIC_COMPOSITOR_HPP__
# define __N_6208373671893816553_11894195__GENERIC_COMPOSITOR_HPP__

#include <glm/glm.hpp>
#include <yaggler_except.hpp>
#include <tools/ownership.hpp>

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      /// \brief wraps the complicated compositor class in a more generic one (speaking of C++ types)
      class compositor_wrapper
      {
        public:
          /// \brief destructor
          ~compositor_wrapper()
          {
            delete wrapper;
          }

          /// \brief Copy another compositor_wrapper but don't manage the lifetime of the wrapped compositor
          compositor_wrapper(const compositor_wrapper &mw) : wrapper(mw.wrapper ? mw.wrapper->clone() : nullptr) {}

          /// \brief Move another compositor_wrapper AND manage the lifetime of the wrapped compositor
          /// \note The moved compositor_wraper will be emptied
          compositor_wrapper(compositor_wrapper &&mw) : wrapper(mw.wrapper)
          {
            mw.wrapper = nullptr;
          }

          /// \brief default constructor, initialize with an empty compositor
          compositor_wrapper() : wrapper(nullptr) {}

          /// \brief wrap a compositor, but don't manage its lifetime
          template<typename CompositorType>
          compositor_wrapper(CompositorType &mt) : wrapper(new spec_wrapper<CompositorType>(&mt)) {}

          /// \brief wrap a compositor, but don't manage its lifetime
          template<typename CompositorType>
          compositor_wrapper(CompositorType *mt) : wrapper(new spec_wrapper<CompositorType>(mt)) {}

          /// \brief wrap a compositor and manage its lifetime
          template<typename CompositorType>
          compositor_wrapper(CompositorType *mt, assume_ownership_t) : wrapper(new spec_wrapper<CompositorType>(mt, neam::assume_ownership)) {}

          /// \brief wrap a compositor and manage its lifetime
          template<typename CompositorType>
          compositor_wrapper(CompositorType &&mt) : wrapper(new spec_wrapper<CompositorType>(new CompositorType(std::move(mt)), neam::assume_ownership)) {}

          /// \brief Copy another compositor_wrapper but don't manage the lifetime of the wrapped compositor
          compositor_wrapper &operator = (const compositor_wrapper &mw)
          {
            if (&mw != this)
            {
              if (wrapper)
                delete wrapper;
              wrapper = (mw.wrapper ? mw.wrapper->clone() : nullptr);
            }
            return *this;
          }

          /// \brief Move another compositor_wrapper AND manage the lifetime of the wrapped compositor
          /// \note The moved compositor_wraper will be emptied
          compositor_wrapper &operator = (compositor_wrapper &&mw)
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
          compositor_wrapper &give_up_ownership()
          {
            if (wrapper)
              wrapper->ownership = false;
            return *this;
          }

          compositor_wrapper &assume_ownership()
          {
            if (wrapper)
              wrapper->ownership = true;
            return *this;
          }

          /// \brief check whether the wrapper wrap a compositor or not
          bool is_empty() const
          {
            return !wrapper;
          }

          /// \brief render the compositor
          void render()
          {
            if (wrapper)
              wrapper->render();
          }

        private:
          struct base_wrapper
          {
            virtual ~base_wrapper() {}

            virtual base_wrapper *clone() const = 0;

            virtual void render() = 0;


            bool ownership = false;
          } *wrapper;

          template<typename CompositorType>
          struct spec_wrapper : base_wrapper
          {
            spec_wrapper(CompositorType *_compptr) : compptr(_compptr) {}
            spec_wrapper(CompositorType *_compptr, assume_ownership_t) : compptr(_compptr)
            {
              ownership = (true);
            }

            virtual ~spec_wrapper()
            {
              if (ownership)
                delete compptr;
            }

            CompositorType *compptr;

            virtual base_wrapper *clone() const final
            {
              return new spec_wrapper(compptr);
            }

            virtual void render() final
            {
              compptr->render();
            }
          };
      };

    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_6208373671893816553_11894195__GENERIC_COMPOSITOR_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

