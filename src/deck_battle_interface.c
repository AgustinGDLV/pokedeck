#include "global.h"
#include "battle_anim.h"
#include "bg.h"
#include "deck_battle.h"
#include "deck_battle_interface.h"
#include "deck_battle_util.h"
#include "decompress.h"
#include "event_object_movement.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "m4a.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "overworld.h"
#include "option_menu.h"
#include "palette.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_icon.h"
#include "random.h"
#include "region_map.h"
#include "save.h"
#include "scanline_effect.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "strings.h"
#include "string_util.h"
#include "international_string_util.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "title_screen.h"
#include "util.h"
#include "window.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/species.h"

/* deck_battle_interface.c
 *
 * This file handles all the graphics for deck battles, i.e.
 * battle boxes and background, player and opponent 'object' sprites,
 * portrait and information displays, and text printing.
 * 
*/

// forward declarations
static void LoadDummySpriteTiles(void);
static void SpriteCB_Cursor(struct Sprite *sprite);
static void SpriteCB_Shadow(struct Sprite *sprite);
static void SpriteCB_BattlerAttack(struct Sprite *sprite);
static void SpriteCB_BattlerHurt(struct Sprite *sprite);

// windows and bgs
enum Windows
{
    WINDOW_BATTLER_INFO,
    WINDOW_HP_PWR,
    WINDOW_MESSAGE,
    WINDOW_COUNT,
};

static const struct WindowTemplate sDeckBattleWinTemplates[WINDOW_COUNT + 1] =
{
    [WINDOW_BATTLER_INFO] =
    {
        .bg = 1,
        .tilemapLeft = 7,
        .tilemapTop = 15,
        .width = 13,
        .height = 4,
        .paletteNum = 0,
        .baseBlock = 1,
    },
    [WINDOW_HP_PWR] =
    {
        .bg = 1,
        .tilemapLeft = 23,
        .tilemapTop = 15,
        .width = 6,
        .height = 4,
        .paletteNum = 0,
        .baseBlock = 1 + 13*4,
    },
    [WINDOW_MESSAGE] =
    {
        .bg = 1,
        .tilemapLeft = 4,
        .tilemapTop = 35,
        .width = 19,
        .height = 4,
        .paletteNum = 0,
        .baseBlock = 1 + 13*4 + 6*4,
    },
    DUMMY_WIN_TEMPLATE,
};

// BG handling is ripped almost exactly from the existing battle engine.
static const struct BgTemplate sDeckBattleBgTemplates[] =
{
    {   // Interface
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 24,
        .screenSize = 2,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0,
    },
    {
        .bg = 1,
        .charBaseIndex = 1,
        .mapBaseIndex = 28,
        .screenSize = 2,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    },
    {
        .bg = 2,
        .charBaseIndex = 1,
        .mapBaseIndex = 30,
        .screenSize = 1,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0,
    },
    {   // Background
        .bg = 3,
        .charBaseIndex = 2,
        .mapBaseIndex = 26,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0,
    },
};

// graphics data
#include "data/graphics/deck_battle.h"

const u8 sDummyObjectGfx[] = INCBIN_U8("graphics/deck_pokemon/slowpoke/player_idle.4bpp");
const u8 sDummyPortraitGfx[] = INCBIN_U8("graphics/deck_pokemon/slowpoke/portrait_normal.4bpp");
const u16 sDummyPal[] = INCBIN_U16("graphics/deck_pokemon/slowpoke/portrait_normal.gbapal");

static const struct SpriteSheet sDummyPortraitSpriteSheet = { sDummyPortraitGfx, sizeof(sDummyPortraitGfx), TAG_PORTRAIT };

static const struct SpriteTemplate sPortraitSpriteTemplate =
{
	.tileTag = TAG_PORTRAIT,
	.paletteTag = TAG_PORTRAIT,
	.oam = &sOamData_Portrait,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct SpriteSheet sDummyObjectSpriteSheets[MAX_DECK_BATTLERS_COUNT] = 
{
    { sDummyObjectGfx, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ },
    { sDummyObjectGfx, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ + 1 },
    { sDummyObjectGfx, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ + 2 },
    { sDummyObjectGfx, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ + 3 },
    { sDummyObjectGfx, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ + 4 },
    { sDummyObjectGfx, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ + 5 },
    { sDummyObjectGfx, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ + 6 },
    { sDummyObjectGfx, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ + 7 },
    { sDummyObjectGfx, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ + 8 },
    { sDummyObjectGfx, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ + 9 },
    { sDummyObjectGfx, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ + 10 },
    { sDummyObjectGfx, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ + 11 },
};

static const struct SpriteTemplate sBattlerSpriteTemplates[MAX_DECK_BATTLERS_COUNT] =
{
    [B_PLAYER_0] =
    {
        .tileTag = TAG_BATTLER_OBJ,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_PLAYER_1] =
    {
        .tileTag = TAG_BATTLER_OBJ + 1,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_PLAYER_2] =
    {
        .tileTag = TAG_BATTLER_OBJ + 2,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_PLAYER_3] =
    {
        .tileTag = TAG_BATTLER_OBJ + 3,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_PLAYER_4] =
    {
        .tileTag = TAG_BATTLER_OBJ + 4,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_PLAYER_5] =
    {
        .tileTag = TAG_BATTLER_OBJ + 5,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_OPPONENT_0] =
    {
        .tileTag = TAG_BATTLER_OBJ + 6,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_OPPONENT_1] =
    {
        .tileTag = TAG_BATTLER_OBJ + 7,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_OPPONENT_2] =
    {
        .tileTag = TAG_BATTLER_OBJ + 8,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_OPPONENT_3] =
    {
        .tileTag = TAG_BATTLER_OBJ + 9,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_OPPONENT_4] =
    {
        .tileTag = TAG_BATTLER_OBJ + 10,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_OPPONENT_5] =
    {
        .tileTag = TAG_BATTLER_OBJ + 11,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
};

// ewram data
EWRAM_DATA struct DeckBattleGraphics gDeckBattleGraphics = {0};

// code
void ClearDeckBattleGraphicsStruct(void)
{
    for (u32 i = 0; i < POSITIONS_COUNT; ++i)
    {
        gDeckBattleGraphics.battlerSpriteIds[B_SIDE_PLAYER] = SPRITE_NONE;
        gDeckBattleGraphics.battlerSpriteIds[B_SIDE_OPPONENT] = SPRITE_NONE;
    }
    gDeckBattleGraphics.portraitSpriteId = SPRITE_NONE;
}

void LoadBattleBoxesAndBackground(void)
{
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sDeckBattleBgTemplates, ARRAY_COUNT(sDeckBattleBgTemplates));

    LZDecompressVram(sDeckBattleInterfaceTiles, (void *)(BG_CHAR_ADDR(0)));
    LZDecompressVram(sDeckBattleInterfaceTilemap, (void *)(BG_SCREEN_ADDR(24)));
    LoadPalette(sDeckBattleInterfacePalette, BG_PLTT_ID(0), PLTT_SIZE_4BPP);

    // TODO: Support more backgrounds.
    LZDecompressVram(sGrassBackgroundTiles, (void *)(BG_CHAR_ADDR(2)));
    LZDecompressVram(sGrassBackgroundTilemap, (void *)(BG_SCREEN_ADDR(26)));
    LoadPalette(sGrassBackgroundPalette, BG_PLTT_ID(1), PLTT_SIZE_4BPP);

    CopyBgTilemapBufferToVram(0);
    CopyBgTilemapBufferToVram(3);
}

void InitDeckBattleWindows(void)
{
    u32 windowId;
    InitWindows(sDeckBattleWinTemplates);
    for (u32 i = 0; i < WINDOW_COUNT; ++i)
    {
        windowId = AddWindow(&sDeckBattleWinTemplates[i]);
        PutWindowTilemap(windowId);
        CopyWindowToVram(i, COPYWIN_FULL);
    }
}

static void LoadDummySpriteTiles(void)
{
    LoadSpriteSheet(&sDummyPortraitSpriteSheet);
    for (u32 i = 0; i < MAX_DECK_BATTLERS_COUNT; ++i)
    {
        LoadSpriteSheet(&sDummyObjectSpriteSheets[i]);
    }
}

#define tTimer  data[0]
#define tActive data[1]
#define tPause  data[2]

void Task_DoBattlerBobEffect(u8 taskId)
{
    u32 battler;
    if (gTasks[taskId].tPause && gTasks[taskId].tTimer == 0)
    {
        gTasks[taskId].tActive = FALSE;
    }
    else
    {
        gTasks[taskId].tActive = TRUE;
        switch (++gTasks[taskId].tTimer)
        {
        // Move left.
        case 16:
            for (battler = B_PLAYER_0; battler < B_OPPONENT_0; ++battler)
                gSprites[gDeckBattleGraphics.battlerSpriteIds[battler]].x2 += 1;
            for (battler = B_OPPONENT_0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
                gSprites[gDeckBattleGraphics.battlerSpriteIds[battler]].x2 -= 1;
            break;
        case 32:
            for (battler = B_PLAYER_0; battler < B_OPPONENT_0; ++battler)
                gSprites[gDeckBattleGraphics.battlerSpriteIds[battler]].x2 += 1;
            for (battler = B_OPPONENT_0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
                gSprites[gDeckBattleGraphics.battlerSpriteIds[battler]].x2 -= 1;
            break;
        case 48:
            for (battler = B_PLAYER_0; battler < B_OPPONENT_0; ++battler)
                gSprites[gDeckBattleGraphics.battlerSpriteIds[battler]].x2 += 1;
            for (battler = B_OPPONENT_0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
                gSprites[gDeckBattleGraphics.battlerSpriteIds[battler]].x2 -= 1;
            break;
        // Pause.
        case 64:
            break;
        // Move right.
        case 80:
            for (battler = B_PLAYER_0; battler < B_OPPONENT_0; ++battler)
                gSprites[gDeckBattleGraphics.battlerSpriteIds[battler]].x2 -= 1;
            for (battler = B_OPPONENT_0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
                gSprites[gDeckBattleGraphics.battlerSpriteIds[battler]].x2 += 1;
            break;
        case 96:
            for (battler = B_PLAYER_0; battler < B_OPPONENT_0; ++battler)
                gSprites[gDeckBattleGraphics.battlerSpriteIds[battler]].x2 -= 1;
            for (battler = B_OPPONENT_0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
                gSprites[gDeckBattleGraphics.battlerSpriteIds[battler]].x2 += 1;
            break;
        case 112:
            for (battler = B_PLAYER_0; battler < B_OPPONENT_0; ++battler)
                gSprites[gDeckBattleGraphics.battlerSpriteIds[battler]].x2 -= 1;
            for (battler = B_OPPONENT_0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
                gSprites[gDeckBattleGraphics.battlerSpriteIds[battler]].x2 += 1;
            break;
        case 128:
            gTasks[taskId].tTimer = 0;
            break;
        }
    }
}

void SetBattlerBobPause(bool32 pause)
{
    gTasks[gDeckBattleGraphics.bobTaskId].tPause = pause;
}

bool32 IsBattlerBobActive(void)
{
    return gTasks[gDeckBattleGraphics.bobTaskId].tActive;
}

#undef tTimer
#undef tActive
#undef tPause

#define sBattlerId  data[0]
#define sCursorId   data[1]
#define sAnimState  data[2]
#define sTimer      data[3]

static void SpriteCB_Shadow(struct Sprite *sprite)
{
    sprite->x2 = gSprites[gDeckBattleGraphics.battlerSpriteIds[sprite->sBattlerId]].x2;
    sprite->y2 = gSprites[gDeckBattleGraphics.battlerSpriteIds[sprite->sBattlerId]].y2;
}

static void SpriteCB_Cursor(struct Sprite *sprite)
{
    sprite->x2 = gSprites[gDeckBattleGraphics.battlerSpriteIds[sprite->sBattlerId]].x2;
    if (++sprite->animDelayCounter > 12)
    {
        sprite->y2 ^= 1;
        sprite->animDelayCounter = 0;
    }
}

void LoadBattlerPortrait(enum BattleId battler)
{
    u32 *dst, *src, index;

    FreeSpritePaletteByTag(TAG_PORTRAIT); // just in case?
    index = LoadSpritePaletteWithTag(gSpeciesDeckInfo[gDeckBattleMons[battler].species].portraitPalette, TAG_PORTRAIT);

    dst = (u32 *)(OBJ_VRAM0 + TILE_OFFSET_4BPP(GetSpriteTileStartByTag(TAG_PORTRAIT)));
    src = (u32 *)gSpeciesDeckInfo[gDeckBattleMons[battler].species].portrait;
    for (u32 i = 0; i < PORTRAIT_SIZE / 4; ++i)
        dst[i] = src[i];

    gSprites[gDeckBattleGraphics.portraitSpriteId].oam.paletteNum = index;
    gSprites[gDeckBattleGraphics.portraitSpriteId].invisible = FALSE;
}

void SetBattlerPortraitVisibility(bool32 visible)
{
    gSprites[gDeckBattleGraphics.portraitSpriteId].invisible = (visible ? FALSE : TRUE);
}

void LoadBattlerObjectSprite(enum BattleId battler)
{
    u32 *dst, *src, index;
    
    FreeSpritePaletteByTag(TAG_BATTLER_OBJ + battler); // just in case?
    index = LoadSpritePaletteWithTag(gSpeciesDeckInfo[gDeckBattleMons[battler].species].objectPalette, TAG_BATTLER_OBJ + battler);

    dst = (u32 *)(OBJ_VRAM0 + TILE_OFFSET_4BPP(GetSpriteTileStartByTag(TAG_BATTLER_OBJ + battler)));
    if (GetDeckBattlerSide(battler) == B_SIDE_PLAYER)
        src = (u32 *)gSpeciesDeckInfo[gDeckBattleMons[battler].species].playerIdle;
    else
        src = (u32 *)gSpeciesDeckInfo[gDeckBattleMons[battler].species].opponentIdle;
    for (u32 i = 0; i < OBJECT_SIZE / 4; ++i)
        dst[i] = src[i];

    gSprites[gDeckBattleGraphics.battlerSpriteIds[battler]].oam.paletteNum = index;
    if (GetDeckBattlerSide(battler) == B_SIDE_PLAYER) // TODO: if ever reloaded, this will continually increase the battler's Y
        gSprites[gDeckBattleGraphics.battlerSpriteIds[battler]].y += gSpeciesDeckInfo[gDeckBattleMons[battler].species].playerYOffset;
    else
        gSprites[gDeckBattleGraphics.battlerSpriteIds[battler]].y += gSpeciesDeckInfo[gDeckBattleMons[battler].species].opponentYOffset;
}

void InitDeckBattleGfx(void)
{
    LoadSpritePalette(&sMiscGfxSpritePalette);
    LoadSpriteSheet(&sShadowSpriteSheet);
    LoadSpriteSheet(&sCursorSpriteSheet);

    LoadDummySpriteTiles();
    gDeckBattleGraphics.portraitSpriteId = CreateSprite(&sPortraitSpriteTemplate, PORTRAIT_X, PORTRAIT_Y, 0);
    for (u32 i = 0; i < MAX_DECK_BATTLERS_COUNT; ++i)
    {
        if (GetDeckBattlerSide(i) == B_SIDE_PLAYER)
        {
            gDeckBattleGraphics.battlerSpriteIds[i] = CreateSprite(&sBattlerSpriteTemplates[i], PLAYER_OBJ_X + OBJ_OFFSET*i, PLAYER_OBJ_Y, 0);
            gDeckBattleGraphics.shadowSpriteIds[i] = CreateSprite(&sShadowSpriteTemplate, PLAYER_OBJ_X + OBJ_OFFSET*i, PLAYER_OBJ_Y, 64);
        }
        else
        {
            gDeckBattleGraphics.battlerSpriteIds[i] = CreateSprite(&sBattlerSpriteTemplates[i], OPPONENT_OBJ_X + OBJ_OFFSET*(i-B_OPPONENT_0), OPPONENT_OBJ_Y, 0);
            gDeckBattleGraphics.shadowSpriteIds[i] = CreateSprite(&sShadowSpriteTemplate, OPPONENT_OBJ_X + OBJ_OFFSET*(i-B_OPPONENT_0), OPPONENT_OBJ_Y, 64);
        }
        gSprites[gDeckBattleGraphics.battlerSpriteIds[i]].sBattlerId = i;
        gSprites[gDeckBattleGraphics.shadowSpriteIds[i]].sBattlerId = i;
    }

    // Blend shadow sprites; may not be the best use of our blend.
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(8, 6));

    CreateTask(Task_DoBattlerBobEffect, 1);
}

void CreateSelectionCursorOverBattler(enum BattleId battler)
{
    struct Sprite *sprite = &gSprites[gDeckBattleGraphics.battlerSpriteIds[battler]];
    sprite->sCursorId = CreateSprite(&sCursorSpriteTemplate, sprite->x, sprite->y - 16, 0);
    gSprites[sprite->sCursorId].sBattlerId = battler;
}

void RemoveSelectionCursorOverBattler(enum BattleId battler)
{
    DestroySprite(&gSprites[gSprites[gDeckBattleGraphics.battlerSpriteIds[battler]].sCursorId]);
    gSprites[gDeckBattleGraphics.battlerSpriteIds[battler]].sCursorId = SPRITE_NONE;
}

void SetBattlerGrayscale(enum BattleId battler, bool32 grayscale)
{
    SetGrayscaleOrOriginalPalette(16 + gSprites[gDeckBattleGraphics.battlerSpriteIds[battler]].oam.paletteNum, (grayscale ? FALSE : TRUE));
}

static void SpriteCB_BattlerAttack(struct Sprite *sprite)
{
    u32 *dst, *src;
    switch (sprite->sAnimState)
    {
    case 0: // Wait for bob to finish.
        if (!IsBattlerBobActive())
        {
            StartSpriteAnim(sprite, ANIM_PAUSED);
            ++sprite->sAnimState;
        }
        break;
    case 1: // Move forward and copy attack sprite tiles.
        switch (++sprite->sTimer)
        {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            if (GetDeckBattlerSide(sprite->sBattlerId) == B_SIDE_PLAYER)
            {
                sprite->x2 += 1;
                sprite->y2 -= 1;
            }
            else
            {
                sprite->x2 -= 1;
                sprite->y2 += 1;
            }
            break;
        case 6:
            dst = (u32 *)(OBJ_VRAM0 + TILE_OFFSET_4BPP(GetSpriteTileStartByTag(TAG_BATTLER_OBJ + sprite->sBattlerId)));
            if (GetDeckBattlerSide(sprite->sBattlerId) == B_SIDE_PLAYER)
                src = (u32 *)gSpeciesDeckInfo[gDeckBattleMons[sprite->sBattlerId].species].playerAttack;
            else
                src = (u32 *)gSpeciesDeckInfo[gDeckBattleMons[sprite->sBattlerId].species].opponentAttack;
            for (u32 i = 0; i < OBJECT_SIZE / 4; ++i)
                dst[i] = src[i];

            StartSpriteAnim(sprite, ANIM_ATTACK);
            sprite->sTimer = 0;
            ++sprite->sAnimState;
            break;
        }
        break;
    case 2: // Play cry on frame 2.
        if (sprite->animCmdIndex == 1)
        {
            PlayCry_Normal(gDeckBattleMons[sprite->sBattlerId].species, 0);
            ++sprite->sAnimState;
        }
        break;
    case 3: // Do shake.
        if (sprite->animCmdIndex == 1)
        {
            sprite->x2 ^= 1;
            sprite->y2 ^= 1;
        }
        else
        {
            sprite->x2 ^= 1; // shake happens an odd number of times, needs to be evened out
            sprite->y2 ^= 1;
            ++sprite->sAnimState;
        }
        break;
    case 4: // Wait for attack anim to finish.
        if (sprite->animEnded)
            ++sprite->sAnimState;
        break;
    case 5: // Move back and copy idle sprite tiles.
        switch (++sprite->sTimer)
        {
        case 4:
            dst = (u32 *)(OBJ_VRAM0 + TILE_OFFSET_4BPP(GetSpriteTileStartByTag(TAG_BATTLER_OBJ + sprite->sBattlerId)));
            if (GetDeckBattlerSide(sprite->sBattlerId) == B_SIDE_PLAYER)
                src = (u32 *)gSpeciesDeckInfo[gDeckBattleMons[sprite->sBattlerId].species].playerIdle;
            else
                src = (u32 *)gSpeciesDeckInfo[gDeckBattleMons[sprite->sBattlerId].species].opponentIdle;
            for (u32 i = 0; i < OBJECT_SIZE / 4; ++i)
                dst[i] = src[i];
            // fall through
        case 8:
        case 12:
        case 16:
        case 20:
            if (GetDeckBattlerSide(sprite->sBattlerId) == B_SIDE_PLAYER)
            {
                sprite->x2 -= 1;
                sprite->y2 += 1;
            }
            else
            {
                sprite->x2 += 1;
                sprite->y2 -= 1;
            }
            break;
        case 24:            
            sprite->sTimer = 0;
            sprite->sAnimState = 0;
            StartSpriteAnim(sprite, ANIM_PAUSED);
            sprite->callback = SpriteCallbackDummy;
            break;
        }
    }
}

static void SpriteCB_BattlerHurt(struct Sprite *sprite)
{
    u32 *dst, *src;
    switch (sprite->sAnimState)
    {
    case 0: // Copy hurt sprite tiles.
        dst = (u32 *)(OBJ_VRAM0 + TILE_OFFSET_4BPP(GetSpriteTileStartByTag(TAG_BATTLER_OBJ + sprite->sBattlerId)));
        if (GetDeckBattlerSide(sprite->sBattlerId) == B_SIDE_PLAYER)
            src = (u32 *)gSpeciesDeckInfo[gDeckBattleMons[sprite->sBattlerId].species].playerHurt;
        else
            src = (u32 *)gSpeciesDeckInfo[gDeckBattleMons[sprite->sBattlerId].species].opponentHurt;
        for (u32 i = 0; i < OBJECT_SIZE / 4; ++i)
            dst[i] = src[i];
        StartSpriteAnim(sprite, ANIM_HURT);
        ++sprite->sAnimState;
        break;
    case 1: // Wait for hurt anim to finish.
        if (sprite->animEnded)
            ++sprite->sAnimState;
        break;
    case 2:
        dst = (u32 *)(OBJ_VRAM0 + TILE_OFFSET_4BPP(GetSpriteTileStartByTag(TAG_BATTLER_OBJ + sprite->sBattlerId)));
        if (GetDeckBattlerSide(sprite->sBattlerId) == B_SIDE_PLAYER)
            src = (u32 *)gSpeciesDeckInfo[gDeckBattleMons[sprite->sBattlerId].species].playerIdle;
        else
            src = (u32 *)gSpeciesDeckInfo[gDeckBattleMons[sprite->sBattlerId].species].opponentIdle;
        for (u32 i = 0; i < OBJECT_SIZE / 4; ++i)
            dst[i] = src[i];

        sprite->sAnimState = 0;
        StartSpriteAnim(sprite, ANIM_PAUSED);
        sprite->callback = SpriteCallbackDummy;
        break;
    }
}

void StartBattlerAnim(enum BattleId battler, u32 animId) // TODO: overwrite sprite tiles?
{
    switch (animId)
    {
        default:
            StartSpriteAnim(&gSprites[gDeckBattleGraphics.battlerSpriteIds[battler]], animId);
            break;
        case ANIM_ATTACK:
            gSprites[gDeckBattleGraphics.battlerSpriteIds[battler]].callback = SpriteCB_BattlerAttack;
            break;
        case ANIM_HURT:
            gSprites[gDeckBattleGraphics.battlerSpriteIds[battler]].callback = SpriteCB_BattlerHurt;
            break;
    }
}

struct Sprite * GetBattlerSprite(enum BattleId battler)
{
    return &gSprites[gDeckBattleGraphics.battlerSpriteIds[battler]];
}

#undef sBattlerId
#undef sCursorId
#undef sAnimState
#undef sTimer

static const u8 sTextColorNormal[] = { 0, 1, 2 };

void PrintBattlerMoveInfo(enum BattleId battler)
{
    StringCopy(gStringVar1, gMovesInfo[gSpeciesDeckInfo[gDeckBattleMons[battler].species].move].name);
    StringAppend(gStringVar1, COMPOUND_STRING(": Damages one\ntarget.")); // TODO: gDeckMovesInfo

    FillWindowPixelBuffer(WINDOW_BATTLER_INFO, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WINDOW_BATTLER_INFO, FONT_SHORT_NARROW, 2, 1, sTextColorNormal, TEXT_SKIP_DRAW, gStringVar1);
    CopyWindowToVram(WINDOW_BATTLER_INFO, COPYWIN_FULL);
}

void PrintBattlerStats(enum BattleId battler)
{
    u8 *strPtr;
    u32 digits = 0;
    ConvertIntToDecimalStringN(gStringVar1, gDeckBattleMons[battler].hp, STR_CONV_MODE_LEFT_ALIGN, 3);
    for (u32 i = gDeckBattleMons[battler].hp; i > 0; i /= 10)
        ++digits;
    strPtr = &gStringVar1[digits];
    *strPtr = CHAR_SLASH;
    ++strPtr;
    ConvertIntToDecimalStringN(strPtr, gDeckBattleMons[battler].maxHP, STR_CONV_MODE_LEFT_ALIGN, 3);
    ConvertIntToDecimalStringN(gStringVar2, gDeckBattleMons[battler].pwr, STR_CONV_MODE_LEFT_ALIGN, 3);

    FillWindowPixelBuffer(WINDOW_HP_PWR, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WINDOW_HP_PWR, FONT_SMALL_NARROW, 0, 0, sTextColorNormal, TEXT_SKIP_DRAW, COMPOUND_STRING("HP"));
    AddTextPrinterParameterized3(WINDOW_HP_PWR, FONT_SMALL_NARROW, 12, 0, sTextColorNormal, TEXT_SKIP_DRAW, gStringVar1);
    AddTextPrinterParameterized3(WINDOW_HP_PWR, FONT_SMALL_NARROW, 0, 12, sTextColorNormal, TEXT_SKIP_DRAW, COMPOUND_STRING("PWR"));
    AddTextPrinterParameterized3(WINDOW_HP_PWR, FONT_SMALL_NARROW, 18, 12, sTextColorNormal, TEXT_SKIP_DRAW, gStringVar2);
    CopyWindowToVram(WINDOW_HP_PWR, COPYWIN_FULL);
}

void PrintTargetBattlerPrompt(enum BattleId battler)
{
    StringCopy(gStringVar2, GetSpeciesName(gDeckBattleMons[battler].species));
    StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("Attack {STR_VAR_2}?"));

    FillWindowPixelBuffer(WINDOW_MESSAGE, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WINDOW_MESSAGE, FONT_SHORT_NARROW, 2, 1, sTextColorNormal, TEXT_SKIP_DRAW, gStringVar1);
    CopyWindowToVram(WINDOW_MESSAGE, COPYWIN_FULL);
}

void PrintMoveUseString(void)
{
    StringCopy(gStringVar2, GetSpeciesName(gDeckBattleMons[gBattlerAttacker].species));
    StringCopy(gStringVar3, GetMoveName(gSpeciesDeckInfo[gDeckBattleMons[gBattlerAttacker].species].move));
    StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("{STR_VAR_2} used {STR_VAR_3}!"));

    FillWindowPixelBuffer(WINDOW_MESSAGE, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WINDOW_MESSAGE, FONT_SHORT_NARROW, 2, 1, sTextColorNormal, TEXT_SKIP_DRAW, gStringVar1);
    CopyWindowToVram(WINDOW_MESSAGE, COPYWIN_FULL);
}

void PrintMoveOutcomeString(void)
{
    StringCopy(gStringVar2, GetSpeciesName(gDeckBattleMons[gBattlerTarget].species));
    ConvertIntToDecimalStringN(gStringVar3, gDeckBattleMons[gBattlerAttacker].pwr, STR_CONV_MODE_LEFT_ALIGN, 2);
    StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("{STR_VAR_2} took {STR_VAR_3} damage!"));

    FillWindowPixelBuffer(WINDOW_MESSAGE, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WINDOW_MESSAGE, FONT_SHORT_NARROW, 2, 1, sTextColorNormal, TEXT_SKIP_DRAW, gStringVar1);
    CopyWindowToVram(WINDOW_MESSAGE, COPYWIN_FULL);
}