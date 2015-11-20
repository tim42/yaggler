//
// file : transform_feedback_base.hpp
// in : file:///home/tim/projects/yaggler/yaggler/geometry/transform_feedback_base.hpp
//
// created by : Timothée Feuillet on linux-vnd3.site
// date: 20/11/2015 16:26:03
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

#ifndef __N_12091126711662440125_9126016__TRANSFORM_FEEDBACK_BASE_HPP__
# define __N_12091126711662440125_9126016__TRANSFORM_FEEDBACK_BASE_HPP__

namespace neam
{
  namespace yaggler
  {
    namespace geometry
    {
      /// \brief this class represent a transform feedback.
      template<typename BufferViewType, typename... Args>
      class transform_feedback
      {
        static_assert(!(sizeof...(Args) + 1), "This transform_feedback type doesn't exist or is not valid.");
      };
    } // namespace geometry
  } // namespace yaggler
} // namespace neam

#endif /*__N_12091126711662440125_9126016__TRANSFORM_FEEDBACK_BASE_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

