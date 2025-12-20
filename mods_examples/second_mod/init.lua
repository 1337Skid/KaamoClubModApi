printonetime = false
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
	--for i = 0, 23 do
	--	system:Create(randomstring(10), i+50, 30, 47)
	--end
end)

RegisterEvent("IsInGame", function()
	if printonetime then return end
	print("hi second script")
	printonetime = true
	--system:Create("icantcreateezfzehufhzefhuez", 328,32,28)
end)