//
// file : font.hpp
// in : file:///home/tim/projects/yaggler/yaggler/bleunw/gui/font.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 11/03/2014 16:21:08
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

#ifndef __N_1292215925922071592_2040385089__FONT_HPP__
# define __N_1292215925922071592_2040385089__FONT_HPP__

#include <string>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>

#include <yaggler_except.hpp>

#include <tools/ct_string.hpp>

#include <texture/texture_base.hpp>
#include <texture/opengl_texture.hpp>
#include <texture/png_loader.hpp>
#include <texture/png_writer.hpp>

#include <geometry/opengl_buffer.hpp>
#include <geometry/opengl_buffer_view.hpp>

namespace neam
{
  namespace bleunw
  {
    namespace yaggler
    {
      namespace gui
      {
        struct fixed_t {} fixed __attribute__((unused));

        // font 'descriptor'
        // (allow something more than only a fixed size font)
        struct font_face
        {
          // NOTE: the version is part of the magic.
          static constexpr string_t magic_line = "[bfont 0.1]";

          // types
          struct character_nfo
          {
            // homogenous coordinates [0-1] (uv coordinates)
            // {0,0} is 'top-left'
            glm::vec2 lower_pos; // +---
            glm::vec2 upper_pos; // ---+

            // where is the range lower_pos/upper_pos is located the original glyph (for X: |----+---------+----|)
            //                                                                               dt.x           dt.x
            glm::vec2 dt; // in homogenous coordinates.

            glm::vec2 left_top; // almost as DT, but for positioning the upper left corner in homogenous coordinates

            float x_inc;
          };

          font_face()
          {
            for (uint16_t i = 0; i < 256; ++i)
            {
              table[i].lower_pos = glm::vec2(i % 16, i / 16) / 16.f;
              table[i].upper_pos = glm::vec2(i % 16 + 1, i / 16 + 1) / 16.f;
              table[i].dt = glm::vec2(0, 0);
              table[i].left_top = glm::vec2(0, 0);
              table[i].x_inc = 1.f / 16.f;
            }

            vbo.set_data(table);
          }

          // init the font for a fixed size font
          font_face(fixed_t, const std::string &font_texture)
          {
            for (uint16_t i = 0; i < 256; ++i)
            {
              table[i].lower_pos = glm::vec2(i % 16, i / 16) / 16.f;
              table[i].upper_pos = glm::vec2(i % 16 + 1, i / 16 + 1) / 16.f;
              table[i].dt = glm::vec2(0, 0);
              table[i].left_top = glm::vec2(0, 0);
              table[i].x_inc = 1.f / 16.f;
            }

            vbo.set_data(table);

            // use the png loader (to load something grey. Distance field.)
            neam::yaggler::texture::options::png_texture_loader<GL_RGBA8> loader(font_texture);
            font.set_texture_data(loader);
            font.generate_mipmaps();
            font.set_gl_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            font.set_gl_parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          }

          // init the font from a file containing the infos of the font
          // (bleunw fonts are a couple of a .bfont and a .png (or .whatever) files)
          // I now, this is a crapy loader. But it works. And is simple.
          font_face(const std::string &init_file)
          {
            // bfont format:

            // magic line
            // font texture file, _relative to the binary_
            // char-value [ x y ]   [ x y ]  [ x y ] [ x y ] x_inc
            //           lower_pos upper_pos   dt      l-t

#ifndef YAGGLER_NO_MESSAGES
            neam::cr::chrono timer;
#endif

            std::string line;
            std::ifstream file(init_file);
            if (!file)
              throw neam::runtime_error<font_face>("bfont file not found: '" + init_file + "'", __FILE__, __LINE__);

            // get the magic line
            std::getline(file, line);
            if (line != magic_line)
              throw neam::runtime_error<font_face>("bad magic bfont number", init_file, 1);

            // get the font texture
            std::string font_texture;
            std::getline(file, font_texture);

            // use the png loader (to load something grey. Distance field.)
            neam::yaggler::texture::options::png_texture_loader<GL_RGBA8> loader(font_texture);
            font.set_texture_data(loader);
            font.generate_mipmaps();
            font.set_gl_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            font.set_gl_parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // load the table
            size_t line_num = 0;
            while (std::getline(file, line))
            {
              char discard;
              size_t index;
              float x_inc;
              glm::vec2 upper;
              glm::vec2 lower;
              glm::vec2 dt;
              glm::vec2 left_top;
              ++line_num;

              if (line.size())
              {
                std::istringstream is(line);

                // char-value [ x y ]   [ x y ]  [ x y ] [ x y ] x_inc
                //           lower_pos upper_pos   dt      l-t
                is >> index
                   >> discard >> lower.x >> lower.y >> discard
                   >> discard >> upper.x >> upper.y >> discard
                   >> discard >> dt.x >> dt.y >> discard
                   >> discard >> left_top.x >> left_top.y >> discard
                   >> x_inc;

                if (index >= 256)
                  throw  neam::runtime_error<font_face>("index greater than the **hardcoded** array size of 256", init_file, line_num);

                table[index].lower_pos = lower;
                table[index].upper_pos = upper;
                table[index].dt = dt;
                table[index].left_top = left_top;
                table[index].x_inc = x_inc;
              }
            }

            // create the vbo from the table (thx yaggler).
            vbo.set_data(table);

#ifndef YAGGLER_NO_MESSAGES
              neam::cr::out.debug() << LOGGER_INFO_TPL(init_file, 0) << "loaded font face in " << timer.delta() << " seconds" << std::endl;
#endif

          }

          // write the conf into the out_file and the texture into the font_texture
          // The output file could then be loaded with font_face(file).
          bool write_out_font_face(const std::string &out_file, const std::string &font_texture) const
          {
            // bfont format:

            // magic line
            // font texture file, _relative to the binary_
            // char-value [ x y ]   [ x y ]  [ x y ] [ x y ] x_inc
            //           lower_pos upper_pos   dt      l-t

            std::ofstream file(out_file);
            if (!file)
              return false;

            // write the magic line and the texture file line
            file << magic_line << "\n"
                 << font_texture << "\n";

            // char-value [ x y ]   [ x y ]  [ x y ] [ x y ] x_inc
            //           lower_pos upper_pos   dt      l-t

            for (size_t i = 0; i < 256; ++i)
            {
              file << i << " [" << table[i].lower_pos.x << " " << table[i].lower_pos.y << " ] [ "
                   << table[i].upper_pos.x << " " << table[i].upper_pos.y << " ] [ "
                   << table[i].dt.x << " " << table[i].dt.y << " ] [ "
                   << table[i].left_top.x << " " << table[i].left_top.y << " ] "
                   << table[i].x_inc << "\n";
            }
            file.close();

            // use the png writer
            if (!(neam::yaggler::texture::options::png_texture_writer<GL_RGB8>::write(font, font_texture)))
              return false;

            return true;
          }

          // helper to init a vao from this font face
          // NOTE: don't forget that attribute index from 1 to 5 are taken by the font_face !!
          template<typename VaoType>
          void init_vao(VaoType &vt) const
          {
            vt.add_buffer(vbo, lower_pos_view, upper_pos_view, dt_view, left_top_view/*, x_inc_view*/);
          }

          // members
          character_nfo table[256];
          neam::yaggler::texture::texture<neam::yaggler::type::opengl, neam::embed::GLenum<GL_TEXTURE_2D>> font;
          neam::yaggler::geometry::buffer<neam::yaggler::type::opengl, neam::embed::GLenum<GL_ARRAY_BUFFER>> vbo;

          // longer type name ? :D
          neam::yaggler::geometry::buffer_view<neam::yaggler::type::opengl, neam::embed::geometry::destination_precision<neam::yaggler::geometry::destination_precision::single_precision>,
                neam::yaggler::geometry::options::ct_buffer_view_init
                <
                  1,                                    // index
                  2,                                    // size
                  GL_FLOAT,                             // type
                  sizeof(character_nfo),                // stride
                  offsetof(character_nfo, lower_pos)    // offset
                >
               > lower_pos_view;

          neam::yaggler::geometry::buffer_view<neam::yaggler::type::opengl, neam::embed::geometry::destination_precision<neam::yaggler::geometry::destination_precision::single_precision>,
                neam::yaggler::geometry::options::ct_buffer_view_init
                <
                  2,                                    // index
                  2,                                    // size
                  GL_FLOAT,                             // type
                  sizeof(character_nfo),                // stride
                  offsetof(character_nfo, upper_pos)    // offset
                >
               > upper_pos_view;

          neam::yaggler::geometry::buffer_view < neam::yaggler::type::opengl, neam::embed::geometry::destination_precision<neam::yaggler::geometry::destination_precision::single_precision>,
                neam::yaggler::geometry::options::ct_buffer_view_init
                <
                  3,                                    // index
                  2,                                    // size
                  GL_FLOAT,                             // type
                  sizeof(character_nfo),                // stride
                  offsetof(character_nfo, dt)           // offset
                >
               > dt_view;

          neam::yaggler::geometry::buffer_view < neam::yaggler::type::opengl, neam::embed::geometry::destination_precision<neam::yaggler::geometry::destination_precision::single_precision>,
                neam::yaggler::geometry::options::ct_buffer_view_init
                <
                  4,                                    // index
                  2,                                    // size
                  GL_FLOAT,                             // type
                  sizeof(character_nfo),                // stride
                  offsetof(character_nfo, left_top)     // offset
                >
               > left_top_view;

          neam::yaggler::geometry::buffer_view < neam::yaggler::type::opengl, neam::embed::geometry::destination_precision<neam::yaggler::geometry::destination_precision::single_precision>,
                neam::yaggler::geometry::options::ct_buffer_view_init
                <
                  5,                                    // index
                  1,                                    // size
                  GL_FLOAT,                             // type
                  sizeof(character_nfo),                // stride
                  offsetof(character_nfo, x_inc)        // offset
                >
               > x_inc_view;

        };
        constexpr string_t font_face::magic_line;

      } // namespace gui
    } // namespace yaggler
  } // namespace bleunw
} // namespace neam

#endif /*__N_1292215925922071592_2040385089__FONT_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

