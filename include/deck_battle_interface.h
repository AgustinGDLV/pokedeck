#ifndef GUARD_DECK_BATTLE_INTERFACE_H
#define GUARD_DECK_BATTLE_INTERFACE_H

#define TAG_PORTRAIT    2000
#define TAG_MISC_PAL    2001 // shadow and cursor
#define TAG_SHADOW      2001
#define TAG_CURSOR      2002
#define TAG_NUMBER      2003
#define TAG_BATTLER_OBJ 2004 // 2004-2015

#define ANIM_PAUSED 0
#define ANIM_IDLE   1
#define ANIM_ATTACK 2
#define ANIM_HURT   3

#define PORTRAIT_SIZE   (64 * 64 / 2)
#define OBJECT_SIZE     (32 * 64 / 2)

#define PORTRAIT_X      36
#define PORTRAIT_Y      126
#define PLAYER_OBJ_X    28
#define PLAYER_OBJ_Y    96
#define OPPONENT_OBJ_X  52
#define OPPONENT_OBJ_Y  56
#define OBJ_OFFSET      32

struct DeckBattleGraphics
{
    u8 battlerSpriteIds[MAX_DECK_BATTLERS_COUNT];
    u8 shadowSpriteIds[MAX_DECK_BATTLERS_COUNT];
    u8 portraitSpriteId;
    u8 bobTaskId;
};

void ClearDeckBattleGraphicsStruct(void);
void LoadBattleBoxesAndBackground(void);
void InitDeckBattleWindows(void);
void InitDeckBattleGfx(void);

void SetBattlerBobPause(bool32 pause);
bool32 IsBattlerBobActive(void);
void LoadBattlerPortrait(enum BattleId battler);
void SetBattlerPortraitVisibility(bool32 visible);
void LoadBattlerObjectSprite(enum BattleId battler);
void CreateSelectionCursorOverBattler(enum BattleId battler);
void RemoveSelectionCursorOverBattler(enum BattleId battler);
void SetBattlerGrayscale(enum BattleId battler, bool32 grayscale);
void StartBattlerAnim(enum BattleId battler, u32 animId);
struct Sprite * GetBattlerSprite(enum BattleId battler);
u32 GetBattlerXCoord(enum BattleId battler);
u32 GetBattlerYCoord(enum BattleId battler);

void PrintBattlerMoveInfo(enum BattleId battler);
void PrintTargetBattlerPrompt(enum BattleId battler);
void PrintMoveUseString(void);
void PrintMoveOutcomeString(void);
void PrintSwapTargetPrompt(enum BattleId battler);
void PrintSwapString(enum BattleId battler1, enum BattleId battler2);

void UpdatePlayerHPBar(enum BattleId battler);
void PrintDamageNumbers(enum BattleId battler, s32 damage);

extern struct DeckBattleGraphics gDeckGraphics;

#endif
