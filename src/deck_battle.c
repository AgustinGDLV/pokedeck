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
#include "constants/pokemon.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/species.h"

/* deck_battle.c
 *
 * This file handles the main flow for the battle engine,
 * i.e. data initalization, opening and ending sequences,
 * and action execution.
 * 
*/

// forward declarations
static void MainCB2_DeckBattle(void);
static void VBlankCB2_DeckBattle(void);
static void Task_OpenDeckBattle(u8 taskId);
static void InitBattleStructData(void);
static void InitBattleMonData(void);
static void ResetTurnValues(void);

// ewram data
EWRAM_DATA struct DeckBattleStruct gDeckStruct = {0};
EWRAM_DATA struct DeckBattlePokemon gDeckMons[MAX_DECK_BATTLERS_COUNT] = {0};

// code
#include "data/graphics/deck_pokemon.h"
#include "data/pokemon/deck_species_info.h"
#include "data/deck_moves.h"

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
    PlayBGM(MUS_VS_WILD);
    FadeScreen(FADE_TO_BLACK, 0);
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
            InitBattleMonData();
            InitBattleStructData();
            gMain.state++;
            break;
        case 5:
            InitDeckBattleGfx();
            gMain.state++;
            break;
        case 6:
            for (enum BattleId battler = 0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
                LoadBattlerObjectSprite(battler);
            gMain.state++;
            break;
        case 7:
            BeginNormalPaletteFade(PALETTES_ALL, 4, 16, 0, RGB_BLACK);
            SetVBlankCallback(VBlankCB2_DeckBattle);
            CreateTask(Task_OpenDeckBattle, 0);
            SetMainCallback2(MainCB2_DeckBattle);
            break;
    }
}

#define tState  data[0]
#define tTimer  data[1]

// Fades in battle UI and sets up remaining graphics.
static void Task_OpenDeckBattle(u8 taskId)
{
    if (gTasks[taskId].tState == 0)
    {
        enum BattleId battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        PrintBattlerMoveInfo(battler);
        // HP bar updated before fade begins
        gDeckStruct.actingSide = B_SIDE_PLAYER;
        CreateSelectionCursorOverBattler(battler);
        StartBattlerAnim(battler, ANIM_IDLE);
        gTasks[taskId].tState += 1;
    }
    else if (!gPaletteFade.active)
    {
        gTasks[taskId].tState = 0;
        gTasks[taskId].func = Task_PlayerSelectAction;
    }
}

// Set up data and reset positions to begin action phase.
void Task_PrepareForActionPhase(u8 taskId)
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

void Task_CheckFaintAndContinue(u8 taskId)
{
    bool32 fainted = FALSE;
    for (enum BattleId battler = B_PLAYER_0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
    {
        if (!GetBattlerSprite(battler)->invisible && !IsDeckBattlerAlive(battler))
        {
            StartBattlerAnim(battler, ANIM_FAINT);
            fainted = TRUE;
        }
    }
    if (fainted)
    {
        PlaySE(SE_FAINT);
        gTasks[taskId].func = Task_WaitForFaintAnim;
    }
    else
    {
        gTasks[taskId].func = Task_ExecuteQueuedActionOrEnd;
    }
}

void Task_WaitForFaintAnim(u8 taskId)
{
    if (++gTasks[taskId].tTimer > 48)
    {
        gTasks[taskId].tTimer = 0;
        gTasks[taskId].func = Task_CheckForBattleEnd;
    }
}

void Task_CheckForBattleEnd(u8 taskId)
{
    if (!IsBattlerAliveOnSide(B_SIDE_PLAYER))
    {
        gBattleOutcome = B_OUTCOME_LOST;
        PrintStringToMessageBox(COMPOUND_STRING("Battle lost…"));
        gTasks[taskId].func = Task_HandleBattleEnd;
    }
    else if (!IsBattlerAliveOnSide(B_SIDE_OPPONENT))
    {
        gBattleOutcome = B_OUTCOME_WON;
        PrintStringToMessageBox(COMPOUND_STRING("Battle won!"));
        gTasks[taskId].func = Task_HandleBattleEnd;
    }
    else
    {
        gTasks[taskId].func = Task_ExecuteQueuedActionOrEnd;
    }
}

void Task_HandleBattleEnd(u8 taskId)
{
    if (++gTasks[taskId].tTimer > 60)
    {
        gTasks[taskId].tTimer = 0;
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
        gTasks[taskId].func = Task_CloseDeckBattle;
    }
}

// Runs main action phase loop until no queued actions are left.
void Task_ExecuteQueuedActionOrEnd(u8 taskId)
{
    // Set up data and execute queued action if any remain.
    if (gDeckStruct.executedCount < gDeckStruct.actionsCount)
    {
        gBattlerAttacker = gDeckStruct.queuedActions[gDeckStruct.executedCount].attacker;
        gBattlerTarget = gDeckStruct.queuedActions[gDeckStruct.executedCount].target;
        gCurrentMove = gDeckStruct.queuedActions[gDeckStruct.executedCount].move;

        gTasks[taskId].tTimer = 0;
        gTasks[taskId].tState = 0;
        if (gDeckStruct.queuedActions[gDeckStruct.executedCount].type == ACTION_ATTACK)
            gTasks[taskId].func = gMoveEffectTasks[gDeckMovesInfo[gCurrentMove].effect];
        else
            gTasks[taskId].func = Task_ExecuteSwap;
        ++gDeckStruct.executedCount;
    }
    // Otherwise, return to action selection.
    else
    {
        ResetTurnValues();
        gDeckStruct.actingSide ^= 1; // get opposite side
        if (gDeckStruct.actingSide == B_SIDE_PLAYER)
        {
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
        else
        {
            gTasks[taskId].func = Task_OpponentSelectAction;
        }
    }
}

// Exits battle UI and sends to overworld.
void Task_CloseDeckBattle(u8 taskId)
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
    FadeOutMapMusic(5);
    SetMainCallback2(CB2_ReturnToField);
}

#undef tState
#undef tTimer

// Initialize gDeckStruct to start battle with clean data.
static void InitBattleStructData(void)
{
    ResetTurnValues();
    gDeckStruct.selectedPos = GetLeftmostOccupiedPosition(B_SIDE_PLAYER);
}

// Reset struct data associated with a single turn.
static void ResetTurnValues(void)
{
    for (enum BattleId battler = 0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
    {
        gDeckMons[battler].hasMoved = FALSE;
        gDeckMons[battler].hasSwapped = FALSE;
        gDeckMons[battler].initialPos = gDeckMons[battler].pos;
        gDeckMons[battler].powerBoost = 0;
    }
    gDeckStruct.actionsCount = 0;
    gDeckStruct.executedCount = 0;
}

// Populate enemy party with dummy data.
static void LoadDummyEnemyParty(void)
{
    CreateMon(&gEnemyParty[0], SPECIES_SLOWPOKE, 5, USE_RANDOM_IVS, 0, 0, OT_ID_PLAYER_ID, 0);
    CreateMon(&gEnemyParty[1], SPECIES_BELLSPROUT, 5, USE_RANDOM_IVS, 0, 0, OT_ID_PLAYER_ID, 0);
    CreateMon(&gEnemyParty[2], SPECIES_SWABLU, 5, USE_RANDOM_IVS, 0, 0, OT_ID_PLAYER_ID, 0);
    CreateMon(&gEnemyParty[3], SPECIES_SLOWBRO, 5, USE_RANDOM_IVS, 0, 0, OT_ID_PLAYER_ID, 0);
    CreateMon(&gEnemyParty[4], SPECIES_MAREEP, 5, USE_RANDOM_IVS, 0, 0, OT_ID_PLAYER_ID, 0);
    CreateMon(&gEnemyParty[5], SPECIES_SLOWPOKE, 5, USE_RANDOM_IVS, 0, 0, OT_ID_PLAYER_ID, 0);

    u32 pos = 0;
    SetMonData(&gEnemyParty[0], MON_DATA_POSITION, &pos); pos++;
    SetMonData(&gEnemyParty[1], MON_DATA_POSITION, &pos); pos++;
    SetMonData(&gEnemyParty[2], MON_DATA_POSITION, &pos); pos++;
    SetMonData(&gEnemyParty[3], MON_DATA_POSITION, &pos); pos++;
    SetMonData(&gEnemyParty[4], MON_DATA_POSITION, &pos); pos++;
    SetMonData(&gEnemyParty[5], MON_DATA_POSITION, &pos);
}

// Load party data into gDeckMons struct.
static void InitBattleMonData(void)
{
    struct Pokemon *mon;
    LoadDummyEnemyParty();
    for (u32 i = 0; i < MAX_DECK_BATTLERS_COUNT; ++i)
    {
        if (GetDeckBattlerSide(i) == B_SIDE_PLAYER)
            mon = &gPlayerParty[i];
        else
            mon = &gEnemyParty[i - POSITIONS_COUNT];
        gDeckMons[i].species = GetMonData(mon, MON_DATA_SPECIES);
        gDeckMons[i].lvl = GetMonData(mon, MON_DATA_LEVEL);
        gDeckMons[i].hp = GetMonData(mon, MON_DATA_HP);
        gDeckMons[i].maxHP = GetMonData(mon, MON_DATA_MAX_HP);
        gDeckMons[i].power = GetMonData(mon, MON_DATA_ATK);
        gDeckMons[i].def = GetMonData(mon, MON_DATA_DEF);
        gDeckMons[i].pos = GetMonData(mon, MON_DATA_POSITION);
        gDeckMons[i].initialPos = gDeckMons[i].pos;
    }
}

// Set up data for a queued action.
void QueueAction(u32 type, u32 battlerAtk, u32 battlerDef, u32 move)
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

// Update data required to swap two battler positions.
void SwapBattlerPositions(u32 battler1, u32 battler2)
{
    u32 temp;
    SWAP(gDeckMons[battler1].pos, gDeckMons[battler2].pos, temp);
    GetBattlerSprite(battler1)->x = GetBattlerXCoord(battler1);
    GetBattlerSprite(battler2)->x = GetBattlerXCoord(battler2);
    gDeckMons[battler1].hasSwapped = TRUE;
    gDeckMons[battler2].hasSwapped = TRUE;
}

s32 CalculateDamage(u32 battlerAtk, u32 battlerDef, u32 move)
{
    u32 movePower = gDeckMovesInfo[move].power;
    u32 level = gDeckMons[battlerAtk].lvl;
    u32 power = gDeckMons[battlerAtk].power + gDeckMons[battlerAtk].powerBoost;
    u32 defense = gDeckMons[battlerDef].def;

    s32 dmg = movePower * power * (2 * level / 5 + 2) / defense / 50 + 2;
    dmg *= DMG_ROLL_PERCENT_HI - RandomUniform(RNG_DAMAGE_MODIFIER, 0, DMG_ROLL_PERCENT_HI - DMG_ROLL_PERCENT_LO);
    dmg /= 100;

    if (dmg != 0)
        return dmg;
    else
        return 1;
}

void UpdateBattlerHP(u32 battler, s32 damage)
{
    if (damage > gDeckMons[battler].hp)
        gDeckMons[battler].hp = 0;
    else
        gDeckMons[battler].hp -= damage;
    PrintDamageNumbers(battler, damage);
}
