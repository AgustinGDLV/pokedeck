#ifndef GUARD_DECK_BATTLE_INTERFACE_H
#define GUARD_DECK_BATTLE_INTERFACE_H

#define TAG_PORTRAIT    2000
#define TAG_MISC_PAL    2001 // shadow and cursor
#define TAG_SHADOW      2001
#define TAG_CURSOR      2002
#define TAG_BATTLER_OBJ 2003 // 2003-2014

#define ANIM_IDLE   0
#define ANIM_ATTACK 1

#define PORTRAIT_SIZE   (64 * 64 / 2)
#define OBJECT_SIZE     (32 * 64 / 2)

#define PORTRAIT_X      36
#define PORTRAIT_Y      124
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
    u8 cursorSpriteId;
};

void ClearDeckBattleGraphicsStruct(void);
void LoadBattleBoxesAndBackground(void);
void InitDeckBattleWindows(void);
void InitDeckBattleGfx(void);
void LoadBattlerPortrait(enum BattleId battler);
void LoadBattlerObjectSprite(enum BattleId battler);

extern struct DeckBattleGraphics gDeckBattleGraphics;

#endif
