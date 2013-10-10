local net = require('Network')

local Sinbad = {
	class = 'Sinbad',

	attributes = {
		position = { 0, 0, 0 },
		orientation = { 0, 0, 0 },
		visible = true,

		mesh = "Sinbad.mesh",
	},

	components = {
		"SceneNode",
		"Mesh",
	},
	
		
	server = {},
	client = {},
}

net.Make(Sinbad, {
	client = {
		{ "foo", 'i' },
	},
	
	server = {
		{ "bar", 'i' },
	}
})

function Sinbad:onSpawn()
	self.play = false
end

function Sinbad:onUpdate( dt )
	if CE3D.keyb.isKeyDown( 30 ) then
		self.play = not self.play

		if self.play then
			self:startAnimation( "Dance", true )
		else
			self:stopAnimation( "Dance" )
		end
	end

	if not isServer then
		self.Server.bar(self, 45454 )
	else
		self.Client.foo(self, 50 )
	end

	local mesh = self.components.Mesh
	mesh:onUpdate(dt)
end

function Sinbad.client.foo( ent, number )
	print('FOOO', number)
end

function Sinbad.server.bar( ent, number )
	print('BAR', number)
end

return Sinbad