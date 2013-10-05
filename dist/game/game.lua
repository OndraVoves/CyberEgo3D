local game = {
	name="Example"
}

function game:onLoad()
	print("Game loaded")
	
	self:loadMap( 'level1' )
end

function game:loadMap( name )
	if self.level then
		-- unload world
	end
	
	self.world = require( name )
	self.world:onLoad()
end

local mt = { __index=game.__index }
return setmetatable( game, mt)
