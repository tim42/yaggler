//
// file : compute_framework.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/compute/compute_framework.hpp
//
// created by : Timothée Feuillet on linux-vnd3.site
// date: 19/11/2015 21:05:50
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

#ifndef __N_18147732601314200755_2068880273__COMPUTE_FRAMEWORK_HPP__
# define __N_18147732601314200755_2068880273__COMPUTE_FRAMEWORK_HPP__

#include <glm/glm.hpp>

#include <version.hpp>
#include <klmb/tools/tools.hpp>
#include "compute.hpp"

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      namespace compute_framework_files
      {
//         extern constexpr neam::string_t main_compute = "data/klmb-framework/compute.frag";
      }

      namespace compute_internal
      {
        // that ugly framework data structure...
        struct _shader_framework_data
        {
          // global stuff (MUST be defined by a framework data)
          uint8_t       fragment_shader_number = 0;
          uint8_t       geometry_shader_number = 0;
          uint8_t       tes_shader_number = 0;
          uint8_t       tcs_shader_number = 0;
          uint8_t       vertex_shader_number = 0;
          uint8_t       compute_shader_number = 0;
        };

        // the default one
        template<GLenum ShaderType>
        struct setup_shader_framework
        {
          template<typename Shader>
          static uint8_t setup(Shader &shdr, uint8_t, _shader_framework_data &)
          {
            std::string shader_type;
            switch (ShaderType)
            {
              case GL_VERTEX_SHADER: shader_type = "vertex"; break;
              case GL_FRAGMENT_SHADER: shader_type = "fragment"; break;
              case GL_GEOMETRY_SHADER: shader_type = "geometry"; break;
              case GL_TESS_CONTROL_SHADER: shader_type = "tess-control"; break;
              case GL_TESS_EVALUATION_SHADER: shader_type = "tess-eval"; break;
              default: shader_type = "[hu...]";
            };
            neam::cr::out.warning() << LOGGER_INFO << "K:LMB Compute shader framework: Unsupported " << shader_type << " shader type (" << shdr.get_source_name() << ")" << std::endl;
            return 0;
          }
        };

        // compute shader stuff
        template<>
        struct setup_shader_framework<GL_COMPUTE_SHADER>
        {
          template<typename Shader>
          static uint8_t setup(Shader &shader, uint8_t, _shader_framework_data &)
          {
            std::string framework_string;

            // LIMITS
            std::string local_size_str = shader.environment.get_preprocessor_value("KLMB_WG_SIZE_CHECK");
            bool get_limits = tools::is_true(shader.environment.get_preprocessor_value("KLMB_GET_LIMITS"));
            if (!local_size_str.empty())
            {
              std::istringstream is(local_size_str);
              glm::ivec3 local_size(0, 0, 0);
              is >> local_size.x >> local_size.y >> local_size.z;
              unsigned int comb_size = local_size.x * local_size.y * local_size.z;

              glm::ivec3 max_size = compute_shader::get_max_work_group_size();
              unsigned int max_comb_size = compute_shader::get_max_work_group_invocations();
              if (local_size.x > max_size.x || local_size.y > max_size.y || local_size.z > max_size.z || comb_size > max_comb_size || get_limits)
              {
                framework_string += "#define MAX_WG_SIZE_X " + __KLMB__VAR_TO_STRING(max_size.x) + "\n";
                framework_string += "#define MAX_WG_SIZE_Y " + __KLMB__VAR_TO_STRING(max_size.y) + "\n";
                framework_string += "#define MAX_WG_SIZE_Z " + __KLMB__VAR_TO_STRING(max_size.z) + "\n";
                framework_string += "#define MAX_COMB_SIZE " + __KLMB__VAR_TO_STRING(max_comb_size) + "\n";

                if (get_limits)
                  framework_string += "#define SHARED_MEM_SIZE " + __KLMB__VAR_TO_STRING(compute_shader::get_max_shared_memory_size()) + "\n";

                if (local_size.x > max_size.x || local_size.y > max_size.y || local_size.z > max_size.z)
                  framework_string += "#define KLMB_WG_SIZE_CHECK_FAIL\n";
                if (comb_size > max_comb_size)
                {
                  framework_string += "#define KLMB_WG_COMBINED_SIZE_CHECK_FAIL\n";
                  framework_string += "#define COMB_SIZE " + __KLMB__VAR_TO_STRING(comb_size) + "\n";
                }
              }
            }

            // cleanup 'additional strings'
            shader.environment.clear_additional_strings();
            shader.environment.append_to_additional_strings(framework_string);

            return 1;
          }
        };
      } // namespace compute_internal

      /// \brief A KLMB compute shader framework
      /// User can define some macros to control the shader behavior. They are:
      ///   KLMB_LOCAL_SIZE_CHECK x Y Z : Allow a runtime check of the local size validity
      ///       If the limits aren't respected (either combined or single limits) the macros are defined:
      ///         KLMB_WG_SIZE_CHECK_FAIL           if a component is bigger than its limit
      ///         KLMB_WG_COMBINED_SIZE_CHECK_FAIL  if the combined size is bigger than the limit
      ///         MAX_WG_SIZE_{X,Y,Z} [max-size]    individual maximum size
      ///         MAX_COMB_SIZE [max-comb-size]     the maximum combined size
      ///         COMB_SIZE [comb-size]             if KLMB_WG_COMBINED_SIZE_CHECK_FAIL defined, the current combined size
      ///       This check allow to set a smaller work group size if your shader exceed the limit
      ///  KLMB_GET_LIMITS : Get the limits:
      ///       This macro trigger the definition of the following macros:
      ///         MAX_WG_SIZE_{X,Y,Z} [max-size]    individual maximum size
      ///         MAX_COMB_SIZE [max-comb-size]     the maximum combined size
      ///         SHARED_MEM_SIZE [max-shared-size] the size of the shared memory
      /// As for every framework, you have to add the KLMB_IS_USING_FRAMEWORK
      /// You can enforce the use of this framework by adding
      /// \code #define FRAMEWORK K:LMB-COMPUTE \endcode
      /// And for enforcing the framework version:
      /// \code #define FRAMEWORK_VERSION [your.yaggler.version.number] \endcode
      ///
      /// \note This is a different framework than the K:LMB "default" one 'cause K:LMB default framework
      ///       allows multiple entry points, distinct variables that have the same name between compilation units
      ///       and this is not something you may want in a compute shader
      ///
      /// \todo Add a way to enforce memory barriers in the compute_shader::dispatch_*() methods
      struct compute_shader_framework
      {
        template<GLenum ShaderType>
        using setup_framework = compute_internal::setup_shader_framework<ShaderType>;

        using framework_data = compute_internal::_shader_framework_data;

        using framework_files = ct::type_list
        <
          // Framework files
//           ct::pair<embed::GLenum<GL_COMPUTE_SHADER>, auto_file_shader<compute_framework_files::main_compute>>
        >;

        static constexpr neam::string_t framework_name = "K:LMB-COMPUTE";
        static constexpr const char *framework_version = *&neam::yaggler::version::short_string;
      };
      constexpr neam::string_t compute_shader_framework::framework_name;
      constexpr const char *compute_shader_framework::framework_version;
    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_18147732601314200755_2068880273__COMPUTE_FRAMEWORK_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

