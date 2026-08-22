#include <stdlib.h>

#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/entity.h>
#include <dw/file.h>
#include <dw/graphics.h>
#include <dw/kar.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/types.h>
#include <dw/ui.h>
#include <dw/world_object.h>

#include "common.h"

typedef struct {
	int32_t state;
	int8_t type;
	int8_t unk5;
	int16_t weight;
	int16_t speed;
	int16_t unkA;
	int32_t angle;
	int8_t ring;
	int8_t prevRing;
	int8_t pad12[2];
	VECTOR target;
	GsDOBJ2 obj;
	GsCOORDINATE2 coord;
	int8_t pad84[16];
	int16_t unk94;
	int16_t selectPhase;
	int16_t unk98;
	int16_t pad9A;
	VECTOR pos;
} KarStone;

typedef struct {
	int8_t score;
	int8_t thrown;
	int16_t unk2;
	uint16_t unk4;
	int8_t pad6[2];
	KarStone stones[5];
} KarStoneRow;

typedef struct {
	struct {
		int16_t dx;
		int16_t dz;
		int16_t radius;
	} ring[4];
} KarZones;

typedef struct {
	char *text[2];
} KarStrPair;

typedef struct {
	char *text[61];
} KarStrTbl;

typedef struct {
	int8_t start[10];
} KarOffTbl;

typedef struct {
	int16_t weight[4];
} KarWeightTbl;

typedef struct {
	int32_t id[3];
} KarModelIds;

typedef struct {
	int8_t id[3];
} KarPeggedModelIds;

typedef struct {
	int32_t x[3];
} KarSpawnX;

typedef struct {
	int32_t type[2][4];
} KarShotPlan;

typedef struct {
	int32_t x;
	int32_t y;
	int32_t z;
} KarPos;

typedef struct {
	int16_t x;
	int16_t y;
	int16_t clutX;
	int16_t clutY;
	int16_t u;
	int16_t v;
	int16_t w;
	int16_t h;
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t pad;
} KarSprite;

typedef struct {
	KarSprite sprite[5];
} KarSpriteSet;

typedef struct {
	KarSprite sprite[2];
} KarSpritePair;

typedef struct {
	struct {
		int16_t u;
		int16_t v;
	} glyph[11];
} KarDigits;

extern GsOT_TAG KAR_D_800637CC[];
extern GsOT_TAG KAR_D_8006384C[];
extern GsOT KAR_D_800638CC[];
extern KarStoneRow KAR_D_8005B5A0[];
extern KarZones KAR_D_8005B438;
extern int32_t ACTIVE_FRAMEBUFFER;
extern int32_t VIEWPORT_DISTANCE;
extern GsOT GS_ORDERING_TABLE[];
extern GsRVIEW2 GS_VIEWPOINT;
extern GsRVIEW2 KAR_D_8005B580;
extern uint16_t MAIN_D_80135252;
extern KarModelIds KAR_D_8005AB80;
extern KarOffTbl KAR_D_8005AB8C;
extern KarSpawnX KAR_D_8005AB98;
extern KarWeightTbl MAIN_D_80134A08;
extern int32_t KAR_D_80063914[];
extern RECT MAIN_D_80134A10;
extern RECT MAIN_D_80134A18;
extern KarStrPair MAIN_D_80134A20;
extern RECT MAIN_D_80134A28;
extern RECT MAIN_D_80134A30;
extern uint8_t MAIN_D_8013522C;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern KarStrTbl KAR_D_8005AF58;
extern KarOffTbl KAR_D_8005B04C;
extern KarStrTbl KAR_D_8005B318;
extern KarOffTbl KAR_D_8005B40C;
extern KarShotPlan KAR_D_8005B418;
extern KarSprite KAR_D_8005B48C;
extern KarDigits KAR_D_8005B4A0;
extern KarSpritePair KAR_D_8005B4CC;
extern int16_t MAIN_D_8013523E;
extern KarSpriteSet KAR_D_8005B4F4;
extern KarSprite KAR_D_8005B558;
extern KarSprite KAR_D_8005B478;
extern KarSprite KAR_D_8005B56C;
extern uint8_t MAIN_D_80135220;
extern char KAR_D_8005ABA4[];
extern u_long KAR_D_8005BFCC[];
extern u_long *MAIN_D_80135240;
extern int32_t MAIN_D_80135244;
extern uint8_t MAIN_D_80135248;
extern KarStone *KAR_D_800639C0[];
extern int16_t MAIN_D_8013523A;
extern int8_t MAIN_D_80135256;
extern KarPeggedModelIds MAIN_D_80134A4C;
extern int8_t MAIN_D_80134A4F[1];
extern GsOT *ACTIVE_ORDERING_TABLE;
extern uint32_t POLLED_INPUT;
extern int8_t MAIN_D_80135250;
extern int8_t MAIN_D_8013523C;
extern int32_t MAIN_D_8013524C;

void renderSelectionCursor(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e);
void clearTextArea(void);
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
void renderUIBox(int32_t id);
void setTextColor(int32_t color);
void drawString(char *text, int32_t color, int32_t pos);
void startAnimation(Entity *entity, int32_t animId);

int32_t KAR_tickMatchState(void);
int32_t KAR_renderAimArrow(void);
void KAR_updateRingMarkers(void);
void KAR_checkStonesStopped(void);
int32_t KAR_distance(int32_t x, int32_t y);
void KAR_debugCollisionStone(KarStone *stone, VECTOR *b, VECTOR *a);
void KAR_debugCollisionPair(KarStone *stoneA, VECTOR *a, KarStone *stoneB, VECTOR *b);
void KAR_reflectOffDiagonal(int32_t *p, int32_t dir);
void KAR_turnAimLeft(int32_t delta);
void KAR_turnAimRight(int32_t delta);
int32_t KAR_findUnusedStoneOfType(int32_t key);
void KAR_initializeOrderingTables(void);
int32_t KAR_tickYesNoPrompt(void);
void KAR_resolveStoneCollision(KarStone *stoneA, VECTOR a, KarStone *stoneB, VECTOR b);
void KAR_collideRestingStone(KarStone *resting, VECTOR a, KarStone *mover, VECTOR b);
void KAR_collideMovingStones(KarStone *stoneA, VECTOR a, KarStone *stoneB, VECTOR b);
int32_t KAR_tickHintBox(int32_t n);
int32_t KAR_computeSeparation(VECTOR *out, KarStone *stone, VECTOR a, VECTOR b);
void KAR_collidePeggedStone(KarStone *pegged, VECTOR a, KarStone *mover, VECTOR b);
void KAR_beginAiming(void);
void KAR_renderSprite(KarSprite *sp);
int32_t KAR_findWallContact(VECTOR *out, KarStone *stone, int32_t flag);
int32_t KAR_aimAtRandomStone(void);
int32_t KAR_computeImpactShare(KarStone *stone, VECTOR a, VECTOR b);
int32_t KAR_getWallZone(int16_t x, int16_t z);
void KAR_selectPreviousStone(void);
void KAR_selectNextStone(void);
void KAR_tickStones(int32_t instanceId);
void KAR_renderScene(int32_t instanceId);
void KAR_setupMatch(int32_t arg);
void KAR_renderStoneCursor(void);
void KAR_renderReadyPrompt(void);
void KAR_registerThrownStone(void);
void KAR_chooseOpponentShot(void);
int32_t KAR_tickScoreTally(void);
void KAR_bounceOffWall(void);
void KAR_updateCollisions(void);
void KAR_classifyStoneRings(void);
void KAR_renderScores(void);
void KAR_finishMatch(void);
int32_t KAR_drawHintPagePenguinmon(int32_t idx, int8_t n);
int32_t KAR_drawHintPageMetalMamemon(int32_t idx, int8_t n);
void KAR_renderPowerMeter(void);
int32_t KAR_computeThrowPower(int32_t a, int32_t b, int32_t c);
void KAR_setOpponentShot(int32_t row, int32_t val, int32_t b, int32_t c);
void KAR_rotatePoint(SVECTOR *p, int32_t ang);
int32_t KAR_aimBankShot(KarStone *stone, int32_t x, int32_t z);
void KAR_placeAtContact(KarStone *stone, KarPos p);
void KAR_handleAimScroll(void);
int32_t KAR_findClearShotAngle(int16_t x, int16_t z);
int32_t KAR_aimAtStoneInRing(int32_t player, int32_t key, int16_t *outX, int16_t *outZ);
void KAR_renderNamePlates(void);
void KAR_beginThrow(void);

static void *kar_functions[] = {
	KAR_renderSprite,
	KAR_rotatePoint,
	KAR_registerThrownStone,
	KAR_computeThrowPower,
	KAR_findUnusedStoneOfType,
	KAR_aimBankShot,
	KAR_aimAtRandomStone,
	KAR_setOpponentShot,
	KAR_aimAtStoneInRing,
	KAR_findClearShotAngle,
	KAR_drawHintPageMetalMamemon,
	KAR_drawHintPagePenguinmon,
	KAR_chooseOpponentShot,
	KAR_tickScoreTally,
	KAR_classifyStoneRings,
	KAR_tickHintBox,
	KAR_tickYesNoPrompt,
	KAR_beginThrow,
	KAR_turnAimRight,
	KAR_turnAimLeft,
	KAR_selectNextStone,
	KAR_selectPreviousStone,
	KAR_beginAiming,
	KAR_placeAtContact,
	KAR_reflectOffDiagonal,
	KAR_findWallContact,
	KAR_getWallZone,
	KAR_debugCollisionPair,
	KAR_computeSeparation,
	KAR_computeImpactShare,
	KAR_debugCollisionStone,
	KAR_collideMovingStones,
	KAR_collideRestingStone,
	KAR_collidePeggedStone,
	KAR_resolveStoneCollision,
	KAR_distance,
	KAR_bounceOffWall,
	KAR_updateCollisions,
	KAR_checkStonesStopped,
	KAR_updateRingMarkers,
	KAR_handleAimScroll,
	KAR_renderNamePlates,
	KAR_renderReadyPrompt,
	KAR_renderPowerMeter,
	KAR_renderScores,
	KAR_renderStoneCursor,
	KAR_renderAimArrow,
	KAR_tickMatchState,
	KAR_tick,
	KAR_finishMatch,
	KAR_renderScene,
	KAR_tickStones,
	KAR_start,
	KAR_setupMatch,
	KAR_initializeOrderingTables,
};

static void kar__garbage__(void)
{
	int32_t t0 = KAR_D_800638CC[0].length;
	int32_t t1 = KAR_D_800638CC[0].offset;
	int32_t t2 = KAR_D_800638CC[0].point;

	KAR_D_800638CC[0].length = t0 + t1 + t2;
}

void KAR_initializeOrderingTables(void)
{
	GsOT_TAG *tags0 = KAR_D_800637CC;
	GsOT_TAG *tags1 = KAR_D_8006384C;

	KAR_D_800638CC[0].length = 5;
	KAR_D_800638CC[0].org = tags0;
	KAR_D_800638CC[1].length = 5;
	KAR_D_800638CC[1].org = tags1;
}

void KAR_setupMatch(int32_t mode)
{
	int32_t obstacles;
	KarModelIds models;
	KarWeightTbl weights;
	KarOffTbl types;
	KarSpawnX spawnX;
	int32_t p;
	int32_t i;
	int32_t n;
	KarStone *stone;

	obstacles = rand() % 3;
	MAIN_D_8013523A = 0;
	MAIN_D_8013523C = 0;
	MAIN_D_8013523E = 0;

	for (p = 0; p < 3; p++) {
		stone = KAR_D_8005B5A0[p].stones;
		rand();
		KAR_D_8005B5A0[p].score = 0;
		KAR_D_8005B5A0[p].thrown = 0;
		KAR_D_8005B5A0[p].unk2 = 0;
		KAR_D_8005B5A0[p].unk4 = 0;

		for (i = 0; i < 5; ) {
			models = KAR_D_8005AB80;
			weights = MAIN_D_80134A08;
			types = KAR_D_8005AB8C;

			n = rand();
			n = n % 10;
			stone->speed = 0;
			stone->angle = 0;
			stone->ring = 0;
			stone->prevRing = 0;
			if (mode == 0) {
				stone->type = types.start[n];
			} else {
				stone->type = i;
				if (stone->type >= 4) {
					stone->type = 3;
				}
			}
			GsLinkObject4((u_long)(MAIN_D_80135240 + 3), &stone->obj, stone->type * 3 + models.id[p]);
			GsInitCoordinate2(NULL, &stone->coord);
			stone->obj.attribute = 0;
			stone->obj.coord2 = &stone->coord;
			if (p < 2) {
				stone->pos.vy = i * 0xaa - 0x44c;
				stone->pos.vx = (p != 0) ? 0x226 : -0x384 - i * 0x1e;
				stone->pos.vz = 0;
				stone->pos.vy = i * 0xaa - 0x50;
				stone->pos.vx = (p != 0) ? 0x2bc : -0x2bc;
				stone->pos.vz = 0;
				stone->unk94 = 0x96;
				stone->selectPhase = 0;
				stone->unk98 = 0;
				stone->state = -1;
			} else if (i < obstacles) {
				spawnX = KAR_D_8005AB98;
				stone->pos.vy = 0;
				stone->pos.vx = spawnX.x[i] - rand() % 0x13f;
				stone->pos.vz = -(rand() % 0x3e8 + 0x3e8);
				stone->unk94 = 0;
				stone->selectPhase = 0;
				stone->unk98 = 0;
				stone->state = 1;
				stone->speed = 0;
			} else {
				stone->pos.vy = 0;
				stone->pos.vx = 0;
				stone->pos.vz = 0;
				stone->unk94 = 0;
				stone->selectPhase = 0;
				stone->unk98 = 0;
				stone->state = -0x65;
				stone->speed = 0;
			}
			stone->weight = weights.weight[stone->type];
			i++;
			stone++;
		}
	}
}

void KAR_start(void)
{
	MAIN_D_80135240 = KAR_D_8005BFCC;
	SOME_SCRIPT_SYNC_BIT = 0;
	readFile(KAR_D_8005ABA4, MAIN_D_80135240);
	GsMapModelingData(MAIN_D_80135240 + 1);
	KAR_initializeOrderingTables();
	ENTITY_TABLE[0]->isOnMap = 1;
	MAIN_D_80135244 = 0;
	KAR_setupMatch(0);
	addObject(0x1388, 0xf, KAR_tickStones, KAR_renderScene);
	MAIN_D_80135248 = readPStat(0x7a);
	MAIN_D_80135248 = (uint32_t)MAIN_D_80135248 + 2;
}

void KAR_tickStones(int32_t instanceId)
{
	KarPeggedModelIds models;
	KarStone *stone;
	int32_t p;
	int32_t i;
	int16_t move;
	int16_t step;

	for (p = 0; p < 3; p++) {
		stone = KAR_D_8005B5A0[p].stones;
		for (i = 0; i < 5; i++, stone++) {
			if (stone->state != 3 && stone->state != 2) {
				stone->target = stone->pos;
			}
			if (MAIN_D_80135244 == 5) {
				if (i == MAIN_D_8013523A && p == MAIN_D_8013523C) {
					stone->selectPhase = stone->selectPhase + 0x14;
				} else {
					stone->selectPhase = 0;
				}
			}
			if (stone->state > 0 && stone->speed >= 0) {
				move = stone->speed / stone->weight;
				stone->speed = (uint32_t)(stone->speed * 98) / 100;
				if (move < 2) {
					stone->speed = 0;
				}
				if (stone->state < 3) {
					step = move;
					stone->pos.vx = stone->pos.vx + move * rcos(stone->angle) / 4096;
					stone->pos.vz = stone->pos.vz + step * rsin(stone->angle) / 4096;
				}
				if (stone->state == 3) {
					if (stone->speed > 0) {
						step = move / 10;
						if ((stone->angle = stone->angle - 0x800) < 0) {
							stone->angle = stone->angle + 0x1000;
						}
						stone->pos.vx = stone->target.vx + step * rcos(stone->angle) / 4096;
						stone->pos.vz = stone->target.vz + step * rsin(stone->angle) / 4096;
						if (step == 0) {
							stone->speed = 0;
						}
					} else {
						stone->speed = 0;
						stone->state = 2;
					}
				}
				if (stone->state == 1 && move <= 0 && stone->type == 3) {
					models = MAIN_D_80134A4C;
					stone->state = 2;
					GsLinkObject4((u_long)(MAIN_D_80135240 + 3), &stone->obj, models.id[p]);
					playSound2(8, 4);
				}
			}
		}
	}

	KAR_updateCollisions();
}

void KAR_renderScene(int32_t instanceId)
{
	MATRIX m;
	KarStone *stone;
	int32_t p;
	int32_t i;
	int32_t state;
	int8_t idx;

	GsClearOt(0, 0xa, &KAR_D_800638CC[ACTIVE_FRAMEBUFFER]);

	for (p = 0; p < 3; p++) {
		stone = KAR_D_8005B5A0[p].stones;

		for (i = 0; i < 5; ) {
			if ((p == 1) && (MAIN_D_80135244 != 3) && (MAIN_D_80135244 < 0x10)) {
				if (MAIN_D_80135250 != 0) {
					goto next;
				}
				if (MAIN_D_80135244 < 5) {
					goto next;
				}
			}
			if (MAIN_D_80135244 < 4) {
				if (MAIN_D_80135252 < i * 0x3c + 0x3c) {
					goto next;
				}
			}

			if (stone->state == 0) {
				idx = (MAIN_D_8013523C != 0) ? MAIN_D_80135248 : 0;
				if (idx == 2) {
					stone->pos = ENTITY_TABLE[idx]->posData->location;
					stone->pos.vx = stone->pos.vx - 0x50;
				} else if (idx == 0) {
					stone->pos.vx = ENTITY_TABLE[idx]->posData[9].posMatrix.workm.t[0];
					stone->pos.vy = ENTITY_TABLE[idx]->posData[9].posMatrix.workm.t[1] + 0x78;
					stone->pos.vz = ENTITY_TABLE[idx]->posData[9].posMatrix.workm.t[2];
					stone->unk98 = 0;
					stone->selectPhase = 0;
					stone->unk94 = 0;
				} else {
					stone->pos.vx = ENTITY_TABLE[idx]->posData[4].posMatrix.workm.t[0];
					stone->pos.vy = ENTITY_TABLE[idx]->posData[4].posMatrix.workm.t[1] + 0x78;
					stone->pos.vz = ENTITY_TABLE[idx]->posData[4].posMatrix.workm.t[2];
					stone->unk98 = 0;
					stone->selectPhase = 0;
					stone->unk94 = 0;
				}
			}

			RotMatrix((SVECTOR *)&stone->unk94, &stone->coord.coord);
			TransMatrix(&stone->coord.coord, &stone->pos);
			stone->coord.flg = 0;

			if (stone->state < 0) {
				GsSetProjection(0x200);
				GsSetRefView2(&KAR_D_8005B580);
			} else {
				GsSetProjection(VIEWPORT_DISTANCE);
				GsSetRefView2(&GS_VIEWPOINT);
			}

			GsGetLw(stone->obj.coord2, &m);
			GsSetLightMatrix(&m);
			GsGetLs(stone->obj.coord2, &m);
			GsSetLsMatrix(&m);

			state = stone->state;
			if (state < 0) {
				if (stone->state < -0x64) {
					goto next;
				}
				GsSortObject4(&stone->obj, &KAR_D_800638CC[ACTIVE_FRAMEBUFFER], 5, (u_long *)0x1f800000);
			} else {
				if (stone->state >= 0x65) {
					goto next;
				}
				GsSortObject4(&stone->obj, &GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER], 2, (u_long *)0x1f800000);
			}
		next:
			i++;
			stone++;
		}
	}

	GsSortOt(&KAR_D_800638CC[ACTIVE_FRAMEBUFFER], &GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER]);
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

void KAR_finishMatch(void)
{
	int8_t v;

	if (MAIN_D_80135244 == 0x14) {
		return;
	}

	MAIN_D_80135244 = 0x14;
	ENTITY_TABLE[MAIN_D_80135248]->posData->rotation.vy = 0x800;
	ENTITY_TABLE[1]->posData->rotation.vy = 0x800;
	ENTITY_TABLE[0]->posData->rotation.vy = 0x800;
	startAnimation(ENTITY_TABLE[0], 2);
	startAnimation(ENTITY_TABLE[1], 2);
	startAnimation(ENTITY_TABLE[MAIN_D_80135248], 2);
	SOME_SCRIPT_SYNC_BIT = 1;
	removeObject(0x1388, 0xf);
	setTextColor(1);

	v = KAR_D_8005B5A0[0].score;
	if (v > KAR_D_8005B5A0[1].score) {
		if (KAR_D_8005B5A0[0].score >= 0xa) {
			writePStat(0x79, 2);
		} else {
			writePStat(0x79, 1);
		}
	} else {
		writePStat(0x79, 0);
	}
}

void KAR_tick(void)
{
	KAR_tickMatchState();
	KAR_renderAimArrow();
	KAR_renderStoneCursor();
	KAR_renderScores();
	KAR_renderPowerMeter();
	KAR_renderReadyPrompt();
	KAR_renderNamePlates();
	KAR_handleAimScroll();
	KAR_updateRingMarkers();
	KAR_checkStonesStopped();
}

INCLUDE_ASM("asm/kar/nonmatchings/kar", KAR_tickMatchState);

INCLUDE_ASM("asm/kar/nonmatchings/kar", KAR_renderAimArrow);

void KAR_renderStoneCursor(void)
{
	KarSprite sprite;

	sprite = KAR_D_8005B478;
	if (MAIN_D_80135244 == 5) {
		sprite.x = (MAIN_D_8013523C != 0) ? 0x64 : -0x78;
		sprite.y = (MAIN_D_8013523A * 30) - 0x2d;
		KAR_renderSprite(&sprite);
	}
}

void KAR_renderScores(void)
{
	KarSprite spr;
	KarDigits dig;
	int32_t i;
	int32_t row;
	int8_t score;
	int32_t scoreW;
	int32_t scoreSign;
	int16_t lo;
	int16_t hi;
	int16_t x;

	spr = KAR_D_8005B48C;
	dig = KAR_D_8005B4A0;
	for (i = 0, row = 0; i < 2; i++, row += 0x364) {
		score = (scoreW = ((int8_t *)KAR_D_8005B5A0)[row]);
		lo = score % 10;
		hi = score / 10;
		scoreSign = scoreW;
		x = (i != 0) ? 0x68 : -0x69;
		if (scoreSign < 0) {
			hi = 10;
		}
		if (lo < 0) {
			lo = -lo;
		}
		spr.u = dig.glyph[lo].u;
		spr.v = dig.glyph[lo].v;
		spr.x = x;
		spr.y = -0x3e;
		KAR_renderSprite(&spr);

		spr.u = dig.glyph[hi].u;
		spr.v = dig.glyph[hi].v;
		spr.x = x - 0x10;
		spr.y = -0x3e;
		KAR_renderSprite(&spr);
	}
}

void KAR_renderPowerMeter(void)
{
	KarSpritePair arr;
	int32_t i;
	int32_t off;
	int32_t thr;

	arr = KAR_D_8005B4CC;
	if ((MAIN_D_80135244 >= 8) && (MAIN_D_80135244 < 0xa)) {
		arr.sprite[1].x = (MAIN_D_8013523C != 0) ? 0x32 : -0x32;
		arr.sprite[1].clutX = (MAIN_D_8013523C != 0) ? 0x80 : 0x70;

		i = 10;
		thr = 0x9c4;
		off = 0x1e;
		while (i > 0) {
			if (MAIN_D_8013523E >= thr) {
				arr.sprite[0].x = (arr.sprite[1].x + off) - 3;
				if (i == 10) {
					arr.sprite[0].clutX = 0x30;
				} else {
					arr.sprite[0].clutX = 0x70;
				}
				KAR_renderSprite(&arr.sprite[0]);
			}
			i--;
			off -= 3;
			thr -= 0xfa;
		}
		KAR_renderSprite(&arr.sprite[1]);
	}
}

void KAR_renderReadyPrompt(void)
{
	KarSprite sprite;
	uint32_t t;

	sprite = KAR_D_8005B56C;
	if (MAIN_D_80135244 == 3) {
		t = MAIN_D_80135220;
		MAIN_D_80135220 = t + 1;
		if (t >= 0x31) {
			MAIN_D_80135220 = 0x30;
		}
		if ((MAIN_D_80135220 & 8) != 0) {
			KAR_renderSprite(&sprite);
		}
	} else {
		MAIN_D_80135220 = 0;
	}
}

void KAR_renderNamePlates(void)
{
	KarSpriteSet set;
	KarSprite extra;
	KarSprite *p;
	int32_t i;

	set = KAR_D_8005B4F4;
	extra = KAR_D_8005B558;
	p = set.sprite;
	if (MAIN_D_80135248 == 3) {
		p[2] = extra;
	}

	for (i = 0; i < 4; i++, p++) {
		KAR_renderSprite(p);
	}
}

INCLUDE_ASM("asm/kar/nonmatchings/kar", KAR_handleAimScroll);

INCLUDE_ASM("asm/kar/nonmatchings/kar", KAR_updateRingMarkers);

INCLUDE_ASM("asm/kar/nonmatchings/kar", KAR_checkStonesStopped);

void KAR_updateCollisions(void)
{
	int32_t collided;
	int32_t next;
	VECTOR cur[15];
	VECTOR prev[15];
	int32_t step;
	int32_t i;
	int32_t p;
	int32_t a;
	int32_t b;
	KarStone *stone;
	int32_t tz1;
	int32_t tz2;
	int32_t tx1;
	int32_t tx2;

	collided = 0;

	for (i = 0; i < 15; i++) {
		prev[i].vy = 0;
	}

	KAR_bounceOffWall();

	for (i = 0; i < 15; i++) {
		cur[i].vy = 0;
	}

	if (MAIN_D_80135244 != 0xb) {
		return;
	}

	for (step = 0; step < 0xb; step++) {
		for (p = 0; p < 3; p++) {
			stone = KAR_D_8005B5A0[p].stones;

			for (i = 0; i < 5; ) {
				if (stone->state > 0) {
					tz1 = stone->target.vz * (10 - step);
					tz2 = stone->pos.vz * step;
					tx1 = stone->target.vx * (10 - step);
					tx2 = stone->pos.vx * step;
					cur[p * 5 + i].vx = (tx1 + tx2) / 10;
					cur[p * 5 + i].vz = (tz1 + tz2) / 10;
					cur[p * 5 + i].vy = stone->target.vy;
				}
				i++;
				stone++;
			}
		}

		for (a = 0, next = 1; a < 15; a++, next++) {
			if (KAR_D_8005B5A0[a / 5].stones[a % 5].state > 0) {
				for (b = next; b < 15; b++) {
					if (KAR_D_8005B5A0[b / 5].stones[b % 5].state > 0) {
						if (KAR_distance(cur[a].vx - cur[b].vx, cur[a].vz - cur[b].vz) < 0x96) {
							collided = 1;
							if (step == 0) {
								KAR_resolveStoneCollision(&KAR_D_8005B5A0[b / 5].stones[b % 5], cur[b],
											  &KAR_D_8005B5A0[a / 5].stones[a % 5], cur[a]);
							} else {
								KAR_resolveStoneCollision(&KAR_D_8005B5A0[b / 5].stones[b % 5], prev[b],
											  &KAR_D_8005B5A0[a / 5].stones[a % 5], prev[a]);
							}
						}
					}
				}
			}
		}

		for (i = 0; i < 15; i++) {
			prev[i] = cur[i];
		}

		if ((collided != 0) && (step == 0xa)) {
			step--;
		}
		collided = 0;
	}
}

INCLUDE_ASM("asm/kar/nonmatchings/kar", KAR_bounceOffWall);

int32_t KAR_distance(int32_t x, int32_t y)
{
	return SquareRoot0((uint32_t)(x * x) + (uint32_t)(y * y));
}

void KAR_resolveStoneCollision(KarStone *stoneA, VECTOR a, KarStone *stoneB, VECTOR b)
{
	if ((int16_t)(stoneA->speed + stoneB->speed) >= 0xbb8) {
		playSound2(8, 1);
	} else {
		playSound2(8, 2);
	}

	if (stoneA->state == 2 || stoneA->state == 3 || stoneB->state == 2 || stoneB->state == 3) {
		if (stoneA->state == 2 || stoneA->state == 3) {
			KAR_collidePeggedStone(stoneA, a, stoneB, b);
		} else {
			KAR_collidePeggedStone(stoneB, b, stoneA, a);
		}
	} else if (stoneA->speed <= 0 || stoneB->speed <= 0) {
		if (stoneA->speed <= 0) {
			KAR_collideRestingStone(stoneA, a, stoneB, b);
		} else {
			KAR_collideRestingStone(stoneB, b, stoneA, a);
		}
	} else {
		KAR_collideMovingStones(stoneB, b, stoneA, a);
	}
}

void KAR_collidePeggedStone(KarStone *pegged, VECTOR a, KarStone *mover, VECTOR b)
{
	int32_t ang1;
	int32_t da;
	int32_t dd;
	int32_t r;
	int32_t t;

	KAR_debugCollisionStone(mover, &b, &a);
	ang1 = ratan2(mover->target.vz - mover->pos.vz, mover->target.vx - mover->pos.vx) & 0xfff;
	r = ratan2(b.vz - a.vz, b.vx - a.vx) & 0xfff;
	da = r * 2 - ang1;
	if (da >= 0x1000) {
		da = da % 0x1000;
	}

	dd = abs(r - ang1);
	dd = dd % 0x400;
	dd = 0x400 - dd;
	r = KAR_distance(mover->pos.vx - b.vx, mover->pos.vz - b.vz);
	r = dd * r / 0x400;
	do {
		mover->pos.vx = b.vx + r * rcos(da) / 4096;
		mover->pos.vz = b.vz + r * rsin(da) / 4096;
		r++;
	} while (KAR_distance(mover->pos.vx - pegged->pos.vx, mover->pos.vz - pegged->pos.vz) < 0x97);

	t = (mover->speed >> 1) * dd / 0x400;
	pegged->speed = t;
	mover->speed = mover->speed - t;
	pegged->angle = ratan2(mover->target.vz - b.vz, mover->target.vx - b.vx);
	mover->angle = da;
	pegged->state = 3;
	mover->target = b;
}

void KAR_collideRestingStone(KarStone *resting, VECTOR a, KarStone *mover, VECTOR b)
{
	VECTOR p1;
	VECTOR p2;
	int32_t d;
	int32_t ang;
	int32_t r;
	int32_t share;
	int32_t n;

	KAR_debugCollisionStone(mover, &b, &a);
	if (resting->speed == 0 && mover->speed == 0) {
		resting->pos = a;
		mover->pos = b;
		return;
	}

	share = KAR_computeImpactShare(mover, b, a);
	KAR_computeSeparation(&p1, mover, b, a);
	resting->pos.vx = resting->pos.vx + p1.vx;
	resting->pos.vz = resting->pos.vz + p1.vz;
	mover->pos.vx = mover->pos.vx - p1.vx;
	mover->pos.vz -= p1.vz;
	d = KAR_distance(resting->pos.vx - mover->pos.vx, resting->pos.vz - mover->pos.vz);
	ang = ratan2(p1.vz, p1.vx);
	n = KAR_distance(p1.vx, p1.vz);
	p1 = resting->pos;
	r = 0;
	p2 = mover->pos;
	if (n != 0) {
		while (d < 0x97) {
			resting->pos.vx = p1.vx + r * rcos(ang) / 4096;
			resting->pos.vz = p1.vz + r * rsin(ang) / 4096;
			mover->pos.vx = p2.vx - r * rcos(ang) / 4096;
			mover->pos.vz = p2.vz - r * rsin(ang) / 4096;
			d = KAR_distance(resting->pos.vx - mover->pos.vx, resting->pos.vz - mover->pos.vz);
			r++;
		}
	} else {
		ang = ratan2(mover->pos.vz - resting->pos.vz, mover->pos.vx - resting->pos.vx);
		while (d < 0x97) {
			mover->pos.vx = p2.vx + r * rcos(ang) / 4096;
			mover->pos.vz = p2.vz + r * rsin(ang) / 4096;
			d = KAR_distance(resting->pos.vx - mover->pos.vx, resting->pos.vz - mover->pos.vz);
			r++;
		}
	}

	resting->target = a;
	mover->target = b;
	mover->speed = mover->speed - share;
	resting->speed = share;
	resting->angle = ratan2(resting->pos.vz - a.vz, resting->pos.vx - a.vx) & 0xfff;
	mover->angle = ratan2(mover->pos.vz - b.vz, mover->pos.vx - b.vx) & 0xfff;
}

void KAR_collideMovingStones(KarStone *stoneA, VECTOR a, KarStone *stoneB, VECTOR b)
{
	VECTOR p1;
	VECTOR p2;
	int32_t share1;
	int32_t share2;
	int32_t dx;
	int32_t dz;
	int32_t d;
	int32_t ang;
	int32_t r;

	KAR_debugCollisionPair(stoneA, &a, stoneB, &b);
	share1 = KAR_computeImpactShare(stoneA, a, b);
	share2 = KAR_computeImpactShare(stoneB, b, a);
	KAR_computeSeparation(&p1, stoneB, b, a);
	KAR_computeSeparation(&p2, stoneA, a, b);
	dx = p2.vx - p1.vx;
	dz = p2.vz - p1.vz;
	ang = KAR_distance(dx, dz);
	if (ang != 0) {
		ang = ratan2(dz, dx);
		stoneB->pos.vx = stoneB->pos.vx + dx;
		stoneB->pos.vz = stoneB->pos.vz + dz;
		stoneA->pos.vx = stoneA->pos.vx - dx;
		stoneA->pos.vz = stoneA->pos.vz - dz;
		d = KAR_distance(stoneA->pos.vx - stoneB->pos.vx, stoneA->pos.vz - stoneB->pos.vz);
		p1 = stoneA->pos;
		p2 = stoneB->pos;
		stoneA->speed += (int16_t)(share2 - share1);
		stoneB->speed += (int16_t)(share1 - share2);
		r = 0;
		if (stoneA->speed > stoneB->speed) {
			while (d < 0x97) {
				stoneA->pos.vx = p1.vx + r * rcos(ang) / 4096;
				stoneA->pos.vz = p1.vz + r * rsin(ang) / 4096;
				d = KAR_distance(stoneB->pos.vx - stoneA->pos.vx, stoneB->pos.vz - stoneA->pos.vz);
				r++;
			}
		} else {
			while (d < 0x97) {
				stoneB->pos.vx = p2.vx + r * rcos(ang) / 4096;
				stoneB->pos.vz = p2.vz + r * rsin(ang) / 4096;
				d = KAR_distance(stoneB->pos.vx - stoneA->pos.vx, stoneB->pos.vz - stoneA->pos.vz);
				r++;
			}
		}
		stoneA->target = a;
		stoneB->target = b;
		stoneA->angle = ratan2(stoneA->pos.vz - a.vz, stoneA->pos.vx - a.vx);
		stoneB->angle = ratan2(stoneB->pos.vz - b.vz, stoneB->pos.vx - b.vx);
	} else {
		KAR_distance(stoneA->pos.vx - stoneB->pos.vx, stoneA->pos.vz - stoneB->pos.vz);
		if (stoneB->speed / stoneA->weight < 5 || stoneA->speed / stoneB->weight < 5) {
			stoneB->speed = 0;
			stoneB->pos = stoneB->target;
			stoneA->pos = stoneA->target;
		} else {
			stoneB->pos = b;
			stoneA->pos = a;
		}
	}
}

void KAR_debugCollisionStone(KarStone *stone, VECTOR *b, VECTOR *a)
{
}

int32_t KAR_computeImpactShare(KarStone *stone, VECTOR a, VECTOR b)
{
	int32_t ang;
	int32_t d;
	int32_t ret;

	ang = stone->angle & 0xfff;
	d = (ang - (ratan2(b.vz - a.vz, b.vx - a.vx) & 0xfff)) & 0xfff;
	ret = 0;
	if (d < 0x400 || d > 0xc00) {
		if (d > 0xc00) {
			d = (0x1000 - d) & 0xfff;
		}
		d = 0x400 - (d % 0x400);
		ret = d * stone->speed / 0x400;
	}

	return ret;
}

int32_t KAR_computeSeparation(VECTOR *out, KarStone *stone, VECTOR a, VECTOR b)
{
	VECTOR v;
	int32_t cur;
	int32_t ang;
	int32_t d;
	int32_t r;
	int32_t n;

	cur = stone->angle & 0xfff;
	ang = ratan2(b.vz - a.vz, b.vx - a.vx) & 0xfff;
	d = abs(cur - ang);
	v.vz = 0;
	v.vy = 0;
	v.vx = 0;
	if (d < 0x400 || d > 0xc00) {
		d = d % 0x400;
		r = (0x400 - d) * KAR_distance(stone->pos.vz - a.vz, stone->pos.vx - a.vx) / 0x400;
		do {
			v.vx = r * rcos(ang) / 4096;
			v.vz = r * rsin(ang) / 4096;
			n = KAR_distance(v.vx, v.vz);
			r++;
		} while (n == 0);
	}

	*out = v;
}

void KAR_debugCollisionPair(KarStone *stoneA, VECTOR *a, KarStone *stoneB, VECTOR *b)
{
}

int32_t KAR_getWallZone(int16_t x, int16_t z)
{
	if (z >= 0x465) {
		return 6;
	}

	if (z >= -0x7d0) {
		if (x < -0x2d4) {
			return 1;
		}
		if (x >= 0x2d5) {
			return 2;
		}
	} else {
		if (z + (x * 2) < -0xdc4 && x < 0x1aa) {
			return 3;
		}
		if (z - (x * 2) < -0xdc4 && x >= 0x1a9) {
			return 4;
		}
		if (z < -0x9dc) {
			return 5;
		}
	}

	return 0;
}

int32_t KAR_findWallContact(VECTOR *out, KarStone *stone, int32_t flag)
{
	int32_t t;
	int32_t i;
	int32_t ang;
	VECTOR v;

	i = 0;
	ang = (stone->angle + 0x800) & 0xfff;
	do {
		v.vx = stone->pos.vx + i * rcos(ang) / 4096;
		v.vz = stone->pos.vz + i * rsin(ang) / 4096;
		i++;
		if (flag == 0) {
			t = (v.vz + 0xdc5) + v.vx * 2;
		} else {
			t = (v.vz + 0xdc5) - v.vx * 2;
		}
	} while (t <= 0);

	*out = v;
}

void KAR_reflectOffDiagonal(int32_t *p, int32_t dir)
{
	int32_t cur;
	int32_t ang;

	cur = (p[3] + 0x800) & 0xfff;
	if (dir == 0) {
		ang = ratan2(1, 2) * 2;
	} else {
		ang = ratan2(1, -2) * 2;
	}

	p[3] = ang - cur;
}

void KAR_placeAtContact(KarStone *stone, KarPos p)
{
	int32_t d;

	d = KAR_distance(p.x - stone->target.vx, p.z - stone->target.vz);
	d = KAR_distance(stone->target.vz - stone->pos.vz, stone->target.vx - stone->pos.vx) - d;
	stone->pos.vx = p.x + ((d * rcos(stone->angle)) / 4096);
	stone->pos.vz = p.z + ((d * rsin(stone->angle)) / 4096);
}

void KAR_beginAiming(void)
{
	int8_t idx;
	int8_t n;

	idx = (MAIN_D_8013523C != 0) ? MAIN_D_80135248 : 0;

	MAIN_D_80135244 = 6;
	if (idx == 0) {
		startAnimation(ENTITY_TABLE[0], 0x23);
	} else if (MAIN_D_80135248 == 2) {
		startAnimation(ENTITY_TABLE[MAIN_D_80135248], 0x1c);
	}

	KAR_D_8005B5A0[MAIN_D_8013523C].stones[MAIN_D_8013523A].state = 0;
	if (KAR_D_8005B5A0[MAIN_D_8013523C].stones[MAIN_D_8013523A].type == 3) {
		n = (MAIN_D_8013523C != 0) ? 1 : 2;
		GsLinkObject4((u_long)(MAIN_D_80135240 + 3), &KAR_D_8005B5A0[MAIN_D_8013523C].stones[MAIN_D_8013523A].obj, n);
	}

	MAIN_D_8013524C = -0x400;
	ENTITY_TABLE[idx]->posData->rotation.vy = 0;
}

void KAR_selectPreviousStone(void)
{
	int32_t row;

	if (MAIN_D_8013523A > 0) {
		for (row = MAIN_D_8013523A - 1; row >= 0; row--) {
			if (KAR_D_8005B5A0[MAIN_D_8013523C].stones[row].state == -1) {
				MAIN_D_8013523A = row;
				return;
			}
		}
	}
}

void KAR_selectNextStone(void)
{
	int32_t row;

	if (MAIN_D_8013523A < 4) {
		for (row = MAIN_D_8013523A + 1; row < 5; row++) {
			if (KAR_D_8005B5A0[MAIN_D_8013523C].stones[row].state == -1) {
				MAIN_D_8013523A = row;
				return;
			}
		}
	}
}

void KAR_turnAimLeft(int32_t delta)
{
	int8_t idx;
	PositionData *pos;

	idx = (MAIN_D_8013523C != 0) ? MAIN_D_80135248 : 0;
	pos = ENTITY_TABLE[idx]->posData;
	pos->rotation.vy = pos->rotation.vy - delta;
	MAIN_D_8013524C += delta;
}

void KAR_turnAimRight(int32_t delta)
{
	int8_t idx;
	PositionData *pos;

	idx = (MAIN_D_8013523C != 0) ? MAIN_D_80135248 : 0;
	pos = ENTITY_TABLE[idx]->posData;
	pos->rotation.vy = pos->rotation.vy - delta;
	MAIN_D_8013524C += delta;
}

void KAR_beginThrow(void)
{
	MAIN_D_80135244 = 7;
	if (MAIN_D_8013523C == 0) {
		startAnimation(ENTITY_TABLE[0], 0x24);
	} else if (MAIN_D_80135248 == 2) {
		startAnimation(ENTITY_TABLE[MAIN_D_80135248], 0x1d);
	} else {
		startAnimation(ENTITY_TABLE[MAIN_D_80135248], 0x1f);
	}
}

int32_t KAR_tickYesNoPrompt(void)
{
	RECT finalPos;
	RECT startPos;
	KarStrPair strs;
	int32_t mode;

	finalPos = MAIN_D_80134A10;
	startPos = MAIN_D_80134A18;
	strs = MAIN_D_80134A20;
	while (MAIN_D_80135250 < 2) {
		mode = MAIN_D_80135250;
		drawString(strs.text[mode], 0, (mode * 13) + 0xdd);
		MAIN_D_80135250 = MAIN_D_80135250 + 1;
		break;
	}

	if (UI_BOX_DATA[0].state == 0) {
		if (MAIN_D_80135250 == 2) {
			createAnimatedUIBox(0, 0, 2, &finalPos, &startPos, NULL, NULL);
			MAIN_D_80135250 = 3;
		} else if (MAIN_D_80135250 == 3 || MAIN_D_80135250 == 4) {
			return (int8_t)(MAIN_D_80135250 - 2);
		}
	}

	if (UI_BOX_DATA[0].state == 1) {
		if ((POLLED_INPUT & 0x40) != 0 && (POLLED_INPUT_PREVIOUS & 0x40) == 0) {
			removeAnimatedUIBox(0, NULL);
			return 0;
		}
		if ((POLLED_INPUT & 0x1000) != 0 && (POLLED_INPUT_PREVIOUS & 0x1000) == 0) {
			MAIN_D_80135250 = (MAIN_D_80135250 == 3) ? 4 : 3;
			playSound(0, 2);
		} else if ((POLLED_INPUT & 0x4000) != 0 && (POLLED_INPUT_PREVIOUS & 0x4000) == 0) {
			MAIN_D_80135250 = (MAIN_D_80135250 == 3) ? 4 : 3;
			playSound(0, 2);
		}
		renderString(0, -0x7c, 0x30, 0x52, 0xd, 0, 0xdc, 6, 1);
		renderString(0, -0x7c, 0x3d, 0x5c, 0xd, 0x53, 0xdc, 6, 1);
		renderSelectionCursor(finalPos.x + 6, finalPos.y + 7 + ((MAIN_D_80135250 - 3) * 13), 0x5e, 0xd, 6);
	}

	return 0;
}

int32_t KAR_tickHintBox(int32_t n)
{
	RECT finalPos;
	RECT startPos;
	int8_t mode;

	finalPos = MAIN_D_80134A28;
	startPos = MAIN_D_80134A30;
	if (MAIN_D_80135250 == 0) {
		return 0;
	}

	mode = MAIN_D_80135250;

	if (n < 4) {
		startPos.x = 0x4b;
		startPos.y = -5;
	}

	if (UI_BOX_DATA[0].state == 0) {
		if (mode == 1) {
			createAnimatedUIBox(0, 0, 2, &finalPos, &startPos, NULL, NULL);
			clearTextArea();
			MAIN_D_8013522C = 0;
			return 1;
		}
		return 0;
	}

	if (MAIN_D_80135248 == 2) {
		n = (int8_t)KAR_drawHintPagePenguinmon(n, MAIN_D_8013522C);
	} else {
		n = (int8_t)KAR_drawHintPageMetalMamemon(n, MAIN_D_8013522C);
	}

	if (MAIN_D_8013522C < n * 4) {
		MAIN_D_8013522C++;
	}

	if (UI_BOX_DATA[0].state == 1) {
		if ((POLLED_INPUT & 0x40) != 0 && (POLLED_INPUT_PREVIOUS & 0x40) == 0) {
			MAIN_D_80135250 = MAIN_D_80135250 + 1;
			playSound(0, 3);
			if (n < MAIN_D_80135250) {
				removeAnimatedUIBox(0, NULL);
				MAIN_D_80135250 = -1;
			}
		}
		if (MAIN_D_80135250 != -1) {
			renderString(0, UI_BOX_DATA[0].finalPos.x + 4, UI_BOX_DATA[0].finalPos.y + 2, 0xfc, 0x34, 0,
			             (MAIN_D_80135250 - 1) * 52, 6, 1);
			renderUIBox(0);
		}
	}

	return 1;
}

void KAR_classifyStoneRings(void)
{
	KarStone *stone;
	KarStone *cur;
	int32_t p;
	int32_t i;
	int32_t zi;

	for (p = 0; p < 2; p++) {
		stone = KAR_D_8005B5A0[p].stones;
		KAR_D_8005B5A0[p].score = 0;
		for (i = 0; i < 5; i++, stone++) {
			cur = stone;
			stone->prevRing = cur->ring;
			stone->ring = 0;
			if (stone->state <= 0) {
				continue;
			}
			if (cur->pos.vz >= -0x564) {
				continue;
			}
			KAR_distance(stone->pos.vx, stone->pos.vz + 0x6a4);
			for (zi = 0; zi < 4; zi++) {
				KarZones z;

				z = KAR_D_8005B438;
				if (KAR_distance(stone->pos.vx + z.ring[zi].dx, cur->pos.vz + z.ring[zi].dz) > z.ring[zi].radius) {
					continue;
				}
				stone->ring = zi + 1;
				if (p != 0) {
					stone->ring = stone->ring | 0x10;
				}
				break;
			}
		}
	}

	KAR_registerThrownStone();
}

INCLUDE_ASM("asm/kar/nonmatchings/kar", KAR_tickScoreTally);

INCLUDE_ASM("asm/kar/nonmatchings/kar", KAR_chooseOpponentShot);

int32_t KAR_drawHintPagePenguinmon(int32_t idx, int8_t n)
{
	KarStrTbl strs;
	KarOffTbl offs;

	strs = KAR_D_8005AF58;
	offs = KAR_D_8005B04C;
	if ((n % 4) == 0) {
		setTextColor(7);
	} else {
		setTextColor(1);
	}

	drawString(strs.text[offs.start[idx] + n], 0, (n * 13) + 1);

	return (int8_t)((offs.start[idx + 1] - offs.start[idx]) / 4);
}

int32_t KAR_drawHintPageMetalMamemon(int32_t idx, int8_t n)
{
	KarStrTbl strs;
	KarOffTbl offs;

	strs = KAR_D_8005B318;
	offs = KAR_D_8005B40C;
	if ((n % 4) == 0) {
		setTextColor(7);
	} else {
		setTextColor(1);
	}

	drawString(strs.text[offs.start[idx] + n], 0, (n * 13) + 1);

	return (int8_t)((offs.start[idx + 1] - offs.start[idx]) / 4);
}

INCLUDE_ASM("asm/kar/nonmatchings/kar", KAR_findClearShotAngle);

int32_t KAR_aimAtStoneInRing(int32_t player, int32_t key, int16_t *outX, int16_t *outZ)
{
	KarStone *stone;
	int32_t i;
	int32_t r;

	stone = KAR_D_8005B5A0[player].stones;
	for (i = 0; i < 5; i++, stone++) {
		if ((stone->state < 2) && (stone->ring == key)) {
			r = KAR_findClearShotAngle((int16_t)stone->pos.vx, (int16_t)stone->pos.vz);
			*outX = stone->pos.vx;
			*outZ = stone->pos.vz;
			return r;
		}
	}

	return -1;
}

void KAR_setOpponentShot(int32_t row, int32_t val, int32_t b, int32_t c)
{
	KarStone *stone;
	int32_t i;
	int32_t n;

	for (i = 0; i < 4; i++) {
		KarShotPlan t;

		t = KAR_D_8005B418;
		n = KAR_findUnusedStoneOfType((int8_t)t.type[row][i]);
		if (n >= 0) {
			KAR_D_8005B5A0[1].unk4 = n;
			stone = &KAR_D_8005B5A0[1].stones[n];
			break;
		}
	}

	stone->angle = val;
	KAR_D_8005B5A0[1].unk2 = KAR_computeThrowPower(stone->weight, b, c);
}

int32_t KAR_aimAtRandomStone(void)
{
	int32_t i;
	KarStone *cands[5];
	KarStone *stone;
	uint8_t count;
	int16_t px;
	int16_t pz;

	stone = KAR_D_8005B5A0[0].stones;
	if (KAR_D_80063914[0] < 0) {
		return ratan2(-0xc1c, -0x190);
	}

	for (i = 0, count = 0; i < 5; i++, stone++) {
		if (stone->state == 1) {
			cands[count] = stone;
			count++;
		}
	}

	if (count == 0) {
		px = 0;
		pz = -0x6a4;
	} else {
		uint8_t idx;

		idx = rand() % count;
		px = cands[idx]->pos.vx;
		pz = cands[idx]->pos.vz;
	}

	return ratan2(pz - 0x578, px - 0x190);
}

int32_t KAR_aimBankShot(KarStone *stone, int32_t x, int32_t z)
{
	int32_t ang;
	int32_t cx;
	int32_t cz;
	int32_t *pd;

	ang = ratan2(stone->pos.vz - z, stone->pos.vx - x);
	cx = stone->pos.vx + ((rcos(ang) * 150) / 4096);
	cz = stone->pos.vz + ((rsin(ang) * 150) / 4096);
	cx = -0x2d5 - (cx + 0x2d5);
	pd = (int32_t *)ENTITY_TABLE[MAIN_D_80135248]->posData;

	return ratan2(cz - pd[0x9c], cx - pd[0x9a]);
}

int32_t KAR_findUnusedStoneOfType(int32_t key)
{
	KarStone *stone;
	int32_t i;

	stone = KAR_D_8005B5A0[1].stones;
	for (i = 0; i < 5; i++, stone++) {
		if ((stone->state == -1) && (stone->type == key)) {
			return i;
		}
	}

	return -1;
}

INCLUDE_ASM("asm/kar/nonmatchings/kar", KAR_computeThrowPower);

void KAR_registerThrownStone(void)
{
	MAIN_D_80135256 = KAR_D_8005B5A0[0].thrown + KAR_D_8005B5A0[1].thrown;
	KAR_D_800639C0[MAIN_D_80135256] =
		&KAR_D_8005B5A0[MAIN_D_8013523C].stones[MAIN_D_8013523A];
	MAIN_D_80134A4F[0] = -1;
}

void KAR_rotatePoint(SVECTOR *p, int32_t ang)
{
	SVECTOR t;

	t = *p;
	p->vx = ((t.vx * rcos(ang)) / 4096) - ((t.vz * rsin(ang)) / 4096);
	p->vz = ((t.vx * rsin(ang)) / 4096) + ((t.vz * rcos(ang)) / 4096);
}

void KAR_renderSprite(KarSprite *sp)
{
	POLY_FT4 *p;

	p = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(p);
	p->tpage = 0xa;
	p->r0 = sp->r;
	p->g0 = sp->g;
	p->b0 = sp->b;
	p->clut = GetClut(sp->clutX, sp->clutY);
	p->u0 = sp->u;
	p->v0 = sp->v;
	p->u1 = sp->u + sp->w;
	p->v1 = sp->v;
	p->u2 = sp->u;
	p->v2 = sp->v + sp->h;
	p->u3 = sp->u + sp->w;
	p->v3 = sp->v + sp->h;
	p->x0 = sp->x;
	p->y0 = sp->y;
	p->x1 = sp->x + sp->w;
	p->y1 = sp->y;
	p->x2 = sp->x;
	p->y2 = sp->y + sp->h;
	p->x3 = sp->x + sp->w;
	p->y3 = sp->y + sp->h;
	AddPrim(ACTIVE_ORDERING_TABLE->org + 0xa, p++);
	GsSetWorkBase((PACKET *)p);
}
