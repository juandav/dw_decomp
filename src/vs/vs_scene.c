#include <string.h>

#include <libetc.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/clock.h>
#include <dw/font.h>
#include <dw/main.h>
#include <dw/math.h>
#include <dw/model.h>
#include <dw/params.h>
#include <dw/sound.h>
#include <dw/types.h>
#include <dw/vs.h>

#include "common.h"

typedef struct {
	int16_t unk0;
	int16_t unk2;
	int16_t unk4;
	int16_t unk6;
	int16_t unk8;
	int16_t unkA;
} StdCameraPreset;

typedef struct {
	int16_t hp;
	int16_t mp;
	int16_t off;
	int16_t def;
	int16_t speed;
	int16_t brain;
	int16_t weight;
	int16_t unk0E;
	uint8_t unk10[0xC];
	uint8_t type;
	uint8_t moves[3];
	uint8_t unk20[0x20];
} VsFighterCard;

typedef struct {
	int16_t w;
	int16_t h;
	int16_t dx;
	int16_t dy;
	uint8_t clut;
	uint8_t u;
	uint8_t v;
	uint8_t uw;
	uint8_t uh;
	uint8_t tpage;
} VsTextPiece;

typedef struct {
	int16_t timer;
	int8_t phase;
	int8_t side;
} CameraChase;

void calculateBoneMatrix(Entity *entity, int32_t boneId, MATRIX *out);
int32_t MAIN_func_800DA63C(int32_t a, int32_t b, int16_t *c, int16_t *d, int16_t *e);
int32_t lerp(int32_t a, int32_t b, int32_t lo, int32_t hi, int32_t t);
int32_t getDistance(int32_t x, int32_t y, int32_t z);
void GsGetTimInfo(unsigned long *tim, GsIMAGE *img);
void readFile(char *path, void *dest);
void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount, int32_t *digits);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY, int32_t width, int32_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t uvX, int32_t uvY, int32_t width, int32_t height);
void VS__tickInput(void);
void VS_resetMatchState(void);
void VS_loadVSAssets(int32_t arena);
void VS_addInputObjects(void);
void VS_loadFighterEntities(uint8_t slot);
void VS_unloadFighterEntities(void);
void VS_renderVersusFlash(void);
void VS_renderFighterNamePlate(int16_t side);
void VS_renderRoundPips(int32_t slot);
void VS_renderRoundScores(int16_t slot);
void VS_renderVersusBanner(int32_t slot);
void VS_renderVersusText(void);
void VS_initializeCamera(void);
void VS_initializeLighting(int32_t mode);
void VS_loadTIMToVRAM(char *path);
int32_t VS_getFirstSpecialMove(int32_t index);
void VS_playVersusIntroSequence(void);
void VS_tickPlaytime(void);
void VS_tickVersusIntro(void);
void VS_renderVersusIntro(void);
void VS_loadArenaTIMToVRAM(char *path, int32_t count);
void VS_loadArenaAssets(void);
void VS_unloadArenaAssets(void);
void VS_addArenaRenderers(void);
void VS_renderArenaViewLeft(void);
void VS_renderArenaViewRight(void);
void VS_renderArenaViewFull(void);
void VS_removeArenaRenderers(void);
void VS_applyCamera(void);
void VS_setCameraOrbit(void);
void VS_setCameraYXZ(void);
void VS_setCameraToEntity(void);
void VS_setViewpointRotationFromEntity(void);
void VS_setCameraLookAtEntity(void);
void VS_applyViewpoint(void);
void VS_setCameraSimple(void);
void VS_setCameraParams(int16_t a, int16_t b, int16_t c, int16_t d, int16_t e, int16_t f);
void VS_setVSPhase(int32_t arg);
void VS_tickVSPhase(void);
void VS_removeVSPhase(void);
int32_t VS_getFighterDistance(VECTOR *self, VECTOR *other, VECTOR *target);
void VS_setViewpointFromBone(Entity *entity, SVECTOR *offset, SVECTOR *rot, int32_t dist);
void VS_updateCameraLerp(int32_t t, int32_t flip);
int32_t VS_isPositionNearEntity(Entity *entity, VECTOR *pos);
int32_t VS_interpolateClamped2(int32_t lo, int32_t hi, int32_t t, int32_t start, int32_t end);
void VS_tickCameraChase(void);
void startAnimation(Entity *entity, int32_t animId);
void VS_tickCameraIntro(void);
void VS_startCameraIntro(Entity *target, Entity *entity);
void VS_addResultModelScene(Entity *entity);
void VS_removeCameraIntro(void);
void VS_applyEntityViewpoint(void);
void VS_renderCounterDigits(int32_t x, int32_t y, int32_t digits, int32_t value, int32_t layer);
void VS_tickFighterCounter(void);
void VS_renderFighterCounter(void);
void renderNumber(int32_t a, int32_t x, int32_t y, int32_t digits, int32_t value, int32_t layer);
void VS__tickDigimonP1(int32_t instanceId);
void VS__tickDigimonP2(int32_t instanceId);
int32_t loadVSSounds(void);
int32_t loadDigimonSounds(int32_t vabId, int32_t type);
void VS__func_800F23D0(int32_t stage);
int32_t VS__combatMain(void);
void initializeBuffModel(TMDModel *model);
void VS_loadStageModels(void);
void VS_initializeFinisherAuraModel(char *tim, char *base);
void VS_initializePoisonBubble(void);
void VS_initializeConfusionEffect(char *base);
void VS_initializeStunEffect(char *base);
int32_t loadTIMFile(char *path, void *buffer);
void renderNumber(int32_t a, int32_t x, int32_t y, int32_t digits, int32_t value, int32_t layer);
void initStringFT4(POLY_FT4 *p);
int32_t VS_initializeLighting__garbage__(int32_t i);
void fadeFromBlack(int32_t frames);
void fadeToBlack(int32_t frames);
void VS_setVersusModelSceneTimer(int16_t value);
int32_t customRandom(int32_t lo, int32_t hi);
void MAIN_func_800DA60C(int32_t n, int16_t *a, int16_t *b, int16_t *c);

extern uint8_t MAIN_D_80135258[2];
extern uint8_t MAIN_D_8013525A[2];
extern uint8_t MAIN_D_80135260;
extern int16_t MAIN_D_8013525C;
extern int16_t MAIN_D_8013525E;
extern uint8_t MAIN_D_80135261;
extern uint8_t MAIN_D_80135262;
extern int32_t MAIN_D_80135268;
extern GsVIEW2 MAIN_D_801B1B98;
extern int32_t MAIN_D_801B1BBC[];
extern char **MAIN_D_80135298;
extern int8_t MAIN_D_8013529C;
extern SVECTOR MAIN_D_801B1C0C[];
extern VECTOR MAIN_D_801B1C14;
extern GsRVIEW2 GS_VIEWPOINT;
extern int16_t VS_D_80071A0C[];
extern uint8_t MAIN_D_80135288;
extern int32_t MAIN_D_801352A8;
extern int32_t VIEWPORT_DISTANCE;
extern SVECTOR MAIN_D_80134AB4;
extern SVECTOR MAIN_D_80134A84;
extern SVECTOR MAIN_D_80134A8C;
extern int16_t VS_D_800706C8[];
extern int16_t VS_D_80071A0E[];
extern int16_t VS_D_80071A10[];
extern int16_t VS_D_80071A12[];
extern Entity *VS_D_80071A18[];
extern int16_t VS_D_80071A30[];
extern int32_t VS_D_80071A34[];
extern int32_t VS_D_80071A38[];
extern int32_t VS_D_80071A3C[];
extern int32_t VS_D_80071A40[];
extern int16_t VS_D_80071A88[];
extern int16_t VS_D_80071AE0[];
extern int32_t VS_D_80071AE4[];
extern int32_t VS_D_80071AE8[];
extern int32_t VS_D_80071AEC[];
extern int32_t VS_D_80071AF0[];
extern int16_t MAIN_D_80134D66;
extern int32_t MAIN_D_8013528C;
extern SVECTOR MAIN_D_80134AA4;
extern SVECTOR MAIN_D_80134AAC;
extern int16_t VS_D_800706A8[];
extern int16_t VS_D_800706AA[];
extern int16_t VS_D_800706AC[];
extern GsOT *ACTIVE_ORDERING_TABLE;
extern GsF_LIGHT LIGHT_DATA[];
extern GsOT VS_D_800716D4[];
extern GsOT VS_D_800716FC[];
extern int32_t ACTIVE_FRAMEBUFFER;
extern StdCameraPreset VS_D_8007063C[];
extern GsCOORDINATE2 VS_D_8007188C[];
extern GsDOBJ2 VS_D_800719CC[];
extern int16_t MAIN_D_801352A4;
extern char *MAIN_D_8013526C;
extern char *MAIN_D_80135270;
extern int16_t MAIN_D_80135278;
extern int16_t MAIN_D_8013527A;
extern int8_t VS_D_80071690[2][5];
extern uint8_t VS_D_8007169C[2][5];
extern uint8_t MAIN_D_80135259;
extern uint8_t MAIN_D_8013525B;
extern int8_t VS_D_80071695[];
extern uint8_t VS_D_800716A1[];
extern int16_t MAIN_D_80135264;
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
extern GsOT_TAG VS_D_800716B4[];
extern GsOT_TAG VS_D_800716C4[];
extern GsOT_TAG VS_D_80071724[];
extern GsOT_TAG VS_D_80071734[];
extern char VS_D_8006FB94[];
extern char VS_D_8006FBA8[];
extern uint8_t MAIN_D_80135274;
extern uint8_t VS_D_8006FA28[];
extern uint16_t VS_D_8006FBC0[];
extern int16_t VS_D_8006FD70[];
extern VsTextPiece VS_D_8006FA6C[21];
extern char MAIN_D_80134A50[4];
extern char MAIN_D_80134A54[4];
extern char MAIN_D_80124C54[];
extern int16_t MAIN_D_80135264;
extern uint8_t MAIN_D_80135259;
extern uint8_t MAIN_D_80135262;
extern char *VS_D_800705FC[];
extern char *VS_D_800705B4[];
extern char *VS_D_80070630[];
extern uint8_t MAIN_D_80134A60[4];
extern uint8_t MAIN_D_80134A5C[4];
extern char TAMER_MODEL_BUFFER[];
extern void *MAIN_D_801352A0;
extern int8_t MAP_COLLISION_DATA[];
extern int16_t MAIN_D_80135294;
extern int32_t MAIN_D_80135284;
extern MATRIX MAIN_D_801B1BC0;
extern VECTOR VS_D_80071754;
extern SVECTOR MAIN_D_80134A64;
extern SVECTOR MAIN_D_80134A6C;
extern int32_t MAIN_D_80135268;
extern char **MAIN_D_80135298;
extern int16_t MAIN_D_801352A4;
extern VECTOR VS_D_80071754;
extern int8_t MAIN_D_801352A7;
extern SVECTOR MAIN_D_80134A74;
extern SVECTOR MAIN_D_80134A7C;
extern int16_t VS_D_80071A14[];
extern int16_t VS_D_80071A16[];
extern Entity *VS_D_80071A1C[];
extern SVECTOR MAIN_D_80134A94;
extern SVECTOR MAIN_D_80134A9C;

static void *vs_scene_functions[] = {
	VS_removeFighterCounter,
	VS_renderFighterCounter,
	VS_tickFighterCounter,
	VS_addFighterCounter,
	VS_renderCounterDigits,
	VS_applyEntityViewpoint,
	VS_removeCameraIntro,
	VS_startCameraIntro,
	VS_tickCameraIntro,
	VS_startCameraChase,
	VS_tickCameraChase,
	VS_interpolateClamped2,
	VS_isPositionNearEntity,
	VS_updateCameraLerp,
	VS_setViewpointFromBone,
	VS_getFighterDistance,
	VS_setRandomViewpoint,
	VS_selectRandomCamera,
	VS_removeVSPhase,
	VS_tickVSPhase,
	VS_setVSPhase,
	VS_setCameraParams,
	VS_setCameraSimple,
	VS_applyViewpoint,
	VS_setCameraLookAtEntity,
	VS_setViewpointRotationFromEntity,
	VS_setCameraToEntity,
	VS_setCameraYXZ,
	VS_setCameraOrbit,
	VS_applyCamera,
	VS_removeArenaRenderers,
	VS_renderArenaViewFull,
	VS_renderArenaViewRight,
	VS_renderArenaViewLeft,
	VS_addArenaRenderers,
	VS_unloadArenaAssets,
	VS_loadArenaAssets,
	VS_loadArenaTIMToVRAM,
	VS_renderVersusIntro,
	VS_tickVersusIntro,
	VS_tickPlaytime,
	VS_playVersusIntroSequence,
	VS_initializeVS,
	VS_getFirstSpecialMove,
	VS_loadTIMToVRAM,
	VS_initializeLighting,
	VS_initializeCamera,
	VS_renderVersusText,
	VS_renderVersusBanner,
	VS_renderRoundScores,
	VS_renderRoundPips,
	VS_renderFighterNamePlate,
	VS_renderVersusFlash,
	VS_unloadFighterEntities,
	VS_loadFighterEntities,
	VS_addInputObjects,
	VS_loadVSAssets,
	VS_resetMatchState,
};

void VS_initializeVS(void)
{
	int16_t result;

	VS_resetMatchState();
	VS_loadVSAssets(VS_D_800716B2[0]);
	VS_addInputObjects();
	MAIN_D_80135264 = -1;
	VS_playVersusIntroSequence();
	MAIN_D_80135264 = 0;
	while (MAIN_D_80135264 < VS_D_800716B3[0]) {
		PLAYTIME_FRAMES = 0;
		VS_loadFighterEntities(MAIN_D_80135264);
		ENTITY_TABLE[1]->isOnScreen = 1;
		ENTITY_TABLE[2]->isOnScreen = 1;
		loadVSSounds();
		((DigimonEntity *)ENTITY_TABLE[1])->stats.current.vabId = 4;
		((DigimonEntity *)ENTITY_TABLE[2])->stats.current.vabId = 5;
		loadDigimonSounds(4, ENTITY_TABLE[1]->type);
		loadDigimonSounds(5, ENTITY_TABLE[2]->type);
		VS_setVSPhase(0xa);
		VS_addArenaRenderers();
		VS__func_800F23D0(VS_D_800716B2[0]);
		MAIN_D_80135268 = 1;
		result = VS__combatMain();
		VS_removeCameraIntro();
		VS_removeVSPhase();
		VS_removeArenaRenderers();
		if (result == 1) {
			VS_D_80071690[0][MAIN_D_80135264] = 1;
			VS_D_800716A1[MAIN_D_80135264] = 1;
			MAIN_D_80135258[0]++;
			MAIN_D_8013525B++;
		}
		if (result == -1) {
			VS_D_80071695[MAIN_D_80135264] = 1;
			VS_D_8007169C[0][MAIN_D_80135264] = 1;
			MAIN_D_80135259++;
			MAIN_D_8013525A[0]++;
		}
		if (result == 2) {
			VS_D_8007169C[0][MAIN_D_80135264] = 1;
			VS_D_800716A1[MAIN_D_80135264] = 1;
			MAIN_D_8013525A[0]++;
			MAIN_D_8013525B++;
		}
		ENTITY_TABLE[1]->isOnScreen = 0;
		ENTITY_TABLE[2]->isOnScreen = 0;
		VS_playVersusIntroSequence();
		MAIN_D_80135264++;
	}

	VS_unloadFighterEntities();
}

void VS_resetMatchState(void)
{
	int32_t i;
	int32_t j;

	MAIN_D_80135259 = 0;
	MAIN_D_8013525B = 0;
	MAIN_D_8013525C = 0;
	MAIN_D_8013525E = 0;
	MAIN_D_80135260 = 0;
	MAIN_D_80135261 = 0;
	MAIN_D_80135258[0] = 0;
	MAIN_D_8013525A[0] = 0;
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 5; j++) {
			VS_D_80071690[i][j] = 0;
			VS_D_8007169C[i][j] = 0;
		}
	}
}

void VS_loadVSAssets(int32_t arena)
{
	switch (arena) {
	case 0:
		MAIN_D_80135274 = 0x1e;
		break;
	case 1:
		MAIN_D_80135274 = 0x20;
		break;
	case 2:
		MAIN_D_80135274 = 0x1f;
		break;
	}

	DRAWING_OFFSET_X = 0xa0;
	DRAWING_OFFSET_Y = 0x78;
	VS_D_800716D4[0].length = 2;
	VS_D_800716D4[0].org = VS_D_800716B4;
	VS_D_800716D4[1].length = 2;
	VS_D_800716D4[1].org = VS_D_800716C4;
	VS_D_800716FC[0].length = 2;
	VS_D_800716FC[0].org = VS_D_80071724;
	VS_D_800716FC[1].length = 2;
	VS_D_800716FC[1].org = VS_D_80071734;
	VS_initializeCamera();
	VS_initializeLighting(arena);
	VS_loadArenaAssets();
	VS_loadStageModels();
	VS_initializeFinisherAuraModel((char *)0x80052ae0, (char *)0x80053800);
	VS_initializePoisonBubble();
	VS_initializeConfusionEffect((char *)0x80054838);
	VS_initializeStunEffect((char *)0x80054d00);
	initializeBuffModel((TMDModel *)0x80055328);
	VS_loadTIMToVRAM(VS_D_8006FB94);
	loadTIMFile(VS_D_8006FBA8, (void *)0x80010000);
}

void VS_addInputObjects(void)
{
	addObject(0x1b2, 0, (TickFunction)VS__tickInput, NULL);
	addObject(0x1b2, 1, (TickFunction)VS__tickInput, NULL);
	PLAYTIME_FRAMES = 0;
	addObject(0xfb9, 0, (TickFunction)VS_tickPlaytime, NULL);
}

void VS_loadFighterEntities(uint8_t slot)
{
	VsFighterCard *card1;
	VsFighterCard *card2;
	Stats *stats;
	int32_t prev;

	card1 = (VsFighterCard *)(MAIN_D_8013526C + VS_D_800716A8[slot] * 64);
	card2 = (VsFighterCard *)(MAIN_D_80135270 + VS_D_800716AD[slot] * 64);

	{
		int32_t type;

		type = card1->type;
		if (ENTITY_TABLE[1] == NULL || ENTITY_TABLE[1]->type != type) {
			if (ENTITY_TABLE[1] != NULL) {
				prev = ENTITY_TABLE[1]->type;
				removeEntity(prev, 1);
				thunkUnloadModel(prev, 3);
			}
			thunkLoadMMD(type, 3);
			ENTITY_TABLE[1] = &PARTNER_ENTITY.digimonEntity.entity;
			initializeDigimonObject(type, 1, VS__tickDigimonP1);
		}
	}

	{
		int32_t type;

		type = card2->type;
		if (ENTITY_TABLE[2] == NULL || ENTITY_TABLE[2]->type != type) {
			if (ENTITY_TABLE[2] != NULL) {
				prev = ENTITY_TABLE[2]->type;
				removeEntity(prev, 2);
				thunkUnloadModel(prev, 0);
			}
			thunkLoadMMD(type, 0);
			ENTITY_TABLE[2] = &NPC_ENTITIES[0].digimonEntity.entity;
			initializeDigimonObject(type, 2, VS__tickDigimonP2);
		}
	}

	stats = &PARTNER_ENTITY.digimonEntity.stats;
	stats->current.chargeMode = 0;
	stats->base.off = card1->off;
	stats->base.def = card1->def;
	stats->base.speed = card1->speed;
	stats->base.brain = card1->brain;
	stats->base.hp = card1->hp;
	stats->base.mp = card1->mp;
	stats->current.currentHP = card1->hp;
	stats->current.currentMP = card1->mp;
	MAIN_D_80135278 = card1->weight;
	stats->base.moves[0] = card1->moves[0];
	stats->base.moves[1] = card1->moves[1];
	stats->base.moves[2] = card1->moves[2];
	stats->base.moves[3] = VS_getFirstSpecialMove(card1->type);

	stats = &NPC_ENTITIES[0].digimonEntity.stats;
	stats->current.chargeMode = 0;
	stats->base.off = card2->off;
	stats->base.def = card2->def;
	stats->base.speed = card2->speed;
	stats->base.brain = card2->brain;
	stats->base.hp = card2->hp;
	stats->base.mp = card2->mp;
	stats->current.currentHP = card2->hp;
	stats->current.currentMP = card2->mp;
	MAIN_D_8013527A = card2->weight;
	stats->base.moves[0] = card2->moves[0];
	stats->base.moves[1] = card2->moves[1];
	stats->base.moves[2] = card2->moves[2];
	stats->base.moves[3] = VS_getFirstSpecialMove(card2->type);

	ENTITY_TABLE[1]->isOnMap = 1;
	ENTITY_TABLE[1]->isOnScreen = 1;
	ENTITY_TABLE[2]->isOnMap = 1;
	ENTITY_TABLE[2]->isOnScreen = 1;
}

void VS_unloadFighterEntities(void)
{
	int32_t type;

	PLAYTIME_FRAMES = 0;
	removeObject(0xFB9, 0);
	removeObject(0x1B2, 0);
	removeObject(0x1B2, 1);

	type = ENTITY_TABLE[1]->type;
	removeEntity(type, 1);
	thunkUnloadModel(type, 3);

	type = ENTITY_TABLE[2]->type;
	removeEntity(type, 2);
	thunkUnloadModel(type, 0);

	VS_unloadArenaAssets();

	ENTITY_TABLE[2] = NULL;
	ENTITY_TABLE[1] = NULL;
}

void VS_renderVersusFlash(void)
{
	POLY_FT4 *prim;
	int32_t y;

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	prim->r0 = 0x80;
	prim->g0 = 0x80;
	prim->b0 = 0x80;
	prim->tpage = 5;
	prim->clut = GetClut(0x40, 0x1E9);
	setUVDataPolyFT4(prim, 0x30, 0x10, 0x4E, 0x18);

	y = MAIN_D_80135261 * 20 - 0x39;
	setPosDataPolyFT4(prim, -0x27, y, 0x4E, 0x18);
	AddPrim(ACTIVE_ORDERING_TABLE->org + 0x1E, prim++);
	GsSetWorkBase((PACKET *)prim);
}

void VS_renderFighterNamePlate(int16_t side)
{
	int32_t cb;
	int32_t cd0;
	int32_t cd1;
	int32_t L;
	int32_t y;
	int32_t y2;
	int32_t x0;
	int32_t x1;
	int32_t sx;
	POLY_FT4 *prim;
	int32_t i;
	int32_t type;
	int32_t u;
	int32_t v;
	int16_t w;
	int16_t h;
	char *name;
	int32_t n;
	int32_t k;
	int16_t g;
	int32_t m;
	int32_t gy;
	int32_t gx;
	int32_t cx;
	uint16_t c;
	uint16_t *np;

	if (side == 0) {
		cb = MAIN_D_8013525E;
	} else {
		cb = -MAIN_D_8013525E << 16;
		cb = cb >> 16;
	}
	prim = (POLY_FT4 *)GsGetWorkBase();
	i = 0;
	y = -0x39;
	y2 = 0;
	sx = side * 0x9a;
	x1 = sx - 0x77;
	x0 = cb + x1;

	for (; i < VS_D_800716B3[0]; i++) {
		if (side == 0) {
			cd0 = (int32_t)(MAIN_D_8013526C + VS_D_800716A8[i] * 64);
			type = ((VsFighterCard *)cd0)->type;
		} else {
			cd1 = (int32_t)(MAIN_D_80135270 + VS_D_800716AD[i] * 64);
			type = ((VsFighterCard *)cd1)->type;
		}
		SetPolyFT4(prim);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->tpage = 0xe;
		prim->clut = GetClut(0x120, VS_D_8006FA28[type] + 0x1e0);
		if (type == 0x73) {
			u = 0xe0;
			v = u;
		} else {
			u = (((type - 3) % 32) << 5) & 0xff;
			v = (((type - 3) / 8) << 5) & 0xff;
		}
		w = (u != 0xf0) ? 0x10 : 0xf;
		h = (v != 0xf0) ? 0x10 : 0xf;
		setUVDataPolyFT4(prim, u, v, w, h);
		if (i == MAIN_D_80135261) {
			setPosDataPolyFT4(prim, x0, y, w, h);
		} else {
			setPosDataPolyFT4(prim, x1, y, w, h);
		}
		AddPrim(ACTIVE_ORDERING_TABLE->org + 0x1e, prim++);

		if (side == 0) {
			name = (char *)cd0 + 0xe;
		} else {
			name = (char *)cd1 + 0xe;
		}
		L = strlen(name) / 2;
		n = 0;
		k = 0;
		cx = cx = cb;
		gy = y2 - 0x35;
		y2 = y2;
		for (; n < L; n++, k += 8) {
			gx = sx - 0x5f;
			SetPolyFT4(prim);
			setRGB0(prim, 0x80, 0x80, 0x80);
			prim->tpage = 7;
			prim->clut = GetClut(0x30, 0x1e8);
			np = (uint16_t *)name;
			c = *np++;
			name = (char *)np;
			c = ((c & 0xff) << 8) + ((c & 0xff00) >> 8);
			for (m = 0; m < 0xd8; m++) {
				if (c == VS_D_8006FBC0[m]) {
					g = m;
					break;
				}
			}
			setUVDataPolyFT4(prim, (VS_D_8006FD70[g] % 15) * 8,
			                 (VS_D_8006FD70[g] / 15) * 8, 8, 8);
			if (i == MAIN_D_80135261) {
				setPosDataPolyFT4(prim, cx + (gx + k), gy, 8, 8);
			} else {
				setPosDataPolyFT4(prim, gx + k, gy, 8, 8);
			}
		}
		y2 += 0x14;
		y += 0x14;
	}
	GsSetWorkBase((PACKET *)prim);
}

void VS_renderRoundPips(int32_t slot)
{
	POLY_FT4 *prim;
	int32_t y;
	int32_t i;

	prim = (POLY_FT4 *)GsGetWorkBase();
	for (i = 0, y = -0x39; i < VS_D_800716B3[0]; i++, y += 0x14) {
		if (i < MAIN_D_80135261) {
			SetPolyFT4(prim);
			prim->tpage = 5;
			setRGB0(prim, 0x80, 0x80, 0x80);
			prim->clut = GetClut(0x40, 0x1e9);
			if (i < MAIN_D_80135261) {
				if (VS_D_80071690[slot][i] == 1) {
					setUVDataPolyFT4(prim, 0x30, 0x28, 0x10, 0x10);
				} else {
					setUVDataPolyFT4(prim, 0x40, 0x28, 0x10, 0x10);
				}
				setPosDataPolyFT4(prim, slot * 0x9a - 0x20, y, 0x10, 0x10);
			}
			AddPrim(ACTIVE_ORDERING_TABLE->org + 0x1e, prim++);
		}
	}
	GsSetWorkBase((PACKET *)prim);
}

void VS_renderRoundScores(int16_t slot)
{
	renderNumber(0, slot * 0x9a - 100, 0x30, 1, MAIN_D_80135258[slot], 0x1e);
	renderNumber(0, slot * 0x9a - 0x1c, 0x30, 1, MAIN_D_8013525A[slot], 0x1e);
}

void VS_renderVersusBanner(int32_t slot)
{
	POLY_FT4 *prim;
	int32_t y;
	int32_t i;
	int32_t base;
	int32_t cluty;
	int32_t k;
	int32_t x;

	if (slot == 0) {
		base = MAIN_D_8013525E;
	} else {
		base = (int16_t)-MAIN_D_8013525E;
	}
	prim = (POLY_FT4 *)GsGetWorkBase();
	x = slot;
	x = x * 0x9a;
	cluty = slot + 0x1e8;
	k = k = -0x90;
	i = 0;
	y = -0x3b;
	base += slot = k + x;
	for (; i < VS_D_800716B3[0]; i++, y += 0x14) {
		SetPolyFT4(prim);
		prim->tpage = 6;
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->clut = GetClut(0, cluty);
		if (i < MAIN_D_80135261) {
			setUVDataPolyFT4(prim, 0, 0xec, 0x86, 0x13);
			setPosDataPolyFT4(prim, slot, y, 0x85, 0x14);
		} else {
			setUVDataPolyFT4(prim, 0, 0xd8, 0x86, 0x14);
			if (i == MAIN_D_80135261) {
				setPosDataPolyFT4(prim, base, y, 0x85, 0x14);
			} else {
				setPosDataPolyFT4(prim, slot, y, 0x85, 0x14);
			}
		}
		AddPrim(ACTIVE_ORDERING_TABLE->org + 0x1e, prim++);
	}
	GsSetWorkBase((PACKET *)prim);
}

void VS_renderVersusText(void)
{
	VsTextPiece *p;
	POLY_FT4 *prim;
	POLY_F4 *prim2;
	int32_t x0;
	int32_t y0;
	int32_t x1;
	int32_t x2;
	int32_t y1;
	int32_t y2;
	int32_t xa;
	int32_t xb;
	int32_t yb;
	int32_t ya;
	int32_t i;

	p = VS_D_8006FA6C;
	prim = (POLY_FT4 *)GsGetWorkBase();
	i = 0;
	y0 = y0 = -0x56;
	x0 = x0 = -0x98;
	for (; i < 0x15; i++, p++) {
		SetPolyFT4(prim);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->tpage = p->tpage;
		if (i >= 0 && i < 4) {
			initStringFT4(prim);
		} else {
			prim->clut = GetClut((i == 10) ? 0x40 : 0, p->clut + 0x1e8);
		}
		setUVDataPolyFT4(prim, p->u, p->v, p->uw, p->uh);
		setPosDataPolyFT4(prim, x0 + p->dx, y0 + p->dy, p->w, p->h);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 0x1e, prim++);
	}
	GsSetWorkBase((PACKET *)prim);
	prim2 = (POLY_F4 *)GsGetWorkBase();
	xa = xa = -0x98;
	xb = xb = xa;
	x1 = x1 = xa + 4;
	x2 = x2 = xb + 0x98;
	yb = yb = ya = ya = -0x56;
	i = 0;
	y1 = y1 = yb + 3;
	y2 = y2 = ya + 0x9a;
	for (; i < 2; i++) {
		SetPolyF4(prim2);
		prim2->r0 = (i != 0) ? 0x80 : 0x5b;
		prim2->g0 = (i != 0) ? 0x5b : 0x70;
		prim2->b0 = (i != 0) ? 0x5b : 0x80;
		prim2->x0 = (i == 0) ? x1 : x2;
		prim2->y0 = y1;
		prim2->x1 = ((i == 0) ? x1 : x2) + 0x94;
		prim2->y1 = y1;
		prim2->x2 = (i == 0) ? x1 : x2;
		prim2->y2 = y2;
		prim2->x3 = ((i == 0) ? x1 : x2) + 0x94;
		prim2->y3 = y2;
		AddPrim(ACTIVE_ORDERING_TABLE->org + 0x1e, prim2++);
	}
	GsSetWorkBase((PACKET *)prim2);
}

int32_t VS_initializeLighting__garbage__(int32_t i)
{
	int32_t v0;
	int32_t v1;
	int32_t v2;
	int32_t v3;
	int32_t v4;
	int32_t v5;
	int32_t v6;
	int32_t v7;
	int32_t v8;
	int32_t v9;

	v0 = i + 0;
	v1 = i + 1;
	v2 = i + 2;
	v3 = i + 3;
	v4 = i + 4;
	v5 = i + 5;
	v6 = i + 6;
	v7 = i + 7;
	v8 = i + 8;
	v9 = i + 9;
	return v0 + v1 + v2 + v3 + v4 + v5 + v6 + v7 + v8 + v9;
}

void VS_initializeLighting(int32_t mode)
{
	int32_t amb;

	LIGHT_DATA[0].vx = 1000;
	LIGHT_DATA[0].vy = 1000;
	LIGHT_DATA[0].vz = 1000;
	LIGHT_DATA[0].r = 255;
	LIGHT_DATA[0].g = 255;
	LIGHT_DATA[0].b = 255;
	GsSetFlatLight(0, &LIGHT_DATA[0]);
	LIGHT_DATA[1].vx = -1000;
	LIGHT_DATA[1].vy = 1000;
	LIGHT_DATA[1].vz = -1000;
	LIGHT_DATA[1].r = 160;
	LIGHT_DATA[1].g = 160;
	LIGHT_DATA[1].b = 160;
	GsSetFlatLight(1, &LIGHT_DATA[1]);
	amb = 0x400;
	LIGHT_DATA[2].vx = -20;
	LIGHT_DATA[2].vy = 20;
	LIGHT_DATA[2].vz = 100;
	LIGHT_DATA[2].r = 96;
	LIGHT_DATA[2].g = 96;
	LIGHT_DATA[2].b = 96;
	GsSetAmbient(amb, amb, amb);
	GsSetLightMode(0);
}

int32_t VS_getFirstSpecialMove(int32_t index)
{
	int32_t i;
	uint8_t move;

	for (i = 15; i >= 0; i--) {
		if (DIGIMON_DATA[index].moves[i] != 0xff) {
			move = DIGIMON_DATA[index].moves[i];
			if (move >= 0x3a) {
				return i + 0x2e;
			}
		}
	}
}

void VS_playVersusIntroSequence(void)
{
	int32_t i;

	clearTextArea();
	drawString(MAIN_D_80134A50, 0, 0);
	drawString(MAIN_D_80134A54, 0, 0xc);
	drawString(MAIN_D_80124C54, 0, 0xf0);
	fadeFromBlack(3);
	i = 3;
	if (MAIN_D_80135264 == VS_D_800716B3[0] - 1) {
		if (MAIN_D_80135258[0] > MAIN_D_80135259) {
			VS_addResultModelScene(ENTITY_TABLE[1]);
			VS_setVersusModelSceneTimer(0x3c);
		}
		if (MAIN_D_80135259 > MAIN_D_80135258[0]) {
			VS_addResultModelScene(ENTITY_TABLE[2]);
			VS_setVersusModelSceneTimer(0x3c);
		}
		if (MAIN_D_80135258[0] == MAIN_D_80135259) {
			VS_loadVersusSceneModel();
			VS_addVersusModelScene();
			VS_setVersusModelSceneTimer(-10);
		}
	}
	while (i < 4) {
		VS_tickFrame();
		i++;
	}
	addObject(0x19d, 0, (TickFunction)VS_tickVersusIntro, (RenderFunction)VS_renderVersusIntro);
	MAIN_D_80135262 = 0;
	MAIN_D_8013525C = 0;
	while (MAIN_D_80135262 == 0) {
		VS_tickFrame();
	}
	MAIN_D_80135262 = 0;
	removeObject(0x19d, 0);
	if (MAIN_D_80135264 == VS_D_800716B3[0] - 1) {
		if (MAIN_D_80135258[0] != MAIN_D_80135259) {
			VS_removeResultModelScene();
		}
		if (MAIN_D_80135258[0] == MAIN_D_80135259) {
			VS_removeVersusModelScene();
		}
	}
	if (MAIN_D_80135264 != VS_D_800716B3[0] - 1) {
		fadeToBlack(3);
		for (i = 0; i < 4; i++) {
			VS_tickFrame();
		}
	}
}

void VS_tickPlaytime(void)
{
	PLAYTIME_FRAMES++;
}

void VS_tickVersusIntro(void)
{
	uint32_t state;

	state = MAIN_D_80135260;
	switch (state) {
	case 0:
		MAIN_D_8013525E++;
		if (MAIN_D_8013525E == 10) {
			MAIN_D_80135260 = state + 1;
		}
		break;
	case 1:
		MAIN_D_8013525C++;
		if (MAIN_D_8013525C == 20) {
			MAIN_D_80135260 = state + 1;
			MAIN_D_80135262 = 1;
		}
		break;
	case 2:
		MAIN_D_8013525E--;
		if (MAIN_D_8013525E == 0) {
			MAIN_D_80135260 = state + 1;
			MAIN_D_8013525C = 0;
		}
		break;
	case 3:
		MAIN_D_8013525C++;
		if (MAIN_D_8013525C == 10) {
			MAIN_D_80135260 = state + 1;
		}
		break;
	case 4:
		MAIN_D_80135261++;
		if (MAIN_D_80135261 == VS_D_800716B3[0]) {
			MAIN_D_80135260 = state + 1;
		} else {
			MAIN_D_80135260 = 0;
			MAIN_D_8013525C = 0;
		}
		break;
	case 5:
		MAIN_D_8013525C++;
		if (MAIN_D_8013525C == 0x50) {
			MAIN_D_80135262 = 1;
		}
		break;
	}
}

void VS_renderVersusIntro(void)
{
	if (MAIN_D_80135260 == 1) {
		VS_renderVersusFlash();
	}
	VS_renderFighterNamePlate(0);
	VS_renderFighterNamePlate(1);
	VS_renderRoundPips(0);
	VS_renderRoundPips(1);
	VS_renderRoundScores(0);
	VS_renderRoundScores(1);
	VS_renderVersusBanner(0);
	VS_renderVersusBanner(1);
	VS_renderVersusText();
}

void VS_loadArenaAssets(void)
{
	int32_t i;

	VS_loadArenaTIMToVRAM(VS_D_800705FC[VS_D_800716B2[0]], MAIN_D_80134A60[VS_D_800716B2[0]]);
	MAIN_D_801352A0 = TAMER_MODEL_BUFFER;
	readFile(VS_D_800705B4[VS_D_800716B2[0]], MAIN_D_801352A0);
	GsMapModelingData((u_long *)MAIN_D_801352A0 + 1);
	for (i = 0; i < MAIN_D_80134A5C[VS_D_800716B2[0]]; i++) {
		GsLinkObject4((u_long)((char *)MAIN_D_801352A0 + 0xc), &VS_D_800719CC[i], i);
		GsInitCoordinate2(NULL, &VS_D_8007188C[i]);
		VS_D_800719CC[i].attribute = 0;
		VS_D_800719CC[i].coord2 = &VS_D_8007188C[i];
	}
	readFile(VS_D_80070630[VS_D_800716B2[0]], MAP_COLLISION_DATA);
}

void VS_unloadArenaAssets(void)
{
}

void VS_addArenaRenderers(void)
{
	switch (VS_D_800716B2[0]) {
	case 0:
		addObject(0x1A7, 0, NULL, (RenderFunction)VS_renderArenaViewLeft);
		break;
	case 1:
		addObject(0x1A7, 0, NULL, (RenderFunction)VS_renderArenaViewRight);
		break;
	case 2:
		addObject(0x1A7, 0, NULL, (RenderFunction)VS_renderArenaViewFull);
		break;
	}
}

void VS_renderArenaViewLeft(void)
{
	MATRIX m;
	int32_t i;

	GsGetLw(&VS_D_8007188C[1], &m);
	GsSetLightMatrix(&m);
	GsGetLs(&VS_D_8007188C[1], &m);
	GsSetLsMatrix(&m);
	GsSortObject4(&VS_D_800719CC[1], ACTIVE_ORDERING_TABLE, 2, getScratchAddr(0));
	VS_D_800719CC[0].attribute |= 0x200;
	GsClearOt(0, 0xfff, &VS_D_800716D4[ACTIVE_FRAMEBUFFER]);
	GsClearOt(0, 0xffe, &VS_D_800716FC[ACTIVE_FRAMEBUFFER]);
	for (i = 2; i >= 0; i--) {
		GsGetLw(&VS_D_8007188C[i], &m);
		GsSetLightMatrix(&m);
		GsGetLs(&VS_D_8007188C[i], &m);
		GsSetLsMatrix(&m);
		switch (i) {
		case 0:
			GsSortObject4(&VS_D_800719CC[i], &VS_D_800716FC[ACTIVE_FRAMEBUFFER], 0xc, getScratchAddr(0));
			break;
		case 2:
			GsSortObject4(&VS_D_800719CC[i], &VS_D_800716D4[ACTIVE_FRAMEBUFFER], 0xc, getScratchAddr(0));
			break;
		}
	}
	GsSortOt(&VS_D_800716D4[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
	GsSortOt(&VS_D_800716FC[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
}

void VS_renderArenaViewRight(void)
{
	MATRIX m;
	int32_t i;

	GsGetLw(&VS_D_8007188C[3], &m);
	GsSetLightMatrix(&m);
	GsGetLs(&VS_D_8007188C[3], &m);
	GsSetLsMatrix(&m);
	GsSortObject4(&VS_D_800719CC[3], ACTIVE_ORDERING_TABLE, 2, getScratchAddr(0));
	VS_D_800719CC[0].attribute |= 0x200;
	GsClearOt(0, 0xfff, &VS_D_800716D4[ACTIVE_FRAMEBUFFER]);
	GsClearOt(0, 0xffe, &VS_D_800716FC[ACTIVE_FRAMEBUFFER]);
	for (i = 2; i >= 0; i--) {
		GsGetLw(&VS_D_8007188C[i], &m);
		GsSetLightMatrix(&m);
		GsGetLs(&VS_D_8007188C[i], &m);
		GsSetLsMatrix(&m);
		switch (i) {
		case 0:
			GsSortObject4(&VS_D_800719CC[i], &VS_D_800716FC[ACTIVE_FRAMEBUFFER], 0xc, getScratchAddr(0));
			break;
		case 1:
		case 2:
			GsSortObject4(&VS_D_800719CC[i], &VS_D_800716D4[ACTIVE_FRAMEBUFFER], 0xc, getScratchAddr(0));
			break;
		}
	}
	GsSortOt(&VS_D_800716D4[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
	GsSortOt(&VS_D_800716FC[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
}

void VS_renderArenaViewFull(void)
{
	MATRIX m;
	int32_t i;

	VS_D_800719CC[0].attribute |= 0x200;
	GsClearOt(0, 0xfff, &VS_D_800716D4[ACTIVE_FRAMEBUFFER]);
	GsClearOt(0, 0xffe, &VS_D_800716FC[ACTIVE_FRAMEBUFFER]);
	for (i = 1; i >= 0; i--) {
		GsGetLw(&VS_D_8007188C[i], &m);
		GsSetLightMatrix(&m);
		GsGetLs(&VS_D_8007188C[i], &m);
		GsSetLsMatrix(&m);
		switch (i) {
		case 0:
			GsSortObject4(&VS_D_800719CC[i], &VS_D_800716FC[ACTIVE_FRAMEBUFFER], 0xc, getScratchAddr(0));
			break;
		case 1:
			GsSortObject4(&VS_D_800719CC[i], &VS_D_800716D4[ACTIVE_FRAMEBUFFER], 0xc, getScratchAddr(0));
			break;
		}
	}
	GsSortOt(&VS_D_800716D4[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
	GsSortOt(&VS_D_800716FC[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
}

void VS_removeArenaRenderers(void)
{
	removeObject(0x1a7, 0);
}

void VS_applyCamera(void)
{
	MAIN_D_801B1B98.super = NULL;
	RotMatrix(MAIN_D_801B1C0C, &MAIN_D_801B1B98.view);
	TransMatrix(&MAIN_D_801B1B98.view, &MAIN_D_801B1C14);
	MAIN_D_801B1BBC[0] = 0;
	GsSetView2(&MAIN_D_801B1B98);
}

void VS_setCameraOrbit(void)
{
	VECTOR *a;
	VECTOR *b;
	int32_t dx;
	int32_t dz;
	int32_t dist;
	int32_t d;
	int32_t ang;
	MATRIX *m;
	int32_t limA;
	int32_t limB;
	int32_t limC;

	a = &ENTITY_TABLE[2]->posData->location;
	b = &ENTITY_TABLE[1]->posData->location;
	dx = a->vx - b->vx;
	dz = a->vz - b->vz;
	dist = SquareRoot0(dx * dx + dz * dz);
	d = (dist * VIEWPORT_DISTANCE) / 200u;
	limA = MAIN_D_80135294;
	limB = limB = limA;
	limC = limC = limA;
	if (d < limA) {
		d = limB;
	}
	if (d < limC + 0x12c) {
		MAIN_D_80135284 = 1;
	} else {
		MAIN_D_80135284 = 0;
	}
	if (d > 0x1068) {
		d = 0x1068;
	}
	ang = 0x1000 - _atan(dx, dz);
	MAIN_D_801B1C14.vx = b->vx + dx / 2 + (d * dz) / dist;
	MAIN_D_801B1C14.vy = -0x3e8;
	MAIN_D_801B1C14.vz = b->vz + dz / 2 - (d * dx) / dist;
	MAIN_D_801B1C0C[0].vx = _atan(d, -0x2bc) + 0x800;
	MAIN_D_801B1C0C[0].vy = ang + 0x800;
	MAIN_D_801B1C0C[0].vz = 0;
	MAIN_D_801B1B98.view = GsIDMATRIX;
	MAIN_D_801B1B98.super = (GsCOORDINATE2 *)MAIN_D_801B1BBC;
	RotMatrixYXZ(MAIN_D_801B1C0C, m = m = &MAIN_D_801B1BC0);
	TransMatrix(m, &MAIN_D_801B1C14);
	MAIN_D_801B1BBC[0] = 0;
	GsSetView2(&MAIN_D_801B1B98);
}

void VS_setCameraYXZ(void)
{
	VECTOR *a;
	VECTOR *b;
	MATRIX *m;

	a = &ENTITY_TABLE[1]->posData->location;
	b = &ENTITY_TABLE[2]->posData->location;
	MAIN_D_801B1B98.view = GsIDMATRIX;
	MAIN_D_801B1B98.super = (GsCOORDINATE2 *)MAIN_D_801B1BBC;
	MAIN_D_801B1C14.vx = a->vx + (b->vx - a->vx) / 2;
	MAIN_D_801B1C14.vz = a->vz + (b->vz - a->vz) / 2;
	MAIN_D_801B1C14.vy = -0x1f40;
	MAIN_D_801B1C0C[0].vz = 0;
	MAIN_D_801B1C0C[0].vy = 0;
	MAIN_D_801B1C0C[0].vx = -0x400;
	RotMatrixYXZ(MAIN_D_801B1C0C, m = m = &MAIN_D_801B1BC0);
	TransMatrix(m, &MAIN_D_801B1C14);
	MAIN_D_801B1BBC[0] = 0;
	GsSetView2(&MAIN_D_801B1B98);
}

void VS_setViewpointRotationFromEntity(void)
{
	MATRIX *m;

	m = (MATRIX *)(MAIN_D_80135298[1] + 0xbc);
	GS_VIEWPOINT.vrx = m->t[0];
	GS_VIEWPOINT.vry = m->t[1];
	GS_VIEWPOINT.vrz = m->t[2];
	GsSetRefView2(&GS_VIEWPOINT);
}

void VS_setCameraLookAtEntity(void)
{
	VECTOR v;
	VECTOR out;
	Entity *self;
	Entity *other;
	VECTOR *selfPos;
	VECTOR *otherPos;
	int32_t dx;
	int32_t dz;

	self = (Entity *)MAIN_D_80135298;
	selfPos = &self->posData->location;
	if (self == ENTITY_TABLE[1]) {
		other = ENTITY_TABLE[2];
	} else {
		other = ENTITY_TABLE[1];
	}
	otherPos = &other->posData->location;
	dx = otherPos->vx - selfPos->vx;
	do {
	} while (0);
	dz = otherPos->vz - selfPos->vz;
	MAIN_D_801B1C0C[0].vy = (-_atan(dz, dx) + 0x800) & 0xfff;
	RotMatrix(MAIN_D_801B1C0C, &MAIN_D_801B1B98.view);
	v = MAIN_D_801B1C14;
	ApplyMatrixLV(&MAIN_D_801B1B98.view,
	              &((Entity *)MAIN_D_80135298)->posData->location, &out);
	v.vx -= out.vx;
	v.vy -= out.vy;
	v.vz -= out.vz;
	TransMatrix(&MAIN_D_801B1B98.view, &v);
	GsSetView2(&MAIN_D_801B1B98);
}

void VS_applyViewpoint(void)
{
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

void VS_setCameraSimple(void)
{
	MAIN_D_801B1C0C[0].vy += 2;
	MAIN_D_801B1C0C[0].vy &= 0xFFF;
	MAIN_D_801B1B98.super = NULL;
	RotMatrix(MAIN_D_801B1C0C, &MAIN_D_801B1B98.view);
	TransMatrix(&MAIN_D_801B1B98.view, &MAIN_D_801B1C14);
	MAIN_D_801B1BBC[0] = 0;
	GsSetView2(&MAIN_D_801B1B98);
}

void VS_setCameraParams(int16_t a, int16_t b, int16_t c, int16_t d, int16_t e, int16_t f)
{
	MAIN_D_801B1C0C[0].vx = a;
	MAIN_D_801B1C0C[0].vy = b;
	MAIN_D_801B1C0C[0].vz = c;
	MAIN_D_801B1C14.vx = d;
	MAIN_D_801B1C14.vy = e;
	MAIN_D_801B1C14.vz = f;
}

void VS_setVSPhase(int32_t arg)
{
	addObject(0x1a8, 0, (TickFunction)VS_tickVSPhase, NULL);
	MAIN_D_80135268 = arg;
	MAIN_D_8013529C = 0;
}

void VS_tickVSPhase(void)
{
	switch (MAIN_D_80135268) {
	case 0:
		VS_applyCamera();
		break;
	case 1:
		VS_setCameraOrbit();
		break;
	case 2:
		VS_setCameraYXZ();
		break;
	case 3:
	case 5:
		VS_setCameraToEntity();
		break;
	case 4:
	case 6:
		VS_setViewpointRotationFromEntity();
		break;
	case 9:
		VS_applyEntityViewpoint();
		break;
	case 7:
		VS_setCameraLookAtEntity();
		break;
	case 8:
		VS_applyViewpoint();
		break;
	case 10:
		VS_setCameraSimple();
		break;
	}
}

void VS_loadArenaTIMToVRAM(char *path, int32_t count)
{
	GsIMAGE img;
	int32_t *p;
	int32_t i;

	p = (int32_t *)GENERAL_BUFFER;
	readFile(path, GENERAL_BUFFER);
	for (i = 0; i < count; i++) {
		p = (int32_t *)((char *)p + 4);
		GsGetTimInfo((u_long *)p, &img);
		p = &p[((img.pw * img.ph) / 2) + 4];
		LoadImage((RECT *)&img.px, img.pixel);
		if ((img.pmode >> 3) & 1) {
			LoadImage((RECT *)&img.cx, img.clut);
			p = &p[((img.cw * img.ch) / 2) + 3];
		}
	}
}

void VS_loadTIMToVRAM(char *path)
{
	GsIMAGE img;
	int32_t *p;
	int32_t i;

	p = (int32_t *)GENERAL_BUFFER;
	readFile(path, GENERAL_BUFFER);
	for (i = 0; i < 6; i++) {
		p = (int32_t *)((char *)p + 4);
		GsGetTimInfo((u_long *)p, &img);
		p = &p[((img.pw * img.ph) / 2) + 4];
		LoadImage((RECT *)&img.px, img.pixel);
		if ((img.pmode >> 3) & 1) {
			LoadImage((RECT *)&img.cx, img.clut);
			p = &p[((img.cw * img.ch) / 2) + 3];
		}
	}
}

void VS_removeVSPhase(void)
{
	removeObject(0x1a8, 0);
}

void VS_selectRandomCamera(DigimonEntity *entity, int32_t mode, int32_t sub)
{
	StdCameraPreset *p;

	if (mode != 5) {
		if (random(3) != 0) {
			return;
		}
	}
	MAIN_D_801B1B98.super = NULL;
	MAIN_D_80135298 = (char **)entity;
	if (sub != 3) {
		p = &VS_D_8007063C[mode];
	} else {
		p = &VS_D_8007063C[random(3) + 6];
	}
	VS_setCameraParams(p->unk0, p->unk2, p->unk4, p->unk6, p->unk8, p->unkA);
	if (mode < 5) {
		MAIN_D_80135268 = 3;
		return;
	}
	VS_addResultModelScene((Entity *)entity == ENTITY_TABLE[2] ? ENTITY_TABLE[1] : ENTITY_TABLE[2]);
	VS_startCameraIntro(NULL, (Entity *)entity);
}

void VS_setRandomViewpoint(Entity *entity, int32_t idx)
{
	VECTOR v;
	VECTOR out;
	MATRIX m;

	if (random(3) != 0) {
		return;
	}
	if (MAIN_D_80135268 == 7) {
		return;
	}

	VIEWPORT_DISTANCE = 0x1f4;
	GS_VIEWPOINT.super = NULL;

	if (idx < 4) {
		MAIN_D_80135298 = (char **)entity;
		MAIN_D_80135268 = 4;
		RotMatrix(&entity->posData->rotation, &m);
		v.vx = VS_D_800706A8[idx * 3];
		v.vy = VS_D_800706AA[idx * 3];
		v.vz = VS_D_800706AC[idx * 3];
		ApplyMatrixLV(&m, &v, &out);
		out.vx += ((Entity *)MAIN_D_80135298)->posData->location.vx;
		out.vz += ((Entity *)MAIN_D_80135298)->posData->location.vz;
		GS_VIEWPOINT.vpx = out.vx;
		GS_VIEWPOINT.vpy = out.vy;
		GS_VIEWPOINT.vpz = out.vz;
	} else {
		MAIN_D_80135268 = 6;
		GS_VIEWPOINT.vpx = VS_D_800706A8[idx * 3];
		GS_VIEWPOINT.vpy = VS_D_800706AA[idx * 3];
		GS_VIEWPOINT.vpz = VS_D_800706AC[idx * 3];
	}

	GS_VIEWPOINT.rz = 0;
}

void VS_initializeCamera(void)
{
	VIEWPORT_DISTANCE = 500;
	GsSetProjection(500);
	MAIN_D_801B1C0C[0].vx = 100;
	MAIN_D_801B1C0C[0].vy = 0;
	MAIN_D_801B1C0C[0].vz = 0;
	MAIN_D_801B1C14.vx = 0;
	MAIN_D_801B1C14.vy = 500;
	MAIN_D_801B1C14.vz = 3000;
	MAIN_D_801B1B98.super = NULL;
	RotMatrix(MAIN_D_801B1C0C, &MAIN_D_801B1B98.view);
	TransMatrix(&MAIN_D_801B1B98.view, &MAIN_D_801B1C14);
	MAIN_D_801B1BBC[0] = 0;
	GsSetView2(&MAIN_D_801B1B98);
}

void VS_setViewpointFromBone(Entity *entity, SVECTOR *offset, SVECTOR *rot, int32_t dist)
{
	MATRIX m1;
	SVECTOR out1;
	MATRIX m2;
	SVECTOR bone;
	MATRIX m3;
	VECTOR v;

	calculateBoneMatrix(entity, offset->pad, &m1);
	ApplyMatrixSV(&m1, offset, &out1);
	GS_VIEWPOINT.vrx = m1.t[0] + out1.vx;
	GS_VIEWPOINT.vry = m1.t[1] + out1.vy;
	GS_VIEWPOINT.vrz = m1.t[2] + out1.vz;
	bone = MAIN_D_80134AB4;
	calculateBoneMatrix(entity, 1, &m2);
	ApplyMatrixSV(&m2, &bone, &bone);
	GS_VIEWPOINT.vry = m2.t[1] + bone.vy;
	RotMatrixZYX(rot, &m3);
	v.vx = 0;
	v.vy = 0;
	v.vz = dist;
	ApplyMatrixLV(&m3, &v, (VECTOR *)&GS_VIEWPOINT);
	GS_VIEWPOINT.vpx += GS_VIEWPOINT.vrx;
	GS_VIEWPOINT.vpy += GS_VIEWPOINT.vry;
	GS_VIEWPOINT.vpz += GS_VIEWPOINT.vrz;
	VIEWPORT_DISTANCE = 0x15e;
}

void VS_setCameraToEntity(void)
{
	SVECTOR rot;
	VECTOR v;
	VECTOR out;

	rot = MAIN_D_801B1C0C[0];
	rot.vy -= ((Entity *)MAIN_D_80135298)->posData->rotation.vy;
	rot.vy &= 0xfff;
	RotMatrix(&rot, &MAIN_D_801B1B98.view);
	v = MAIN_D_801B1C14;
	ApplyMatrixLV(&MAIN_D_801B1B98.view,
	              &((Entity *)MAIN_D_80135298)->posData->location, &out);
	v.vx -= out.vx;
	v.vy -= out.vy;
	v.vz -= out.vz;
	TransMatrix(&MAIN_D_801B1B98.view, &v);
	MAIN_D_801B1BBC[0] = 0;
	GsSetView2(&MAIN_D_801B1B98);
}

int32_t VS_getFighterDistance(VECTOR *self, VECTOR *other, VECTOR *target)
{
	int32_t toTarget;
	int32_t toOther;

	toTarget = getDistance(self->vx - target->vx, self->vy - target->vy, self->vz - target->vz);
	toOther = getDistance(other->vx - self->vx, other->vy - self->vy, other->vz - self->vz);
	return (toTarget * 100) / toOther;
}

void VS_updateCameraLerp(int32_t t, int32_t flip)
{
	SVECTOR off;
	SVECTOR rot;
	int32_t base;
	int32_t dbl;
	int32_t dist;

	off = MAIN_D_80134AA4;
	rot = MAIN_D_80134AAC;
	base = ((((DIGIMON_DATA[((Entity *)MAIN_D_80135298)->type].height +
	           DIGIMON_DATA[((Entity *)MAIN_D_80135298)->type].radius) /
	          2) *
	         0x62c) /
	        450);
	dbl = base * 2;
	rot.vx = lerp(0x2aa, 0xe3, 0, 0x64, t);
	rot.vy = lerp(0x5c7, 0xa38, 0, 0x64, t);

	if (rot.vy < 0x801) {
		dist = lerp(base, dbl, 0x5c7, 0x800, rot.vy);
	} else {
		dist = lerp(dbl, base, 0x800, 0xa38, rot.vy);
	}

	if (flip != 0) {
		rot.vy = -rot.vy;
	}

	rot.vy += ((Entity *)MAIN_D_80135298)->posData->rotation.vy;
	off.vy = (-DIGIMON_DATA[((Entity *)MAIN_D_80135298)->type].height * 2) / 3;
	VS_setViewpointFromBone((Entity *)MAIN_D_80135298, &off, &rot, dist);
}

int32_t VS_isPositionNearEntity(Entity *entity, VECTOR *pos)
{
	if (pos->vx - 50 > entity->posData->location.vx) {
		goto no;
	}
	if (pos->vx + 50 < entity->posData->location.vx) {
		goto no;
	}
	if (pos->vz - 50 > entity->posData->location.vz) {
		goto no;
	}
	if (entity->posData->location.vz > pos->vz + 50) {
		goto no;
	}

	return 1;
no:
	return 0;
}

int32_t VS_interpolateClamped2(int32_t lo, int32_t hi, int32_t t, int32_t start, int32_t end)
{
	int32_t tmp;

	if (hi < lo) {
		tmp = lo;
		lo = hi;
		hi = tmp;
	}

	t = t < lo ? lo : (hi < t ? hi : t);
	if (lo >= t) {
		return start;
	}

	return start + ((end - start) * (t - lo) / (hi - lo));
}

void VS_tickCameraChase(void)
{
	SVECTOR off;
	SVECTOR rot;
	CameraChase *cc;
	int32_t dist;
	int32_t d2;
	int32_t slot;
	int32_t i;

	cc = (CameraChase *)&MAIN_D_801352A4;
	if (MAIN_D_801352A4 < 0x14) {
		return;
	}
	if (cc->timer < 0x14) {
		goto inc;
	}
	if (cc->timer == 0x14) {
		startAnimation((Entity *)MAIN_D_80135298, 0x23);
	}
	if (cc->phase == 0) {
		dist = VS_getFighterDistance(&VS_D_80071754, &VS_D_80071744, &((Entity *)MAIN_D_80135298)->posData->location);
		if (dist >= 0x23) {
			VS_D_80071754 = ((Entity *)MAIN_D_80135298)->posData->location;
			cc->phase = 1;
			MAIN_D_80135268 = 8;
		} else {
			off = MAIN_D_80134A64;
			rot = MAIN_D_80134A6C;
			if (cc->side == 0) {
				rot.vy = lerp(-0x638, -0x293, 0, 0x23, dist);
			} else {
				rot.vy = lerp(0x638, 0x293, 0, 0x23, dist);
			}
			rot.vy += ((Entity *)MAIN_D_80135298)->posData->rotation.vy;
			off.vy = (-DIGIMON_DATA[((Entity *)MAIN_D_80135298)->type].height * 7) / 10;
			d2 = (((DIGIMON_DATA[((Entity *)MAIN_D_80135298)->type].height + DIGIMON_DATA[((Entity *)MAIN_D_80135298)->type].radius) / 2) * 0x5aa) / 450;
			VS_setViewpointFromBone((Entity *)MAIN_D_80135298, &off, &rot, d2);
			GS_VIEWPOINT.vpy = (off.vy * 7) / 10;
			if (GS_VIEWPOINT.vpy > -0xb4) {
				GS_VIEWPOINT.vpy = -0xb4;
			}
			goto inc;
		}
	}
	dist = VS_getFighterDistance(&VS_D_80071754, &VS_D_80071744, &((Entity *)MAIN_D_80135298)->posData->location);
	VS_updateCameraLerp(dist, cc->side);
	if (VS_isPositionNearEntity((Entity *)MAIN_D_80135298, &VS_D_80071744) == 1) {
		for (i = 0; i < 3; i++) {
			if (((uint8_t *)MAIN_D_80135298 + i)[0x44] != 0xff) {
				startAnimation((Entity *)MAIN_D_80135298, ((uint8_t *)MAIN_D_80135298 + i)[0x44]);
				break;
			}
		}
		((Entity *)MAIN_D_80135298)->anim.animFlag |= 2;
		cc->timer = -1;
		return;
	}
inc:
	cc->timer++;
}

void VS_startCameraChase(Entity *entity, int32_t dx, int32_t side)
{
	SVECTOR off;
	SVECTOR rot;
	int32_t dist;

	MAIN_D_80135298 = (char **)entity;
	VS_D_80071754.vx = entity->posData->location.vx;
	VS_D_80071754.vy = entity->posData->location.vy;
	VS_D_80071754.vz = entity->posData->location.vz;
	VS_D_80071744.vx = VS_D_80071754.vx - dx;
	VS_D_80071744.vy = VS_D_80071754.vy;
	VS_D_80071744.vz = VS_D_80071754.vz;
	startAnimation(entity, 0x21);
	MAIN_D_80135268 = 9;
	((CameraChase *)&MAIN_D_801352A4)->timer = 0;
	((CameraChase *)&MAIN_D_801352A4)->phase = 0;
	((CameraChase *)&MAIN_D_801352A4)->side = side;
	addObject(0x1aa, 0, (TickFunction)VS_tickCameraChase, NULL);
	off = MAIN_D_80134A74;
	rot = MAIN_D_80134A7C;
	if (MAIN_D_801352A7 == 0) {
		rot.vy = -0x638;
	} else {
		rot.vy = 0x638;
	}
	rot.vy += ((Entity *)MAIN_D_80135298)->posData->rotation.vy;
	off.vy = (-DIGIMON_DATA[((Entity *)MAIN_D_80135298)->type].height * 2) / 3;
	dist = (((DIGIMON_DATA[((Entity *)MAIN_D_80135298)->type].height + DIGIMON_DATA[((Entity *)MAIN_D_80135298)->type].radius) / 2) * 0x5aa) / 450;
	VS_setViewpointFromBone((Entity *)MAIN_D_80135298, &off, &rot, dist);
	GS_VIEWPOINT.vpy = (off.vy * 3) / 10;
	if (GS_VIEWPOINT.vpy > -0xb4) {
		GS_VIEWPOINT.vpy = -0xb4;
	}
}

void VS_tickCameraIntro(void)
{
	SVECTOR off;
	SVECTOR rot;
	int16_t *p;

	p = VS_D_80071A0C;
	if ((p[0] >= 0x1e) && (p[0] < 0x3c)) {
		VS_D_80071A10[0] = lerp(VS_D_80071A10[0], VS_D_80071A12[0], p[0], 0x3c, p[0] + 1);
	}

	if (p[0] >= 0x1e) {
		p[1] += (int16_t)VS_interpolateClamped2(0x1e, 0x3c, p[0], 0, 0x5b);
	}

	off = MAIN_D_80134A84;
	rot = MAIN_D_80134A8C;
	rot.vy = VS_D_80071A0E[0];
	VS_D_80071A10[0] = MAIN_func_800DA63C(0x16, p[0], VS_D_800706C8, VS_D_80071A30, VS_D_80071A88);
	VS_setViewpointFromBone(*(Entity **)&p[6], &off, &rot, VS_D_80071A10[0]);
	GS_VIEWPOINT.vry = (-DIGIMON_DATA[VS_D_80071A18[0]->type].height * 2) / 3;
	GS_VIEWPOINT.vpy = -MAIN_func_800DA63C(0x16, p[0], VS_D_800706C8, VS_D_80071AE0, VS_D_80071A88);
	p[0]++;
}

void VS_startCameraIntro(Entity *target, Entity *entity)
{
	SVECTOR off;
	SVECTOR rot;
	int16_t dx;
	int16_t dz;
	int32_t d;
	int32_t i;

	VS_D_80071A0C[0] = 0;
	VS_D_80071A1C[0] = entity;
	if (target == NULL) {
		if ((Entity *)MAIN_D_80135298 == ENTITY_TABLE[1]) {
			target = ENTITY_TABLE[2];
		} else {
			target = ENTITY_TABLE[1];
		}
	}
	VS_D_80071A18[0] = target;
	MAIN_D_80135268 = 8;
	addObject(0x1ad, 0, (TickFunction)VS_tickCameraIntro, NULL);

	off = MAIN_D_80134A94;
	rot = MAIN_D_80134A9C;
	dx = entity->posData->location.vx - target->posData->location.vx;
	dz = entity->posData->location.vz - target->posData->location.vz;
	VS_D_80071A0E[0] = (-_atan(dz, dx) + 0x7de) & 0xfff;
	rot.vy = VS_D_80071A0E[0];
	VS_D_80071A10[0] = getDistance(dx, 0, dz);
	VS_D_80071A10[0] = VS_D_80071A10[0] + 0x2bc;
	if (VS_D_80071A10[0] < 0x5dc) {
		VS_D_80071A10[0] = 0x5dc;
	}
	VS_setViewpointFromBone(target, &off, &rot, VS_D_80071A10[0]);

	VS_D_80071A12[0] = DIGIMON_DATA[target->type].radius * 3 * 2;
	GS_VIEWPOINT.vry = (-DIGIMON_DATA[target->type].height * 2) / 3;
	GS_VIEWPOINT.vpy = -DIGIMON_DATA[entity->type].height;
	VS_D_80071A14[0] = GS_VIEWPOINT.vpy;
	VS_D_80071A16[0] = GS_VIEWPOINT.vpy * 250 / 100;
	((int32_t *)VS_D_80071AE0)[0] = DIGIMON_DATA[entity->type].height;
	VS_D_80071AE4[0] = DIGIMON_DATA[entity->type].height;
	VS_D_80071AE8[0] = DIGIMON_DATA[entity->type].height * 200 / 100;
	VS_D_80071AEC[0] = DIGIMON_DATA[target->type].height * 380 / 100;
	VS_D_80071AF0[0] = DIGIMON_DATA[target->type].height * 250 / 100;
	for (i = 5; i < 0x16; i++) {
		((int32_t *)VS_D_80071AE0)[i] =
			customRandom(0x50, DIGIMON_DATA[target->type].height * 180 / 100);
	}
	MAIN_func_800DA60C(0x16, VS_D_800706C8, VS_D_80071AE0, VS_D_80071A88);

	d = getDistance(dx, 0, dz) + 0x2bc;
	((int32_t *)VS_D_80071A30)[0] = d;
	VS_D_80071A34[0] = d;
	if (d < 0x5dc) {
		d = 0x5dc;
	}
	VS_D_80071A38[0] = d * 120 / 100;
	VS_D_80071A3C[0] = d * 80 / 100;
	VS_D_80071A40[0] = VS_D_80071A12[0];
	for (i = 5; i < 0x16; i++) {
		((int32_t *)VS_D_80071A30)[i] =
			customRandom(VS_D_80071A12[0] * 45 * 2 / 100,
		                     VS_D_80071A12[0] * 45 * 4 / 100);
	}
}

void VS_removeCameraIntro(void)
{
	removeObject(0x1ad, 0);
	VS_D_80071A0C[0] = -1;
}

void VS_applyEntityViewpoint(void)
{
	char *p;

	VIEWPORT_DISTANCE = 0x15E;
	GsSetProjection(0x15E);
	p = MAIN_D_80135298[1] + 0x34;
	GS_VIEWPOINT.vrx = *(int32_t *)(p + 0x14);
	GS_VIEWPOINT.vry = -DIGIMON_DATA[(int32_t)MAIN_D_80135298[0]].height * 2 / 3;
	GS_VIEWPOINT.vrz = *(int32_t *)(p + 0x1C);
	GsSetRefView2(&GS_VIEWPOINT);
}

void VS_renderCounterDigits(int32_t x, int32_t y, int32_t digits, int32_t value, int32_t layer)
{
	POLY_FT4 *prim;
	int32_t i;
	uint32_t width;
	int32_t count;
	int32_t buf[6];

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	prim->r0 = 0x80;
	prim->g0 = 0x80;
	prim->b0 = 0x80;
	prim->tpage = 0xD;
	prim->clut = GetClut(0x10, 0x1E0);
	setUVDataPolyFT4(prim, 0x78, 0x30, 8, 0x12);
	setPosDataPolyFT4(prim, -0x1C, -0x63, 8, 0x12);
	AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	SetPolyFT4(prim);
	prim->r0 = 0x80;
	prim->g0 = 0x80;
	prim->b0 = 0x80;
	prim->tpage = 0xD;
	prim->clut = GetClut(0x10, 0x1E0);
	setUVDataPolyFT4(prim, 0x80, 0x30, 8, 0x12);
	setPosDataPolyFT4(prim, 0x14, -0x63, 8, 0x12);
	AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	width = digits;
	convertValueToDigits(digits, value, &count, buf);

	for (i = count - 1; i >= 0; i--) {
		SetPolyFT4(prim);
		prim->r0 = 0x80;
		prim->g0 = 0x80;
		prim->b0 = 0x80;
		prim->tpage = 0xD;
		prim->clut = GetClut(0x10, 0x1E0);
		setUVDataPolyFT4(prim, buf[i] * 12, 0x30, 0xC, 0xF);
		setPosDataPolyFT4(prim, x + ((((int32_t)width - 1) - i) * 14), y, 0xC, 0xF);
		AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	}

	SetPolyFT4(prim);
	prim->r0 = 0x80;
	prim->g0 = 0x80;
	prim->b0 = 0x80;
	prim->tpage = GetTPage(1, 0, 0x2C0, 0);
	prim->clut = GetClut(0x200, 0xFF);
	setUVDataPolyFT4(prim, 0, 0x78, 0x42, 0x1D);
	setPosDataPolyFT4(prim, -0x21, -0x68, 0x42, 0x1D);
	AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	GsSetWorkBase((PACKET *)prim);
}

void VS_addFighterCounter(uint8_t arg)
{
	if ((MAIN_D_801352A8 == 0) && (arg != 0)) {
		MAIN_D_80135288 = arg;
		addObject(0x1ac, 0, (TickFunction)VS_tickFighterCounter, (RenderFunction)VS_renderFighterCounter);
		MAIN_D_801352A8 = 1;
	}
}

void VS_tickFighterCounter(void)
{
	if (MAIN_D_8013528C == 1) {
		MAIN_D_80134D66++;
		if (ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[0]]->anim.animId != 0x2B) {
			if (ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]]->anim.animId != 0x2B) {
				if (MAIN_D_80134D66 % 0x14 == 0) {
					if (MAIN_D_80135288 != 0) {
						MAIN_D_80135288--;
					}
				}
			}
		}
	}
}

void VS_renderFighterCounter(void)
{
	VS_renderCounterDigits(-0xd, -0x61, 2, MAIN_D_80135288, 3);
}

void VS_removeFighterCounter(void)
{
	if (MAIN_D_801352A8 != 0) {
		removeObject(0x1ac, 0);
		MAIN_D_801352A8 = 0;
	}
}
