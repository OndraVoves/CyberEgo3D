local net = Network

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

net.entity.make{
    class=Player,
	client = {
            { "move", 'fff' },
	},

	server = {
            { "move", 'iiii' },
	}
}

function Player:onUpdate( dt )
	if not isServer then
		local u = CE3D.keyb.isKeyDown( 200 )
		local d = CE3D.keyb.isKeyDown( 208 )
		local l = CE3D.keyb.isKeyDown( 203 )
		local r = CE3D.keyb.isKeyDown( 205 )

		if u or d or l or r then
			self:move( u, d, l, r )
			self.Server.move(self, u, d, l, r )
		end
	end
end

function Player.server.move( ent, up, down, left, right )
	ent:move( up>0, down>0, left>0, right>0 )
	local v = ent:getPosition( v )

	ent.Client.move( ent, v[1], v[2], v[3] )
end

function Player.client.move( ent, x, y, z )
	ent:setPosition{ x, y, z }
end

function Player:move( up, down, left, right )
	local v = self:getPosition()
	local d = 0.0001

	if up then
		v[2] = v[2] + d;
	end

	if down then
		v[2] = v[2] - d;
	end

	if left then
		v[1] = v[1] + d;
	end

	if right then
		v[1] = v[1] - d;
	end

	self:setPosition( v )
end

return Player
