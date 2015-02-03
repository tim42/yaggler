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

#include <iostream>
#include <iomanip>

constexpr neam::string_t vert = "data/shaders/shader_test.vert";
constexpr neam::string_t frag = "data/shaders/shader_test.frag";

constexpr neam::string_t yaggler_logo = "data/textures/yaggler-w.png";
constexpr neam::string_t bg_image = "data/textures/bg.png";

constexpr uint32_t image_1x1_data[] =
{
  0x001111FF,
};

constexpr uint32_t image_4x4_data[] =
{
  0xFF00FF, 0xFFAAFF, 0x445566, 0x112233,
  0x0000FF, 0xAAAAFF, 0x556600, 0x334455,
  0xFF0000, 0xFFAAAA, 0x440066, 0x556677,
  0x00FF00, 0xAAFFAA, 0x005566, 0x778899,
};

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
  neam::yaggler::glfw_window win(neam::yaggler::window_mode::windowed, {800, 800}, "[ :) / Y: ]");
  win.set_position({0, 0});

  // the shader
  neam::yaggler::shader::program<neam::yaggler::type::opengl,
       neam::yaggler::shader::shader
       <
        neam::yaggler::type::opengl, neam::embed::GLenum<GL_FRAGMENT_SHADER>,
        neam::yaggler::shader::opengl::file, neam::embed::string<frag>,
        neam::embed::shader::option<neam::yaggler::shader::shader_option::reload_on_change>
       >,
       neam::yaggler::shader::shader
       <
        neam::yaggler::type::opengl, neam::embed::GLenum<GL_VERTEX_SHADER>,
        neam::yaggler::shader::opengl::file, neam::embed::string<vert>,
        neam::embed::shader::option<neam::yaggler::shader::shader_option::reload_on_change>
       >
  > prog;

  prog.bind_attribute_location("in_position", 0);

  float rm_step = 55.;
  prog.get_shader_at_index<0>().append_to_additional_strings("#define RM_STEP " + static_cast<std::ostringstream&>(std::ostringstream() << rm_step).str()); // base define

  prog. link();

  // the texture
//   neam::yaggler::texture::texture < neam::yaggler::type::opengl, neam::embed::GLenum<GL_TEXTURE_2D>,
//        neam::yaggler::texture::options::ct_texture_init<GL_RGB, neam::ct::vector<4, 4>, GL_BGRA, GL_UNSIGNED_BYTE, neam::embed::uint32_ptr<image_4x4_data> >> my_test_texture(0);
  neam::yaggler::texture::texture < neam::yaggler::type::opengl, neam::embed::GLenum<GL_TEXTURE_2D>,
       neam::yaggler::texture::options::png_texture_init<GL_RGBA, neam::embed::string<yaggler_logo> >> my_test_texture(0);

  my_test_texture.set_gl_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  my_test_texture.set_gl_parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//   neam::yaggler::texture::texture < neam::yaggler::type::opengl, neam::embed::GLenum<GL_TEXTURE_2D>,
//        neam::yaggler::texture::options::ct_texture_init<GL_RGB, neam::ct::vector<4, 4>, GL_BGRA, GL_UNSIGNED_BYTE, neam::embed::uint32_ptr<image_4x4_data> >> my_fg_texture(1);
  neam::yaggler::texture::texture < neam::yaggler::type::opengl, neam::embed::GLenum<GL_TEXTURE_2D>,
       neam::yaggler::texture::options::png_texture_init<GL_RGBA, neam::embed::string<bg_image> >> my_fg_texture(1);

  my_fg_texture.set_gl_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  my_fg_texture.set_gl_parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // some vars
  neam::yaggler::shader::uniform_variable resolution_var = prog.get_uniform_variable("screen_resolution");
  neam::yaggler::shader::uniform_variable time_var = prog.get_uniform_variable("global_time");
  neam::yaggler::shader::uniform_variable texture_var = prog.get_uniform_variable("texture_sampler");
  neam::yaggler::shader::uniform_variable fg_texture_var = prog.get_uniform_variable("fg_texture_sampler");

  // the autobinder
  auto ctx = neam::yaggler::shader::variable_context<neam::yaggler::shader::contexts::none>::create<neam::yaggler::shader::contexts::fixed>
             (
               neam::cr::make_tuple(&neam::cr::chrono::now_relative, neam::cr::make_const_ref(fixed_resolution), neam::cr::make_const_ref(my_test_texture), neam::cr::make_const_ref(my_fg_texture)),  // values
               time_var, resolution_var, texture_var, fg_texture_var                                                                                                                                   // vars
             );

  // the FS quad vao
  neam::yaggler::geometry::vao < neam::yaggler::type::opengl, neam::yaggler::geometry::options::ct_vao_init
  <
       neam::yaggler::geometry::buffer < neam::yaggler::type::opengl, neam::embed::GLenum<GL_ARRAY_BUFFER>,
       neam::yaggler::geometry::options::ct_buffer_init<neam::embed::GLfloat_array(fs_quad_data), GL_STATIC_DRAW >> ,
       neam::yaggler::geometry::buffer_view < neam::yaggler::type::opengl, neam::embed::geometry::destination_precision<neam::yaggler::geometry::destination_precision::single_precision>,
       neam::yaggler::geometry::options::ct_buffer_view_init<0, 3, GL_FLOAT, 0, 0, false >>
  >> fs_vao;


  glBindBuffer(GL_ARRAY_BUFFER, 0);


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
//     glClearColor(0.30, 0.30, 0.30, 0.1); // not fast on intel HD
//     glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, neam::ct::conversion::to<GLint>(fixed_resolution.x), neam::ct::conversion::to<GLint>(fixed_resolution.y));
    /* Clear background with the NOT BLACK colour */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // the magic, at ~no cost :) (but wait to see materials =) )
    prog.use();
    ctx.use();

    my_test_texture.use();
    my_fg_texture.use();

    // the geom
    fs_vao.use();

    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles

    win.swap_buffers();
  }

  return 0;
}
