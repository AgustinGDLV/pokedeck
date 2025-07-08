#include "global.h"
#include "battle.h"
#include "bg.h"
#include "deck_battle.h"
#include "deck_battle_interface.h"
#include "deck_battle_util.h"
#include "event_object_movement.h"
#include "gpu_regs.h"
#include "m4a.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "overworld.h"
#include "palette.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "text.h"
#include "util.h"
#include "constants/abilities.h"
#include "constants/battle.h"
#include "constants/moves.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/species.h"

// forward declarations
static void MainCB2_DeckBattle(void);
static void VBlankCB2_DeckBattle(void);
static void Task_OpenDeckBattle(u8 taskId);
static void Task_CloseDeckBattle(u8 taskId);
static void Task_PlayerSelectAction(u8 taskId);
static void Task_PlayerSelectTarget(u8 taskId);
static void InitBattleStructData(void);
static void InitBattleMonData(void);

// ewram data
EWRAM_DATA struct DeckBattleStruct gDeckBattleStruct = {0};
EWRAM_DATA struct DeckBattlePokemon gDeckBattleMons[MAX_DECK_BATTLERS_COUNT] = {0};

// code
#include "data/graphics/deck_pokemon.h"
#include "data/pokemon/deck_species_info.h"

static void MainCB2_DeckBattle(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VBlankCB2_DeckBattle(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void OpenDeckBattle(void)
{
    SetMainCallback2(CB2_OpenDeckBattleCustom);
}

void CB2_OpenDeckBattleCustom(void)
{
    switch (gMain.state) {
        default:
        case 0:
            UpdatePaletteFade();
            if (!gPaletteFade.active)
                gMain.state++;
            break;
        case 1:
            PlayBGM(MUS_NONE);
            SetVBlankCallback(NULL); 
            ClearVramOamPlttRegs();
            SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
            ClearTasksAndGraphicalStructs();
            ClearDeckBattleGraphicsStruct();
            gMain.state++;
            break;
        case 2:
            LoadBattleBoxesAndBackground();
            ShowBg(0);
            ShowBg(1);
            HideBg(2);
            ShowBg(3);
            gMain.state++;
            break;
        case 3:
            InitDeckBattleWindows();
            DeactivateAllTextPrinters();
            gMain.state++;
            break;
        case 4:
            InitDeckBattleGfx();
            gMain.state++;
        case 5:
            InitBattleStructData();
            InitBattleMonData();
            gMain.state++;
        case 6:
            for (enum BattleId battler = 0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
                LoadBattlerObjectSprite(battler);
            gMain.state++;
        case 7:
            SetVBlankCallback(VBlankCB2_DeckBattle);
            CreateTask(Task_OpenDeckBattle, 0);
            SetMainCallback2(MainCB2_DeckBattle);
            break;
    }
}

static void Task_OpenDeckBattle(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        LoadBattlerPortrait(B_PLAYER_0);
        PrintBattlerMoveInfo(B_PLAYER_0);
        PrintBattlerStats(B_PLAYER_0);
        CreateSelectionCursorOverBattler(GetDeckBattlerAtPosition(B_SIDE_PLAYER, POSITION_0));
        StartSpriteAnim(&gSprites[gDeckBattleGraphics.battlerSpriteIds[GetDeckBattlerAtPosition(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition)]], ANIM_IDLE);
        gTasks[taskId].func = Task_PlayerSelectAction;
    }
}

static void Task_PlayerSelectAction(u8 taskId)
{
    u32 battler;
    if ((gMain.newKeys & DPAD_LEFT) && gDeckBattleStruct.selectedPosition > POSITION_0) // TODO: select accounting for expended actions and missing battlers
    {
        PlaySE(SE_SELECT);
        battler = GetDeckBattlerAtPosition(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition);
        RemoveSelectionCursorOverBattler(battler);
        StartBattlerAnim(battler, ANIM_PAUSED);
        gDeckBattleStruct.selectedPosition -= 1;

        battler = GetDeckBattlerAtPosition(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition);
        CreateSelectionCursorOverBattler(battler);
        LoadBattlerPortrait(battler);
        PrintBattlerMoveInfo(battler);
        PrintBattlerStats(battler);
        StartBattlerAnim(battler, ANIM_IDLE);
    }
    if ((gMain.newKeys & DPAD_RIGHT) && gDeckBattleStruct.selectedPosition < POSITION_5)
    {
        PlaySE(SE_SELECT);
        battler = GetDeckBattlerAtPosition(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition);
        RemoveSelectionCursorOverBattler(battler);
        StartBattlerAnim(battler, ANIM_PAUSED);
        gDeckBattleStruct.selectedPosition += 1;

        battler = GetDeckBattlerAtPosition(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition);
        CreateSelectionCursorOverBattler(battler);
        LoadBattlerPortrait(battler);
        PrintBattlerMoveInfo(battler);
        PrintBattlerStats(battler);
        StartBattlerAnim(battler, ANIM_IDLE);
    }
    if (gMain.newKeys & A_BUTTON)
    {
        PlaySE(SE_SELECT);
        gBattlerAttacker = GetDeckBattlerAtPosition(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition);
        RemoveSelectionCursorOverBattler(gBattlerAttacker);
        SetBattlerGrayscale(gBattlerAttacker, TRUE);

        gDeckBattleStruct.selectedPosition = POSITION_0;
        battler = GetDeckBattlerAtPosition(B_SIDE_OPPONENT, gDeckBattleStruct.selectedPosition);
        CreateSelectionCursorOverBattler(battler);
        PrintTargetBattlerPrompt(battler);
        PrintBattlerStats(battler);
        StartBattlerAnim(battler, ANIM_IDLE);

        SetBattlerPortraitVisibility(FALSE);
        SetGpuReg(REG_OFFSET_BG0VOFS, DISPLAY_HEIGHT);
        SetGpuReg(REG_OFFSET_BG1VOFS, DISPLAY_HEIGHT);
        gTasks[taskId].func = Task_PlayerSelectTarget;
        SetBattlerBobPause(TRUE);
    }
    if (gMain.newKeys & B_BUTTON)
    {
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
        gTasks[taskId].func = Task_CloseDeckBattle;
        PlaySE(SE_SELECT);
    }
}

static void Task_PlayerSelectTarget(u8 taskId)
{
    u32 battler;
    if ((gMain.newKeys & DPAD_LEFT) && gDeckBattleStruct.selectedPosition > POSITION_0)
    {
        PlaySE(SE_SELECT);
        battler = GetDeckBattlerAtPosition(B_SIDE_OPPONENT, gDeckBattleStruct.selectedPosition);
        RemoveSelectionCursorOverBattler(battler);
        StartBattlerAnim(battler, ANIM_PAUSED);
        gDeckBattleStruct.selectedPosition -= 1;

        battler = GetDeckBattlerAtPosition(B_SIDE_OPPONENT, gDeckBattleStruct.selectedPosition);
        CreateSelectionCursorOverBattler(battler);
        PrintTargetBattlerPrompt(battler);
        StartBattlerAnim(battler, ANIM_IDLE);
    }
    if ((gMain.newKeys & DPAD_RIGHT) && gDeckBattleStruct.selectedPosition < POSITION_5)
    {
        PlaySE(SE_SELECT);
        battler = GetDeckBattlerAtPosition(B_SIDE_OPPONENT, gDeckBattleStruct.selectedPosition);
        RemoveSelectionCursorOverBattler(battler);
        StartBattlerAnim(battler, ANIM_PAUSED);
        gDeckBattleStruct.selectedPosition += 1;

        battler = GetDeckBattlerAtPosition(B_SIDE_OPPONENT, gDeckBattleStruct.selectedPosition);
        CreateSelectionCursorOverBattler(battler);
        PrintTargetBattlerPrompt(battler);
        StartBattlerAnim(battler, ANIM_IDLE);
    }
    if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        battler = GetDeckBattlerAtPosition(B_SIDE_OPPONENT, gDeckBattleStruct.selectedPosition);
        RemoveSelectionCursorOverBattler(battler);
        StartBattlerAnim(battler, ANIM_PAUSED);

        gDeckBattleStruct.selectedPosition = gDeckBattleMons[gBattlerAttacker].position;
        battler = GetDeckBattlerAtPosition(B_SIDE_PLAYER, gBattlerAttacker);
        LoadBattlerPortrait(gDeckBattleStruct.selectedPosition);
        PrintBattlerMoveInfo(gDeckBattleStruct.selectedPosition);
        PrintBattlerStats(gDeckBattleStruct.selectedPosition);
        CreateSelectionCursorOverBattler(battler);
        SetBattlerGrayscale(gBattlerAttacker, FALSE);

        SetBattlerPortraitVisibility(TRUE);
        SetGpuReg(REG_OFFSET_BG0VOFS, 0);
        SetGpuReg(REG_OFFSET_BG1VOFS, 0);
        gTasks[taskId].func = Task_PlayerSelectAction;
    }
}

static void Task_CloseDeckBattle(u8 taskId)
{
    // Change transparency color to black so fade out looks better.
    u16 palette = RGB_BLACK;
    LoadPalette(&palette, BG_PLTT_ID(0), PLTT_SIZEOF(1));

    // Reset data and destroy task.
    FreeAllWindowBuffers();
    ResetSpriteData();
    UnfreezeObjectEvents();
    DestroyTask(taskId);

    // Return to overworld.
    SetMainCallback2(CB2_ReturnToField);
}

static void InitBattleStructData(void)
{
    for (u32 i = 0; i < NUM_BATTLE_SIDES; ++i)
    {
        for (u32 j = 0; j < POSITIONS_COUNT; ++j)
        {
            gDeckBattleStruct.battlerAtPosition[i][j] = i*6 + j;
        }
    }
}

// Placeholder until party data is used.
static void InitBattleMonData(void)
{
    for (u32 i = 0; i < MAX_DECK_BATTLERS_COUNT; ++i)
    {
        if (i % 6 == 3)
            gDeckBattleMons[i].species = SPECIES_SLOWBRO;
        else
            gDeckBattleMons[i].species = SPECIES_SLOWPOKE;
        gDeckBattleMons[i].hp = gSpeciesDeckInfo[gDeckBattleMons[i].species].baseHP;
        gDeckBattleMons[i].maxHP = gSpeciesDeckInfo[gDeckBattleMons[i].species].baseHP;
        gDeckBattleMons[i].pwr = gSpeciesDeckInfo[gDeckBattleMons[i].species].basePWR;
        gDeckBattleMons[i].position = i % 6;
    }
}
