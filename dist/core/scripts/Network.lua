local ffi = require("ffi")
local C = ffi.C
ffi.cdef[[
	void server_call( int ent, const char* cmd, const char* args_format, ... );
	void client_call( int client, int ent, const char* cmd, const char* args_format, ... );
]]

local Network = { entity={}, gamelogic={} }

----
-- TODO: ukladat a radit... odeslat az na pozadani
----

local function convert_values( format, in_val  )
    local values = {}

    for i=1,#in_val do
        local ch = string.char(string.byte(format,i))
        if ch == 'i' then
            values[#values+1] = ffi.new("int", in_val[i] )

        elseif ch == 'f' then
            values[#values+1] = ffi.new("float", in_val[i] )
        end
    end

    return values
end

Network.clientcall_c = 0
Network.clientcall_buf = {}

Network.servercall_c = 0
Network.servercall_buf = {}
function Network:sendClientCallBuffer()
    local buf = self.clientcall_buf
    for i=1, self.clientcall_c do
        C.client_call( unpack( buf[i] ) )
    end

    self.clientcall_c = 0
end

function Network:sendServerCallBuffer()
    local buf = self.servercall_buf
    for i=1,self.servercall_c do
        C.server_call( unpack( buf[i] ) )
    end
    
    self.servercall_c = 0
end

local function clientcall( to, id, name, format, ... )
    local args = { ... }
    local values = convert_values( format, args )
    local buf = Network.clientcall_buf

    Network.clientcall_c = Network.clientcall_c + 1

    buf[Network.clientcall_c] = { to, id, name, format , unpack( values ) }
    --C.client_call( to, id, name, format , unpack( values ) )
end

local function servercall( id, name, format, ... )
    local args = { ... }
    local values = convert_values( format, args )

    local buf = Network.servercall_buf
    
    Network.servercall_c = Network.servercall_c + 1

    buf[Network.servercall_c] = { id, name, format , unpack( values ) }    
    --C.server_call( id, name, format , unpack( values ) )
end

function Network.entity.make( tbl )
    local cls = tbl.class

    cls.Client = {}
    cls.Server = {}
   		
    for _, c in ipairs(tbl.client) do
        local name = c[1]
        local format = c[2]

        cls.Client[ name ] = function(entity, ...)
            clientcall( -1, entity.id, name, format, ... )
        end
    end

    for _, s in ipairs(tbl.server) do
        local name = s[1]
        local format = s[2]
        
        cls.Server[ name ] = function(entity, ...)
            servercall( entity.id, name, format, ... )
        end
    end
end

function Network.gamelogic.make( tbl )
    local cls = tbl.class


    cls.Client = {}
    cls.Server = {}

    for _, c in ipairs(tbl.client) do
        local name = c[1]
        local format = c[2]
			
        cls.Client[ name ] = function(...)
            clientcall( -1, -1, name, format, ... )
        end
    end

	for _, s in ipairs(tbl.server) do
        local name = s[1]
        local format = s[2]
			
        cls.Server[ name ] = function( ... )
            servercall( -1, name, format, ... )
        end
    end
end

return Network
