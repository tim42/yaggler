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


#include <setup.hpp>

using opengl_version = neam::yaggler::setup::opengl<3, 3, neam::yaggler::setup::opengl_profile::core>;

// disable messages
// #define YAGGLER_NO_MESSAGES

#include <yaggler.hpp>

// lets include klmb main header.
#include <klmb/klmb.hpp>

// lets include some bleunw files
#include <bleunw/gui.hpp>

#include <iostream>
#include <iomanip>
#include <cmath>


#include <ft2build.h>
#include FT_FREETYPE_H

// the blur shader
#include "blur.hpp"

// 32 | 28 is a also good one.
#define FONT_GLYPH_SIZE         64
#define FONT_SIZE               60

#define FONT_TEXTURE_SIZE       (FONT_GLYPH_SIZE * 16)


#define PXL_MAP(x, y)       ((x) + FONT_GLYPH_SIZE * (y))
#define IS_INSIDE(x, y, m)  (m[PXL_MAP(x, y)] >= 200)
#define CHK_POS(x, y)       ((x) >= FONT_GLYPH_SIZE || (y) >= FONT_GLYPH_SIZE ? 0 : IS_INSIDE(x, y, glyph))
// check if the pixel is an edge
bool is_edge(uint8_t *glyph, size_t x, size_t y)
{
  return IS_INSIDE(x, y, glyph) &&
         (
           /*!CHK_POS(x - 1, y - 1) ||*/ !CHK_POS(x + 0, y - 1) /*|| !CHK_POS(x + 1, y - 1)*/ ||
           !CHK_POS(x - 1, y + 0) || /*      center      */    !CHK_POS(x + 1, y + 0) ||
           /*!CHK_POS(x - 1, y + 1) ||*/ !CHK_POS(x + 0, y + 1) /*|| !CHK_POS(x + 1, y + 1)*/
         );
}
#undef CHK_POS

// dangerous pow2 (x is computed two time, but the compiler is smart enough to just compute then square only one x)
#define WPOW2(x)            ((x) * (x))
#define PXL_SQDST(px, py, x, y)   (WPOW2(x - px) + WPOW2(y - py))
void bruteforce_sdf(uint8_t *glyph, const glm::vec2 &dt,  uint8_t *output)
{
  uint8_t *temp = new uint8_t[FONT_GLYPH_SIZE * FONT_GLYPH_SIZE];
  float *dst = new float[FONT_GLYPH_SIZE * FONT_GLYPH_SIZE];
  memset(temp, 0, FONT_GLYPH_SIZE * FONT_GLYPH_SIZE);
  memset(dst, 0, FONT_GLYPH_SIZE * FONT_GLYPH_SIZE * sizeof(float));

  glm::vec2 hdt = dt / 2.f;

  // transpose glyph to a FONT_GLYPH_SIZE * FONT_GLYPH_SIZE temporary array (centering it in the process)
  for (size_t x = 0; x < FONT_GLYPH_SIZE - dt.x; ++x)
  {
    for (size_t y = 0; y < FONT_GLYPH_SIZE - dt.y; ++y)
      temp[(x + (int)hdt.x + FONT_GLYPH_SIZE * (y + (int)hdt.y))] = glyph[(int)(x + (FONT_GLYPH_SIZE - dt.x) * y)];
  }

  // brute-force the SDF
  // (yep, for each pixel at (x, y) we check all (bx, by) pixels.
  for (size_t x = 0; x < FONT_GLYPH_SIZE; ++x)
  {
    for (size_t y = 0; y < FONT_GLYPH_SIZE; ++y)
    {
      // already an edge
      if (is_edge(temp, x, y))
        dst[PXL_MAP(x, y)] = 0;
      else
      {
        for (size_t bx = 0; bx < FONT_GLYPH_SIZE; ++bx)
        {
          for (size_t by = 0; by < FONT_GLYPH_SIZE; ++by)
          {
            // we found an edge
            if (is_edge(temp, bx, by))
            {
              float sqdst = PXL_SQDST(bx, by, x, y);
              if (sqdst < dst[PXL_MAP(x, y)] || dst[PXL_MAP(x, y)] == 0)
                dst[PXL_MAP(x, y)] = sqdst;
            }
            // else, discard the pixel
          }
        }
      }

    }
  }

  // we now have the squared distance field, we 'only' have to normalize it (done in two passes)
  float max_dst = 0;
//   float max_out_dst = 0;
  for (size_t x = 0; x < FONT_GLYPH_SIZE; ++x)
  {
    for (size_t y = 0; y < FONT_GLYPH_SIZE; ++y)
    {
      if (dst[PXL_MAP(x, y)] > max_dst)
        max_dst = dst[PXL_MAP(x, y)];
    }
  }

  // hardcoded value. looks good :p
  if (max_dst > 325)
    max_dst = 325;

  max_dst = sqrtf(max_dst) / 2.f;

  // and the output pass:
  for (size_t x = 0; x < FONT_GLYPH_SIZE; ++x)
  {
    for (size_t y = 0; y < FONT_GLYPH_SIZE; ++y)
    {
      if (IS_INSIDE(x, y, temp))   // inside
        output[PXL_MAP(x, y)] = glm::clamp<float>(127.f + sqrtf(dst[PXL_MAP(x, y)]) * 127.f / max_dst, 127.f, 255.f);
      else      // outside
        output[PXL_MAP(x, y)] = glm::clamp<float>(127.f - sqrtf(dst[PXL_MAP(x, y)]) * 127.f / max_dst, 0.f, 127.f);
    }
  }

  delete [] temp;
  delete [] dst;
}


// usage: ./font-generator font/file.ttf font-name [output-folder]
int main(int argc, char **argv)
{
  // //
  // // get the parameters
  // //

  if (argc != 3 && argc != 4)
  {
    neam::cr::out.info() << LOGGER_INFO << "usage: " << argv[0] << " path/to/font/file.ttf font-name [output-folder]" << neam::cr::newline
              << "  font-name: must not be a path. the font generator will output two files: font-name.png and font-name.bfont" << neam::cr::newline
              << "  output-folder: optional. The path to the output folder. Default is './'" << neam::cr::newline << neam::cr::newline
              << "NOTE: as the font-generator is using freetype, the font-generator supports more than only ttf fonts." << std::endl;
    neam::cr::out.info() << LOGGER_INFO << "usage: " << argv[0] << " fixed font-name [output-folder]" << neam::cr::newline
              << "  output a fixed bfont. It only generate the .bfont file" << std::endl;
    return 1;
  }

  std::string ttf_file;
  std::string font_name;
  std::string output_folder = "./";

  if (argc >= 3)
  {
    ttf_file = argv[1];
    font_name = argv[2];
  }
  if (argc == 4)
    output_folder = argv[3];

  // //
  // // init the font generator
  // //

  // init yaggler (+glfw)
  neam::yaggler::yaggler_init yi;
  neam::yaggler::glfw_window win(neam::yaggler::window_mode::windowed, {FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE}, "[bfont generator]", {{GLFW_VISIBLE, false}});
  win.hide();

  // catch now the case where the font is fixed
  if (ttf_file == "fixed")
  {
    neam::bleunw::yaggler::gui::font_face font;
    font.write_out_font_face(output_folder + "/" + font_name + ".bfont", output_folder + "/" + font_name + ".png");
    return 0;
  }

  // un-initialised font-face
  neam::bleunw::yaggler::gui::font_face font;
  {
    font.font.set_texture_data(GL_RGBA8, neam::ct::vector2(FONT_GLYPH_SIZE * 16, FONT_GLYPH_SIZE * 16), GL_RGBA, GL_BYTE, nullptr, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  }

  // init freetype
  FT_Library ft_lib;

  if (FT_Init_FreeType(&ft_lib))
  {
    neam::cr::out.error() << LOGGER_INFO << "FONT-GENERATOR: ERROR: Could not init freetype library" << std::endl;
    return 1;
  }

  // load the font
  FT_Face face;

  if (FT_New_Face(ft_lib, ttf_file.data(), 0, &face))
  {
    neam::cr::out.error() << LOGGER_INFO << "FONT-GENERATOR: Could not open the font" << std::endl;
    return 1;
  }

  FT_Set_Pixel_Sizes(face, 0, FONT_SIZE);

  // //
  // // run the font generator
  // //

  // for each chars in [0-255] brute-force the signed distance field
  neam::cr::out.info() << LOGGER_INFO <<  "FONT-GENERATOR: Computing the signed distance field..." << std::endl;

  uint8_t *output = new uint8_t[FONT_GLYPH_SIZE * FONT_GLYPH_SIZE];

  FT_GlyphSlot glyph = face->glyph;
  for (size_t i = 0; i < 256; ++i)
  {
    if (i != '\n' && i != '\t' && i != '\v')
    {
      // some errors (do not exit on those errors)
      if (FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_LINEAR_DESIGN))
        neam::cr::out.error() << LOGGER_INFO << "FONT-GENERATOR: Could not load the character " << i << " ('" << static_cast<char>(i < 32 ? ' ' : i) << ")" << std::endl;

      // some warnings...
      if (glyph->advance.y != 0)
        neam::cr::out.warning() << LOGGER_INFO << "FONT-GENERATOR: the character " << i << " ('" << static_cast<char>(i < 32 ? ' ' : i) << ") will not be displayed correctly (advance.y is not 0)" << std::endl;
      if (glyph->bitmap.width >= FONT_GLYPH_SIZE)
        neam::cr::out.warning() << LOGGER_INFO << "FONT-GENERATOR: the character " << i << " ('" << static_cast<char>(i < 32 ? ' ' : i) << ") will not be displayed correctly (width too big)" << std::endl;
      if (glyph->bitmap.rows >= FONT_GLYPH_SIZE)
        neam::cr::out.warning() << LOGGER_INFO << "FONT-GENERATOR: the character " << i << " ('" << static_cast<char>(i < 32 ? ' ' : i) << ") will not be displayed correctly (height too big)" << std::endl;

      // compute the SDF
      glm::vec2 dt = glm::vec2(FONT_GLYPH_SIZE - glyph->bitmap.width, FONT_GLYPH_SIZE - glyph->bitmap.rows);
      bruteforce_sdf(glyph->bitmap.buffer, dt, output);

      // setup the table
      font.table[i].lower_pos = glm::vec2(i % 16, i / 16) / 16.f;
      font.table[i].upper_pos = glm::vec2(i % 16 + 1, i / 16 + 1) / 16.f;
      font.table[i].dt = (dt / 2.f) / glm::vec2(FONT_GLYPH_SIZE, FONT_GLYPH_SIZE);
      font.table[i].left_top = glm::vec2(glyph->bitmap_left, glyph->bitmap_top) / glm::vec2(FONT_GLYPH_SIZE, FONT_GLYPH_SIZE);
      font.table[i].x_inc = glyph->advance.x / 64.0f / FONT_SIZE;

      font.font.set_sub_texture_data(neam::ct::vector2((font.table[i].lower_pos.x * FONT_TEXTURE_SIZE), (font.table[i].lower_pos.y * FONT_TEXTURE_SIZE)),
                                     neam::ct::vector2(FONT_GLYPH_SIZE, FONT_GLYPH_SIZE),
                                     GL_RED, GL_UNSIGNED_BYTE, output, 0);

      // a nice text to show progression
      if (i % 10 == 0)
        neam::cr::out.info() << LOGGER_INFO << "done: " << (i * 100 / 256) << "% ('" << static_cast<char>(isprint(i) ? i : '-') << "')" << std::endl;
    }
  }

  delete [] output;

  neam::cr::out.info() << LOGGER_INFO << "FONT-GENERATOR: blurring..." << std::endl;

  neam::yaggler::texture::texture<neam::yaggler::type::opengl, neam::embed::GLenum<GL_TEXTURE_2D>, neam::yaggler::texture::options::empty_texture_init<GL_RGBA8, neam::ct::vector<FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE>> > tmp_texture;
  auto blur_compositor = neam::klmb::yaggler::make_compositor
  (
    neam::klmb::yaggler::make_framebuffer_pack
    (
      neam::yaggler::texture::framebuffer<neam::yaggler::type::opengl>(0), // default framebuffer

      neam::klmb::yaggler::make_texture_entry("fnt_texture", font.font, 0),
      neam::klmb::yaggler::make_texture_entry("fnt_texture", tmp_texture, 0)
    )
  );
  blur_compositor.add_pass<neam::klmb::yaggler::shared_ct_string_shader<GL_FRAGMENT_SHADER, blur_shader_string>>(GL_COLOR_BUFFER_BIT, neam::klmb::yaggler::make_output_indexes<1>(GL_COLOR_ATTACHMENT0), neam::klmb::yaggler::input_texture_indexes<0>(),
      // UNIFORMS
      neam::klmb::yaggler::make_ctx_pair("buffer_size", glm::vec2(FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE)),
      neam::klmb::yaggler::make_ctx_pair("direction", glm::vec2(1, 1)),
      neam::klmb::yaggler::make_ctx_pair("initial", 0.51f)
  );
  blur_compositor.add_pass<neam::klmb::yaggler::shared_ct_string_shader<GL_FRAGMENT_SHADER, blur_shader_string>>(GL_COLOR_BUFFER_BIT, neam::klmb::yaggler::make_output_indexes<0>(GL_COLOR_ATTACHMENT0), neam::klmb::yaggler::input_texture_indexes<1>(),
      // UNIFORMS
      neam::klmb::yaggler::make_ctx_pair("buffer_size", glm::vec2(FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE)),
      neam::klmb::yaggler::make_ctx_pair("direction", glm::vec2(-1., 1.)),
      neam::klmb::yaggler::make_ctx_pair("initial", 0.51f)
  );

  // render
  blur_compositor.render();

  neam::cr::out.info() << LOGGER_INFO << "FONT-GENERATOR: Writing to file..." << std::endl;
  font.write_out_font_face(output_folder + "/" + font_name + ".bfont", output_folder + "/" + font_name + ".png");

  neam::cr::out.info() << LOGGER_INFO << "FONT-GENERATOR: DONE !" << std::endl;
  return 0;
}
