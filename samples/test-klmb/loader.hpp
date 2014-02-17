//
// file : loader.hpp
// in : file:///home/tim/projects/yaggler/samples/test-klmb/loader.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 16/02/2014 10:16:32
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

#ifndef __N_1132275967347556179_1237476321__LOADER_HPP__
# define __N_1132275967347556179_1237476321__LOADER_HPP__

#include <string>
#include <fstream>

#include <klmb/klmb.hpp>
#include <yaggler.hpp>

namespace neam
{
  namespace klmb
  {
    namespace sample
    {
      // load a model from ply file
      // (fuckingly simple loader)
      neam::klmb::yaggler::object<GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER> load_model(const std::string &filename)
      {
        std::ifstream file(filename);

        std::string line;
        std::getline(file, line);

        // filetype
        if (line != "ply")
          throw neam::yaggler::yaggler_exception("bad model format: not ply");

        // 'parse' header
        long vert_count = 0;
        long tri_count = 0;

        while (std::getline(file, line))
        {
          if (line.substr(0, 15) == "element vertex ")
            vert_count = atol(line.substr(15).data());
          if (line.substr(0, 13) == "element face ")
            tri_count = atol(line.substr(13).data());
          if (line == "end_header")
            break;
        }

        // allocate the arrays
        glm::vec3 *vertex = new glm::vec3[vert_count];
        GLuint *idxs = new GLuint[tri_count * 3];

        long i = 0;
        std::istringstream is;
        while (i < vert_count && std::getline(file, line))
        {
          is.clear();
          is.str(line);
          is >> vertex[i].x >> vertex[i].y >> vertex[i].z;
          ++i;
        }

        i = 0;
        while (i < tri_count && std::getline(file, line))
        {
          int d;
          is.clear();
          is.str(line);
          is >> d >> idxs[i * 3 + 0] >> idxs[i * 3 + 1] >> idxs[i * 3 + 2];
          ++i;
        }

        // setup object
        neam::klmb::yaggler::object<GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER> object;
        // setup drawer
        object.drawer.set_draw_method(neam::yaggler::geometry::draw_method::indexed);
        object.drawer.set_draw_triangles(tri_count);
        object.drawer.set_index_type(GL_UNSIGNED_INT);

        // YägGLer stuff
        object.ct_buffers.get_ref<0>().set_data(neam::array_wrapper<glm::vec3>(vertex, vert_count));
        object.ct_buffers.get_ref<1>().set_data(neam::array_wrapper<GLuint>(idxs, tri_count * 3));

        // add buffers to vao
        object.vao.add_buffer(object.ct_buffers.get_ref<0>(), neam::yaggler::geometry::buffer_view<neam::yaggler::type::opengl, neam::embed::geometry::destination_precision<neam::yaggler::geometry::destination_precision::single_precision>>
                       (0, 3, GL_FLOAT, 0, 0));
        object.vao.add_buffer(object.ct_buffers.get_ref<1>());

        std::cout << "YAGGLER: loaded model file  '" << filename << "'" << " with " << tri_count << " triangles and " << vert_count << " vertices" << std::endl;

        return std::move(object);
      }
    } // namespace sample
  } // namespace klmb
} // namespace neam

#endif /*__N_1132275967347556179_1237476321__LOADER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

