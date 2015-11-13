//
// file : easing.hpp
// in : file:///home/tim/projects/yaggler/yaggler/bleunw/animator/easing.hpp
//
// created by : Timothée Feuillet on linux-vnd3.site
// date: 13/11/2015 13:12:16
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

#ifndef __N_15184475972003110334_1285238881__EASING_HPP__
# define __N_15184475972003110334_1285238881__EASING_HPP__

#include <functional>
#include <cmath>

namespace neam
{
  namespace bleunw
  {
    namespace yaggler
    {
      namespace easing
      {
        /// \brief The signature of an easing function
        /// \note An easing function takes a number in the interval [0-1] and should return a number in the interval [0-1]
        typedef std::function<float(float)> func_t;

        /// \brief A linear easing function
        float linear(float x) { return x; }

        /// \brief A quadratic easing function
        float quad(float x) { return x * x; }

        /// \brief A cubic easing function
        float cubic(float x) { return x * x * x; }

        /// \brief A quartic easing function
        float quart(float x) { return x * x * x * x; }

        /// \brief A square-root easing function
        float sqrt(float x) { return sqrtf(x); }

        /// \brief A Circle easing function
        float circle(float x) { return 1.f - sqrtf(1.f - x * x); }

        /// \brief Return an easing function.
        /// \note A strength value of 0.4 is very similar to the circle easing function
        func_t strange_pow(float strength = 2.f) { return [ = ](float x) -> float { return 1.f - powf(1.f - x, x * strength); }; }

        /// \brief Return a configurable exp() easing function
        /// \param strength A number in the interval ]0;+inf[ indicating the strength of the exponential
        /// the nearer strength is from 0, the more the curve will look like a linear curve
        func_t exp(float strength = 1.f)
        {
          const float factor = 1.f / (1.f - expf(-1.f * strength));
          return [ = ](float x) -> float { return (1.f - expf(-1.f * x)) * factor; };
        }

        /// \brief Reverse an easing curve (was an in curve, make it a out curve)
        func_t reverse(func_t func) { return [ = ] (float x) -> float { return 1.f - func(1.f - x); }; }

        /// \brief Make an easing function in-out
        func_t inout(func_t func)
        {
          return [ = ] (float x) -> float
          {
            if (x < 0.5)
              return func(x * 2.f) / 2.f;
            else
              return func((x - 0.5f) * 2.f) / 2.f + 0.5f;
          };
        }

        /// \brief Make two easing function into one in-out function
        func_t inout(func_t in_func, func_t out_func)
        {
          return [ = ] (float x) -> float
          {
            if (x < 0.5)
              return in_func(x * 2.f) / 2.f;
            else
              return out_func((x - 0.5f) * 2.f) / 2.f + 0.5f;
          };
        }
      } // namespace easing
    } // namespace yaggler
  } // namespace bleunw
} // namespace neam

#endif /*__N_15184475972003110334_1285238881__EASING_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 


