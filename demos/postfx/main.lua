local System = require("tofu.core").System
local Bank = require("tofu.graphics").Bank
local Canvas = require("tofu.graphics").Canvas
local Font = require("tofu.graphics").Font
local Input = require("tofu.events").Input
local File = require("tofu.io").File
local Class = require("tofu.util").Class

local LITTER_SIZE = 64

local Main = Class.define()

function Main:__ctor()
  Canvas.palette("nes")
  Canvas.transparent({ [0] = false, [13] = true })
  Canvas.background(63)
  Canvas.shader(File.as_string("assets/shaders/water.glsl"))
--Canvas.send("u_strength", 100)

  self.sprites = {}
  self.bank = Bank.new("assets/images/diamonds.png", 16, 16)
  self.font = Font.default(13, 0)
  self.speed = 1.0
  self.running = true
end

function Main:input()
  if Input.is_pressed(Input.START) then
    local Sprite = require("lib.sprite") -- Lazily require the module only in this scope.
    for _ = 1, LITTER_SIZE do
      table.insert(self.sprites, Sprite.new(self.bank, #self.sprites))
    end
  elseif Input.is_pressed(Input.LEFT) then
    self.speed = self.speed * 0.5
  elseif Input.is_pressed(Input.RIGHT) then
    self.speed = self.speed * 2.0
  elseif Input.is_pressed(Input.DOWN) then
    self.speed = 1.0
  elseif Input.is_pressed(Input.SELECT) then
    self.sprites = {}
  elseif Input.is_pressed(Input.Y) then
    self.running = not self.running
  end
end

function Main:update(delta_time)
  if not self.running then
    return
  end
  for _, sprite in pairs(self.sprites) do
    sprite:update(delta_time * self.speed)
  end
end

function Main:render(_)
  Canvas.clear()
  for _, sprite in pairs(self.sprites) do
    sprite:render()
  end
  self.font:write(string.format("FPS: %d", System.fps()), 0, 0, "left")
  self.font:write(string.format("#%d sprites", #self.sprites), Canvas.width(), 0, "right")
end

return Main
