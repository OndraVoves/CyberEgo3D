local Entity = require( "entity" )
local World = require( "world" )

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
	print( dt )
end

function script1:onServerTick( dt )
end

function main()
	print( "main" )
	
--	print (CE3D)

 	world = World()
	
	ent1 = world:addEntity( {
		name="ssss",
		components={ CSceneNode.new(), CCamera.new() }
	} )
	

	ent2 = world:addEntity( { 
		components={ CSceneNode.new(), CMesh.new(), script1 }
	})
	
	ent1:setAttr( "position", { 0, 0, 150 } )
	ent2:setAttr( "position", { 0, 0, 100 } )
	ent2:setAttr( "mesh_file", "Sinbad.mesh" )
end

function ce3d.clientTick( dt )
	world:clientTick(dt)

	--print("client:", dt)
	local dt_s = dt / 1000.0
	local speed = 50;
	
	-- Position
	if CE3D.keyb.isKeyDown( 17 ) then
		local p = ent2.attr.position;
		p[3] = p[3] + (speed*dt_s)
		  
		ent2:setAttr( "position", p )
	end				
				
	if CE3D.keyb.isKeyDown( 31 ) then
		local p = ent2.attr.position;
		p[3] = p[3] - (speed*dt_s)
		  
		ent2:setAttr( "position", p )
	end
	
	if CE3D.keyb.isKeyDown( 30 ) then
		local p = ent2.attr.position;
		p[1] = p[1] - (speed*dt_s)
		  
		ent2:setAttr( "position", p )
	end
	
	if CE3D.keyb.isKeyDown( 32 ) then
		local p = ent2.attr.position;
		p[1] = p[1] + (speed*dt_s)
		  
		ent2:setAttr( "position", p )
	end
end

function ce3d.serverTick( dt )
	world:serverTick(dt)
end

function ce3d.keyPressed( keycode )
	print("keyPressed: "..keycode)

	local d = ent2.attr.orientation;	

	-- Direction
	if keycode == 200 then
		d[2] = d[2] + (0.1)	  
	end
	if keycode == 208 then
		d[1] = d[1] + (0.1)	  
	end

	
	ent2:setAttr( "orientation", d )
end

function ce3d.keyReleased( keycode )
	--print("keyReleased")
end

function ce3d.mouseMoved()
	--print("moveMoved")
end

function ce3d.mousePressed()
	--print("movePressed")
end

function ce3d.mouseReleased()
	--print("moveReleased")
end
