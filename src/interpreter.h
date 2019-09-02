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

#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include <limits.h>
#include <stdbool.h>

#include "configuration.h"
#include "environment.h"

#include "core/luax.h"
#include "core/timerpool.h"

typedef struct _Interpreter_t {
    const Environment_t *environment;

    double gc_age;

    lua_State *state; // TODO: rename to `L`?

    Timer_Pool_t timer_pool;
} Interpreter_t;

extern bool Interpreter_initialize(Interpreter_t *interpreter, Configuration_t *configuration, const Environment_t *environment);
extern bool Interpreter_init(Interpreter_t *interpreter);
extern bool Interpreter_input(Interpreter_t *interpreter);
extern bool Interpreter_update(Interpreter_t *interpreter, const double delta_time);
extern bool Interpreter_render(Interpreter_t *interpreter, const double ratio);
extern void Interpreter_terminate(Interpreter_t *interpreter);

#endif  /* __INTERPRETER_H__ */