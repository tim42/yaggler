//
// file : font_manager.hpp
// in : file:///home/tim/projects/yaggler/yaggler/bleunw/gui/font_manager.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 20/03/2014 19:10:39
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

#ifndef __N_802108825307370420_146740715__FONT_MANAGER_HPP__
# define __N_802108825307370420_146740715__FONT_MANAGER_HPP__

#include <initializer_list>
#include <string>
#include <map>
#include <bleunw/gui/font_face.hpp>

namespace neam
{
  namespace bleunw
  {
    namespace yaggler
    {
      namespace gui
      {
        // a class to deal with font faces
        class font_manager
        {
          public:
            font_manager()
            {
            }

            // load a batch of font (see batch_load_font)
            font_manager(std::initializer_list<std::pair<std::string, std::string>> load_list)
            {
              batch_load_font(load_list);
            }

            font_manager(const font_manager &) = delete;
            font_manager &operator = (const font_manager &) = delete;

            ~font_manager()
            {
            }

            // create a font from a bfont file that could be later accessed with 'name'.
            bool load_font(const std::string &name, const std::string &file)
            {
              if (font_faces.count(name))
                return false;

              try
              {
                font_faces.emplace(name, file);
              }
              catch (neam::base_runtime_error &e)
              {
                std::cerr << "YAGGLER: FONT_MANAGER ERROR: " << e.what() << std::endl;
                return false;
              }
              return true;
            }

            // retrieve a font
            const font_face &get_font_ref(const std::string &name) const
            {
              auto it = font_faces.find(name);
              if (it == font_faces.end())
                throw neam::runtime_error<font_manager>("could not find the font '" + name + "'");
              return it->second;
            }

            font_face *get_font_ptr(const std::string &name)
            {
              auto it = font_faces.find(name);
              if (it == font_faces.end())
                return nullptr;
              return &it->second;
            }

            // check if a font is present
            bool has_font(const std::string &name) const
            {
              return font_faces.count(name);
            }

            // remove a font
            void remove_font(const std::string &name)
            {
              font_faces.erase(name);
            }

            // batch load
            // the first string is the name, the second the file (as in load_font)
            bool batch_load_font(std::initializer_list<std::pair<std::string, std::string>> load_list)
            {
              bool ret = true;
              for (auto &it : load_list)
                ret = ret && load_font(it.first, it.second);
              return ret;
            }

            // batch remove (takes a list of name)
            void batch_remove_font(std::initializer_list<std::string> rm_list)
            {
              for (auto &it : rm_list)
                remove_font(it);
            }

          private:
            std::map<std::string, font_face> font_faces;
        };

      } // namespace gui
    } // namespace yaggler
  } // namespace bleunw
} // namespace neam

#endif /*__N_802108825307370420_146740715__FONT_MANAGER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

