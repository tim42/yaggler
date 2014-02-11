//
// file : tools.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/tools/tools.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 10/02/2014 19:01:04
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

#ifndef __N_2314074161230893906_114729529__TOOLS_HPP__
# define __N_2314074161230893906_114729529__TOOLS_HPP__

#include <string>
#include <fstream>
#include <sstream>

// if anybody says there's a shorter way to load a file into a string in C++, perhaps he is lying ;)
// (but is this safe ??)
#define __KLMB__FILE_TO_STRING(filename)        static_cast<std::ostringstream&>(std::ostringstream() << (std::ifstream(filename).rdbuf())).str()
// THIS IS SO UGLY.
#define __KLMB__VAR_TO_STRING(var)              static_cast<std::ostringstream&>(std::ostringstream() << (var)).str()


namespace neam
{
  namespace klmb
  {
    namespace tools
    {
      static inline bool is_true(const std::string &str)
      {
        if (!str.size())
          return false;

        if (str.size() == 1)
          return str[0] != '0';

        return str != "false";
      }
    } // namespace tools
  } // namespace klmb
} // namespace neam

#endif /*__N_2314074161230893906_114729529__TOOLS_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

