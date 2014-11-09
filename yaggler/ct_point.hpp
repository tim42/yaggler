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
#include "tools/ct_list.hpp"

#ifndef __CT_POINT_HPP__
#define __CT_POINT_HPP__

// TODO: doc ?
namespace neam
{
  namespace ct
  {
    struct vector2
    {
      fixed_t x;
      fixed_t y;

      constexpr vector2(fixed_t _x = conversion::from(0), fixed_t _y = conversion::from(0)) : x(_x), y(_y) {}
      constexpr vector2(const vector2 &_v) : x(_v.x), y(_v.y) {}

      constexpr vector2 convert_to_fixed() const
      {
        return vector2
        {
          conversion::from<fixed_t>(x),
          conversion::from<fixed_t>(y),
        };
      }
      constexpr vector2 convert_to_integer() const
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
        constexpr static inline vector2 _project(const vector3 &p, fixed_t x_factor)
        {
          return p.z ? vector2
          {
            conversion::to<fixed_t>(mul(div(p.x * 19, p.z), x_factor)),
            conversion::to<fixed_t>(div(p.y * 19, p.z))
          }
        :
          vector2 { -1, -1};
        }
        constexpr vector3 inline _transform_last(const vector3 &p, const vector3 &a, const vector3 &t, const vector3 &/*s*/) const // FIXME: why s is unused ???
        {
          return
          {
            p.x,
            mul(p.y, cos(a.x)) - mul(p.z, sin(a.x)),
            mul(p.y, sin(a.x)) + mul(p.z, cos(a.x)) + t.z
          };
        }
        constexpr inline vector3 _transform(const vector3 &p, const vector3 &a, const vector3 &t, const vector3 &s) const
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

        constexpr vector3(const vector3 &_v) : x(_v.x), y(_v.y), z(_v.z) {}

        constexpr vector3(const vector2 &ov2, fixed_t _z = conversion::from(0))
          : x(ov2.x), y(ov2.y), z(_z)
        {
        }
        constexpr vector3(fixed_t _x = conversion::from(0), fixed_t _y = conversion::from(0), fixed_t _z = conversion::from(0))
          : x(_x), y(_y), z(_z)
        {
        }

        // FIXME: is the sin/cos twice calls worth a copy + a single sin/cos call ??
        constexpr inline vector3 transform(const vector3 &a, const vector3 &t, const vector3 &s) const
        {
          return _transform(
          {
            mul(mul(s.x, x + t.x), cos(a.y)) - mul(mul(s.z, z), sin(a.y)),
            0,
            mul(mul(s.x, x + t.x), sin(a.y)) + mul(mul(s.z, z), cos(a.y))
          }, a, t, s);
        }
        constexpr inline vector2 project(const vector3 &a, const vector3 &t, const vector3 &s, fixed_t x_factor) const
        {
          return _project(transform(a, t, s), x_factor);
        }
        constexpr inline vector3 convert_to_fixed() const
        {
          return vector3
          {
            conversion::from<fixed_t>(x),
            conversion::from<fixed_t>(y),
            conversion::from<fixed_t>(z)
          };
        }
        constexpr inline vector3 convert_to_integer() const
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

      constexpr vector4(fixed_t _x = conversion::from(0), fixed_t _y = conversion::from(0), fixed_t _z = conversion::from(0), fixed_t _w = conversion::from(0)) : x(_x), y(_y), z(_z), w(_w) {}
      constexpr vector4(const vector4 &_v) : x(_v.x), y(_v.y), z(_v.z), w(_v.w) {}

      constexpr inline vector4 convert_to_fixed() const
      {
        return vector4
        {
          conversion::from<fixed_t>(x),
          conversion::from<fixed_t>(y),
          conversion::from<fixed_t>(z),
          conversion::from<fixed_t>(w),
        };
      }
      constexpr inline vector4 convert_to_integer() const
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

        using __types__ = neam::ct::type_list<fixed_t, vector2, vector3, vector4>;

        template<typename T>
        using remove_cref = typename std::remove_const<typename std::remove_reference<T>::type>::type;

      public:
        using vec_type = remove_cref<typename __types__::get_type<sizeof...(Vals) - 1>>;

        static constexpr vec_type value = vec_type{Vals...};
        static constexpr size_t dimensionality = sizeof...(Vals);

        // implicit conversion
        constexpr operator vec_type()
        {
          return value;
        }
    };

    template<fixed_t... Vals>
    constexpr typename vector<Vals...>::vec_type vector<Vals...>::value;

  } // namespace ct
} // namespace neam

#endif /*__CT_POINT_HPP__*/
