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

// windows and bgs
enum Windows
{
    WINDOW_1,
    WINDOW_COUNT,
};

static const struct WindowTemplate sDeckBattleWinTemplates[WINDOW_COUNT + 1] =
{
    [WINDOW_1] =
    {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 4,
        .width = 16,
        .height = 10,
        .paletteNum = 15,
        .baseBlock = 1,
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
        .priority = 0,
        .baseTile = 0,
    },
    {
        .bg = 1,
        .charBaseIndex = 1,
        .mapBaseIndex = 28,
        .screenSize = 2,
        .paletteMode = 0,
        .priority = 1,
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
	.oam = &sOamData_64x64,
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
        .oam = &sOamData_32x32,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_PLAYER_1] =
    {
        .tileTag = TAG_BATTLER_OBJ + 1,
        .paletteTag = 0,
        .oam = &sOamData_32x32,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_PLAYER_2] =
    {
        .tileTag = TAG_BATTLER_OBJ + 2,
        .paletteTag = 0,
        .oam = &sOamData_32x32,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_PLAYER_3] =
    {
        .tileTag = TAG_BATTLER_OBJ + 3,
        .paletteTag = 0,
        .oam = &sOamData_32x32,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_PLAYER_4] =
    {
        .tileTag = TAG_BATTLER_OBJ + 4,
        .paletteTag = 0,
        .oam = &sOamData_32x32,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_PLAYER_5] =
    {
        .tileTag = TAG_BATTLER_OBJ + 5,
        .paletteTag = 0,
        .oam = &sOamData_32x32,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_OPPONENT_0] =
    {
        .tileTag = TAG_BATTLER_OBJ + 6,
        .paletteTag = 0,
        .oam = &sOamData_32x32,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_OPPONENT_1] =
    {
        .tileTag = TAG_BATTLER_OBJ + 7,
        .paletteTag = 0,
        .oam = &sOamData_32x32,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_OPPONENT_2] =
    {
        .tileTag = TAG_BATTLER_OBJ + 8,
        .paletteTag = 0,
        .oam = &sOamData_32x32,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_OPPONENT_3] =
    {
        .tileTag = TAG_BATTLER_OBJ + 9,
        .paletteTag = 0,
        .oam = &sOamData_32x32,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_OPPONENT_4] =
    {
        .tileTag = TAG_BATTLER_OBJ + 10,
        .paletteTag = 0,
        .oam = &sOamData_32x32,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_OPPONENT_5] =
    {
        .tileTag = TAG_BATTLER_OBJ + 11,
        .paletteTag = 0,
        .oam = &sOamData_32x32,
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
    gDeckBattleGraphics.cursorSpriteId = SPRITE_NONE;
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
}

void InitDeckBattleGfx(void)
{
    LoadSpritePalette(&sMiscGfxSpritePalette);
    LoadSpriteSheet(&sShadowSpriteSheet);
    LoadSpriteSheet(&sCursorSpriteSheet);

    gDeckBattleGraphics.cursorSpriteId = CreateSprite(&sCursorSpriteTemplate, 28, 80, 0);
    gSprites[gDeckBattleGraphics.cursorSpriteId].invisible = TRUE;

    LoadDummySpriteTiles();
    gDeckBattleGraphics.portraitSpriteId = CreateSprite(&sPortraitSpriteTemplate, PORTRAIT_X, PORTRAIT_Y, 0);
    for (u32 i = 0; i < MAX_DECK_BATTLERS_COUNT; ++i)
    {
        if (GetDeckBattlerSide(i) == B_SIDE_PLAYER)
        {
            gDeckBattleGraphics.battlerSpriteIds[i] = CreateSprite(&sBattlerSpriteTemplates[i], PLAYER_OBJ_X + OBJ_OFFSET*i, PLAYER_OBJ_Y, 0);
            gDeckBattleGraphics.shadowSpriteIds[i] = CreateSprite(&sShadowSpriteTemplate, PLAYER_OBJ_X + OBJ_OFFSET*i, PLAYER_OBJ_Y, 0);
        }
        else
        {
            gDeckBattleGraphics.battlerSpriteIds[i] = CreateSprite(&sBattlerSpriteTemplates[i], OPPONENT_OBJ_X + OBJ_OFFSET*(i-B_OPPONENT_0), OPPONENT_OBJ_Y, 0);
            gDeckBattleGraphics.shadowSpriteIds[i] = CreateSprite(&sShadowSpriteTemplate, OPPONENT_OBJ_X + OBJ_OFFSET*(i-B_OPPONENT_0), OPPONENT_OBJ_Y, 0);
        }
    }
}

static void SpriteCB_Cursor(struct Sprite *sprite)
{
    if (++sprite->animDelayCounter > 12)
    {
        sprite->y2 ^= 1;
        sprite->animDelayCounter = 0;
    }
}
