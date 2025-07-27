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

u32 GetDeckBattlerAtPos(u32 side, enum BattlePosition position)
{
    if (side == B_SIDE_PLAYER)
    {
        for (enum BattleId battler = B_PLAYER_0; battler < B_OPPONENT_0; ++battler)
            if (gDeckMons[battler].species != SPECIES_NONE && gDeckMons[battler].pos == position)
                return battler;
    }
    else
    {
        for (enum BattleId battler = B_OPPONENT_0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
            if (gDeckMons[battler].species != SPECIES_NONE && gDeckMons[battler].pos == position)
                return battler;
    }
    return MAX_DECK_BATTLERS_COUNT; // no luck
}

enum BattlePosition GetLeftmostOccupiedPosition(u32 side)
{
    enum BattleId battler;
    for (enum BattlePosition pos = POSITION_0; pos < POSITIONS_COUNT; ++pos)
    {
        battler = GetDeckBattlerAtPos(side, pos);
        if (gDeckMons[battler].species != SPECIES_NONE && gDeckMons[battler].hp != 0)
            return pos;
    }
    return POSITION_0; // no luck
}

enum BattlePosition GetLeftmostPositionToMove(u32 side)
{
    enum BattleId battler;
    for (enum BattlePosition pos = POSITION_0; pos < POSITIONS_COUNT; ++pos)
    {
        battler = GetDeckBattlerAtPos(side, pos);
        if (!gDeckMons[battler].hasMoved && gDeckMons[battler].hp != 0)
            return pos;
    }
    return POSITIONS_COUNT; // no luck
}

enum BattlePosition GetOccupiedOnLeft(u32 side, enum BattlePosition position)
{
    enum BattleId battler;
    if (position == POSITION_0)
        return POSITIONS_COUNT;

    for (enum BattlePosition pos = position - 1; pos >= POSITION_0 && pos != (-1); --pos)
    {
        battler = GetDeckBattlerAtPos(side, pos);
        if (gDeckMons[battler].species != SPECIES_NONE && gDeckMons[battler].hp != 0)
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
        battler = GetDeckBattlerAtPos(side, pos);
        if (gDeckMons[battler].species != SPECIES_NONE && gDeckMons[battler].hp != 0 && battler != gBattlerAttacker)
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
        battler = GetDeckBattlerAtPos(side, pos);
        if (!gDeckMons[battler].hasMoved && gDeckMons[battler].hp != 0)
            return pos;
    }

    return POSITIONS_COUNT;
}

enum BattlePosition GetOccupiedOnRight(u32 side, enum BattlePosition position)
{
    enum BattleId battler;
    for (enum BattlePosition pos = position + 1; pos < POSITIONS_COUNT; ++pos)
    {
        battler = GetDeckBattlerAtPos(side, pos);
        if (gDeckMons[battler].species != SPECIES_NONE && gDeckMons[battler].hp != 0)
            return pos;
    }
    return POSITIONS_COUNT;
}

enum BattlePosition GetNonAttackerOnRight(u32 side, enum BattlePosition position)
{
    enum BattleId battler;
    for (enum BattlePosition pos = position + 1; pos < POSITIONS_COUNT; ++pos)
    {
        battler = GetDeckBattlerAtPos(side, pos);
        if (gDeckMons[battler].species != SPECIES_NONE && gDeckMons[battler].hp != 0 && battler != gBattlerAttacker)
            return pos;
    }
    return POSITIONS_COUNT;
}

enum BattlePosition GetToMoveOnRight(u32 side, enum BattlePosition position)
{
    enum BattleId battler;
    for (enum BattlePosition pos = position + 1; pos < POSITIONS_COUNT; ++pos)
    {
        battler = GetDeckBattlerAtPos(side, pos);
        if (!gDeckMons[battler].hasMoved && gDeckMons[battler].hp != 0)
            return pos;
    }
    return POSITIONS_COUNT;
}

enum BattleId GetRandomBattlerOnSide(u32 side)
{
    enum BattleId battler;
    enum BattleId ids[PARTY_SIZE];
    u32 occupiedCount = 0;

    for (enum BattlePosition pos = POSITION_0; pos < POSITIONS_COUNT; ++pos)
    {
        battler = GetDeckBattlerAtPos(side, pos);
        if (!gDeckMons[battler].hasMoved && gDeckMons[battler].hp != 0)
        {
            ids[occupiedCount] = battler;
            ++occupiedCount;
        }
    }

    return ids[Random() % occupiedCount];
}
