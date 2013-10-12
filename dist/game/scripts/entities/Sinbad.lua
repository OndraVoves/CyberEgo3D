local net = Network

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

net.entity.make{
    class=Sinbad,
	client = {
		{ "foo", 'i' },
		{ "anim", 'i' },
	},

	server = {
		{ "bar", 'i' },
	}
}

function Sinbad:onSpawn()
	self.play = false
end

function Sinbad:onUpdate( dt )
	if keyb.isKeyDown( 30 ) then
		self.play = not self.play

		if self.play then
			self:startAnimation( "Dance", true )
		else
			self:stopAnimation( "Dance" )
		end
	end

	if not isServer then
		--self.Server.bar(self, 45454 )
	else
		--self.Client.foo(self, 50 )
		--self.Client.anim(self, 1 )
	end

	local mesh = self.components.Mesh
	mesh:onUpdate(dt)
end

function Sinbad.client.foo( ent, number )
	--print('FOOO', number)
end

function Sinbad.server.bar( ent, number )
	--print('BAR', number)
end

function Sinbad.client.anim( ent, play )
	if play then
		ent:startAnimation( "Dance", true )
	else
		ent:stop( "Dance" )
	end
end

return Sinbad
