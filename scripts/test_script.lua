function start(e)
	Log("this cool msg from: "..tostring(e))	
	LogError("uncool error msg from: "..tostring(e))

	local xPos, yPos = GetPosition(e)
	Log(tostring(e).." pos: "..tostring(xPos)..", "..tostring(yPos))
	SetPosition(e, 4.4, 3.1)
	xPos, yPos = GetPosition(e)
	Log(tostring(e).." pos: "..tostring(xPos)..", "..tostring(yPos))

	local xVel, yVel = GetVelocity(e)
	Log(tostring(e).." vel: "..tostring(xVel)..", "..tostring(yVel))
	SetVelocity(e, 2.1, 9.78)
	xVel, yVel = GetVelocity(e)
	Log(tostring(e).." vel: "..tostring(xVel)..", "..tostring(yVel))
end