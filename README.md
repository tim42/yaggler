![YägGLer logo](data/yaggler-small.png)
=======

**YägGLer** is a kind of 3D renderer, mainly focused on being non user-friendly.

_(although some side effects make him a fast and lightweight renderer)_

-------------

_(should build on g++ 4.8+ and clang 3.2/3.3)_

-------------

about
-----

YägGLer is composed of three levels of code:
- **YägGLer** : what's directly over openGL functions and provide some tools to deal with openGL. Their is almost no additional code, and the generated code aim to be the same as the one that you would have if ou do the same job in pure openGL.
- **K:LMB** : _"the usable YägGLer"_. add some overhead (as little as possible, but it still exists) to YägGLer but provide more functionality and an easier way to deal with the renderer. This mostly include camera, _transformation tree_, objects (_mesh_), models (_instances_) and materials (_shaders framework_)
- **:bleunw** : On top of _K:LMB_ there's a set of tools that deal with rendering, events, GUI, ... Here, there's a little more overhead (inheritance, interfaces, ...) , but it provide a flexible/easy way to deal with some concepts (application, events, GUI, _"scene manager"_ (probably not what you want)).

The main YägGLer conception pattern is the concept of _ownership_ (and the _use()_ / _bind()_ methods)
It's a bit like reference counting, but without the counter. There's an 'owner' of the underlying openGL object that is responsible to destroy the object,
and there is 0 or more 'links': objects that share the openGL ID but will not destroy it when their life come to an end.

The worst thing is you can do almost everything you want.
There's no encapsulation, no _explicit_ inheritance (at least in YägGLer and K:LMB), thousand of characters long type names, only headers, dozens of namespaces, hideous class names that doesn't reflect the concept behind them, a really bad English, ...

You've ever dreamed of all this in one single project ? *I've brought it to life*.

-------------

Credits:
-  code: **neam :)** (aka. _tim42_, aka. Timothée Feuillet)
-  GLFW, GLEW, png++ that are located in the 'third_party' folder belongs to their respective authors.

-------------

*Except for the contents of the 'third_party' folder, that are licensed by their respective owners in their respective licenses:*
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
