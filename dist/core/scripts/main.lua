EntitySystem = require("scripts/EntitySystem")
ComponentSystem = require("scripts/ComponentSystem")
Network  = require("scripts/Network")

local net = Network
Game = {
    client={},
    server={},
}

net.gamelogic.make {
	class=Game,
    
    server = {
        { "pong", "n" },
    },
    
    client = {
        { "ping", "n" }
    }
}

function Game.server.pong()
	--print("pong")
end
function Game.client.ping()
	--print("ping")
end

function Game:onClientConnect( id )
	print( 'CLIENT CONNECT', id )
end

function Game:onClientDisconnect( id )
	print( 'CLIENT CONNECT', id )
end

function Game:onUpdate( dt )
    if not isServer then
        --self.Server.pong( )
    else
        --self.Client.ping( )
    end
end

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
	Game:onUpdate( dt )
	EntitySystem:update( dt )
	
	Network:sendServerCallBuffer()
	Network:sendClientCallBuffer()
end

function ce3d.keyPressed( keycode )
	print( "Keycode", keycode )
	if keycode == 19 then
		EntitySystem:reloadFactory()

		local m = json.decode( file2str("map.json") )
		EntitySystem:load( m )
	end
end

function ce3d.keyReleased( keycode )
end

function ce3d.mouseMoved()
end

function ce3d.mousePressed()
end

function ce3d.mouseReleased()
end

function ce3d.clientCall( type, ent_id, cmd, ... )
    local args = { ... }
	
    if ent_id >= 0 then
        local e = EntitySystem:getEntity( ent_id )

        if type == 1 then
            e.client[cmd]( e, unpack(args) )
        else
            e.server[cmd]( e, unpack(args) )
        end

    else
        if type == 1 then
            Game.client[cmd]( unpack(args) )
        else
            Game.server[cmd]( unpack(args) )
        end
    end
end

function ce3d.onClientConnect( id )
	Game:onClientConnect(id)
end

function ce3d.onClientDisconnect( id )
	Game:onClientDisconnect(id)
end
