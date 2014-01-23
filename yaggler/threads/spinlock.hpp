//
// file : spinlock.hpp
// in : file:///home/tim/projects/yaggler/yaggler/threads/spinlock.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 19/10/2013 05:02:34
//
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


#ifndef __N_343181348955182040_1435630634__SPINLOCK_HPP__
# define __N_343181348955182040_1435630634__SPINLOCK_HPP__

#include <atomic>

namespace neam
{
  // I do like classes without constructors :D
  //
  //
  // I also do like comment unrelated with the class functionnality :)
  //
  // Also, compatible with std::lock_guard (--> '<neam::spinlock>').
  class spinlock
  {
    public:
      inline void lock()
      {
        while (lock_flag.test_and_set(std::memory_order_acquire));
      }

      bool try_lock()
      {
        return !lock_flag.test_and_set(std::memory_order_acquire);
      }

      void unlock()
      {
        lock_flag.clear(std::memory_order_release);
      }

    private:
      std::atomic_flag lock_flag = ATOMIC_FLAG_INIT;
  };
} // namespace neam

#endif /*__N_343181348955182040_1435630634__SPINLOCK_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

