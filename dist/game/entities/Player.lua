local Player = {
	class = 'Player',

	attributes = {
		position = { 0, 0, 0 },
		orientation = { 0, 0, 0 },
		visible = true,
		
		look_at = { 0, 0, 0 },
		far =  5000,
		near = 0.01,		
	},
		
	components = {
		"SceneNode",
		"Camera",
	},
}

return Player