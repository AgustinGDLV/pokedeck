#include "global.h"
#include "battle.h"
#include "bg.h"
#include "deck_battle.h"
#include "deck_battle_interface.h"
#include "event_object_movement.h"
#include "gpu_regs.h"
#include "m4a.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "overworld.h"
#include "palette.h"
#include "sound.h"
#include "task.h"
#include "text.h"
#include "util.h"
#include "constants/abilities.h"
#include "constants/moves.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/species.h"

// forward declarations
static void MainCB2_DeckBattle(void);
static void VBlankCB2_DeckBattle(void);
static void Task_OpenDeckBattle(u8 taskId);
static void Task_CloseDeckBattle(u8 taskId);
static void Task_DeckBattleWaitForKeypress(u8 taskId);

// ewram data
EWRAM_DATA struct DeckBattleStruct gDeckBattleStruct = {0};
EWRAM_DATA struct DeckBattlePokemon gDeckBattleMons[MAX_DECK_BATTLERS_COUNT] = {0};

// code
#include "data/graphics/deck_pokemon.h"
#include "data/pokemon/deck_species_info.h"

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
            ClearDeckBattleGraphicsStruct();
            gMain.state++;
            break;
        case 2:
            LoadBattleBoxesAndBackground();
            ShowBg(0);
            HideBg(1);
            HideBg(2);
            ShowBg(3);
            gMain.state++;
            break;
        case 3:
            InitDeckBattleWindows();
            DeactivateAllTextPrinters();
            gMain.state++;
            break;
        case 4:
            InitDeckBattleGfx();
            gMain.state++;
        case 5:
            for (enum BattleId battler = 0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
            {
                gDeckBattleMons[battler].species = SPECIES_SLOWPOKE;
                LoadBattlerObjectSprite(battler);
            }
            LoadBattlerPortrait(B_PLAYER_0);
            gMain.state++;
        case 6:
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
