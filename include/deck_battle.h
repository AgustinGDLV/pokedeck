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
    u16 basePWR;
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
    u16 hp;
    u16 maxHP;
    u16 pwr;
    enum BattlePosition position;
    bool8 hasMoved;
};

struct DeckBattleStruct
{
    u8 actingSide;
    u8 remainingActions[NUM_BATTLE_SIDES];
    u8 maxActions[NUM_BATTLE_SIDES];
    enum BattlePosition selectedPosition; // for player selection
    enum BattleId battlerAtPosition[NUM_BATTLE_SIDES][POSITIONS_COUNT];
};

void CB2_OpenDeckBattleCustom(void);

extern struct DeckBattleStruct gDeckBattleStruct;
extern struct DeckBattlePokemon gDeckBattleMons[MAX_DECK_BATTLERS_COUNT];
extern const struct DeckSpeciesInfo gSpeciesDeckInfo[NUM_SPECIES];

#endif
