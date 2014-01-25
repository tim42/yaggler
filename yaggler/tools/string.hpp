//
// file : string.hpp
// in : file:///home/tim/projects/yaggler/yaggler/tools/string.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 08/01/2014 20:36:58
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


#ifndef __N_18449644451804198853_1316734238__STRING_HPP__
# define __N_18449644451804198853_1316734238__STRING_HPP__

#include <cstring>
#include <string>
#include <stdlib.h>

#include "tools/ct_string.hpp"
#include "ownership.hpp"

namespace neam
{
  namespace cr
  {
    class string
    {
      public:
        // default / C str
        constexpr string()
          : cdata(nullptr), to_delete(false), str_size(0)
        {
        }

        string(const std::string &str)
          : cdata(strdup(str.data())), to_delete(true), str_size(str.size())
        {
        }

        // for hardcoded c string
        constexpr string(const char * const ptr)
          : cdata(const_cast<char *>(ptr)), to_delete(false), str_size(neam::ct::strlen(ptr))
        {
        }

        string(char *ptr, assume_ownership_t)
          : cdata((ptr)), to_delete(true), str_size(strlen(ptr))
        {
        }
        explicit string(char *ptr)
          : cdata(strdup(ptr)), to_delete(false), str_size(strlen(ptr))
        {
        }

        // copy
        string(const string &o)
          : cdata(!o.to_delete ? o.cdata : strdup(o.cdata)), to_delete(o.to_delete), str_size(o.str_size)
        {
        }
        string &operator = (const string &o)
        {
          if (this != &o)
          {
            if (cdata && to_delete)
              free(cdata);
            cdata = (!o.to_delete ? o.cdata : strdup(o.cdata));
            to_delete = (o.to_delete);
            str_size = (o.str_size);
          }
          return *this;
        }

        // move
        string(string &&o)
          : cdata(o.cdata), to_delete(o.to_delete), str_size(o.str_size)
        {
          o.cdata = nullptr;
          o.to_delete = false;
          o.str_size = 0;
        }
        string &operator = (string &&o)
        {
          if (this != &o)
          {
            if (cdata && to_delete)
              free(cdata);
            cdata = (o.cdata);
            to_delete = (o.to_delete);
            str_size = (o.str_size);
            o.cdata = nullptr;
            o.str_size = 0;
          }
          return *this;
        }

        ~string()
        {
          if (to_delete && cdata)
            free(cdata);
          cdata = nullptr;
          str_size = 0;
        }

        // getters
        size_t size() const
        {
          return str_size;
        }
        size_t empty() const
        {
          return !str_size;
        }

        char *data()
        {
          return cdata;
        }
        const char *data() const
        {
          return cdata;
        }

        char & operator [] (size_t idx)
        {
          return cdata[idx];
        }

        char operator [] (size_t idx) const
        {
          return cdata[idx];
        }

        // its
        char *begin()
        {
          return cdata;
        }
        const char *begin() const
        {
          return cdata;
        }
        char *end()
        {
          return cdata + str_size;
        }
        const char *end() const
        {
          return cdata + str_size;
        }

        char *rbegin()
        {
          return cdata + str_size - 1;
        }
        const char *rbegin() const
        {
          return cdata + str_size - 1;
        }
        char *rend()
        {
          return cdata - 1;
        }
        const char *rend() const
        {
          return cdata - 1;
        }

        // +
        string operator + (const char *s) const
        {
          // plain old C, unsafe and all
          return string(strcat(strcpy((char *)malloc(size() + strlen(s) + 1), cdata), s), assume_ownership);
        }
        string operator + (const string &s) const
        {
          // plain old C, unsafe and all
          return string(strcat(strcpy((char *)malloc(size() + s.size()), cdata), s.cdata), assume_ownership);
        }

        bool operator == (const string &s) const
        {
          if (str_size != s.str_size)
            return false;
          return !memcmp(cdata, s.cdata, str_size);
        }
        bool operator != (const string &s) const
        {
          if (str_size != s.str_size)
            return true;
          return memcmp(cdata, s.cdata, str_size);
        }

      private:
        char *cdata;
        bool to_delete;
        size_t str_size;
    };
  } // namespace cr
} // namespace neam

#endif /*__N_18449644451804198853_1316734238__STRING_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

