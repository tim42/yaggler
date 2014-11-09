//
// file : raii_unuser.hpp
// in : file:///home/tim/projects/yaggler/yaggler/raii_unuser.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 23/03/2014 14:10:41
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

#ifndef __N_1295324869188544720_1988340647__RAII_UNUSER_HPP__
# define __N_1295324869188544720_1988340647__RAII_UNUSER_HPP__

#include <tools/ownership.hpp>

namespace neam
{
  namespace yaggler
  {
    template<typename Class>
    class scoped_use
    {
      public:
        scoped_use(const Class &_cl) : cl(_cl)
        {
          cl.use();
        }

        scoped_use(const Class &_cl, neam::assume_ownership_t) : cl(_cl)
        {
        }

        ~scoped_use()
        {
          cl.unuse();
        }

        const Class &cl;
    };

    template<typename Class>
    class scoped_bind
    {
      public:
        scoped_bind(const Class &_cl) : cl(_cl)
        {
          cl.bind();
        }
        scoped_bind(const Class &_cl, neam::assume_ownership_t) : cl(_cl)
        {
        }

        ~scoped_bind()
        {
          cl.unbind();
        }

        const Class &cl;
    };

#define __YAGG_SCOPED_UB(x, t, n)       neam::yaggler::scoped_ ## t <decltype(x)> _n_yagg_u_scu_ ## n (x)
#define _YAGG_SCOPED_USE(x, c)          __YAGG_SCOPED_UB(x, use, c)
#define _YAGG_SCOPED_BIND(x, c)         __YAGG_SCOPED_UB(x, bind, c)

#define YAGG_SCOPED_USE(x)              _YAGG_SCOPED_USE(x, __COUNTER__)
#define YAGG_SCOPED_BIND(x)             _YAGG_SCOPED_BIND(x, __COUNTER__)


  } // namespace yaggler
} // namespace neam

#endif /*__N_1295324869188544720_1988340647__RAII_UNUSER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

