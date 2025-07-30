#include "global.h"
#include "battle.h"
#include "bg.h"
#include "title_screen.h"
#include "sprite.h"
#include "gba/m4a_internal.h"
#include "clear_save_data_menu.h"
#include "decompress.h"
#include "event_data.h"
#include "intro.h"
#include "m4a.h"
#include "main.h"
#include "main_menu.h"
#include "main_menu_custom.h"
#include "malloc.h"
#include "menu.h"
#include "palette.h"
#include "reset_rtc_screen.h"
#include "berry_fix_program.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "scanline_effect.h"
#include "gpu_regs.h"
#include "trig.h"
#include "graphics.h"
#include "constants/rgb.h"
#include "constants/songs.h"

#define CLEAR_SAVE_BUTTON_COMBO (B_BUTTON | SELECT_BUTTON | DPAD_UP)
#define RESET_RTC_BUTTON_COMBO (B_BUTTON | SELECT_BUTTON | DPAD_LEFT)
#define BERRY_UPDATE_BUTTON_COMBO (B_BUTTON | SELECT_BUTTON)
#define A_B_START_SELECT (A_BUTTON | B_BUTTON | START_BUTTON | SELECT_BUTTON)

// const rom data
static const struct BgTemplate sTitleScreenBgTemplates[] =
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
static const u32 sTitleScreenGfx[] = INCBIN_U32("graphics/title_screen/tiles.4bpp.lz");
static const u32 sTitleScreenTilemap[] = INCBIN_U32("graphics/title_screen/map.bin.lz");
static const u16 sTitleScreenPal[] = INCBIN_U16("graphics/title_screen/tiles.gbapal");

// ewram data
EWRAM_DATA static u32 *sTitleScreenTilemapPtr = NULL;
EWRAM_DATA static u8 sFrameCounter = 0;

// forward declarations
static void MainCB2_TitleScreen(void);
static void VBlankCB2_TitleScreen(void);
static void Task_OpenTitleScreen(u8 taskId);
static void Task_TitleScreenWaitForKeypress(u8 taskId);
static void CB2_GoToMainMenu(void);
static void CB2_GoToClearSaveDataScreen(void);
static void CB2_GoToResetRtcScreen(void);
static void CB2_GoToBerryFixScreen(void);
static void UpdateLegendaryMarkingColor(u8);

// UI functions
static void MainCB2_TitleScreen(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VBlankCB2_TitleScreen(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void CB2_InitTitleScreen(void)
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
            sTitleScreenTilemapPtr = AllocZeroed(BG_SCREEN_SIZE);
            ResetBgsAndClearDma3BusyFlags(0);
            InitBgsFromTemplates(0, sTitleScreenBgTemplates, ARRAY_COUNT(sTitleScreenBgTemplates));
            SetBgTilemapBuffer(2, sTitleScreenTilemapPtr);
            gMain.state++;
            break;
        case 4:
            DecompressAndCopyTileDataToVram(2, sTitleScreenGfx, 0, 0, 0);
            LZDecompressWram(sTitleScreenTilemap, sTitleScreenTilemapPtr);
            LoadPalette(sTitleScreenPal, BG_PLTT_ID(0), PLTT_SIZE_4BPP);
            Menu_LoadStdPalAt(BG_PLTT_ID(15));
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
                gMain.state++;
            }
            break;
        case 6:
            PlayBGM(MUS_TITLE);
            SetVBlankCallback(VBlankCB2_TitleScreen);
            CreateTask(Task_OpenTitleScreen, 0);
            SetMainCallback2(MainCB2_TitleScreen);
            break;
    }
}

static void Task_OpenTitleScreen(u8 taskId)
{
    sFrameCounter = 0;
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_TitleScreenWaitForKeypress;
}

static void Task_TitleScreenWaitForKeypress(u8 taskId)
{
    UpdateLegendaryMarkingColor(++sFrameCounter);
    if (JOY_NEW(A_BUTTON) || JOY_NEW(START_BUTTON))
    {
        FadeOutBGM(4);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        SetMainCallback2(CB2_GoToMainMenu);
    }
    else if (JOY_HELD(CLEAR_SAVE_BUTTON_COMBO) == CLEAR_SAVE_BUTTON_COMBO)
    {
        SetMainCallback2(CB2_GoToClearSaveDataScreen);
    }
    else if (JOY_HELD(RESET_RTC_BUTTON_COMBO) == RESET_RTC_BUTTON_COMBO
      && CanResetRTC() == TRUE)
    {
        FadeOutBGM(4);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        SetMainCallback2(CB2_GoToResetRtcScreen);
    }
    else if (JOY_HELD(BERRY_UPDATE_BUTTON_COMBO) == BERRY_UPDATE_BUTTON_COMBO)
    {
        FadeOutBGM(4);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        SetMainCallback2(CB2_GoToBerryFixScreen);
    }
}

static void CB2_GoToMainMenu(void)
{
    if (!UpdatePaletteFade())
        SetMainCallback2(CB2_OpenMainMenuCustom);
}

static void CB2_GoToClearSaveDataScreen(void)
{
    if (!UpdatePaletteFade())
        SetMainCallback2(CB2_InitClearSaveDataScreen);
}

static void CB2_GoToResetRtcScreen(void)
{
    if (!UpdatePaletteFade())
        SetMainCallback2(CB2_InitResetRtcScreen);
}

static void CB2_GoToBerryFixScreen(void)
{
    if (!UpdatePaletteFade())
    {
        m4aMPlayAllStop();
        SetMainCallback2(CB2_InitBerryFixProgram);
    }
}

static void UpdateLegendaryMarkingColor(u8 frameNum)
{
    if ((frameNum % 4) == 0) // Change color every 4th frame
    {
        for (u32 i = 3; i <= 12; ++i)
        {
            if (i == 11)
                continue;

            s32 intensity = Cos(frameNum, 4);
            s32 r = (gPlttBufferUnfaded[i] & 31) + intensity;
            s32 g = ((gPlttBufferUnfaded[i] & (31 << 5)) >> 5) + intensity;
            s32 b = ((gPlttBufferUnfaded[i] & (31 << 10)) >> 10); //+ (frameNum % 3) - 1;

            u16 color = RGB(r, g, b);
            gPlttBufferFaded[i] = color;
        }
    }
}
