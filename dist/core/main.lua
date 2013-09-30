local Entity = require( "entity" )
local World = require( "world" )

C1 = { types="C1" }
function C1:onAttrChange( name, old, new )
	print( name, old, new)
end

function C1:onEntityChange( ent )
	print( "name: "..ent.name )
end

local function main()
	print( "main" )

	world = World()
	ent1 = world:addEntity( { name="ssss" } )
	
	ent2 = world:addEntity( { 
		components={ C1 }
	})

	ent2:setAttr( "A1", {0.0, 0.0, 0.0} )
	ent2:setAttr( "A2", {0.0, 0.0, 1.0} )

	print( ent1.name )
	print( ent2.name )

	local json = require( "json" )
	local e = json.encode
	
	print( e( ent1:getDataTbl() ) )
	print( e( ent2:getDataTbl() ) )	
end

main()

