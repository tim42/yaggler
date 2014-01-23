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


#ifndef __CT_FIXED_HPP__
#define __CT_FIXED_HPP__

#include <stdint.h>
#include <boost/concept_check.hpp>

// fixed point for neam/yaggler
// TODO: doc ?
namespace neam
{
  namespace ct
  {
#ifdef CT_RASTER_FAST_FP
    using fixed_t = int16_t;
    using __fixed32_t = int32_t;
    using __upper_t = int32_t;
    constexpr char fixed_fractionnal_bits = 6;
    constexpr char fixed_integer_bits = sizeof(fixed_t) * 8 - fixed_fractionnal_bits;
#else
    using fixed_t = int32_t;
    using __fixed32_t = int32_t;
    using __upper_t = int64_t;
    constexpr char fixed_fractionnal_bits = 10;
    constexpr char fixed_integer_bits = sizeof(fixed_t) * 8 - fixed_fractionnal_bits;
#endif
    // simple check (else you cannon correctly do a square)
    static_assert(!(fixed_fractionnal_bits % 2), "the fractional part MUST be a multiple of 2");

    // inline/constexpr operations
    // http://en.wikipedia.org/wiki/Q_%28number_format%29
    constexpr fixed_t mul(fixed_t a, fixed_t b)
    {
      return (static_cast<__upper_t>(a) * static_cast<__upper_t>(b) >> (fixed_fractionnal_bits + 1));
    }
    constexpr fixed_t div(fixed_t a, fixed_t b)
    {
      return (static_cast<__upper_t>(a << (fixed_fractionnal_bits + 1))) / b;
    }

    namespace __internal // for sine(cos)/sqrt computation use only
    {
      // sine (cos)

      constexpr int qN = fixed_fractionnal_bits + 1;
      constexpr int qA = fixed_fractionnal_bits + 1;
      constexpr int qP = 15;
      constexpr int qR = 2 * qN - qP;
      constexpr int qS = qN + qP + 1 - qA;

// NOTE: http://www.coranac.com/2009/07/sines/
// NOTE:       x = x << (30 - qN);     // shift to full s32 range (Q13->Q30)
// NOTE:       if ((x ^ (x << 1)) < 0) // test for quadrant 1 or 2
// NOTE:         x = (1 << 31) - x;
// NOTE:       x = x >> (30 - qN);

      constexpr __fixed32_t _sine_quadrant_test(__fixed32_t x)
      {
        return ((x ^ (x << 1)) < 0 ? (1 << 31) - x : x) >> (30 - qN);
      }
      constexpr __fixed32_t _sine_compute(__fixed32_t x)
      {
        return (x) * ((3 << qP) - ((x) * (x) >> qR)) >> qS;
      }
      constexpr __fixed32_t _sine_boot(__fixed32_t x)
      {
        return _sine_compute(_sine_quadrant_test(x << (30 - qN)));
      }
    } // namespace __internal

    constexpr fixed_t sin(fixed_t x)
    {
      return __internal::_sine_boot(x);
    }
    constexpr fixed_t cos(fixed_t x)
    {
      return __internal::_sine_boot(x + (2 << fixed_fractionnal_bits));
    }

    // square with a loss of precision (and long conversion)
    constexpr fixed_t sqwl(__upper_t x)
    {
      return ((x >> (fixed_fractionnal_bits / 2)) * (x >> (fixed_fractionnal_bits / 2)));
    }

    // some conversion functions
    namespace conversion
    {
      template<typename Conv>
      constexpr Conv to(fixed_t x)
      {
        return static_cast<Conv>(x) / static_cast<Conv>(2ul << fixed_fractionnal_bits);
      }
      template<typename Conv>
      constexpr fixed_t from(Conv x)
      {
        return static_cast<Conv>(x) * static_cast<Conv>(2ul << fixed_fractionnal_bits);
      }
    } // namespace conversion

    namespace constant // mathematical constants
    {
      constexpr fixed_t pi = conversion::from<int>(2); // FIXME: is that true ?
      constexpr fixed_t e = conversion::from<float>(2.71828183);
    } // namespace constant
  } // namespace ct
} // namespace neam

#endif /*__CT_FIXED_HPP__*/
// kate: indent-mode cstyle; indent-width 2; replace-tabs on;
