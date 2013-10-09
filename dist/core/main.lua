EntitySystem = require("EntitySystem")
ComponentsFactory = require("ComponentsFactory")

local EntCls1 = {
	class = 'class1',

	attributes = {
		position = { 0, 0, 150 },
		orientation = { 0, 0, 0 },
		visible = true,
		
		look_at = { 0, 0, 0 },
		far =  5000,
		near = 0.01,
	},
		
	components = {
		"SceneNode",
		"Camera",
	}
}
function EntCls1:onSpawn()
	print( "a1 spawn id:", self.id )
end

local EntCls2 = {
	class = 'class2',

	attributes = {
		position = { 0, 0, 100 },
		orientation = { 0, 0, 0 },
		visible = true,
		
		mesh = "Sinbad.mesh",
	},
		
	components = {
		"SceneNode",
		"Mesh",
	},
}
function EntCls2:onSpawn()
	print( "b1 spawn id:", self.id )
end


EntitySystem:register( EntCls1 )
EntitySystem:register( EntCls2 )

function main()
	print( "main" )
	
	ComponentsFactory:loadFromDirs( './core/components/' )
	ComponentsFactory:loadFromDirs( './game/components/' )

--	boot_components()
	
	--local id1 = EntitySystem:spawn( 'class1' )
	--id2 = EntitySystem:spawn( 'class2' )
	
	
		
--	local EntityFactory = require("EntityFactory")
--	local ef = EntityFactory()
		
--	ef:register( EntCls1 )
--
--	local e = ef["class1"]	
--	local e2 = ef["class1"]
	
	
--	ef["player"] = { name="aaa", class='class'}
	
--	print( ef["player"] )

	-- Components factory
	--ce3d.Components = { }
	
	--ce3d.game = require( 'game' )
	--ce3d.game:onLoad()
end

function ce3d.clientTick( dt )
--	local w = ce3d.game.world
--	if w then w:clientTick(dt) end
end

function ce3d.serverTick( dt )
--	local w = ce3d.game.world
--	if w then w:serverTick(dt) end
end

function ce3d.keyPressed( keycode )
	local ent = EntitySystem[ id2 ]
	local v = ent:getPosition()
	
	ent:setPosition{ v[1], v[2], v[3] + 1 }
end

function ce3d.keyReleased( keycode )
end

function ce3d.mouseMoved()
end

function ce3d.mousePressed()
end

function ce3d.mouseReleased()
end
