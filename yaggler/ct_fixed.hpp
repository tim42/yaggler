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

// CT fixed point math for neam/yaggler
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
    // simple check (else you cannot correctly do a square)
    static_assert(!(fixed_fractionnal_bits % 2), "the fractional part MUST be a multiple of 2");

    // inline/constexpr operations
    // http://en.wikipedia.org/wiki/Q_%28number_format%29
    constexpr static inline fixed_t mul(fixed_t a, fixed_t b)
    {
      return (static_cast<__upper_t>(a) * static_cast<__upper_t>(b) >> (fixed_fractionnal_bits + 1));
    }
    constexpr static inline fixed_t div(fixed_t a, fixed_t b)
    {
      return (static_cast<__upper_t>(a << (fixed_fractionnal_bits + 1))) / b;
    }

    namespace __internal // for sine(cos)/sqrt computation only usage
    {
      // sine (and cos)

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

      constexpr static inline __fixed32_t _sine_quadrant_test(__fixed32_t x)
      {
        return ((x ^ (x << 1)) < 0 ? (1 << 31) - x : x) >> (30 - qN);
      }
      constexpr static inline __fixed32_t _sine_compute(__fixed32_t x)
      {
        return (x) * ((3 << qP) - ((x) * (x) >> qR)) >> qS;
      }
      constexpr static inline __fixed32_t _sine_boot(__fixed32_t x)
      {
        return _sine_compute(_sine_quadrant_test(x << (30 - qN)));
      }
    } // namespace __internal

    constexpr static inline fixed_t sin(fixed_t x)
    {
      return __internal::_sine_boot(x);
    }
    constexpr static inline fixed_t cos(fixed_t x)
    {
      return __internal::_sine_boot(x + (2 << fixed_fractionnal_bits));
    }

    // square with a loss of precision (and long conversion)
    constexpr static inline fixed_t sqwl(__upper_t x)
    {
      return ((x >> (fixed_fractionnal_bits / 2)) * (x >> (fixed_fractionnal_bits / 2)));
    }

    // some conversion functions
    namespace conversion
    {
      template<typename Conv>
      constexpr static inline Conv to(fixed_t x)
      {
        return static_cast<Conv>(x) / static_cast<Conv>(2ul << fixed_fractionnal_bits);
      }
      template<typename Conv>
      constexpr static inline fixed_t from(Conv x)
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

#include <tools/embed.hpp>
namespace neam
{
  namespace embed
  {
    // embed a fixed_t
    N_EMBED_USING(fixed_t, ct::fixed_t);

    // convert a fixed_t to a floating point
    template<typename Type, ct::fixed_t FP>
    struct _fixed_to_floating_point {};

    template<ct::fixed_t FP>
    struct _fixed_to_floating_point<double, FP>
    {
      constexpr _fixed_to_floating_point() {}

      constexpr static double get_val()
      {
        return ct::conversion::to<double>(FP);
      }

      constexpr operator double() const
      {
        return get_val();
      }

      // the preferred way to use this class :)
      constexpr static double get()
      {
        return get_val();
      }

      using type = double;
      static constexpr double value __attribute__((deprecated)) = get_val();
      static constexpr ct::fixed_t fixed_value = FP;
    };
    template<ct::fixed_t FP>
    struct _fixed_to_floating_point<float, FP>
    {
      constexpr _fixed_to_floating_point() {}

      constexpr static float get_val()
      {
        return ct::conversion::to<float>(FP);
      }

      constexpr operator float() const
      {
        return get_val();
      }

      // the preferred way to use this class :)
      constexpr static float get()
      {
        return get_val();
      }

      using type = float;
      static constexpr float value __attribute__((deprecated)) = get_val();
      static constexpr ct::fixed_t fixed_value = FP;
    };

    // embedding for floats/doubles
    template<ct::fixed_t FP>
    using fixed_to_float = _fixed_to_floating_point< float, FP >;
    template<ct::fixed_t FP>
    using fixed_to_double = _fixed_to_floating_point< double, FP >;

#define N_EMBED_FIXED_TO_FLOAT(FP_VALUE)        neam::embed::fixed_to_float<FP_VALUE>
#define N_EMBED_FIXED_TO_DOUBLE(FP_VALUE)        neam::embed::fixed_to_double<FP_VALUE>

  } // namespace embed
} // namespace neam

#endif /*__CT_FIXED_HPP__*/
// kate: indent-mode cstyle; indent-width 2; replace-tabs on;
