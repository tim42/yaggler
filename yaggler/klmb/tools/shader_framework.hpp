//
// file : shader_framework.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/tools/shader_framework.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 05/02/2014 16:14:58
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

#ifndef __N_7454232491046447101_1059789292__SHADER_FRAMEWORK_HPP__
# define __N_7454232491046447101_1059789292__SHADER_FRAMEWORK_HPP__

#include <string>
#include <fstream>
#include <sstream>

#include <cstdlib>

#include <klmb/tools/tools.hpp>

// lovely defines for GLSL * shaders
#define KLMB_YAGGLER_GLOBAL_DEFS                "./data/klmb-framework/defs.glsl"

// lovely defines for GLSL fragments
#define KLMB_YAGGLER_FRAGMENT_DEFS_FILE         "./data/klmb-framework/defs.frag"
#define KLMB_YAGGLER_FRAGMENT_MAIN_FILE         "./data/klmb-framework/main.frag"


namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      namespace internal
      {
        // that ugly framework data structure...
        struct _shader_framework_data
        {
          uint8_t fragment__output_buffers = 0;
          uint8_t fragment__shader_number = 0;
          bool    fragment__framework_registered = false;
        };

        // the basical one
        template<GLenum ShaderType>
        struct setup_shader_framework
        {
          template<typename Shader>
          static uint8_t setup(Shader &, uint8_t , _shader_framework_data &)
          {
            // do nothing :)
            // TODO: remove this.
            // (or static assert for a shader type beeing unsupported
            return 0;
          }
        };


        // the others
        template<>
        struct setup_shader_framework<GL_FRAGMENT_SHADER>
        {
          template<typename Shader>
          static uint8_t setup(Shader &shader, uint8_t, _shader_framework_data &fdata)
          {
            bool is_entry_point = tools::is_true(shader.get_preprocessor_value("KLMB_IS_ENTRY_POINT"));
            bool is_framework_main = tools::is_true(shader.get_preprocessor_value("KLMB_FRAMEWORK_MAIN"));

            if (is_framework_main && fdata.fragment__framework_registered)
              std::cerr << "K:LMB/YAGGLER : ERROR : material: fragment shader framework: framework has already been registered." << std::endl;

            fdata.fragment__framework_registered |= is_framework_main;

            // get output buffer number
            uint8_t number_of_output_buffer = atoi(shader.get_preprocessor_value("KLMB_NUMBER_OF_OUTPUT_BUFFER").data());
            number_of_output_buffer = fdata.fragment__output_buffers = std::max(fdata.fragment__output_buffers, number_of_output_buffer);

            // setup / load defs
            std::string base_framework_defs = "#define KLMB_PROG_ID " + std::string(__KLMB__VAR_TO_STRING(static_cast<uint16_t>(fdata.fragment__shader_number))) + '\n';
            base_framework_defs += "#define KLMB_PROG_NUMBER " + __KLMB__VAR_TO_STRING(static_cast<uint16_t>(fdata.fragment__shader_number)) + '\n';
            if (is_framework_main)
            {
              base_framework_defs += "#define KLMB_IS_MAIN\n";
              base_framework_defs += "#define KLMB_TOTAL_PROG_NUMBER " + __KLMB__VAR_TO_STRING(static_cast<uint16_t>(fdata.fragment__shader_number)) + '\n';
              base_framework_defs += "#define KLMB_TOTAL_OUTPUT_BUFFER_NUMBER " + __KLMB__VAR_TO_STRING(static_cast<uint16_t>(fdata.fragment__output_buffers)) + '\n';
            }
            base_framework_defs += __KLMB__FILE_TO_STRING(KLMB_YAGGLER_GLOBAL_DEFS) + "\n";

            // framework main specifics
            if (is_framework_main)
            {
              // for-each output buffers
              std::ostringstream fe_ob;
              fe_ob << "#define KLMB_FOR_EACH_OUTPUT_BUFFER(x) ";
              for (size_t i = 0; i < number_of_output_buffer; ++i)
                fe_ob << "x(" << i << ") ";
              fe_ob << "\n";

              // for-each shaders
              std::ostringstream fe_shad;
              fe_shad << "#define KLMB_FOR_EACH_SHADER(x) ";
              for (size_t i = 0; i < fdata.fragment__shader_number; ++i)
                fe_shad << "x(" << i << ") ";
              fe_shad << "\n";

              // for-each shaders {for-each output buffers}
              std::ostringstream fe_shad_ob;
              fe_shad_ob << "#define KLMB_FOR_EACH_SHADER_AND_OUTPUT_BUFFER(x) ";
              for (size_t i = 0; i < fdata.fragment__shader_number; ++i)
              {
                for (size_t j = 0; j < number_of_output_buffer; ++j)
                  fe_shad_ob << "x(" << i << "," << j << ") ";
              }
              fe_shad_ob << "\n";

              base_framework_defs += fe_ob.str() + fe_shad.str() + fe_shad_ob.str();
            }

            // cleanup 'additional strings'
            shader.clear_additional_strings();
            shader.append_to_additional_strings(base_framework_defs);

            // increment the prog counter.
            if (is_entry_point)
            {
              ++fdata.fragment__shader_number;
              return 1;
            }
            return 0;
          }
        };

      } // namespace internal
    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_7454232491046447101_1059789292__SHADER_FRAMEWORK_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

