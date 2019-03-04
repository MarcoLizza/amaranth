# Tofu Engine

Welcome!

Guess what? Yup, that's another game engine/framework.

## Pros

* C99 code only.
* Self-contained, no external modules/libraries required.
* Multi-platform (hopefully).

## Cons

* **Wren** is not as popular as others (e.g. **Lua** or **JavaScript**), which translates into less *ready-to-use* code. However, embedding is much more seamless than with **Lua**.

## Uses

* [raylib](https://www.raylib.com/) v2.3-dev
* [Wren](https://wren.io/) v0.1.0
* [jsmn](https://zserge.com/jsmn.html/) v1.0.0

## TODOs

* [*] Library of 8/16/32 color palettes.
* [ ] Add tiled-map support.
* [ ] Out-of-the-box timer and easing support.
* [ ] Game state e display transitions, at which level? Engine or script?
* [ ] Library of "retro-feel" shaders.
* [ ] Add engine splash screen (during which resources are loaded).
* [ ] Hot-reload of selected resources (fonts, banks, maps, shaders, sounds).
* [ ] Add game controller support.
* [ ] Support for TARed/ZIPed games ([rxi/microtar](https://github.com/rxi/microtar), [kuba--/zip](https://github.com/kuba--/zip)).
* [ ] Implement a *smarter* string library (not too distant from C strings).
* [ ] Use a custom memory-management allocator.
* [ ] Switch to [Vulkan API](https://www.khronos.org/vulkan/) (through [GLFW](https://www.glfw.org/)).
* [ ] Change the API to be event-based (with explicit registration).

## Inspirations

* [LOVE2D](https://love2d.org/)
* [DOME Engine](https://github.com/avivbeeri/dome/blob/master/src/vm.c)

## License

Copyright (c) 2019 Marco Lizza (marco.lizza@gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
