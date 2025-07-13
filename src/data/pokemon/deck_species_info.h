const struct DeckSpeciesInfo gDeckSpeciesInfo[NUM_SPECIES] =
{
    [SPECIES_BELLSPROUT] =
    {
        .baseHP = 80,
        .basePWR = 12,
        .move = MOVE_TACKLE,
        .ability = ABILITY_NONE,

        .playerIdle = gBellsproutPlayerIdleGfx,
        .opponentIdle = gBellsproutOpponentIdleGfx,
        .playerAttack = gBellsproutPlayerAttackGfx,
        .opponentAttack = gBellsproutOpponentAttackGfx,
        .playerHurt = gBellsproutPlayerHurtGfx,
        .opponentHurt = gBellsproutOpponentHurtGfx,
        .objectPalette = gBellsproutObjectPal,
        .playerYOffset = 0,
        .opponentYOffset = 0,

        .portrait = gBellsproutPortraitGfx,
        .portraitPalette = gBellsproutPortraitPal,
    },

    [SPECIES_SLOWPOKE] =
    {
        .baseHP = 100,
        .basePWR = 10,
        .move = MOVE_TACKLE,
        .ability = ABILITY_NONE,

        .playerIdle = gSlowpokePlayerIdleGfx,
        .opponentIdle = gSlowpokeOpponentIdleGfx,
        .playerAttack = gSlowpokePlayerAttackGfx,
        .opponentAttack = gSlowpokeOpponentAttackGfx,
        .playerHurt = gSlowpokePlayerHurtGfx,
        .opponentHurt = gSlowpokeOpponentHurtGfx,
        .objectPalette = gSlowpokeObjectPal,
        .playerYOffset = -1,
        .opponentYOffset = -1,

        .portrait = gSlowpokePortraitGfx,
        .portraitPalette = gSlowpokePortraitPal,
    },

    [SPECIES_SLOWBRO] =
    {
        .baseHP = 120,
        .basePWR = 15,
        .move = MOVE_HEADBUTT,
        .ability = ABILITY_NONE,

        .playerIdle = gSlowbroPlayerIdleGfx,
        .opponentIdle = gSlowbroOpponentIdleGfx,
        .playerAttack = gSlowbroPlayerAttackGfx,
        .opponentAttack = gSlowbroOpponentAttackGfx,
        .playerHurt = gSlowbroPlayerHurtGfx,
        .opponentHurt = gSlowbroOpponentHurtGfx,
        .objectPalette = gSlowbroObjectPal,
        .playerYOffset = 0,
        .opponentYOffset = -4,

        .portrait = gSlowbroPortraitGfx,
        .portraitPalette = gSlowbroPortraitPal,
    },

    [SPECIES_MAREEP] =
    {
        .baseHP = 90,
        .basePWR = 10,
        .move = MOVE_TACKLE,
        .ability = ABILITY_NONE,

        .playerIdle = gMareepPlayerIdleGfx,
        .opponentIdle = gMareepOpponentIdleGfx,
        .playerAttack = gMareepPlayerAttackGfx,
        .opponentAttack = gMareepOpponentAttackGfx,
        .playerHurt = gMareepPlayerHurtGfx,
        .opponentHurt = gMareepOpponentHurtGfx,
        .objectPalette = gMareepObjectPal,
        .playerYOffset = 0,
        .opponentYOffset = 0,

        .portrait = gMareepPortraitGfx,
        .portraitPalette = gMareepPortraitPal,
    },

    [SPECIES_SWABLU] =
    {
        .baseHP = 90,
        .basePWR = 10,
        .move = MOVE_TACKLE,
        .ability = ABILITY_NONE,

        .playerIdle = gSwabluPlayerIdleGfx,
        .opponentIdle = gSwabluOpponentIdleGfx,
        .playerAttack = gSwabluPlayerAttackGfx,
        .opponentAttack = gSwabluOpponentAttackGfx,
        .playerHurt = gSwabluPlayerHurtGfx,
        .opponentHurt = gSwabluOpponentHurtGfx,
        .objectPalette = gSwabluObjectPal,
        .playerYOffset = 0,
        .opponentYOffset = 0,

        .portrait = gSwabluPortraitGfx,
        .portraitPalette = gSwabluPortraitPal,
    },
};
