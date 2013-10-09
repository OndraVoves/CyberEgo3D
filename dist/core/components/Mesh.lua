local ffi = require("ffi")
local C = ffi.C
ffi.cdef[[
	void* mesh_new( const char* name, void* scene_node );
	void  mesh_del( void *mesh );
	void* mesh_get_anim( void* mesh, const char* name );
	
	void mesh_anim_update( void *state, int dt );
	void mesh_anim_play( void *state );	
]]


local function new()
	local cls = {
		types="Mesh",
		anims={},
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
	
	function ent_api:playAnim( name )
		local a = self.components.Mesh.anims[name]
		C.mesh_anim_play( a )
	end
	
	local function parseAnimStateAttr()
		local anim = cls._ent.attributes.animations
		cls.anims = { }
		
		for k, v in pairs( anim ) do
			cls.anims[ v ] = C.mesh_get_anim( cls._ent.data.mesh, v )
		end
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
		
		if attr.animations then
			parseAnimStateAttr()
		end
	end
	
	function cls:onDestroy()
		C.mesh_del( self._ent.data.mesh )
	end	
	
	function cls:onUpdate( dt )
		for _, v in pairs(self.anims)  do
			C.mesh_anim_update( v ,dt )
		end
	end
	
	return cls
end

return new
--ComponentsFactory:register( "Mesh", new )