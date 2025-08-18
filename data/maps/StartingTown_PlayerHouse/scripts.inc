StartingTown_PlayerHouse_MapScripts::
# 2 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	map_script MAP_SCRIPT_ON_FRAME_TABLE, StartingTown_PlayerHouse_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE
	.byte 0

StartingTown_PlayerHouse_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE:
# 3 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	map_script_2 VAR_LITTLEROOT_INTRO_STATE, 1, StartingTown_PlayerHouse_FirstDay
	.2byte 0


StartingTown_PlayerHouse_FirstDay::
# 8 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	lock
# 9 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	addvar VAR_LITTLEROOT_INTRO_STATE, 1
# 10 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	applymovement LOCALID_PLAYER, StartingTown_PlayerHouse_FirstDay_Movement_0
# 14 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	waitmovement
# 15 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	playse SE_PIN
# 16 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	applymovement LOCALID_MOM, StartingTown_PlayerHouse_FirstDay_Movement_1
# 20 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	waitmovement
# 21 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	delay 40
# 22 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	message StartingTown_PlayerHouse_FirstDay_Text_0
# 23 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	waitmessage
# 24 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	delay 40
# 25 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	closemessage
# 26 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	applymovement LOCALID_PLAYER, StartingTown_PlayerHouse_FirstDay_Movement_2
# 33 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	waitmovement
# 34 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	msgbox StartingTown_PlayerHouse_FirstDay_Text_1
# 36 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	release
	end


# 10 "data/maps/StartingTown_PlayerHouse/scripts.pory"
StartingTown_PlayerHouse_FirstDay_Movement_0:
# 11 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	walk_down
# 12 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	walk_down
	step_end

# 16 "data/maps/StartingTown_PlayerHouse/scripts.pory"
StartingTown_PlayerHouse_FirstDay_Movement_1:
# 17 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	emote_exclamation_mark
# 18 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	jump_in_place_right
	step_end

# 26 "data/maps/StartingTown_PlayerHouse/scripts.pory"
StartingTown_PlayerHouse_FirstDay_Movement_2:
# 27 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	walk_down
# 28 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	walk_down
# 29 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	walk_down
# 30 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	walk_left
# 31 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	walk_left
	step_end

StartingTown_PlayerHouse_FirstDay_Text_0:
# 22 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	.string "You're awake!$"

StartingTown_PlayerHouse_FirstDay_Text_1:
# 34 "data/maps/StartingTown_PlayerHouse/scripts.pory"
	.string "I bet the professor is already\n"
	.string "waiting, you should hurry over!$"
