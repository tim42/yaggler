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


#include "ct_fixed.hpp"
#include "tools/tuple.hpp"

#ifndef __CT_POINT_HPP__
#define __CT_POINT_HPP__

// obviously, it's in this header that a 3D -> 2D projections/transformations is done...
// TODO: doc ?
namespace neam
{
  namespace ct
  {
    struct vector2
    {
      fixed_t x;
      fixed_t y;

      constexpr vector2() : x(0), y(0) {}
      constexpr vector2(fixed_t _x, fixed_t _y) : x(_x), y(_y) {}
      constexpr vector2(const vector2 &_v) : x(_v.x), y(_v.y) {}

      constexpr vector2 convert_to_fixed()
      {
        return vector2
        {
          conversion::from<fixed_t>(x),
          conversion::from<fixed_t>(y),
        };
      }
      constexpr vector2 convert_to_integer()
      {
        return vector2
        {
          conversion::to<fixed_t>(x),
          conversion::to<fixed_t>(y),
        };
      }
    };

    struct vector3
    {
      private: // constexpr helpers
        constexpr static vector2 _project(const vector3 &p, fixed_t x_factor)
        {
          return p.z ? vector2
          {
            conversion::to<fixed_t>(mul(div(p.x * 19, p.z), x_factor)),
            conversion::to<fixed_t>(div(p.y * 19, p.z))
          }
        :
          vector2 { -1, -1};
        }
        constexpr vector3 _transform_last(const vector3 &p, const vector3 &a, const vector3 &t, const vector3 &/*s*/) // FIXME: why s is unused ???
        {
          return
          {
            p.x,
            mul(p.y, cos(a.x)) - mul(p.z, sin(a.x)),
            mul(p.y, sin(a.x)) + mul(p.z, cos(a.x)) + t.z
          };
        }
        constexpr vector3 _transform(const vector3 &p, const vector3 &a, const vector3 &t, const vector3 &s)
        {
          return _transform_last(
          {
            mul(p.x, cos(a.z)) - mul(mul(s.y, y + t.y), sin(a.z)),
            mul(p.x, sin(a.z)) + mul(mul(s.y, y + t.y), cos(a.z)),
            p.z
          }, a, t, s);
        }

      public: // members
        fixed_t x;
        fixed_t y;
        fixed_t z;

        constexpr vector3() : x(0), y(0), z(0) {}
        constexpr vector3(fixed_t _x, fixed_t _y, fixed_t _z) : x(_x), y(_y), z(_z) {}
        constexpr vector3(const vector3 &_v) : x(_v.x), y(_v.y), z(_v.z) {}

        constexpr vector3(const vector2 &ov2, fixed_t _z = conversion::from(0))
          : x(ov2.x), y(ov2.y), z(_z)
        {
        }
//         constexpr vector3(fixed_t _x = conversion::from(0), fixed_t _y = conversion::from(0), fixed_t _z = conversion::from(0))
//           : x(_x), y(_y), z(_z)
//         {
//         }

        // FIXME: is the sin/cos twice calls worth a copy + a single sin/cos call ??
        constexpr vector3 transform(const vector3 &a, const vector3 &t, const vector3 &s)
        {
          return _transform(
          {
            mul(mul(s.x, x + t.x), cos(a.y)) - mul(mul(s.z, z), sin(a.y)),
            0,
            mul(mul(s.x, x + t.x), sin(a.y)) + mul(mul(s.z, z), cos(a.y))
          }, a, t, s);
        }
        constexpr vector2 project(const vector3 &a, const vector3 &t, const vector3 &s, fixed_t x_factor)
        {
          return _project(transform(a, t, s), x_factor);
        }
        constexpr vector3 convert_to_fixed()
        {
          return vector3
          {
            conversion::from<fixed_t>(x),
            conversion::from<fixed_t>(y),
            conversion::from<fixed_t>(z)
          };
        }
        constexpr vector3 convert_to_integer()
        {
          return vector3
          {
            conversion::to<fixed_t>(x),
            conversion::to<fixed_t>(y),
            conversion::to<fixed_t>(z)
          };
        }
    };

    struct vector4
    {
      fixed_t x;
      fixed_t y;
      fixed_t z;
      fixed_t w;

      constexpr vector4() : x(0), y(0), z(0), w(0) {}
      constexpr vector4(fixed_t _x, fixed_t _y, fixed_t _z, fixed_t _w) : x(_x), y(_y), z(_z), w(_w) {}
      constexpr vector4(const vector4 &_v) : x(_v.x), y(_v.y), z(_v.z), w(_v.w) {}

      constexpr vector4 convert_to_fixed()
      {
        return vector4
        {
          conversion::from<fixed_t>(x),
          conversion::from<fixed_t>(y),
          conversion::from<fixed_t>(z),
          conversion::from<fixed_t>(w),
        };
      }
      constexpr vector4 convert_to_integer()
      {
        return vector4
        {
          conversion::to<fixed_t>(x),
          conversion::to<fixed_t>(y),
          conversion::to<fixed_t>(z),
          conversion::to<fixed_t>(w),
        };
      }
    };


    // templated generic vector type
    template<fixed_t... Vals>
    struct vector
    {
      private:
        static_assert(sizeof...(Vals), "attempt to create a neam::ct::vector of dimensionality 0");
        static_assert(sizeof...(Vals) <= 4, "attempt to create a neam::ct::vector of dimensionality > 4");
        neam::cr::tuple<fixed_t, vector2, vector3, vector4> __types__;

        template<typename T>
        using remove_cref = typename std::remove_const<typename std::remove_reference<T>::type>::type;

      public:
        using vec_type = remove_cref<decltype(__types__.get<sizeof...(Vals) - 1>())>;

        static constexpr vec_type value = vec_type{Vals...};
        static constexpr size_t dimensionality = sizeof...(Vals);
    };

  } // namespace ct
} // namespace neam

#endif /*__CT_POINT_HPP__*/
