local GLOBALS = require("globals")

RegisterEvent("EarlyInit", function()
	local systemid = system:Create("Plaion", 20, 30, 21, 2, 0, 7, 2)
	GLOBALS.mission2_stationid = station:Create("Gatekeeper", 8, 1, systemid)
	station:Create("Wa'hu", 2, 4, systemid)
	GLOBALS.mission1_id = mission:Create(GLOBALS.mission1_stationid, "A description", 0)
	GLOBALS.mission2_id = mission:Create(GLOBALS.mission2_stationid, "A description", 0) 
	GLOBALS.mission3_id = mission:Create(GLOBALS.mission3_stationid, "A description", 0) -- stationid, description, type (0 = campaign, 1 = freelance)
end)

RegisterEvent("OnStationDocked", function()
	if mission.id >= 84 then -- checking if we completed valkyrie so aka the last possible real campaign mission on PC version
		if GLOBALS.mission_counter == 0 then
			mission:Enable(GLOBALS.mission1_id) -- enable our first mission so marker is visible and the mission is located on the station id we specified in the create function
			GLOBALS.mission_counter = 1 -- look in globals.lua to see what it does
		elseif GLOBALS.mission_counter == 1 and station.id == GLOBALS.mission1_stationid then
			wait(1)
			level:CreateDialogueWindow(GLOBALS.dialogues1)
			GLOBALS.mission_counter = 2
			mission:Enable(GLOBALS.mission2_id)
		elseif GLOBALS.mission_counter == 3 and station.id == GLOBALS.mission3_stationid then
			wait(1)
			level:CreateDialogueWindow(GLOBALS.dialogues3)
			player.money = player.money + 500000 -- reward the player 500k credits
			GLOBALS.mission_counter = 999 -- finished the mission! let's put a big number
		end
	end
end)

RegisterEvent("OnStationChanged", function(id)
	if id == GLOBALS.mission1_stationid and GLOBALS.mission_counter == 1 then
		wait(2) -- so we have no entities on the level
		mission:Disable(GLOBALS.mission1_id) -- because we want to get in the station to start the dialogue
	elseif id == GLOBALS.mission2_stationid and GLOBALS.mission_counter == 2 then
		GLOBALS.killsneeded = player.enemieskilled + 10 -- because we have 10 enemies
		local points = {
    			{ pos = {0, 8000, 0}, duration = 5  },
    			{ pos = {-25000, 0, 0}, duration = 5 }
		} -- camera points
		wait(2)
		level:CreateCutScene(points)
		mission:Disable(GLOBALS.mission2_id) -- yes the game is so stupid you need to disable mission to put a radiomessage (soon with the modding api it will be auto so no need to do call this anymore)
		level:CreateRadioMessage("Keith T. Maxwell", "Wow there are a tons of pirates i'll crush them all", GLOBALS.maxwellimage)
		wait(10)
		level:CreateRadioMessage("Pirate", "Hey guys look at this idiot approaching us! let's kill him", GLOBALS.pirateimage)
		mission:Enable(GLOBALS.mission2_id)
	elseif id == GLOBALS.mission3_stationid and GLOBALS.mission_counter == 3 then
		wait(2)
		mission:Disable(GLOBALS.mission3_id)
	end
end)

RegisterEvent("OnEnemieKilled", function(count)
	if GLOBALS.mission_counter == 2 and count == GLOBALS.killsneeded then
		wait(1)
		mission:Disable(GLOBALS.mission2_id)
		GLOBALS.mission_counter = 3
		level:CreateRadioMessage("Keith T. Maxwell", "Mmh I wonder whats the code Jean Lacroix told me about, probably some outdated stuff...", GLOBALS.maxwellimage)
		wait(10)
		level:CreateDialogueWindow(GLOBALS.dialogues2)
		wait(1)
		mission:Enable(GLOBALS.mission3_id)
	end
end)

-- hooking the real in game function that is making ships on campaign missions
HookFunction("Level::createCampaignMission", function(ctx)
	if GLOBALS.mission_counter == 2 and station.id == GLOBALS.mission2_stationid then
		for i = 0, 9 do
			ctx:CreateFighter(19, 8)
		end
	end
	ctx:call() -- calling real function
end)