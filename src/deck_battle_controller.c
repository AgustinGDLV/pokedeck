#include "global.h"
#include "battle.h"
#include "bg.h"
#include "deck_battle.h"
#include "deck_battle_effects.h"
#include "deck_battle_interface.h"
#include "deck_battle_util.h"
#include "deck_battle_controller.h"
#include "deck_battle_ai.h"
#include "event_object_movement.h"
#include "field_weather.h"
#include "gpu_regs.h"
#include "m4a.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "overworld.h"
#include "palette.h"
#include "scanline_effect.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "text.h"
#include "util.h"
#include "constants/abilities.h"
#include "constants/battle.h"
#include "constants/moves.h"
#include "constants/songs.h"
#include "constants/rgb.h"

/* deck_battle_controller.c
 *
 * This file handles all the player input processing for
 * action selection. Move target types can each have their
 * own selection function to allow for greater flexibility.
 * 
*/

static void Task_PlayerSelectAllyToSwap(u8 taskId);
static void Task_PlayerSelectLeftAlly(u8 taskId);
static void Task_PlayerSelectSingleOpponent(u8 taskId);
static void Task_PlayerSelectAllOpponents(u8 taskId);

static void (*const sPlayerMoveTargetTasks[MOVE_TARGET_COUNT])(u8 taskId) =
{
    [MOVE_TARGET_SINGLE_OPPONENT]   = Task_PlayerSelectSingleOpponent,
    [MOVE_TARGET_ALL_OPPONENTS]     = Task_PlayerSelectAllOpponents,
    [MOVE_TARGET_LEFT_ALLY]         = Task_PlayerSelectLeftAlly,
};

#define tState  data[0]
#define tTimer  data[1]

void Task_PlayerSelectAction(u8 taskId)
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

        // Set up UI for targeting.
        SetBattlerPortraitVisibility(FALSE);
        SetGpuReg(REG_OFFSET_BG0VOFS, DISPLAY_HEIGHT);
        SetGpuReg(REG_OFFSET_BG1VOFS, DISPLAY_HEIGHT);
        gTasks[taskId].func = sPlayerMoveTargetTasks[gDeckMovesInfo[gDeckSpeciesInfo[gDeckMons[gBattlerAttacker].species].move].target];
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
        if (gDeckStruct.actionsCount == 0)
        {
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
            gTasks[taskId].func = Task_CloseDeckBattle;
            PlaySE(SE_SELECT);
        }
        else
        {
            struct BattleAction *action = &gDeckStruct.queuedActions[gDeckStruct.actionsCount - 1];
            if (action->type == ACTION_SWAP)
            {
                SwapBattlerPositions(action->attacker, action->target);
                GetBattlerSprite(action->attacker)->oam.objMode = ST_OAM_OBJ_NORMAL;
                GetBattlerSprite(action->target)->oam.objMode = ST_OAM_OBJ_NORMAL;
                gDeckMons[action->attacker].hasSwapped = FALSE;
                gDeckMons[action->target].hasSwapped = FALSE;

                if (gDeckMons[action->attacker].pos == gDeckStruct.selectedPos || gDeckMons[action->target].pos == gDeckStruct.selectedPos)
                {
                    UpdateBattlerSelection(action->attacker, FALSE);
                    UpdateBattlerSelection(action->target, FALSE);
                    UpdateBattlerSelection(GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos), TRUE);
                    DisplayActionSelectionInfo(GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos));
                }
                --gDeckStruct.actionsCount;
            }
            else
            {
                SetBattlerGrayscale(action->attacker, FALSE);
                gDeckMons[action->attacker].hasMoved = FALSE;
                --gDeckStruct.actionsCount;
            }
        }
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

static void Task_PlayerSelectLeftAlly(u8 taskId)
{
    enum BattleId battler;
    if (gTasks[taskId].tState == 0)
    {
        battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, GetOccupiedOnLeft(B_SIDE_PLAYER, gDeckMons[gBattlerAttacker].pos));
        if (battler != MAX_DECK_BATTLERS_COUNT && gDeckMons[battler].species != SPECIES_NONE && gDeckMons[battler].hp != 0)
        {
            UpdateBattlerSelection(battler, TRUE);
            DisplayTargetSelectionInfo(battler);
        }
        else
        {
            DisplayTargetSelectionInfo(gBattlerAttacker); // *TODO - handle no left ally
        }
        ++gTasks[taskId].tState;
    }
    if (gMain.newKeys & B_BUTTON)
    {
        // Deselect target.
        PlaySE(SE_SELECT);
        battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, GetOccupiedOnLeft(B_SIDE_PLAYER, gDeckMons[gBattlerAttacker].pos));
        if (battler != MAX_DECK_BATTLERS_COUNT && gDeckMons[battler].species != SPECIES_NONE && gDeckMons[battler].hp != 0)
            UpdateBattlerSelection(battler, FALSE);

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
        gTasks[taskId].tState = 0;
    }
    if (gMain.newKeys & A_BUTTON)
    {
        // Deselect target.
        PlaySE(SE_SELECT);
        gBattlerTarget = GetDeckBattlerAtPos(B_SIDE_PLAYER, GetOccupiedOnLeft(B_SIDE_PLAYER, gDeckMons[gBattlerAttacker].pos));
        if (gBattlerTarget != MAX_DECK_BATTLERS_COUNT && gDeckMons[gBattlerTarget].species != SPECIES_NONE && gDeckMons[gBattlerTarget].hp != 0)
            UpdateBattlerSelection(gBattlerTarget, FALSE);

        // Queue attack action and update data.
        QueueAction(ACTION_ATTACK, gBattlerAttacker, gBattlerTarget, gDeckSpeciesInfo[gDeckMons[gBattlerAttacker].species].move);
        SetBattlerGrayscale(gBattlerAttacker, TRUE);
        gDeckMons[gBattlerAttacker].hasMoved = TRUE;
        StartBattlerAnim(gBattlerAttacker, ANIM_PAUSED);

        // Select next battler for action selection or begin action phase.
        gDeckStruct.selectedPos = GetLeftmostPositionToMove(B_SIDE_PLAYER);
        gTasks[taskId].tState = 0;
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

static void Task_PlayerSelectSingleOpponent(u8 taskId)
{
    enum BattleId battler;
    enum BattlePosition pos;
    if (gTasks[taskId].tState == 0)
    {
        // Display first possible target.
        gDeckStruct.selectedPos = GetLeftmostOccupiedPosition(B_SIDE_OPPONENT);
        battler = GetDeckBattlerAtPos(B_SIDE_OPPONENT, gDeckStruct.selectedPos);
        UpdateBattlerSelection(battler, TRUE);
        DisplayTargetSelectionInfo(battler);
        ++gTasks[taskId].tState;
    }
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
        gTasks[taskId].tState = 0;
    }
    if (gMain.newKeys & A_BUTTON)
    {
        // Deselect target.
        PlaySE(SE_SELECT);
        gBattlerTarget = GetDeckBattlerAtPos(B_SIDE_OPPONENT, gDeckStruct.selectedPos);
        UpdateBattlerSelection(gBattlerTarget, FALSE);

        // Queue attack action and update data.
        QueueAction(ACTION_ATTACK, gBattlerAttacker, gBattlerTarget, gDeckSpeciesInfo[gDeckMons[gBattlerAttacker].species].move);
        SetBattlerGrayscale(gBattlerAttacker, TRUE);
        gDeckMons[gBattlerAttacker].hasMoved = TRUE;
        StartBattlerAnim(gBattlerAttacker, ANIM_PAUSED);

        // Select next battler for action selection or begin action phase.
        gDeckStruct.selectedPos = GetLeftmostPositionToMove(B_SIDE_PLAYER);
        gTasks[taskId].tState = 0;
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

static void Task_PlayerSelectAllOpponents(u8 taskId)
{
    enum BattleId battler;
    if (gTasks[taskId].tState == 0)
    {
        for (battler = B_OPPONENT_0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
        {
            if (gDeckMons[battler].species != SPECIES_NONE && gDeckMons[battler].hp != 0)
                UpdateBattlerSelection(battler, TRUE);
        }
        DisplayTargetSelectionInfo(B_OPPONENT_0);
        ++gTasks[taskId].tState;
    }
    if (gMain.newKeys & B_BUTTON)
    {
        // Deselect target.
        PlaySE(SE_SELECT);
        for (battler = B_OPPONENT_0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
        {
            if (gDeckMons[battler].species != SPECIES_NONE && gDeckMons[battler].hp != 0)
                UpdateBattlerSelection(battler, FALSE);
        }

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
        gTasks[taskId].tState = 0;
    }
    if (gMain.newKeys & A_BUTTON)
    {
        // Deselect target.
        PlaySE(SE_SELECT);
        for (battler = B_OPPONENT_0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
        {
            if (gDeckMons[battler].species != SPECIES_NONE && gDeckMons[battler].hp != 0)
                UpdateBattlerSelection(battler, FALSE);
        }

        // Queue attack action and update data.
        QueueAction(ACTION_ATTACK, gBattlerAttacker, MAX_DECK_BATTLERS_COUNT, gDeckSpeciesInfo[gDeckMons[gBattlerAttacker].species].move);
        SetBattlerGrayscale(gBattlerAttacker, TRUE);
        gDeckMons[gBattlerAttacker].hasMoved = TRUE;
        StartBattlerAnim(gBattlerAttacker, ANIM_PAUSED);

        // Select next battler for action selection or begin action phase.
        gDeckStruct.selectedPos = GetLeftmostPositionToMove(B_SIDE_PLAYER);
        gTasks[taskId].tState = 0;
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

#undef tState
#undef tTimer
