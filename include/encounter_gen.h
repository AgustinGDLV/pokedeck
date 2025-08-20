#ifndef GUARD_ENCOUNTER_GEN_H
#define GUARD_ENCOUNTER_GEN_H

#define MAX_ENCOUNTER_VARIETIES 3
#define MAX_ENCOUNTER_PARTIES   3

struct EncounterParty
{
    u16 species[6];
    u16 weight;
    u16 levelRange[2]; // low, high
};

struct Encounter
{
    u16 graphicsId;
    struct EncounterParty parties[MAX_ENCOUNTER_PARTIES];
    u16 partyCount;
    u16 weight;
};

struct EncounterSet
{
    struct Encounter encounters[MAX_ENCOUNTER_VARIETIES];
    u32 count;
};

u32 GetEncounterGraphicsIdByLocalId(u32 localId);

#endif
