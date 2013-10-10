local ffi = require("ffi")
local C = ffi.C
ffi.cdef[[
	void server_call( int ent, const char* cmd, const char* args_format, ... );
	void client_call( int client, int ent, const char* cmd, const char* args_format, ... );	
]]

local Network = {}

function Network.Make( ent, config )
	ent.Client = {}
	ent.Server = {}
	
	if config.client then
		local c = config.client
		
		for i=1,#c do
			local name = c[i][1]
			local format = c[i][2]
			
			ent.Client[ name ] = function(entity, ...)
				local args = { ... }
				local values = {}
				for i=1,#args do
					local ch = string.char(string.byte(format,i))
					if ch == 'i' then
						values[#values+1] = ffi.new("int", args[i] )
					end
				end			
				C.client_call( -1, entity.id, name, format , unpack( values ) )
			end
		end
	end
	
	if config.server then
		local c = config.server
		
		for i=1,#c do
			local name = c[i][1]
			local format = c[i][2]
					
			ent.Server[ name ] = function(entity, ...)
				local args = { ... }
				local values = {}
				for i=1,#args do
					local ch = string.char(string.byte(format,i))
					if ch == 'i' then
						values[#values+1] = ffi.new("int", args[i] )
					end
				end	
				
				C.server_call( entity.id, name, format , unpack( values ) )
			end
		end
	end	
end

return Network