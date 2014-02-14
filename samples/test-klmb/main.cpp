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

constexpr neam::string_t vert = "data/shaders/klmb_test.vert";
constexpr neam::string_t frag = "data/shaders/klmb_test.frag";

constexpr neam::string_t yaggler_white_logo = "data/textures/yaggler-w.png";

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
GLfloat g_vertex_buffer_data[] = {
  -1.0f,-1.0f,-1.0f, // triangle 1 : begin
  -1.0f,-1.0f, 1.0f,
  -1.0f, 1.0f, 1.0f, // triangle 1 : end
  1.0f, 1.0f,-1.0f, // triangle 2 : begin
  -1.0f,-1.0f,-1.0f,
  -1.0f, 1.0f,-1.0f, // triangle 2 : end
  1.0f,-1.0f, 1.0f,
  -1.0f,-1.0f,-1.0f,
  1.0f,-1.0f,-1.0f,
  1.0f, 1.0f,-1.0f,
  1.0f,-1.0f,-1.0f,
  -1.0f,-1.0f,-1.0f,
  -1.0f,-1.0f,-1.0f,
  -1.0f, 1.0f, 1.0f,
  -1.0f, 1.0f,-1.0f,
  1.0f,-1.0f, 1.0f,
  -1.0f,-1.0f, 1.0f,
  -1.0f,-1.0f,-1.0f,
  -1.0f, 1.0f, 1.0f,
  -1.0f,-1.0f, 1.0f,
  1.0f,-1.0f, 1.0f,
  1.0f, 1.0f, 1.0f,
  1.0f,-1.0f,-1.0f,
  1.0f, 1.0f,-1.0f,
  1.0f,-1.0f,-1.0f,
  1.0f, 1.0f, 1.0f,
  1.0f,-1.0f, 1.0f,
  1.0f, 1.0f, 1.0f,
  1.0f, 1.0f,-1.0f,
  -1.0f, 1.0f,-1.0f,
  1.0f, 1.0f, 1.0f,
  -1.0f, 1.0f,-1.0f,
  -1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f,
  -1.0f, 1.0f, 1.0f,
  1.0f,-1.0f, 1.0f
};

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  // init yaggler (+glfw)
  neam::yaggler::yaggler_init yi;

  // will holds the resolution, in fixed point
  neam::ct::vector2 fixed_resolution;

  // create a window
//   neam::yaggler::glfw_window win(neam::yaggler::window_mode::fullscreen);
  neam::yaggler::glfw_window win(neam::yaggler::window_mode::windowed, {1000, 1000}, "[ :) / K: / Y: ]");
  win.set_position({0, 0});


  // camera stuff
  neam::klmb::yaggler::camera cam[2];
  neam::klmb::yaggler::camera_holder cam_holder;
  cam_holder.use_camera(cam[0]);

  // init cam
  cam[0].position = glm::vec3(0., 0., 5.);
  cam[0].look_at = glm::vec3(0., 0., 0.);

  cam[0].aspect = 1.;
  cam[0].recompute_matrices();

  cam[1].position = glm::vec3(0., 0., -15.);
  cam[1].look_at = glm::vec3(0., 0., 0.);

  cam[1].aspect = 1.;
  cam[1].recompute_matrices();


  // object stuff
  glm::mat4 object_matrix(1.0f);

  // the material
  // (much easier than using only the vanilla YägGLer, isn't it ?? ;) )
  auto material = neam::klmb::yaggler::create_base_material
  <
    // SHADERS
    neam::klmb::yaggler::shader_list
    <
      neam::klmb::yaggler::auto_file_shader<frag>,
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
    neam::klmb::yaggler::make_ctx_pair("texture", neam::klmb::yaggler::reference_to_texture<0>()),
    neam::klmb::yaggler::make_ctx_pair("myuniform", neam::klmb::yaggler::variable<int>(2)),

    neam::klmb::yaggler::make_ctx_pair("vp_matrix", neam::cr::make_ref(cam_holder.vp_matrix)), // allow camera switchs
    neam::klmb::yaggler::make_ctx_pair("object_matrix", neam::cr::make_ref(object_matrix))
  );

  // some ops on vars
  material.get_variable<0>() = 4;


  // the FS quad vao
  neam::yaggler::geometry::vao < neam::yaggler::type::opengl, neam::yaggler::geometry::options::ct_vao_init
  <
       neam::yaggler::geometry::buffer < neam::yaggler::type::opengl, neam::embed::GLenum<GL_ARRAY_BUFFER>,
       neam::yaggler::geometry::options::ct_buffer_init<neam::embed::GLfloat_array(g_vertex_buffer_data), GL_STATIC_DRAW >> ,
       neam::yaggler::geometry::buffer_view < neam::yaggler::type::opengl, neam::embed::geometry::destination_precision<neam::yaggler::geometry::destination_precision::single_precision>,
       neam::yaggler::geometry::options::ct_buffer_view_init<0, 3, GL_FLOAT, 0, 0, false >>
  >> fs_vao;


  neam::cr::chrono chronos;
  int frame_counter = 0;

  std::cout << "YAGGLER: [starting the render loop]" << std::endl;

  // yay: gl calls :D
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // 4 3V32 (at least, as the windows is open)
  float time_accumulator = 0;
  int camid = 0;
  while (!win.should_close())
  {
    float delta = chronos.delta();
    time_accumulator += delta;
    ++frame_counter;

    if (time_accumulator >= 2)
    {
      std::cout << "f/s: "       << std::setw(9) << std::left <<  frame_counter / time_accumulator
                << "  |  ms/f: " << std::setw(9) << std::left << time_accumulator / frame_counter * 1000.f
                << std::endl;

      frame_counter = 0;
      time_accumulator = 0;

      // switch cam:
      camid = (camid + 1) % 2;
      cam_holder.use_camera(cam[camid]);
    }

    glfwPollEvents();

    // get windows size, convert it to fixed position (to get it work with shaders)
    // this will be automatically bound to the shader (via the neam::cr::make_const_ref(fixed_resolution) in the autobinder).
    fixed_resolution = win.get_framebuffer_size().convert_to_fixed();

    // should be done 'on demand'
    cam[camid].aspect = static_cast<float>(win.get_framebuffer_size().x) / static_cast<float>(win.get_framebuffer_size().y);
    cam[camid].recompute_proj_and_vp_matrices();

    object_matrix = glm::rotate(object_matrix, (float)(M_PI / 8. * delta), glm::vec3(0, 1, 0));
    object_matrix = glm::rotate(object_matrix, (float)(M_PI / 10. * delta), glm::vec3(1, 1, 0));

    /* Set background colour to NOT BLACK */
    glClearColor(0.30, 0.30, 0.30, 0.1); // not fast on intel HD
//     glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, neam::ct::conversion::to<GLint>(fixed_resolution.x), neam::ct::conversion::to<GLint>(fixed_resolution.y));
    /* Clear background with the NOT BLACK colour */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    // magical !!! :)
    material.use();

    // the geom
    fs_vao.use();

    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 2*3 indices starting at 0 -> 2 triangles

    win.swap_buffers();
  }

  return 0;
}
