#include "global.h"
#include "encounter_gen.h"
#include "event_data.h"
#include "pokemon.h"
#include "random.h"
#include "constants/event_objects.h"
#include "constants/pokemon.h"
#include "constants/species.h"

static u32 GetEncounterTotalWeight(const struct EncounterSet *set);
static u32 GetEncounterVariety(const struct EncounterSet *set);

#include "data/encounters.h"

static u32 GetEncounterTotalWeight(const struct EncounterSet *set)
{
    u32 total = 0;
    for (u32 i = 0; i < set->count; ++i)
    {
        total += set->encounters[i].weight;
    }
    return total;
}

static u32 GetEncounterVariety(const struct EncounterSet *set)
{
    u32 weight = Random() % GetEncounterTotalWeight(set);
    u32 index = 0;

    while (weight > set->encounters[index].weight && index < set->count)
    {
        weight -= set->encounters[index].weight;
        ++index;
    }
    return index;
}

u32 GetEncounterGraphicsIdByLocalId(u32 localId)
{
    const struct EncounterSet *set = &gEncountersInfo[gMapHeader.regionMapSectionId][localId];

    // Safety check.
    if (set->count == 0)
        return OBJ_EVENT_GFX_SPECIES(PORYGON);

    u32 index = GetEncounterVariety(set);
    return set->encounters[index].graphicsId;
}

static const struct Encounter *GetEncounterFromSetBySpecies(const struct EncounterSet *set, u16 species)
{
    for (u32 i = 0; i < MAX_ENCOUNTER_VARIETIES; ++i)
    {
        if (set->encounters[i].graphicsId == species + OBJ_EVENT_MON)
            return &set->encounters[i];
    }
    return &set->encounters[0]; // failsafe
}

static u32 GetPartyTotalWeight(const struct Encounter *encounter)
{
    u32 total = 0;
    for (u32 i = 0; i < encounter->partyCount; ++i)
    {
        total += encounter->parties[i].weight;
    }
    return total;
}

static const struct EncounterParty *GetPartyFromEncounter(const struct Encounter *encounter)
{
    u32 weight = Random() % GetPartyTotalWeight(encounter);
    u32 index = 0;

    while (weight > encounter->parties[index].weight && index < encounter->partyCount)
    {
        weight -= encounter->parties[index].weight;
        ++index;
    }
    return &encounter->parties[index];
}

void InitEnemyPartyFromEncounter(void) // used by callnative
{
    const struct EncounterSet *set = &gEncountersInfo[gMapHeader.regionMapSectionId][gSpecialVar_0x8001];
    const struct Encounter *encounter = GetEncounterFromSetBySpecies(set, gSpecialVar_0x8000);
    const struct EncounterParty *party = GetPartyFromEncounter(encounter);

    u32 level;
    for (u32 i = 0; i < PARTY_SIZE; ++i)
    {
        level = Random() % (party->levelRange[1] - party->levelRange[0]) + party->levelRange[0];
        CreateMon(&gEnemyParty[i], party->species[i], level, USE_RANDOM_IVS, 0, 0, OT_ID_PLAYER_ID, 0);
        SetMonData(&gEnemyParty[i], MON_DATA_POSITION, &i);
    }
}
