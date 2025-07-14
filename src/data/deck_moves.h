const struct DeckMoveInfo gDeckMovesInfo[MOVES_COUNT] =
{
    [MOVE_TACKLE] =
    {
        .name = COMPOUND_STRING("Tackle"),
        .description = COMPOUND_STRING("Damages one opponent."),
        .target = MOVE_TARGET_SINGLE_OPPONENT,
        .effect = DECK_EFFECT_HIT,
    },

    [MOVE_VINE_WHIP] =
    {
        .name = COMPOUND_STRING("Vine Whip"),
        .description = COMPOUND_STRING("Damages one opponent."),
        .target = MOVE_TARGET_SINGLE_OPPONENT,
        .effect = DECK_EFFECT_HIT,
    },

    [MOVE_HELPING_HAND] =
    {
        .name = COMPOUND_STRING("Helping Hand"),
        .description = COMPOUND_STRING("Powers up left ally."),
        .target = MOVE_TARGET_LEFT_ALLY,
        .effect = DECK_EFFECT_POWER_UP,
    },

    [MOVE_SURF] =
    {
        .name = COMPOUND_STRING("Surf"),
        .description = COMPOUND_STRING("Damages all opponents."),
        .target = MOVE_TARGET_ALL_OPPONENTS,
        .effect = DECK_EFFECT_HIT,
    },
};
