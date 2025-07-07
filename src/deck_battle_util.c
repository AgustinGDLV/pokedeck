#include "global.h"
#include "battle.h"
#include "deck_battle.h"
#include "deck_battle_interface.h"
#include "deck_battle_util.h"

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