#include "global.h"
#include "battle.h"
#include "deck_battle.h"
#include "deck_battle_interface.h"
#include "deck_battle_util.h"
#include "constants/species.h"

u32 GetDeckBattlerSide(enum BattleId battler)
{
    if (battler < B_OPPONENT_0)
        return B_SIDE_PLAYER;
    else
        return B_SIDE_OPPONENT;
}

u32 GetDeckBattlerAtPosition(u32 side, enum BattlePosition position)
{
    return gDeckBattleStruct.battlerAtPosition[side][position];
}

enum BattlePosition GetLeftmostOccupiedPosition(u32 side)
{
    enum BattleId battler;
    for (enum BattlePosition pos = POSITION_0; pos < POSITIONS_COUNT; ++pos)
    {
        battler = GetDeckBattlerAtPosition(side, pos);
        if (gDeckBattleMons[battler].species != SPECIES_NONE && gDeckBattleMons[battler].hp != 0)
            return pos;
    }
    return POSITION_0; // no luck
}

enum BattlePosition GetLeftmostPositionToMove(u32 side)
{
    enum BattleId battler;
    for (enum BattlePosition pos = POSITION_0; pos < POSITIONS_COUNT; ++pos)
    {
        battler = GetDeckBattlerAtPosition(side, pos);
        if (!gDeckBattleMons[battler].hasMoved && gDeckBattleMons[battler].hp != 0)
            return pos;
    }
    return POSITION_0; // no luck
}

enum BattlePosition GetOccupiedOnLeft(u32 side, enum BattlePosition position)
{
    enum BattleId battler;
    if (position == POSITION_0)
        return POSITIONS_COUNT;

    for (enum BattlePosition pos = position - 1; pos >= POSITION_0 && pos != (-1); --pos)
    {
        battler = GetDeckBattlerAtPosition(side, pos);
        if (gDeckBattleMons[battler].species != SPECIES_NONE && gDeckBattleMons[battler].hp != 0)
            return pos;
    }

    return POSITIONS_COUNT;
}

enum BattlePosition GetNonAttackerOnLeft(u32 side, enum BattlePosition position)
{
    enum BattleId battler;
    if (position == POSITION_0)
        return POSITIONS_COUNT;

    for (enum BattlePosition pos = position - 1; pos >= POSITION_0 && pos != (-1); --pos)
    {
        battler = GetDeckBattlerAtPosition(side, pos);
        if (gDeckBattleMons[battler].species != SPECIES_NONE && gDeckBattleMons[battler].hp != 0 && battler != gBattlerAttacker)
            return pos;
    }

    return POSITIONS_COUNT;
}

enum BattlePosition GetToMoveOnLeft(u32 side, enum BattlePosition position)
{
    enum BattleId battler;
    if (position == POSITION_0)
        return POSITIONS_COUNT;

    for (enum BattlePosition pos = position - 1; pos >= POSITION_0 && pos != (-1); --pos)
    {
        battler = GetDeckBattlerAtPosition(side, pos);
        if (!gDeckBattleMons[battler].hasMoved && gDeckBattleMons[battler].hp != 0)
            return pos;
    }

    return POSITIONS_COUNT;
}

enum BattlePosition GetOccupiedOnRight(u32 side, enum BattlePosition position)
{
    enum BattleId battler;
    for (enum BattlePosition pos = position + 1; pos < POSITIONS_COUNT; ++pos)
    {
        battler = GetDeckBattlerAtPosition(side, pos);
        if (gDeckBattleMons[battler].species != SPECIES_NONE && gDeckBattleMons[battler].hp != 0)
            return pos;
    }
    return POSITIONS_COUNT;
}

enum BattlePosition GetNonAttackerOnRight(u32 side, enum BattlePosition position)
{
    enum BattleId battler;
    for (enum BattlePosition pos = position + 1; pos < POSITIONS_COUNT; ++pos)
    {
        battler = GetDeckBattlerAtPosition(side, pos);
        if (gDeckBattleMons[battler].species != SPECIES_NONE && gDeckBattleMons[battler].hp != 0 && battler != gBattlerAttacker)
            return pos;
    }
    return POSITIONS_COUNT;
}

enum BattlePosition GetToMoveOnRight(u32 side, enum BattlePosition position)
{
    enum BattleId battler;
    for (enum BattlePosition pos = position + 1; pos < POSITIONS_COUNT; ++pos)
    {
        battler = GetDeckBattlerAtPosition(side, pos);
        if (!gDeckBattleMons[battler].hasMoved && gDeckBattleMons[battler].hp != 0)
            return pos;
    }
    return POSITIONS_COUNT;
}
