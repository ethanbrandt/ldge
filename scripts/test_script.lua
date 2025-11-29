function start(e)
	Log("this cool msg from: "..tostring(e))	
	LogError("uncool error msg from: "..tostring(e))

	local xPos, yPos = GetPosition(e)
	Log(tostring(e).." pos: "..tostring(xPos)..", "..tostring(yPos))
	SetPosition(e, 4.4, 3.1)
	xPos, yPos = GetPosition(e)
	Log(tostring(e).." pos: "..tostring(xPos)..", "..tostring(yPos))

	SetSprite(e, "assets/lettuce.bmp")

	LoadAudioFromFile("assets/game_sword.json")
	LoadAudioFromFile("assets/game_music.json")
end

function update(e, dt)
	local aPressed = IsPressed("A")
	local sPressed = IsPressed("S")
	local dPressed = IsPressed("D")
	local wPressed = IsPressed("W")

	local moveSpeed = 64
	local xVel = 0
	local yVel = 0


	if aPressed then
		Log("IsPressed(\'A\'): "..tostring(aPressed))
		xVel = -moveSpeed
	end
	if sPressed then
		Log("IsPressed(\'S\'): "..tostring(sPressed))
		yVel = -moveSpeed
	end
	if dPressed then
		Log("IsPressed(\'D\'): "..tostring(dPressed))
		xVel = moveSpeed	
	end
	if wPressed then
		Log("IsPressed(\'W\'): "..tostring(wPressed))
		yVel = moveSpeed	
	end

	SetVelocity(e, xVel, yVel)
end