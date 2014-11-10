//
// file : text.hpp
// in : file:///home/tim/projects/yaggler/yaggler/bleunw/gui/text.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 11/03/2014 16:12:10
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

#ifndef __N_5056306421379269814_1366371712__TEXT_HPP__
# define __N_5056306421379269814_1366371712__TEXT_HPP__

#include <bleunw/gui/font_face.hpp>
#include <bleunw/gui/renderable.hpp>

#include <klmb/object/transformation_tree.hpp>
#include <klmb/object/object.hpp>

#include <klmb/material/generic_material.hpp>
#include <klmb/material/material.hpp>

#include <raii_use.hpp>

namespace neam
{
  namespace bleunw
  {
    namespace yaggler
    {
      namespace gui
      {
        // a text, rendered with a geometry shader.
        namespace defaults
        {
          constexpr neam::string_t text_fragment_shader_file = "data/shaders/bleunw/text/text.frag";
          using text_fragment_shader = klmb::yaggler::auto_file_shader<text_fragment_shader_file>;
        } // namespace default

        namespace internal
        {
          constexpr neam::string_t text_vertex_file = "data/shaders/bleunw/text/text.vert";
          constexpr neam::string_t text_geom_file = "data/shaders/bleunw/text/text.geom";

          using text_vertex_shader = klmb::yaggler::auto_shared_file_shader<text_vertex_file>;
          using text_geom_shader = klmb::yaggler::auto_shared_file_shader<text_geom_file>;
        } // namespace internal

        template<typename FragmentShader>
        class base_text : public renderable
        {
          public:
            explicit base_text(const std::string &_value = "", font_face *_font = nullptr)
              : font(nullptr), changed(true), value(_value), drawer(neam::yaggler::geometry::draw_method::indexed)
            {
              bounding_box.min = 0._vec3_xyz;
              bounding_box.max = 0.1_vec3_z;

              init_shader();

              drawer.set_index_type(GL_UNSIGNED_BYTE);

              if (_font)
                set_font(_font);

              x_pos.set_binding_point(1);
              vao.add_buffer(indices);
            }

            // this is dangerous: in YägGLer, this is used to create a "link".
            // There is no such things here: You copy a text, you also copy/recreate everything in it.
            // NOTE: custom uniforms won't get copied.
            base_text(const base_text &o)
              : bounding_box(o.bounding_box), font(nullptr), changed(true), value(o.value)
            {
              init_shader();

              drawer.set_index_type(GL_UNSIGNED_BYTE);

              color = o.color;
              if (o.font)
                set_font(o.font);

              x_pos.set_binding_point(1);
              vao.add_buffer(indices);
            }

            base_text &operator = (const base_text &o)
            {
              bounding_box = o.bounding_box;
              color = o.color;
              set_text(o.value);
              set_font(o.font);
            }

            base_text &operator = (const std::string &s)
            {
              set_text(s);
            }

            ~base_text()
            {
            }

            void set_font(font_face *_font)
            {
              if (font != _font)
              {
                font = _font;
                changed = true;
                if (font)
                {
                  font_texture = font->font;
                  font->init_vao(vao);
                }
              }
            }

            // -1 to: no width limit
            void set_max_width(float _max_width = -1)
            {
              max_width = _max_width;
              changed = true;
            }

            void set_text(const std::string &_value)
            {
              if (value != _value)
              {
                value = _value;
                changed = true;
              }
            }

            void set_dirty()
            {
              changed = true;
            }

            bool has_changed() const
            {
              return changed;
            }

            bool is_dirty() const
            {
              return changed;
            }

            FragmentShader &get_fragment_shader()
            {
              return *fragment_shader;
            }
            const FragmentShader &get_fragment_shader() const
            {
              return *fragment_shader;
            }

            virtual void render()
            {
              if (font)
              {
                if (vp_matrix)
                  material.get_vp_matrix() = *vp_matrix;
                material.get_object_matrix() = world_pos;

                if (changed)
                  _rebuild_text();

                // uses and draw
                YAGG_SCOPED_USE(vao);
                YAGG_SCOPED_USE(x_pos);

                font->font.use();

                material.use();
                drawer.draw();
              }
            }

            // this could be used to prevent/delay changes
            virtual void render() const
            {
              if (font)
              {
                // uses and draw
                YAGG_SCOPED_USE(vao);
                YAGG_SCOPED_USE(x_pos);

                font->font.use();

                material.use();
                drawer.draw();
              }
            }

            void _rebuild_text()
            {
              GLfloat *ar_xypos = new GLfloat[value.size() * 2 + (value.size() * 2) % 4];

              char_count = value.size();

              // compute indexes and x pos
              GLfloat acc = 0.;
              GLfloat yacc = 0.;
              size_t idx = 0;
              for (size_t i = 0; i < value.size(); ++i, ++idx)
              {
                ar_xypos[i * 2 + 0] = acc;
                ar_xypos[i * 2 + 1] = yacc;
                if (value[i] == '\n')
                {
                  acc = 0;
                  idx = 0;
                  ++yacc;
                }
                else if (value[i] == '\v')
                  ++yacc;
                else if (value[i] == '\t')
                  acc = 4 - (idx % 4) + round(acc);
                else
                  acc += font->table[static_cast<unsigned int>(value[i])].x_inc;

                if (max_width > 0 && acc > max_width)
                {
                  acc = 0;
                  ++yacc;
                }
              }

              bounding_box.max = glm::vec3(acc, yacc, 0.1);

              // setup buffers (send directly the string to openGL ;) )
              indices.set_data(array_wrapper<const GLubyte>(reinterpret_cast<const unsigned char *>(value.data()), value.size()), GL_DYNAMIC_DRAW);
              x_pos.set_data(array_wrapper<GLfloat>(ar_xypos, value.size() * 2 + (value.size() * 2) % 4), GL_DYNAMIC_DRAW);

              // setup drawer
              drawer.set_draw_points(value.size());

              changed = false;

              delete [] ar_xypos;
            }

          public:
            glm::vec4 color = glm::vec4(1., 1., 1., 1.);

            klmb::yaggler::aabb bounding_box;

          private:
            void init_shader()
            {
              auto *real_material = klmb::yaggler::create_material_ptr
              <
                // SHADERS
                neam::klmb::yaggler::shader_list
                <
                  FragmentShader,
                  internal::text_geom_shader,
                  internal::text_vertex_shader
                >,
                // TEXTURES
                neam::klmb::yaggler::no_textures
              >
              (
                // CONTEXT
//                 neam::klmb::yaggler::make_ctx_pair("global_time", &neam::cr::chrono::now_relative),
                neam::klmb::yaggler::make_ctx_pair("font_texture", cr::make_const_ref(font_texture)),
                neam::klmb::yaggler::make_ctx_pair("font_color", cr::make_const_ref(color)),
//                 neam::klmb::yaggler::make_ctx_pair("char_count", cr::make_const_ref(char_count)),
                neam::klmb::yaggler::make_ctx_buffer_pair("displacement_block", cr::make_const_ref(x_pos))
              );
              material = real_material;
              material.assume_ownership();
              material.get_object_matrix() = world_pos;
              if (vp_matrix)
                material.get_vp_matrix() = *vp_matrix;
              fragment_shader = &(real_material->get_shader_program().template get_shader_at_index<0>());
            }

          private:
            font_face *font = nullptr;
            bool changed = true;
            std::string value;
            float max_width = -1.f;

            // // GL (klmb/YägGLer) stuff
            FragmentShader *fragment_shader;
            klmb::yaggler::material_wrapper material;

            // uniforms links
            GLint font_texture;
            GLfloat char_count;

            // buffers
            neam::yaggler::geometry::buffer<neam::yaggler::type::opengl, embed::GLenum<GL_UNIFORM_BUFFER>> x_pos;
            neam::yaggler::geometry::buffer<neam::yaggler::type::opengl, embed::GLenum<GL_ELEMENT_ARRAY_BUFFER>> indices;

            // draw state
            neam::yaggler::geometry::draw_state<neam::yaggler::type::opengl> drawer;

            // vao
            neam::yaggler::geometry::vao<neam::yaggler::type::opengl> vao;

            // view
            neam::yaggler::geometry::buffer_view < neam::yaggler::type::opengl, neam::embed::geometry::destination_precision<neam::yaggler::geometry::destination_precision::single_precision>,
                neam::yaggler::geometry::options::ct_buffer_view_init
                <
                  6,                                    // index
                  1,                                    // size
                  GL_FLOAT,                             // type
                  sizeof(GLfloat),                      // stride
                  0                                     // offset
                >
               > x_pos_view;
        };

        // helpers
        using text = base_text<defaults::text_fragment_shader>;

      } // namespace gui
    } // namespace yaggler
  } // namespace bleunw
} // namespace neam

#endif /*__N_5056306421379269814_1366371712__TEXT_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

