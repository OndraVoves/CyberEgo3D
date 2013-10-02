local Entity = require( "entity" )
local World = require( "world" )

local SN = require( "scenenode" )

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
	print(s.types)

	world = World()
	ent1 = world:addEntity( { name="ssss" } )
	
	ent2 = world:addEntity( { 
		components={ s  }
	})

	ent2:setAttr( "A1", { 0.0, 0.0, 0.0 } )
	ent2:setAttr( "A2", { 0.0, 0.0, 1.0 } )

	print( ent1:toJSON() )
	print( ent2:toJSON() )
	
	SN.clientTick(10)
	
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

