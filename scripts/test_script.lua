function start(e)
	Log("this cool msg from: "..tostring(e))	
	LogError("uncool error msg from: "..tostring(e))

	local xPos, yPos = GetPosition(e)
	Log(tostring(e).." pos: "..tostring(xPos)..", "..tostring(yPos))
	--SetPosition(e, 4.4, 3.1)
	xPos, yPos = GetPosition(e)
	Log(tostring(e).." pos: "..tostring(xPos)..", "..tostring(yPos))

	if (e == 1) then
		SetSpriteSheet(e, "assets/lettuce.bmp", 32, 32)
	end

	if (e == 0) then
		SetSpriteSheet(e, "assets/sshot0002.bmp", 32, 32)
	end

	SetSpriteIndex(e, 5, 5)

	LoadAudioFromFile("assets/game_sword.json")
	--LoadAudioFromFile("assets/game_music.json")
end

function update(e, dt)
	--xPos, yPos = GetPosition(e)
	--Log(tostring(e).." pos: "..tostring(xPos)..", "..tostring(yPos))
	if e ~= 0 then
		return
	end

	local aPressed = IsPressed("A")
	local sPressed = IsPressed("S")
	local dPressed = IsPressed("D")
	local wPressed = IsPressed("W")

	local moveSpeed = 64
	local xVel = 0
	local yVel = 0


	if aPressed then
		xVel = -moveSpeed
	end
	if sPressed then
		yVel = -moveSpeed
	end
	if dPressed then
		xVel = moveSpeed	
	end
	if wPressed then
		yVel = moveSpeed	
	end

	SetVelocity(e, xVel, yVel)
end

function on_collision(e, o)
	Log("COLLISION")
end