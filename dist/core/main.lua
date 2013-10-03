local Entity = require( "entity" )
local World = require( "world" )

local CSceneNode = require( "scenenode" )
local CMesh		 = require( "mesh" )
local CCamera    = require( "camera" )

local function main()
	print( "main" )

 	world = World()
	
	ent1 = world:addEntity( {
		name="ssss",
		components={ CSceneNode.new(), CCamera.new() }
	} )
	
	
	ent2 = world:addEntity( { 
		components={ CSceneNode.new(), CMesh.new() }
	})
	
	ent1:setAttr( "position", { 0, 0, 150 } )
	
	ent2:setAttr( "position", { 0, 0, 100 } )
	ent2:setAttr( "mesh_file", "Sinbad.mesh" )
	
	print( ent1:toJSON() )
	print( ent2:toJSON() )
end

function clientTick( dt )
	print("client:", dt)
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

