if _G.SceneNodeComponent == nil then
	_G.SceneNodeComponent = {}
end

local function new()
	cls = { types="SceneNode" }
	
	local mt = { __index=cls }
	setmetatable( cls, mt)
	
	_G.SceneNodeComponent[#_G.SceneNodeComponent+1] = cls
	
	function cls:onAttrChange( name, old, new )
	end
	
	function cls:onEntityChange( ent )
		ent:initAttr( "position", {0.0, 0.0, 0.0}  )
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