StartingTown_PlayerHouse2_MapScripts::
# 2 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	map_script MAP_SCRIPT_ON_FRAME_TABLE, StartingTown_PlayerHouse2_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE
	.byte 0

StartingTown_PlayerHouse2_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE:
# 3 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	map_script_2 VAR_LITTLEROOT_INTRO_STATE, 0, StartingTown_PlayerHouse2_WakeUp
	.2byte 0


StartingTown_PlayerHouse2_WakeUp::
# 8 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	lock
# 9 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	addvar VAR_LITTLEROOT_INTRO_STATE, 1
# 10 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	msgbox StartingTown_PlayerHouse2_WakeUp_Text_0
# 11 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	release
	end


StartingTown_PlayerHouse2_Radio::
# 16 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	lock
# 17 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	fadenewbgm MUS_GAME_CORNER
# 18 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	msgbox StartingTown_PlayerHouse2_Radio_Text_0
# 22 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	fadedefaultbgm
# 23 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	release
	end


StartingTown_PlayerHouse2_Bed::
# 28 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	lock
# 29 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	message StartingTown_PlayerHouse2_Bed_Text_0
# 31 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	waitmessage
# 32 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	multichoice 19, 6, MULTI_MORNING_NIGHT, TRUE
# 33 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	switch VAR_RESULT
# 34 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	case 0, StartingTown_PlayerHouse2_Bed_3
# 38 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	case 1, StartingTown_PlayerHouse2_Bed_4
# 42 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	case 2, StartingTown_PlayerHouse2_Bed_5
StartingTown_PlayerHouse2_Bed_1:
# 45 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	release
	end

StartingTown_PlayerHouse2_Bed_3:
# 35 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	callnative FakeRtc_SkipToMorning
# 36 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	call Common_EventScript_OutOfCenterPartyHeal
# 37 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	msgbox StartingTown_PlayerHouse2_Bed_Text_1
	goto StartingTown_PlayerHouse2_Bed_1

StartingTown_PlayerHouse2_Bed_4:
# 39 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	callnative FakeRtc_SkipToNight
# 40 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	call Common_EventScript_OutOfCenterPartyHeal
# 41 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	msgbox StartingTown_PlayerHouse2_Bed_Text_1
	goto StartingTown_PlayerHouse2_Bed_1

StartingTown_PlayerHouse2_Bed_5:
# 43 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	msgbox StartingTown_PlayerHouse2_Bed_Text_2
	goto StartingTown_PlayerHouse2_Bed_1


StartingTown_PlayerHouse2_Console::
# 50 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	lock
# 51 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	msgbox StartingTown_PlayerHouse2_Console_Text_0, MSGBOX_YESNO
# 52 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	switch VAR_RESULT
# 53 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	case YES, StartingTown_PlayerHouse2_Console_3
# 76 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	case NO, StartingTown_PlayerHouse2_Console_4
StartingTown_PlayerHouse2_Console_1:
# 79 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	release
	end

StartingTown_PlayerHouse2_Console_3:
# 54 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	closemessage
# 55 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	applymovement LOCALID_PLAYER, StartingTown_PlayerHouse2_Console_Movement_0
# 60 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	waitmovement 0
# 61 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	setmetatile 4, 3, 0x5D, TRUE
# 62 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	setmetatile 4, 4, 0x65, TRUE
# 63 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	special DrawWholeMapView
# 64 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	playbgm MUS_MEGALOVANIA, FALSE
# 65 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	waitbuttonpress
# 66 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	setmetatile 4, 3, 0x4D, TRUE
# 67 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	setmetatile 4, 4, 0x55, TRUE
# 68 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	special DrawWholeMapView
# 69 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	playbgm MUS_RG_PALLET, FALSE
# 70 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	applymovement LOCALID_PLAYER, StartingTown_PlayerHouse2_Console_Movement_1
# 74 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	waitmovement 0
# 75 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	msgbox StartingTown_PlayerHouse2_Console_Text_1
	goto StartingTown_PlayerHouse2_Console_1

StartingTown_PlayerHouse2_Console_4:
# 77 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	msgbox StartingTown_PlayerHouse2_Console_Text_2
	goto StartingTown_PlayerHouse2_Console_1


# 55 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
StartingTown_PlayerHouse2_Console_Movement_0:
# 56 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	walk_down
# 57 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	walk_left
# 58 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	face_up
	step_end

# 70 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
StartingTown_PlayerHouse2_Console_Movement_1:
# 71 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	walk_right
# 72 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	walk_up
	step_end

StartingTown_PlayerHouse2_WakeUp_Text_0:
# 10 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	.string "Today's your first day!$"

StartingTown_PlayerHouse2_Radio_Text_0:
# 18 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	.string "It seems like there's a raffle\n"
	.string "going on…"
	.string "You'll have to enter\n"
	.string "another time!$"

StartingTown_PlayerHouse2_Bed_Text_0:
# 29 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	.string "You could use some rest…\n"
	.string "Until when will you sleep?$"

StartingTown_PlayerHouse2_Bed_Text_1:
# 37 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	.string "Much better!$"

StartingTown_PlayerHouse2_Bed_Text_2:
# 43 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	.string "You'll rest later…$"

StartingTown_PlayerHouse2_Console_Text_0:
# 51 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	.string "Play a game?$"

StartingTown_PlayerHouse2_Console_Text_1:
# 75 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	.string "That was tough!$"

StartingTown_PlayerHouse2_Console_Text_2:
# 77 "data/maps/StartingTown_PlayerHouse2/scripts.pory"
	.string "Another time…$"
