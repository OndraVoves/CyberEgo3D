local World = require( "world" )
local Entity = require( "entity" )

local CSceneNode = require( "scenenode" )
local CMesh		 = require( "mesh" )
local CCamera    = require( "camera" )

script1 = {
	types="script",
}
function script1:onAttrChange( name, old, new )
end

function script1:onEntityChange( ent )
	self.entity = ent
end

function script1:onClientTick( dt )
	local dt_s = dt / 1000.0
	local speed = 50;
	local ent = self.entity
	
	-- Position
	if CE3D.keyb.isKeyDown( 17 ) then
		local p = ent.attr.position;
		p[3] = p[3] + (speed*dt_s)
		  
		ent:setAttr( "position", p )
	end				
				
	if CE3D.keyb.isKeyDown( 31 ) then
		local p = ent.attr.position;
		p[3] = p[3] - (speed*dt_s)
		  
		ent:setAttr( "position", p )
	end
	
	if CE3D.keyb.isKeyDown( 30 ) then
		local p = ent.attr.position;
		p[1] = p[1] - (speed*dt_s)
		  
		ent:setAttr( "position", p )
	end
	
	if CE3D.keyb.isKeyDown( 32 ) then
		local p = ent.attr.position;
		p[1] = p[1] + (speed*dt_s)
		  
		ent:setAttr( "position", p )
	end	
end

function script1:onServerTick( dt )	
end

local w = World()
function w:onLoad()
	ent1 = self:addEntity( {
		name="ssss",
		components={ CSceneNode.new(), CCamera.new() }
	} )
	

	ent2 = self:addEntity( { 
		components={ CSceneNode.new(), CMesh.new(), script1 }
	})
	
	ent1:setAttr( "position", { 0, 0, 150 } )
	ent2:setAttr( "position", { 0, 0, 100 } )
	ent2:setAttr( "mesh_file", "Sinbad.mesh" )
end

return w