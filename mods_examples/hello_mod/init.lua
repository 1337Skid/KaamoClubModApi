-- even if some code is commented the bindings exist don't worry

assetchanged = false

function get_every_assets_filepath()
	local filepath
	-- game will crash cuz there aren't 8000 textures lol but this is for making sure we are printing everythings
	for i = 0,8000 do
		filepath = asset:GetAssetFilePath(i)
		--if filepath ~= "" and string.find(filepath:lower(), "interface") then print(filepath .. " index: " .. i) end
		if filepath ~= "" then print(filepath .. " index: " .. i) end
	end
end

RegisterEvent("IsInGame", function()
	-- is in game ticks
end)

RegisterEvent("OnJoinGame", function()
	print("joined game")
end)

RegisterEvent("OnAsteroidDestroyed", function(count)
	print("New asteroid destroyed : " .. count)
	level:CreateRadioMessage("Asteroid event", "asteroid destroyed " .. count)
	--mission:NextCampaignMission()
	--player:ToggleCloaking()
	--player:SetPosition(10,10,10)
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
	--local shipinfo = {
	--	id = 0,
	--	price = 10
	--}
	--shipinfo["id"] = math.random(1,29)
	--print(shipinfo["id"])
	--station:SetHangarShipInfo(0, shipinfo)
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
	print("Asset changed!")
	asset:SetAssetFilePath(2050, "mods/hello_mod/my_assets/custom_gof2_interface.aei") -- feel free to custom the gof2 interface with any tools (I don't know if we have any) also you can call this setassetfilepath function while the game is running BUT it won't be edited instantly, for the asset to be edited you need to 'reload' the game aka going to a station, changing system etc..
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