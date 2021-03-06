//
// file : except.hpp
// in : file:///home/tim/projects/yaggler/yaggler/shader/except.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 18/10/2013 02:37:59
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


#ifndef __N_1173249597772044501_1956632000__EXCEPT_HPP__
# define __N_1173249597772044501_1956632000__EXCEPT_HPP__

#include <yaggler_except.hpp>

namespace neam
{
  namespace yaggler
  {
    namespace shader
    {
      /// \brief a class of exception related with shaders
      class shader_exception : public opengl_exception
      {
        public:
          explicit shader_exception(const std::string &__arg, const std::string &file, size_t line) noexcept
           : opengl_exception(__arg, file, line)
          {
          }

          virtual ~shader_exception() noexcept
          {
          }

      private:
      };
    } // namespace shader
  } // namespace yaggler
} // namespace neam

#endif /*__N_1173249597772044501_1956632000__EXCEPT_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

