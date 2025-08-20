const struct DeckSpeciesInfo gDeckSpeciesInfo[NUM_SPECIES] =
{
    [SPECIES_BELLSPROUT] =
    {
        .baseHP = 50,
        .basePower = 75,
        .baseDef = 35,
        .move = MOVE_VINE_WHIP,
        .ability = ABILITY_NONE,

        .playerIdle = gBellsproutPlayerIdleGfx,
        .opponentIdle = gBellsproutOpponentIdleGfx,
        .playerAttack = gBellsproutPlayerAttackGfx,
        .opponentAttack = gBellsproutOpponentAttackGfx,
        .playerHurt = gBellsproutPlayerHurtGfx,
        .opponentHurt = gBellsproutOpponentHurtGfx,
        .objectPalette = gBellsproutObjectPal,
        .playerYOffset = 0,
        .opponentYOffset = 1,

        .portrait = gBellsproutPortraitGfx,
        .portraitPalette = gBellsproutPortraitPal,
    },

    [SPECIES_ZUBAT] =
    {
        .baseHP = 50,
        .basePower = 75,
        .baseDef = 35,
        .move = MOVE_TACKLE,
        .ability = ABILITY_NONE,

        .playerIdle = gZubatPlayerIdleGfx,
        .opponentIdle = gZubatOpponentIdleGfx,
        .playerAttack = gZubatPlayerAttackGfx,
        .opponentAttack = gZubatOpponentAttackGfx,
        .playerHurt = gZubatPlayerHurtGfx,
        .opponentHurt = gZubatOpponentHurtGfx,
        .objectPalette = gZubatObjectPal,
        .playerYOffset = -2,
        .opponentYOffset = -2,

        .portrait = gZubatPortraitGfx,
        .portraitPalette = gZubatPortraitPal,
    },

    [SPECIES_SLOWPOKE] =
    {
        .baseHP = 90,
        .basePower = 55,
        .baseDef = 55,
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
        .baseHP = 95,
        .basePower = 90,
        .baseDef = 95,
        .move = MOVE_SURF,
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

    [SPECIES_KRABBY] =
    {
        .baseHP = 50,
        .basePower = 75,
        .baseDef = 35,
        .move = MOVE_CRABHAMMER,
        .ability = ABILITY_NONE,

        .playerIdle = gKrabbyPlayerIdleGfx,
        .opponentIdle = gKrabbyOpponentIdleGfx,
        .playerAttack = gKrabbyPlayerAttackGfx,
        .opponentAttack = gKrabbyOpponentAttackGfx,
        .playerHurt = gKrabbyPlayerHurtGfx,
        .opponentHurt = gKrabbyOpponentHurtGfx,
        .objectPalette = gKrabbyObjectPal,
        .playerYOffset = 1,
        .opponentYOffset = 1,

        .portrait = gKrabbyPortraitGfx,
        .portraitPalette = gKrabbyPortraitPal,
    },

    [SPECIES_MAREEP] =
    {
        .baseHP = 55,
        .basePower = 55,
        .baseDef = 45,
        .move = MOVE_HELPING_HAND,
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

    [SPECIES_CLEFFA] =
    {
        .baseHP = 35,
        .basePower = 80,
        .baseDef = 25,
        .move = MOVE_HELPING_HAND,
        .ability = ABILITY_NONE,

        .playerIdle = gCleffaPlayerIdleGfx,
        .opponentIdle = gCleffaOpponentIdleGfx,
        .playerAttack = gCleffaPlayerAttackGfx,
        .opponentAttack = gCleffaOpponentAttackGfx,
        .playerHurt = gCleffaPlayerHurtGfx,
        .opponentHurt = gCleffaOpponentHurtGfx,
        .objectPalette = gCleffaObjectPal,
        .playerYOffset = 0,
        .opponentYOffset = 0,

        .portrait = gCleffaPortraitGfx,
        .portraitPalette = gCleffaPortraitPal,
    },

    [SPECIES_SWINUB] =
    {
        .baseHP = 50,
        .basePower = 40,
        .baseDef = 40,
        .move = MOVE_EARTHQUAKE,
        .ability = ABILITY_NONE,

        .playerIdle = gSwinubPlayerIdleGfx,
        .opponentIdle = gSwinubOpponentIdleGfx,
        .playerAttack = gSwinubPlayerAttackGfx,
        .opponentAttack = gSwinubOpponentAttackGfx,
        .playerHurt = gSwinubPlayerHurtGfx,
        .opponentHurt = gSwinubOpponentHurtGfx,
        .objectPalette = gSwinubObjectPal,
        .playerYOffset = 0,
        .opponentYOffset = -1,

        .portrait = gSwinubPortraitGfx,
        .portraitPalette = gSwinubPortraitPal,
    },

    [SPECIES_SWABLU] =
    {
        .baseHP = 45,
        .basePower = 40,
        .baseDef = 70,
        .move = MOVE_HELPING_HAND,
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

    [SPECIES_CHINGLING] =
    {
        .baseHP = 50,
        .basePower = 75,
        .baseDef = 35,
        .move = MOVE_HELPING_HAND,
        .ability = ABILITY_NONE,

        .playerIdle = gChinglingPlayerIdleGfx,
        .opponentIdle = gChinglingOpponentIdleGfx,
        .playerAttack = gChinglingPlayerAttackGfx,
        .opponentAttack = gChinglingOpponentAttackGfx,
        .playerHurt = gChinglingPlayerHurtGfx,
        .opponentHurt = gChinglingOpponentHurtGfx,
        .objectPalette = gChinglingObjectPal,
        .playerYOffset = 0,
        .opponentYOffset = 0,

        .portrait = gChinglingPortraitGfx,
        .portraitPalette = gChinglingPortraitPal,
    },

    [SPECIES_APPLIN] =
    {
        .baseHP = 40,
        .basePower = 40,
        .baseDef = 60,
        .move = MOVE_TACKLE,
        .ability = ABILITY_NONE,

        .playerIdle = gApplinPlayerIdleGfx,
        .opponentIdle = gApplinOpponentIdleGfx,
        .playerAttack = gApplinPlayerAttackGfx,
        .opponentAttack = gApplinOpponentAttackGfx,
        .playerHurt = gApplinPlayerHurtGfx,
        .opponentHurt = gApplinOpponentHurtGfx,
        .objectPalette = gApplinObjectPal,
        .playerYOffset = 0,
        .opponentYOffset = -1,

        .portrait = gApplinPortraitGfx,
        .portraitPalette = gApplinPortraitPal,
    },

    [SPECIES_WIGLETT] =
    {
        .baseHP = 50,
        .basePower = 75,
        .baseDef = 35,
        .move = MOVE_TACKLE,
        .ability = ABILITY_NONE,

        .playerIdle = gWiglettPlayerIdleGfx,
        .opponentIdle = gWiglettOpponentIdleGfx,
        .playerAttack = gWiglettPlayerAttackGfx,
        .opponentAttack = gWiglettOpponentAttackGfx,
        .playerHurt = gWiglettPlayerHurtGfx,
        .opponentHurt = gWiglettOpponentHurtGfx,
        .objectPalette = gWiglettObjectPal,
        .playerYOffset = 3,
        .opponentYOffset = 3,

        .portrait = gWiglettPortraitGfx,
        .portraitPalette = gWiglettPortraitPal,
    },
};
