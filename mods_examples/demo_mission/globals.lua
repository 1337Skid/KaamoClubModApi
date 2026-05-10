-- // Our globals variables
local GLOBALS = {}

GLOBALS.mission1_stationid = 68
GLOBALS.mission2_stationid = 0
GLOBALS.mission3_stationid = 68
GLOBALS.mission1_id = 0
GLOBALS.mission2_id = 0
GLOBALS.mission3_id = 0
GLOBALS.mission_counter = 0 -- our own mission counter to see which mission the player is in right now (if you ever want to save this state, save it in a file with the lua os library)
GLOBALS.maxwellimage = {
	race = 0,
	hair = 0,
	eyes = 0,
	mouth = 0,
	armor = 0
}
GLOBALS.pirateimage = {
	race = 0,
	hair = 3,
	eyes = 4,
	mouth = 2,
	armor = 2
}
GLOBALS.dialogues1 = {
	{ name = "Keith T. Maxwell",  content = "Alright i'm okay for the task so if i'm correct you are dealing with pirates in the newly discovered Plaion system, right?", image = GLOBALS.maxwellimage, isplayer = 1 },
	{ name = "Jean Lacroix", content = "Yes the pirates are really annoying in this sytem and I have heard rumors about a 'code' they are hiding", image = {0, 1, 3, 0, 0}, isplayer = 0 },
	{ name = "Keith T. Maxwell",  content = "A code? what type of code?", image = GLOBALS.maxwellimage, isplayer = 1 },
	{ name = "Jean Lacroix", content = "I don't have a lot of infos about this code but you can try looking around once you are done with the pirates", image = {0, 1, 3, 0, 0}, isplayer = 0 },
	{ name = "Keith T. Maxwell",  content = "Okay i'll go now", image = GLOBALS.maxwellimage, isplayer = 1 },
}
GLOBALS.dialogues2 = {
	{ name = "Keith T. Maxwell",  content = "Computer, call Jean Lacroix", image = GLOBALS.maxwellimage, isplayer = 1 },
	{ name = "Jean Lacroix", content = "Hello Keith T. Maxwell you finished the job?", image = {0, 1, 3, 0, 0}, isplayer = 0 },
	{ name = "Keith T. Maxwell",  content = "Yes it was a piece of cake, by the way how much credits are you going to give me?", image = GLOBALS.maxwellimage, isplayer = 1 },
	{ name = "Jean Lacroix", content = "I'll pay a good 500.000 credits, come back to Sorox", image = {0, 1, 3, 0, 0}, isplayer = 0 },
	{ name = "Keith T. Maxwell",  content = "I'm on my way!", image = GLOBALS.maxwellimage, isplayer = 1 },
}
GLOBALS.dialogues3 = {
	{ name = "Jean Lacroix",  content = "Thank you very much for the clean up, did you find the code by the way?", image = {0, 1, 3, 0, 0}, isplayer = 0 },
	{ name = "Keith T. Maxwell", content = "Nope the code is probably lost", image = GLOBALS.maxwellimage, isplayer = 1 },
	{ name = "Jean Lacroix", content = "Alright, anyways you deserved your 500.000 credits", image = {0, 1, 3, 0, 0}, isplayer = 0 },
}
GLOBALS.killsneeded = 0

return GLOBALS