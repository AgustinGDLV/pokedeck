StartingTown_MapScripts::
	.byte 0


StartingTown_PlayerHouseSign::
# 6 "data/maps/StartingTown/scripts.pory"
	msgbox StartingTown_PlayerHouseSign_Text_0
# 7 "data/maps/StartingTown/scripts.pory"
	release
	end


StartingTown_Sign::
# 12 "data/maps/StartingTown/scripts.pory"
	msgbox StartingTown_Sign_Text_0
# 13 "data/maps/StartingTown/scripts.pory"
	release
	end


StartingTown_PlayerHouseSign_Text_0:
# 6 "data/maps/StartingTown/scripts.pory"
	.string "{PLAYER}'s HOUSE$"

StartingTown_Sign_Text_0:
# 12 "data/maps/StartingTown/scripts.pory"
	.string "STARTING TOWN$"
