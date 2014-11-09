//
// file : ct_point_udl.hpp
// in : file:///home/tim/projects/yaggler/yaggler/ct_point_udl.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 27/01/2014 11:49:13
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

#ifndef __N_1814989461818831215_234927224__CT_POINT_UDL_HPP__
# define __N_1814989461818831215_234927224__CT_POINT_UDL_HPP__

#include "ct_point.hpp"

// ct::vector* constexpr user defined literals
namespace neam
{
  namespace ct
  {
    // 1D vector
    static inline constexpr fixed_t operator "" _fixed(long double val)
    {
      return conversion::from(val);
    }
    static inline constexpr fixed_t operator "" _fixed(unsigned long long val)
    {
      return conversion::from(val);
    }

    // 2D vector
    static inline constexpr vector2 operator "" _2d_x(long double val)
    {
      return vector2(conversion::from(val), conversion::from(0));
    }
    static inline constexpr vector2 operator "" _2d_x(unsigned long long val)
    {
      return vector2(conversion::from(val), conversion::from(0));
    }

    static inline constexpr vector2 operator "" _2d_y(long double val)
    {
      return vector2(conversion::from(0), conversion::from(val));
    }
    static inline constexpr vector2 operator "" _2d_y(unsigned long long val)
    {
      return vector2(conversion::from(0), conversion::from(val));
    }

    static inline constexpr vector2 operator "" _2d_xy(long double val)
    {
      return vector2(conversion::from(val), conversion::from(val));
    }
    static inline constexpr vector2 operator "" _2d_xy(unsigned long long val)
    {
      return vector2(conversion::from(val), conversion::from(val));
    }

    // 3D vector
    static inline constexpr vector3 operator "" _3d_x(long double val)
    {
      return vector3(conversion::from(val), conversion::from(0), conversion::from(0));
    }
    static inline constexpr vector3 operator "" _3d_x(unsigned long long val)
    {
      return vector3(conversion::from(val), conversion::from(0), conversion::from(0));
    }

    static inline constexpr vector3 operator "" _3d_y(long double val)
    {
      return vector3(conversion::from(0), conversion::from(val), conversion::from(0));
    }
    static inline constexpr vector3 operator "" _3d_y(unsigned long long val)
    {
      return vector3(conversion::from(0), conversion::from(val), conversion::from(0));
    }

    static inline constexpr vector3 operator "" _3d_z(long double val)
    {
      return vector3(conversion::from(0), conversion::from(0), conversion::from(val));
    }
    static inline constexpr vector3 operator "" _3d_z(unsigned long long val)
    {
      return vector3(conversion::from(0), conversion::from(0), conversion::from(val));
    }

    static inline constexpr vector3 operator "" _3d_xy(long double val)
    {
      return vector3(conversion::from(val), conversion::from(val), conversion::from(0));
    }
    static inline constexpr vector3 operator "" _3d_xy(unsigned long long val)
    {
      return vector3(conversion::from(val), conversion::from(val), conversion::from(0));
    }

    static inline constexpr vector3 operator "" _3d_xz(long double val)
    {
      return vector3(conversion::from(val), conversion::from(0), conversion::from(val));
    }
    static inline constexpr vector3 operator "" _3d_xz(unsigned long long val)
    {
      return vector3(conversion::from(val), conversion::from(0), conversion::from(val));
    }

    static inline constexpr vector3 operator "" _3d_yz(long double val)
    {
      return vector3(conversion::from(0), conversion::from(val), conversion::from(val));
    }
    static inline constexpr vector3 operator "" _3d_yz(unsigned long long val)
    {
      return vector3(conversion::from(0), conversion::from(val), conversion::from(val));
    }

    static inline constexpr vector3 operator "" _3d_xyz(long double val)
    {
      return vector3(conversion::from(val), conversion::from(val), conversion::from(val));
    }
    static inline constexpr vector3 operator "" _3d_xyz(unsigned long long val)
    {
      return vector3(conversion::from(val), conversion::from(val), conversion::from(val));
    }

    // 4D vector
    static inline constexpr vector4 operator "" _4d_x(long double val)
    {
      return vector4(conversion::from(val), conversion::from(0), conversion::from(0));
    }
    static inline constexpr vector4 operator "" _4d_x(unsigned long long val)
    {
      return vector4(conversion::from(val), conversion::from(0), conversion::from(0));
    }

    static inline constexpr vector4 operator "" _4d_y(long double val)
    {
      return vector4(conversion::from(0), conversion::from(val), conversion::from(0));
    }
    static inline constexpr vector4 operator "" _4d_y(unsigned long long val)
    {
      return vector4(conversion::from(0), conversion::from(val), conversion::from(0));
    }

    static inline constexpr vector4 operator "" _4d_z(long double val)
    {
      return vector4(conversion::from(0), conversion::from(0), conversion::from(val));
    }
    static inline constexpr vector4 operator "" _4d_z(unsigned long long val)
    {
      return vector4(conversion::from(0), conversion::from(0), conversion::from(val));
    }

    static inline constexpr vector4 operator "" _4d_w(long double val)
    {
      return vector4(conversion::from(0), conversion::from(0), conversion::from(0), conversion::from(val));
    }
    static inline constexpr vector4 operator "" _4d_w(unsigned long long val)
    {
      return vector4(conversion::from(0), conversion::from(0), conversion::from(0), conversion::from(val));
    }

    ///

    static inline constexpr vector4 operator "" _4d_xy(long double val)
    {
      return vector4(conversion::from(val), conversion::from(val), conversion::from(0));
    }
    static inline constexpr vector4 operator "" _4d_xy(unsigned long long val)
    {
      return vector4(conversion::from(val), conversion::from(val), conversion::from(0));
    }

    static inline constexpr vector4 operator "" _4d_xz(long double val)
    {
      return vector4(conversion::from(val), conversion::from(0), conversion::from(val));
    }
    static inline constexpr vector4 operator "" _4d_xz(unsigned long long val)
    {
      return vector4(conversion::from(val), conversion::from(0), conversion::from(val));
    }

    static inline constexpr vector4 operator "" _4d_yz(long double val)
    {
      return vector4(conversion::from(0), conversion::from(val), conversion::from(val));
    }
    static inline constexpr vector4 operator "" _4d_yz(unsigned long long val)
    {
      return vector4(conversion::from(0), conversion::from(val), conversion::from(val));
    }

    static inline constexpr vector4 operator "" _4d_xw(long double val)
    {
      return vector4(conversion::from(val), conversion::from(0), conversion::from(0), conversion::from(val));
    }
    static inline constexpr vector4 operator "" _4d_xw(unsigned long long val)
    {
      return vector4(conversion::from(val), conversion::from(0), conversion::from(0), conversion::from(val));
    }

    static inline constexpr vector4 operator "" _4d_yw(long double val)
    {
      return vector4(conversion::from(0), conversion::from(val), conversion::from(0), conversion::from(val));
    }
    static inline constexpr vector4 operator "" _4d_yw(unsigned long long val)
    {
      return vector4(conversion::from(0), conversion::from(val), conversion::from(0), conversion::from(val));
    }

    static inline constexpr vector4 operator "" _4d_zw(long double val)
    {
      return vector4(conversion::from(0), conversion::from(0), conversion::from(val), conversion::from(val));
    }
    static inline constexpr vector4 operator "" _4d_zw(unsigned long long val)
    {
      return vector4(conversion::from(0), conversion::from(0), conversion::from(val), conversion::from(val));
    }

    ///
    static inline constexpr vector4 operator "" _4d_xyz(long double val)
    {
      return vector4(conversion::from(val), conversion::from(val), conversion::from(val));
    }
    static inline constexpr vector4 operator "" _4d_xyz(unsigned long long val)
    {
      return vector4(conversion::from(val), conversion::from(val), conversion::from(val));
    }

    static inline constexpr vector4 operator "" _4d_yzw(long double val)
    {
      return vector4(conversion::from(0), conversion::from(val), conversion::from(val), conversion::from(val));
    }
    static inline constexpr vector4 operator "" _4d_yzw(unsigned long long val)
    {
      return vector4(conversion::from(0), conversion::from(val), conversion::from(val), conversion::from(val));
    }

    static inline constexpr vector4 operator "" _4d_xzw(long double val)
    {
      return vector4(conversion::from(val), conversion::from(0), conversion::from(val), conversion::from(val));
    }
    static inline constexpr vector4 operator "" _4d_xzw(unsigned long long val)
    {
      return vector4(conversion::from(val), conversion::from(0), conversion::from(val), conversion::from(val));
    }

    static inline constexpr vector4 operator "" _4d_xyw(long double val)
    {
      return vector4(conversion::from(val), conversion::from(val), conversion::from(0), conversion::from(val));
    }
    static inline constexpr vector4 operator "" _4d_xyw(unsigned long long val)
    {
      return vector4(conversion::from(val), conversion::from(val), conversion::from(0), conversion::from(val));
    }

    ///

    static inline constexpr vector4 operator "" _4d_xyzw(long double val)
    {
      return vector4(conversion::from(val), conversion::from(val), conversion::from(val), conversion::from(val));
    }
    static inline constexpr vector4 operator "" _4d_xyzw(unsigned long long val)
    {
      return vector4(conversion::from(val), conversion::from(val), conversion::from(val), conversion::from(val));
    }
  } // namespace ct
} // namespace neam

#endif /*__N_1814989461818831215_234927224__CT_POINT_UDL_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

