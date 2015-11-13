//
// file : make_animator.hpp
// in : file:///home/tim/projects/yaggler/yaggler/bleunw/animator/make_animator.hpp
//
// created by : Timothée Feuillet on linux-vnd3.site
// date: 13/11/2015 13:12:25
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

#ifndef __N_107119973888792883_869172185__MAKE_ANIMATOR_HPP__
# define __N_107119973888792883_869172185__MAKE_ANIMATOR_HPP__

#include "../task/task_control.hpp"
#include "../task/task.hpp"
#include "easing.hpp"

namespace neam
{
  namespace bleunw
  {
    namespace yaggler
    {
      /// \brief Make a task control an animator: the destination function will be called with an number in the interval [0-1]
      ///        instead of a delta time
      /// \param[in,out] ctrl The task control to change to an animator
      /// \param duration The animator duration
      /// \param easing_func The easing function to use
      /// \note If ctrl.repeat is true, the animator will be repeated (the timer won't be reversed, but instead set to 0)
      void make_animator(task::task_control &ctrl, float duration, easing::func_t easing_func = &easing::linear)
      {
        float acc = 0.f;
        ctrl.run_func = [acc, duration, easing_func](float dt, task::task_control &ctrl, double now) mutable
        {
          if (acc >= duration)
            acc = 0.f;
          else
          {
            acc += dt;

            if (acc > duration)
              acc = duration;
          }

          if (acc < duration || ctrl.repeat)
            ctrl.register_task(now);

          try
          {
            ctrl.func(easing_func(acc / duration), ctrl);
          }
          catch (...)
          {
            ctrl.dismiss();
            throw;
          }
        };
      }

      /// \brief Make a task control an animator: the destination function will be called with an number in the interval [0-1]
      ///        instead of a delta time
      /// \param[in,out] ctrl The task control to change to an animator
      /// \param duration The animator duration
      /// \param easing_func The easing function to use
      /// \note If ctrl.repeat is true, the animator will be repeated (and the timer WILL be reversed)
      void make_reversable_animator(task::task_control &ctrl, float duration, easing::func_t easing_func = &easing::linear)
      {
        float acc = 0.f;
        float sgn = 1.f;
        ctrl.run_func = [acc, sgn, duration, easing_func](float dt, task::task_control &ctrl, double now) mutable
        {
          if (acc >= duration)
            sgn *= -1.f;
          acc += dt * sgn;

          if (acc > duration)
            acc = duration;
          else if (acc < 0.f)
          {
            acc = 0.f;
            sgn *= -1.f;
          }

          if (acc < duration || ctrl.repeat)
            ctrl.register_task(now);

          try
          {
            ctrl.func(easing_func(acc / duration), ctrl);
          }
          catch (...)
          {
            ctrl.dismiss();
            throw;
          }
        };
      }
    } // namespace yaggler
  } // namespace bleunw
} // namespace neam

#endif /*__N_107119973888792883_869172185__MAKE_ANIMATOR_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 


