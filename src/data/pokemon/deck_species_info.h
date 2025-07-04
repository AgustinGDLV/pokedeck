const struct DeckSpeciesInfo gSpeciesDeckInfo[NUM_SPECIES] =
{
    [SPECIES_SLOWPOKE] =
    {
        .baseHP = 100,
        .basePWR = 10,
        .move = MOVE_TACKLE,
        .ability = ABILITY_NONE,

        .playerIdle = gSlowpokePlayerIdleGfx,
        .opponentIdle = gSlowpokeOpponentIdleGfx,
        .playerAttack = gSlowpokePlayerAttackGfx,
        .opponentAttack = NULL,
        .playerHurt = NULL,
        .opponentHurt = NULL,
        .objectPalette = gSlowpokeObjectPal,

        .portrait = gSlowpokePortraitGfx,
        .portraitPalette = gSlowpokePortraitPal,
    },
};
