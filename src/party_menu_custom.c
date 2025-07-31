#include "global.h"
#include "battle_anim.h"
#include "bg.h"
#include "decompress.h"
#include "deck_battle.h"
#include "deck_battle_interface.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_screen_effect.h"
#include "field_weather.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "line_break.h"
#include "m4a.h"
#include "main.h"
#include "main_menu_custom.h"
#include "malloc.h"
#include "menu.h"
#include "overworld.h"
#include "option_menu.h"
#include "palette.h"
#include "party_menu.h"
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
#include "trainer_pokemon_sprites.h"
#include "util.h"
#include "window.h"
#include "constants/event_objects.h"
#include "constants/pokedex.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/species.h"
#include "constants/flags.h"

// definitions
#define sPartyIndex     data[0]
#define sShadowSpriteId data[1]

struct PartyMonData
{
    u16 species;
    u8 name[POKEMON_NAME_LENGTH];
    u16 hp;
    u16 maxHP;
    u16 pwr;
    u16 def;
    u16 lvl;
    u16 exp;
    u16 expToNextLvl;
    u16 position;
};

struct PartyMenuData
{
    u8 selectedPosition;
    u8 swapPosition;
    u8 battlerSpriteIds[POSITIONS_COUNT];
    u8 cursorSpriteId;
    struct PartyMonData mons[POSITIONS_COUNT];
};

// windows and bgs
enum Windows
{
    WINDOW_MON_INFO,
    WINDOW_MOVE_INFO,
    WINDOW_COUNT,
};

static const struct WindowTemplate sPartyMenuWinTemplates[WINDOW_COUNT + 1] =
{
    [WINDOW_MON_INFO] =
    {
        .bg = 1,
        .tilemapLeft = 3,
        .tilemapTop = 9,
        .width = 24,
        .height = 4,
        .paletteNum = 0,
        .baseBlock = 1,
    },
    [WINDOW_MOVE_INFO] =
    {
        .bg = 1,
        .tilemapLeft = 3,
        .tilemapTop = 14,
        .width = 24,
        .height = 5,
        .paletteNum = 0,
        .baseBlock = 1+24*4,
    },
    DUMMY_WIN_TEMPLATE,
};

static const struct BgTemplate sPartyMenuBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = 2,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    { // Interface
        .bg = 2,
        .charBaseIndex = 0,
        .mapBaseIndex = 14,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0,
    },
    { // Text
        .bg = 1,
        .charBaseIndex = 2,
        .mapBaseIndex = 6,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0,
    },
    { // Environment
        .bg = 3,
        .charBaseIndex = 1,
        .mapBaseIndex = 21,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0,
    },
};

static const struct OamData sOamData_Battler =
{
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 1,
};

static const union AnimCmd sAnim_Paused[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Idle[] =
{
    ANIMCMD_FRAME(0, 12),
    ANIMCMD_FRAME(16, 12),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnims_Battler[] =
{
    sAnim_Paused,
    sAnim_Idle,
};

static const u8 sDummyObjectGfx[] = INCBIN_U8("graphics/deck_pokemon/slowpoke/player_idle.4bpp");
static const struct SpriteTemplate sBattlerSpriteTemplates[] =
{
    [POSITION_0] =
    {
        .tileTag = TAG_BATTLER_OBJ,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [POSITION_1] =
    {
        .tileTag = TAG_BATTLER_OBJ + 1,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [POSITION_2] =
    {
        .tileTag = TAG_BATTLER_OBJ + 2,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [POSITION_3] =
    {
        .tileTag = TAG_BATTLER_OBJ + 3,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [POSITION_4] =
    {
        .tileTag = TAG_BATTLER_OBJ + 4,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [POSITION_5] =
    {
        .tileTag = TAG_BATTLER_OBJ + 5,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
};

// graphics data
static const u16 sPartyMenuPalette[] = INCBIN_U16("graphics/party_menu_custom/tiles.gbapal");
static const u32 sPartyMenuTiles[] = INCBIN_U32("graphics/party_menu_custom/tiles.4bpp.lz");
static const u32 sPartyMenuTilemap[] = INCBIN_U32("graphics/party_menu_custom/map.bin.lz");
static const u32 sPartyMenuEmptyTilemap[] = INCBIN_U32("graphics/party_menu_custom/empty.bin.lz");

// ewram data
EWRAM_DATA static struct PartyMenuData sPartyMenuData = {0};
EWRAM_DATA static u32 *sPartyMenuTilemapPtr = NULL;
EWRAM_DATA static u32 *sEnvironmentTilemapPtr = NULL;

// forward declarations
static void MainCB2_PartyMenu(void);
static void VBlankCB2_PartyMenu(void);
static void Task_OpenPartyMenu(u8 taskId);
static void Task_ClosePartyMenu(u8 taskId);
static void Task_PartyMenuHandleDefaultInput(u8 taskId);
static void Task_PartyMenuHandleSwapInput(u8 taskId);
static void LoadPartyMenuGfx(void);
static void CreatePartyMenuWindows(void);
static void PrintMonInfo(vu32 index);
static void PrintMoveInfo(u32 index);
static void DrawBattlerSprites(void);
static void MoveCursorOverPosition(u32 position);
static void InitPartyDataStruct(void);
static void CopyPartyDataToMonData(void);

// UI functions
static void MainCB2_PartyMenu(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VBlankCB2_PartyMenu(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void CB2_OpenPartyMenuCustom(void)
{
    switch (gMain.state) {
        default:
        case 0:
            SetVBlankCallback(NULL); 
            ClearVramOamPlttRegs();
            SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
            gMain.state++;
            break;
        case 1:
            ClearTasksAndGraphicalStructs();
            gMain.state++;
            break;
        case 2:
            sPartyMenuTilemapPtr = AllocZeroed(BG_SCREEN_SIZE);
            sEnvironmentTilemapPtr = AllocZeroed(BG_SCREEN_SIZE);
            ResetBgsAndClearDma3BusyFlags(0);
            InitBgsFromTemplates(0, sPartyMenuBgTemplates, ARRAY_COUNT(sPartyMenuBgTemplates));
            SetBgTilemapBuffer(2, sPartyMenuTilemapPtr);
            SetBgTilemapBuffer(3, sEnvironmentTilemapPtr);
            gMain.state++;
            break;
        case 3:
            DecompressAndCopyTileDataToVram(2, sPartyMenuTiles, 0, 0, 0);
            LZDecompressWram(sPartyMenuTilemap, sPartyMenuTilemapPtr);
            LoadPalette(sPartyMenuPalette, BG_PLTT_ID(0), PLTT_SIZE_4BPP);
            Menu_LoadStdPalAt(BG_PLTT_ID(15));

            DecompressAndCopyTileDataToVram(3, gGrassBackgroundTiles, 0, 0, 0);
            LZDecompressVram(gGrassBackgroundTilemap, sEnvironmentTilemapPtr);
            LoadPalette(gGrassBackgroundPalette, BG_PLTT_ID(1), PLTT_SIZE_4BPP);
            gMain.state++;
            break;
        case 4:
            if (IsDma3ManagerBusyWithBgCopy() != TRUE)
            {
                HideBg(0);
                ShowBg(1);
                ShowBg(2);
                ShowBg(3);
                CopyBgTilemapBufferToVram(2);
                CopyBgTilemapBufferToVram(3);
                gMain.state++;
            }
            break;
        case 5:
            InitWindows(sPartyMenuWinTemplates);
            DeactivateAllTextPrinters();
            gMain.state++;
            break;
        case 6:
            BeginNormalPaletteFade(PALETTES_ALL, 1, 16, 0, RGB_BLACK);
            m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 0x80);
            InitPartyDataStruct();
            SetVBlankCallback(VBlankCB2_PartyMenu);
            CreateTask(Task_OpenPartyMenu, 0);
            SetMainCallback2(MainCB2_PartyMenu);
            break;
    }
}

static void Task_OpenPartyMenu(u8 taskId)
{
    if (gTasks[taskId].data[0] == 0)
    {
        LoadPartyMenuGfx();
        gTasks[taskId].data[0] = 1;
    }
    else if (!gPaletteFade.active)
    {
        gTasks[taskId].func = Task_PartyMenuHandleDefaultInput;
        gTasks[taskId].data[0] = 0;
    }
}

static void UpdateDisplayedMonInfo(u32 index)
{
    if (sPartyMenuData.mons[index].species != SPECIES_NONE)
    {
        PrintMonInfo(index);
        PrintMoveInfo(index);
    }
    else
    {
        FillWindowPixelBuffer(WINDOW_MON_INFO, PIXEL_FILL(0));
        FillWindowPixelBuffer(WINDOW_MOVE_INFO, PIXEL_FILL(0));
    }
}

static u32 GetPartyIndexAtPosition(u32 position)
{
    for (u32 i = 0; i < PARTY_SIZE; ++i)
    {
        if (sPartyMenuData.mons[i].species != SPECIES_NONE && sPartyMenuData.mons[i].position == position)
            return i;
    }
    return PARTY_SIZE;
}

static u32 GetOccupiedPositionToLeft(u32 position)
{
    u32 index;
    if (position == POSITION_0)
        return POSITIONS_COUNT;

    for (u32 i = position - 1; i >= POSITION_0 && i != (-1); --i)
    {
        index = GetPartyIndexAtPosition(i);
        if (sPartyMenuData.mons[index].species != SPECIES_NONE)
            return i;
    }
    return POSITIONS_COUNT;
}

static u32 GetOccupiedPositionToRight(u32 position)
{
    u32 index;
    for (u32 i = position + 1; i < POSITIONS_COUNT; ++i)
    {
        index = GetPartyIndexAtPosition(i);
        if (sPartyMenuData.mons[index].species != SPECIES_NONE)
            return i;
    }
    return POSITIONS_COUNT;
}

static void Task_PartyMenuHandleDefaultInput(u8 taskId)
{
    u32 pos;
    if ((gMain.newKeys & DPAD_RIGHT) && 
        (pos = GetOccupiedPositionToRight(sPartyMenuData.selectedPosition)) != POSITIONS_COUNT)
    {
        PlaySE(SE_SELECT);
        sPartyMenuData.selectedPosition = pos;
        UpdateDisplayedMonInfo(GetPartyIndexAtPosition(pos));
        MoveCursorOverPosition(pos);
    }
    if ((gMain.newKeys & DPAD_LEFT) && 
        (pos = GetOccupiedPositionToLeft(sPartyMenuData.selectedPosition)) != POSITIONS_COUNT)
    {
        PlaySE(SE_SELECT);
        sPartyMenuData.selectedPosition = pos;
        UpdateDisplayedMonInfo(GetPartyIndexAtPosition(pos));
        MoveCursorOverPosition(pos);
    }
    if ((gMain.newKeys & A_BUTTON) || (gMain.newKeys & START_BUTTON))
    {
        PlaySE(SE_SELECT);
        if (sPartyMenuData.selectedPosition == POSITION_0)
            sPartyMenuData.swapPosition = POSITION_1;
        else
            sPartyMenuData.swapPosition = POSITION_0;
        DebugPrintf("selected pos: %d, swap pos %d", sPartyMenuData.selectedPosition, sPartyMenuData.swapPosition);
        gSprites[sPartyMenuData.battlerSpriteIds[GetPartyIndexAtPosition(sPartyMenuData.selectedPosition)]].oam.objMode = ST_OAM_OBJ_BLEND;
        MoveCursorOverPosition(sPartyMenuData.swapPosition);
        gTasks[taskId].func = Task_PartyMenuHandleSwapInput;
    }
    if (gMain.newKeys & B_BUTTON)
    {
        FadeScreen(FADE_TO_BLACK, 0);
        gTasks[taskId].func = Task_ClosePartyMenu;
        PlaySE(SE_SELECT);
    }
}

static void Task_PartyMenuHandleSwapInput(u8 taskId)
{
    u32 index1, index2, temp;
    if (gMain.newKeys & DPAD_RIGHT && sPartyMenuData.swapPosition < POSITIONS_COUNT - 1)
    {
        if (sPartyMenuData.swapPosition + 1 != sPartyMenuData.selectedPosition)
            sPartyMenuData.swapPosition += 1;
        else if (sPartyMenuData.selectedPosition != POSITION_5)
            sPartyMenuData.swapPosition += 2;
        else
            return;

        PlaySE(SE_SELECT);
        MoveCursorOverPosition(sPartyMenuData.swapPosition);
    }
    if (gMain.newKeys & DPAD_LEFT && sPartyMenuData.swapPosition > POSITION_0)
    {
        if (sPartyMenuData.swapPosition - 1 != sPartyMenuData.selectedPosition)
            sPartyMenuData.swapPosition -= 1;
        else if (sPartyMenuData.selectedPosition != POSITION_0)
            sPartyMenuData.swapPosition -= 2;
        else
            return;

        PlaySE(SE_SELECT);
        MoveCursorOverPosition(sPartyMenuData.swapPosition);
    }
    if ((gMain.newKeys & A_BUTTON) || (gMain.newKeys & START_BUTTON))
    {
        PlaySE(SE_SELECT);
        index1 = GetPartyIndexAtPosition(sPartyMenuData.selectedPosition);
        index2 = GetPartyIndexAtPosition(sPartyMenuData.swapPosition);
        if (index2 != PARTY_SIZE)
        {
            SWAP(sPartyMenuData.mons[index1].position, sPartyMenuData.mons[index2].position, temp);
            gSprites[sPartyMenuData.battlerSpriteIds[index2]].x = 40 + OBJ_OFFSET*sPartyMenuData.mons[index2].position;
            gSprites[gSprites[sPartyMenuData.battlerSpriteIds[index2]].sShadowSpriteId].x = 40 + OBJ_OFFSET*sPartyMenuData.mons[index1].position;
        }
        else
        {
            sPartyMenuData.mons[index1].position = sPartyMenuData.swapPosition;
        }
        gSprites[sPartyMenuData.battlerSpriteIds[index1]].oam.objMode = ST_OAM_OBJ_NORMAL;
        gSprites[sPartyMenuData.battlerSpriteIds[index1]].x = 40 + OBJ_OFFSET*sPartyMenuData.mons[index1].position;
        gSprites[gSprites[sPartyMenuData.battlerSpriteIds[index1]].sShadowSpriteId].x = 40 + OBJ_OFFSET*sPartyMenuData.mons[index1].position;
        sPartyMenuData.selectedPosition = sPartyMenuData.swapPosition;
        gTasks[taskId].func = Task_PartyMenuHandleDefaultInput;
    }
    if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_SELECT);
        gSprites[sPartyMenuData.battlerSpriteIds[GetPartyIndexAtPosition(sPartyMenuData.selectedPosition)]].oam.objMode = ST_OAM_OBJ_NORMAL;
        MoveCursorOverPosition(sPartyMenuData.selectedPosition);
        gTasks[taskId].func = Task_PartyMenuHandleDefaultInput;
    }
}

static void Task_ClosePartyMenu(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        // Change transparency color to black so fade out looks better.
        u16 palette = RGB_BLACK;
        LoadPalette(&palette, BG_PLTT_ID(0), PLTT_SIZEOF(1));

        // Free tilemap pointer and data.
        Free(sPartyMenuTilemapPtr);
        sPartyMenuTilemapPtr = NULL;
        Free(sEnvironmentTilemapPtr);
        sEnvironmentTilemapPtr = NULL;

        // Reset data and destroy task.
        FreeAllWindowBuffers();
        ResetSpriteData();
        UnfreezeObjectEvents();
        DestroyTask(taskId);

        // Return to overworld.
        CopyPartyDataToMonData();
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 0x100);
        SetMainCallback2(CB2_ReturnToFieldWithOpenMenu);
    }
}

static u32 GetLeftmostOccupiedPosition(void)
{
    u32 index;
    for (u32 i = 0; i < POSITIONS_COUNT; ++i)
    {
        index = GetPartyIndexAtPosition(i);
        if (sPartyMenuData.mons[index].species != SPECIES_NONE)
            return i;
    }
    return POSITIONS_COUNT;
}

static void LoadPartyMenuGfx(void)
{
    // Draw text.
    u32 position = GetLeftmostOccupiedPosition();
    u32 index = GetPartyIndexAtPosition(position);
    CreatePartyMenuWindows();
    PrintMonInfo(index);
    PrintMoveInfo(index);

    // Load battler sprites.
    LoadSpritePalette(&gMiscGfxSpritePalette);
    LoadSpriteSheet(&gShadowSpriteSheet);
    DrawBattlerSprites();
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(8, 6));

    // Load cursor.
    LoadSpriteSheet(&gCursorSpriteSheet);
    sPartyMenuData.cursorSpriteId = CreateSprite(&gCursorSpriteTemplate, 40, 48-16, 0);
    MoveCursorOverPosition(position);
    sPartyMenuData.selectedPosition = position;
}

static void CreatePartyMenuWindows(void)
{
    u32 i, windowId;
    for (i = 0; i < WINDOW_COUNT; ++i)
    {
        windowId = AddWindow(&sPartyMenuWinTemplates[i]);
        PutWindowTilemap(windowId);
        CopyWindowToVram(i, COPYWIN_FULL);
    }
}

static const u8 sTextColor_Black[] = {0, 3, 0}; // white text, transparent bg and shadow
static const u8 sTextColor_Red[] = {0, 2, 0}; // gray text, transparent bg and shadow

static void PrintMonInfo(u32 index)
{
    u8 *strPtr;
    u32 digits = 0;

    FillWindowPixelBuffer(WINDOW_MON_INFO, PIXEL_FILL(0));

    // Print nickname.
    AddTextPrinterParameterized3(WINDOW_MON_INFO, FONT_NORMAL, 0, 2, sTextColor_Red, TEXT_SKIP_DRAW, sPartyMenuData.mons[index].name);

    // Print HP.
    ConvertIntToDecimalStringN(gStringVar1, sPartyMenuData.mons[index].hp, STR_CONV_MODE_LEFT_ALIGN, 3);
    for (u32 i = sPartyMenuData.mons[index].hp; i > 0; i /= 10)
        ++digits;
    strPtr = &gStringVar1[digits];
    *strPtr = CHAR_SLASH;
    ++strPtr;
    ConvertIntToDecimalStringN(strPtr, sPartyMenuData.mons[index].maxHP, STR_CONV_MODE_LEFT_ALIGN, 3);
    AddTextPrinterParameterized3(WINDOW_MON_INFO, FONT_NORMAL, 16*8+4, 2, sTextColor_Black, TEXT_SKIP_DRAW, gStringVar1);
    
    // Print LVL and EXP.
    ConvertIntToDecimalStringN(gStringVar1, sPartyMenuData.mons[index].lvl, STR_CONV_MODE_LEFT_ALIGN, 3);
    AddTextPrinterParameterized3(WINDOW_MON_INFO, FONT_NORMAL, 3*8+4, 11, sTextColor_Black, TEXT_SKIP_DRAW, gStringVar1);

    ConvertIntToDecimalStringN(gStringVar1, sPartyMenuData.mons[index].exp, STR_CONV_MODE_LEFT_ALIGN, 4);
    digits = 0;
    for (u32 i = sPartyMenuData.mons[index].exp; i > 0; i /= 10)
        ++digits;
    strPtr = &gStringVar1[digits];
    *strPtr = CHAR_SLASH;
    ++strPtr;
    ConvertIntToDecimalStringN(strPtr, sPartyMenuData.mons[index].expToNextLvl, STR_CONV_MODE_LEFT_ALIGN, 4);
    AddTextPrinterParameterized3(WINDOW_MON_INFO, FONT_NORMAL, 3*8+4, 20, sTextColor_Black, TEXT_SKIP_DRAW, gStringVar1);

    // Print PWR and DEF.
    ConvertIntToDecimalStringN(gStringVar1, sPartyMenuData.mons[index].pwr, STR_CONV_MODE_LEFT_ALIGN, 3);
    AddTextPrinterParameterized3(WINDOW_MON_INFO, FONT_NORMAL, 16*8+4, 11, sTextColor_Black, TEXT_SKIP_DRAW, gStringVar1);
    ConvertIntToDecimalStringN(gStringVar1, sPartyMenuData.mons[index].def, STR_CONV_MODE_LEFT_ALIGN, 3);
    AddTextPrinterParameterized3(WINDOW_MON_INFO, FONT_NORMAL, 16*8+4, 20, sTextColor_Black, TEXT_SKIP_DRAW, gStringVar1);
    CopyWindowToVram(WINDOW_MON_INFO, COPYWIN_FULL);
}

static void PrintMoveInfo(u32 index)
{
    const struct DeckMoveInfo *info = &gDeckMovesInfo[gDeckSpeciesInfo[sPartyMenuData.mons[index].species].move];
    FillWindowPixelBuffer(WINDOW_MOVE_INFO, PIXEL_FILL(0));

    AddTextPrinterParameterized3(WINDOW_MOVE_INFO, FONT_NORMAL, 0, 4, sTextColor_Red, TEXT_SKIP_DRAW, info->name);

    StringCopy(gStringVar1, info->description);
    BreakStringAutomatic(gStringVar1, 176, 2, FONT_NORMAL, SHOW_SCROLL_PROMPT);
    AddTextPrinterParameterized3(WINDOW_MOVE_INFO, FONT_NORMAL, 0, 14, sTextColor_Black, TEXT_SKIP_DRAW, gStringVar1);    
    CopyWindowToVram(WINDOW_MOVE_INFO, COPYWIN_FULL);
}

static void DrawBattlerSprites(void)
{
    u32 species, palIndex;
    for (u32 i = 0; i < PARTY_SIZE; ++i)
    {
        species = sPartyMenuData.mons[i].species;
        if (species == SPECIES_NONE || gDeckSpeciesInfo[species].baseHP == 0)
        {
            sPartyMenuData.battlerSpriteIds[i] = 0xFF;
        }
        else
        {
            // Draw battler sprite.
            palIndex = LoadSpritePaletteWithTag(gDeckSpeciesInfo[species].objectPalette, TAG_BATTLER_OBJ + i);
            const struct SpriteSheet spriteSheet = {gDeckSpeciesInfo[species].opponentIdle, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ + i};
            LoadSpriteSheet(&spriteSheet);
            sPartyMenuData.battlerSpriteIds[i] = CreateSprite(&sBattlerSpriteTemplates[i], 40 + OBJ_OFFSET*sPartyMenuData.mons[i].position, 48 + gDeckSpeciesInfo[species].opponentYOffset, 0);
            gSprites[sPartyMenuData.battlerSpriteIds[i]].oam.paletteNum = palIndex;

            // Draw shadow.
            gSprites[sPartyMenuData.battlerSpriteIds[i]].sShadowSpriteId = CreateSprite(&gShadowSpriteTemplate, 40 + OBJ_OFFSET*sPartyMenuData.mons[i].position, 48, 16);
            gSprites[gSprites[sPartyMenuData.battlerSpriteIds[i]].sShadowSpriteId].callback = SpriteCallbackDummy;
        }
    }
}

static void MoveCursorOverPosition(u32 position)
{
    gSprites[sPartyMenuData.cursorSpriteId].x = 40 + OBJ_OFFSET*position;
}

static void InitPartyDataStruct(void)
{
    sPartyMenuData.selectedPosition = 0;
    sPartyMenuData.swapPosition = 0;
    sPartyMenuData.cursorSpriteId = SPRITE_NONE;

    for (u32 i = 0; i < PARTY_SIZE; ++i)
    {
        sPartyMenuData.battlerSpriteIds[i] = SPRITE_NONE;
        sPartyMenuData.mons[i].species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES);
        if (sPartyMenuData.mons[i].species != SPECIES_NONE || gDeckSpeciesInfo[sPartyMenuData.mons[i].species].baseHP == 0)
        {
            GetMonNickname(&gPlayerParty[i], sPartyMenuData.mons[i].name);
            sPartyMenuData.mons[i].hp = GetMonData(&gPlayerParty[i], MON_DATA_HP);
            sPartyMenuData.mons[i].maxHP = GetMonData(&gPlayerParty[i], MON_DATA_MAX_HP);
            sPartyMenuData.mons[i].pwr = 100;
            sPartyMenuData.mons[i].def = 100;
            sPartyMenuData.mons[i].lvl = GetMonData(&gPlayerParty[i], MON_DATA_LEVEL);
            sPartyMenuData.mons[i].exp = 100;
            sPartyMenuData.mons[i].expToNextLvl = 100;
            sPartyMenuData.mons[i].position = GetMonData(&gPlayerParty[i], MON_DATA_POSITION);
        }
    }
}

static void CopyPartyDataToMonData(void)
{
    for (u32 i = 0; i < PARTY_SIZE; ++i)
    {
        if (sPartyMenuData.mons[i].species != SPECIES_NONE || gDeckSpeciesInfo[sPartyMenuData.mons[i].species].baseHP == 0)
        {
            SetMonData(&gPlayerParty[i], MON_DATA_POSITION, &sPartyMenuData.mons[i].position);
            SetMonData(&gPlayerParty[i], MON_DATA_HP, &sPartyMenuData.mons[i].hp);
            SetMonData(&gPlayerParty[i], MON_DATA_LEVEL, &sPartyMenuData.mons[i].lvl);
        }
    }
}
