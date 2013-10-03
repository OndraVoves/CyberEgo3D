local Entity = require( "entity" )
local World = require( "world" )

local CSceneNode = require( "scenenode" )
local CMesh		 = require( "mesh" )
local CCamera    = require( "camera" )

function main()
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
end

function clientTick( dt )
	--print("client:", dt)
	local dt_s = dt / 1000.0
	local speed = 50;
	
	-- Position
	if Keyboard.isKeyDown( 17 ) then
		local p = ent2.attr.position;
		p[3] = p[3] + (speed*dt_s)
		  
		ent2:setAttr( "position", p )
	end				
				
	if Keyboard.isKeyDown( 31 ) then
		local p = ent2.attr.position;
		p[3] = p[3] - (speed*dt_s)
		  
		ent2:setAttr( "position", p )
	end
	
	if Keyboard.isKeyDown( 30 ) then
		local p = ent2.attr.position;
		p[1] = p[1] - (speed*dt_s)
		  
		ent2:setAttr( "position", p )
	end
	
	if Keyboard.isKeyDown( 32 ) then
		local p = ent2.attr.position;
		p[1] = p[1] + (speed*dt_s)
		  
		ent2:setAttr( "position", p )
	end

function serverTick( dt )
	--print("server:", dt)
end

function keyPressed( keycode )
	print("keyPressed: "..keycode)
	
	-- Direction
	if keycode == 200 then
		local d = ent2.attr.direction;
		d[2] = d[2] + (0.00000001)
		  
		ent2:setAttr( "direction", d )
	end		
end

function keyReleased( keycode )
	--print("keyReleased")
end

function mouseMoved()
	--print("moveMoved")
end

function mousePressed()
	--print("movePressed")
end

function mouseReleased()
	--print("moveReleased")
end
