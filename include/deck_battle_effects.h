#ifndef GUARD_DECK_BATTLE_EFFECTS_H
#define GUARD_DECK_BATTLE_EFFECTS_H

enum DeckMoveEffect
{
    DECK_EFFECT_HIT,
    DECK_EFFECT_HIT_ALL_OPPONENTS,
    DECK_EFFECT_POWER_UP,
    DECK_EFFECT_COUNT,
};

void Task_ExecuteSwap(u8 taskId);

extern void (*const gMoveEffectTasks[DECK_EFFECT_COUNT])(u8 taskId);

#endif
