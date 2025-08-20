const struct EncounterSet gEncountersInfo[MAPSEC_COUNT][16] =
{
    [MAPSEC_ROUTE_1] = // Route 1
    {
        [1] = // nook by the beach
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
                    .graphicsId = OBJ_EVENT_GFX_SPECIES(SLOWPOKE),
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
            },
            .count = 1,
        },
    }
};
