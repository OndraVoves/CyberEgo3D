local ffi = require("ffi")
local C = ffi.C
ffi.cdef[[
	void* scenenode_new(const char* name);
	void scenenode_del(void* node);
	void  scenenode_setPosition( void* node, float x, float y, float z );
	void  scenenode_setOrientation( void* node, float x, float y, float z );
	void  scenenode_setVisible( void* node, bool visible );
]]

local function new()
	local cls = {
		types="SceneNode",
	}

	local mt = { __index=cls }
	setmetatable( cls, mt)

	-- Entity API
	local ent_api = {}
	-- Position
	function ent_api:setPosition( value )
		self.attributes.position = value
		C.scenenode_setPosition( self.data.scenenode,
								 value[1], value[2], value[3] )
	end

	function ent_api:getPosition()
		return self.attributes.position
	end

	-- Orientation
	function ent_api:setOrientation( value )
		self.attributes.orientation = value
		C.scenenode_setOrientation( self.data.scenenode,
								 value[1], value[2], value[3] )
	end

	function ent_api:getOrientation()
		return self.attributes.orientation
	end

	-- Visible
	function ent_api:setVisible( value )
		self.attributes.visible = value
		C.scenenode_setVisible( self.data.scenenode, value )
	end

	function ent_api:isVisible()
		return self.attributes.visible
	end


	function cls:onInit( ent_id )
		local ent = EntitySystem[ ent_id ]
		self._ent = ent
		ent.data.scenenode = C.scenenode_new( "SN"..tostring(ent_id) )
		
		--inject api
		for k, v in pairs( ent_api ) do
			ent[k] = v
		end		
				
		local attr = self._ent.attributes
		ent:setPosition( attr.position )
		ent:setOrientation( attr.orientation )
		--ent:setVisible( attr.visible )				
	end

	function cls:onDestroy()
		C.scenenode_del( self._ent.data.scenenode )
	end	

	return cls
end

return new

--ComponentsFactory:register( "SceneNode", new )