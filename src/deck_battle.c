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
static void Task_PlayerSelectAllyToSwap(u8 taskId);
static void Task_PlayerSelectSingleOpponent(u8 taskId);
static void Task_PrepareForActionPhase(u8 taskId);
static void Task_ExecuteQueuedActionOrEnd(u8 taskId);
static void Task_ExecuteMove(u8 taskId);
static void Task_ExecuteSwap(u8 taskId);
static void InitBattleStructData(void);
static void InitBattleMonData(void);
static void QueueAction(u32 type, u32 battlerAtk, u32 battlerDef, u32 move);

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

#define tState  data[0]
#define tTimer  data[1]

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
    enum BattleId battler;
    enum BattlePosition pos;
    if ((gMain.newKeys & DPAD_LEFT) // Check for a battler to move to the left.
        && (pos = GetToMoveOnLeft(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition)) != POSITIONS_COUNT)
    {
        PlaySE(SE_SELECT);
        battler = GetDeckBattlerAtPosition(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition);
        RemoveSelectionCursorOverBattler(battler);
        StartBattlerAnim(battler, ANIM_PAUSED);
        gDeckBattleStruct.selectedPosition = pos;

        battler = GetDeckBattlerAtPosition(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition);
        CreateSelectionCursorOverBattler(battler);
        LoadBattlerPortrait(battler);
        PrintBattlerMoveInfo(battler);
        PrintBattlerStats(battler);
        StartBattlerAnim(battler, ANIM_IDLE);
    }
    if ((gMain.newKeys & DPAD_RIGHT) // Check for a battler to move to the right.
        && (pos = GetToMoveOnRight(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition)) != POSITIONS_COUNT)
    {
        PlaySE(SE_SELECT);
        battler = GetDeckBattlerAtPosition(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition);
        RemoveSelectionCursorOverBattler(battler);
        StartBattlerAnim(battler, ANIM_PAUSED);
        gDeckBattleStruct.selectedPosition = pos;

        battler = GetDeckBattlerAtPosition(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition);
        CreateSelectionCursorOverBattler(battler);
        LoadBattlerPortrait(battler);
        PrintBattlerMoveInfo(battler);
        PrintBattlerStats(battler);
        StartBattlerAnim(battler, ANIM_IDLE);
    }
    if (gMain.newKeys & A_BUTTON) // Choose target to attack.
    {
        PlaySE(SE_SELECT);
        gBattlerAttacker = GetDeckBattlerAtPosition(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition);
        RemoveSelectionCursorOverBattler(gBattlerAttacker);
        SetBattlerGrayscale(gBattlerAttacker, TRUE);

        gDeckBattleStruct.selectedPosition = GetLeftmostOccupiedPosition(B_SIDE_OPPONENT);
        battler = GetDeckBattlerAtPosition(B_SIDE_OPPONENT, gDeckBattleStruct.selectedPosition);
        CreateSelectionCursorOverBattler(battler);
        PrintTargetBattlerPrompt(battler);
        PrintBattlerStats(battler);
        StartBattlerAnim(battler, ANIM_IDLE);

        SetBattlerPortraitVisibility(FALSE);
        SetGpuReg(REG_OFFSET_BG0VOFS, DISPLAY_HEIGHT);
        SetGpuReg(REG_OFFSET_BG1VOFS, DISPLAY_HEIGHT);
        gTasks[taskId].func = Task_PlayerSelectSingleOpponent;
    }
    if (gMain.newKeys & START_BUTTON) // Choose target to swap.
    {
        gBattlerAttacker = GetDeckBattlerAtPosition(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition);
        if (gDeckBattleMons[gBattlerAttacker].hasSwapped)
        {
            PlaySE(SE_FAILURE);
        }
        else
        {
            RemoveSelectionCursorOverBattler(gBattlerAttacker);
            GetBattlerSprite(gBattlerAttacker)->oam.objMode = ST_OAM_OBJ_BLEND;

            PlaySE(SE_SELECT);
            pos = gDeckBattleStruct.selectedPosition;
            gDeckBattleStruct.selectedPosition = GetLeftmostOccupiedPosition(B_SIDE_PLAYER);
            if (gDeckBattleStruct.selectedPosition == pos) // make sure it's not the attacker
                gDeckBattleStruct.selectedPosition = GetNonAttackerOnRight(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition);
            battler = GetDeckBattlerAtPosition(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition);
            CreateSelectionCursorOverBattler(battler);
            PrintSwapTargetPrompt(battler);
            PrintBattlerStats(battler);
            StartBattlerAnim(battler, ANIM_IDLE);

            SetBattlerPortraitVisibility(FALSE);
            SetGpuReg(REG_OFFSET_BG0VOFS, DISPLAY_HEIGHT);
            SetGpuReg(REG_OFFSET_BG1VOFS, DISPLAY_HEIGHT);
            gTasks[taskId].func = Task_PlayerSelectAllyToSwap;
        }
    }
    if (gMain.newKeys & B_BUTTON)
    {
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
        gTasks[taskId].func = Task_CloseDeckBattle;
        PlaySE(SE_SELECT);
    }
}

static void Task_PlayerSelectAllyToSwap(u8 taskId)
{
    enum BattleId battler;
    enum BattlePosition pos;
    if ((gMain.newKeys & DPAD_LEFT)
        && (pos = GetNonAttackerOnLeft(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition)) != POSITIONS_COUNT)
    {
        PlaySE(SE_SELECT);
        battler = GetDeckBattlerAtPosition(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition);
        RemoveSelectionCursorOverBattler(battler);
        StartBattlerAnim(battler, ANIM_PAUSED);
        gDeckBattleStruct.selectedPosition = pos;

        battler = GetDeckBattlerAtPosition(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition);
        CreateSelectionCursorOverBattler(battler);
        PrintSwapTargetPrompt(battler);
        StartBattlerAnim(battler, ANIM_IDLE);
    }
    if ((gMain.newKeys & DPAD_RIGHT)
        && (pos = GetNonAttackerOnRight(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition)) != POSITIONS_COUNT)
    {
        PlaySE(SE_SELECT);
        battler = GetDeckBattlerAtPosition(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition);
        RemoveSelectionCursorOverBattler(battler);
        StartBattlerAnim(battler, ANIM_PAUSED);
        gDeckBattleStruct.selectedPosition = pos;

        battler = GetDeckBattlerAtPosition(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition);
        CreateSelectionCursorOverBattler(battler);
        PrintSwapTargetPrompt(battler);
        StartBattlerAnim(battler, ANIM_IDLE);
    }
    if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        battler = GetDeckBattlerAtPosition(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition);
        RemoveSelectionCursorOverBattler(battler);
        StartBattlerAnim(battler, ANIM_PAUSED);

        gDeckBattleStruct.selectedPosition = gDeckBattleMons[gBattlerAttacker].position;
        battler = GetDeckBattlerAtPosition(B_SIDE_PLAYER, gBattlerAttacker);
        LoadBattlerPortrait(gDeckBattleStruct.selectedPosition);
        PrintBattlerMoveInfo(gDeckBattleStruct.selectedPosition);
        PrintBattlerStats(gDeckBattleStruct.selectedPosition);
        CreateSelectionCursorOverBattler(battler);
        GetBattlerSprite(gBattlerAttacker)->oam.objMode = ST_OAM_OBJ_NORMAL;        

        SetBattlerPortraitVisibility(TRUE);
        SetGpuReg(REG_OFFSET_BG0VOFS, 0);
        SetGpuReg(REG_OFFSET_BG1VOFS, 0);
        gTasks[taskId].func = Task_PlayerSelectAction;
    }
    if ((gMain.newKeys & A_BUTTON) || (gMain.newKeys & START_BUTTON))
    {
        gBattlerTarget = GetDeckBattlerAtPosition(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition);
        if (gDeckBattleMons[gBattlerTarget].hasSwapped)
        {
            PlaySE(SE_FAILURE);
        }
        else
        {
            PlaySE(SE_SELECT);
            RemoveSelectionCursorOverBattler(gBattlerTarget);
            GetBattlerSprite(gBattlerTarget)->oam.objMode = ST_OAM_OBJ_BLEND;

            QueueAction(ACTION_SWAP, gBattlerAttacker, gBattlerTarget, MOVE_NONE);
            u32 temp = gDeckBattleMons[gBattlerAttacker].position;
            gDeckBattleMons[gBattlerAttacker].position = gDeckBattleMons[gBattlerTarget].position;
            gDeckBattleMons[gBattlerTarget].position = temp;
            GetBattlerSprite(gBattlerAttacker)->x = GetBattlerXCoord(gBattlerAttacker);
            GetBattlerSprite(gBattlerTarget)->x = GetBattlerXCoord(gBattlerTarget);
            gDeckBattleMons[gBattlerAttacker].hasSwapped = TRUE;
            gDeckBattleMons[gBattlerTarget].hasSwapped = TRUE;
            StartBattlerAnim(gBattlerAttacker, ANIM_PAUSED);
            StartBattlerAnim(gBattlerTarget, ANIM_PAUSED);

            gDeckBattleStruct.selectedPosition = GetLeftmostPositionToMove(B_SIDE_PLAYER);
            battler = GetDeckBattlerAtPosition(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition);
            LoadBattlerPortrait(battler);
            PrintBattlerMoveInfo(battler);
            PrintBattlerStats(battler);
            CreateSelectionCursorOverBattler(battler);
            StartBattlerAnim(battler, ANIM_IDLE);

            SetBattlerPortraitVisibility(TRUE);
            SetGpuReg(REG_OFFSET_BG0VOFS, 0);
            SetGpuReg(REG_OFFSET_BG1VOFS, 0);
            gTasks[taskId].func = Task_PlayerSelectAction;
        }
    }
}

static void Task_PlayerSelectSingleOpponent(u8 taskId)
{
    enum BattleId battler;
    enum BattlePosition pos;
    if ((gMain.newKeys & DPAD_LEFT)
        && (pos = GetOccupiedOnLeft(B_SIDE_OPPONENT, gDeckBattleStruct.selectedPosition)) != POSITIONS_COUNT)
    {
        PlaySE(SE_SELECT);
        battler = GetDeckBattlerAtPosition(B_SIDE_OPPONENT, gDeckBattleStruct.selectedPosition);
        RemoveSelectionCursorOverBattler(battler);
        StartBattlerAnim(battler, ANIM_PAUSED);
        gDeckBattleStruct.selectedPosition = pos;

        battler = GetDeckBattlerAtPosition(B_SIDE_OPPONENT, gDeckBattleStruct.selectedPosition);
        CreateSelectionCursorOverBattler(battler);
        PrintTargetBattlerPrompt(battler);
        StartBattlerAnim(battler, ANIM_IDLE);
    }
    if ((gMain.newKeys & DPAD_RIGHT)
        && (pos = GetOccupiedOnRight(B_SIDE_OPPONENT, gDeckBattleStruct.selectedPosition)) != POSITIONS_COUNT)
    {
        PlaySE(SE_SELECT);
        battler = GetDeckBattlerAtPosition(B_SIDE_OPPONENT, gDeckBattleStruct.selectedPosition);
        RemoveSelectionCursorOverBattler(battler);
        StartBattlerAnim(battler, ANIM_PAUSED);
        gDeckBattleStruct.selectedPosition = pos;

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
        LoadBattlerPortrait(gDeckBattleStruct.selectedPosition);
        PrintBattlerMoveInfo(gDeckBattleStruct.selectedPosition);
        PrintBattlerStats(gDeckBattleStruct.selectedPosition);
        CreateSelectionCursorOverBattler(gBattlerAttacker);
        SetBattlerGrayscale(gBattlerAttacker, FALSE);

        SetBattlerBobPause(FALSE);
        SetBattlerPortraitVisibility(TRUE);
        SetGpuReg(REG_OFFSET_BG0VOFS, 0);
        SetGpuReg(REG_OFFSET_BG1VOFS, 0);
        gTasks[taskId].func = Task_PlayerSelectAction;
    }
    if (gMain.newKeys & A_BUTTON)
    {
        PlaySE(SE_SELECT);
        gBattlerTarget = GetDeckBattlerAtPosition(B_SIDE_OPPONENT, gDeckBattleStruct.selectedPosition);
        RemoveSelectionCursorOverBattler(gBattlerTarget);
        StartBattlerAnim(gBattlerTarget, ANIM_PAUSED);

        QueueAction(ACTION_ATTACK, gBattlerAttacker, gBattlerTarget, MOVE_NONE);
        u32 side = GetDeckBattlerSide(gBattlerAttacker);
        SetBattlerGrayscale(gBattlerAttacker, TRUE);
        gDeckBattleMons[gBattlerAttacker].hasMoved = TRUE;
        StartBattlerAnim(gBattlerAttacker, ANIM_PAUSED);

        gDeckBattleStruct.selectedPosition = GetLeftmostPositionToMove(side);
        if (gDeckBattleStruct.selectedPosition != POSITIONS_COUNT)
        {
            battler = GetDeckBattlerAtPosition(side, gDeckBattleStruct.selectedPosition);
            LoadBattlerPortrait(battler);
            PrintBattlerMoveInfo(battler);
            PrintBattlerStats(battler);
            CreateSelectionCursorOverBattler(battler);
            StartBattlerAnim(battler, ANIM_IDLE);

            SetBattlerPortraitVisibility(TRUE);
            SetGpuReg(REG_OFFSET_BG0VOFS, 0);
            SetGpuReg(REG_OFFSET_BG1VOFS, 0);

            gTasks[taskId].func = Task_PlayerSelectAction;
        }
        else
        {
            gTasks[taskId].func = Task_PrepareForActionPhase; 
        }
    }
}

static void Task_PrepareForActionPhase(u8 taskId)
{
    struct Sprite *sprite;
    switch (gTasks[taskId].tState)
    {
    case 0:
        for (enum BattleId battler = 0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
        {
            gDeckBattleMons[battler].position = gDeckBattleStruct.initialPositions[battler];
            SetBattlerGrayscale(battler, FALSE);
            sprite = GetBattlerSprite(battler);
            sprite->oam.objMode = ST_OAM_OBJ_NORMAL;
            sprite->x = GetBattlerXCoord(battler);
        }
        ++gTasks[taskId].tState;
        break;
    case 1:
        gTasks[taskId].tState = 0;
        gTasks[taskId].func = Task_ExecuteQueuedActionOrEnd;
        break;
    }
}

static void Task_ExecuteQueuedActionOrEnd(u8 taskId)
{
    if (gDeckBattleStruct.executedCount < gDeckBattleStruct.actionsCount)
    {
        gBattlerAttacker = gDeckBattleStruct.queuedActions[gDeckBattleStruct.executedCount].attacker;
        gBattlerTarget = gDeckBattleStruct.queuedActions[gDeckBattleStruct.executedCount].target;

        gTasks[taskId].tTimer = 0;
        gTasks[taskId].tState = 0;
        if (gDeckBattleStruct.queuedActions[gDeckBattleStruct.executedCount].type == ACTION_ATTACK)
            gTasks[taskId].func = Task_ExecuteMove;
        else
            gTasks[taskId].func = Task_ExecuteSwap;
        ++gDeckBattleStruct.executedCount;
    }
    else
    {
        SetBattlerBobPause(FALSE);
        SetBattlerPortraitVisibility(TRUE);
        SetGpuReg(REG_OFFSET_BG0VOFS, 0);
        SetGpuReg(REG_OFFSET_BG1VOFS, 0);

        for (u32 i = 0; i < MAX_DECK_BATTLERS_COUNT; ++i)
        {
            gDeckBattleMons[i].hasMoved = FALSE;
            gDeckBattleMons[i].hasSwapped = FALSE;
        }
        gDeckBattleStruct.actionsCount = 0;
        gDeckBattleStruct.executedCount = 0;
        
        gDeckBattleStruct.selectedPosition = GetLeftmostPositionToMove(B_SIDE_PLAYER);
        u32 battler = GetDeckBattlerAtPosition(B_SIDE_PLAYER, gDeckBattleStruct.selectedPosition);
        LoadBattlerPortrait(battler);
        PrintBattlerMoveInfo(battler);
        PrintBattlerStats(battler);
        CreateSelectionCursorOverBattler(battler);
        StartBattlerAnim(battler, ANIM_IDLE);

        gTasks[taskId].func = Task_PlayerSelectAction;
    }
}

static void Task_ExecuteMove(u8 taskId)
{
    switch (gTasks[taskId].tState)
    {
    case 0: // Do attack animation.
        SetBattlerGrayscale(gBattlerAttacker, FALSE);
        StartBattlerAnim(gBattlerAttacker, ANIM_ATTACK);
        PrintMoveUseString();
        ++gTasks[taskId].tState;
        break;
    case 1: // Wait for attack animation to execute cry.
        if (GetBattlerSprite(gBattlerAttacker)->animCmdIndex == 2) // right after cry
            ++gTasks[taskId].tState;
        break;
    case 2: // TODO: Do animation depending on move effect.
        StartBattlerAnim(gBattlerTarget, ANIM_HURT);
        PrintMoveOutcomeString();
        PlaySE(SE_EFFECTIVE);
        ++gTasks[taskId].tState;
        break;
    case 3: // Wait for hurt animation.
        if (++gTasks[taskId].tTimer >= 60)
        {
            GetBattlerSprite(gBattlerTarget)->invisible = FALSE;
            ++gTasks[taskId].tState;
        }
        else if (gTasks[taskId].tTimer < 32)
        {
            if (gTasks[taskId].tTimer % 4 < 2)
                GetBattlerSprite(gBattlerTarget)->invisible = TRUE;
            else
                GetBattlerSprite(gBattlerTarget)->invisible = FALSE;
        }
        break;
    case 4:
        gTasks[taskId].tTimer = 0;
        gTasks[taskId].tState = 0;
        gTasks[taskId].func = Task_ExecuteQueuedActionOrEnd;
        break;
    }
}

static void Task_ExecuteSwap(u8 taskId)
{
    switch (gTasks[taskId].tState)
    {
    case 0: // Do attack animation.
        StartBattlerAnim(gBattlerAttacker, ANIM_ATTACK);
        StartBattlerAnim(gBattlerTarget, ANIM_ATTACK);
        PrintSwapString(gBattlerAttacker, gBattlerTarget);
        ++gTasks[taskId].tState;
        break;
    case 1: // Wait for attack animation to execute cry.
        if (GetBattlerSprite(gBattlerAttacker)->animCmdIndex == 2) // right after cry
            ++gTasks[taskId].tState;
        break;
    case 2: // Swap battler positions.
        if (++gTasks[taskId].tTimer > 8)
        {
            u32 temp = gDeckBattleMons[gBattlerAttacker].position;
            gDeckBattleMons[gBattlerAttacker].position = gDeckBattleMons[gBattlerTarget].position;
            gDeckBattleMons[gBattlerTarget].position = temp;

            GetBattlerSprite(gBattlerAttacker)->x = GetBattlerXCoord(gBattlerAttacker);
            GetBattlerSprite(gBattlerTarget)->x = GetBattlerXCoord(gBattlerTarget);

            PlaySE(SE_M_DOUBLE_TEAM);
            gTasks[taskId].tTimer = 0;
            ++gTasks[taskId].tState;
        }
        break;
    case 3: // Wait for animation.
        if (++gTasks[taskId].tTimer > 32)
            ++gTasks[taskId].tState;
        break;
    case 4:
        gTasks[taskId].tTimer = 0;
        gTasks[taskId].tState = 0;
        gTasks[taskId].func = Task_ExecuteQueuedActionOrEnd;
        break;
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

#undef tState
#undef tTimer

static void InitBattleStructData(void)
{
    gDeckBattleStruct.selectedPosition = GetLeftmostOccupiedPosition(B_SIDE_PLAYER);
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
        gDeckBattleStruct.initialPositions[i] = i % 6;
    }
}

static void QueueAction(u32 type, u32 battlerAtk, u32 battlerDef, u32 move)
{
    gDeckBattleStruct.queuedActions[gDeckBattleStruct.actionsCount].type = type;
    gDeckBattleStruct.queuedActions[gDeckBattleStruct.actionsCount].attacker = battlerAtk;
    gDeckBattleStruct.queuedActions[gDeckBattleStruct.actionsCount].target = battlerDef;
    gDeckBattleStruct.queuedActions[gDeckBattleStruct.actionsCount].move = move;

    if (type == ACTION_SWAP)
    {
        gDeckBattleStruct.initialPositions[battlerAtk] = gDeckBattleMons[battlerAtk].position;
        gDeckBattleStruct.initialPositions[battlerDef] = gDeckBattleMons[battlerDef].position;
    }

    ++gDeckBattleStruct.actionsCount;
}
