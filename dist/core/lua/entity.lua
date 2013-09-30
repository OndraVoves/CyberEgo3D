local function new( obj )
	local cls = obj or {}
	
	local mt = { __index=cls }
	setmetatable( cls, mt)
	return cls
end

return new