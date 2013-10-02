if _G.SceneNodeComponent == nil then
	_G.SceneNodeComponent = {}
end

local function new()
	cls = { types="SceneNode",
			entity=nil }
	
	local mt = { __index=cls }
	setmetatable( cls, mt)
	
	_G.SceneNodeComponent[#_G.SceneNodeComponent+1] = cls
	
	function cls:onAttrChange( name, old, new )
		if self.entity.data.scenenode ~= nil then
			if name == "position" then
				OGRESceneNode.setPosition( self.entity.data.scenenode,
										   new[1], new[2], new[3] )
			elseif name == "direction" then
				OGRESceneNode.setDirection( self.entity.data.scenenode,
										    new[1], new[2], new[3] )				
			end
		end
	end
	
	function cls:onEntityChange( ent )
		print("entity change")
		-- TODO: zrusit data v prechozi entite
		self.entity = ent
				
		ent:initAttr( "scene_node_name", ent.name  )
		ent:initAttr( "position", {0.0, 0.0, 0.0}  )
		ent:initAttr( "direction", {0.0, 0.0, 0.0}  )
				
		ent.data.scenenode = OGRESceneNode.new( ent.attr.scene_node_name )
	end

	function cls:onClientTick( dt )
		print(dt)
	end	
	
	function cls:onServerTick( dt )
	end		
	
	return cls
end

local function clientTick( dt )
	for i, c in ipairs( _G.SceneNodeComponent ) do
		c:onClientTick( dt )
	end
end

local function serverTick( dt )
	for i, c in ipairs(_G.SceneNodeComponent ) do
		c:onServerTick( dt )
	end
end


return { new=new,
		 clientTick=clientTick,
		 serverTick=serverTick,
	   } 