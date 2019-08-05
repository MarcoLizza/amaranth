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

#include "luax.h"

/*
http://webcache.googleusercontent.com/search?q=cache:RLoR9dkMeowJ:howtomakeanrpg.com/a/classes-in-lua.html+&cd=4&hl=en&ct=clnk&gl=it
https://hisham.hm/2014/01/02/how-to-write-lua-modules-in-a-post-module-world/
https://www.oreilly.com/library/view/creating-solid-apis/9781491986301/ch01.html
file:///C:/Users/mlizza/Downloads/[Roberto_Ierusalimschy]_Programming_in_Lua(z-lib.org).pdf (page 269)
https://nachtimwald.com/2014/07/12/wrapping-a-c-library-in-lua/
https://www.lua.org/pil/28.5.html
https://stackoverflow.com/questions/16713837/hand-over-global-custom-data-to-lua-implemented-functions
https://stackoverflow.com/questions/29449296/extending-lua-check-number-of-parameters-passed-to-a-function
https://stackoverflow.com/questions/32673835/how-do-i-create-a-lua-module-inside-a-lua-module-in-c
*/

int luaX_newclass(lua_State *L, const luaL_Reg *f, const luaL_Reg *m, const luaX_Const *c, const char *name)
{
    size_t methods = 0;
    for (int i = 0; m[i].name; ++i) {
        methods++;
    }
    luaL_newmetatable(L, name); /* create metatable */
    lua_pushvalue(L, -1); /* duplicate the metatable */
    lua_setfield(L, -2, "__index"); /* mt.__index = mt */
    luaL_setfuncs(L, f, 0); /* register metamethods */
//    luaL_newlib(L, m); /* create lib table */
    lua_createtable(L, 0, methods);
    luaL_setfuncs(L, m, 0);
    for (; c->name; c++) {
        switch (c->type) {
            case LUA_CT_BOOLEAN: { lua_pushboolean(L, c->value.b); } break;
            case LUA_CT_INTEGER: { lua_pushinteger(L, c->value.i); } break;
            case LUA_CT_NUMBER: { lua_pushnumber(L, c->value.n); } break;
            case LUA_CT_STRING: { lua_pushstring(L, c->value.sz); } break;
        }
        lua_setfield(L, -2, c->name);
    }
    return 1;
}

void luaX_preload(lua_State *L, const char *name, lua_CFunction f)
{
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    lua_pushcfunction(L, f);
    lua_setfield(L, -2, name);
    lua_pop(L, 2);
}

void luaX_require(lua_State *L, const char *name)
{
	lua_getglobal(L, "require");
	lua_pushstring(L, name);
	lua_call(L, 1, 1);
}

int luaX_checkfunction(lua_State *L, int arg)
{
    if (lua_type(L, arg) != LUA_TFUNCTION) {
#if 0
        tag_error(L, arg, LUA_TFUNCTION);
#endif
        return -1;
    }
    lua_pushvalue(L, arg);
    return luaL_ref(L, LUA_REGISTRYINDEX);
}

void luaX_setuserdata(lua_State *L, const char *name, void *p)
{
    lua_pushlightuserdata(L, p);  //Set your userdata as a global
    lua_setglobal(L, name);
}

void *luaX_getuserdata(lua_State *L, const char *name)
{
    lua_getglobal(L, name);
    return lua_touserdata(L, -1);  //Get it from the top of the stack
}

void luaX_getnumberarray(lua_State *L, int idx, double *array)
{
    int j = 0;
    lua_pushnil(L); // first key
    while (lua_next(L, idx) != 0) {
#if 0
        const char *key_type = lua_typename(L, lua_type(L, -2)); // uses 'key' (at index -2) and 'value' (at index -1)
#endif
        array[j++] = lua_tonumber(L, -1);

        lua_pop(L, 1); // removes 'value'; keeps 'key' for next iteration
    }
}
