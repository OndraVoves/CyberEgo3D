local function new()
	cls = {
		types="SceneNode",
		entity=nil
	}

	local mt = { __index=cls }
	setmetatable( cls, mt)

	function cls:onAttrChange( name, old, new )
		if self.entity.data.scenenode ~= nil then
			if name == "position" then
				CE3D.OGRESceneNode.setPosition( self.entity.data.scenenode,
				new[1], new[2], new[3] )
				
			elseif name == "orientation" then
				CE3D.OGRESceneNode.setOrientation( self.entity.data.scenenode,
				new[1], new[2], new[3] )
				
			elseif name == "visible" then
				CE3D.OGRESceneNode.setVisible( self.entity.data.scenenode,
				new )
			end
		end
	end

	function cls:onEntityChange( ent )
		-- TODO: zrusit data v prechozi entite
		self.entity = ent
		
		ent.data.scenenode = CE3D.OGRESceneNode.new( ent.name )
		
		ent:initAttr( "position", {0.0, 0.0, 0.0} )
		ent:initAttr( "orientation", {0.0, 0.0, 0.0} )
		ent:initAttr( "visible", true )
	end

	function cls:onClientTick( dt )
	end

	function cls:onServerTick( dt )
	end

	return cls
end

ce3d.Components.SceneNode = new