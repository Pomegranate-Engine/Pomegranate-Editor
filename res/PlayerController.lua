print("Lua PlayerController loaded")

local player = Entity.new("player")
local root = Group.find("root")
root:add(player)
print(tostring(player.name))
player:add_component("class Pomegranate::Transform")
function tick(e)

end

function draw(e)

end

function post_draw()

end