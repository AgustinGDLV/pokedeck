#ifndef GUARD_DECK_BATTLE_UTIL_H
#define GUARD_DECK_BATTLE_UTIL_H

u32 GetDeckBattlerSide(enum BattleId battler);
u32 GetDeckBattlerAtPosition(u32 side, enum BattlePosition position);
enum BattlePosition GetLeftmostPositionToMove(u32 side);
enum BattlePosition GetPositionToMoveOnLeft(u32 side, enum BattlePosition position);
enum BattlePosition GetPositionToMoveOnRight(u32 side, enum BattlePosition position);

#endif
