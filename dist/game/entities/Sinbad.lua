local Sinbad = {
	class = 'Sinbad',

	attributes = {
		position = { 0, 0, 0 },
		orientation = { 0, 0, 0 },
		visible = true,
		
		mesh = "Sinbad.mesh",
		
		animations = {
			"RunBase", "RunTop",
			"Dance", "IdleTop",
		}
	},
		
	components = {
		"SceneNode",
		"Mesh",
	},
}

function Sinbad:onSpawn()	
	self:playAnim( "Dance" )
	self:playAnim( "RunBase" )
end

function Sinbad:onUpdate( dt )
	local mesh = self.components.Mesh
	mesh:onUpdate(dt)
end

return Sinbad