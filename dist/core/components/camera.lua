local function new()
	cls = { types="Camera",
			entity=nil }
	
	local mt = { __index=cls.__index }
	setmetatable( cls, mt)
	
	function cls:onAttrChange( name, old, new )
		local ent = self.entity
		
		if ent and ent.data.camera ~= nil then
			if name == "look_at" then
				CE3D.OGRECamera.setLookAt( ent.data.camera, new[1], new[2], new[3] )
				
			elseif name == "near_clip" then
				CE3D.OGRECamera.setNear( ent.data.camera, new )
			
			elseif name == "far_clip" then
				CE3D.OGRECamera.setFar( ent.data.camera, new )
				
			end
		end
	end
	
	function cls:onEntityChange( ent )
		-- TODO: zrusit data v prechozi entite
		self.entity = ent

		ent.data.camera = CE3D.OGRECamera.new( ent.name.."_Camera", ent.data.scenenode )
		
		ent:initAttr( "look_at", { 0.0, 0.0, 0.0 } )
		ent:initAttr( "near_clip", 0.01 )
		ent:initAttr( "far_clip", 5000 )
	end

	function cls:onClientTick( dt )
	end	
	
	function cls:onServerTick( dt )
	end		
	
	return cls
end

ce3d.Components.Camera = new