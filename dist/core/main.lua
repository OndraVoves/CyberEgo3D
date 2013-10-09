EntitySystem = require("EntitySystem")
ComponentSystem = require("ComponentSystem")

function str2file( file, string )
	local f,err = io.open(file,"w")
	if not f then return print(err) end
	f:write(string)
	f:close()
end

function file2str( file )
	local f,err = io.open(file,"r")
	
	if not f then return print(err) end
	local content = f:read("*all")
	
	f:close()
	
	return content
end

function main()
	print( "main" )
	
	ComponentSystem:reloadFactory()
	EntitySystem:reloadFactory()
	
	local json = require('json')	
	local m = json.decode( file2str("map.json") )
	EntitySystem:load( m )
end

function ce3d.update( dt )
	EntitySystem:update( dt )
end

function ce3d.keyPressed( keycode )
	if keycode == 19 then
		EntitySystem:reloadFactory()
			
		local m = json.decode( file2str("map.json") )
		EntitySystem:load( m )	
	end
--	local ent = EntitySystem[ id2 ]
--	local v = ent:getPosition()
--	
--	ent:setPosition{ v[1], v[2], v[3] + 1 }
end

function ce3d.keyReleased( keycode )
end

function ce3d.mouseMoved()
end

function ce3d.mousePressed()
end

function ce3d.mouseReleased()
end
