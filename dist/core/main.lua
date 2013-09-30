Entity = require( "entity" )

player = {
	components = { 
		"transformation",
		"Mesh"
	},

	attributes= {
		position={ 0.0, 0.0, 0.0 }
	},
}

local function main()
	print( "main" )

	ent1 = Entity{ a=55 }
	
end

main()

