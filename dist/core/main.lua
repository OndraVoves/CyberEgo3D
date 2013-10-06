function main()
	print( "main" )
	
	-- Components factory
	ce3d.Components = { }
	
	ce3d.game = require( 'game' )
	ce3d.game:onLoad()
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
