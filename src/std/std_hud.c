#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libcd.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/aabb.h>
#include <dw/clock.h>
#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/graphics.h>
#include <dw/math.h>
#include <dw/model.h>
#include <dw/move.h>
#include <dw/params.h>
#include <dw/sound.h>
#include <dw/std.h>
#include <dw/types.h>

#include "common.h"

typedef struct {
	uint32_t *vertTop;
	int32_t numVert;
	uint32_t *normalTop;
	int32_t numNormal;
	uint32_t *primTop;
	int32_t numPrim;
	int32_t scale;
} StdTmdObject;

extern GsOT *ACTIVE_ORDERING_TABLE;
extern uint32_t POLLED_INPUT;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern int16_t MAIN_D_8013518C[2];
extern int32_t MAIN_D_80135194;
extern int16_t STD_D_8007A738[][2];
extern int16_t STD_D_8007A73A[][2];
extern int8_t MAIN_D_80135190;
extern char STD_D_8007BB94[];
extern int16_t MAIN_D_801351A4;
extern uint8_t MAIN_D_801351B4;
extern int16_t STD_D_8007FA08[];
extern uint8_t MAIN_D_80134D64;
extern int8_t GAME_STATE;
extern uint8_t MAIN_D_80135198[2];
extern uint8_t MAIN_D_8013519A[2];
extern uint8_t MAIN_D_8013519C[2];
extern uint8_t MAIN_D_8013519E[2];
extern uint8_t MAIN_D_801351A0[2];
extern uint8_t MAIN_D_801351A2[2];
extern int16_t MAIN_D_8013518E;
extern PositionData STD_D_8007C7B0[4];
extern int16_t MAIN_D_801348DC[4];
extern GsOT STD_D_8007B714[];
extern int32_t STD_D_8007AA10[];
extern GsRVIEW2 GS_VIEWPOINT;
extern int32_t ACTIVE_FRAMEBUFFER;
extern int32_t VIEWPORT_DISTANCE;
extern uint8_t *MAIN_D_801351A8;
extern char *MAIN_D_801348D8;
extern int32_t STD_D_8007A9A4[];
extern uint8_t MAIN_D_801348C0[8];
extern uint8_t MAIN_D_801348C8[8];
extern uint8_t MAIN_D_801348D0[8];

void STD_func_8006B1F0(uint32_t *tmd, int32_t vofs, int32_t nofs, int32_t objIdx);
void STD_func_8006B6F4(void);
void startAnimation(Entity *entity, int32_t animId);
void STD_func_8005A830(void);
void STD_renderBattleStartText(void);
void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount, int32_t *buf);
void setEntityTextDigit(POLY_FT4 *poly, int32_t x, int32_t y);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t u, int32_t v, int32_t w, int32_t h);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t x, int32_t y, int32_t w, int32_t h);
void swapByte(char *a, char *b);
void STD_renderBattleStartTextBurst(void);
void addObject(int32_t objectId, int32_t instanceId, void *tick, void *render);
void removeObject(int32_t objectId, int32_t instanceId);
void STD_tickFinisherChargeup(void);
void STD_renderFinisherChargeup(void);
void STD_removeFinisherChargeup(void);
void STD_shuffleBattleStartTextPieces(void);
void STD_initializeBattleStartText(void);
void STD_func_8006A044(void);
void STD_initializeBattleStartTextBurst(void);
void STD_func_8006A508(void);
int32_t STD_func_8006A514(void);
void STD_renderNumber(int32_t a, int32_t digits, int32_t x, int32_t y, int16_t value, int32_t layer);
void STD_func_8006B1E4(int32_t i);
int32_t STD_isVersusModelSceneFinished(void);
void STD_func_8006B6E8(void);
void STD_func_80069468(void);
void STD_func_8006AD68(void);
void STD_func_8006BA18(void);
void STD_func_8006B468(void);
void tickAnimation(Entity *entity);
void STD_func_8006AD00(int32_t i);
void STD_func_80069134(int16_t tech);
void setupModelMatrix(PositionData *posData);
void GsSortBoxFill(GsBOXF *bp, GsOT *ot, unsigned short pri);
void STD_func_8006A824(int32_t id);
void STD_renderVersusModelScene(void);
void STD_func_8006BE64(void);
int32_t readFile(char *path, uint8_t *buffer);
void STD_func_8006B2BC(void);
void STD_tickVersusModelScene(void);
void STD_renderHPBarDigits(int16_t i, int32_t id);
void STD_renderFighterHPBar(int32_t id);

static void *std_hud_functions[] = {
	STD_func_8006BE64,
	STD_func_8006BA18,
	STD_func_8006B6F4,
	STD_func_8006B6E8,
	STD_isVersusModelSceneFinished,
	STD_renderVersusModelScene,
	STD_tickVersusModelScene,
	STD_func_8006B468,
	STD_func_8006B2BC,
	STD_func_8006B1F0,
	STD_func_8006B1E4,
	STD_func_8006AD68,
	STD_func_8006AD00,
	STD_renderHPBarDigits,
	STD_func_8006A824,
	STD_renderFighterHPBar,
	STD_renderNumber,
	STD_func_8006A514,
	STD_func_8006A508,
	STD_renderBattleStartTextBurst,
	STD_initializeBattleStartTextBurst,
	STD_func_8006A044,
	STD_renderBattleStartText,
	STD_initializeBattleStartText,
	STD_shuffleBattleStartTextPieces,
	STD_tickNPCTournament,
	STD_tickPartnerTournament,
	STD_tickTamerTournament,
	STD_func_80069468,
	STD_removeFinisherChargeup,
	STD_renderFinisherChargeup,
	STD_tickFinisherChargeup,
	STD_func_80069134,
};

void STD_func_80069134(int16_t tech)
{
	COMBAT_DATA_PTR->player.finisherChargeup[0] = 0;
	COMBAT_DATA_PTR->player.remainingChargeupTime[0] = 0x50;
	COMBAT_DATA_PTR->fighter[0].finisherProgress = 0;
	if (COMBAT_DATA_PTR->player.hoveredCommand[0] == 0) {
		COMBAT_DATA_PTR->player.hoveredCommand[0] = COMBAT_DATA_PTR->player.numCommands[0] - 1;
	}

	COMBAT_DATA_PTR->player.currentCommand[0] = 3;
	COMBAT_DATA_PTR->player.bufferedCommand[0] = 3;
	MAIN_D_8013518C[0] = -0x8C;
	MAIN_D_8013518E = -0x4A;
	addObject(0x19A, 0, STD_tickFinisherChargeup, STD_renderFinisherChargeup);
}

void STD_tickFinisherChargeup(void)
{
	int32_t up;
	uint32_t charge;

	COMBAT_DATA_PTR->player.remainingChargeupTime[0]--;
	up = 0;
	if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 4) != 0) {
		up = 1;
	}

	if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 8) != 0) {
		up = 1;
	}

	charge = COMBAT_DATA_PTR->player.finisherChargeup[0];
	if ((charge < 0x50) && (up != 0)) {
		COMBAT_DATA_PTR->player.finisherChargeup[0] = charge + 2;
	}
}

void STD_renderFinisherChargeup(void)
{
	POLY_FT4 prim;
	int32_t i;
	int32_t x;
	int16_t bars;

	SetPolyFT4(&prim);
	prim.tpage = 0x1f;
	prim.clut = GetClut(0x110, 0x1f2);
	prim.r0 = 0x80;
	prim.g0 = 0x80;
	prim.b0 = 0x80;
	prim.u0 = 0x58;
	prim.v0 = 0xe0;
	prim.u1 = 0x86;
	prim.v1 = 0xe0;
	prim.u2 = 0x58;
	prim.v2 = 0xec;
	prim.u3 = 0x86;
	prim.v3 = 0xec;
	prim.x0 = MAIN_D_8013518C[0];
	prim.y0 = MAIN_D_8013518C[1];
	prim.x1 = MAIN_D_8013518C[0] + 0x2e;
	prim.y1 = MAIN_D_8013518C[1];
	prim.x2 = MAIN_D_8013518C[0];
	prim.y2 = MAIN_D_8013518C[1] + 0xc;
	prim.x3 = MAIN_D_8013518C[0] + 0x2e;
	prim.y3 = MAIN_D_8013518C[1] + 0xc;
	GsSortPoly(&prim, ACTIVE_ORDERING_TABLE, 7);
	bars = COMBAT_DATA_PTR->player.finisherChargeup[0] / 8;
	prim.u0 = 0x88;
	prim.v0 = 0xe0;
	prim.u1 = 0x8c;
	prim.v1 = 0xe0;
	prim.u2 = 0x88;
	prim.v2 = 0xe6;
	prim.u3 = 0x8c;
	prim.v3 = 0xe6;
	for (i = 0, x = 0; i < bars; i++, x += 4) {
		prim.x0 = MAIN_D_8013518C[0] + 3 + x;
		prim.y0 = MAIN_D_8013518C[1] + 3;
		prim.x1 = (int32_t)(MAIN_D_8013518C[0] + 3 + x) + 4;
		prim.y1 = MAIN_D_8013518C[1] + 3;
		prim.x2 = MAIN_D_8013518C[0] + 3 + x;
		prim.y2 = MAIN_D_8013518C[1] + 9;
		prim.x3 = (int32_t)(MAIN_D_8013518C[0] + 3 + x) + 4;
		prim.y3 = MAIN_D_8013518C[1] + 9;
		GsSortPoly(&prim, ACTIVE_ORDERING_TABLE, 7);
	}

	if (COMBAT_DATA_PTR->player.remainingChargeupTime[0] == 0) {
		STD_removeFinisherChargeup();
	}
}

void STD_removeFinisherChargeup(void)
{
	if (COMBAT_DATA_PTR->player.remainingChargeupTime[0] != -1) {
		removeObject(0x19a, 0);
		COMBAT_DATA_PTR->player.remainingChargeupTime[0] = -1;
	}
}

void STD_func_80069468(void)
{
	int32_t d;

	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x8000) {
		playSound(0, 2);
		COMBAT_DATA_PTR->player.hoveredCommand[0]++;
		if (COMBAT_DATA_PTR->player.numCommands[0] - 1 < COMBAT_DATA_PTR->player.hoveredCommand[0]) {
			COMBAT_DATA_PTR->player.hoveredCommand[0] = 1;
		}
	}
	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x2000) {
		playSound(0, 2);
		COMBAT_DATA_PTR->player.hoveredCommand[0]--;
		if (COMBAT_DATA_PTR->player.hoveredCommand[0] <= 0) {
			COMBAT_DATA_PTR->player.hoveredCommand[0] = COMBAT_DATA_PTR->player.numCommands[0] - 1;
		}
	}
	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x40) {
		playSound(0, 3);
		COMBAT_DATA_PTR->player.bufferedCommand[0] = COMBAT_DATA_PTR->player.availableCommands[0][COMBAT_DATA_PTR->player.hoveredCommand[0]];
		if ((d = PARTNER_PARA.discipline) < 0x46) {
			COMBAT_DATA_PTR->player.commandDelay[0] = 0xa0 - d / 10;
		} else {
			COMBAT_DATA_PTR->player.commandDelay[0] = (0xa - d / 10) * 10;
		}
		COMBAT_DATA_PTR->player.commandDelay[0] = 0;
	}
	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x80) {
		if (COMBAT_DATA_PTR->fighter[0].finisherProgress == COMBAT_DATA_PTR->fighter[0].finisherGoal) {
			COMBAT_DATA_PTR->player.bufferedCommand[0] = 0xb;
			playSound(0, 3);
			COMBAT_DATA_PTR->player.commandDelay[0] = 0;
			COMBAT_DATA_PTR->player.currentCommand[0] = 0xb;
		}
	}
}
void STD_tickTamerTournament(int32_t instanceId)
{
	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x100) {
		MAIN_D_80134D64 = (MAIN_D_80134D64 + 1) & 1;
	}
	if (GAME_STATE == 4) {
		STD_func_80069468();
	}
}

void STD_tickPartnerTournament(int32_t i)
{
	tickAnimation(ENTITY_TABLE[i]);
}

void STD_tickNPCTournament(int32_t i)
{
	tickAnimation(ENTITY_TABLE[i]);
}

void STD_shuffleBattleStartTextPieces(void)
{
	int32_t i;

	for (i = 0; i < 0x9b; i++) {
		swapByte(&((char (*)[20])STD_D_8007BB94)[i][0x11], &((char (*)[20])STD_D_8007BB94)[random(0x9b)][0x11]);
	}
}

void STD_initializeBattleStartText(void)
{
	char (*p)[20];
	int32_t sgn;
	int32_t i;
	int32_t r;
	int32_t t;

	MAIN_D_80135190 = 0;
	MAIN_D_80135194 = 0;
	p = (char (*)[20])STD_D_8007BB94;
	for (i = 0; i < 0x9b; i++, p++) {
		(*p)[0x11] = i;
		(*p)[0x12] = 0x18;
		(*p)[0x13] = random(3);
	}

	STD_shuffleBattleStartTextPieces();

	p = (char (*)[20])STD_D_8007BB94;
	for (i = 0; i < 0x9b; i++, p++) {
		if (random(2) == 1) {
			sgn = 1;
		} else {
			sgn = -1;
		}
		((int16_t *)*p)[5] = STD_D_8007A73A[i][0];
		(*p)[0x10] = -sgn * ((random(3) + 1) << 5);
		if ((0 <= i) && (i < 0x33)) {
			((int16_t *)*p)[4] = (sgn * 500) + random(100) - 50;
		} else if ((0x33 <= i) && (i < 0x65)) {
			((int16_t *)*p)[4] = (sgn * 600) + random(100) - 50;
		} else {
			((int16_t *)*p)[4] = (sgn * 700) + random(100) - 50;
		}
		r = random(5);
		t = STD_D_8007A738[i][0];
		((int16_t *)*p)[6] = (r + 8) * t / 8;
		t = STD_D_8007A73A[i][0];
		((int16_t *)*p)[7] = (r + 8) * t / 8;
		((int16_t *)*p)[0] = 0;
		((int16_t *)*p)[1] = 0;
		((int16_t *)*p)[2] = 0;
	}

	addObject(0x1a6, 0, NULL, STD_renderBattleStartText);
}

INCLUDE_ASM("asm/std/nonmatchings/std_hud", STD_renderBattleStartText);

void STD_func_8006A044(void)
{
	removeObject(0x1a6, 0);
}

void STD_initializeBattleStartTextBurst(void)
{
	MAIN_D_80135194 = 0;
	addObject(0x1a6, 0, NULL, STD_renderBattleStartTextBurst);
}

INCLUDE_ASM("asm/std/nonmatchings/std_hud", STD_renderBattleStartTextBurst);

void STD_func_8006A508(void)
{
	removeObject(0x1a6, 0);
}

int32_t STD_func_8006A514(void)
{
	return MAIN_D_80135194;
}

void STD_renderNumber(int32_t a, int32_t digits, int32_t x, int32_t y, int16_t value, int32_t layer)
{
	POLY_FT4 *prim;
	int32_t i;
	uint32_t width;
	int32_t count;
	int32_t buf[4];

	prim = (POLY_FT4 *)GsGetWorkBase();

	width = digits;
	convertValueToDigits(digits, value, &count, buf);

	for (i = count - 1; i >= 0; i--) {
		setEntityTextDigit(prim, 256, 492);
		prim->r0 = 0x80;
		prim->g0 = 0x80;
		prim->b0 = 0x80;
		setUVDataPolyFT4(prim, buf[i] * 7, 172, 7, 11);
		setPosDataPolyFT4(prim, x + ((((int32_t)width - 1) - i) * 7), y, 7, 11);
		AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	}

	GsSetWorkBase((PACKET *)prim);
}

void STD_renderFighterHPBar(int32_t id)
{
	int32_t n;
	int32_t i;
	uint8_t v;
	uint32_t k;
	uint32_t v2;

	if (GAME_STATE != 4) {
		return;
	}

	k = id;
	n = (COMBAT_DATA_PTR->fighter[id].finisherProgress * 6) / COMBAT_DATA_PTR->fighter[id].finisherGoal;

	if (MAIN_D_801351A0[k] != n) {
		MAIN_D_8013519C[k] = 0;
		MAIN_D_801351A0[k] = n;
	}

	MAIN_D_8013519E[k] = STD_D_8007A9A4[MAIN_D_8013519C[k]];
	if (MAIN_D_8013519C[k] < 0xb) {
		MAIN_D_8013519C[k]++;
	}

	if (n == 6) {
		if (MAIN_D_8013519A[k] < 0xa) {
			MAIN_D_8013519A[k]++;
		}
		v = MAIN_D_8013519A[k];
		v2 = v;
		if (v >= 3) {
			n++;
		}
		if (v2 >= 5) {
			n++;
			STD_func_8006A824(id);
			if (MAIN_D_8013519A[k] == 0xa) {
				MAIN_D_801351A2[k] = 1;
				MAIN_D_8013519C[k] = (uint32_t)MAIN_D_8013519C[k] % 0xb;
			}
		}
	}

	for (i = 0; i < n; i++) {
		STD_renderHPBarDigits((int16_t)i, id);
	}
}

void STD_func_8006A824(int32_t id)
{
	GsBOXF box;
	POLY_FT4 *prim;
	uint32_t n;

	prim = (POLY_FT4 *)GsGetWorkBase();
	setEntityTextDigit(prim, 256, 0x1e2);
	n = id;
	if ((MAIN_D_801351A2[id] != 1) || (((uint8_t *)COMBAT_DATA_PTR + n)[0x64e] == 0xb)) {
		prim->r0 = 0x80;
		prim->g0 = 0x80;
		prim->b0 = 0x80;
	} else {
		prim->r0 = MAIN_D_8013519E[n];
		prim->g0 = MAIN_D_8013519E[n];
		prim->b0 = MAIN_D_8013519E[n];
	}

	prim->u0 = 0x80;
	prim->v0 = 0x88;
	prim->u1 = 0xa5;
	prim->v1 = 0x88;
	prim->u2 = 0x80;
	prim->v2 = 0x91;
	prim->u3 = 0xa5;
	prim->v3 = 0x91;
	prim->x0 = (id == 0 ? -0x56 : 0x6e);
	prim->y0 = -0x4b;
	prim->x1 = (id == 0 ? -0x56 : 0x6e) + 0x25;
	prim->y1 = -0x4b;
	prim->x2 = (id == 0 ? -0x56 : 0x6e);
	prim->y2 = -0x42;
	prim->x3 = (id == 0 ? -0x56 : 0x6e) + 0x25;
	prim->y3 = -0x42;
	AddPrim(ACTIVE_ORDERING_TABLE->org + 8, prim++);
	GsSetWorkBase((PACKET *)prim);

	box.attribute = 0x40000000;
	if ((MAIN_D_801351A2[n] != 1) || (((uint8_t *)COMBAT_DATA_PTR + n)[0x64e] == 0xb)) {
		box.r = box.g = box.b = 0x80;
	} else {
		box.r = box.g = box.b = MAIN_D_8013519E[n];
	}

	box.w = 0x29;
	box.h = 0xb;
	box.x = -0x58;
	box.y = -0x4c;
	GsSortBoxFill(&box, ACTIVE_ORDERING_TABLE, 8);
}

void STD_renderHPBarDigits(int16_t i, int32_t id)
{
	POLY_FT4 *prim;
	int32_t n;
	uint32_t k;

	k = id;
	n = (COMBAT_DATA_PTR->fighter[id].finisherProgress * 6) / COMBAT_DATA_PTR->fighter[id].finisherGoal;
	prim = (POLY_FT4 *)GsGetWorkBase();
	setEntityTextDigit(prim, 0x100, 0x1ec);

	if ((((n - 1) == i) || (MAIN_D_801351A2[k] == 1)) && (((uint8_t *)COMBAT_DATA_PTR + k)[0x64e] != 0xb)) {
		prim->r0 = MAIN_D_8013519E[k];
		prim->g0 = MAIN_D_8013519E[k];
		prim->b0 = MAIN_D_8013519E[k];
	} else {
		prim->r0 = 0x80;
		prim->g0 = 0x80;
		prim->b0 = 0x80;
	}

	prim->u0 = MAIN_D_801348C0[i];
	prim->v0 = 0x9d;
	prim->u1 = MAIN_D_801348C0[i] + MAIN_D_801348C8[i];
	prim->v1 = 0x9d;
	prim->u2 = MAIN_D_801348C0[i];
	prim->v2 = 0xac;
	prim->u3 = MAIN_D_801348C0[i] + MAIN_D_801348C8[i];
	prim->v3 = 0xac;
	prim->x0 = (id == 0 ? MAIN_D_801348D0[i] - 0x8d : MAIN_D_801348D0[i] + 0x37);
	prim->y0 = -0x4f;
	prim->x1 = MAIN_D_801348C8[i] + ((id == 0 ? MAIN_D_801348D0[i] - 0x8d : MAIN_D_801348D0[i] + 0x37));
	prim->y1 = -0x4f;
	prim->x2 = (id == 0 ? MAIN_D_801348D0[i] - 0x8d : MAIN_D_801348D0[i] + 0x37);
	prim->y2 = -0x40;
	prim->x3 = MAIN_D_801348C8[i] + ((id == 0 ? MAIN_D_801348D0[i] - 0x8d : MAIN_D_801348D0[i] + 0x37));
	prim->y3 = -0x40;
	AddPrim(ACTIVE_ORDERING_TABLE->org + 8, prim++);
	GsSetWorkBase((PACKET *)prim);
}

void STD_func_8006AD00(int32_t id)
{
	uint32_t n;

	MAIN_D_80135198[id] = 0;
	n = id;
	MAIN_D_8013519A[n] = 0;
	MAIN_D_8013519C[n] = 0xb;
	MAIN_D_8013519E[n] = 0;
	MAIN_D_801351A0[n] = 0;
	MAIN_D_801351A2[n] = 0;
	addObject(0x19c, id, NULL, STD_func_8006AD68);
}

INCLUDE_ASM("asm/std/nonmatchings/std_hud", STD_func_8006AD68);

void STD_func_8006B1E4(int32_t i)
{
	removeObject(0x19c, i);
}

INCLUDE_ASM("asm/std/nonmatchings/std_hud", STD_func_8006B1F0);

void STD_func_8006B2BC(void)
{
	int32_t i;

	MAIN_D_801351A4 = 0;
	MAIN_D_801351A8 = (uint8_t *)0x80038000;
	readFile(MAIN_D_801348D8, MAIN_D_801351A8);
	GsMapModelingData((u_long *)(MAIN_D_801351A8 + 4));

	for (i = 0; i < 4; i++) {
		GsLinkObject4((u_long)(MAIN_D_801351A8 + 0xc), &STD_D_8007C7B0[i].obj, i);
		GsInitCoordinate2(NULL, &STD_D_8007C7B0[i].posMatrix);
		STD_D_8007C7B0[i].obj.attribute = 0;
		STD_D_8007C7B0[i].obj.coord2 = &STD_D_8007C7B0[i].posMatrix;
	}

	for (i = 0; i < 4; i++) {
		STD_D_8007C7B0[i].scale.vx = 0x1000;
		STD_D_8007C7B0[i].scale.vy = 0x1000;
		STD_D_8007C7B0[i].scale.vz = 0x1000;
		STD_D_8007C7B0[i].rotation.vx = 0;
		STD_D_8007C7B0[i].rotation.vy = 0;
		STD_D_8007C7B0[i].rotation.vz = 0;
		STD_D_8007C7B0[i].location.vx = 0x3e8;
		STD_D_8007C7B0[i].location.vy = 0x78;
		STD_D_8007C7B0[i].location.vz = 0x280;
		setupModelMatrix(&STD_D_8007C7B0[i]);
	}
}

void STD_func_8006B468(void)
{
	addObject(0x19D, 0, STD_tickVersusModelScene, STD_renderVersusModelScene);
}

void STD_tickVersusModelScene(void)
{
	int32_t i;

	MAIN_D_801351A4++;
	for (i = 0; i < 4; i++) {
		if (i * 5 < MAIN_D_801351A4) {
			if (STD_D_8007C7B0[i].location.vx != MAIN_D_801348DC[i]) {
				STD_D_8007C7B0[i].location.vx -= 200;
				if (STD_D_8007C7B0[i].location.vx < MAIN_D_801348DC[i]) {
					STD_D_8007C7B0[i].location.vx = MAIN_D_801348DC[i];
				}
			}
		}
		setupModelMatrix(&STD_D_8007C7B0[i]);
	}
}
void STD_renderVersusModelScene(void)
{
	MATRIX lw;
	MATRIX ls;
	int32_t i;

	GsSetProjection(0x200);
	GsWSMATRIX = *(MATRIX *)STD_D_8007AA10;
	GsClearOt(0, 4, &STD_D_8007B714[ACTIVE_FRAMEBUFFER]);

	for (i = 0; i < 4; i++) {
		GsGetLws(STD_D_8007C7B0[i].obj.coord2, &lw, &ls);
		GsSetLightMatrix(&lw);
		GsSetLsMatrix(&ls);
		GsSortObject4(&STD_D_8007C7B0[i].obj, &STD_D_8007B714[ACTIVE_FRAMEBUFFER], 9, getScratchAddr(0));
	}

	GsSortOt(&STD_D_8007B714[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

int32_t STD_isVersusModelSceneFinished(void)
{
	if (MAIN_D_801351A4 >= 0x51) {
		return 1;
	} else {
		return 0;
	}
}

void STD_func_8006B6E8(void)
{
	removeObject(0x19d, 0);
}

INCLUDE_ASM("asm/std/nonmatchings/std_hud", STD_func_8006B6F4);

INCLUDE_ASM("asm/std/nonmatchings/std_hud", STD_func_8006BA18);

void STD_func_8006BE64(void)
{
	RECT rect;
	int32_t i;

	MAIN_D_801351B4 = 0;
	GAME_STATE = 5;
	startAnimation(ENTITY_TABLE[1], 0x21);
	ENTITY_TABLE[1]->posData->location.vx = 0;
	ENTITY_TABLE[1]->posData->location.vy = 0;
	ENTITY_TABLE[1]->posData->location.vz = 0;
	ENTITY_TABLE[1]->posData->location.vy = -0x1e;
	ENTITY_TABLE[1]->anim.locY = -0xf0000;
	ENTITY_TABLE[1]->anim.locX = 0;
	ENTITY_TABLE[1]->anim.locZ = 0;
	ENTITY_TABLE[1]->posData->rotation.vy = 0x200;
	STD_func_8005A830();
	rect.x = 0x378;
	rect.y = 0;
	rect.w = 6;
	MAIN_D_801351A4 = 0;
	rect.h = 0x30;

	for (i = 0; i < 9; i++) {
		STD_D_8007FA08[i] = 0;
		MoveImage(&rect, (i * 6) + 0x348, 0x30);
	}
}
