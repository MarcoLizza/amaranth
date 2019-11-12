/*
 * Copyright (c) 2019 Marco Lizza (marco.lizza@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

// TODO: rename Display to Video?

#include <config.h>
#include <libs/gl/gl.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdbool.h>
#include <stddef.h>

#include "display/program.h"

typedef enum _Display_Programs_t {
    Display_Programs_t_First = 0,
    DISPLAY_PROGRAM_PASSTHRU = Display_Programs_t_First,
    DISPLAY_PROGRAM_CUSTOM,
    Display_Programs_t_Last = DISPLAY_PROGRAM_CUSTOM,
    Display_Programs_t_CountOf
} Display_Programs_t;

typedef struct _Display_Configuration_t {
    int width, height, scale;
    bool fullscreen;
    bool hide_cursor;
} Display_Configuration_t;

typedef struct _Display_t {
    Display_Configuration_t configuration;

    GLFWwindow *window;
    int window_width, window_height, window_scale;
    int physical_width, physical_height;

    GL_Color_t *vram; // Temporary buffer to create the OpenGL texture from `GL_Pixel_t` array.
    GLuint vram_texture;
    GL_Quad_t vram_destination;

    Program_t programs[Display_Programs_t_CountOf];
    Program_t *active_program;

    GL_Palette_t palette;
    GL_Context_t gl;
} Display_t;

extern bool Display_initialize(Display_t *display, const Display_Configuration_t *configuration, const char *title);
extern void Display_terminate(Display_t *display);
extern bool Display_should_close(Display_t *display);
extern void Display_present(Display_t *display);

extern void Display_shader(Display_t *display, const char *code);
extern void Display_palette(Display_t *display, const GL_Palette_t *palette);

#endif  /* __DISPLAY_H__ */