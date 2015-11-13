//
// file : task_control.hpp
// in : file:///home/tim/projects/yaggler/yaggler/bleunw/task/task_control.hpp
//
// created by : Timothée Feuillet on linux-vnd3.site
// date: 22/10/2015 18:24:53
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

#ifndef __N_10238735211806126413_313215291__TASK_CONTROL_HPP__
# define __N_10238735211806126413_313215291__TASK_CONTROL_HPP__

#include <atomic>
#include <functional>
#include "types.hpp"

namespace neam
{
  namespace bleunw
  {
    namespace yaggler
    {
      namespace task
      {
        class scheduler;
        class task;

        /// \brief a simple task (a function call, repeated and delayed)
        /// \note If you have to set repeat, do it at the task creation (else, in the task, check ctrl.is_registered() and if false, call ctrl.register())
        class task_control
        {
          public:
            ctrl_func_t run_func; ///< \brief The method to run (either a proxy function or the final function)
            task_func_t func;     ///< \brief The original task function

            scheduler *task_scheduler = nullptr; ///< \brief The scheduler to use to re-register tasks

            float delay = 0; ///< \brief Delay (in s) between each invocations

            execution_type task_type = execution_type::normal; ///< \brief Whether or not this task *has* to run only on the main thread

            bool repeat = false; ///< \brief Whether or not the task has to be re-registered after having executed

            int priority = 0; ///< \brief Task priority (mostly used to change the orger of tasks with execution_type::pre_buffer_swap).
                              ///         Priority increase with the number. (negative number mean very low priority)

          public:

            /// \brief Register the task.
            void register_task(double now = -1.);

            /// \brief Dismiss the task, but does not unregister it
            /// register_once() will undo the dismiss and handle the case where
            /// the task has been removed from the queue
            void dismiss()
            {
              dismissed = true;
            }

            /// \brief return true if the task is both registered and not dismissed
            bool is_registered() const
            {
              return registered && !dismissed;
            }

          public:
            /// \brief constructor
            task_control() {}

          private:
            void reset()
            {
              registered = false;
              dismissed = false;
            }

          private:
            std::atomic<bool> registered = ATOMIC_VAR_INIT(false);
            std::atomic<bool> dismissed = ATOMIC_VAR_INIT(false);
            neam::bleunw::yaggler::task::task *linked_task = nullptr;

            friend class scheduler;
            friend class task;
        };
      } // namespace task
    } // namespace yaggler
  } // namespace bleunw
} // namespace neam

#endif /*__N_10238735211806126413_313215291__TASK_CONTROL_HPP__*/

#ifdef IN_IDE_PARSER
#define TASK_SCHEDULER_DEFINED
#endif /*IN_IDE_PARSER*/

#ifndef __N_10238735211806126413_313215291__TASK_CONTROL_HPP__D__
#ifdef TASK_SCHEDULER_DEFINED
# define __N_10238735211806126413_313215291__TASK_CONTROL_HPP__D__

namespace neam
{
  namespace bleunw
  {
    namespace yaggler
    {
      namespace task
      {
        void task_control::register_task(double now)
        {
          if (!task_scheduler)
            return;
          dismissed = false;
          bool f = false;
          if (!registered.compare_exchange_strong(f, true)) // Make sure we have been here only one time
            return;
          task_scheduler->push_task(std::move(*linked_task), task_type);
          if (now > 0.)
            linked_task->registered_ts = now;
        }
      } // namespace task
    } // namespace yaggler
  } // namespace bleunw
} // namespace neam

#endif /*TASK_SCHEDULER_DEFINED*/
#endif /*__N_10238735211806126413_313215291__TASK_CONTROL_HPP__D__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

