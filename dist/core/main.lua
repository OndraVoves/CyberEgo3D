local Entity = require( "entity" )
local World = require( "world" )

local SN     = require( "scenenode" )
local MESH   = require( "mesh" )
local CAMERA = require( "camera" )

C1 = { types="C1" }
function C1:onAttrChange( name, old, new )
	print( name, old, new)
end

function C1:onEntityChange( ent )
	print( "name: "..ent.name )
end


local function main()
	print( "main" )

	s = SN.new()
	
	m = MESH.new()
	c = CAMERA.new()
 	
 	world = World()
	
	ent1 = world:addEntity( {
		name="ssss",
		components={ SN.new(), c }
	} )
	
	
	ent2 = world:addEntity( { 
		components={ s, m }
	})
	
	ent1:setAttr( "position", { 0, 0, 150 } )
	--ent1:setAttr( "look_at", { 0, 0, -1.0 } )
	
	ent2:setAttr( "position", { 0, 0, 100 } )
	ent2:setAttr( "mesh_file", "Sinbad.mesh" )
	
	print( ent1:toJSON() )
	print( ent2:toJSON() )
	
	--SN.clientTick(10)
	
end

function clientTick( dt )
	--print("client:", dt)
end

function serverTick( dt )
	--print("server:", dt)
end

function keyPressed()
	print("keyPressed")
end

function keyReleased()
	print("keyReleased")
end

function mouseMoved()
	print("moveMoved")
end

function mousePressed()
	print("movePressed")
end

function mouseReleased()
	print("moveReleased")
end


main()

