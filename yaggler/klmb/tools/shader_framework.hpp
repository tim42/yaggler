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

#include <cstdlib>

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      namespace internal
      {
        // the basical one
        template<GLenum ShaderType>
        struct setup_shader_framework
        {
          template<typename Shader>
          static uint8_t setup(Shader &shader, uint8_t prog_count)
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
          static uint8_t setup(Shader &shader, uint8_t prog_count)
          {
            bool is_entry_point = shader.get_preprocessor_value("KLMB_IS_ENTRY_POINT")[0] == '1';

            uint8_t number_of_output_buffer = atoi(shader.get_preprocessor_value("KLMB_NUMBER_OF_OUTPUT_BUFFERS").data());

            // cleanup 'additional strings'
            shader.clear_additional_strings();

            if (is_entry_point)
              return 1;
            return 0;
          }
        };

      } // namespace internal
    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_7454232491046447101_1059789292__SHADER_FRAMEWORK_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

