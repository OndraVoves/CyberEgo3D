local net = require('Network')

local Player = {
	class = 'Player',

	attributes = {
		position = { 0, 0, 0 },
		orientation = { 0, 0, 0 },
		visible = true,
		
		look_at = { 0, 0, 0 },
		far =  5000,
		near = 0.01,		
	},
		
	components = {
		"SceneNode",
		"Camera",
	},
	
	server = {},
	client = {},
}

net.Make(Player, {
	client = {
		{ "move", 'iii' },
	},
	
	server = {
		{ "move", 'iiii' },
	}
})

function Player:onUpdate( dt )
	if not isServer then
		local u = CE3D.keyb.isKeyDown( 200 )
		local d = CE3D.keyb.isKeyDown( 208 )
		local l = CE3D.keyb.isKeyDown( 203 )
		local r = CE3D.keyb.isKeyDown( 205 )
		
		self.Server.move(self, u, d, l, r )
	end
end

function Player.server.move( ent, up, down, left, right )
	local v = ent:getPosition()
	
	if up ~= 0 then
		v[2] = v[2] + 1; 
	end

	if down ~= 0 then
		v[2] = v[2] - 1; 
	end

	if left ~= 0 then
		v[1] = v[1] + 1; 
	end

	if right ~= 0 then
		v[1] = v[1] - 1; 
	end
	
	ent:setPosition( v )
	ent.Client.move( ent, v[1], v[2], v[3] )
end

function Player.client.move( ent, x, y, z )
	ent:setPosition{ x, y, z }	
end

return Player