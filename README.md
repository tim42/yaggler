![YägGLer logo](data/yaggler-small.png)
#  YägGLer


**YägGLer** is a kind of 3D renderer, mainly focused on being non user-friendly.

_(although some side effects make him a fast and lightweight renderer)_

This project is an **experiement** about making an really fast renderer which
resulting assembly code would be as simple as possible.
This project is unfinished yet, and is not meant for everyday use.

There isn't yet a documentation of the project but here are some points of
interest:
* the `samples` folder
* on a technical point, `yaggler/klmb` (materials and compositors) are
  interesting

## how to build

```sh
$ git submodule init
$ git submodule update
$ mkdir build && cd build && cmake ..
$ make
```

-------------

**NOTE:** _(should build on g++ 4.8+ and clang 3.2/3.3)_


## credits
* YägGLer: Timothée Feuillet ( *neam :)* or tim42 )
* **[GLFW](http://www.glfw.org/)**, **[GLEW](http://glew.sourceforge.net/)**,
   **[lodePNG](http://lodev.org/lodepng/)**, that are located in the
   `third_party` folder, belongs to their respective authors.

-------------

*Except for the contents of the `third_party` folder, that are licensed by their
respective owners in their respective licenses:*

Copyright (C) 2013-2014  Timothée Feuillet

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
