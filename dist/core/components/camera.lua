local ffi = require("ffi")
local C = ffi.C
ffi.cdef[[
	void* camera_new( const char* name, void* scene_node );
	void camera_setFar( void* camera, float far );
	void camera_setNear( void* camera, float near );
	void camera_setLookAt( void* camera, float x, float y, float z );
]] 
	

local function new()
	local cls = {
		types="Camera",
	}
	
	local mt = { __index=cls.__index }
	setmetatable( cls, mt)
		
	-- Look At
	local ent_api = {}
	function ent_api:setLookAt( value )
		self.attributes.look_at = value
		C.camera_setLookAt( self.data.camera,
							value[1], value[2], value[3] )
	end

	function ent_api:getLookAt()
		return self.attributes.look_at
	end

	-- Near
	function ent_api:setNear( v )
		self.attributes.near = v
		C.camera_setNear( self.data.camera, v )
	end
	
	function ent_api:getNear( v )
		return self.attributes.near
	end

	-- Near
	function ent_api:setFar( v )
		self.attributes.Far = v
		C.camera_setFar( self.data.camera, v )
	end
	
	function ent_api:getFar( v )
		return self.attributes.Far
	end


	function cls:onInit( ent_id )
		local ent = EntitySystem[ ent_id ]
		self._ent = ent
		ent.data.camera =  C.camera_new( "C"..tostring( ent_id ), ent.data.scenenode )
		
		--inject api
		for k, v in pairs( ent_api ) do
			ent[k] = v
		end
				
		local attr = self._ent.attributes
		ent:setLookAt( attr.look_at )
		ent:setFar( attr.far )
		ent:setNear( attr.near )				
	end
	
	return cls
end

ComponentsFactory:register( "Camera", new )