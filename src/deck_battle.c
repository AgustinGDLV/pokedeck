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
static void UpdateBattlerSelection(enum BattleId battler, bool32 selected);
static void DisplayActionSelectionInfo(enum BattleId battler);
static void DisplaySwapSelectionInfo(enum BattleId battler);
static void DisplayTargetSelectionInfo(enum BattleId battler);
static void QueueAction(u32 type, u32 battlerAtk, u32 battlerDef, u32 move);
static void SwapBattlerPositions(u32 battler1, u32 battler2);
static void ResetTurnValues(void);

// ewram data
EWRAM_DATA struct DeckBattleStruct gDeckStruct = {0};
EWRAM_DATA struct DeckBattlePokemon gDeckMons[MAX_DECK_BATTLERS_COUNT] = {0};

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
        CreateSelectionCursorOverBattler(GetDeckBattlerAtPos(B_SIDE_PLAYER, POSITION_0));
        StartSpriteAnim(&gSprites[gDeckGraphics.battlerSpriteIds[GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos)]], ANIM_IDLE);
        gTasks[taskId].func = Task_PlayerSelectAction;
    }
}

static void Task_PlayerSelectAction(u8 taskId)
{
    enum BattleId battler;
    enum BattlePosition pos;
    if ((gMain.newKeys & DPAD_LEFT) // Check for a battler to move to the left.
        && (pos = GetToMoveOnLeft(B_SIDE_PLAYER, gDeckStruct.selectedPos)) != POSITIONS_COUNT)
    {
        // Deselect battler.
        PlaySE(SE_SELECT);
        UpdateBattlerSelection(GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos), FALSE);

        // Select new battler.
        gDeckStruct.selectedPos = pos;
        battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        UpdateBattlerSelection(battler, TRUE);
        DisplayActionSelectionInfo(battler);
    }
    if ((gMain.newKeys & DPAD_RIGHT) // Check for a battler to move to the right.
        && (pos = GetToMoveOnRight(B_SIDE_PLAYER, gDeckStruct.selectedPos)) != POSITIONS_COUNT)
    {
        // Deselect battler.
        PlaySE(SE_SELECT);
        UpdateBattlerSelection(GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos), FALSE);

        // Select new battler.
        gDeckStruct.selectedPos = pos;
        battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        UpdateBattlerSelection(battler, TRUE);
        DisplayActionSelectionInfo(battler);
    }
    if (gMain.newKeys & A_BUTTON) // Choose target to attack.
    {
        // Remove cursor but keep idle anim while selecting target.
        PlaySE(SE_SELECT);
        gBattlerAttacker = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        RemoveSelectionCursorOverBattler(gBattlerAttacker);
        SetBattlerGrayscale(gBattlerAttacker, TRUE);

        // Display first possible target.
        gDeckStruct.selectedPos = GetLeftmostOccupiedPosition(B_SIDE_OPPONENT);
        battler = GetDeckBattlerAtPos(B_SIDE_OPPONENT, gDeckStruct.selectedPos);
        UpdateBattlerSelection(battler, TRUE);
        DisplayTargetSelectionInfo(battler);

        // Set up UI for targeting.
        SetBattlerPortraitVisibility(FALSE);
        SetGpuReg(REG_OFFSET_BG0VOFS, DISPLAY_HEIGHT);
        SetGpuReg(REG_OFFSET_BG1VOFS, DISPLAY_HEIGHT);
        gTasks[taskId].func = Task_PlayerSelectSingleOpponent;
    }
    if (gMain.newKeys & START_BUTTON) // Choose target to swap.
    {
        gBattlerAttacker = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        if (!gDeckMons[gBattlerAttacker].hasSwapped)
        {
            // Remove cursor but keep idle anim while selecting target.
            RemoveSelectionCursorOverBattler(gBattlerAttacker);
            GetBattlerSprite(gBattlerAttacker)->oam.objMode = ST_OAM_OBJ_BLEND;

            // Try to select first possible target.
            pos = gDeckStruct.selectedPos;
            gDeckStruct.selectedPos = GetLeftmostOccupiedPosition(B_SIDE_PLAYER);
            if (gDeckStruct.selectedPos == pos) // make sure it's not the attacker
                gDeckStruct.selectedPos = GetNonAttackerOnRight(B_SIDE_PLAYER, gDeckStruct.selectedPos);
            if (gDeckStruct.selectedPos != POSITIONS_COUNT)
            {
                // Mark target as selected.
                PlaySE(SE_SELECT);
                battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
                UpdateBattlerSelection(battler, TRUE);
                DisplaySwapSelectionInfo(battler);
                
                // Set up UI for targeting.
                SetBattlerPortraitVisibility(FALSE);
                SetGpuReg(REG_OFFSET_BG0VOFS, DISPLAY_HEIGHT);
                SetGpuReg(REG_OFFSET_BG1VOFS, DISPLAY_HEIGHT);
                gTasks[taskId].func = Task_PlayerSelectAllyToSwap;
                return;
            }
        }
        // Swap not possible.
        PlaySE(SE_FAILURE);
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
        && (pos = GetNonAttackerOnLeft(B_SIDE_PLAYER, gDeckStruct.selectedPos)) != POSITIONS_COUNT)
    {
        // Deselect battler.
        PlaySE(SE_SELECT);
        UpdateBattlerSelection(GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos), FALSE);

        // Select new battler.
        gDeckStruct.selectedPos = pos;
        battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        UpdateBattlerSelection(battler, TRUE);
        DisplaySwapSelectionInfo(battler);
    }
    if ((gMain.newKeys & DPAD_RIGHT)
        && (pos = GetNonAttackerOnRight(B_SIDE_PLAYER, gDeckStruct.selectedPos)) != POSITIONS_COUNT)
    {
        // Deselect battler.
        PlaySE(SE_SELECT);
        UpdateBattlerSelection(GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos), FALSE);

        // Select new battler.
        gDeckStruct.selectedPos = pos;
        battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        UpdateBattlerSelection(battler, TRUE);
        DisplaySwapSelectionInfo(battler);
    }
    if (gMain.newKeys & B_BUTTON)
    {
        // Deselect target battler.
        PlaySE(SE_SELECT);
        UpdateBattlerSelection(GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos), FALSE);

        // Reselect acting battler.
        UpdateBattlerSelection(gBattlerAttacker, TRUE);
        DisplayActionSelectionInfo(gBattlerAttacker);
        GetBattlerSprite(gBattlerAttacker)->oam.objMode = ST_OAM_OBJ_NORMAL;   
        gDeckStruct.selectedPos = gDeckMons[gBattlerAttacker].pos;     

        // Set up UI for action selection.
        SetBattlerPortraitVisibility(TRUE);
        SetGpuReg(REG_OFFSET_BG0VOFS, 0);
        SetGpuReg(REG_OFFSET_BG1VOFS, 0);
        gTasks[taskId].func = Task_PlayerSelectAction;
    }
    if ((gMain.newKeys & A_BUTTON) || (gMain.newKeys & START_BUTTON))
    {
        gBattlerTarget = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        if (!gDeckMons[gBattlerTarget].hasSwapped)
        {
            // Deselect battler and mark as swapped with transparency.
            PlaySE(SE_SELECT);
            UpdateBattlerSelection(gBattlerTarget, FALSE);
            GetBattlerSprite(gBattlerTarget)->oam.objMode = ST_OAM_OBJ_BLEND;

            // Queue swap action.
            QueueAction(ACTION_SWAP, gBattlerAttacker, gBattlerTarget, MOVE_NONE);
            SwapBattlerPositions(gBattlerAttacker, gBattlerTarget);
            StartBattlerAnim(gBattlerAttacker, ANIM_PAUSED);

            // Prepare to select next battler for action.
            gDeckStruct.selectedPos = GetLeftmostPositionToMove(B_SIDE_PLAYER);
            battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
            UpdateBattlerSelection(battler, TRUE);
            DisplayActionSelectionInfo(battler);

            // Set up UI for action selection.
            SetBattlerPortraitVisibility(TRUE);
            SetGpuReg(REG_OFFSET_BG0VOFS, 0);
            SetGpuReg(REG_OFFSET_BG1VOFS, 0);
            gTasks[taskId].func = Task_PlayerSelectAction;
        }
        else
        {
            PlaySE(SE_FAILURE);
        }
    }
}

static void Task_PlayerSelectSingleOpponent(u8 taskId)
{
    enum BattleId battler;
    enum BattlePosition pos;
    if ((gMain.newKeys & DPAD_LEFT)
        && (pos = GetOccupiedOnLeft(B_SIDE_OPPONENT, gDeckStruct.selectedPos)) != POSITIONS_COUNT)
    {
        // Deselect battler.
        PlaySE(SE_SELECT);
        UpdateBattlerSelection(GetDeckBattlerAtPos(B_SIDE_OPPONENT, gDeckStruct.selectedPos), FALSE);

        // Select new battler.
        gDeckStruct.selectedPos = pos;
        battler = GetDeckBattlerAtPos(B_SIDE_OPPONENT, gDeckStruct.selectedPos);
        UpdateBattlerSelection(battler, TRUE);
        DisplayTargetSelectionInfo(battler);
    }
    if ((gMain.newKeys & DPAD_RIGHT)
        && (pos = GetOccupiedOnRight(B_SIDE_OPPONENT, gDeckStruct.selectedPos)) != POSITIONS_COUNT)
    {
        // Deselect battler.
        PlaySE(SE_SELECT);
        UpdateBattlerSelection(GetDeckBattlerAtPos(B_SIDE_OPPONENT, gDeckStruct.selectedPos), FALSE);

        // Select new battler.
        gDeckStruct.selectedPos = pos;
        battler = GetDeckBattlerAtPos(B_SIDE_OPPONENT, gDeckStruct.selectedPos);
        UpdateBattlerSelection(battler, TRUE);
        DisplayTargetSelectionInfo(battler);
    }
    if (gMain.newKeys & B_BUTTON)
    {
        // Deselect target.
        PlaySE(SE_SELECT);
        UpdateBattlerSelection(GetDeckBattlerAtPos(B_SIDE_OPPONENT, gDeckStruct.selectedPos), FALSE);

        // Reselect acting battler.
        UpdateBattlerSelection(gBattlerAttacker, TRUE);
        DisplayActionSelectionInfo(gBattlerAttacker);
        SetBattlerGrayscale(gBattlerAttacker, FALSE);
        gDeckStruct.selectedPos = gDeckMons[gBattlerAttacker].pos;

        // Set up UI for action selection.
        SetBattlerPortraitVisibility(TRUE);
        SetGpuReg(REG_OFFSET_BG0VOFS, 0);
        SetGpuReg(REG_OFFSET_BG1VOFS, 0);
        gTasks[taskId].func = Task_PlayerSelectAction;
    }
    if (gMain.newKeys & A_BUTTON)
    {
        // Deselect target.
        PlaySE(SE_SELECT);
        gBattlerTarget = GetDeckBattlerAtPos(B_SIDE_OPPONENT, gDeckStruct.selectedPos);
        UpdateBattlerSelection(gBattlerTarget, FALSE);

        // Queue attack action and update data.
        QueueAction(ACTION_ATTACK, gBattlerAttacker, gBattlerTarget, MOVE_NONE);
        SetBattlerGrayscale(gBattlerAttacker, TRUE);
        gDeckMons[gBattlerAttacker].hasMoved = TRUE;
        StartBattlerAnim(gBattlerAttacker, ANIM_PAUSED);

        // Select next battler for action selection or begin action phase.
        gDeckStruct.selectedPos = GetLeftmostPositionToMove(B_SIDE_PLAYER);
        if (gDeckStruct.selectedPos != POSITIONS_COUNT)
        {
            battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
            UpdateBattlerSelection(battler, TRUE);
            DisplayActionSelectionInfo(battler);

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
        // Reset battler positions and remove grayscale and transparency.
        for (enum BattleId battler = 0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
        {
            gDeckMons[battler].pos = gDeckMons[battler].initialPos;
            SetBattlerGrayscale(battler, FALSE);
            sprite = GetBattlerSprite(battler);
            sprite->oam.objMode = ST_OAM_OBJ_NORMAL;
            sprite->x = GetBattlerXCoord(battler);
        }
        ++gTasks[taskId].tState;
        break;
    case 1:
        // Execute queued actions
        gTasks[taskId].tState = 0;
        gTasks[taskId].func = Task_ExecuteQueuedActionOrEnd;
        break;
    }
}

static void Task_ExecuteQueuedActionOrEnd(u8 taskId)
{
    // Set up data and execute queued action if any remain.
    if (gDeckStruct.executedCount < gDeckStruct.actionsCount)
    {
        gBattlerAttacker = gDeckStruct.queuedActions[gDeckStruct.executedCount].attacker;
        gBattlerTarget = gDeckStruct.queuedActions[gDeckStruct.executedCount].target;

        gTasks[taskId].tTimer = 0;
        gTasks[taskId].tState = 0;
        if (gDeckStruct.queuedActions[gDeckStruct.executedCount].type == ACTION_ATTACK)
            gTasks[taskId].func = Task_ExecuteMove;
        else
            gTasks[taskId].func = Task_ExecuteSwap;
        ++gDeckStruct.executedCount;
    }
    // Otherwise, return to action selection.
    else
    {
        ResetTurnValues();
        gDeckStruct.selectedPos = GetLeftmostPositionToMove(B_SIDE_PLAYER);
        u32 battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        UpdateBattlerSelection(battler, TRUE);
        DisplayActionSelectionInfo(battler);

        // Set up UI for action selection.
        SetBattlerBobPause(FALSE);
        SetBattlerPortraitVisibility(TRUE);
        SetGpuReg(REG_OFFSET_BG0VOFS, 0);
        SetGpuReg(REG_OFFSET_BG1VOFS, 0);
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
            PlaySE(SE_M_DOUBLE_TEAM);
            SwapBattlerPositions(gBattlerAttacker, gBattlerTarget);
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
    gDeckStruct.selectedPos = GetLeftmostOccupiedPosition(B_SIDE_PLAYER);
}

// Placeholder until party data is used.
static void InitBattleMonData(void)
{
    for (u32 i = 0; i < MAX_DECK_BATTLERS_COUNT; ++i)
    {
        switch (i % 6)
        {
        case 1:
            gDeckMons[i].species = SPECIES_BELLSPROUT;
            break;
        case 2:
            gDeckMons[i].species = SPECIES_MAREEP;
            break;
        case 3:
            gDeckMons[i].species = SPECIES_SLOWBRO;
            break;
        case 4:
            gDeckMons[i].species = SPECIES_SWABLU;
            break;
        default:
            gDeckMons[i].species = SPECIES_SLOWPOKE;
            break;
        }
        gDeckMons[i].hp = gDeckSpeciesInfo[gDeckMons[i].species].baseHP;
        gDeckMons[i].maxHP = gDeckSpeciesInfo[gDeckMons[i].species].baseHP;
        gDeckMons[i].pwr = gDeckSpeciesInfo[gDeckMons[i].species].basePWR;
        gDeckMons[i].pos = i % 6;
        gDeckMons[i].initialPos = i % 6;
    }
}

// Update graphics for selecting and deselecting a battler.
static void UpdateBattlerSelection(enum BattleId battler, bool32 selected)
{
    if (selected)
    {
        CreateSelectionCursorOverBattler(battler);
        StartBattlerAnim(battler, ANIM_IDLE);
    }
    else
    {
        RemoveSelectionCursorOverBattler(battler);
        StartBattlerAnim(battler, ANIM_PAUSED);
    }
}

// Update graphics for action selection.
static void DisplayActionSelectionInfo(enum BattleId battler)
{
    LoadBattlerPortrait(battler);
    PrintBattlerMoveInfo(battler);
    PrintBattlerStats(battler);
}

// Update graphics for swap selection.
static void DisplaySwapSelectionInfo(enum BattleId battler)
{
    PrintSwapTargetPrompt(battler);
    PrintBattlerStats(battler);
}

// Update graphics for target selection.
static void DisplayTargetSelectionInfo(enum BattleId battler)
{
    PrintTargetBattlerPrompt(battler);
    PrintBattlerStats(battler);
}

// Set up data for a queued action.
static void QueueAction(u32 type, u32 battlerAtk, u32 battlerDef, u32 move)
{
    gDeckStruct.queuedActions[gDeckStruct.actionsCount].type = type;
    gDeckStruct.queuedActions[gDeckStruct.actionsCount].attacker = battlerAtk;
    gDeckStruct.queuedActions[gDeckStruct.actionsCount].target = battlerDef;
    gDeckStruct.queuedActions[gDeckStruct.actionsCount].move = move;

    if (type == ACTION_SWAP)
    {
        gDeckMons[battlerAtk].initialPos = gDeckMons[battlerAtk].pos;
        gDeckMons[battlerDef].initialPos = gDeckMons[battlerDef].pos;
    }

    ++gDeckStruct.actionsCount;
}

static void SwapBattlerPositions(u32 battler1, u32 battler2)
{
    u32 temp;
    SWAP(gDeckMons[battler1].pos, gDeckMons[battler2].pos, temp);
    GetBattlerSprite(gBattlerAttacker)->x = GetBattlerXCoord(gBattlerAttacker);
    GetBattlerSprite(gBattlerTarget)->x = GetBattlerXCoord(gBattlerTarget);
    gDeckMons[gBattlerAttacker].hasSwapped = TRUE;
    gDeckMons[gBattlerTarget].hasSwapped = TRUE;
}

static void ResetTurnValues(void)
{
    for (enum BattleId battler = 0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
    {
        gDeckMons[battler].hasMoved = FALSE;
        gDeckMons[battler].hasSwapped = FALSE;
        gDeckMons[battler].initialPos = gDeckMons[battler].pos;
    }
    gDeckStruct.actionsCount = 0;
    gDeckStruct.executedCount = 0;
}
