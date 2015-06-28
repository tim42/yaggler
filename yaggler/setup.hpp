//
// file : setup.hpp
// in : file:///home/tim/projects/yaggler/yaggler/setup.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 19/10/2013 04:07:49
//
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


#ifndef __N__SETUP_HPP__
# define __N__SETUP_HPP__

#include <tools/logger/logger.hpp>
#include <cstddef>

namespace neam
{
  namespace yaggler
  {
    /// \namespace setup
    /// \brief contains everything needed to setup the opengl version used by yaggler
    /// \example
    ///   \code
    ///     // using opengl 3.3 in core profile with the debug flag activated
    ///     using opengl_version = neam::yaggler::setup::opengl<3, 3, neam::yaggler::setup::opengl_profile::core, neam::yaggler::setup::opengl_context_flag::debug>;
    ///   \endcode
    /// \example
    ///   \code
    ///     // using opengl 4.5
    ///     using opengl_version = neam::yaggler::setup::opengl<4, 5>;
    ///   \endcode
    namespace setup
    {
      /// \brief what kind of opengl profile to use
      /// \e core profile is the default
      enum class opengl_profile
      {
        any = 0,
        core = 1 << 1,
        compat = 1 << 2,
      };

      /// \brief some flags for the creation of the opengl context
      /// \note the \e debug flag create a debug profile and will also register a default logging function
      enum class opengl_context_flag
      {
        none = 0,
        forward_compat = 1 << 1,
        debug = 1 << 2,
      };

      /// \brief the opengl object, describing what kind of gl context will be created by yaggler
      template<size_t GlMajor, size_t GlMinor, opengl_profile Profile = opengl_profile::core, opengl_context_flag CtxFlags = opengl_context_flag::none>
      struct opengl
      {
        static constexpr size_t gl_major = GlMajor;
        static constexpr size_t gl_minor = GlMinor;
        static constexpr opengl_profile profile = Profile;
        static constexpr bool forward_compat = (int)CtxFlags & (int)opengl_context_flag::forward_compat;
        static constexpr bool debug = (int)CtxFlags & (int)opengl_context_flag::debug;

        /// \brief log the context information in the neam::cr::out stream
        static inline void _log()
        {
          neam::cr::out.info() << LOGGER_INFO << "using OPENGL " << gl_major << "." << gl_minor
                               << (profile == opengl_profile::core ? " core profile" : (profile == opengl_profile::compat ? " compatibility profile" : ""))
                               << (debug || forward_compat ? " flags: [" : "" )
                               << (debug ? "debug" : "" )
                               << (debug && forward_compat ? ", " : "" )
                               << (forward_compat ? "forward compatible" : "" )
                               << (debug || forward_compat ? "]" : "" )
                               << std::endl;
        }
      };
    } // namespace setup
  } // namespace yaggler
} // namespace neam

#endif /*__N__SETUP_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

