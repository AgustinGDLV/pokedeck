#include "global.h"
#include "battle_anim.h"
#include "bg.h"
#include "deck_battle_interface.h"
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

// constants


// structs


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
static const u16 sDeckBattleInterfacePalette[] = INCBIN_U16("graphics/deck_battle_interface/tiles.gbapal");
static const u32 sDeckBattleInterfaceTiles[] = INCBIN_U32("graphics/deck_battle_interface/tiles.4bpp.lz");
static const u32 sDeckBattleInterfaceTilemap[] = INCBIN_U32("graphics/deck_battle_interface/map.bin.lz");

static const u16 sGrassBackgroundPalette[] = INCBIN_U16("graphics/deck_battle_backgrounds/grass_tiles.gbapal");
static const u32 sGrassBackgroundTiles[] = INCBIN_U32("graphics/deck_battle_backgrounds/grass_tiles.4bpp.lz");
static const u32 sGrassBackgroundTilemap[] = INCBIN_U32("graphics/deck_battle_backgrounds/grass_map.bin.lz");

const struct OamData sOamData_8x8 =
{
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x8),
    .size = SPRITE_SIZE(8x8),
    .priority = 1,
};

const struct OamData sOamData_32x32 =
{
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 1,
};

const struct OamData sOamData_64x64 =
{
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .size = SPRITE_SIZE(64x64),
    .priority = 1,
};

static const u8 sSlowpokeNormalGfx[] = INCBIN_U8("graphics/deck_pokemon/slowpoke/portrait_normal.4bpp");
static const u16 sSlowpokeNormalPal[] = INCBIN_U16("graphics/deck_pokemon/slowpoke/portrait_normal.gbapal");

static const struct SpriteSheet sSlowpokeSpriteSheet = { sSlowpokeNormalGfx, sizeof(sSlowpokeNormalGfx), 2000 };
static const struct SpritePalette sSlowpokeSpritePalette = { sSlowpokeNormalPal, 2000 };

static const struct SpriteTemplate sSlowpokeSpriteTemplate =
{
	.tileTag = 2000,
	.paletteTag = 2000,
	.oam = &sOamData_64x64,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const u8 sSlowpokePlayerIdleGfx[] = INCBIN_U8("graphics/deck_pokemon/slowpoke/player_idle.4bpp");
static const u16 sSlowpokePlayerIdlePal[] = INCBIN_U16("graphics/deck_pokemon/slowpoke/player_idle.gbapal");

static const struct SpriteSheet sSlowpokePlayerIdleSpriteSheet = { sSlowpokePlayerIdleGfx, sizeof(sSlowpokePlayerIdleGfx), 2001 };
static const struct SpritePalette sSlowpokePlayerIdleSpritePalette = { sSlowpokePlayerIdlePal, 2001 };

static const struct SpriteTemplate sSlowpokePlayerIdleSpriteTemplate =
{
	.tileTag = 2001,
	.paletteTag = 2001,
	.oam = &sOamData_32x32,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const u8 sSlowpokeOpponentIdleGfx[] = INCBIN_U8("graphics/deck_pokemon/slowpoke/opponent_idle.4bpp");
static const u16 sSlowpokeOpponentIdlePal[] = INCBIN_U16("graphics/deck_pokemon/slowpoke/opponent_idle.gbapal");

static const struct SpriteSheet sSlowpokeOpponentIdleSpriteSheet = { sSlowpokeOpponentIdleGfx, sizeof(sSlowpokeOpponentIdleGfx), 2002 };

static const struct SpriteTemplate sSlowpokeOpponentIdleSpriteTemplate =
{
	.tileTag = 2002,
	.paletteTag = 2001,
	.oam = &sOamData_32x32,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const u8 sShadowGfx[] = INCBIN_U8("graphics/deck_battle_interface/shadow.4bpp");
static const u16 sShadowPal[] = INCBIN_U16("graphics/deck_battle_interface/shadow.gbapal");

static const struct SpriteSheet sShadowSpriteSheet = { sShadowGfx, sizeof(sShadowGfx), 2003 };
static const struct SpritePalette sShadowSpritePalette = { sShadowPal, 2003 };

static const struct SpriteTemplate sShadowSpriteTemplate =
{
	.tileTag = 2003,
	.paletteTag = 2003,
	.oam = &sOamData_32x32,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const u8 sCursorGfx[] = INCBIN_U8("graphics/deck_battle_interface/cursor.4bpp");

static const struct SpriteSheet sCursorSpriteSheet = { sCursorGfx, sizeof(sCursorGfx), 2004 };

static void SpriteCB_Cursor(struct Sprite *sprite)
{
    if (++sprite->animDelayCounter > 12)
    {
        sprite->y2 ^= 1;
        sprite->animDelayCounter = 0;
    }
}

static const struct SpriteTemplate sCursorSpriteTemplate =
{
	.tileTag = 2004,
	.paletteTag = 2003,
	.oam = &sOamData_8x8,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_Cursor,
};

static const u8 sSlowpokePlayerAttackGfx[] = INCBIN_U8("graphics/deck_pokemon/slowpoke/player_attack.4bpp");

static const struct SpriteSheet sSlowpokePlayerAttackSpriteSheet = { sSlowpokePlayerAttackGfx, sizeof(sSlowpokePlayerAttackGfx), 2005 };

static const union AnimCmd sAnim_Attack[] =
{
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(16, 8),
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnims_Attack[] =
{
    sAnim_Attack,
};

static void SpriteCB_Attack(struct Sprite *sprite);

static const struct SpriteTemplate sSlowpokePlayerAttackSpriteTemplate =
{
	.tileTag = 2005,
	.paletteTag = 2001,
	.oam = &sOamData_32x32,
	.anims = sAnims_Attack,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_Attack,
};

// ewram data
EWRAM_DATA static u8 sCursorSpriteId = 0;
EWRAM_DATA static u8 sSelectedPartyId = 0;
EWRAM_DATA static u8 sPlayerSpriteIds[PARTY_SIZE] = {0};

// forward declarations
static void MainCB2_DeckBattle(void);
static void VBlankCB2_DeckBattle(void);
static void Task_OpenDeckBattle(u8 taskId);
static void Task_CloseDeckBattle(u8 taskId);
static void Task_DeckBattleWaitForKeypress(u8 taskId);
static void LoadDeckBattleGfx(void);
static void CreateDeckBattleWindows(void);

// UI functions
static void MainCB2_DeckBattle(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VBlankCB2_DeckBattle(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void OpenDeckBattle(void)
{
    SetMainCallback2(CB2_OpenDeckBattleCustom);
}

void CB2_OpenDeckBattleCustom(void)
{
    switch (gMain.state) {
        default:
        case 0:
            UpdatePaletteFade();
            if (!gPaletteFade.active)
                gMain.state++;
            break;
        case 1:
            SetVBlankCallback(NULL); 
            ClearVramOamPlttRegs();
            SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
            ClearTasksAndGraphicalStructs();
            gMain.state++;
            break;
        case 2: // Init backgrounds.
            ResetBgsAndClearDma3BusyFlags(0);
            InitBgsFromTemplates(0, sDeckBattleBgTemplates, ARRAY_COUNT(sDeckBattleBgTemplates));

            LZDecompressVram(sDeckBattleInterfaceTiles, (void *)(BG_CHAR_ADDR(0)));
            LZDecompressVram(sDeckBattleInterfaceTilemap, (void *)(BG_SCREEN_ADDR(24)));
            LoadPalette(sDeckBattleInterfacePalette, BG_PLTT_ID(0), PLTT_SIZE_4BPP);

            LZDecompressVram(sGrassBackgroundTiles, (void *)(BG_CHAR_ADDR(2)));
            LZDecompressVram(sGrassBackgroundTilemap, (void *)(BG_SCREEN_ADDR(26)));
            LoadPalette(sGrassBackgroundPalette, BG_PLTT_ID(1), PLTT_SIZE_4BPP);

            CopyBgTilemapBufferToVram(0);
            CopyBgTilemapBufferToVram(3);
            gMain.state++;
            break;
        case 3:
            if (IsDma3ManagerBusyWithBgCopy() != TRUE)
            {
                ShowBg(0);
                HideBg(1);
                ShowBg(2);
                ShowBg(3);
                gMain.state++;
            }
            break;
        case 4: // Init windows.
            InitWindows(sDeckBattleWinTemplates);
            CreateDeckBattleWindows();
            DeactivateAllTextPrinters();
            gMain.state++;
            break;
        case 7: // Load final graphics and start battle.
            LoadDeckBattleGfx();
            SetVBlankCallback(VBlankCB2_DeckBattle);
            CreateTask(Task_OpenDeckBattle, 0);
            SetMainCallback2(MainCB2_DeckBattle);
            break;
    }
}

static void Task_OpenDeckBattle(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_DeckBattleWaitForKeypress;
}

static void Task_DeckBattleWaitForKeypress(u8 taskId)
{
    if ((gMain.newKeys & DPAD_LEFT) && sSelectedPartyId > 0)
    {
        gSprites[sCursorSpriteId].x2 -= 32;
        sSelectedPartyId -= 1;
        PlaySE(SE_SELECT);
    }
    if ((gMain.newKeys & DPAD_RIGHT) && sSelectedPartyId + 1 < PARTY_SIZE)
    {
        gSprites[sCursorSpriteId].x2 += 32;
        sSelectedPartyId += 1;
        PlaySE(SE_SELECT);
    }
    if (gMain.newKeys & A_BUTTON)
    {
        LoadSpriteSheet(&sSlowpokePlayerAttackSpriteSheet);
        gSprites[sPlayerSpriteIds[sSelectedPartyId]].invisible = TRUE;
        u32 spriteId = CreateSprite(&sSlowpokePlayerAttackSpriteTemplate, 28 + sSelectedPartyId*32, 96, 0);
        gSprites[spriteId].data[0] = sSelectedPartyId;
        StartSpriteAnim(&gSprites[sPlayerSpriteIds[sSelectedPartyId]], 0);
        PlayCry_Normal(SPECIES_SLOWPOKE, 0);
    }
    if (gMain.newKeys & B_BUTTON)
    {
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
        gTasks[taskId].func = Task_CloseDeckBattle;
        PlaySE(SE_SELECT);
    }
}

static void Task_CloseDeckBattle(u8 taskId)
{
    // Change transparency color to black so fade out looks better.
    u16 palette = RGB_BLACK;
    LoadPalette(&palette, BG_PLTT_ID(0), PLTT_SIZEOF(1));

    // Reset data and destroy task.
    FreeAllWindowBuffers();
    ResetSpriteData();
    UnfreezeObjectEvents();
    DestroyTask(taskId);

    // Return to overworld.
    SetMainCallback2(CB2_ReturnToField);
}

static void LoadDeckBattleGfx(void)
{
    // Load portrait.
    LoadSpriteSheet(&sSlowpokeSpriteSheet);
    LoadSpritePalette(&sSlowpokeSpritePalette);
    CreateSprite(&sSlowpokeSpriteTemplate, 36, 124, 0);

    // Load player and opponent sprites.
    u32 i;
    LoadSpriteSheet(&sSlowpokePlayerIdleSpriteSheet);
    LoadSpriteSheet(&sSlowpokeOpponentIdleSpriteSheet);
    LoadSpriteSheet(&sShadowSpriteSheet);
    LoadSpritePalette(&sSlowpokePlayerIdleSpritePalette);
    LoadSpritePalette(&sShadowSpritePalette);
    for (i = 0; i < PARTY_SIZE; ++i)
    {
        sPlayerSpriteIds[i] = CreateSprite(&sSlowpokePlayerIdleSpriteTemplate, 28 + 32*i, 96, 0);
        CreateSprite(&sShadowSpriteTemplate, 28 + 32*i, 96, 8);

        CreateSprite(&sSlowpokeOpponentIdleSpriteTemplate, 52 + 32*i, 56, 0);
        CreateSprite(&sShadowSpriteTemplate, 52 + 32*i, 56, 8);
    }

    // Load cursor.
    LoadSpriteSheet(&sCursorSpriteSheet);
    sCursorSpriteId = CreateSprite(&sCursorSpriteTemplate, 28, 80, 0);
}

static void CreateDeckBattleWindows(void)
{
    u32 i, windowId;
    for (i = 0; i < WINDOW_COUNT; ++i)
    {
        windowId = AddWindow(&sDeckBattleWinTemplates[i]);
        PutWindowTilemap(windowId);
        CopyWindowToVram(i, COPYWIN_FULL);
    }
}

static void SpriteCB_Attack(struct Sprite *sprite)
{
    if (sprite->animEnded)
    {
        gSprites[sPlayerSpriteIds[sprite->data[0]]].invisible = FALSE;
        DestroySprite(sprite);
        FreeSpriteTilesByTag(2005);
    }
}
