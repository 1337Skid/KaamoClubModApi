local KEY_INSERT = 0x2D
local credits = 0
local boolean = false

-- Arguments: title, togglekey, lockinput (1 = lock, 0 = don't lock)
RegisterWindow("My mod menu", function()
    imgui:Text("Money giver")
    imgui:Separator()
    if imgui:Button("Give 99999 Credits") then
        player.money = 99999
    end
    imgui:Spacing()
    credits = imgui:SliderInt("Money", credits, 0, 999999)
    if imgui:Button("Apply Money") then
        player.money = credits
    end
    imgui:Separator()
    boolean = imgui:Checkbox("test", boolean)
    imgui:ColoredText("press insert to toggle", 0.4, 1.0, 0.4, 1.0)
end, KEY_INSERT, 1) -- Set to 1 to lock the mouse and the keyboard, 0 to allow movement while menu is open

RegisterWindow("wow", function()
    imgui:Text("Asteroid destroyed!")
    
    -- Let the user close the popup themselves
    if imgui:Button("Close") then
        imgui:SetWindowOpen("wow", false) 
    end
end, 0x30, 0)

-- Hide the "wow" window
imgui:SetWindowOpen("wow", false)

RegisterEvent("OnAsteroidDestroyed", function(count)
    imgui:SetWindowOpen("wow", true)
end)