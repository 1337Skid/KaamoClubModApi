printonetime = false

RegisterEvent("IsInGame", function()
	if printonetime then return end
	print("hi second script")
	printonetime = true
end)