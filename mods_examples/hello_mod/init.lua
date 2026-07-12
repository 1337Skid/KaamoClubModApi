-- even if some code is commented the bindings exist don't worry

local assetchanged = false
local mytextureid = 0
local mysprite = 0
local mybutton = 0
local mybutton2 = 0
local mybutton3 = 0
local mychoicewindow = 0
local mychoicewindow2 = 0

function get_every_assets_filepath()
	local filepath
	-- game will crash cuz there aren't 8000 textures lol but this is for making sure we are printing everythings
	for i = 0,8000 do
		filepath = asset:GetAssetFilePath(i)
		--if filepath ~= "" and string.find(filepath:lower(), "interface") then print(filepath .. " index: " .. i) end
		if filepath ~= "" then print(filepath .. " index: " .. i) end
	end
end

HookFunction("GameText::getText", function(ctx)
	--print(string.format("0x%X", ctx.returnaddr))
	--ctx:OverrideText("Hello from lua")
	--print(ctx.id)
	ctx:call() -- ctx:call() isn't required because gametext::gettext always needs to be called or else the game crash
end)

HookFunction("AbyssEngine::PaintCanvas::SetColor", function(ctx)
	--print(string.format("0x%X", ctx.returnaddr))
	--print(string.format("0x%X", ctx.hexcolor))
	--ctx:OverrideColor(0, 0, 255, 255) -- r,g,b,a
	ctx:call() -- ctx:call() isn't required
end)

HookFunction("Globals::init", function(ctx)
	mytextureid = asset:CreateTexture("mods/hello_mod/my_assets/Trollface.aei")
	mysprite = asset:CreateSprite(mytextureid, 0)
	print("SPRITE ID INJECTED : " .. mysprite)
    	mychoicewindow = ctx:CreateChoiceWindow("Hello world", "Cool choice window", true, 
        	function()
            		print("ok")
			wait(2)
			print("choice window async")
        	end, 
        	function()
            		print("bye")
        	end
	)
    	mychoicewindow2 = ctx:CreateChoiceWindow("Yo", "Hey another one!", true, 
        	function()
            		print("ok")
			wait(2)
			print("choice window async")
        	end, 
        	function()
            		print("bye")
        	end
	)
	mybutton = ctx:CreateTouchButton("Hello world", "7.", 800, 900, -1, 0, function()
		mybutton:SetText("Clicked!")
		player:ResetGame()
		player.money = 1337 -- starter money
		mission.id = 78 -- valkyrie station animation mission id
		player:SetStation(108)
		engine:SetCurrentApplicationModule(2) -- set the game in the module MGame
		wait(5)
		print("async works")
	end)
	-- 999 = when we click on a blueprint, 997 = blueprint menu 996 = shop tab, 998 = ship tab
	mybutton2 = ctx:CreateTouchButton("Hello world", "", 800, 900, -1, 999, function()
		mybutton2:SetText("Wow")
		mychoicewindow:Show()
	end)
	mybutton3 = ctx:CreateTouchButton("Show choice window", "8.", 1200, 300, -1, 0, function()
		mychoicewindow:Show()
	end)
	-- ctx:call() doesn't exist there because it makes no sense to stop globals::init, your game will just crash if it was a thing
end)

RegisterEvent("IsInGame", function()
	-- is in game ticks
end)

RegisterEvent("OnJoinGame", function()
	print("joined game")
	--for i = 0, 20 do
	--	system:Get(i).mapcoordinate_x = math.random(30,150)
	--	system:Get(i).mapcoordinate_y = math.random(30,150)
	--	system:Get(i).mapcoordinate_z = math.random(30,150)
	--end
	--wait(0.5)
	--station.level = 99
	--for i = 0, 105 do
	--	station:Get(i).name = "hi " .. i
	--end
	--wait(1)
	--station.name = "Hello world!!"
end)

RegisterEvent("OnAsteroidDestroyed", function(count)
	print("New asteroid destroyed : " .. count)
	local imageinfo = {
		race = 6,
		hair = 0,
		eyes = 0,
		mouth = 0,
		armor = 0
	} -- a bobolian
	--level:CreateRadioMessage("Asteroid event", "asteroid destroyed " .. count, imageinfo)
	--local dialogues = {
    	--	{ name = "Keith T. Maxwell",  content = "hello", image = {0, 0, 0, 0, 0}, isplayer = 1 },
    	--	{ name = "lil snail", content = "hi", image = {6, 1, 2, 0, 0}, isplayer = 0 },
	--	{ name = "Keith T. Maxwell",  content = "bleh", image = {0, 0, 0, 0, 0}, isplayer = 1 },
    	--	{ name = "lil snail", content = "OK!", image = {6, 1, 2, 0, 0}, isplayer = 1 },
	--}
    	--wait(5)
	--level:CreateDialogueWindow(dialogues)
	--mission:NextCampaignMission()
	--player:ToggleCloaking()
	--player:SetPosition(10,10,10)
	--local points = {
    	--	{ pos = {0, 8000, 0}, duration = 5  },
    	--	{ pos = {2500, 200, 0}, duration = 5,  shake = 8.0, shakefrequency = 100 },
    	--	{ pos = {0, 0, 30000}, duration = 5  },
	--}
	--level:CreateCutScene(points)
	--local route = level:CreateRoute({
    	--	{ 100000.0, 0.0, 100000.0 },
    	--	{ 200000.0, 0.0, 200000.0 },
    	--	{ 300000.0, 0.0, 100000.0 },
	--})
	--if route:IsValid() then
	--	print("ROUTE CREATED!")
	--end
	--level:GetEntities()[2]:SetRoute(route)
	--for k, v in pairs(level:GetEntities()) do
	--	print(v)
	--	v:SetRoute(route)
	--end
	--level:CreateAsteroid(0, 0, 50000, 30, 17000)
	--for k, v in pairs(level:GetEntities()) do
	--	print(v)
	--end
	mychoicewindow:Show()
end)

RegisterEvent("OnEnemieKilled", function(count)
	print("wow congrats you killed " .. count .. " enemies!!!")
end)

RegisterEvent("OnCargoChanged", function(count)
	print("Current cargo : " .. count)
end)

RegisterEvent("OnStationChanged", function(id)
	if station:IsVoid() then
		print("wow you joined the voids!!!")
		return
	end
	print("New station ! : " .. id)
end)

RegisterEvent("OnStationDocked", function()
	print("Docked on this station: " .. station.name)
	wait(5)
	mychoicewindow2:Show()
	--local shipinfo = {
	--	id = 40,
	--	price = 10
	--}
	--shipinfo["id"] = math.random(1,29)
	--print(shipinfo["id"])
	--station:SetHangarShipInfo(0, shipinfo)
	--station:AddHangarShip(4)
	--local shipinfo = {
	--	maxcargo = 9999,
	--	armor = 9999,
	--	price = 99999999,
	--	maxhealth = 9999
	--}
	--player:SetShipInfo(shipinfo)
	--for i = 0, 90000 do
	--	if asset:GetText(i) == "Error Language" then return end
	--	print("text: " .. asset:GetText(i) .. " id : " .. i)
	--end
	--print(asset:GetText(1059))
end)

RegisterEvent("IsInMainMenu", function()
	if assetchanged then return end
	--get_every_assets_filepath()
	--print("Asset changed!")
	--asset:SetAssetFilePath(2050, "mods/hello_mod/my_assets/custom_gof2_interface.aei") -- feel free to custom the gof2 interface with any tools (I don't know if we have any) also you can call this setassetfilepath function while the game is running BUT it won't be edited instantly, for the asset to be edited you need to 'reload' the game aka going to a station, changing system etc..
	assetchanged = true
end)

RegisterEvent("OnMoneyChanged", function(money)
	print(money)
end)

RegisterEvent("OnSystemChanged", function(id)
	print("System id : " .. id)
	print("System name : " .. system.name)
	print("System risk : " .. system.risk)
	print("System faction : " .. system.faction)
	print("System map coordinate x : " .. system.mapcoordinate_x)
	print("System map coordinate y : " .. system.mapcoordinate_y)
	print("System map coordinate z : " .. system.mapcoordinate_z)
	print("System texture id : " .. system.textureid)
	print("Station name : " .. station.name)
	print("Station level : " .. station.level)
	print("Station id : " .. station.id)
	print("Station angar items count : " .. station.hangaritemscount)
	print("Station angar ships count : " .. station.hangarshipscount)
	print("Station agents count : " .. station.agentscount)
	for i = 0, station.agentscount - 1 do
		print("Station agent #" .. i .. " name: " .. station:GetAgentName(i) .. " | faction: " .. station:GetAgentFaction(i))
	end
	print("Player money : " .. player.money)
	print("Player Ship cargo : " .. player:GetShipInfo()["cargo"])
	print("Player Ship maxcargo : " .. player:GetShipInfo()["maxcargo"])
	print("Player Ship armor : " .. player:GetShipInfo()["armor"])
	print("Player Ship maxhealth : " .. player:GetShipInfo()["maxhealth"])
	print("Player Enemies killed : " .. player.enemieskilled)
	print("Player level : " .. player.level)
	print("Player visisted stations : " .. player.visitedstations)
	print("Player Completed side missions : " .. mission.completedsidemissions)
	print("Player Cargo salvaged count : " .. player.cargosalvagedcount)
	print("Player Asteroids destroyed count : " .. player.asteroidsdestroyedcount)
	print("Player current mission : " .. mission.id)
	if player:HasShipArmor() then
		print("The ship has an armor")
	end
	if player:HasJumpDrive() then
		print("The ship has a jump drive")
	end
	if player:IsDocked() then
		print("player is docked in a station")
	end
	if not system:IsVisible(26) then
		print("Shima system is not visible")
	end

	print("GOF2 Interface AEI : " .. asset:GetAssetFilePath(2050)) -- 2050 is the id of the interface
end)

RegisterEvent("OnUpdate", function()
	-- every ticks
end)

HookFunction("Level::createMission", function(ctx)
	print("Hello level!")
	--ctx:CreateStaticObject(0, 55000, 5000, 10000) -- object_type(0=pirate station;1=valkyrie turret), x, y, z
	--for i = 0, 100 do
	--	ctx:CreateFighter(math.random(0,41), 8) -- meshid, faction
	--end
	--local mykiplayer = ctx:CreateFighter(math.random(0,41), 5) -- meshid, faction
	--local route = level:CreateRoute({
    	--	{ 100000.0, 0.0, 100000.0 },
    	--	{ 200000.0, 0.0, 200000.0 },
    	--	{ 300000.0, 0.0, 100000.0 },
	--})
	--mykiplayer:SetRoute(route)
	--print(mykiplayer)
	--local object = ctx:CreateStaticObject(0, 55000, 5000, 10000)
	--print(object)
	ctx:call() -- call original function
end)

HookFunction("Level::createCampaignMission", function(ctx)
	print("Hello i'm in a mission")
	--ctx:CreateStaticObject(0, 55000, 5000, 10000) -- object_type(0=pirate station;1=valkyrie turret), x, y, z
	--for i = 0, 3 do
	--	ctx:CreateFighter(math.random(0,41), 8) -- meshid, faction
	--end
	ctx:call() -- call original function
end)

HookFunction("ModMainMenu::OnRender2D", function(ctx)
	ctx:DrawString("Hello main menu !!", 30, 30, 255, 255, 255, 255)
	sprite = asset:Image2DCreate(mysprite) -- put our sprite in the textures pool
	ctx:DrawImage2D(sprite, 20, 200, 255, 255, 255, 255)
	ctx:call()
end)

local full_text = "Hello MGame!!! yes you can animate your string !"
local current_length = 0
local frame_counter = 0
HookFunction("MGame::OnRender2D", function(ctx)    
    if current_length < string.len(full_text) then
        frame_counter = frame_counter + 1
        if frame_counter >= 4 then
            current_length = current_length + 1
            frame_counter = 0
        end
    end
    local display = string.sub(full_text, 1, current_length)
    display = display .. "_"
    ctx:DrawString(display, 500, 30, 255, 0, 0, 255)
    ctx:call()
end)