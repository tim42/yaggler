//
// file : types.hpp
// in : file:///home/tim/projects/yaggler/yaggler/bleunw/task/types.hpp
//
// created by : Timothée Feuillet on linux-vnd3.site
// date: 22/10/2015 18:12:40
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

#ifndef __N_9242203241113295496_328999000__TYPES_HPP__
# define __N_9242203241113295496_328999000__TYPES_HPP__

#include <functional>

namespace neam
{
  namespace bleunw
  {
    namespace yaggler
    {
      namespace task
      {
        class task_control;

        /// \brief The function type used by tasks
        typedef std::function<void(float, task_control &)> task_func_t;
        /// \brief The function type used by task controls
        typedef std::function<void(float, task_control &, double)> ctrl_func_t;
        /// \brief The function type used by task controls for the then
        typedef std::function<void(task_control &)> then_func_t;

        enum class execution_type
        {
          normal,
          gl,
          pre_buffer_swap
        };
      } // namespace task
    } // namespace yaggler
  } // namespace bleunw
} // namespace neam

#endif /*__N_9242203241113295496_328999000__TYPES_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

