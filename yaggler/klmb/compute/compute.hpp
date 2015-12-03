//
// file : compute.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/compute/compute.hpp
//
// created by : Timothée Feuillet on linux-vnd3.site
// date: 18/11/2015 20:24:20
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

#ifndef __N_1000661691497218312_1267877902__COMPUTE_HPP__
# define __N_1000661691497218312_1267877902__COMPUTE_HPP__

#include <GLEW/glew.h>
#include "../material/generic_material.hpp"

#include <geometry/opengl_buffer.hpp>

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      /// \brief Handle a compute shader
      /// \see compute_shader_framework to get a simple compute shader framework
      class compute_shader
      {
        public:
          compute_shader() {}
          compute_shader(material_wrapper &&_mw) : mw(std::move(_mw)) {}
          compute_shader(const material_wrapper &_mw) : mw(_mw) {}

          ~compute_shader() {}

          /// \brief Set/change the compute shader
          void set_compute_shader(material_wrapper && _mw)
          {
            mw = std::move(_mw);
          }

          /// \brief Set/change the compute shader
          void set_compute_shader(const material_wrapper &_mw)
          {
            mw = _mw;
          }

          /// \brief Ask the driver to start the compute shader using a previously defined work group count
          void dispatch()
          {
            if (!mw.is_empty())
            {
              mw.use();
              glDispatchCompute(wg_count.x, wg_count.y, wg_count.z);
            }
          }

          /// \brief Ask the driver to start the compute shader, also, set the work group count
          void dispatch(const glm::uvec3 &count)
          {
            wg_count = count;
            dispatch();
          }

          /// \brief Ask the driver to start the compute shader using a work group count stored in a buffer object
          template<typename... BufferTypes>
          void dispatch_indirect(neam::yaggler::geometry::buffer<neam::yaggler::type::opengl, BufferTypes...> buffer, size_t offset = 0)
          {
            if (!mw.is_empty())
            {
              buffer.bind_to_target(GL_DISPATCH_INDIRECT_BUFFER);
              mw.use();
              glDispatchComputeIndirect(offset);
            }
          }

          /// \brief set the work group count
          void set_work_group_count(const glm::uvec3 &count)
          {
            wg_count = count;
          }

          /// \brief set the work group count
          glm::uvec3 get_work_group_count() const
          {
            return wg_count;
          }

          // static helpers

          /// \brief Defines a barrier ordering memory transactions
          /// \note default value is the most strict possible
          /// \link https://www.opengl.org/sdk/docs/man4/html/glMemoryBarrier.xhtml
          static void memory_barrier(GLbitfield barriers = GL_ALL_BARRIER_BITS)
          {
            glMemoryBarrier(barriers);
          }

#ifdef glMemoryBarrierByRegion // Well that solve GLEW not supporting some of the GL 4.5 things
          /// \brief Defines a barrier ordering memory transactions, but narrow the region under consideration
          ///        to a smaller region.
          /// \note default value is the most strict possible
          /// \link https://www.opengl.org/sdk/docs/man4/html/glMemoryBarrier.xhtml
          static void memory_barrier_by_region(GLbitfield barriers = GL_ALL_BARRIER_BITS)
          {
            glMemoryBarrierByRegion(barriers);
          }
#endif

          /// \brief Return the maximum number of work group available on this GPU
          /// \link https://www.opengl.org/wiki/Compute_Shader#Limitations
          static glm::ivec3 get_max_work_group_count()
          {
            glm::ivec3 ret;
            glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &ret.x);
            glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &ret.y);
            glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &ret.z);
            return ret;
          }

          /// \brief Return the maximum size of work group allowed on this GPU
          /// \link https://www.opengl.org/wiki/Compute_Shader#Limitations
          static glm::ivec3 get_max_work_group_size()
          {
            glm::ivec3 ret;
            glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &ret.x);
            glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &ret.y);
            glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &ret.z);
            return ret;
          }

          /// \brief Return the maximum work group invocations allowed on this GPU
          /// (this is the maximum X * Y * Z combined work group size)
          /// \link https://www.opengl.org/wiki/Compute_Shader#Limitations
          static int get_max_work_group_invocations()
          {
            int ret;
            glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &ret);
            return ret;
          }

          /// \brief Return the maximum shared memory size available on this GPU
          /// \link https://www.opengl.org/wiki/Compute_Shader#Limitations
          static int get_max_shared_memory_size()
          {
            int ret;
            glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &ret);
            return ret;
          }

        private:
          material_wrapper mw;
          glm::uvec3 wg_count;
      };
    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_1000661691497218312_1267877902__COMPUTE_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

