function main()
	print( "main" )
	
	ce3d.game = require( 'game' )
	ce3d.game:onLoad()
	
-- 	world = World()
--	
--	ent1 = world:addEntity( {
--		name="ssss",
--		components={ CSceneNode.new(), CCamera.new() }
--	} )
--	
--
--	ent2 = world:addEntity( { 
--		components={ CSceneNode.new(), CMesh.new(), script1 }
--	})
--	
--	ent1:setAttr( "position", { 0, 0, 150 } )
--	ent2:setAttr( "position", { 0, 0, 100 } )
--	ent2:setAttr( "mesh_file", "Sinbad.mesh" )
end

function ce3d.clientTick( dt )
	local w = ce3d.game.world
	if w then w:clientTick(dt) end
end

function ce3d.serverTick( dt )
	local w = ce3d.game.world
	if w then w:serverTick(dt) end
end

function ce3d.keyPressed( keycode )
end

function ce3d.keyReleased( keycode )
end

function ce3d.mouseMoved()
end

function ce3d.mousePressed()
end

function ce3d.mouseReleased()
end
