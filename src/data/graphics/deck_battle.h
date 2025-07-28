static const u16 sDeckBattleInterfacePalette[] = INCBIN_U16("graphics/deck_battle_interface/tiles.gbapal");
static const u32 sDeckBattleInterfaceTiles[] = INCBIN_U32("graphics/deck_battle_interface/tiles.4bpp.lz");
static const u32 sDeckBattleInterfaceTilemap[] = INCBIN_U32("graphics/deck_battle_interface/map.bin.lz");

const u16 gGrassBackgroundPalette[] = INCBIN_U16("graphics/deck_battle_backgrounds/grass_tiles.gbapal");
const u32 gGrassBackgroundTiles[] = INCBIN_U32("graphics/deck_battle_backgrounds/grass_tiles.4bpp.lz");
const u32 gGrassBackgroundTilemap[] = INCBIN_U32("graphics/deck_battle_backgrounds/grass_map.bin.lz");

const struct OamData sOamData_8x8 =
{
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x8),
    .size = SPRITE_SIZE(8x8),
    .priority = 0,
};

const struct OamData sOamData_Battler =
{
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 1,
};

const struct OamData sOamData_Shadow =
{
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 1,
};

const struct OamData sOamData_Portrait =
{
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .size = SPRITE_SIZE(64x64),
    .priority = 2,
};

const u8 sShadowGfx[] = INCBIN_U8("graphics/deck_battle_interface/shadow.4bpp");
const u8 sCursorGfx[] = INCBIN_U8("graphics/deck_battle_interface/cursor.4bpp");
const u8 sNumberGfx[] = INCBIN_U8("graphics/deck_battle_interface/number.4bpp");
const u16 sMiscGfxPal[] = INCBIN_U16("graphics/deck_battle_interface/shadow.gbapal");

static const struct SpriteSheet sShadowSpriteSheet = { sShadowGfx, sizeof(sShadowGfx), TAG_SHADOW };
static const struct SpritePalette sMiscGfxSpritePalette = { sMiscGfxPal, TAG_MISC_PAL };
static const struct SpriteTemplate sShadowSpriteTemplate =
{
	.tileTag = TAG_SHADOW,
	.paletteTag = TAG_MISC_PAL,
	.oam = &sOamData_Shadow,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_Shadow,
};

static const struct SpriteSheet sCursorSpriteSheet = { sCursorGfx, sizeof(sCursorGfx), TAG_CURSOR };
static const struct SpriteTemplate sCursorSpriteTemplate =
{
	.tileTag = TAG_CURSOR,
	.paletteTag = TAG_MISC_PAL,
	.oam = &sOamData_8x8,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_Cursor,
};

static const union AnimCmd sAnim_Number0[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END,
};
static const union AnimCmd sAnim_Number1[] =
{
    ANIMCMD_FRAME(1, 1),
    ANIMCMD_END,
};
static const union AnimCmd sAnim_Number2[] =
{
    ANIMCMD_FRAME(2, 1),
    ANIMCMD_END,
};
static const union AnimCmd sAnim_Number3[] =
{
    ANIMCMD_FRAME(3, 1),
    ANIMCMD_END,
};
static const union AnimCmd sAnim_Number4[] =
{
    ANIMCMD_FRAME(4, 1),
    ANIMCMD_END,
};
static const union AnimCmd sAnim_Number5[] =
{
    ANIMCMD_FRAME(5, 1),
    ANIMCMD_END,
};
static const union AnimCmd sAnim_Number6[] =
{
    ANIMCMD_FRAME(6, 1),
    ANIMCMD_END,
};
static const union AnimCmd sAnim_Number7[] =
{
    ANIMCMD_FRAME(7, 1),
    ANIMCMD_END,
};
static const union AnimCmd sAnim_Number8[] =
{
    ANIMCMD_FRAME(8, 1),
    ANIMCMD_END,
};
static const union AnimCmd sAnim_Number9[] =
{
    ANIMCMD_FRAME(9, 1),
    ANIMCMD_END,
};
static const union AnimCmd *const sAnims_Number[] =
{
    sAnim_Number0,
    sAnim_Number1,
    sAnim_Number2,
    sAnim_Number3,
    sAnim_Number4,
    sAnim_Number5,
    sAnim_Number6,
    sAnim_Number7,
    sAnim_Number8,
    sAnim_Number9,
};

static const struct SpriteSheet sNumberSpriteSheet = { sNumberGfx, sizeof(sNumberGfx), TAG_NUMBER };
static const struct SpriteTemplate sNumberSpriteTemplate =
{
	.tileTag = TAG_NUMBER,
	.paletteTag = TAG_MISC_PAL,
	.oam = &sOamData_8x8,
	.anims = sAnims_Number,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_DamageNumber,
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

static const union AnimCmd sAnim_Attack[] =
{
    ANIMCMD_FRAME(0, 16),
    ANIMCMD_FRAME(16, 16),
    ANIMCMD_FRAME(0, 16),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Hurt[] =
{
    ANIMCMD_FRAME(0, 32),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnims_Battler[] =
{
    sAnim_Paused,
    sAnim_Idle,
    sAnim_Attack,
    sAnim_Hurt,
};
