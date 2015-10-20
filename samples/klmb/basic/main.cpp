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

using opengl_version = neam::yaggler::setup::opengl<3, 3, neam::yaggler::setup::opengl_profile::core, neam::yaggler::setup::opengl_context_flag::debug>;

#include <yaggler.hpp>

// lets include klmb main header.
#include <klmb/klmb.hpp>

#include <iostream>
#include <iomanip>

constexpr neam::string_t vert = "samples_data/shaders/klmb_test.vert";
constexpr neam::string_t frag = "samples_data/shaders/klmb_test.frag";

constexpr neam::string_t yaggler_white_logo = "samples_data/textures/yaggler-w.png";

// The fullscreen quad's FBO
GLfloat fs_quad_data [] =
{
  -1.0f, -1.f, 0.0f,
  1.0f, -1.0f, 0.0f,
  -1.0f,  1.0f, 0.0f,
  -1.0f,  1.0f, 0.0f,
  1.0f, -1.0f, 0.0f,
  1.0f,  1.0f, 0.0f,
};

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  neam::cr::out.log_level = neam::cr::stream_logger::verbosity_level::debug;

  // init yaggler (+glfw)
  neam::yaggler::yaggler_init yi;

  // will holds the resolution, in fixed point
  neam::ct::vector2 fixed_resolution;

  // create a window
//   neam::yaggler::glfw_window win(neam::yaggler::window_mode::fullscreen);
  neam::yaggler::glfw_window win(neam::yaggler::window_mode::windowed, {800, 800}, "[ :) / K: / Y: ]");
  win.set_position({0, 0});



  // the material
  // (much easier than using only the vanilla YägGLer, isn't it ?? ;) )
  auto material = neam::klmb::yaggler::create_base_material
  <
    // SHADERS
    neam::klmb::yaggler::shader_list
    <
      neam::klmb::yaggler::auto_file_shader<frag, neam::yaggler::shader::reload_on_change>,
      neam::klmb::yaggler::auto_file_shader<vert>
    >,
    // TEXTURES
    neam::klmb::yaggler::rgba_png_2d_texture_list
    <
      yaggler_white_logo
    >
  >
  // CONTEXT
  (
    neam::klmb::yaggler::make_ctx_pair("screen_resolution", neam::cr::make_const_ref(fixed_resolution)),
    neam::klmb::yaggler::make_ctx_pair("global_time", &neam::cr::chrono::now_relative),
    neam::klmb::yaggler::make_ctx_pair("texture", neam::klmb::yaggler::reference_to_texture<0>())
  );

  // the FS quad vao
  neam::yaggler::geometry::vao < neam::yaggler::type::opengl, neam::yaggler::geometry::options::ct_vao_init
  <
       neam::yaggler::geometry::buffer
       <
         neam::yaggler::type::opengl, neam::embed::GLenum<GL_ARRAY_BUFFER>,
         neam::yaggler::geometry::options::ct_buffer_init<neam::embed::GLfloat_array(fs_quad_data), GL_STATIC_DRAW >
       >,
       neam::yaggler::geometry::buffer_view
       <
         neam::yaggler::type::opengl, neam::embed::geometry::destination_precision<neam::yaggler::geometry::destination_precision::single_precision>,
         neam::yaggler::geometry::options::ct_buffer_view_init<0, 3, GL_FLOAT, 0, 0, false >
       >
  >> fs_vao;


  neam::cr::chrono chronos;
  int frame_counter = 0;

  std::cout << "YAGGLER: [starting the render loop]" << std::endl;

  // 4 3V32 (at least, as the windows is open)
  while (!win.should_close())
  {
    ++frame_counter;
    if (chronos.get_accumulated_time() >= 2)
    {
      std::cout << "f/s: "       << std::setw(9) << std::left <<  frame_counter / chronos.get_accumulated_time()
                << "  |  ms/f: " << std::setw(9) << std::left << chronos.get_accumulated_time() / frame_counter * 1000.f
                << std::endl;

      frame_counter = 0;
      chronos.reset();
    }

    glfwPollEvents();

    // get windows size, convert it to fixed position (to get it work with shaders)
    // this will be automatically bound to the shader (via the neam::cr::make_const_ref(fixed_resolution) in the autobinder).
    fixed_resolution = win.get_framebuffer_size().convert_to_fixed();


    /* Set background colour to NOT BLACK */
//     glClearColor(0.30, 0.30, 0.30, 0.1);
//     glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, neam::ct::conversion::to<GLint>(fixed_resolution.x), neam::ct::conversion::to<GLint>(fixed_resolution.y));
    /* Clear background with the NOT BLACK colour */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    // magical !!! :)
    material.use();

    // the geom
    fs_vao.use();

    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles

    win.swap_buffers();
  }

  return 0;
}
