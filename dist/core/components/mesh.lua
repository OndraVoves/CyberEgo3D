if _G.MeshComponent == nil then
	_G.MeshComponent = {}
end

local function new()
	cls = { types="Mesh",
			entity=nil }
	
	local mt = { __index=cls.__index }
	setmetatable( cls, mt)
	
	_G.MeshComponent[#_G.MeshComponent+1] = cls
	
	function cls:onAttrChange( name, old, new )
		local ent = self.entity
		
		if ent and ent.data.scenenode ~= nil then
			if name == "mesh_file" then
				--todo delete
				if #new ~= 0 then
					ent.data.mesh = OGREMesh.new( ent.attr.mesh_file, ent.data.scenenode )
				end
			end
		end
	end
	
	function cls:onEntityChange( ent )
		-- TODO: zrusit data v prechozi entite
		self.entity = ent
		
		ent:initAttr( "mesh_file", "" )
	end

	function cls:onClientTick( dt )
	end	
	
	function cls:onServerTick( dt )
	end		
	
	return cls
end

local function clientTick( dt )
	for i, c in ipairs( _G.MeshComponent ) do
		c:onClientTick( dt )
	end
end

local function serverTick( dt )
	for i, c in ipairs(_G.MeshComponent ) do
		c:onServerTick( dt )
	end
end


return { new=new,
		 clientTick=clientTick,
		 serverTick=serverTick,
	   } 