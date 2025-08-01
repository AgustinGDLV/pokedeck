#ifndef GUARD_DECK_BATTLE_H
#define GUARD_DECK_BATTLE_H

enum BattleId
{
    B_PLAYER_0,
    B_PLAYER_1,
    B_PLAYER_2,
    B_PLAYER_3,
    B_PLAYER_4,
    B_PLAYER_5,
    B_OPPONENT_0,
    B_OPPONENT_1,
    B_OPPONENT_2,
    B_OPPONENT_3,
    B_OPPONENT_4,
    B_OPPONENT_5,
    MAX_DECK_BATTLERS_COUNT
};

enum BattlePosition // left to right for each side
{
    POSITION_0,
    POSITION_1,
    POSITION_2,
    POSITION_3,
    POSITION_4,
    POSITION_5,
    POSITIONS_COUNT,
};

struct DeckSpeciesInfo
{
    u16 baseHP;
    u16 basePower;
    u16 baseDef;
    u16 move;
    u16 ability;

    const u8 *playerIdle;
    const u8 *opponentIdle;
    const u8 *playerAttack;
    const u8 *opponentAttack;
    const u8 *playerHurt;
    const u8 *opponentHurt;
    const u16 *objectPalette;
    const s8 playerYOffset;
    const s8 opponentYOffset;

    const u8 *portrait;
    const u16 *portraitPalette;
};

struct DeckBattlePokemon
{
    u16 species;
    u16 lvl;
    u16 hp;
    u16 maxHP;
    u16 power;
    u16 def;
    enum BattlePosition pos;
    enum BattlePosition initialPos;
    bool8 hasMoved;
    bool8 hasSwapped;

    u16 powerBoost; // flat power boost
};

// action constants
#define ACTION_ATTACK   0
#define ACTION_SWAP     1

#define MAX_ACTIONS     12

struct BattleAction
{
    u8 type;
    u8 attacker;
    u8 target;
    u16 move;
};

// move constants
enum MoveTarget
{
    MOVE_TARGET_SINGLE_OPPONENT,
    MOVE_TARGET_ALL_OPPONENTS,
    MOVE_TARGET_ALL_OPPONENTS_ADJACENT_ALLIES,
    MOVE_TARGET_LEFT_ALLY,
    MOVE_TARGET_COUNT,
};

struct DeckMoveInfo
{
    const u8* name;
    const u8* description;
    u8 power;
    u8 target;
    u8 effect;
};

struct DeckBattleStruct
{
    enum BattlePosition selectedPos; // for player selection
    struct BattleAction queuedActions[MAX_ACTIONS];
    u8 actionsCount;
    u8 executedCount;
    u8 actingSide;
};

void CB2_OpenDeckBattleCustom(void);
void Task_PrepareForActionPhase(u8 taskId);
void Task_CheckFaintAndContinue(u8 taskId);
void Task_WaitForFaintAnim(u8 taskId);
void Task_CheckForBattleEnd(u8 taskId);
void Task_HandleBattleEnd(u8 taskId);
void Task_CloseDeckBattle(u8 taskId);
void Task_ExecuteQueuedActionOrEnd(u8 taskId);
void QueueAction(u32 type, u32 battlerAtk, u32 battlerDef, u32 move);
void SwapBattlerPositions(u32 battler1, u32 battler2);
s32 CalculateDamage(u32 battlerAtk, u32 battlerDef, u32 move);
void UpdateBattlerHP(u32 battler, s32 damage);

extern struct DeckBattleStruct gDeckStruct;
extern struct DeckBattlePokemon gDeckMons[MAX_DECK_BATTLERS_COUNT];
extern const struct DeckSpeciesInfo gDeckSpeciesInfo[NUM_SPECIES];
extern const struct DeckMoveInfo gDeckMovesInfo[MOVES_COUNT];

#endif
