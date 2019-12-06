--[[
  Copyright (c) 2019 Marco Lizza (marco.lizza@gmail.com)

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
]]--

local Class = require("tofu.util").Class
local Timer = require("tofu.util").Timer

local Tofu = Class.define() -- To be precise, the class name is irrelevant since it's locally used.

function Tofu:__ctor()
  self.main = nil
end

function Tofu:setup()
  self.configuration = require("configuration")
  return self.configuration
end

function Tofu:prepare()
  local Main = require("main") -- Lazily require, to permit a `Tofu:setup()` call prior main script loading.
  self.main = Main.new()
end

function Tofu:process()
  self.main:input()
end

function Tofu:update(delta_time)
  Timer.pool:update(delta_time)
  self.main:update(delta_time)
end

function Tofu:render(ratio)
  self.main:render(ratio)
end

return Tofu.new()
