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

#include "canvas.h"

#include "../core/luax.h"

#include "../config.h"
#include "../environment.h"
#include "../log.h"
#include "../gl/gl.h"

#include "graphics/palettes.h"
#include "graphics/sheets.h"

#include <math.h>
#include <string.h>

typedef struct _Canvas_Class_t {
} Canvas_Class_t;

static int canvas_width(lua_State *L);
static int canvas_height(lua_State *L);
static int canvas_palette(lua_State *L);
static int canvas_background(lua_State *L);
static int canvas_shader(lua_State *L);
static int canvas_color(lua_State *L);
static int canvas_points(lua_State *L);
static int canvas_polyline(lua_State *L);
static int canvas_strip(lua_State *L);
static int canvas_fan(lua_State *L);

static const struct luaL_Reg canvas_f[] = {
    { "width", canvas_width },
    { "height", canvas_height },
    { "palette", canvas_palette },
    { "background", canvas_background },
    { "shader", canvas_shader },
    { "color", canvas_color },
    { "points", canvas_points },
    { "polyline", canvas_polyline },
    { "strip", canvas_strip },
    { "fan", canvas_fan },
    { NULL, NULL }
};

static const struct luaL_Reg canvas_m[] = {
    { NULL, NULL }
};

static const luaX_Const canvas_c[] = {
    { NULL }
};

const char canvas_script[] =
    "local Canvas = require(\"tofu.graphics.Canvas\")\n"
    "\n"
    "Canvas.MODE_LINE = 0\n"
    "Canvas.MODE_FILL = 1\n"
    "\n"
    "Canvas.point = function(x0, y0, color)\n"
    "  Canvas.points({ x0, y0 }, color)\n"
    "end\n"
    "\n"
    "Canvas.line = function(x0, y0, x1, y1, color)\n"
    "  Canvas.polyline({ x0, y0, x1, y1, x0, y0 }, color)\n"
    "end\n"
    "\n"
    "Canvas.triangle = function(mode, x0, y0, x1, y1, x2, y2, color)\n"
    "  if mode == Canvas.MODE_LINE then\n"
    "    Canvas.polyline({ x0, y0, x1, y1, x2, y2, x0, y0 }, color)\n"
    "  else\n"
    "    Canvas.strip({ x0, y0, x1, y1, x2, y2 }, color)\n"
    "  end\n"
    "end\n"
    "\n"
    "Canvas.rectangle = function(mode, x, y, width, height, color)\n"
    "  local offset = mode == Canvas.MODE_LINE and 1 or 0\n"
    "  local x0 = x\n"
    "  local y0 = y\n"
    "  local x1 = x0 + width - offset\n"
    "  local y1= y0 + height - offset\n"
    "  if mode == Canvas.MODE_LINE then\n"
    "    Canvas.polyline({ x0, y0, x0, y1, x1, y1, x1, y0, x0, y0 }, color)\n"
    "  else\n"
    "    Canvas.strip({ x0, y0, x0, y1, x1, y0, x1, y1 }, color)\n"
    "  end\n"
    "end\n"
    "\n"
    "Canvas.square = function(mode, x, y, size, color)\n"
    "  Canvas.rectangle(mode, x, y, size, size, color)\n"
    "end\n"
    "\n"
    "Canvas.circle = function(mode, x, y, radius, color, segments)\n"
    "  segments = segments or 30\n"
    "  local step = (2 * math.PI) / segments\n"
    "  if mode == Canvas.MODE_LINE then\n"
    "    local vertices = {}\n"
    "    for i = 1, segments do\n"
    "      local angle = step * i\n"
    "      table.insert(vertices, x + math.sin(angle) * radius)\n"
    "      table.insert(vertices, y + math.cos(angle) * radius)\n"
    "    end\n"
    "    Canvas.polyline(vertices, color)\n"
    "  else\n"
    "    local vertices = {}\n"
    "    vertices.insert(-1, x)\n"
    "    vertices.insert(-1, y)\n"
    "    for i = 1, segments do\n"
    "      local angle = step * i\n"
    "      table.insert(vertices, x + math.sin(angle) * radius)\n"
    "      table.insert(vertices, y + math.cos(angle) * radius)\n"
    "    end\n"
    "    Canvas.fan(vertices, color)\n"
    "  end\n"
    "end\n"
;

int canvas_loader(lua_State *L)
{
    return luaX_newclass(L, canvas_f, canvas_m, canvas_c, LUAX_CLASS(Canvas_Class_t));
}

static int canvas_width(lua_State *L)
{
    if (lua_gettop(L) != 0) {
        return luaL_error(L, "<GRAPHICS> canvas function 0 arguments");
    }
#ifdef __DEBUG_API_CALLS__
    Log_write(LOG_LEVELS_DEBUG, "Canvas.width()");
#endif

    Environment_t *environment = (Environment_t *)luaX_getuserdata(L, "environment");

    lua_pushinteger(L, environment->display->configuration.width);

    return 1;
}

static int canvas_height(lua_State *L)
{
    if (lua_gettop(L) != 0) {
        return luaL_error(L, "<GRAPHICS> canvas function 0 arguments");
    }
#ifdef __DEBUG_API_CALLS__
    Log_write(LOG_LEVELS_DEBUG, "Canvas.height()");
#endif

    Environment_t *environment = (Environment_t *)luaX_getuserdata(L, "environment");

    lua_pushinteger(L, environment->display->configuration.height);

    return 1;
}

static int canvas_palette0(lua_State *L)
{
#ifdef __DEBUG_API_CALLS__
    Log_write(LOG_LEVELS_DEBUG, "Canvas.palette()");
#endif

    Environment_t *environment = (Environment_t *)luaX_getuserdata(L, "environment");

    GL_Palette_t *palette = &environment->display->palette;

    lua_newtable(L);
    for (size_t i = 0; i < palette->count; ++i) {
        char argb[12] = {};
        GL_palette_format_color(argb, palette->colors[i]);

        lua_pushstring(L, argb);
        lua_rawseti(L, -2, i + 1);
    }

    return 1;
}

static int canvas_palette1(lua_State *L)
{
    int type = lua_type(L, 1);
#ifdef __DEBUG_API_CALLS__
    Log_write(LOG_LEVELS_DEBUG, "Canvas.palette(%d)", type);
#endif

    Environment_t *environment = (Environment_t *)luaX_getuserdata(L, "environment");

    GL_Palette_t palette = {};

    if (type == LUA_TSTRING) { // Predefined palette!
        const char *id = luaL_checkstring(L, 1);
        const GL_Palette_t *predefined_palette = graphics_palettes_find(id);
        if (predefined_palette != NULL) {
            palette = *predefined_palette;

            Log_write(LOG_LEVELS_DEBUG, "<GRAPHICS> setting predefined palette '%s' w/ %d color(s)", id, predefined_palette->count);
        } else {
            Log_write(LOG_LEVELS_WARNING, "<GRAPHICS> unknown predefined palette w/ id '%s'", id);
        }
    } else
    if (type == LUA_TTABLE) { // User supplied palette.
        palette.count = lua_rawlen(L, 1);
        Log_write(LOG_LEVELS_DEBUG, "<GRAPHICS> setting custom palette of #%d color(s)", palette.count);

        if (palette.count > GL_MAX_PALETTE_COLORS) {
            Log_write(LOG_LEVELS_WARNING, "<GRAPHICS> palette has too many colors (%d) - clamping", palette.count);
            palette.count = MAX_PALETTE_COLORS;
        }

        lua_pushnil(L); // first key
        int i = 0; // TODO: define a helper function
        while (lua_next(L, 1) != 0) {
#if 0
            const char *key_type = lua_typename(L, lua_type(L, -2)); // uses 'key' (at index -2) and 'value' (at index -1)
#endif
            const char *argb = lua_tostring(L, -1);
            palette.colors[i++] = GL_palette_parse_color(argb);

            lua_pop(L, 1); // removes 'value'; keeps 'key' for next iteration
        }
    } else {
        Log_write(LOG_LEVELS_ERROR, "<GRAPHICS> wrong palette type, need to be string or list");
    }

    if (palette.count == 0) {
        return 0;
    }

    Display_palette(environment->display, &palette);

    return 0;
}

static int canvas_palette(lua_State *L)
{
    if (lua_gettop(L) == 0) {
        return canvas_palette0(L);
    } else
    if (lua_gettop(L) == 1) {
        return canvas_palette1(L);
    } else {
        return luaL_error(L, "<GRAPHICS> canvas function 0 arguments");
    }
}

static int canvas_background(lua_State *L)
{
    if (lua_gettop(L) != 1) {
        return luaL_error(L, "<GRAPHICS> canvas function takes 1 arguments");
    }
    int color = luaL_checkinteger(L, 1);
#ifdef __DEBUG_API_CALLS__
    Log_write(LOG_LEVELS_DEBUG, "Canvas.background(%d)", color);
#endif

    Environment_t *environment = (Environment_t *)luaX_getuserdata(L, "environment");

    Display_background(environment->display, color);

    return 0;
}

static int canvas_shader(lua_State *L)
{
    if (lua_gettop(L) != 1) {
        return luaL_error(L, "<GRAPHICS> canvas function takes 1 arguments");
    }
    const char *code = luaL_checkstring(L, 1);
#ifdef __DEBUG_API_CALLS__
    Log_write(LOG_LEVELS_DEBUG, "Canvas.shader('%s')", code);
#endif

    Environment_t *environment = (Environment_t *)luaX_getuserdata(L, "environment");

    Display_shader(environment->display, code);

    return 0;
}

static int canvas_color(lua_State *L)
{
    if (lua_gettop(L) != 1) {
        return luaL_error(L, "<GRAPHICS> canvas function takes 1 arguments");
    }
    const char *argb = luaL_checkstring(L, 1);
#ifdef __DEBUG_API_CALLS__
    Log_write(LOG_LEVELS_DEBUG, "Canvas.color('%s')", argb);
#endif

    Environment_t *environment = (Environment_t *)luaX_getuserdata(L, "environment");

    GL_Color_t color = GL_palette_parse_color(argb);
    size_t index = GL_palette_find_nearest_color(&environment->display->palette, color);
#ifdef __DEBUG_API_CALLS__
    Log_write(LOG_LEVELS_DEBUG, "color '%s' mapped to index %d", argb, index);
#endif

    lua_pushinteger(L, index);

    return 1;
}

// When drawing points and lines we need to ensure to be in mid-pixel coordinates, according to
// the "diamond exit rule" in OpenGL's rasterization. Also, the ending pixel of a line segment
// is not drawn to avoid lighting a pixel twice in a loop.
//
// http://glprogramming.com/red/appendixg.html#name1
static int canvas_points(lua_State *L)
{
    if (lua_gettop(L) != 2) {
        return luaL_error(L, "<GRAPHICS> canvas function takes 2 arguments");
    }
    int vertices = lua_rawlen(L, 1);
    int color = luaL_checkinteger(L, 2);
#ifdef __DEBUG_API_CALLS__
    Log_write(LOG_LEVELS_DEBUG, "Canvas.points(%d, %d)", vertices, color);
#endif

    const size_t count = vertices / 2;
    if (count == 0) {
        Log_write(LOG_LEVELS_INFO, "<GRAPHICS> point-sequence as no vertices");
        return 0;
    }

    double array[vertices];
    luaX_getnumberarray(L, 1, array);

    GL_Point_t points[count];
    for (size_t i = 0; i < count; ++i) {
        double x = array[(i * 2)];
        double y = array[(i * 2) + 1];

        points[i] = (GL_Point_t){
                .x = (GLfloat)x + 0.375f, .y = (GLfloat)y + 0.375f
            };
    }

    GL_primitive_points(points, count, (GL_Color_t){ color, color, color, 255 });

    return 0;
}

static int canvas_polyline(lua_State *L)
{
    if (lua_gettop(L) != 2) {
        return luaL_error(L, "<GRAPHICS> canvas function takes 2 arguments");
    }
    int vertices = lua_rawlen(L, 1);
    int color = luaL_checkinteger(L, 2);
#ifdef __DEBUG_API_CALLS__
    Log_write(LOG_LEVELS_DEBUG, "Canvas.polyline(%d, %d)", vertices, color);
#endif

    const size_t count = vertices / 2;
    if (count == 0) {
        Log_write(LOG_LEVELS_INFO, "<GRAPHICS> polyline as no vertices");
        return 0;
    }

    double array[vertices];
    luaX_getnumberarray(L, 1, array);

    GL_Point_t points[count];
    for (size_t i = 0; i < count; ++i) {
        double x = array[(i * 2)];
        double y = array[(i * 2) + 1];

        points[i] = (GL_Point_t){
                .x = (GLfloat)x + 0.375f, .y = (GLfloat)y + 0.375f
            };
    }

    GL_primitive_polyline(points, count, (GL_Color_t){ color, color, color, 255 });

    return 0;
}

static int canvas_strip(lua_State *L)
{
    if (lua_gettop(L) != 2) {
        return luaL_error(L, "<GRAPHICS> canvas function takes 2 arguments");
    }
    int vertices = lua_rawlen(L, 1);
    int color = luaL_checkinteger(L, 2);
#ifdef __DEBUG_API_CALLS__
    Log_write(LOG_LEVELS_DEBUG, "Canvas.strip(%d, %d)", vertices, color);
#endif

    const size_t count = vertices / 2;
    if (count == 0) {
        Log_write(LOG_LEVELS_INFO, "<GRAPHICS> strip as no vertices");
        return 0;
    }

    double array[vertices];
    luaX_getnumberarray(L, 1, array);

    GL_Point_t points[count];
    for (size_t i = 0; i < count; ++i) {
        double x = array[(i * 2)];
        double y = array[(i * 2) + 1];

        points[i] = (GL_Point_t){
                .x = (GLfloat)x + 0.375f, .y = (GLfloat)y + 0.375f
            };
    }

    GL_primitive_strip(points, count, (GL_Color_t){ color, color, color, 255 });

    return 0;
}

static int canvas_fan(lua_State *L)
{
    if (lua_gettop(L) != 2) {
        return luaL_error(L, "<GRAPHICS> canvas function takes 2 arguments");
    }
    int vertices = lua_rawlen(L, 1);
    int color = luaL_checkinteger(L, 2);
#ifdef __DEBUG_API_CALLS__
    Log_write(LOG_LEVELS_DEBUG, "Canvas.fan(%d, %d)", vertices, color);
#endif

    const size_t count = vertices / 2;
    if (count == 0) {
        Log_write(LOG_LEVELS_INFO, "<GRAPHICS> fan as no vertices");
        return 0;
    }

    double array[vertices];
    luaX_getnumberarray(L, 1, array);

    GL_Point_t points[count];
    for (size_t i = 0; i < count; ++i) {
        double x = array[(i * 2)];
        double y = array[(i * 2) + 1];

        points[i] = (GL_Point_t){
                .x = (GLfloat)x + 0.375f, .y = (GLfloat)y + 0.375f
            };
    }

    GL_primitive_fan(points, count, (GL_Color_t){ color, color, color, 255 });

    return 0;
}
