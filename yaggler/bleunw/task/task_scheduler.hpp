//
// file : task_scheduler.hpp
// in : file:///home/tim/projects/yaggler/yaggler/bleunw/task/task_scheduler.hpp
//
// created by : Timothée Feuillet on linux-vnd3.site
// date: 21/10/2015 17:09:36
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

#ifndef __N_1565169418532046895_210799306__TASK_SCHEDULER_HPP__
# define __N_1565169418532046895_210799306__TASK_SCHEDULER_HPP__

#include <atomic>
#include <thread>
#include <mutex>
#include <algorithm>
#include <vector>
#include <tools/chrono.hpp>
#include <tools/logger/logger.hpp>
#include "task.hpp"
#include <yaggler_except.hpp>

namespace neam
{
  namespace bleunw
  {
    namespace yaggler
    {
      namespace task
      {
        /// \brief This is a quite simple scheduler that aim to run tasks
        /// It don't create any threads but can handle them (just call the run_some() method)
        /// \note Because it sorts the task arrays, It may be preferable to have less tasks that does a bit more.
        ///       But under 4k tasks, it won't really hurt the framerate (if a majority of task are NOT to be executed on the GL thread).
        ///       You can also play with the different flags to see if this change something for you, but the more task you have,
        ///       the more your time per frame will be consumed by sorting the arrays.
        /// \todo disable array sorting with a flag.
        /// \note To remove the debug messages you can define the macro BLEUNW_SCHEDULER_NO_DEBUG_MESSAGES
        class scheduler
        {
          public:
            scheduler()
            {
              here_be_gl_thread();
              active_thread_count = 0;
              current = buffers + ((index + 1) % 2);
              sort_flag.test_and_set(std::memory_order_relaxed);
            }

            ~scheduler()
            {
              end_frame(); // This way, there won't be any remaining thread
            }

            // // flags // //

            /// \brief Let the scheduler assume that there isn't only the main thread
            /// to run tasks.
            /// This is an important optimization that allow reporting tasks on other threads
            /// as needed to lighten the burden of the GL thread
            /// Default is \b true
            /// \note Disable the multi-threading could make a little performance gain (as some atomic operation are skipped)
            ///       but when the multiple threading of the scheduler is disbaled, calling run_some() in another thread
            ///       won't do anything
            /// \see assume_stable_second_thread()
            void enable_multiple_threads(bool enable) { multiple_threads = enable; }

            /// \brief Let the scheduler assume that there is a second thread dedicated almost uniquely for himself
            /// This will allow a more aggressive optimization by running FIRST tasks that are to be run on the GL thread
            /// and THEN other tasks, as they remain.
            /// It will, however, run some tasks in the gl thread as long as the time limit is reached or task list is exhausted
            /// Implies \code assume_multiple_threads(true) \endcode
            /// \note Having just this flag activated, you will get the maximum throughput of the task scheduler
            /// Default is \b false
            /// \note setting \p assume at true you also mean for the scheduler that there will have at least one call
            ///       of run_some() in another thread per frame
            /// \see enforce_task_repartition()
            void assume_stable_second_thread(bool assume)
            {
              enable_multiple_threads(assume || multiple_threads);
              stable_second_thread = assume;
            }

            /// \brief Disallow any task that do not do any GL operation to be run on the GL thread.
            /// Implies \code assume_stable_second_thread(true) \endcode
            /// \note Having this flag activated, you will get the maximum framerate, so enable it only if your framerate is endangered
            ///       by the task scheduler and the fact non-gl tasks could run on the GL thread, but this flag could increase the lateness factor.
            /// Default is \b false
            void enforce_task_repartition(bool enforce)
            {
              assume_stable_second_thread(enforce || stable_second_thread);
              task_repartition = enforce;
            }

            // TODO: skip task time & task priority ? (disable tasks sorting)

            // // settings // //

            /// \brief Sets the maximum duration of the run_some() method
            /// Default is 1/120s
            /// \note If a task is unexpectedly long, the delay could be reached and exceeded
            /// \note Some tasks may have to run at each frame, and they can slow everything / exceed the delay
            /// \note You MUST set a > 0.f maximum_run_duration.
            void set_maximum_run_duration(float duration = 1.f/120.f) { maximum_run_duration = duration; }

            /// \brief Tell the scheduler that the GL thread (or "main thread") is the thread where this method is called
            /// \note Call this method if the GL thread is not the thread where the scheduler has been created
            void here_be_gl_thread() { gl_thread_id = std::this_thread::get_id(); }

            /// \brief Set the speed factor of the scheduler
            /// Default is 1.f
            /// \note The factor could not be negative nor == 0;
            /// \note It is probably a good idea to leave it at 1.f
            void set_speed_factor(float factor = 1.f) { speed_factor = factor > 0.0001 ? factor : speed_factor; }

            // // queries // //

            /// \brief true if the scheduler is late on more than 50% (or the \p lateness if setted) of the frames during the last second
            /// \note This is a good indicator on whether or not spawning a new thread !
            bool is_late(float _lateness = 0.5) const
            {
              return get_lateness() >= _lateness;
            }

            /// \brief Return the current lateness of the task scheduler (rolling average on 1s)
            float get_lateness() const
            {
              return (lateness / (this_second_frame_count + 1.f)) * this_second_advancement
                     + last_second_lateness * (1. - this_second_advancement);
            }

            /// \brief Return the number of time we hit a critical error that have laid to discarding thread safety
            /// If not 0, you could try to exit all the threads one by one, reset (calling _reset() )
            //  and continue the execution ( end_frame() on the main thread, then run_some() at some place)
            size_t get_critical_hit_count() const
            {
              return critical_hit;
            }

            // TODO: average time / immediate time for this frame
            // TODO: average parallelism of the tasks: if 0, no parallelism possible, if 1, parallelism on one thread, 2, on two threads... average on 1s + immediate

            // // methods // //

            /// \brief Push a task to the scheduler
            /// \note execution_type::pre_buffer_swap task CAN'T BE TIMED, they will run at the next frame end
            ///       but they can have a priority, and this will be accounted in the call order
            ///       There won't have any other task that will run while a pre_buffer_swap task is running
            task_control &push_task(const task_func_t &t, execution_type etype = execution_type::normal)
            {
              std::lock_guard<neam::spinlock> _u0(buffers[index].push_lock);

              task_control *ctrl = nullptr;

              if (etype == execution_type::pre_buffer_swap)
              {
                buffers[index].gl_pre_swap_task_list.emplace_back(t);
                buffers[index].gl_pre_swap_task_list.back().registered_ts = chrono.now();
                ctrl = &buffers[index].gl_pre_swap_task_list.back().get_task_control();
              }
              else if (etype == execution_type::gl || !multiple_threads)
              {
                buffers[index].gl_task_list.emplace_back(t);
                buffers[index].gl_task_list.back().registered_ts = chrono.now();
                ctrl = &buffers[index].gl_task_list.back().get_task_control();
              }
              else
              {
                buffers[index].other_task_list.emplace_back(t);
                buffers[index].other_task_list.back().registered_ts = chrono.now();
                ctrl = &buffers[index].other_task_list.back().get_task_control();
              }
              ctrl->task_type = etype;
              ctrl->task_scheduler = this;
              ctrl->registered = true;
              return *ctrl;
            }

            /// \brief Push a task to the scheduler
            /// \note execution_type::pre_buffer_swap task CAN'T BE TIMED, they will run at the next frame end
            ///       but they can have a priority, and this will be accounted in the call order
            ///       There won't have any other task that will run while a pre_buffer_swap task is running
            task_control &push_task(task_control &t)
            {
              std::lock_guard<neam::spinlock> _u0(buffers[index].push_lock);

              if (t.task_type == execution_type::pre_buffer_swap)
              {
                buffers[index].gl_pre_swap_task_list.push_back(task(t));
                buffers[index].gl_pre_swap_task_list.back().registered_ts = chrono.now();
              }
              else if (t.task_type == execution_type::gl || !multiple_threads)
              {
                buffers[index].gl_task_list.push_back(task(t));
                buffers[index].gl_task_list.back().registered_ts = chrono.now();
              }
              else
              {
                buffers[index].other_task_list.push_back(task(t));
                buffers[index].other_task_list.back().registered_ts = chrono.now();
              }
              t.task_scheduler = this;
              t.registered = true;
              return t;
            }

            /// \brief Create a un-registered task_control (this is wait-free)
            /// \note the _do_not_delete property won't be set to true, every values of the object is kept as if default initialized
            task_control *create_task_control(const task_func_t &t, execution_type etype = execution_type::normal)
            {
              task temp_task(t); // For the allocation + initialization of the task control

              task_control *ctrl = temp_task.ctrl; // Retrieve the task_control + remove it from the task object
              temp_task.ctrl = nullptr;
              ctrl->linked_task = nullptr;

              // Initialize it
              ctrl->task_type = etype;
              ctrl->task_scheduler = this;

              return ctrl;
            }

            /// \brief Clear the task scheduler, also make every thread in run_some() quit the execution as soon as
            ///        the method is called.
            /// \note To start again, you have to call end_frame()
            void clear()
            {
              // sync threads !
              frame_sync_lock = -1; // HURRY UP ! (also: run_some is forbidden)
              if (multiple_threads)
              {
                while (active_thread_count.load(std::memory_order_seq_cst) != 0)
                {
                  std::this_thread::yield();

                  if (frame_chrono.get_accumulated_time() > maximum_run_duration * 40) // We are very late !!
                  {
                    ++lateness;
                    ++critical_hit;
                    if (!no_critical_messages_about_wait)
                    {
                      neam::cr::out.critical() << LOGGER_INFO << "task::scheduler::end_frame(): scheduler is late of "
                                               << frame_chrono.get_accumulated_time() - maximum_run_duration << "s waiting for " << active_thread_count
                                               << " threads to terminate." << neam::cr::newline
                                               << "Please report: this is a bug." << neam::cr::newline
                                               << "Going to ignore remaining threads. This error won't be printed again." << std::endl;
                      no_critical_messages_about_wait = true;
                    }
                    break;
                  }
                }
              }
              frame_sync_lock = -2;

              // prepare buffers for the clear
              {
                std::lock_guard<neam::spinlock> _u0(buffers[index].push_lock);
                buffers[index].clear();
              }
              current->clear();

              frame_sync_lock = 1;
            }

            /// \brief Reset the internal state of the scheduler
            /// \warning As this function is intended to be used for recovery in case of a critical hit, it is \b NOT \b THREAD \b SAFE
            ///          You will have exit all secondary thread yourself (or make sure they will not call \b ANY of the scheduler methods in the process)
            /// \note This method have to be called in the GL thread, though no checks are made.
            /// \note Marked as ADVANCED 'cause this could be DANGEROUS and should be used with CAUTION
            void _reset()
            {
              here_be_gl_thread(); // reset the GL thread
              frame_sync_lock = 0; // reset the frame sync/lock
              active_thread_count = 0; // reset the number of active thread count
              gl_thread_entered = false;
              sort_flag.test_and_set();
              frame_chrono.reset();

              lateness = 0.f;
              last_second_lateness = 0.25f;
              this_second_frame_count = 0.f;
              this_second_advancement = 0.f;
              chrono.reset();

              gl_task_in_time_this_frame = false;
              other_task_in_time_this_frame = false;

              critical_hit = 0; // reset the number of critical hits

              // cleanup buffers
              //    force unlock the spinlocks (in case where the lock was held and not released)
              buffers[0].push_lock.unlock();
              buffers[1].push_lock.unlock();

              buffers[0].gl_task_list_index = 0;
              buffers[1].gl_task_list_index = 0;

              buffers[0].other_task_list_index = 0;
              buffers[1].other_task_list_index = 0;

              // cleanup buffer 0 (remove tasks that don't have any task_control or aren't registered: moved tasks or tasks that have already run)
              buffers[0].gl_task_list.erase(std::remove_if(buffers[0].gl_task_list.begin(), buffers[0].gl_task_list.end(), [](const task &x)
              {
                return !x.ctrl || !x.get_task_control().is_registered();
              }));
              buffers[0].other_task_list.erase(std::remove_if(buffers[0].other_task_list.begin(), buffers[0].other_task_list.end(), [](const task &x)
              {
                return !x.ctrl || !x.get_task_control().is_registered();
              }));
              buffers[0].gl_pre_swap_task_list.erase(std::remove_if(buffers[0].gl_pre_swap_task_list.begin(), buffers[0].gl_pre_swap_task_list.end(), [](const task &x)
              {
                return !x.ctrl || !x.get_task_control().is_registered();
              }));

              // cleanup buffer 1 (remove tasks that don't have any task_control or aren't registered: moved tasks or tasks that have already run)
              buffers[1].gl_task_list.erase(std::remove_if(buffers[1].gl_task_list.begin(), buffers[1].gl_task_list.end(), [](const task &x)
              {
                return !x.ctrl || !x.get_task_control().is_registered();
              }));
              buffers[1].other_task_list.erase(std::remove_if(buffers[1].other_task_list.begin(), buffers[1].other_task_list.end(), [](const task &x)
              {
                return !x.ctrl || !x.get_task_control().is_registered();
              }));
              buffers[1].gl_pre_swap_task_list.erase(std::remove_if(buffers[1].gl_pre_swap_task_list.begin(), buffers[1].gl_pre_swap_task_list.end(), [](const task &x)
              {
                return !x.ctrl || !x.get_task_control().is_registered();
              }));

              // insert non-run tasks in the buffer 0
              for (size_t i = 0; i < buffers[1].gl_task_list.size(); ++i)
                buffers[0].gl_task_list.push_back(std::move(buffers[1].gl_task_list[i]));
              for (size_t i = 0; i < buffers[1].other_task_list.size(); ++i)
                buffers[0].other_task_list.push_back(std::move(buffers[1].other_task_list[i]));
              for (size_t i = 0; i < buffers[1].gl_pre_swap_task_list.size(); ++i)
                buffers[0].gl_pre_swap_task_list.push_back(std::move(buffers[1].gl_pre_swap_task_list[i]));

              // clear the buffer 1 (we just moved everything in the buffer 0)
              buffers[1].clear();

              // free unused memory
              buffers[0].shrink_to_fit();
              buffers[1].shrink_to_fit();

              // reset indexes
              index = 0;
              current = buffers + 1;
            }

            /// \brief Mark the end of a frame. If the sync flag is true, it will wait other threads to finish their tasks and swap,
            ///        whereas if the sync flag is false, it will just swap thus possibly accumulating delay in favor of a possibly
            ///        higher framerate for CPU limited progs.
            /// \note You \b HAVE \b TO call this method.
            /// \see request_frame_end()
            void end_frame()
            {
              if (std::this_thread::get_id() != gl_thread_id) // see request_frame_end()
                return;

              // sync threads !
              frame_sync_lock = -1; // HURRY UP ! (also: run_some is forbidden)
              if (multiple_threads)
              {
                while (active_thread_count.load(std::memory_order_seq_cst) != 0)
                {
                  std::this_thread::yield();

                  if (frame_chrono.get_accumulated_time() > maximum_run_duration * 40) // We are very late !!
                  {
                    ++lateness;
                    ++critical_hit;
                    if (!no_critical_messages_about_wait)
                    {
                      neam::cr::out.critical() << LOGGER_INFO << "task::scheduler::end_frame(): scheduler is late of "
                                               << frame_chrono.get_accumulated_time() - maximum_run_duration << "s waiting for " << active_thread_count
                                               << " threads to terminate." << neam::cr::newline
                                               << "Please report: this is a bug." << neam::cr::newline
                                               << "Going to ignore remaining threads. This error won't be printed again." << std::endl;
                      no_critical_messages_about_wait = true;
                    }
                    break;
                  }
                }
              }
              frame_sync_lock = -2;

              // run pre-swap-buffer task list
              std::sort(current->gl_pre_swap_task_list.begin(), current->gl_pre_swap_task_list.end(), [](const task &t1, const task &t2)
              {
                return t1.get_task_control().priority < t2.get_task_control().priority;
              });
              for (task &i : current->gl_pre_swap_task_list)
              {
                double now = chrono.now();
                i.get_task_control().registered = false;
                i.get_task_control().run_func((now - i.registered_ts) * speed_factor, i.get_task_control(), now);
              }

              // Update the lateness
              if (!gl_task_in_time_this_frame)
                ++lateness;
              if (!other_task_in_time_this_frame)
                ++lateness;
              // reset in-time status flag
              gl_task_in_time_this_frame = false;
              other_task_in_time_this_frame = false;

              const float acc_time = chrono.get_accumulated_time();
              if (acc_time >= (1.f - 1.f/150.f)) // end that second
              {
                last_second_lateness = lateness / (this_second_frame_count + 1);
                chrono.reset(); // reset the chrono
                lateness = 0.f;
                this_second_advancement = 0.f;
                this_second_frame_count = 0.f;
#ifndef YAGGLER_NO_MESSAGES
#ifndef BLEUNW_SCHEDULER_NO_DEBUG_MESSAGES
                // We print a nice debug message on whether or not we should spawn some another threads.
                if (last_second_lateness > 0.95)
                  neam::cr::out.debug() << LOGGER_INFO << "WARNING: task::scheduler: Lateness factor for the last second: " << last_second_lateness << std::endl;
#endif /*BLEUNW_SCHEDULER_NO_DEBUG_MESSAGES*/
#endif /*YAGGLER_NO_MESSAGES*/
              }
              else // continue that second
              {
                this_second_advancement = acc_time;
                this_second_frame_count += 1.f;
              }

              // reset the sort flag
              sort_flag.test_and_set(std::memory_order_acquire);

              // prepare buffers for the swap
              {
                std::lock_guard<neam::spinlock> _u0(buffers[index].push_lock);
                buffers[index].insert(*current);
              }
              current->clear();

              frame_sync_lock = 0; // OK TO GO
            }

            /// \brief Make the thread sleep until the end of the frame, where you can call again run_some() safely
            /// \note Use this only in non-gl threads (it won't do anything if called in the gl thread)
            /// An example on ow to use it:
            /// \code
            ///   { // scope of the thread
            ///     // [...]
            ///     while (is_app_working)
            ///     {
            ///       // [...]
            ///       scheduler->wait_for_frame_end();
            ///       scheduler->run_some();
            ///       // [...]
            ///     }
            ///   }
            /// \endcode
            bool wait_for_frame_end()
            {
              // avoid a stupid deadlock !
              if (std::this_thread::get_id() == gl_thread_id || !multiple_threads)
                return false; // Hum, you're on the main thread you stupid people

              // wait for a favorable sync-lock
              while (frame_sync_lock.load(std::memory_order_acquire) != 0)
                std::this_thread::yield();

              // Well, we might have something to do right now...
              if (frame_chrono.get_accumulated_time() < maximum_run_duration)
                return !other_task_in_time_this_frame && current->other_task_list.size();
              return true;
            }

            /// \brief Same as wait_for_frame_end(), but returns immediately
            bool is_frame_end()
            {
              if (std::this_thread::get_id() == gl_thread_id || !multiple_threads)
                return false; // Hum, you're on the main thread you stupid people

              // wait for a favorable sync-lock
              if (frame_sync_lock.load(std::memory_order_acquire) != 0)
                return false;

              // Well, we might have something to do right now...
              if (frame_chrono.get_accumulated_time() < maximum_run_duration)
                return !other_task_in_time_this_frame && current->other_task_list.size();
              return true;
            }

            /// \brief Run some of the tasks.
            /// Maximum duration is controlled by set_maximum_run_duration()
            /// If in the main/GL Thread, don't forget to call end_frame() each time you swap the buffers !
            /// \see set_maximum_run_duration()
            /// \param run_for Set a custom run duration (mostly for being used in additional threads).
            ///                Please note that a frame end will trigger the return of this method disregarding the remaining number of task
            void run_some(float run_for = -1.f)
            {
              const bool is_gl_thread = (std::this_thread::get_id() == gl_thread_id);
              float time_limit = std::min(run_for <= 0.0001f ? maximum_run_duration : run_for, maximum_run_duration);
              time_limit = time_limit > 0.0001f ? time_limit : 1.f; // Make the hardcoded maximum to 1s of run_time (this is HUGE for a realtime app).
              time_limit -= 0.0001f;

              if (is_gl_thread)
              {
                frame_chrono.reset();
                gl_thread_entered = true;
              }
              else if (!multiple_threads)
                return;

              const bool is_sort_thread = !active_thread_count++;
              if (frame_sync_lock < 0)
              {
                --active_thread_count;
                return;
              }
              if (is_sort_thread && frame_sync_lock == 0)
              {
                if (sort_flag.test_and_set())
                {
                  frame_chrono.reset(); // reset the chrono (we are in a thread safe context !)

                  {
                    std::lock_guard<neam::spinlock> _u0(buffers[(index    )    ].push_lock);
                    std::lock_guard<neam::spinlock> _u1(buffers[(index + 1) % 2].push_lock);
                    current = buffers + index; // Set the index
                    index = (index + 1) % 2; // this is the only place where the index is modified
                    current->shrink_to_fit();
                  }

                  if (multiple_threads)
                  {
                    // As we shouldn't have so much tasks, this won't be the bottleneck (I HOPE !!)
                    // But this would be easily parallelisable
                    std::thread t([this]() // TODO: remove this
                    {
                      if (current->gl_task_list.size() > 1)
                        std::sort(current->gl_task_list.begin(), current->gl_task_list.end());
                    });
                    if (current->other_task_list.size() > 1)
                      std::sort(current->other_task_list.begin(), current->other_task_list.end());
                    t.join();
                  }
                  else
                  {
                    if (current->gl_task_list.size() > 1)
                      std::sort(current->gl_task_list.begin(), current->gl_task_list.end());
                    if (current->other_task_list.size() > 1)
                      std::sort(current->other_task_list.begin(), current->other_task_list.end());
                  }
                }

                // we have the ownership of the sort_flag
                sort_flag.clear(std::memory_order_release);
              }
              else
              {
                // No constraints other than atomicity here.
                // Thus this bit of code is quite overkill, there isn't any other fast alternative...
                // (I won't introduce conditional variables here 'cause it would be really overkill !!)
                // And with some kind of tri-state flags it would have been possible to make the sort of the 4 array parallel
                // (and why not an atomic<int> ? atomic_flag is *guaranted* to be lock free).
                while (sort_flag.test_and_set(std::memory_order_acquire))
                  std::this_thread::yield();
                sort_flag.clear(std::memory_order_release);
              }

              // Here we have both this->current that is correctly set and our different lists that are sorted.

              try // We HAVE to decrement thread_count !
              {
                double inital_time = frame_chrono.now() - frame_chrono.get_accumulated_time();
                double end_time = inital_time + maximum_run_duration;

                // We skip OT in the GL thread only if the task repartition is enforced
                bool skip_ot = current->other_task_list.empty() || (is_gl_thread && multiple_threads && stable_second_thread && task_repartition);

                // We skip GL in all threads except in the gl thread
                bool skip_gl = !is_gl_thread;//!(is_gl_thread && current->gl_task_list.size());

                bool prioritize_gl_tasks = is_gl_thread && multiple_threads && stable_second_thread;

                task *ot_task_ptr = nullptr;
                task *gl_task_ptr = nullptr;

                if (current->other_task_list.empty())
                  other_task_in_time_this_frame.store(true, std::memory_order_relaxed);
                if (current->gl_task_list.empty())
                  gl_task_in_time_this_frame = true;

                if (skip_gl && skip_ot)
                {
                  frame_sync_lock = 1;
                  --active_thread_count;
                  return;
                }

                bool is_timer_to_account = gl_thread_entered.load(std::memory_order_acquire);
                size_t cnt = 0;

                // frame_chrono.get_accumulated_time() wouldn't cause any thread-safety problem as the method is const.
                while ((!is_timer_to_account || frame_chrono.get_accumulated_time() < time_limit) && frame_sync_lock >= 0)
                {
                  if (!is_timer_to_account)
                  {
                    is_timer_to_account = gl_thread_entered.load(std::memory_order_acquire);
                    if (is_timer_to_account) // We now know when the frame will end, so let's set good termination points !
                    {
                      inital_time = frame_chrono.now() - frame_chrono.get_accumulated_time();
                      end_time = inital_time + maximum_run_duration;
                    }
                  }

                  if (skip_ot && skip_gl)
                    break;

                  // We only run GL tasks in the GL thread (that sounds like that could makes sense)
                  // gl tasks handling is simpler, we don't have to be thread safe
                  if (!skip_gl && current->gl_task_list_index < current->gl_task_list.size())
                  {
                    gl_task_ptr = &current->gl_task_list[current->gl_task_list_index];
                    if (gl_task_ptr->registered_ts + gl_task_ptr->get_task_control().delay >= end_time)
                    {
                      gl_task_ptr = nullptr;
                      skip_gl = true;
                    }
                    else
                      ++current->gl_task_list_index;
                  }
                  else
                    skip_gl = true;
                  if (gl_task_ptr && gl_task_ptr->get_task_control().dismissed)
                  {
                    if (gl_task_ptr->get_task_control().then)
                      gl_task_ptr->get_task_control().then(gl_task_ptr->get_task_control()); // Call the then() method
                    gl_task_ptr->get_task_control().registered = false;
                    ot_task_ptr->end();
                    gl_task_ptr = nullptr;
                    continue;
                  }

                  // look for the next non-gl task
                  if (((prioritize_gl_tasks && skip_gl) || (!prioritize_gl_tasks)) && !skip_ot && !ot_task_ptr)
                  {
                    const size_t grab_task = current->other_task_list_index++;
                    if (grab_task < current->other_task_list.size())
                    {
                      ot_task_ptr = &current->other_task_list[grab_task];
                      if (ot_task_ptr->registered_ts + ot_task_ptr->get_task_control().delay >= end_time)
                      {
                        // We've gone too far, this task can't be run in the interval time dedicated for this frame
                        skip_ot = true;
                        other_task_in_time_this_frame.store(true, std::memory_order_relaxed);
                        double registered_ts = ot_task_ptr->registered_ts;
                        task_control &ctrl = push_task(ot_task_ptr->get_task_control());
                        ot_task_ptr->ctrl = nullptr;
                        ctrl.linked_task->registered_ts = registered_ts;
                        ot_task_ptr = nullptr;
                      }
                    }
                    else
                    {
                      other_task_in_time_this_frame.store(true, std::memory_order_relaxed);
                      skip_ot = true;
                    }
                  }
                  if (ot_task_ptr && ot_task_ptr->get_task_control().dismissed)
                  {
                    if (ot_task_ptr->get_task_control().then)
                      ot_task_ptr->get_task_control().then(ot_task_ptr->get_task_control()); // Call the then() method
                    ot_task_ptr->get_task_control().registered = false;
                    ot_task_ptr->end();
                    ot_task_ptr = nullptr;
                    continue;
                  }

                  if (gl_task_ptr || ot_task_ptr) // We have something to run !
                  {
                    task *t = nullptr;
                    float ts_run_gl = gl_task_ptr ? gl_task_ptr->registered_ts + gl_task_ptr->get_task_control().delay : INFINITY;
                    float ts_run_ot = ot_task_ptr ? ot_task_ptr->registered_ts + ot_task_ptr->get_task_control().delay : INFINITY;

                    if (!ot_task_ptr || ts_run_gl <= ts_run_ot)
                    {
                      t = gl_task_ptr;
                      gl_task_ptr = nullptr;
                    }
                    else if (!gl_task_ptr || ts_run_ot < ts_run_gl)
                    {
                      t = ot_task_ptr;
                      ot_task_ptr = nullptr;
                    }

                    const double now = frame_chrono.now();
                    float delta = t->get_task_control().delay;
                    double fnow = now;
                    if (delta <= 0.f || end_time < now) // We tend to run in advance whenever it's possible, but in that case we may be late !
                    {
                      delta = now - t->registered_ts;
                      fnow = t->registered_ts + delta;
                    }
                    try
                    {
                      ++cnt;
                      t->get_task_control().registered = false;
                      t->get_task_control().run_func(delta * speed_factor, t->get_task_control(), fnow); // Here we go ! we run that func !
                      if (!t->get_task_control().registered && t->get_task_control().then)
                        t->get_task_control().then(t->get_task_control());
                      t->end();
                    }
                    catch (std::exception &e) { neam::cr::out.error() << LOGGER_INFO << "task::scheduler::run_some(): caught exception: " << e.what() << std::endl; }
                    catch (...) { neam::cr::out.error() << LOGGER_INFO << "task::scheduler::run_some(): caught unknown exception" << std::endl; }
                  }
                }

                if (is_gl_thread && skip_gl)
                  gl_task_in_time_this_frame = true;

                // We have a remaining GL task, discard it
                if (gl_task_ptr)
                  --current->gl_task_list_index;

                // We have a remaining "Other" task, push it back
                if (ot_task_ptr)
                {
                  double registered_ts = ot_task_ptr->registered_ts;
                  task_control &ctrl = push_task(ot_task_ptr->get_task_control());
                  ot_task_ptr->ctrl = nullptr;
                  ctrl.linked_task->registered_ts = registered_ts;
                }
              }
              // Don't allow to die without decrementing the thread_count...
              catch (std::exception &e) { neam::cr::out.error() << LOGGER_INFO << "task::scheduler::run_some(): caught exception: " << e.what() << std::endl; }
              catch (...) { neam::cr::out.error() << LOGGER_INFO << "task::scheduler::run_some(): caught unknown exception" << std::endl; }

              frame_sync_lock = 1; // END.
              --active_thread_count;
            }

          private:
            // flags
            bool multiple_threads = true;
            bool stable_second_thread = false;
            bool task_repartition = false;

            bool no_critical_messages_about_wait = false;

            // settings
            float maximum_run_duration = 1.f / 120.f;
            float speed_factor = 1.f;

            // infos
            float lateness = 0.f;
            float this_second_frame_count = 0.f;
            float this_second_advancement = 0.f;
            float last_second_lateness = 0.25f;

            size_t critical_hit = 0;

            bool gl_task_in_time_this_frame = false;
            std::atomic<bool> other_task_in_time_this_frame = ATOMIC_VAR_INIT(false);

            std::atomic<int> frame_sync_lock = ATOMIC_VAR_INIT(0);
            std::atomic<bool> gl_thread_entered = ATOMIC_VAR_INIT(false);

            // task lists / ...
            struct task_buffers
            {
              // NOTE: There are ton of advantages in having vectors instead of lists, the first one being a lockfree/waitfree
              //       access during the run_some call (we only have one atomic index per list)
              //       But then, insertion could be slow (O(n) at worst case, but we simply have to preallocate enough to avoid this case)
              //       The only issue remaining is the sorting of the array that has to be done one time per frame (O(n.log n)), but considering
              //       how C++ containers are made, there isn't a way to have an already sorted container (like std::set) that allow a O(1) pop_front operation.
              //       The only solution appear to be in having a unsorted vector (O(1) amortized, O(n) worst, but avoidable), sort it one time (O(n.log n))
              //       and use it in a waitfree way (leaving the duty of cleaning the vector (O(n)) to the end_frame() method)
              //       But then, the insert() operation for immediate task list will no more be O(1)...

              std::vector<neam::bleunw::yaggler::task::task> gl_task_list;
              size_t gl_task_list_index = 0;
              std::vector<neam::bleunw::yaggler::task::task> other_task_list;
              std::atomic<size_t> other_task_list_index = ATOMIC_VAR_INIT(0);

              neam::spinlock push_lock;

              // NOTE This is a very special array containing tasks that have to be done
              //      At the pre-post-frame level (after ALL tasks have been performed, but before the buffer swap)
              //      All those tasks are called into frame_end, after ensuring all the threads have done their work
              //      All those tasks are called EVEN IF the scheduler is late, meaning that those tasks should only be GL DRAW CALLS
              //      The lateness factor don't account them.
              std::vector<neam::bleunw::yaggler::task::task> gl_pre_swap_task_list;

              /// \brief clear everything
              void clear()
              {
                gl_task_list.clear();
                gl_task_list_index = 0;
                other_task_list.clear();
                other_task_list_index = 0;
                gl_pre_swap_task_list.clear();
              }

              /// \brief free unused memory in containers
              void shrink_to_fit()
              {
                gl_task_list.shrink_to_fit();
                other_task_list.shrink_to_fit();
              }

              /// \brief insert (merge-splice) for re-use (mostly for non-run tasks)
              void insert(task_buffers &o)
              {
                if (&o == this)
                  return;

                // immediate/short delay tasks
                gl_task_list.reserve(o.gl_task_list.size());
                for (size_t i = o.gl_task_list_index; i < o.gl_task_list.size(); ++i)
                  gl_task_list.emplace_back(std::move(o.gl_task_list[i]));

                other_task_list.reserve(o.other_task_list.size());
                for (size_t i = o.other_task_list_index; i < o.other_task_list.size(); ++i)
                  other_task_list.emplace_back(std::move(o.other_task_list[i]));
              }
            };

            // task pointers
            task_buffers *current = nullptr; // WARNING: Only valid into run_some() !!!
            std::atomic<unsigned int> index = ATOMIC_VAR_INIT(0);

            std::atomic_flag sort_flag = ATOMIC_FLAG_INIT; // not set: done, set: not done

            // conf
            neam::cr::chrono chrono; // the main chronometer of the scheduler
            neam::cr::chrono frame_chrono; // the per-frame chronometer of the scheduler

            // others
            std::thread::id gl_thread_id;
            std::atomic<int> active_thread_count;
            task_buffers buffers[2];

        };
      } // namespace task
    } // namespace yaggler
  } // namespace bleunw
} // namespace neam

#define TASK_SCHEDULER_DEFINED
#include "task_control.hpp"
#undef TASK_SCHEDULER_DEFINED

#endif /*__N_1565169418532046895_210799306__TASK_SCHEDULER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

