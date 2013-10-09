local ComponentsFactory = {
	_factory = {}
}
	
function ComponentsFactory:register( class, new )
	self._factory[class] = new
end

function ComponentsFactory:newInstance( cls )
	return self._factory[ cls ]()
end 

local mt = {
	__index = ComponentsFactory,
}
	 
return setmetatable( ComponentsFactory, mt )
