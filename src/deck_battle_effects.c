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

/* deck_battle_interface.c
 *
 * This file holds the handling for all move execution and
 * animation tasks. The two are currently interwoven, but
 * should be fairly easy to copy for new move effects.
 * 
*/

static void Task_ExecuteHit(u8 taskId);
static void Task_ExecuteHitAll(u8 taskId);
static void Task_ExecutePowerUp(u8 taskId);

void (*const gMoveEffectTasks[DECK_EFFECT_COUNT])(u8 taskId) =
{
    [DECK_EFFECT_HIT]           = Task_ExecuteHit,
    [DECK_EFFECT_HIT_ALL_OPPONENTS]       = Task_ExecuteHitAll,
    [DECK_EFFECT_POWER_UP]      = Task_ExecutePowerUp,
};

#define tState  data[0]
#define tTimer  data[1]

static void Task_ExecuteHit(u8 taskId)
{
    s32 damage;
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
    case 2: // Check for target change.
        if (!IsDeckBattlerAlive(gBattlerTarget))
            gBattlerTarget = GetRandomBattlerOnSide(GetDeckBattlerSide(gBattlerTarget));
        ++gTasks[taskId].tState;  
        break;
    case 3: // Damage target(s).
        StartBattlerAnim(gBattlerTarget, ANIM_HURT);
        damage = CalculateDamage(gBattlerAttacker, gBattlerTarget, gCurrentMove);
        UpdateBattlerHP(gBattlerTarget, damage);
        PrintMoveOutcomeString(damage);
        PlaySE(SE_EFFECTIVE);
        ++gTasks[taskId].tState;
        break;
    case 4: // Wait for hurt animation.
        if (++gTasks[taskId].tTimer >= 60)
            ++gTasks[taskId].tState;
        break;
    case 5:
        gTasks[taskId].tTimer = 0;
        gTasks[taskId].tState = 0;
        gTasks[taskId].func = Task_CheckFaintAndContinue;
        break;
    }
}

static void Task_ExecuteHitAll(u8 taskId)
{
    s32 damage;
    u32 battlerStart, battlerEnd;

    // Set up indices for targeting.
    if (GetDeckBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
    {
        battlerStart = B_OPPONENT_0;
        battlerEnd = MAX_DECK_BATTLERS_COUNT;
    }
    else
    {
        battlerStart = B_PLAYER_0;
        battlerEnd = B_OPPONENT_0;
    }

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
    case 2: // Damage target(s).
        for (enum BattleId battler = battlerStart; battler < battlerEnd; ++battler)
        {
            if (gDeckMons[battler].species != SPECIES_NONE && gDeckMons[battler].hp != 0)
            {
                StartBattlerAnim(battler, ANIM_HURT);
                damage = CalculateDamage(gBattlerAttacker, battler, gCurrentMove);
                UpdateBattlerHP(battler, damage);
            }
        }
        PrintMoveOutcomeString(0);
        PlaySE(SE_EFFECTIVE);
        ++gTasks[taskId].tState;
        break;
    case 3: // Wait for hurt animation.
        if (++gTasks[taskId].tTimer >= 60)
            ++gTasks[taskId].tState;
        break;
    case 4:
        gTasks[taskId].tTimer = 0;
        gTasks[taskId].tState = 0;
        gTasks[taskId].func = Task_CheckFaintAndContinue;
        break;
    }
}

static void Task_ExecutePowerUp(u8 taskId)
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
    case 2:
        if (!IsDeckBattlerAlive(gBattlerTarget))
        {
            PrintStringToMessageBox(COMPOUND_STRING("But it failed…"));
            gTasks[taskId].tState = 6;
        }
        else
        {
            ++gTasks[taskId].tState;
        }
        break;
    case 3: // Play sound and print string.
        PrintMoveOutcomeString(0);
        PlaySE(SE_M_STAT_INCREASE);
        gDeckMons[gBattlerTarget].powerBoost += gDeckMons[gBattlerAttacker].power / 2;
        ++gTasks[taskId].tState;
        break;
    case 4: // Blend target.
        if (++gTasks[taskId].tTimer >= 60)
        {
            BlendPalettes(1 << (16 + GetBattlerSprite(gBattlerTarget)->oam.paletteNum), 0, RGB_WHITE);
            ++gTasks[taskId].tState;
        }
        else if (gTasks[taskId].tTimer < 60)
        {
            if (gTasks[taskId].tTimer % 4 < 2)
                BlendPalettes(1 << (16 + GetBattlerSprite(gBattlerTarget)->oam.paletteNum), 8, RGB_WHITE);
            else
                BlendPalettes(1 << (16 + GetBattlerSprite(gBattlerTarget)->oam.paletteNum), 0, RGB_WHITE);
        }
        break;
    case 5:
        gTasks[taskId].tTimer = 0;
        gTasks[taskId].tState = 0;
        gTasks[taskId].func = Task_ExecuteQueuedActionOrEnd;
        break;
    case 6:
        if (++gTasks[taskId].tTimer >= 30)
            gTasks[taskId].tState = 5;
        break;
    }
}

void Task_ExecuteSwap(u8 taskId)
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

#undef tState
#undef tTimer
