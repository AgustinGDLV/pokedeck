#include "global.h"
#include "battle_anim.h"
#include "bg.h"
#include "decompress.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_screen_effect.h"
#include "field_weather.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "m4a.h"
#include "main.h"
#include "main_menu_custom.h"
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
#include "trainer_pokemon_sprites.h"
#include "util.h"
#include "window.h"
#include "constants/event_objects.h"
#include "constants/pokedex.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/species.h"
#include "constants/flags.h"

// windows and bgs
enum Windows
{
    WINDOW_PLAYER_INFO,
    WINDOW_DEX_PLAY_TIME,
    WINDOW_NEW_GAME,
    WINDOW_OPTIONS,
    WINDOW_COUNT,
};

static const struct WindowTemplate sMainMenuWinTemplates[WINDOW_COUNT + 1] =
{
    [WINDOW_PLAYER_INFO] =
    {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 4,
        .width = 16,
        .height = 10,
        .paletteNum = 0,
        .baseBlock = 1,
    },
    DUMMY_WIN_TEMPLATE,
};

static const struct BgTemplate sMainMenuBgTemplates[] =
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

// graphics data
static const u16 sMainMenuPalette[] = INCBIN_U16("graphics/main_menu_custom/tiles.gbapal");
static const u32 sMainMenuTiles[] = INCBIN_U32("graphics/main_menu_custom/tiles.4bpp.lz");
static const u32 sMainMenuContinueTilemap[] = INCBIN_U32("graphics/main_menu_custom/continue.bin.lz");
static const u32 sMainMenuNewGameTilemap[] = INCBIN_U32("graphics/main_menu_custom/new_game.bin.lz");
static const u32 sMainMenuOptionsTilemap[] = INCBIN_U32("graphics/main_menu_custom/options.bin.lz");

// constant data
#define SELECTED_CONTINUE   0
#define SELECTED_NEW_GAME   1
#define SELECTED_OPTIONS    2

// ewram data
EWRAM_DATA static MainCallback sMainMenuExitCallback = NULL;
EWRAM_DATA static u32 *sMainMenuTilemapPtr = NULL;
EWRAM_DATA static u32 *sEnvironmentTilemapPtr = NULL;
EWRAM_DATA static u8 sMainMenuSelectedOption = 0;
EWRAM_DATA static u8 sMainMenuLastSelectedOption = 0;
EWRAM_DATA static u32 sPlayerSpriteId = 0;
EWRAM_DATA static u32 sPartySpriteIds[PARTY_SIZE] = {0};

// forward declarations
static void MainCB2_MainMenu(void);
static void VBlankCB2_MainMenu(void);
static void Task_OpenMainMenu(u8 taskId);
static void Task_CloseMainMenu(u8 taskId);
static void Task_MainMenuWaitForKeypress(u8 taskId);
static void LoadMainMenuGfx(void);
static void CreateMainMenuWindows(void);
static void PrintPlayerInfo(void);
static void LoadMainMenuTilemap(u32 selectedId);
static void DrawPlayerSprite(void);
static void DrawPartyIcons(void);
static void ToggleGrayscaleAndAnims(void);

// UI functions
static void MainCB2_MainMenu(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VBlankCB2_MainMenu(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void CB2_OpenMainMenuCustom(void)
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
            gMain.state++;
            break;
        case 2:
            ClearTasksAndGraphicalStructs();
            gMain.state++;
            break;
        case 3:
            sMainMenuTilemapPtr = AllocZeroed(BG_SCREEN_SIZE);
            sEnvironmentTilemapPtr = AllocZeroed(BG_SCREEN_SIZE);
            ResetBgsAndClearDma3BusyFlags(0);
            InitBgsFromTemplates(0, sMainMenuBgTemplates, ARRAY_COUNT(sMainMenuBgTemplates));
            SetBgTilemapBuffer(2, sMainMenuTilemapPtr);
            SetBgTilemapBuffer(3, sEnvironmentTilemapPtr);
            gMain.state++;
            break;
        case 4:
            DecompressAndCopyTileDataToVram(2, sMainMenuTiles, 0, 0, 0);
            LoadMainMenuTilemap(sMainMenuSelectedOption);
            LoadPalette(sMainMenuPalette, BG_PLTT_ID(0), PLTT_SIZE_4BPP);
            Menu_LoadStdPalAt(BG_PLTT_ID(15));

            DecompressAndCopyTileDataToVram(3, gGrassBackgroundTiles, 0, 0, 0);
            LZDecompressVram(gGrassBackgroundTilemap, sEnvironmentTilemapPtr);
            LoadPalette(gGrassBackgroundPalette, BG_PLTT_ID(1), PLTT_SIZE_4BPP);
            BlendPalettes(1 << 1, 12, RGB_BLACK);
            gMain.state++;
            break;
        case 5:
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
        case 6:
            InitWindows(sMainMenuWinTemplates);
            DeactivateAllTextPrinters();
            gMain.state++;
            break;
        case 7:
            SetVBlankCallback(VBlankCB2_MainMenu);
            CreateTask(Task_OpenMainMenu, 0);
            SetMainCallback2(MainCB2_MainMenu);
            break;
    }
}

static void Task_OpenMainMenu(u8 taskId)
{
    LoadMainMenuGfx();
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_MainMenuWaitForKeypress;
}

static void Task_MainMenuWaitForKeypress(u8 taskId)
{
    // Handle input depending on selected option.
    switch (sMainMenuSelectedOption)
    {
        case SELECTED_CONTINUE:
            if (gMain.newKeys & A_BUTTON)
            {
                PlaySE(SE_SELECT);
                FadeScreen(FADE_TO_BLACK, 0);
                sMainMenuExitCallback = CB2_ContinueSavedGame;
                gTasks[taskId].func = Task_CloseMainMenu;
            }
            if (gMain.newKeys & DPAD_DOWN)
            {
                PlaySE(SE_SELECT);
                if (sMainMenuLastSelectedOption == SELECTED_OPTIONS)
                    sMainMenuSelectedOption = SELECTED_OPTIONS;
                else
                    sMainMenuSelectedOption = SELECTED_NEW_GAME;
                LoadMainMenuTilemap(sMainMenuSelectedOption);
                ToggleGrayscaleAndAnims();
            }
            break;
        case SELECTED_NEW_GAME:
            if (gMain.newKeys & A_BUTTON)
            {
                PlaySE(SE_SELECT);
                FadeScreen(FADE_TO_BLACK, 0);
                sMainMenuExitCallback = CB2_NewGame; // TODO: proper callback
                gTasks[taskId].func = Task_CloseMainMenu;
            }
            if (gMain.newKeys & DPAD_UP)
            {
                PlaySE(SE_SELECT);
                sMainMenuLastSelectedOption = SELECTED_NEW_GAME;
                sMainMenuSelectedOption = SELECTED_CONTINUE;
                LoadMainMenuTilemap(sMainMenuSelectedOption);
                ToggleGrayscaleAndAnims();
            }
            if (gMain.newKeys & DPAD_RIGHT)
            {
                PlaySE(SE_SELECT);
                sMainMenuSelectedOption = SELECTED_OPTIONS;
                LoadMainMenuTilemap(sMainMenuSelectedOption);
            }
            break;
        case SELECTED_OPTIONS:
            if (gMain.newKeys & A_BUTTON)
            {
                PlaySE(SE_SELECT);
                FadeScreen(FADE_TO_BLACK, 0);              
                gMain.savedCallback = CB2_OpenMainMenuCustom;
                sMainMenuExitCallback = CB2_InitOptionMenu;
                gTasks[taskId].func = Task_CloseMainMenu;
            }
            if (gMain.newKeys & DPAD_UP)
            {
                PlaySE(SE_SELECT);
                sMainMenuLastSelectedOption = SELECTED_OPTIONS;
                sMainMenuSelectedOption = SELECTED_CONTINUE;
                LoadMainMenuTilemap(sMainMenuSelectedOption);
                ToggleGrayscaleAndAnims();
            }
            if (gMain.newKeys & DPAD_LEFT)
            {
                PlaySE(SE_SELECT);
                sMainMenuSelectedOption = SELECTED_NEW_GAME;
                LoadMainMenuTilemap(sMainMenuSelectedOption);
            }
            break;
    }

    // Exit is universal for all options.
    if (gMain.newKeys & B_BUTTON)
    {
        FadeScreen(FADE_TO_BLACK, 0);
        sMainMenuExitCallback = CB2_InitTitleScreen;
        gTasks[taskId].func = Task_CloseMainMenu;
        PlaySE(SE_SELECT);
    }
}

static void Task_CloseMainMenu(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        // Change transparency color to black so fade out looks better.
        u16 palette = RGB_BLACK;
        LoadPalette(&palette, BG_PLTT_ID(0), PLTT_SIZEOF(1));

        // Free tilemap pointer and data.
        Free(sMainMenuTilemapPtr);
        sMainMenuTilemapPtr = NULL;
        Free(sEnvironmentTilemapPtr);
        sEnvironmentTilemapPtr = NULL;

        // Reset data and destroy task.
        FreeAllWindowBuffers();
        ResetSpriteData();
        UnfreezeObjectEvents();
        DestroyTask(taskId);

        // Return to title screen.
        SetMainCallback2(sMainMenuExitCallback);
    }
}

static void LoadMainMenuGfx(void)
{
    u16 palette = RGB(0, 0, 0);
    LoadPalette(&palette, BG_PLTT_ID(15) + 2, PLTT_SIZEOF(1));
    CreateMainMenuWindows();
    PrintPlayerInfo();
    DrawPlayerSprite();
    DrawPartyIcons();
    LoadMainMenuTilemap(sMainMenuSelectedOption);
    if (sMainMenuSelectedOption != SELECTED_CONTINUE)
        ToggleGrayscaleAndAnims();
}

static void CreateMainMenuWindows(void)
{
    u32 i, windowId;
    for (i = 0; i < WINDOW_COUNT; ++i)
    {
        windowId = AddWindow(&sMainMenuWinTemplates[i]);
        PutWindowTilemap(windowId);
        CopyWindowToVram(i, COPYWIN_FULL);
    }
}

static const u8 sTextColor_Normal[] = {0, 1, 0}; // white text, transparent bg and shadow
static const u8 sTextColor_Deselected[] = {0, 3, 0}; // gray text, transparent bg and shadow

static void PrintPlayerInfo(void)
{
    u8 str[20];
    u8 *strPtr;
    const u8 *textColor;
    if (sMainMenuSelectedOption == SELECTED_CONTINUE)
        textColor = sTextColor_Normal;
    else
        textColor = sTextColor_Deselected;

    // Print player name.
    AddTextPrinterParameterized3(WINDOW_PLAYER_INFO, FONT_NORMAL, 21, 14, textColor, TEXT_SKIP_DRAW, gSaveBlock2Ptr->playerName);
    CopyWindowToVram(WINDOW_PLAYER_INFO, COPYWIN_FULL);

    // Print location.
    const struct MapHeader *mapHeader = Overworld_GetMapHeaderByGroupAndId(gSaveBlock1Ptr->location.mapGroup, gSaveBlock1Ptr->location.mapNum);
    GetMapNameGeneric(str, mapHeader->regionMapSectionId);
    AddTextPrinterParameterized3(WINDOW_PLAYER_INFO, FONT_NORMAL, 1, 26, textColor, TEXT_SKIP_DRAW, str);
    
    // Print dex count.
    ConvertIntToDecimalStringN(str, GetNationalPokedexCount(FLAG_GET_CAUGHT), STR_CONV_MODE_LEFT_ALIGN, 3);
    AddTextPrinterParameterized3(WINDOW_PLAYER_INFO, FONT_NORMAL, 64, 37, textColor, TEXT_SKIP_DRAW, str);

    // Print play time.
    ConvertIntToDecimalStringN(str, gSaveBlock2Ptr->playTimeHours, STR_CONV_MODE_LEADING_ZEROS, 2);
    strPtr = &str[2];
    *strPtr = CHAR_COLON;
    ++strPtr;
    ConvertIntToDecimalStringN(strPtr, gSaveBlock2Ptr->playTimeMinutes, STR_CONV_MODE_LEADING_ZEROS, 2);
    AddTextPrinterParameterized3(WINDOW_PLAYER_INFO, FONT_NORMAL, 40, 49, textColor, TEXT_SKIP_DRAW, str);
    CopyWindowToVram(WINDOW_PLAYER_INFO, COPYWIN_FULL);
}

static void DrawPlayerSprite(void)
{
    if (gSaveBlock2Ptr->playerGender == MALE)
        sPlayerSpriteId = CreateObjectGraphicsSprite(OBJ_EVENT_GFX_BRENDAN_NORMAL, SpriteCallbackDummy, 24, 41, 0);
    else
        sPlayerSpriteId = CreateObjectGraphicsSprite(OBJ_EVENT_GFX_MAY_NORMAL, SpriteCallbackDummy, 24, 41, 0);
    SetAndStartSpriteAnim(&gSprites[sPlayerSpriteId], ANIM_STD_GO_SOUTH, 0);
}

static void DrawPartyIcons(void)
{
    u32 i, species;
    for (i = 0; i < PARTY_SIZE; ++i)
    {
        species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES);
        if (species == SPECIES_NONE)
        {
            sPartySpriteIds[i] = 0xFF;
        }
        else
        {
            LoadMonIconPalette(species);
            sPartySpriteIds[i] = CreateMonIconNoPersonality(GetIconSpeciesNoPersonality(species),
                                    SpriteCB_MonIcon, 152 + 24*(i%3), 51 + 24*(i/3), 0);
        }
    }
}

static void ToggleGrayscaleAndAnims(void)
{
    u32 i;
    PrintPlayerInfo(); // reprint text
    if (sMainMenuSelectedOption == SELECTED_CONTINUE)
    {
        SetAndStartSpriteAnim(&gSprites[sPlayerSpriteId], ANIM_STD_GO_SOUTH, 0);
        SetGrayscaleOrOriginalPalette(16 + gSprites[sPlayerSpriteId].oam.paletteNum, TRUE);
        for (i = 0; i < PARTY_SIZE; ++i)
        {
            gSprites[sPartySpriteIds[i]].callback = SpriteCB_MonIcon;
            SetGrayscaleOrOriginalPalette(16 + gSprites[sPartySpriteIds[i]].oam.paletteNum, TRUE);
        }
    }
    else
    {
        SetAndStartSpriteAnim(&gSprites[sPlayerSpriteId], ANIM_STD_FACE_SOUTH, 0);
        SetGrayscaleOrOriginalPalette(16 + gSprites[sPlayerSpriteId].oam.paletteNum, FALSE);
        for (i = 0; i < PARTY_SIZE; ++i)
        {
            gSprites[sPartySpriteIds[i]].callback = SpriteCallbackDummy;
            SetGrayscaleOrOriginalPalette(16 + gSprites[sPartySpriteIds[i]].oam.paletteNum, FALSE);
        }
    }
}

static void LoadMainMenuTilemap(u32 selectedId)
{
    const u32 *tilemap;
    switch (selectedId)
    {
        default:
        case SELECTED_CONTINUE:
            tilemap = sMainMenuContinueTilemap;
            break;
        case SELECTED_NEW_GAME:
            tilemap = sMainMenuNewGameTilemap;
            break;
        case SELECTED_OPTIONS:
            tilemap = sMainMenuOptionsTilemap;
            break;
    }
    LZDecompressWram(tilemap, sMainMenuTilemapPtr);
    CopyBgTilemapBufferToVram(2);
}
