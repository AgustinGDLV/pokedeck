const struct EncounterSet gEncountersInfo[MAPSEC_COUNT][16] =
{
    // Route 1
    [MAPSEC_ROUTE_1] =
    {
        // nook by the beach
        [1] =
        {
            .encounters = 
            {
                {
                    .graphicsId = OBJ_EVENT_GFX_SPECIES(APPLIN),
                    .parties =
                    {
                        {
                            .species = {SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                        {
                            {SPECIES_NONE, SPECIES_NONE, SPECIES_BELLSPROUT, SPECIES_APPLIN, SPECIES_APPLIN, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                    },
                    .partyCount = 2,
                    .weight = 50,
                },
                {
                    .graphicsId = OBJ_EVENT_GFX_SPECIES(RATTATA),
                    .parties =
                    {
                        {
                            .species = {SPECIES_NONE, SPECIES_SLOWPOKE, SPECIES_NONE, SPECIES_SLOWPOKE, SPECIES_NONE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                        {
                            {SPECIES_NONE, SPECIES_NONE, SPECIES_SLOWPOKE, SPECIES_SLOWBRO, SPECIES_SLOWPOKE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                    },
                    .partyCount = 2,
                    .weight = 50,
                },
            },
            .count = 2,
        },
        // grass near Route 2
        [2] =
        {
            .encounters = 
            {
                {
                    .graphicsId = OBJ_EVENT_GFX_SPECIES(RATTATA),
                    .parties =
                    {
                        {
                            .species = {SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                        {
                            {SPECIES_NONE, SPECIES_NONE, SPECIES_BELLSPROUT, SPECIES_APPLIN, SPECIES_APPLIN, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                    },
                    .partyCount = 2,
                    .weight = 50,
                },
                {
                    .graphicsId = OBJ_EVENT_GFX_SPECIES(POOCHYENA),
                    .parties =
                    {
                        {
                            .species = {SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                        {
                            {SPECIES_NONE, SPECIES_NONE, SPECIES_BELLSPROUT, SPECIES_APPLIN, SPECIES_APPLIN, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                    },
                    .partyCount = 2,
                    .weight = 50,
                },
            },
            .count = 2,
        },
        // beach
        [3] =
        {
            .encounters = 
            {
                {
                    .graphicsId = OBJ_EVENT_GFX_SPECIES(KRABBY),
                    .parties =
                    {
                        {
                            .species = {SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                        {
                            {SPECIES_NONE, SPECIES_NONE, SPECIES_BELLSPROUT, SPECIES_APPLIN, SPECIES_APPLIN, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                    },
                    .partyCount = 2,
                    .weight = 50,
                },
                {
                    .graphicsId = OBJ_EVENT_GFX_SPECIES(SLOWPOKE),
                    .parties =
                    {
                        {
                            .species = {SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                        {
                            {SPECIES_NONE, SPECIES_NONE, SPECIES_BELLSPROUT, SPECIES_APPLIN, SPECIES_APPLIN, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                    },
                    .partyCount = 2,
                    .weight = 50,
                },
            },
            .count = 2,
        },
        // nook near Route 2
        [4] =
        {
            .encounters = 
            {
                {
                    .graphicsId = OBJ_EVENT_GFX_SPECIES(APPLIN),
                    .parties =
                    {
                        {
                            .species = {SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                        {
                            {SPECIES_NONE, SPECIES_NONE, SPECIES_BELLSPROUT, SPECIES_APPLIN, SPECIES_APPLIN, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                    },
                    .partyCount = 2,
                    .weight = 50,
                },
                {
                    .graphicsId = OBJ_EVENT_GFX_SPECIES(SWABLU),
                    .parties =
                    {
                        {
                            .species = {SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                        {
                            {SPECIES_NONE, SPECIES_NONE, SPECIES_BELLSPROUT, SPECIES_APPLIN, SPECIES_APPLIN, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                    },
                    .partyCount = 2,
                    .weight = 50,
                },
            },
            .count = 2,
        },
        // near Starting Town
        [5] =
        {
            .encounters = 
            {
                {
                    .graphicsId = OBJ_EVENT_GFX_SPECIES(SPEAROW),
                    .parties =
                    {
                        {
                            .species = {SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                        {
                            {SPECIES_NONE, SPECIES_NONE, SPECIES_BELLSPROUT, SPECIES_APPLIN, SPECIES_APPLIN, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                    },
                    .partyCount = 2,
                    .weight = 50,
                },
                {
                    .graphicsId = OBJ_EVENT_GFX_SPECIES(RATTATA),
                    .parties =
                    {
                        {
                            .species = {SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                        {
                            {SPECIES_NONE, SPECIES_NONE, SPECIES_BELLSPROUT, SPECIES_APPLIN, SPECIES_APPLIN, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                    },
                    .partyCount = 2,
                    .weight = 50,
                },
                {
                    .graphicsId = OBJ_EVENT_GFX_SPECIES(MAREEP),
                    .parties =
                    {
                        {
                            .species = {SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                        {
                            {SPECIES_NONE, SPECIES_NONE, SPECIES_BELLSPROUT, SPECIES_APPLIN, SPECIES_APPLIN, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                    },
                    .partyCount = 2,
                    .weight = 50,
                },
            },
            .count = 3,
        },
        // near Ruins
        [6] =
        {
            .encounters = 
            {
                {
                    .graphicsId = OBJ_EVENT_GFX_SPECIES(SPEAROW),
                    .parties =
                    {
                        {
                            .species = {SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                        {
                            {SPECIES_NONE, SPECIES_NONE, SPECIES_BELLSPROUT, SPECIES_APPLIN, SPECIES_APPLIN, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                    },
                    .partyCount = 2,
                    .weight = 50,
                },
                {
                    .graphicsId = OBJ_EVENT_GFX_SPECIES(POOCHYENA),
                    .parties =
                    {
                        {
                            .species = {SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                        {
                            {SPECIES_NONE, SPECIES_NONE, SPECIES_BELLSPROUT, SPECIES_APPLIN, SPECIES_APPLIN, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                    },
                    .partyCount = 2,
                    .weight = 50,
                },
            },
            .count = 2,
        },
    },
    // Ruins
    [MAPSEC_TANOBY_RUINS] =
    {
        // lower level bottom
        [1] =
        {
            .encounters = 
            {
                {
                    .graphicsId = OBJ_EVENT_GFX_SPECIES(VULPIX),
                    .parties =
                    {
                        {
                            .species = {SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                        {
                            {SPECIES_NONE, SPECIES_NONE, SPECIES_BELLSPROUT, SPECIES_APPLIN, SPECIES_APPLIN, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                    },
                    .partyCount = 2,
                    .weight = 50,
                },
                {
                    .graphicsId = OBJ_EVENT_GFX_SPECIES(CLEFFA),
                    .parties =
                    {
                        {
                            .species = {SPECIES_NONE, SPECIES_SLOWPOKE, SPECIES_NONE, SPECIES_SLOWPOKE, SPECIES_NONE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                        {
                            {SPECIES_NONE, SPECIES_NONE, SPECIES_SLOWPOKE, SPECIES_SLOWBRO, SPECIES_SLOWPOKE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                    },
                    .partyCount = 2,
                    .weight = 50,
                },
            },
            .count = 2,
        },
        // upper level left
        [2] =
        {
            .encounters = 
            {
                {
                    .graphicsId = OBJ_EVENT_GFX_SPECIES(CLEFFA),
                    .parties =
                    {
                        {
                            .species = {SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                        {
                            {SPECIES_NONE, SPECIES_NONE, SPECIES_BELLSPROUT, SPECIES_APPLIN, SPECIES_APPLIN, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                    },
                    .partyCount = 2,
                    .weight = 50,
                },
                {
                    .graphicsId = OBJ_EVENT_GFX_SPECIES(BALTOY),
                    .parties =
                    {
                        {
                            .species = {SPECIES_NONE, SPECIES_SLOWPOKE, SPECIES_NONE, SPECIES_SLOWPOKE, SPECIES_NONE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                        {
                            {SPECIES_NONE, SPECIES_NONE, SPECIES_SLOWPOKE, SPECIES_SLOWBRO, SPECIES_SLOWPOKE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                    },
                    .partyCount = 2,
                    .weight = 50,
                },
                {
                    .graphicsId = OBJ_EVENT_GFX_SPECIES(CHINGLING),
                    .parties =
                    {
                        {
                            .species = {SPECIES_NONE, SPECIES_SLOWPOKE, SPECIES_NONE, SPECIES_SLOWPOKE, SPECIES_NONE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                        {
                            {SPECIES_NONE, SPECIES_NONE, SPECIES_SLOWPOKE, SPECIES_SLOWBRO, SPECIES_SLOWPOKE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                    },
                    .partyCount = 2,
                    .weight = 50,
                },
            },
            .count = 3,
        },
        // upper level right
        [3] =
        {
            .encounters = 
            {
                {
                    .graphicsId = OBJ_EVENT_GFX_SPECIES(CHINGLING),
                    .parties =
                    {
                        {
                            .species = {SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                        {
                            {SPECIES_NONE, SPECIES_NONE, SPECIES_BELLSPROUT, SPECIES_APPLIN, SPECIES_APPLIN, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                    },
                    .partyCount = 2,
                    .weight = 50,
                },
                {
                    .graphicsId = OBJ_EVENT_GFX_SPECIES(VULPIX),
                    .parties =
                    {
                        {
                            .species = {SPECIES_NONE, SPECIES_SLOWPOKE, SPECIES_NONE, SPECIES_SLOWPOKE, SPECIES_NONE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                        {
                            {SPECIES_NONE, SPECIES_NONE, SPECIES_SLOWPOKE, SPECIES_SLOWBRO, SPECIES_SLOWPOKE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                    },
                    .partyCount = 2,
                    .weight = 50,
                },
            },
            .count = 2,
        },
        // middle level nook
        [4] =
        {
            .encounters = 
            {
                {
                    .graphicsId = OBJ_EVENT_GFX_SPECIES(VULPIX),
                    .parties =
                    {
                        {
                            .species = {SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_APPLIN, SPECIES_NONE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                        {
                            {SPECIES_NONE, SPECIES_NONE, SPECIES_BELLSPROUT, SPECIES_APPLIN, SPECIES_APPLIN, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                    },
                    .partyCount = 2,
                    .weight = 50,
                },
                {
                    .graphicsId = OBJ_EVENT_GFX_SPECIES(CLEFFA),
                    .parties =
                    {
                        {
                            .species = {SPECIES_NONE, SPECIES_SLOWPOKE, SPECIES_NONE, SPECIES_SLOWPOKE, SPECIES_NONE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                        {
                            {SPECIES_NONE, SPECIES_NONE, SPECIES_SLOWPOKE, SPECIES_SLOWBRO, SPECIES_SLOWPOKE, SPECIES_NONE},
                            .weight = 50,
                            .levelRange = {4, 6},
                        },
                    },
                    .partyCount = 2,
                    .weight = 50,
                },
            },
            .count = 2,
        },
    },
};
