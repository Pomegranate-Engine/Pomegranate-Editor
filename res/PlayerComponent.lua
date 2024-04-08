local g = Group.find("root")
local e = Entity.new("Thing")

local test_g = Group.new("test")
test_g:add(e)

local transform = e:add_component("class Pomegranate::Transform")
g:add(test_g)
print("Transform Component: ", transform.pos)

transform.pos = Vec2.new(64, 64)

return {
    name="PlayerComponent",
    velocity=Vec2.new(0,0),
    speed=100,
    deceleration=0.1,
}