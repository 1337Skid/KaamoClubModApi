printonetime = false
myitem2 = 0
function randomstring(length)
    local charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
    local result = ""
    for i = 1, length do
        local randIndex = math.random(1, #charset)
        result = result .. charset:sub(randIndex, randIndex)
    end
    return result
end

math.randomseed(os.time())

RegisterEvent("EarlyInit", function()
	-- // Example of how to create a system with stations in
	local systemid = system:Create("cool system", 50, 30, 47, 3, 3, 9, {26,0}) -- name, x, y, z, faction, risk, textureid (star), linkedsystemids (jumpgate which system? 26 is shima) -1 = no jumpgate
	local stationid = station:Create("test 1", 10, 9, systemid) -- name, techlevel, textureid (planet), systemid
	local stationid2 = station:Create("test 2", 1, 3, systemid)
	local stationid3 = station:Create("test 3", 2, 2, systemid)
	local stationid4 = station:Create("test 4", 3, 1, systemid)
	local mytextureid = asset:CreateTexture("mods/second_mod/item.aei")
	local mysprite = asset:CreateSprite(mytextureid, 0)
	print("MYSPRITE ID: " .. mysprite)
	testitem = {
	type = "Laser",
	techlevel = 9,
	baseprice = 1234,
	minprice = 12,
	maxprice = 1234,
	damage = 120,
	loadingspeed = 1300,
	range = 1000,
	speed = 3000
	}
	local myitem = item:Create("my test item", "cool description", testitem)
	print("my test item id: " .. myitem)
	testitem2 = {
	type = "Laser",
	techlevel = 9,
	baseprice = 1234,
	minprice = 12,
	maxprice = 1234,
	damage = 120,
	loadingspeed = 1300,
	range = 1000,
	speed = 3000,
	spriteicon = mysprite,
	blueprint_ingredient_ids = {8, 2, 5, 4, 9, 111},
	blueprint_ingredient_counts = {1234, 1234, 3, 4, 1, 2}
	}
	myitem2 = item:Create("A nice blueprint", "craft me if you can", testitem2)
	--print("Made station id : " .. stationid)
	--print("Made station id : " .. stationid2)
	--print("Made station id : " .. stationid3)
	--print("Made station id : " .. stationid4)
	--print("My custom system id : " .. systemid)
	--for i = 0, 23 do
	--	system:Create(randomstring(10), i+50, 30, 47)
	--end
end)

RegisterEvent("IsInGame", function()
	if printonetime then return end
	print("hi second script")
	item:UnlockBlueprint(myitem2)
	printonetime = true
	--system:Create("icantcreateezfzehufhzefhuez", 328,32,28,3, 3, 11)
	--station:Create("cant create outside of earlyinit", 10, 4, 11111111)
end)