const struct DeckMoveInfo gDeckMovesInfo[MOVES_COUNT] =
{
    [MOVE_TACKLE] =
    {
        .name = COMPOUND_STRING("TACKLE"),
        .description = COMPOUND_STRING("Damages one opponent."),
        .power = 40,
        .target = MOVE_TARGET_SINGLE_OPPONENT,
        .effect = DECK_EFFECT_HIT,
    },

    [MOVE_VINE_WHIP] =
    {
        .name = COMPOUND_STRING("VINE WHIP"),
        .description = COMPOUND_STRING("Damages one opponent."),
        .power = 60,
        .target = MOVE_TARGET_SINGLE_OPPONENT,
        .effect = DECK_EFFECT_HIT,
    },

    [MOVE_HELPING_HAND] =
    {
        .name = COMPOUND_STRING("HELPING HAND"),
        .description = COMPOUND_STRING("Powers up left ally."),
        .power = 0,
        .target = MOVE_TARGET_LEFT_ALLY,
        .effect = DECK_EFFECT_POWER_UP,
    },

    [MOVE_SURF] =
    {
        .name = COMPOUND_STRING("SURF"),
        .description = COMPOUND_STRING("Damages all opponents."),
        .power = 60,
        .target = MOVE_TARGET_ALL_OPPONENTS,
        .effect = DECK_EFFECT_HIT_ALL_OPPONENTS,
    },

    [MOVE_EARTHQUAKE] =
    {
        .name = COMPOUND_STRING("EARTHQUAKE"),
        .description = COMPOUND_STRING("Damages all opponents."),
        .power = 60,
        .target = MOVE_TARGET_ALL_OPPONENTS_ADJACENT_ALLIES,
        .effect = DECK_EFFECT_HIT_ALL_OPPONENTS_ADJACENT_ALLIES,
    },
};
