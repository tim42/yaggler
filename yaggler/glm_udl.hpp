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

#ifndef __N_1814989461818831215_234927224__GLM_UDL_HPP__
# define __N_1814989461818831215_234927224__GLM_UDL_HPP__

#include <glm/glm.hpp>

// this is SO MUCH EASIER than hitting your keyboard for the same things, ever and ever... (poor keyboard... :/ )

namespace neam
{
  // 2D glm::vec
  static glm::vec2 operator "" _vec2_x(long double val)
  {
    return glm::vec2((val), (0));
  }
  static glm::vec2 operator "" _vec2_x(unsigned long long val)
  {
    return glm::vec2((val), (0));
  }

  static glm::vec2 operator "" _vec2_y(long double val)
  {
    return glm::vec2((0), (val));
  }
  static glm::vec2 operator "" _vec2_y(unsigned long long val)
  {
    return glm::vec2((0), (val));
  }

  static glm::vec2 operator "" _vec2_xy(long double val)
  {
    return glm::vec2((val), (val));
  }
  static glm::vec2 operator "" _vec2_xy(unsigned long long val)
  {
    return glm::vec2((val), (val));
  }

  // 3D glm::vec
  static glm::vec3 operator "" _vec3_x(long double val)
  {
    return glm::vec3((val), (0), (0));
  }
  static glm::vec3 operator "" _vec3_x(unsigned long long val)
  {
    return glm::vec3((val), (0), (0));
  }

  static glm::vec3 operator "" _vec3_y(long double val)
  {
    return glm::vec3((0), (val), (0));
  }
  static glm::vec3 operator "" _vec3_y(unsigned long long val)
  {
    return glm::vec3((0), (val), (0));
  }

  static glm::vec3 operator "" _vec3_z(long double val)
  {
    return glm::vec3((0), (0), (val));
  }
  static glm::vec3 operator "" _vec3_z(unsigned long long val)
  {
    return glm::vec3((0), (0), (val));
  }

  static glm::vec3 operator "" _vec3_xy(long double val)
  {
    return glm::vec3((val), (val), (0));
  }
  static glm::vec3 operator "" _vec3_xy(unsigned long long val)
  {
    return glm::vec3((val), (val), (0));
  }

  static glm::vec3 operator "" _vec3_xz(long double val)
  {
    return glm::vec3((val), (0), (val));
  }
  static glm::vec3 operator "" _vec3_xz(unsigned long long val)
  {
    return glm::vec3((val), (0), (val));
  }

  static glm::vec3 operator "" _vec3_yz(long double val)
  {
    return glm::vec3((0), (val), (val));
  }
  static glm::vec3 operator "" _vec3_yz(unsigned long long val)
  {
    return glm::vec3((0), (val), (val));
  }

  static glm::vec3 operator "" _vec3_xyz(long double val)
  {
    return glm::vec3((val), (val), (val));
  }
  static glm::vec3 operator "" _vec3_xyz(unsigned long long val)
  {
    return glm::vec3((val), (val), (val));
  }

  // 4D glm::vec
  static glm::vec4 operator "" _vec4_x(long double val)
  {
    return glm::vec4((val), (0), (0), 0);
  }
  static glm::vec4 operator "" _vec4_x(unsigned long long val)
  {
    return glm::vec4((val), (0), (0), 0);
  }

  static glm::vec4 operator "" _vec4_y(long double val)
  {
    return glm::vec4((0), (val), (0), 0);
  }
  static glm::vec4 operator "" _vec4_y(unsigned long long val)
  {
    return glm::vec4((0), (val), (0), 0);
  }

  static glm::vec4 operator "" _vec4_z(long double val)
  {
    return glm::vec4((0), (0), (val), 0);
  }
  static glm::vec4 operator "" _vec4_z(unsigned long long val)
  {
    return glm::vec4((0), (0), (val), 0);
  }

  static glm::vec4 operator "" _vec4_w(long double val)
  {
    return glm::vec4((0), (0), (0), (val));
  }
  static glm::vec4 operator "" _vec4_w(unsigned long long val)
  {
    return glm::vec4((0), (0), (0), (val));
  }

  ///

  static glm::vec4 operator "" _vec4_xy(long double val)
  {
    return glm::vec4((val), (val), (0), 0);
  }
  static glm::vec4 operator "" _vec4_xy(unsigned long long val)
  {
    return glm::vec4((val), (val), (0), 0);
  }

  static glm::vec4 operator "" _vec4_xz(long double val)
  {
    return glm::vec4((val), (0), (val), 0);
  }
  static glm::vec4 operator "" _vec4_xz(unsigned long long val)
  {
    return glm::vec4((val), (0), (val), 0);
  }

  static glm::vec4 operator "" _vec4_yz(long double val)
  {
    return glm::vec4((0), (val), (val), 0);
  }
  static glm::vec4 operator "" _vec4_yz(unsigned long long val)
  {
    return glm::vec4((0), (val), (val), 0);
  }

  static glm::vec4 operator "" _vec4_xw(long double val)
  {
    return glm::vec4((val), (0), (0), (val));
  }
  static glm::vec4 operator "" _vec4_xw(unsigned long long val)
  {
    return glm::vec4((val), (0), (0), (val));
  }

  static glm::vec4 operator "" _vec4_yw(long double val)
  {
    return glm::vec4((0), (val), (0), (val));
  }
  static glm::vec4 operator "" _vec4_yw(unsigned long long val)
  {
    return glm::vec4((0), (val), (0), (val));
  }

  static glm::vec4 operator "" _vec4_zw(long double val)
  {
    return glm::vec4((0), (0), (val), (val));
  }
  static glm::vec4 operator "" _vec4_zw(unsigned long long val)
  {
    return glm::vec4((0), (0), (val), (val));
  }

  ///
  static glm::vec4 operator "" _vec4_xyz(long double val)
  {
    return glm::vec4((val), (val), (val), 0);
  }
  static glm::vec4 operator "" _vec4_xyz(unsigned long long val)
  {
    return glm::vec4((val), (val), (val), 0);
  }

  static glm::vec4 operator "" _vec4_yzw(long double val)
  {
    return glm::vec4((0), (val), (val), (val));
  }
  static glm::vec4 operator "" _vec4_yzw(unsigned long long val)
  {
    return glm::vec4((0), (val), (val), (val));
  }

  static glm::vec4 operator "" _vec4_xzw(long double val)
  {
    return glm::vec4((val), (0), (val), (val));
  }
  static glm::vec4 operator "" _vec4_xzw(unsigned long long val)
  {
    return glm::vec4((val), (0), (val), (val));
  }

  static glm::vec4 operator "" _vec4_xyw(long double val)
  {
    return glm::vec4((val), (val), (0), (val));
  }
  static glm::vec4 operator "" _vec4_xyw(unsigned long long val)
  {
    return glm::vec4((val), (val), (0), (val));
  }

  ///

  static glm::vec4 operator "" _vec4_xyzw(long double val)
  {
    return glm::vec4((val), (val), (val), (val));
  }
  static glm::vec4 operator "" _vec4_xyzw(unsigned long long val)
  {
    return glm::vec4((val), (val), (val), (val));
  }
} // namespace neam

#endif /*__N_1814989461818831215_234927224__GLM_UDL_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

