-- even if some code is commented the bindings exist don't worry

isingame = false
assetchanged = false

function get_every_assets_filepath()
	local filepath
	for i = 0x0,0x8000 do
		filepath = asset:GetAssetFilePath(i)
		if filepath ~= "" then print(filepath) end
	end
end

RegisterEvent("IsInGame", function()
	isingame = true
end)

RegisterEvent("OnAsteroidDestroyed", function(count)
	if not isingame then return end
	print("New asteroid destroyed : " .. count)
end)

RegisterEvent("OnEnemieKilled", function(count)
	if not isingame then return end
	print("wow congrats you killed " .. count .. " enemies!!!")
end)

RegisterEvent("OnCargoChanged", function(count)
	if not isingame then return end
	print("Current cargo : " .. count)
end)

RegisterEvent("OnStationChanged", function(id)
	if not isingame then return end
	if station:IsVoid() then
		print("wow you joined the voids!!!")
		return
	end
	print("New station ! : " .. id)
end)

RegisterEvent("OnStationDocked", function()
	print("Docked on this station: " .. station.name)
	--local randomship = math.random(1,29)
	--print("Setting the first ship of the angar to : " .. randomship)
	--station:SetAngarShipId(0, randomship)
end)

RegisterEvent("IsInMainMenu", function()
	if assetchanged then return end
	--get_every_assets_filepath()
	print("Asset changed!")
	asset:SetAssetFilePath(0x2008, "mods/hello_mod/my_assets/custom_gof2_interface.aei") -- feel free to custom the gof2 interface with any tools (I don't know if we have any) also you can call this setassetfilepath function while the game is running BUT it won't be edited instantly, for the asset to be edited you need to 'reload' the game aka going to a station, changing system etc..
	assetchanged = true
end)

RegisterEvent("OnMoneyChanged", function(money)
	if not isingame then return end
	print(money)
end)

RegisterEvent("OnSystemChanged", function(id)
	if not isingame then return end
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
	print("Station angar items count : " .. station.angaritemscount)
	print("Station angar ships count : " .. station.angarshipscount)
	print("Player money : " .. player.money)
	print("Player Ship cargo : " .. player.cargo)
	print("Player Ship maxcargo : " .. player.maxcargo)
	print("Player Ship armor : " .. player.armor)
	print("Player Ship maxhealth : " .. player.maxhealth)
	print("Player Enemies killed : " .. player.enemieskilled)
	print("Player level : " .. player.level)
	print("Player visisted stations : " .. player.visitedstations)
	print("Player Completed side missions : " .. mission.completedsidemissions)
	print("Player Cargo salvaged count : " .. player.cargosalvagedcount)
	print("Player Asteroids destroyed count : " .. player.asteroidsdestroyedcount)
	if player:HasShipArmor() then
		print("The ship has an armor")
	end
	if player:IsDocked() then
		print("player is docked in a station")
	end
	if not system:IsVisible(26) then
		print("Shima system is not visible")
	end
	print("GOF2 Interface AEI : " .. asset:GetAssetFilePath(0x2008)) -- 0x2008 is the offset of the interface
end)

RegisterEvent("OnUpdate", function()
	-- every ticks
end)