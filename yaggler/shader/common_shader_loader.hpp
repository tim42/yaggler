//
// file : common_shader_loader.hpp
// in : file:///home/tim/projects/yaggler/yaggler/shader/common_shader_loader.hpp
//
// created by : Timothée Feuillet on linux.site
// date: 03/02/2015 12:15:10
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

#ifndef __N_126795002240104393_326004308__COMMON_SHADER_LOADER_HPP__
# define __N_126795002240104393_326004308__COMMON_SHADER_LOADER_HPP__

#include <fstream>
#include <sstream>

#ifdef __linux // WARNING: untested yet !
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
#endif

#include "shader_loader.hpp"
#include "shader_options.hpp"

namespace neam
{
  namespace yaggler
  {
    namespace shader
    {
      /// \brief "load" from a constexpr string
      template<typename CTString>
      struct shader_loader<constexpr_string, CTString>
      {
        /// \brief return true if we need to recompile the shader
        static constexpr bool has_source_changed()
        {
          return false;
        }

        /// \brief return the source code of the shader
        static constexpr const char *get_source_string()
        {
          return CTString::get();
        }

        /// \brief return the source name
        static constexpr const char *get_source_name()
        {
          return "<constexpr_string source>";
        }
      };

      /// \brief "load" from a string that can change with the time
      /// \note DString could be anything (std::string, const char *, and everything that compare with a std::string)
      /// \note we recommend putting here a reference
      template<typename DString>
      struct shader_loader<dyn_string, DString>
      {
        /// \brief return true if we need to recompile the shader
        static inline bool has_source_changed()
        {
          return old != DString::get();
        }

        /// \brief return the source code of the shader
        static inline std::string get_source_string()
        {
          old = DString::get();
          return old;
        }

        /// \brief return the source name
        static inline const char *get_source_name()
        {
          return "<string source>";
        }

        static std::string old;
      };

      /// \brief load from a file
      template<typename FileName>
      struct shader_loader<file, FileName>
      {
        /// \brief return true if we need to recompile the shader
        static inline bool has_source_changed()
        {
#ifndef __linux
          return false;
#else
          // only on linux:
          //
          // http://stackoverflow.com/q/9376975
          static double old_time = 0.f;
          double old_old_time = old_time;
          struct stat file_stat;

          int err = stat(FileName::get(), &file_stat);
          if (err != 0)
            return false;

          old_time = static_cast<double>(file_stat.st_mtim.tv_sec) + (static_cast<double>(file_stat.st_mtim.tv_nsec) * 1e-9f);

          if (!old_old_time)
            return true;

          return old_old_time != old_time;
#endif
        }

        /// \brief return the source code of the shader
        static inline std::string get_source_string()
        {
          // load the content of a file into a std::sring
          return static_cast<const std::ostringstream &>(std::ostringstream() << (std::ifstream(FileName::get()).rdbuf())).str();
        }

        /// \brief return the source name
        static inline const char *get_source_name()
        {
          return FileName::get();
        }
      };

    } // namespace shader
  } // namespace yaggler
} // namespace neam

#endif /*__N_126795002240104393_326004308__COMMON_SHADER_LOADER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

