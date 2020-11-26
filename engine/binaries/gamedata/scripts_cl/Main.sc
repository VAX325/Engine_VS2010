--Main File

function Main ()
	
	CLogClient:Msg("Lua started!")
	
end

function Update ()
	str = strPlusStr("FPS IS : ", GetFPS())
	CLogClient:Msg()
end