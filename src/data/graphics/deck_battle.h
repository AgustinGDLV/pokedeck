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

const u8 sShadowGfx[] = INCBIN_U8("graphics/deck_battle_interface/shadow.4bpp");
const u8 sCursorGfx[] = INCBIN_U8("graphics/deck_battle_interface/cursor.4bpp");
const u16 sMiscGfxPal[] = INCBIN_U16("graphics/deck_battle_interface/shadow.gbapal");

static const struct SpriteSheet sShadowSpriteSheet = { sShadowGfx, sizeof(sShadowGfx), TAG_SHADOW };
static const struct SpritePalette sMiscGfxSpritePalette = { sMiscGfxPal, TAG_MISC_PAL };
static const struct SpriteTemplate sShadowSpriteTemplate =
{
	.tileTag = TAG_SHADOW,
	.paletteTag = TAG_MISC_PAL,
	.oam = &sOamData_32x32,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
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

static const union AnimCmd sAnim_None[] =
{
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Idle[] =
{
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(16, 8),
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Attack[] =
{
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(16, 8),
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnims_Battler[] =
{
    sAnim_None,
    sAnim_Idle,
    sAnim_Attack,
};
