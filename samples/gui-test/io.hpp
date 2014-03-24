//
// file : io.hpp (2)
// in : file:///home/tim/projects/yaggler/samples/test-klmb/io.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 24/02/2014 11:02:03
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

#ifndef __N_19611469271973957199_1668343139__IO_HPP__2___
# define __N_19611469271973957199_1668343139__IO_HPP__2___

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iosfwd>

std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec)
{
  out << " [ " << vec.x << " " << vec.y << " " << vec.z << " ] ";
  return out;
}
std::istream &operator>> (std::istream &in, glm::vec3 &vec)
{
  char c;
  in >> c >> vec.x >> vec.y >> vec.z >> c;
  return in;
}

std::ostream &operator<< (std::ostream &out, const glm::quat &q)
{
  out << " [ " << q.x << " " << q.y << " " << q.z << " " << q.w << " ] ";
  return out;
}
std::istream &operator>> (std::istream &in, glm::quat &q)
{
  char c;
  in >> c >> q.x >> q.y >> q.z >> q.w >> c;
  return in;
}


#endif /*__N_19611469271973957199_1668343139__IO_HPP__2___*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

