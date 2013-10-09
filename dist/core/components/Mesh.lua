local ffi = require("ffi")
local C = ffi.C
ffi.cdef[[
	void* mesh_new( const char* name, void* scene_node );
	void  mesh_del( void *mesh );
]]


local function new()
	local cls = {
		types="Mesh"
	}
	
	local mt = { __index=cls.__index }
	setmetatable( cls, mt)


	-- Mesh
	local ent_api = {}
	function ent_api:setMesh( value )
		self.data.mesh = C.mesh_new( value, self.data.scenenode )
		self.attributes.mesh = value
	end

	function ent_api:getMesh()
		return self.attributes.mesh
	end
	
	function cls:onInit( ent_id )
		local ent = EntitySystem[ ent_id ]
		self._ent = ent
		
		--inject api
		for k, v in pairs( ent_api ) do
			ent[k] = v
		end
				
		local attr = self._ent.attributes
		ent:setMesh( attr.mesh )				
	end
	
	return cls
end

return new
--ComponentsFactory:register( "Mesh", new )