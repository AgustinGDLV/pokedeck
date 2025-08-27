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
#include "event_data.h"
#include "field_weather.h"
#include "gpu_regs.h"
#include "m4a.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "menu_helpers.h"
#include "money.h"
#include "overworld.h"
#include "palette.h"
#include "scanline_effect.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
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
#include "constants/vars.h"

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

static void Task_HandleBattleVictory(u8 taskId);
static void Task_HandleBattleLoss(u8 taskId);

static u32 GetBattleSpeedScale(void);

// ewram data
EWRAM_DATA struct DeckBattleStruct gDeckStruct = {0};
EWRAM_DATA struct DeckBattlePokemon gDeckMons[MAX_DECK_BATTLERS_COUNT] = {0};

// code
#include "data/graphics/deck_pokemon.h"
#include "data/pokemon/deck_species_info.h"
#include "data/deck_moves.h"

static void MainCB2_DeckBattle(void) // battle speed up ported from Pokeabbie by Alex/Rain
{
    u32 speedScale = GetBattleSpeedScale();
    if (PrevPaletteFadeResult() == PALETTE_FADE_STATUS_LOADING)
        speedScale = 1;

    if (speedScale <= 1)
    {
        RunTasks();
        AnimateSprites();
        BuildOamBuffer();
        DoScheduledBgTilemapCopiesToVram();
        UpdatePaletteFade();
        RunTextPrinters();
    }
    else
    {
        u32 s;
        u32 fadeResult;

        // Update select entries at higher speed
        // disable speed up during palette fades otherwise we run into issues with blending
        // (e.g. moves that change background like Psychic can get stuck or have their colours overflow)
        for(s = 1; s < speedScale; ++s)
        {
            AnimateSprites();
            RunTextPrinters();
            fadeResult = UpdatePaletteFade();

            if (fadeResult == PALETTE_FADE_STATUS_LOADING)
            {
                // minimal final update as we've just started a fade
                BuildOamBuffer();
                RunTasks();
                break;
            }
            else
            {
                RunTasks();
                VBlankCB2_DeckBattle();

                // Call it again to make sure everything is behaving as it should (this is crazy town now)
                if (gMain.callback1)
                    gMain.callback1();
            }
        }

        if (fadeResult != PALETTE_FADE_STATUS_LOADING)
        {
            // final update
            RunTasks();
            AnimateSprites();
            BuildOamBuffer();
            DoScheduledBgTilemapCopiesToVram();
            UpdatePaletteFade();
            RunTextPrinters();
        }
    }
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
            CalculatePlayerPartyCount();
            CalculateEnemyPartyCount();
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
            gDeckStruct.isSelectionPhase = TRUE;
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
        gDeckStruct.actingSide = B_SIDE_PLAYER;
        SetBattlerPortraitVisibility(FALSE);
        SetGpuReg(REG_OFFSET_BG0VOFS, DISPLAY_HEIGHT);
        SetGpuReg(REG_OFFSET_BG1VOFS, DISPLAY_HEIGHT);
        PrintStringToMessageBox(COMPOUND_STRING("Wild Pokémon appeared!"));
        gTasks[taskId].tState += 1;
    }
    else if (!gPaletteFade.active && (gMain.newKeys & A_BUTTON))
    {
        PlaySE(SE_SELECT);
        if (gSaveBlock2Ptr->optionsBattleStyle == OPTIONS_BATTLE_STYLE_AUTO)
        {
            SetGpuReg(REG_OFFSET_BG0VOFS, DISPLAY_HEIGHT);
            SetGpuReg(REG_OFFSET_BG1VOFS, DISPLAY_HEIGHT);
            gTasks[taskId].func = Task_AutoSelectAction;
            gTasks[taskId].tState = 0;
            gTasks[taskId].tTimer = 0;
        }
        else
        {
            // Start selection phase and update display.
            enum BattleId battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
            PrintBattlerMoveInfo(battler);
            SetBattlerPortraitVisibility(TRUE);
            // HP bar updated before fade begins
            CreateSelectionCursorOverBattler(battler);
            StartBattlerAnim(battler, ANIM_IDLE);

            SetGpuReg(REG_OFFSET_BG0VOFS, 0);
            SetGpuReg(REG_OFFSET_BG1VOFS, 0);
            gTasks[taskId].func = Task_PlayerSelectAction;
            gTasks[taskId].tState = 0;
            gTasks[taskId].tTimer = 0;
        }
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
        gDeckStruct.isSelectionPhase = FALSE;
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
            if (GetDeckBattlerSide(battler) == B_SIDE_OPPONENT)
                gDeckStruct.exp += gSpeciesInfo[gDeckMons[battler].species].expYield * gDeckMons[battler].lvl;
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
        gTasks[taskId].func = Task_HandleBattleLoss;
    }
    else if (!IsBattlerAliveOnSide(B_SIDE_OPPONENT))
    {
        gBattleOutcome = B_OUTCOME_WON;
        gTasks[taskId].func = Task_HandleBattleVictory;
    }
    else
    {
        gTasks[taskId].func = Task_ExecuteQueuedActionOrEnd;
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
        gDeckStruct.isSelectionPhase = TRUE;
        if (gDeckStruct.actingSide == B_SIDE_PLAYER)
        {
            // Return to auto battle if enabled.
            if (gSaveBlock2Ptr->optionsBattleStyle == OPTIONS_BATTLE_STYLE_AUTO)
            {
                SetGpuReg(REG_OFFSET_BG0VOFS, DISPLAY_HEIGHT);
                SetGpuReg(REG_OFFSET_BG1VOFS, DISPLAY_HEIGHT);
                gTasks[taskId].func = Task_AutoSelectAction;
                gTasks[taskId].tState = 0;
                gTasks[taskId].tTimer = 0;
            }
            // Or set up UI for action selection.
            else
            {
                gDeckStruct.selectedPos = GetLeftmostPositionToMove(B_SIDE_PLAYER);
                u32 battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
                UpdateBattlerSelection(battler, TRUE);
                DisplayActionSelectionInfo(battler);

                SetBattlerBobPause(FALSE);
                SetBattlerPortraitVisibility(TRUE);
                SetGpuReg(REG_OFFSET_BG0VOFS, 0);
                SetGpuReg(REG_OFFSET_BG1VOFS, 0);
                gTasks[taskId].func = Task_PlayerSelectAction;
            }
        }
        else
        {
            gTasks[taskId].func = Task_OpponentSelectAction;
        }
    }
}

static void Task_HandleBattleVictory(u8 taskId)
{
    switch (gTasks[taskId].tState)
    {
    default:
    case 0:
    {
        u32 exp = gDeckStruct.exp / gPlayerPartyCount; // *TODO - variable exp gain per battler
        ConvertIntToDecimalStringN(gStringVar2, exp, STR_CONV_MODE_LEFT_ALIGN, 5);
        StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("Your party gained an average of {STR_VAR_2} Exp. Points!"));
        PrintStringToMessageBox(gStringVar1);

        PlayBGM(MUS_VICTORY_WILD);
        gDeckStruct.battlerExp = B_PLAYER_0;
        ++gTasks[taskId].tState;
        break;
    }
    case 1:
        if (++gTasks[taskId].tTimer > 10 && (gMain.newKeys & A_BUTTON))
        {
            PlaySE(SE_SELECT);
            gTasks[taskId].tTimer = 0;
            ++gTasks[taskId].tState;
        }
        break;
    case 2: // Give experience to the current battler and print message if they level.
        if (gDeckMons[gDeckStruct.battlerExp].species != SPECIES_NONE)
        {
            u32 level = gDeckMons[gDeckStruct.battlerExp].lvl;
            u32 species = gDeckMons[gDeckStruct.battlerExp].species;
            u32 currentExp = GetMonData(&gPlayerParty[gDeckMons[gDeckStruct.battlerExp].partyIndex], MON_DATA_EXP);
            u32 nextLevelExp = gExperienceTables[gSpeciesInfo[species].growthRate][level + 1];
            u32 gainedExp = gDeckStruct.exp / gPlayerPartyCount;
            u32 expAfterGain = currentExp + gainedExp;

            SetMonData(&gPlayerParty[gDeckMons[gDeckStruct.battlerExp].partyIndex], MON_DATA_EXP, &expAfterGain);
            CalculateMonStats(&gPlayerParty[gDeckMons[gDeckStruct.battlerExp].partyIndex]);
            if (expAfterGain >= nextLevelExp)
                gTasks[taskId].tState = 3;
            else
                gTasks[taskId].tState = 4;
        }
        else
        {
            gTasks[taskId].tState = 4;
        }
        break;
    case 3: // Print level up message.
        if (gTasks[taskId].tTimer == 0)
        {
            StringCopy(gStringVar2, GetSpeciesName(gDeckMons[gDeckStruct.battlerExp].species));
            StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("{STR_VAR_2} leveled up!"));
            PrintStringToMessageBox(gStringVar1);
            PlaySE(MUS_LEVEL_UP);
            ++gTasks[taskId].tTimer;
        }
        else if (++gTasks[taskId].tTimer > 10 && (gMain.newKeys & A_BUTTON))
        {
            PlaySE(SE_SELECT);
            gTasks[taskId].tTimer = 0;
            ++gTasks[taskId].tState;
        }
        break;
    case 4: // Loop until all player positions have been given exp.
        if (++gDeckStruct.battlerExp > B_PLAYER_5)
            gTasks[taskId].tState = 5;
        else
            gTasks[taskId].tState = 2;
        break;
    case 5:
        gTasks[taskId].tState = 0;
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
        gTasks[taskId].func = Task_CloseDeckBattle;
        break;
    }
}

static void Task_HandleBattleLoss(u8 taskId)
{
    switch (gTasks[taskId].tState)
    {
    default:
    case 0:
        PrintStringToMessageBox(COMPOUND_STRING("You have no more Pokémon that can fight!"));
        ++gTasks[taskId].tState;
        break;
    case 1:
        if (++gTasks[taskId].tTimer > 10 && (gMain.newKeys & A_BUTTON))
        {
            PlaySE(SE_SELECT);
            gTasks[taskId].tTimer = 0;
            ++gTasks[taskId].tState;
        }
        break;
    case 2: // *TODO: check for scripted loss
    {
        u32 partyLevel = 0;
        for (enum BattleId battler = B_PLAYER_0; battler < B_OPPONENT_0; ++battler)
            partyLevel += gDeckMons[battler].lvl;
        RemoveMoney(&gSaveBlock1Ptr->money, 8 * partyLevel);
        ConvertIntToDecimalStringN(gStringVar2, 8 * partyLevel, STR_CONV_MODE_LEFT_ALIGN, 5);
        StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("You panicked and dropped ¥{STR_VAR_2}!"));
        PrintStringToMessageBox(gStringVar1);
        ++gTasks[taskId].tState;
        break;
    }
    case 3:
        if (++gTasks[taskId].tTimer > 10 && (gMain.newKeys & A_BUTTON))
        {
            PlaySE(SE_SELECT);
            gTasks[taskId].tTimer = 0;
            ++gTasks[taskId].tState;
        }
        break;
    case 4:
        PrintStringToMessageBox(COMPOUND_STRING("You were overwhelmed by your defeat!"));
        ++gTasks[taskId].tState;
    case 5:
        if (++gTasks[taskId].tTimer > 10 && (gMain.newKeys & A_BUTTON))
        {
            PlaySE(SE_SELECT);
            gTasks[taskId].tTimer = 0;
            ++gTasks[taskId].tState;
        }
        break;
    case 6:
        gTasks[taskId].tState = 0;
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
        gTasks[taskId].func = Task_CloseDeckBattle;
        break;
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
    if (gBattleOutcome == B_OUTCOME_LOST)
        SetMainCallback2(CB2_WhiteOut); // *TODO: fade out looks off
    else
        SetMainCallback2(CB2_ReturnToField);
}

#undef tState
#undef tTimer

// Initialize gDeckStruct to start battle with clean data.
static void InitBattleStructData(void)
{
    ResetTurnValues();
    gDeckStruct.exp = 0;
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
void LoadDummyEnemyParty(void)
{
    CreateMon(&gEnemyParty[0], SPECIES_BELLSPROUT, 5, USE_RANDOM_IVS, 0, 0, OT_ID_PLAYER_ID, 0);
    CreateMon(&gEnemyParty[1], SPECIES_SLOWPOKE, 5, USE_RANDOM_IVS, 0, 0, OT_ID_PLAYER_ID, 0);
    CreateMon(&gEnemyParty[2], SPECIES_SLOWBRO, 8, USE_RANDOM_IVS, 0, 0, OT_ID_PLAYER_ID, 0);
    CreateMon(&gEnemyParty[3], SPECIES_APPLIN, 5, USE_RANDOM_IVS, 0, 0, OT_ID_PLAYER_ID, 0);
    CreateMon(&gEnemyParty[4], SPECIES_BELLSPROUT, 5, USE_RANDOM_IVS, 0, 0, OT_ID_PLAYER_ID, 0);
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
        gDeckMons[i].partyIndex = i;
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

// Performs basic damage calc formula using two battler IDs and a move.
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

// Performs the HP change when a battler is hurt or healed.
void UpdateBattlerHP(u32 battler, s32 damage)
{
    if (damage < -999) // cap damage at 3 digits
        damage = -999;

    if (damage > gDeckMons[battler].hp) // correctly bound HP
        gDeckMons[battler].hp = 0;
    else if (-damage > gDeckMons[battler].maxHP - gDeckMons[battler].hp)
        gDeckMons[battler].hp = gDeckMons[battler].maxHP;
    else
        gDeckMons[battler].hp -= damage;

    PrintDamageNumbers(battler, damage);
}

u32 GetBattleSpeedScale(void)
{
    // Don't speed up during selection phase or if holding L.
    if (JOY_HELD(L_BUTTON) || gDeckStruct.isSelectionPhase)
        return 1;
    
    switch (VarGet(VAR_BATTLE_SPEED))
    {
    default:
    case 0:
        return 1;
    case 1:
        return 2;
    case 2:
        return 4;
    }
}
