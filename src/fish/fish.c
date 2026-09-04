#include <stdlib.h>

#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>
#include <mwinline_n.h>

#include <dw/types.h>
#include <dw/entity.h>
#include <dw/file.h>
#include <dw/fish.h>
#include <dw/item.h>
#include <dw/clock.h>
#include <dw/params.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/ui.h>

#include "common.h"

#define FISH_SCRATCH ((FishScratch *)getScratchAddr(0))
#define FISH_BAIT_CHANCES ((FishBaitChance *)FISH_D_8007A1F4)
#define FISH_SPRITES ((FishSprite *)&FISH_D_8007A1F4[0xC])

#pragma optimize_for_size on

typedef struct {
	int16_t v[4];
} FishHist;

typedef struct {
	uint8_t item;
	uint8_t threshold;
} FishBaitChance;

typedef struct {
	uint16_t tpage;
	uint16_t clut;
	uint8_t u0;
	uint8_t v0;
	uint8_t u1;
	uint8_t v1;
	uint8_t ox;
	uint8_t oy;
} FishSprite;

typedef struct {
	VECTOR pos;
	int32_t state;
	int32_t timer;
	int32_t msg;
	int32_t mode;
	VECTOR vel;
	int32_t dist;
	int32_t unk34;
	int32_t unk38;
	int32_t gravity;
	int32_t unk40;
} FishingSwimmer;

typedef struct {
	uint32_t unk0[3];
	int32_t sz;
	uint32_t flag;
	int32_t otz;
	int32_t unk18;
	uint32_t unk1C[3];
	int32_t x;
	int32_t y;
	uint32_t unk30[4];
	uint32_t unk40;
	SVECTOR rot;
	uint32_t unk4C[8];
	SVECTOR cur;
	SVECTOR prev;
	SVECTOR pts[4];
	VECTOR v0;
	uint32_t unkAC[4];
	VECTOR v1;
	uint32_t unkCC[4];
	MATRIX m[4];
	u_long ot[1];
} FishScratch;

typedef struct {
	int32_t unk0;
	int32_t lo;
	int32_t hi;
	int32_t unkC;
	int32_t unk10;
	int16_t bait;
	int16_t unk16;
} FishKindInfo;

extern SVECTOR MAIN_D_8013497C;
extern int8_t MAIN_D_8013499C[8];
extern int32_t FISH_D_8007A4D8[];
extern int32_t FISH_D_8007A4E4[];
extern int32_t FISH_D_8007A4F8[];
extern int32_t FISH_D_8007A540[];
extern int32_t FISH_D_8007A554[];
extern SVECTOR MAIN_D_801349D0;
extern int16_t MAIN_D_801349D4;
extern int32_t MAIN_D_801349D8;
extern int16_t MAIN_D_80154F80[];
extern int32_t FISH_D_8007AA18[];
extern int32_t FISH_D_8007AA20[];
extern char FISH_D_8007A5F4[];
extern char FISH_D_8007A608[];
extern char FISH_D_8007A614[];
extern char FISH_D_8007A628[];
extern char FISH_D_8007A634[];
extern char FISH_D_8007A650[];
extern char FISH_D_8007A670[];
extern char FISH_D_8007A688[];
extern char FISH_D_8007A69C[];
extern char FISH_D_8007A6A8[];
extern char FISH_D_8007A6C4[];
extern char FISH_D_8007A6D8[];
extern char FISH_D_8007A6F8[];
extern char FISH_D_8007A70C[];
extern char FISH_D_8007A720[];
extern char FISH_D_8007A73C[];
extern char FISH_D_8007A758[];
extern char FISH_D_8007A774[];
extern char FISH_D_8007A784[];
extern int16_t MAIN_D_80134984;
extern int16_t MAIN_D_80134986;
extern int16_t MAIN_D_80134988;
extern int16_t MAIN_D_801351DC[3];
extern int16_t MAIN_D_801351DE;
extern int16_t MAIN_D_801351E0;
extern int16_t DRAW_OFFSET_LIMIT_Y_MIN;
extern int16_t DRAW_OFFSET_LIMIT_Y_MAX;
extern int16_t DRAW_OFFSET_LIMIT_X_MIN;
extern int16_t DRAW_OFFSET_LIMIT_X_MAX;
extern char FISH_D_8007A4C8[];
extern int8_t FISH_D_8007A1F4[];
extern FishKindInfo FISH_D_8007AA28[];
extern MATRIX FISH_D_8007AB68[];
extern DVECTOR FISH_D_8007A890[];
extern FishHist FISH_D_8007AB40[];
extern SVECTOR FISH_D_8007A56C[];
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
extern uint32_t POLLED_INPUT;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern int32_t MAIN_D_801351D8;
extern uint8_t MAIN_D_801349A4[8];
extern uint8_t MAIN_D_801349AC[4];
extern uint8_t MAIN_D_801349B0[8];
extern uint8_t MAIN_D_801349B8[8];
extern int8_t PARTNER_STATE;
extern uint8_t TEXTBOX_OPEN_TIMER;
extern int32_t VIEWPORT_DISTANCE;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern SVECTOR MAIN_D_8013498C;
extern int16_t MAIN_D_80134994;
extern int16_t MAIN_D_80134996;
extern int16_t MAIN_D_80134998;
extern int8_t MAIN_D_80134958[8];
extern int8_t MAIN_D_80134960[8];
extern int8_t MAIN_D_80134968[4];
extern int8_t MAIN_D_8013496C[4];
extern int8_t FISH_D_8007A4B4[];
extern int16_t MAIN_D_80134974[4];
extern FishSprite FISH_D_8007A340[];
extern uint8_t MAIN_D_80127BDC[];
extern int8_t FISH_D_8007A3EC[];
extern SVECTOR FISH_D_8007A5BC[];
extern SVECTOR FISH_D_8007A594[];
extern VECTOR FISH_D_8007AC08[];
extern SVECTOR MAIN_D_801349C0;
extern SVECTOR MAIN_D_801349C8;
extern DVECTOR FISH_D_8007A9B0[];
extern char FISH_D_8007A5E4[];
extern int16_t CAMERA_X[];
extern int16_t CAMERA_Y[];

int32_t getDistance(int32_t x, int32_t y, int32_t z);
int32_t customRandom(int32_t a, int32_t b);
int VSync(int mode);
void renderSelectionCursor(int32_t a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4);
void MAIN_func_800E58A4(int32_t a, int32_t b, int32_t x, int32_t y, int32_t n, int32_t layer);
void renderLinePrimitive(uint32_t color, int32_t x0, int32_t y0, int32_t x1,
                         int32_t y1, int32_t layer, int32_t semiTrans);
void setCameraFollowPlayer(void);
void unsetCameraFollowPlayer(void);
void startAnimation(Entity *entity, int32_t animId);
void toEulerAngles(SVECTOR *out, int32_t x, int32_t y, int32_t z);
void rotateVectorYXZ(SVECTOR *rotation, VECTOR *input, VECTOR *output);
MATRIX *RotMatrixYXZ(SVECTOR *r, MATRIX *m);
VECTOR *ApplyMatrix(MATRIX *m, SVECTOR *v0, VECTOR *v1);
void MAIN_func_800E4038(VECTOR *out, int32_t x, int32_t z, int32_t *f);
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
void renderUIBox(int32_t id);
void drawString(char *text, int32_t color, int32_t pos);
void clearTextSubArea(RECT *area);
void clearTextArea(void);
void renderItemSprite(int32_t itemId, int32_t x, int32_t y, int32_t depth);
void calculatePosition(GsCOORDINATE2 *coord, MATRIX *matrix);
void matrixToEuler2(MATRIX *m, SVECTOR *out);
void exit(int32_t code);
int sprintf(char *s, char *fmt, ...);
VECTOR *ApplyTransposeMatrixLV(MATRIX *m, VECTOR *v0, VECTOR *v1);
VECTOR *ApplyMatrixLV(MATRIX *m, VECTOR *v0, VECTOR *v1);

int32_t FISH_panCameraTo(int32_t radius, VECTOR *pos, int32_t maxDX, int32_t maxDY);
int32_t FISH_panCameraToTamer(int32_t a);
int32_t FISH_panCameraWithDPad(void);
int32_t FISH_isScreenPointVisible(int32_t x, int32_t y);
int32_t FISH_projectPoint(VECTOR *p, int32_t *outX, int32_t *outY);
int32_t FISH_isWithinCastRange(VECTOR *p, int32_t extra);
int32_t FISH_isBaitMenuVisible(void);
void FISH_closeBaitMenu(void);
void FISH_tickBaitMenu(void);
void FISH_drawBaitMenuRow(int32_t row);
void FISH_renderBaitMenu(int32_t boxId);
int32_t FISH_openBaitMenu(void);
void FISH_initBaitMenu(void);
void FISH_closeBaitMenuIfOpen(void);
void FISH_scrollViewToTarget(int32_t a);
int32_t FISH_isTextBoxVisible(void);
void FISH_closeTextBox(void);
void FISH_tickTextBox(void);
void FISH_renderTextBox(int32_t boxId);
int32_t FISH_showTextBox(uint32_t l0, uint32_t l1, uint32_t l2, uint32_t l3, VECTOR *v);
void FISH_initTextBox(void);
void FISH_shutdownTextBox(void);
void FISH_randomPointInSpot(int32_t index, VECTOR *out);
void FISH_drawStub(void);
void FISH_drawSprite2D(int32_t x, int32_t y, int32_t order, uint8_t *desc, int32_t sx, int32_t sy, int32_t rot, int32_t color, int32_t flags);
void FISH_drawSprite3D(VECTOR *pos, SVECTOR *rot, VECTOR *scale, uint8_t *desc, int32_t color, int32_t flags, int32_t depth);
void FISH_freeMarker(FishMarker *s);
void FISH_tickMarker(FishMarker *s);
void FISH_drawMarker(FishMarker *s);
void FISH_initMarkerPool(void);
FishMarker *FISH_allocMarker(void);
void FISH_spawnMarker(int32_t a, VECTOR *v);
void FISH_tickMarkers(void);
void FISH_drawMarkers(void);
void FISH_clearMarkers(void);
void FISH_freeRipple(FishRipple *s);
void FISH_tickRipple(FishRipple *s);
void FISH_drawRipple(FishRipple *s);
void FISH_initRipplePool(void);
FishRipple *FISH_allocRipple(void);
void FISH_spawnRipple(int32_t a, VECTOR *v, int32_t c, int32_t spread, int32_t e);
void FISH_tickRipples(void);
void FISH_drawRipples(void);
void FISH_loadSpritesAndSeadramon(TMDFileLoadingData *m);
void FISH_onRodModelLoaded(TMDFileLoadingData *m);
void FISH_resetTension(void);
void FISH_beginTension(void);
int32_t FISH_updateTension(void);
void FISH_drawTensionGauge(void);
void FISH_hideBaitSprite(void);
void FISH_initBaitSprite(void);
void FISH_clearBaitSprite(void);
void FISH_showBaitSprite(VECTOR *pos, int32_t sprite);
int32_t FISH_moveBaitToRodTip(int32_t maxStep);
void FISH_tickBaitSprite(void);
void FISH_drawBaitSprite(void);
void FISH_resetBobberAnim(FishRipple *s);
void FISH_initBobberAnim(FishRipple *s);
void FISH_clearBobberAnim(FishRipple *s);
void FISH_getRodTipPos(VECTOR *out);
void FISH_bobberCommand(VECTOR *v, int32_t n);
void FISH_tickBobberPhysics(void);
void FISH_pushSeadramonSegment(VECTOR *unused, FishHist *src);
void FISH_drawSeadramon(VECTOR *start, SVECTOR *rot);
void FISH_spawnSeadramon(Fish *f);
void FISH_tickSeadramon(Fish *f);
void FISH_spawnFish(Fish *f, int32_t kind);
void FISH_tickFish(Fish *f);
void FISH_drawFishSprite(int32_t kind, VECTOR *pos, SVECTOR *rot, int32_t size, int32_t phase, int32_t transparency, int32_t flag);
void FISH_drawFish(Fish *f);
Fish *FISH_allocFish(void);
int32_t FISH_isSeadramonAlive(void);
void FISH_tickFishPopulation(void);
void FISH_drawFishPool(void);
void FISH_initFishPool(void);
void FISH_fishPoolStub(void);
Fish *FISH_findFishToBite(Fish *pool, VECTOR *loc, int32_t depth);
void FISH_clearHookedFish(void);
void FISH_hookFish(Fish *f);
void FISH_tickHookedFish(int32_t mode);
int32_t FISH_tryStrike(void);
void FISH_drawHookedFish(void);
void FISH_updateRodPose(FishingRod *rod);
void FISH_drawRodSegments(FishingRod *rod);
int32_t FISH_clipLineToRect(SVECTOR *pts, int32_t xMin, int32_t xMax, int32_t yMin, int32_t yMax);
void FISH_drawFishingLine(FishingRod *rod);
void FISH_resetRodState(FishingRod *rod);
void FISH_tickRod(FishingRod *rod);
void FISH_tickFishingWorld(int32_t instanceId);
void FISH_renderFishingWorld(int32_t instanceId);
int32_t FISH_lerpTableByX(int32_t x, int32_t base, DVECTOR *tbl, int32_t n);
int32_t FISH_lerpTableByY(int32_t base, int32_t y, DVECTOR *tbl, int32_t n);
int32_t FISH_isFishableWater1(int32_t a, int32_t y);
void FISH_waterTestStub1(void);
int32_t FISH_isFishableWater2(int32_t x, int32_t y);
void FISH_waterTestStub2(void);
void FISH_getBoneTransform(Entity *e, int32_t bone, VECTOR *outPos, SVECTOR *outRot);
void FISH_drawItemSprite(int32_t itemId, int32_t x, int32_t y, int32_t depth, int32_t e);
void FISH_endFishing(FishingRod *rod);
void FISH_scrollCameraTo(int32_t x, int32_t y);
void FISH_applySavedCameraScroll(void);
void FISH_enableCameraControl(void);
void FISH_unloadModel(TMDFileLoadingData *m);
void FISH_loadTmdModel(TMDFileLoadingData *m, char *path, uint8_t *buf, uint32_t bufSize);
GsDOBJ2 *FISH_getModelObject(TMDFileLoadingData *m, int32_t idx);
int32_t FISH_remapClamped(int32_t x, int32_t x0, int32_t x1, int32_t y0, int32_t y1);
void handleTileUpdate(uint32_t mask, int32_t flag);

static void *fish_functions[] = {
	FISH_remapClamped,
	FISH_getModelObject,
	FISH_loadTmdModel,
	FISH_unloadModel,
	FISH_enableCameraControl,
	FISH_applySavedCameraScroll,
	FISH_scrollCameraTo,
	FISH_tamerTick,
	FISH_deinitialize,
	FISH_init,
	FISH_loadFishing,
	FISH_endFishing,
	FISH_drawItemSprite,
	FISH_getBoneTransform,
	FISH_waterTestStub2,
	FISH_isFishableWater2,
	FISH_waterTestStub1,
	FISH_isFishableWater1,
	FISH_lerpTableByY,
	FISH_lerpTableByX,
	FISH_renderFishingWorld,
	FISH_tickFishingWorld,
	FISH_tickRod,
	FISH_resetRodState,
	FISH_drawFishingLine,
	FISH_clipLineToRect,
	FISH_drawRodSegments,
	FISH_updateRodPose,
	FISH_drawHookedFish,
	FISH_tryStrike,
	FISH_tickHookedFish,
	FISH_hookFish,
	FISH_clearHookedFish,
	FISH_findFishToBite,
	FISH_fishPoolStub,
	FISH_initFishPool,
	FISH_drawFishPool,
	FISH_tickFishPopulation,
	FISH_isSeadramonAlive,
	FISH_allocFish,
	FISH_drawFish,
	FISH_drawFishSprite,
	FISH_tickFish,
	FISH_spawnFish,
	FISH_tickSeadramon,
	FISH_spawnSeadramon,
	FISH_drawSeadramon,
	FISH_pushSeadramonSegment,
	FISH_tickBobberPhysics,
	FISH_bobberCommand,
	FISH_getRodTipPos,
	FISH_clearBobberAnim,
	FISH_initBobberAnim,
	FISH_resetBobberAnim,
	FISH_drawBaitSprite,
	FISH_tickBaitSprite,
	FISH_moveBaitToRodTip,
	FISH_showBaitSprite,
	FISH_clearBaitSprite,
	FISH_initBaitSprite,
	FISH_hideBaitSprite,
	FISH_drawTensionGauge,
	FISH_updateTension,
	FISH_beginTension,
	FISH_resetTension,
	FISH_onRodModelLoaded,
	FISH_loadSpritesAndSeadramon,
	FISH_drawRipples,
	FISH_tickRipples,
	FISH_spawnRipple,
	FISH_allocRipple,
	FISH_initRipplePool,
	FISH_drawRipple,
	FISH_tickRipple,
	FISH_freeRipple,
	FISH_clearMarkers,
	FISH_drawMarkers,
	FISH_tickMarkers,
	FISH_spawnMarker,
	FISH_allocMarker,
	FISH_initMarkerPool,
	FISH_drawMarker,
	FISH_tickMarker,
	FISH_freeMarker,
	FISH_drawSprite3D,
	FISH_drawSprite2D,
	FISH_drawStub,
	FISH_randomPointInSpot,
	FISH_shutdownTextBox,
	FISH_initTextBox,
	FISH_showTextBox,
	FISH_renderTextBox,
	FISH_tickTextBox,
	FISH_closeTextBox,
	FISH_isTextBoxVisible,
	FISH_scrollViewToTarget,
	FISH_closeBaitMenuIfOpen,
	FISH_initBaitMenu,
	FISH_openBaitMenu,
	FISH_renderBaitMenu,
	FISH_drawBaitMenuRow,
	FISH_tickBaitMenu,
	FISH_closeBaitMenu,
	FISH_isBaitMenuVisible,
	FISH_isWithinCastRange,
	FISH_projectPoint,
	FISH_isScreenPointVisible,
	FISH_panCameraWithDPad,
	FISH_panCameraToTamer,
	FISH_panCameraTo,
};

int32_t FISH_panCameraTo(int32_t radius, VECTOR *pos, int32_t maxDX, int32_t maxDY)
{
	SVECTOR v;
	int32_t cx;
	int32_t cy;
	int32_t dx;
	int32_t dy;
	int32_t ang;
	int32_t ox;

	SetRotMatrix(&GsWSMATRIX);
	SetTransMatrix(&GsWSMATRIX);
	v.vx = pos->vx;
	v.vy = pos->vy;
	v.vz = pos->vz;
	gte_ldv0(&v);
	gte_rtps();
	gte_stsxy(&v);

	cx = -DRAWING_OFFSET_X + 0xA0;
	dx = v.vx - cx;
	if (maxDX >= ((dx > 0) ? dx : -dx)) {
		v.vx = -DRAWING_OFFSET_X + 0xA0;
	} else {
		cx = -DRAWING_OFFSET_X + 0xA0;
		maxDX = (cx < v.vx) ? -maxDX : maxDX;
		v.vx += (int16_t)maxDX;
	}

	cy = -DRAWING_OFFSET_Y + 0x78;
	dy = v.vy - cy;
	if (maxDY >= ((dy > 0) ? dy : -dy)) {
		v.vy = -DRAWING_OFFSET_Y + 0x78;
	} else {
		maxDY = ((-DRAWING_OFFSET_Y + 0x78) < v.vy) ? -maxDY : maxDY;
		v.vy += (int16_t)maxDY;
	}

	v.vx = (v.vx < -DRAW_OFFSET_LIMIT_X_MAX + 0xA0)
	               ? -DRAW_OFFSET_LIMIT_X_MAX + 0xA0
	               : ((-DRAW_OFFSET_LIMIT_X_MIN + 0xA0 < v.vx) ? -DRAW_OFFSET_LIMIT_X_MIN + 0xA0 : v.vx);
	v.vy = (v.vy < -DRAW_OFFSET_LIMIT_Y_MAX + 0x78)
	               ? -DRAW_OFFSET_LIMIT_Y_MAX + 0x78
	               : ((-DRAW_OFFSET_LIMIT_Y_MIN + 0x78 < v.vy) ? -DRAW_OFFSET_LIMIT_Y_MIN + 0x78 : v.vy);

	if ((v.vx - (-DRAWING_OFFSET_X + 0xA0) == 0) && (v.vy - (-DRAWING_OFFSET_Y + 0x78) == 0)) {
		return 0;
	}

	ox = v.vx;

	if (radius > 0) {
		if (getDistance(ox - (-DRAWING_OFFSET_X + 0xA0),
		                v.vy - (-DRAWING_OFFSET_Y + 0x78), 0) >= radius) {
			ang = ratan2(v.vy - (-DRAWING_OFFSET_Y + 0x78),
			             v.vx - (-DRAWING_OFFSET_X + 0xA0));
			v.vx = (-DRAWING_OFFSET_X + 0xA0) + ((radius * rcos(ang)) >> 12);
			v.vy = (-DRAWING_OFFSET_Y + 0x78) + ((radius * rsin(ang)) >> 12);
		}
	}

	FISH_scrollCameraTo(v.vx, v.vy);

	return 1;
}

int32_t FISH_panCameraToTamer(int32_t a)
{
	return FISH_panCameraTo(a, &TAMER_ENTITY.entity.posData->location, 0, 0);
}

int32_t FISH_panCameraWithDPad(void)
{
	int32_t cx;
	int32_t ylo;
	int32_t cy;
	int16_t x;
	int16_t y;
	int32_t d;

	x = -DRAWING_OFFSET_X + 0xA0;
	cx = -DRAWING_OFFSET_X + 0xA0;
	y = -DRAWING_OFFSET_Y + 0x78;
	cy = -DRAWING_OFFSET_Y + 0x78;

	if ((POLLED_INPUT & 0x1000) != 0) {
		y = y - 5;
	}

	if ((POLLED_INPUT & 0x4000) != 0) {
		y = y + 5;
	}

	if ((POLLED_INPUT & 0x8000) != 0) {
		x = x - 5;
	}

	if ((POLLED_INPUT & 0x2000) != 0) {
		x = x + 5;
	}

	x = (x < -DRAW_OFFSET_LIMIT_X_MAX + 0xA0)
	            ? -DRAW_OFFSET_LIMIT_X_MAX + 0xA0
	            : ((-DRAW_OFFSET_LIMIT_X_MIN + 0xA0 < x) ? -DRAW_OFFSET_LIMIT_X_MIN + 0xA0 : x);
	ylo = -DRAW_OFFSET_LIMIT_Y_MAX;
	y = (y < ylo + 0x78)
	            ? ylo + 0x78
	            : ((-DRAW_OFFSET_LIMIT_Y_MIN + 0x78 < y) ? -DRAW_OFFSET_LIMIT_Y_MIN + 0x78 : y);

	cx = x - cx;
	d = cx;
	d = (d > 0) ? d : -d;
	if (d > 0) {
		goto move;
	}

	d = y - cy;
	d = (d > 0) ? d : -d;
	if (d > 0) {
		goto move;
	}

	return 0;
move:
	FISH_scrollCameraTo(x, y);

	return 1;
}

int32_t FISH_isScreenPointVisible(int32_t x, int32_t y)
{
	int32_t ret;
	int32_t okY;
	int32_t okX;
	int32_t origin;

	origin = 0;
	ret = 0;
	okY = 0;
	okX = 0;

	if (!(x < (-DRAW_OFFSET_LIMIT_X_MAX + origin))) {
		if (!((-DRAW_OFFSET_LIMIT_X_MIN + 0x140) < x)) {
			okX = 1;
		}
	}

	if (okX != 0) {
		if (!(y < (-DRAW_OFFSET_LIMIT_Y_MAX + origin))) {
			okY = 1;
		}
	}

	if (okY != 0) {
		if (!((-DRAW_OFFSET_LIMIT_Y_MIN + 0xF0) < y)) {
			ret = 1;
		}
	}

	return ret;
}

int32_t FISH_projectPoint(VECTOR *p, int32_t *outX, int32_t *outY)
{
	MATRIX *in = &FISH_SCRATCH->m[1];
	MATRIX *out = &FISH_SCRATCH->m[0];

	in->t[0] = p->vx;
	in->t[1] = p->vy;
	in->t[2] = p->vz;

	GsMulCoord0(&GsWSMATRIX, in, out);

	if ((out->t[2] <= 0) || (out->t[2] >= 0x10000)) {
		return 0;
	}

	if (outX != NULL) {
		*outX = out->t[0] * VIEWPORT_DISTANCE / out->t[2];
	}

	if (outY != NULL) {
		*outY = out->t[1] * VIEWPORT_DISTANCE / out->t[2];
	}

	return 1;
}

int32_t FISH_isWithinCastRange(VECTOR *p, int32_t extra)
{
	int32_t dist;

	dist = getDistance(p->vx - TAMER_ENTITY.entity.posData->location.vx, 0,
	                   p->vz - TAMER_ENTITY.entity.posData->location.vz);

	return !((FISHING_DATA_PTR->mapDataPtr->unk28 + extra) < dist);
}

int32_t FISH_isBaitMenuVisible(void)
{
	FishingBaitMenu *msg = &FISHING_DATA_PTR->msg;
	int32_t isOpen;
	int32_t ret;

	if (msg->boxId <= 0) {
		return 0;
	}

	ret = 1;
	isOpen = ret;
	if ((UI_BOX_DATA[msg->boxId].state != 2) && (UI_BOX_DATA[msg->boxId].state != 3)) {
		isOpen = 0;
	}

	if (isOpen == 0) {
		if (msg->shown == 0) {
			ret = 0;
		}
	}

	return ret;
}

void FISH_closeBaitMenu(void)
{
	FishingBaitMenu *msg = &FISHING_DATA_PTR->msg;

	if (msg->shown != 0) {
		msg->shown = 0;
		removeAnimatedUIBox(msg->boxId, &msg->unk58);
	}
}

void FISH_tickBaitMenu(void)
{
	FishingBaitMenu *m = &FISHING_DATA_PTR->msg;
	int32_t v;
	int32_t n;
	int32_t w;

	if (POLLED_INPUT == POLLED_INPUT_PREVIOUS) {
		MAIN_D_801351D8 = MAIN_D_801351D8 + 1;
	} else {
		MAIN_D_801351D8 = 0;
	}

	if (m->shown == 0) {
		return;
	}

	m->unk50 = -1;

	if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x1000) != 0) ||
	    ((MAIN_D_801351D8 >= 0xB) && ((POLLED_INPUT & 0x1000) != 0))) {
		playSound(0, 2);
		v = m->unkC;
		v = v - 1;
		m->unkC = v;
		if (v < 0) {
			m->unkC = 0;
		}
		if (m->unkC < m->unk10) {
			m->unk10 = m->unkC;
			m->unk50 = m->unk10;
		}
	}

	if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x4000) != 0) ||
	    ((MAIN_D_801351D8 >= 0xB) && ((POLLED_INPUT & 0x4000) != 0))) {
		playSound(0, 2);
		w = m->unkC + 1;
		m->unkC = w;
		n = m->unk4;
		if (w >= n) {
			m->unkC = n - 1;
		}
		if (m->unkC >= m->unk10 + m->unk54) {
			m->unk10 = m->unkC - m->unk54 + 1;
			m->unk50 = m->unk10 + m->unk54 - 1;
		}
	}

	if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x8000) != 0) ||
	    ((MAIN_D_801351D8 >= 0xB) && ((POLLED_INPUT & 0x8000) != 0))) {
		playSound(0, 2);
		v = m->unk8;
		v = v - 1;
		m->unk8 = v;
		if (v < 0) {
			m->unk8 = 1;
		}
	}

	if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x2000) != 0) ||
	    ((MAIN_D_801351D8 >= 0xB) && ((POLLED_INPUT & 0x2000) != 0))) {
		playSound(0, 2);
		v = m->unk8;
		v = v + 1;
		m->unk8 = v;
		if (v >= 2) {
			m->unk8 = 0;
		}
	}

	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x40) != 0) {
		playSound(0, 2);
		if (m->unk14[m->unk8 + m->unkC * 2] != -1) {
			m->unk6C = m->unk14[m->unk8 + m->unkC * 2];
			FISH_closeBaitMenu();
		}
	}

	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x10) != 0) {
		playSound(0, 2);
		m->unk6C = -1;
		FISH_closeBaitMenu();
	}
}

void FISH_drawBaitMenuRow(int32_t row)
{
	FishingBaitMenu *m = &FISHING_DATA_PTR->msg;
	RECT rect;
	RECT area;
	int32_t i;
	int32_t item;

	for (i = 0; i < 2; i++) {
		item = m->unk14[row * 2 + i];
		if (item != -1) {
			item = INVENTORY.types.array[item];
			if (item != 0xFF) {
				rect.x = i * 0x60;
				rect.y = (row % m->unk54) * 12;
				rect.w = 0x60;
				rect.h = 0xC;
				area = rect;
				clearTextSubArea(&area);
				drawString((char *)&ITEM_PARA[item], rect.x, rect.y);
				DrawSync(0);
			}
		}
	}
}

void FISH_renderBaitMenu(int32_t boxId)
{
	FishingBaitMenu *m = &FISHING_DATA_PTR->msg;
	UIBoxData *b;
	POLY_FT4 *prim;
	int32_t layer;
	int32_t x;
	int32_t y;
	int32_t row;
	int32_t col;
	int32_t item;
	int32_t q;
	int32_t r;

	layer = 6 - boxId;
	b = &UI_BOX_DATA[boxId];
	if (m->shown == 0) {
		return;
	}

	y = b->finalPos.y;
	x = b->finalPos.x;

	for (row = 0; row < m->unk54; row++) {
		for (col = 0; col < 2; col++) {
			r = row + m->unk10;
			item = m->unk14[col + r * 2];
			if (item == -1) {
				continue;
			}
			q = INVENTORY.types.array[item];
			if (q == 0xFF) {
				continue;
			}
			FISH_drawItemSprite(q,
			                    x + ((col != 0) ? 0x98 : 8),
			                    y + 0xE + row * 0x12, layer, 0);
			renderString(0, x + ((col != 0) ? 0xAB : 0x1B),
			             y + 0x10 + row * 0x12, 0x60, 0xC, col * 0x60,
			             (r % m->unk54) * 12, layer, 1);

			prim = (POLY_FT4 *)GsGetWorkBase();
			prim->tpage = 0x1E;
			prim->clut = GetClut(0x100, 0x1E4);
			*(int32_t *)&prim->r0 = 0x808080;
			prim->u0 = 0x80;
			prim->v0 = 0x9C;
			prim->u1 = 0x88;
			prim->v1 = 0x9C;
			prim->u2 = 0x80;
			prim->v2 = 0xA4;
			prim->u3 = 0x88;
			prim->v3 = 0xA4;
			prim->x0 = x + ((col != 0) ? 0x10D : 0x7D);
			prim->y0 = y + 0x14 + row * 0x12;
			prim->x1 = x + ((col != 0) ? 0x10D : 0x7D) + 8;
			prim->y1 = y + 0x14 + row * 0x12;
			prim->x2 = x + ((col != 0) ? 0x10D : 0x7D);
			prim->y2 = y + 0x1C + row * 0x12;
			prim->x3 = x + ((col != 0) ? 0x10D : 0x7D) + 8;
			prim->y3 = y + 0x1C + row * 0x12;
			setPolyFT4(prim);
			addPrim(ACTIVE_ORDERING_TABLE->org + layer, prim);
			GsSetWorkBase((PACKET *)(prim + 1));

			MAIN_func_800E58A4(0, 2, x + ((col != 0) ? 0x116 : 0x86),
			                   y + 0x12 + row * 0x12,
			                   (INVENTORY.types.array + item)[0x1E], layer);
		}
	}

	renderSelectionCursor(x + 6 + m->unk8 * 144, y + 0xD + (m->unkC - m->unk10) * 18,
	                      0x8E, 0x12, layer);

	if (m->unk4 > m->unk54) {
		UI_BOX_DATA[m->boxId].rowOffset = m->unk10;
		if (m->unk50 != -1) {
			FISH_drawBaitMenuRow(m->unk50);
			m->unk50 = -1;
		}
	}
}

int32_t FISH_openBaitMenu(void)
{
	FishingBaitMenu *m = &FISHING_DATA_PTR->msg;
	DVECTOR pos;
	int32_t n;
	int32_t i;
	int32_t flag;
	int32_t features;
	int32_t k;

	m->unk50 = -1;
	m->unk8 = 0;
	m->unkC = 0;
	m->unk10 = 0;
	m->unk54 = 9;

	n = 0;
	for (i = 0; i < 30U; i++) {
		if (INVENTORY.types.array[i] == 0xFF) {
			continue;
		}
		if (ITEM_PARA[INVENTORY.types.array[i]].sortingValue != 2) {
			continue;
		}
		m->unk14[n++] = i;
		if (n >= 30U) {
			break;
		}
	}

	m->unk4 = (n + 1) / 2;
	for (; n < 30U; n++) {
		m->unk14[n] = -1;
	}

	if (m->unk4 <= 0) {
		return 0;
	}

	DrawSync(0);
	clearTextArea();
	for (k = 0; k < m->unk54; k++) {
		FISH_drawBaitMenuRow(k);
	}

	getEntityScreenPos(ENTITY_TABLE[0], 1, (int16_t *)&pos);
	m->unk58.x = pos.vx - 5;
	m->unk58.y = pos.vy - 5;
	m->unk58.w = 10;
	m->unk58.h = 10;
	m->unk60.x = -0x98;
	m->unk60.y = -0x68;
	m->unk60.w = 0x132;
	m->unk60.h = 0xB6;
	m->boxId = 3;

	flag = 0;
	if (m->unk4 > m->unk54) {
		UI_BOX_DATA[m->boxId].rowOffset = m->unk10;
		UI_BOX_DATA[m->boxId].visibleRows = m->unk54;
		UI_BOX_DATA[m->boxId].totalRows = m->unk4;
		flag = 1;
	}

	features = flag ? 6 : 2;
	createAnimatedUIBox(m->boxId, 0, features, &m->unk60, &m->unk58,
	                    (TickFunction)FISH_tickBaitMenu, (RenderFunction)FISH_renderBaitMenu);
	m->shown = 1;
}

void FISH_initBaitMenu(void)
{
	FishingBaitMenu *msg = &FISHING_DATA_PTR->msg;

	msg->unk50 = -1;
	msg->shown = 0;
	msg->boxId = -1;
	msg->unk54 = 9;
}

void FISH_closeBaitMenuIfOpen(void)
{
	if (FISHING_DATA_PTR->msg.shown != 0) {
		FISH_closeBaitMenu();
	}
}

void FISH_scrollViewToTarget(int32_t a)
{
	FishingView *view = &FISHING_DATA_PTR->view;

	if (view->isScrolling != 0) {
		FISH_panCameraTo(0x10, &view->scrollTo, 0, 0);
		view->isScrolling = 0;
	}
}

int32_t FISH_isTextBoxVisible(void)
{
	FishingBox *box = &FISHING_DATA_PTR->view.box;
	int32_t isOpen;
	int32_t ret;

	if (box->boxId <= 0) {
		return 0;
	}

	ret = 1;
	isOpen = ret;
	if ((UI_BOX_DATA[box->boxId].state != 2) && (UI_BOX_DATA[box->boxId].state != 3)) {
		isOpen = 0;
	}

	if (isOpen == 0) {
		if (box->shown == 0) {
			ret = 0;
		}
	}

	return ret;
}

void FISH_closeTextBox(void)
{
	FishingBox *box = &FISHING_DATA_PTR->view.box;

	if (box->shown != 0) {
		removeStaticUIBox(box->boxId);
	}

	box->shown = 0;
}

void FISH_tickTextBox(void)
{
	FishingView *view = &FISHING_DATA_PTR->view;

	if (view->box.shown == 0) {
		return;
	}

	view->isScrolling = 1;

	if (((view->unk3C >= 10) && ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x40) != 0)) ||
	    ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x10) != 0)) {
		POLLED_INPUT = 0;
		FISH_closeTextBox();
		playSound(0, 3);
	}

	if (view->unk3C < 0x64) {
		view->unk3C += 1;
	}

	TEXTBOX_OPEN_TIMER = (uint32_t)TEXTBOX_OPEN_TIMER + 1;
}

void FISH_renderTextBox(int32_t boxId)
{
	FishingView *v = &FISHING_DATA_PTR->view;
	UIBoxData *b;
	int32_t i;
	int32_t x;
	int32_t y;
	int32_t layer;

	layer = 6 - boxId;
	b = &UI_BOX_DATA[boxId];
	if (v->box.shown == 0) {
		return;
	}

	x = b->finalPos.x;
	y = b->finalPos.y;
	for (i = 0; i < 4U; i++) {
		if (v->unk18[i] != 0) {
			renderString(0, x + 5, y + i * 13 + 3, 0xFF, 0xC, 0, i * 12, layer, 1);
		}
	}

	renderUIBox(boxId);
}

int32_t FISH_showTextBox(uint32_t l0, uint32_t l1, uint32_t l2, uint32_t l3, VECTOR *v)
{
	FishingView *view = &FISHING_DATA_PTR->view;
	int16_t pos[2];
	int32_t i;

	FISH_closeTextBox();
	POLLED_INPUT = 0;

	view->scrollTo = *v;
	view->unk18[0] = l0;
	view->unk18[1] = l1;
	view->unk18[2] = l2;
	view->unk18[3] = l3;
	view->unk3C = 0;

	DrawSync(0);
	clearTextArea();

	for (i = 0; i < 4U; i++) {
		if (view->unk18[i] != 0) {
			DrawSync(0);
			drawString((char *)view->unk18[i], 0, i * 0xC);
			DrawSync(0);
		}
	}

	getEntityScreenPos(ENTITY_TABLE[0], 1, pos);

	view->box.startPos.x = -DRAWING_OFFSET_X + 0xA0;
	view->box.startPos.y = -DRAWING_OFFSET_Y + 0x78;
	view->box.startPos.w = 1;
	view->box.startPos.h = 1;
	view->box.finalPos.x = -0x82;
	view->box.finalPos.y = 0x2A;
	view->box.finalPos.w = 0x106;
	view->box.finalPos.h = 0x3B;

	view->box.boxId = 2;

	createStaticUIBox(view->box.boxId, 0, 2, &view->box.finalPos,
	                  (TickFunction)FISH_tickTextBox, (RenderFunction)FISH_renderTextBox);

	view->box.shown = 1;
	TEXTBOX_OPEN_TIMER = 0;
}

void FISH_initTextBox(void)
{
	FishingBox *box = &FISHING_DATA_PTR->view.box;

	box->boxId = -1;
	box->shown = 0;
}

void FISH_shutdownTextBox(void)
{
	FishingBox *box = &FISHING_DATA_PTR->view.box;

	FISH_closeTextBox();
	box->boxId = -1;
}

void FISH_randomPointInSpot(int32_t index, VECTOR *out)
{
	int32_t t;
	DVECTOR *pts[3];
	FishingSpot *spot;
	int32_t j;
	int32_t dup;
	int16_t x;
	int16_t y;
	int32_t lo;

	spot = &FISHING_DATA_PTR->mapDataPtr->fishingSpots[index];
	if ((index < 0) || (index >= 6)) {
		out->vx = 0;
		out->vy = 0;
		out->vz = 0;
		return;
	}

retry:
	for (t = 0; t < 3U; t++) {
		do {
			dup = 0;
			pts[t] = &spot->unk8[customRandom(0, spot->unkC - 1)];
			for (j = 0; j < t; j++) {
				if (pts[t] == pts[j]) {
					dup = 1;
				}
			}
		} while (dup);
	}

	lo = 0;
	for (t = 0; t < 3U; t++) {
		pts[t]->vx = (pts[t]->vx < -DRAW_OFFSET_LIMIT_X_MAX + lo)
		                     ? -DRAW_OFFSET_LIMIT_X_MAX + lo
		                     : ((-DRAW_OFFSET_LIMIT_X_MIN + 320 < pts[t]->vx) ? -DRAW_OFFSET_LIMIT_X_MIN + 320 : pts[t]->vx);
		pts[t]->vy = (pts[t]->vy < -DRAW_OFFSET_LIMIT_Y_MAX + lo)
		                     ? -DRAW_OFFSET_LIMIT_Y_MAX + lo
		                     : ((-DRAW_OFFSET_LIMIT_Y_MIN + 240 < pts[t]->vy) ? -DRAW_OFFSET_LIMIT_Y_MIN + 240 : pts[t]->vy);
	}

	t = customRandom(0, 0x1000);
	x = (pts[0]->vx * t + pts[1]->vx * (0x1000 - t)) >> 12;
	y = (pts[0]->vy * t + pts[1]->vy * (0x1000 - t)) >> 12;
	t = customRandom(0, 0x1000);
	x = (pts[2]->vx * t + x * (0x1000 - t)) >> 12;
	y = (pts[2]->vy * t + y * (0x1000 - t)) >> 12;

	if (FISH_isScreenPointVisible(x, y) == 0) {
		goto retry;
	}

	if (FISHING_DATA_PTR->mapDataPtr->unk20(x, y) == 0) {
		goto retry;
	}

	MAIN_func_800E4038(out, x, y, &t);
}

void FISH_drawStub(void)
{
}

void FISH_drawSprite2D(int32_t x, int32_t y, int32_t order, uint8_t *desc,
                       int32_t sx, int32_t sy, int32_t rot, int32_t color, int32_t flags)
{
	POLY_FT4 *prim;
	int32_t t;
	int32_t w;
	int32_t i;

	prim = (POLY_FT4 *)GsGetWorkBase();
	*(int32_t *)&prim->r0 = color;
	prim->u0 = prim->u2 = desc[4];
	prim->u1 = prim->u3 = desc[6];
	prim->v0 = prim->v1 = desc[5];
	prim->v2 = prim->v3 = desc[7];

	t = (-desc[8] * sx) >> 12;
	FISH_SCRATCH->pts[2].vx = t;
	FISH_SCRATCH->pts[0].vx = t;

	w = ((desc[6] - desc[4]) > 0) ? (desc[6] - desc[4]) : -(desc[6] - desc[4]);
	t = (sx * (-desc[8] + w)) >> 12;
	FISH_SCRATCH->pts[3].vx = t;
	FISH_SCRATCH->pts[1].vx = t;

	t = (-desc[9] * sy) >> 12;
	FISH_SCRATCH->pts[1].vy = t;
	FISH_SCRATCH->pts[0].vy = t;

	w = ((desc[7] - desc[5]) > 0) ? (desc[7] - desc[5]) : -(desc[7] - desc[5]);
	t = (sy * (-desc[9] + w)) >> 12;
	FISH_SCRATCH->pts[3].vy = t;
	FISH_SCRATCH->pts[2].vy = t;

	FISH_SCRATCH->rot.vx = 0;
	FISH_SCRATCH->rot.vy = 0;
	FISH_SCRATCH->rot.vz = rot;
	RotMatrix(&FISH_SCRATCH->rot, &FISH_SCRATCH->m[2]);

	for (i = 0; i < 4; i++) {
		ApplyMatrix(&FISH_SCRATCH->m[2], &FISH_SCRATCH->pts[i],
		            &FISH_SCRATCH->v0);
		FISH_SCRATCH->pts[i].vx = FISH_SCRATCH->v0.vx;
		FISH_SCRATCH->pts[i].vy = FISH_SCRATCH->v0.vy;
		FISH_SCRATCH->pts[i].vz = FISH_SCRATCH->v0.vz;
		FISH_SCRATCH->pts[i].vx += (int16_t)x;
		FISH_SCRATCH->pts[i].vy += (int16_t)y;
	}

	*(int32_t *)&prim->x0 = *(int32_t *)&FISH_SCRATCH->pts[0];
	*(int32_t *)&prim->x1 = *(int32_t *)&FISH_SCRATCH->pts[1];
	*(int32_t *)&prim->x2 = *(int32_t *)&FISH_SCRATCH->pts[2];
	*(int32_t *)&prim->x3 = *(int32_t *)&FISH_SCRATCH->pts[3];
	prim->tpage = *(uint16_t *)desc | ((flags & 3) << 5);
	prim->clut = *(uint16_t *)(desc + 2);
	setPolyFT4(prim);
	setSemiTrans(prim, flags >> 2);
	addPrim(ACTIVE_ORDERING_TABLE->org + order, prim);
	GsSetWorkBase((PACKET *)(prim + 1));
}

void FISH_drawSprite3D(VECTOR *pos, SVECTOR *rot, VECTOR *scale, uint8_t *desc,
                       int32_t color, int32_t flags, int32_t depth)
{
	POLY_FT4 *prim;
	int32_t t;
	int32_t w;
	int32_t i;
	int32_t j;

	RotMatrixYXZ(rot, &FISH_SCRATCH->m[1]);
	FISH_SCRATCH->m[1].t[0] = pos->vx;
	FISH_SCRATCH->m[1].t[1] = pos->vy;
	FISH_SCRATCH->m[1].t[2] = pos->vz;
	GsMulCoord0(&GsWSMATRIX, &FISH_SCRATCH->m[1], &FISH_SCRATCH->m[0]);
	GsSetLsMatrix(&FISH_SCRATCH->m[0]);
	prim = (POLY_FT4 *)GsGetWorkBase();

	t = (-desc[8] * scale->vx) >> 12;
	FISH_SCRATCH->pts[2].vx = t;
	FISH_SCRATCH->pts[0].vx = t;

	w = desc[6] - desc[4];
	w = (w > 0) ? w : -w;
	t = (scale->vx * (-desc[8] + w)) >> 12;
	FISH_SCRATCH->pts[3].vx = t;
	FISH_SCRATCH->pts[1].vx = t;

	t = (-desc[9] * scale->vy) >> 12;
	FISH_SCRATCH->pts[1].vy = t;
	FISH_SCRATCH->pts[0].vy = t;

	w = desc[7] - desc[5];
	w = (w > 0) ? w : -w;
	t = (scale->vy * (-desc[9] + w)) >> 12;
	FISH_SCRATCH->pts[3].vy = t;
	FISH_SCRATCH->pts[2].vy = t;

	for (i = 0; i < 4; i++) {
		FISH_SCRATCH->pts[i].vz = 0;
	}

	FISH_SCRATCH->otz = RotTransPers4(FISH_SCRATCH->pts, &FISH_SCRATCH->pts[1],
	                                  &FISH_SCRATCH->pts[2], &FISH_SCRATCH->pts[3],
	                                  (long *)&prim->x0, (long *)&prim->x1, (long *)&prim->x2, (long *)&prim->x3,
	                                  (long *)&FISH_SCRATCH->sz, (long *)&FISH_SCRATCH->flag);

	if ((FISH_SCRATCH->flag & 0x80000000) != 0) {
		return;
	}

	*(int32_t *)&prim->r0 = color;
	prim->u0 = prim->u2 = desc[4];
	prim->u1 = prim->u3 = desc[6];
	prim->v0 = prim->v1 = desc[5];
	prim->v2 = prim->v3 = desc[7];
	prim->tpage = *(uint16_t *)desc | ((flags & 3) << 5);
	prim->clut = *(uint16_t *)(desc + 2);
	setPolyFT4(prim);
	setSemiTrans(prim, flags >> 2);
	addPrim(ACTIVE_ORDERING_TABLE->org + (FISH_SCRATCH->otz >> 2) + depth, prim);
	GsSetWorkBase((PACKET *)(prim + 1));
}

void FISH_freeMarker(FishMarker *s)
{
	FishMarkerPool *pool = &FISHING_DATA_PTR->pool2;

	if (s->unk0 != 0) {
		s->unk0 = 0;
		pool->count = pool->count - 1;
	}
}

void FISH_tickMarker(FishMarker *s)
{
	if (s->unk0 != 0) {
		if ((s->unk1 = s->unk1 - 1) == 0) {
			FISH_freeMarker(s);
		}
	}
}

void FISH_drawMarker(FishMarker *s)
{
	uint8_t *p;

	if (s->unk0 == 0) {
		return;
	}

	if (FISH_projectPoint(&s->unk4, &FISH_SCRATCH->x, &FISH_SCRATCH->y) == 0) {
		return;
	}

	p = (uint8_t *)(FISH_SPRITES + s->unk2);

	FISH_drawSprite2D(FISH_SCRATCH->x, FISH_SCRATCH->y, 0x42, p,
	                  0x1000, 0x1000, 0, 0x808080, 0);
}

void FISH_initMarkerPool(void)
{
	FishMarkerPool *pool = &FISHING_DATA_PTR->pool2;
	int32_t i;

	for (i = 0; i < 16U; i++) {
		pool->items[i].unk0 = 0;
	}

	pool->count = 0;
	pool->next = pool->items;
}

FishMarker *FISH_allocMarker(void)
{
	FishMarkerPool *pool = &FISHING_DATA_PTR->pool2;
	FishMarker *end;
	FishMarker *start;
	FishMarker *p;

	if (pool->count >= 16U) {
		return NULL;
	}

	end = &pool->items[16];
	p = pool->next;
	start = p;

	do {
		if (p->unk0 == 0) {
			pool->next = p;
			return p;
		}

		p++;
		if (p >= end) {
			p = pool->items;
		}
	} while (p != start);

	pool->count = 16;

	return NULL;
}

void FISH_spawnMarker(int32_t a, VECTOR *v)
{
	FishMarkerPool *pool = &FISHING_DATA_PTR->pool2;
	FishMarker *s;

	s = FISH_allocMarker();
	if (s != NULL) {
		pool->count = pool->count + 1;
		s->unk0 = 1;
		s->unk4 = *v;
		s->unk2 = a;
		s->unk1 = 10;
	}
}

void FISH_tickMarkers(void)
{
	FishMarkerPool *pool = &FISHING_DATA_PTR->pool2;
	int32_t i;

	for (i = 0; i < 16U; i++) {
		if (pool->items[i].unk0 != 0) {
			FISH_tickMarker(&pool->items[i]);
		}
	}
}

void FISH_drawMarkers(void)
{
	FishMarkerPool *pool = &FISHING_DATA_PTR->pool2;
	int32_t i;

	for (i = 0; i < 16U; i++) {
		if (pool->items[i].unk0 != 0) {
			FISH_drawMarker(&pool->items[i]);
		}
	}
}

void FISH_clearMarkers(void)
{
	FishMarkerPool *pool = &FISHING_DATA_PTR->pool2;
	int32_t i;

	for (i = 0; i < 16U; i++) {
		pool->items[i].unk0 = 0;
	}

	pool->count = 0;
}

void FISH_freeRipple(FishRipple *s)
{
	FishRipplePool *pool = &FISHING_DATA_PTR->pool1;

	if (s->unk1C != 0) {
		s->unk1C = 0;
		pool->count = pool->count - 1;
	}
}

void FISH_tickRipple(FishRipple *s)
{
	if (s->unk1C != 0) {
		switch (s->unk10) {
		case 0:
			s->unk18 = MAIN_D_80134958[s->unk14 / 2];
			break;
		case -1:
			s->unk18 = MAIN_D_80134960[s->unk14];
			break;
		case -2:
			s->unk18 = MAIN_D_80134968[s->unk14 / 3];
			break;
		case -3:
			s->unk18 = MAIN_D_8013496C[s->unk14];
			break;
		case -4:
			s->unk18 = FISH_D_8007A4B4[s->unk14];
			break;
		default:
			FISH_freeRipple(s);
			return;
		}
		s->unk14++;
		if (s->unk18 == -1) {
			FISH_freeRipple(s);
			return;
		}
		if (s->unk18 >= 13) {
			FISH_freeRipple(s);
		}
	}
}

void FISH_drawRipple(FishRipple *s)
{
	VECTOR scale;
	SVECTOR rot;
	uint8_t *desc;

	if (s->unk1C == 0) {
		return;
	}

	if (s->unk18 <= 0) {
		return;
	}

	if (s->unk10 == 0) {
		desc = (uint8_t *)(FISH_SPRITES + (s->unk18 - 1));
	} else {
		desc = (uint8_t *)(FISH_SPRITES + (s->unk18 + 4));
	}

	rot.vx = 0;
	rot.vy = 0;
	rot.vz = 0;
	scale.vx = (FISHING_DATA_PTR->mapDataPtr->unk4 * s->unk20) >> 12;
	scale.vy = (FISHING_DATA_PTR->mapDataPtr->unk4 * s->unk20) >> 12;
	scale.vz = (FISHING_DATA_PTR->mapDataPtr->unk4 * s->unk20) >> 12;
	FISH_drawSprite3D(&s->unk0, &rot, &scale, desc, 0x808080, 5, -10);
}

void FISH_initRipplePool(void)
{
	FishRipplePool *pool = &FISHING_DATA_PTR->pool1;
	int32_t i;

	for (i = 0; i < 16U; i++) {
		pool->items[i].unk1C = 0;
	}

	pool->count = 0;
	pool->next = pool->items;
	pool->unk244 = 0;
}

FishRipple *FISH_allocRipple(void)
{
	FishRipplePool *pool = &FISHING_DATA_PTR->pool1;
	FishRipple *end;
	FishRipple *start;
	FishRipple *p;

	if (pool->count >= 16U) {
		return NULL;
	}

	end = &pool->items[16];
	p = pool->next;
	start = p;

	do {
		if (p->unk1C == 0) {
			pool->next = p;
			return p;
		}

		p++;
		if (p >= end) {
			p = pool->items;
		}
	} while (p != start);

	pool->count = 16;

	return NULL;
}

void FISH_spawnRipple(int32_t a, VECTOR *v, int32_t c, int32_t spread, int32_t e)
{
	FishRipplePool *pool = &FISHING_DATA_PTR->pool1;
	FishRipple *s;

	s = FISH_allocRipple();
	if (s == NULL) {
		return;
	}

	pool->count = pool->count + 1;
	s->unk1C = 1;
	s->unk0 = *v;
	s->unk10 = a;
	s->unk14 = 0;
	s->unk18 = 0;
	s->unk0.vx = s->unk0.vx + customRandom(-spread, spread);
	s->unk0.vz = s->unk0.vz + customRandom(-spread, spread);
	s->unk0.vy = 0;
	s->unk20 = e;

	if (c != 0) {
		pool->unk244 = (uint32_t)s;
		pool->unk24C = c;
	}
}

void FISH_tickRipples(void)
{
	FishRipplePool *pool = &FISHING_DATA_PTR->pool1;
	FishRipple *p;
	int32_t i;

	for (i = 0; i < 16U; i++) {
		if (pool->items[i].unk1C != 0) {
			FISH_tickRipple(&pool->items[i]);
		}
	}

	if (pool->unk244 != 0) {
		p = (FishRipple *)pool->unk244;
		if (p->unk1C == 0) {
			pool->unk244 = 0;
		}
	}
}

void FISH_drawRipples(void)
{
	FishRipplePool *pool = &FISHING_DATA_PTR->pool1;
	int32_t i;

	for (i = 0; i < 16U; i++) {
		if (pool->items[i].unk1C != 0) {
			FISH_drawRipple(&pool->items[i]);
		}
	}
}

void FISH_loadSpritesAndSeadramon(TMDFileLoadingData *m)
{
	uint32_t tpage;
	uint32_t clut;
	FishingData *d;

	loadTextureFile(FISHING_DATA_PTR->mapDataPtr->spriteFilePath, &tpage, &clut);

	d = FISHING_DATA_PTR;
	FISH_loadTmdModel(&d->seadraModel, FISH_D_8007A4C8, d->seadraModelBuffer, 0x1000);
}

void FISH_onRodModelLoaded(TMDFileLoadingData *m)
{
}

void FISH_resetTension(void)
{
	FishingTension *t = &FISHING_DATA_PTR->tension;

	t->display = 0;
	t->level = 0;
	t->unk10 = 0;
	t->color.word = 0xFFFFFF;
}

void FISH_beginTension(void)
{
	FishingTension *t = &FISHING_DATA_PTR->tension;

	FISH_resetTension();
	t->display = 1;
	t->limit = (FISHING_DATA_PTR->rod.swimmerType == 1) ? 0x1388 : 0x2710;
}

inline int32_t FISH_tensionOverLimit(FishingTension *t, int32_t level)
{
	return t->limit < level;
}

int32_t FISH_updateTension(void)
{
	FishingTension *t = &FISHING_DATA_PTR->tension;
	int32_t n;
	int32_t m;

	n = 0;
	n += (FISHING_DATA_PTR->rod.pressedFrames > 0) ? 2 : 0;
	n += (FISHING_DATA_PTR->hooked.fish.target == 0x32) ? 1 : 0;

	switch (n) {
	case 0:
		t->level = t->level +
		           FISH_remapClamped(FISHING_DATA_PTR->hooked.fish.size, 0x32, 0x12C, -0xA6, -0xA6);
		break;
	case 1:
		t->level = t->level +
		           FISH_remapClamped(FISHING_DATA_PTR->hooked.fish.size, 0x32, 0x12C, 0x29, 0x53);
		break;
	case 2:
		t->level = t->level +
		           FISH_remapClamped(FISHING_DATA_PTR->hooked.fish.size, 0x32, 0x12C, 0x47, 0x7D);
		break;
	case 3:
		t->level = t->level +
		           FISH_remapClamped(FISHING_DATA_PTR->hooked.fish.size, 0x32, 0x12C, 0xA6, 0x1F4);
		break;
	}

	t->level = (t->level < -t->limit)
	                   ? -t->limit
	                   : (FISH_tensionOverLimit(t, t->level) ? t->limit : t->level);

	if (t->level >= t->limit / 2) {
		t->color.rgb[0] = FISH_remapClamped(t->level, t->limit / 2, t->limit, 0xFF, 0xFF);
		t->color.rgb[1] = FISH_remapClamped(t->level, t->limit / 2, t->limit, 0xFF, 0);
		t->color.rgb[2] = FISH_remapClamped(t->level, t->limit / 2, t->limit, 0, 0);
	} else if (t->level >= 0) {
		t->color.rgb[0] = FISH_remapClamped(t->level, 0, t->limit / 2, 0xFF, 0xFF);
		t->color.rgb[1] = FISH_remapClamped(t->level, 0, t->limit / 2, 0xFF, 0xFF);
		t->color.rgb[2] = FISH_remapClamped(t->level, 0, t->limit / 2, 0xFF, 0);
	} else if (t->level >= -t->limit / 2) {
		t->color.rgb[0] = FISH_remapClamped(t->level, -t->limit / 2, 0, 0, 0xFF);
		t->color.rgb[1] = FISH_remapClamped(t->level, -t->limit / 2, 0, 0xFF, 0xFF);
		t->color.rgb[2] = FISH_remapClamped(t->level, -t->limit / 2, 0, 0xFF, 0xFF);
	} else {
		t->color.rgb[0] = FISH_remapClamped(t->level, -t->limit, -t->limit / 2, 0, 0);
		t->color.rgb[1] = FISH_remapClamped(t->level, -t->limit, -t->limit / 2, 0, 0xFF);
		t->color.rgb[2] = FISH_remapClamped(t->level, -t->limit, -t->limit / 2, 0xFF, 0xFF);
	}

	if (t->level == -t->limit) {
		return 2;
	}

	if (t->level == t->limit) {
		m = 4 - FISH_remapClamped(FISHING_DATA_PTR->hooked.fish.size, 0x32, 0x12C, 1, 3);
		n = t->unk10 + 1;
		t->unk10 = n;
		if (n == m * 5) {
			return 1;
		}
	} else {
		t->unk10 = 0;
	}

	return 0;
}

INCLUDE_ASM("asm/fish/nonmatchings/fish", FISH_drawTensionGauge);

void FISH_hideBaitSprite(void)
{
	FISHING_DATA_PTR->item.shown = 0;
}

void FISH_initBaitSprite(void)
{
	FISH_hideBaitSprite();
}

void FISH_clearBaitSprite(void)
{
	FISH_hideBaitSprite();
}

void FISH_showBaitSprite(VECTOR *pos, int32_t sprite)
{
	FishingItem *item = &FISHING_DATA_PTR->item;

	item->shown = 1;
	item->sprite = sprite;
	if (pos != NULL) {
		item->pos = *pos;
	}
}

int32_t FISH_moveBaitToRodTip(int32_t maxStep)
{
	FishingItem *item = &FISHING_DATA_PTR->item;
	VECTOR target;
	VECTOR delta;
	SVECTOR rot;
	MATRIX m;
	int32_t dist;

	FISH_getBoneTransform(&TAMER_ENTITY.entity, 6, &target, &rot);
	RotMatrixYXZ(&rot, &m);
	ApplyMatrix(&m, &MAIN_D_8013497C, &delta);

	target.vx = target.vx + delta.vx;
	target.vy = target.vy + delta.vy;
	target.vz = target.vz + delta.vz;

	target.vx = target.vx + MAIN_D_80134984;
	target.vy = target.vy + MAIN_D_80134986;
	target.vz = target.vz + MAIN_D_80134988;

	dist = (!maxStep) ? 0 : getDistance(target.vx - item->pos.vx, target.vy - item->pos.vy, target.vz - item->pos.vz);

	if ((maxStep != 0) && (maxStep < dist)) {
		toEulerAngles(&rot, target.vx - item->pos.vx, target.vy - item->pos.vy,
		              target.vz - item->pos.vz);

		delta.vx = 0;
		delta.vy = 0;
		delta.vz = maxStep;

		rotateVectorYXZ(&rot, &delta, &target);

		item->pos.vx = item->pos.vx + target.vx;
		item->pos.vy = item->pos.vy + target.vy;
		item->pos.vz = item->pos.vz + target.vz;
		return 1;
	}

	item->pos = target;

	return 0;
}

void FISH_tickBaitSprite(void)
{
	FishingItem *item = &FISHING_DATA_PTR->item;
	VECTOR pos;
	VECTOR step;
	SVECTOR rot;
	VECTOR probe;
	VECTOR *ip;
	int32_t blocked;

	if (item->shown == 0) {
		return;
	}

	pos = item->pos;

	toEulerAngles(&rot,
	              FISHING_DATA_PTR->swimmerLoc.vx - FISHING_DATA_PTR->rod.lineStart.vx,
	              FISHING_DATA_PTR->swimmerLoc.vy - FISHING_DATA_PTR->rod.lineStart.vy,
	              FISHING_DATA_PTR->swimmerLoc.vz - FISHING_DATA_PTR->rod.lineStart.vz);

	step.vx = 0;
	step.vy = 0;
	step.vz = 0x64;

	rotateVectorYXZ(&rot, &step, ip = &item->pos);

	item->pos.vx = item->pos.vx + FISHING_DATA_PTR->swimmerLoc.vx;
	item->pos.vy = item->pos.vy + FISHING_DATA_PTR->swimmerLoc.vy;
	item->pos.vz = item->pos.vz + FISHING_DATA_PTR->swimmerLoc.vz;

	if (pos.vy >= 0) {
		return;
	}

	if (item->pos.vy < 0) {
		return;
	}

	if (FISHING_DATA_PTR->rod.fishingState != 8) {
		return;
	}

	blocked = 0;
	probe = item->pos;
	probe.vy = 0;

	if (FISH_projectPoint(&probe, &FISH_SCRATCH->x, &FISH_SCRATCH->y) == 0) {
		blocked = 1;
	} else if ((FISH_isScreenPointVisible(FISH_SCRATCH->x, FISH_SCRATCH->y) == 0) ||
	           (FISHING_DATA_PTR->mapDataPtr->unk20(FISH_SCRATCH->x,
	                                                FISH_SCRATCH->y) == 0)) {
		blocked = 1;
	}

	if (blocked != 0) {
		return;
	}

	FISH_spawnRipple(0, ip, 0x64, 0, FISHING_DATA_PTR->mapDataPtr->unk8 * 75);
}

void FISH_drawBaitSprite(void)
{
	FishingItem *item = &FISHING_DATA_PTR->item;
	POLY_FT4 *prim;
	MATRIX m1;
	MATRIX m2;
	int32_t r;

	if (item->shown == 0) {
		return;
	}

	if (item->pos.vy >= 0) {
		return;
	}

	((VECTOR *)m1.t)->vx = item->pos.vx;
	((VECTOR *)m1.t)->vy = item->pos.vy;
	((VECTOR *)m1.t)->vz = item->pos.vz;
	GsMulCoord0(&GsWSMATRIX, &m1, &m2);

	if (m2.t[2] <= 0) {
		return;
	}

	if (m2.t[2] >= 0x10000) {
		return;
	}

	m2.t[0] = (m2.t[0] * VIEWPORT_DISTANCE) / m2.t[2];
	m2.t[1] = (m2.t[1] * VIEWPORT_DISTANCE) / m2.t[2];
	r = (FISHING_DATA_PTR->mapDataPtr->unk8 * VIEWPORT_DISTANCE) / m2.t[2];

	prim = (POLY_FT4 *)GsGetWorkBase();
	*(int32_t *)&prim->r0 = 0x808080;
	prim->tpage = 5;
	prim->clut = GetClut(0xE0, MAIN_D_80127BDC[item->sprite] + 0x1E8);
	prim->u0 = prim->u2 = (item->sprite % 16) * 16;
	prim->v0 = prim->v1 = (item->sprite / 16) * 16;
	prim->u1 = prim->u3 = prim->u0 + 0xF;
	prim->v2 = prim->v3 = prim->v0 + 0xF;
	prim->x0 = m2.t[0] - r;
	prim->y0 = m2.t[1] - r;
	prim->x1 = (m2.t[0] - r) + r * 2;
	prim->y1 = m2.t[1] - r;
	prim->x2 = m2.t[0] - r;
	prim->y2 = (m2.t[1] - r) + r * 2;
	prim->x3 = (m2.t[0] - r) + r * 2;
	prim->y3 = (m2.t[1] - r) + r * 2;
	setPolyFT4(prim);
	addPrim(ACTIVE_ORDERING_TABLE->org + (m2.t[2] >> 4), prim);
	GsSetWorkBase((PACKET *)(prim + 1));
}

void FISH_resetBobberAnim(FishRipple *s)
{
	s->unk10 = 3;
	s->unk18 = 0;
	s->unk14 = 0;
	s->unk1C = 0;
}

void FISH_initBobberAnim(FishRipple *s)
{
	FISH_resetBobberAnim(s);
}

void FISH_clearBobberAnim(FishRipple *s)
{
	FISH_resetBobberAnim(s);
}

void FISH_getRodTipPos(VECTOR *out)
{
	VECTOR v;
	SVECTOR rot;
	MATRIX m;

	FISH_getBoneTransform(&TAMER_ENTITY.entity, 6, &v, &rot);
	RotMatrixYXZ(&rot, &m);
	ApplyMatrix(&m, &MAIN_D_8013498C, out);

	out->vx = out->vx + v.vx;
	out->vy = out->vy + v.vy;
	out->vz = out->vz + v.vz;

	out->vx = out->vx + MAIN_D_80134994;
	out->vy = out->vy + MAIN_D_80134996;
	out->vz = out->vz + MAIN_D_80134998;
}

void FISH_bobberCommand(VECTOR *v, int32_t n)
{
	FishingSwimmer *s = (FishingSwimmer *)v;
	VECTOR scale;
	SVECTOR rot;
	uint8_t *desc;
	int32_t t;
	int32_t k;
	int32_t m;

	switch (n) {
	case 0:
		goto stateDispatch;
	case 1:
		goto drawSwimmer;
	case 2:
		goto snap2;
	case 4:
		goto start4;
	case 5:
		goto start5;
	case 6:
		goto start6;
	case 7:
		goto start7;
	case 11:
		goto snap11;
	case 12:
		goto snap12;
	case 3:
	case 8:
	case 9:
	case 10:
	case 13:
		goto end;
	case 14:
		goto start14;
	default:
		goto end;
	}

stateDispatch:
	switch (s->state) {
	case 3:
		goto clearMsg;
	case 7:
		goto msg7;
	case 8:
		goto msg8;
	case 9:
		goto msg9;
	case 10:
		goto msg10;
	case 11:
		goto msg11;
	case 12:
		goto msg12;
	case 4:
	case 5:
	case 6:
	case 13:
		goto tick;
	case 14:
		goto msg14;
	default:
		goto tick;
	}

clearMsg:
	s->msg = 0;
	goto tick;

start4:
	FISH_getRodTipPos(v);
	s->msg = 1;
	s->state = 4;
	s->timer = -1;
	goto tick;

start5:
	s->msg = 1;
	s->state = 5;
	s->timer = -1;
	goto tick;

start6:
	s->msg = 1;
	s->state = 6;
	s->timer = -1;
	goto tick;

start7:
	FISH_spawnRipple(-1, v, 0x64, 0, 0x1000);
	s->state = 7;
	s->timer = -1;
	goto tick;

msg7:
	t = (s->timer < 0) ? 0 : ((s->timer >= 8U) ? 7 : s->timer);
	s->msg = MAIN_D_8013499C[t];
	if (s->msg > -1) {
		goto tick;
	}

setMsg3:
	k = 3;
setMsgAdvance:
	s->msg = k;
	s->state = 8;
	s->timer = -1;
	goto tick;

msg8:
	if (customRandom(1, 2) == 1) {
		s->pos.vx = s->pos.vx + customRandom(-1, 1);
		s->pos.vz = s->pos.vz + customRandom(-1, 1);
	}

	if (customRandom(1, 0x3C) != 1) {
		goto tick;
	}

	FISH_spawnRipple(-2, v, 0, 0, 0x1000);
	s->state = 9;
	s->timer = -1;
	goto tick;

msg9:
	t = (s->timer / 3 < 0) ? 0 : ((s->timer / 3 >= 3U) ? 2 : s->timer / 3);
	s->msg = FISH_D_8007A4D8[t];
	if (s->msg != -1) {
		goto tick;
	}

	k = 3;
	goto setMsgAdvance;

snap2:
	playSound(8, 3);
	FISH_spawnRipple(-3, v, 0, 0, 0x1000);
	s->state = 0xA;
	s->timer = -1;
	goto tick;

msg10:
	t = (s->timer < 0) ? 0 : ((s->timer >= 5U) ? 4 : s->timer);
	s->msg = FISH_D_8007A4E4[t];
	if (s->msg != -1) {
		goto tick;
	}

	k = 3;
	goto setMsgAdvance;

snap12:
	playSound(8, 4);
	FISH_spawnRipple(-4, v, 0, 0, 0x1000);
	s->state = 0xB;
	s->timer = -1;
	goto tick;

msg12:
	t = (s->timer < 0) ? 0 : ((s->timer >= 0x12U) ? 0x11 : s->timer);
	s->msg = FISH_D_8007A4F8[t];
	if (s->msg != -1) {
		goto tick;
	}

	k = 3;
	goto setMsgAdvance;

snap11:
	playSound(8, 4);
	FISH_spawnRipple(-4, v, 0, 0, 0x1000);
	s->state = 0xB;
	s->timer = -1;
	goto tick;

msg11:
	t = (s->timer < 0) ? 0 : ((s->timer >= 5U) ? 4 : s->timer);
	s->msg = FISH_D_8007A540[t];
	if (s->msg != -1) {
		goto tick;
	}

	s->state = 0xD;
	s->timer = -1;
	goto tick;

start14:
	s->state = 0xE;
	s->timer = -1;
	goto tick;

msg14:
	t = (s->timer < 0) ? 0 : ((s->timer >= 6U) ? 5 : s->timer);
	s->msg = FISH_D_8007A554[t];
	if (s->msg == -1) {
		goto setMsg3;
	}

tick:
	s->timer = s->timer + 1;
	goto end;

drawSwimmer:
	if (s->msg <= 0) {
		goto end;
	}

	t = (FISHING_DATA_PTR->rod.swimmerType == 1) ? 0x11 : 0x18;
	desc = (uint8_t *)(FISH_SPRITES + (s->msg - 1 + t));
	rot.vx = 0;
	rot.vy = 0;
	rot.vz = 0;
	scale.vx = FISHING_DATA_PTR->mapDataPtr->unk4;
	scale.vy = FISHING_DATA_PTR->mapDataPtr->unk4;
	scale.vz = FISHING_DATA_PTR->mapDataPtr->unk4;
	FISH_drawSprite3D(v, &rot, &scale, desc, 0x808080, 0, -10);

end:;
}

INCLUDE_ASM("asm/fish/nonmatchings/fish", FISH_tickBobberPhysics);

void FISH_pushSeadramonSegment(VECTOR *unused, FishHist *src)
{
	int32_t i;

	for (i = 4; i > 0; i--) {
		FISH_D_8007AB40[i] = FISH_D_8007AB40[i - 1];
	}

	FISH_D_8007AB40[0] = *src;
}

void FISH_drawSeadramon(VECTOR *start, SVECTOR *rot)
{
	VECTOR pos;
	GsDOBJ2 *obj;
	int32_t i;

	pos = *start;

	for (i = 0; i < 5; i++) {
		obj = FISH_getModelObject(&FISHING_DATA_PTR->seadraModel, i);

		FISH_D_8007AB40[i].v[0] += 0x800;
		FISH_D_8007AB40[i].v[2] = 0x800 - FISH_D_8007AB40[i].v[2];

		RotMatrixYXZ((SVECTOR *)&FISH_D_8007AB40[i], &FISH_SCRATCH->m[1]);

		FISH_D_8007AB40[i].v[0] = FISH_D_8007AB40[i].v[0] - 0x800;
		FISH_D_8007AB40[i].v[2] = 0x800 - FISH_D_8007AB40[i].v[2];

		FISH_SCRATCH->v0.vx = 0x1000;
		FISH_SCRATCH->v0.vy = 0x1000;
		FISH_SCRATCH->v0.vz = 0x1000;
		ScaleMatrix(&FISH_SCRATCH->m[1], &FISH_SCRATCH->v0);

		FISH_SCRATCH->m[1].t[0] = pos.vx;
		FISH_SCRATCH->m[1].t[1] = pos.vy;
		FISH_SCRATCH->m[1].t[2] = pos.vz;

		GsMulCoord0(&GsWSMATRIX, &FISH_SCRATCH->m[1], &FISH_SCRATCH->m[0]);

		obj->coord2 = NULL;
		obj->attribute = 0;

		GsSetLsMatrix(&FISH_SCRATCH->m[0]);
		GsSetLightMatrix(&FISH_SCRATCH->m[1]);
		GsSortObject4(obj, ACTIVE_ORDERING_TABLE, 2, FISH_SCRATCH->ot);

		RotMatrixYXZ((SVECTOR *)&FISH_D_8007AB40[i], &FISH_SCRATCH->m[1]);

		FISH_SCRATCH->v0.vx = 0x1000;
		FISH_SCRATCH->v0.vy = 0x1000;
		FISH_SCRATCH->v0.vz = 0x1000;
		ScaleMatrix(&FISH_SCRATCH->m[1], &FISH_SCRATCH->v0);

		ApplyMatrix(&FISH_SCRATCH->m[1], &FISH_D_8007A56C[i], &FISH_SCRATCH->v0);

		pos.vx = pos.vx + FISH_SCRATCH->v0.vx;
		pos.vy = pos.vy + FISH_SCRATCH->v0.vy;
		pos.vz = pos.vz + FISH_SCRATCH->v0.vz;
	}
}

void FISH_spawnSeadramon(Fish *f)
{
	FishSeadra *sea = &FISHING_DATA_PTR->seadra;

	f->kind = 6;
	f->size = customRandom(FISH_D_8007AA28[f->kind].lo, FISH_D_8007AA28[f->kind].hi);
	f->lifetime = customRandom(0x258, 0x4B0);
	f->target = 0;
	f->unk28 = 0;
	f->transparency = 0;
	f->timeThisMove = 0;

	sea->path = FISHING_DATA_PTR->mapDataPtr->seadramonWaypoints;
	sea->unk4 = FISHING_DATA_PTR->mapDataPtr->unk34;

	f->target = 0;
	MAIN_func_800E4038(&f->pos, sea->path[f->target].vx, sea->path[f->target].vy, NULL);
	f->target = f->target + 1;
	MAIN_func_800E4038(&FISH_SCRATCH->v0, sea->path[f->target].vx, sea->path[f->target].vy, NULL);

	toEulerAngles(&f->rotation, FISH_SCRATCH->v0.vx - f->pos.vx, 0,
	              FISH_SCRATCH->v0.vz - f->pos.vz);
}

INCLUDE_ASM("asm/fish/nonmatchings/fish", FISH_tickSeadramon);

void FISH_spawnFish(Fish *f, int32_t kind)
{
	VECTOR v;

	f->kind = (uint8_t)kind;
	f->size = customRandom(FISH_D_8007AA28[f->kind].lo, FISH_D_8007AA28[f->kind].hi);
	f->lifetime = customRandom(0x258, 0x4B0);
	f->target = 0;
	f->unk28 = 0;
	f->transparency = 0;
	FISH_randomPointInSpot(f->kind, &f->pos);
	FISH_randomPointInSpot(f->kind, &v);
	toEulerAngles(&f->rotation, v.vx - f->pos.vx, 0, v.vz - f->pos.vz);
}

void FISH_tickFish(Fish *f)
{
	int32_t e;
	VECTOR step;
	VECTOR out;
	VECTOR target;
	VECTOR sw2;
	FishRipple *p;
	PositionData *pd;
	int32_t d;

	if (f->kind == 6) {
		FISH_tickSeadramon(f);
		return;
	}

	switch (f->target) {
	case 0:
		step.vx = 0;
		step.vy = 0;
		step.vz = 0x1E;
		rotateVectorYXZ(&f->rotation, &step, &out);
		f->pos.vx = f->pos.vx + out.vx;
		f->pos.vy = f->pos.vy + out.vy;
		f->pos.vz = f->pos.vz + out.vz;
		f->transparency = f->transparency + 8;
		f->transparency = (f->transparency < 0) ? 0
		                                        : ((f->transparency > 0x80) ? 0x80 : f->transparency);
		if (f->transparency != 0x80) {
			break;
		}
pick:
		f->timeNextMove = customRandom(FISH_D_8007AA28[f->kind].unkC,
		                               FISH_D_8007AA28[f->kind].unk10);
		f->target = 0xA;
		f->unk28 = -1;
		break;

	case 0xA:
		d = f->lifetime;
		d = d - 1;
		f->lifetime = d;
		if (d == 0) {
			goto tamer;
		}
		d = f->timeNextMove;
		d = d - 1;
		f->timeNextMove = d;
		if (d == 0) {
			goto newtarget;
		}
check:
		if (FISHING_DATA_PTR->pool1.unk244 != 0) {
			p = (FishRipple *)FISHING_DATA_PTR->pool1.unk244;
			if (getDistance(p->unk0.vx - f->pos.vx, 0, p->unk0.vz - f->pos.vz) <= FISHING_DATA_PTR->pool1.unk24C) {
				FISHING_DATA_PTR->pool.escapeTimer = 0x1E;
				goto lookat;
			}
		}
		if (FISHING_DATA_PTR->rod.bait != FISH_D_8007AA28[f->kind].bait) {
			break;
		}
		d = getDistance(FISHING_DATA_PTR->swimmerLoc.vx - f->pos.vx, 0,
		                FISHING_DATA_PTR->swimmerLoc.vz - f->pos.vz);
		if (d < 0xC9) {
			break;
		}
		if (d < 0x258) {
			goto approach;
		}
		break;
lookat:
		p = (FishRipple *)FISHING_DATA_PTR->pool1.unk244;
		toEulerAngles(&f->rotation, p->unk0.vx - f->pos.vx, 0,
		              p->unk0.vz - f->pos.vz);
		f->rotation.vy = f->rotation.vy + 0x800;
		f->target = 0x1E;
		f->unk28 = -1;
		break;
newtarget:
		FISH_randomPointInSpot(f->kind, &target);
		toEulerAngles(&f->rotation, target.vx - f->pos.vx, 0,
		              target.vz - f->pos.vz);
		f->timeThisMove = getDistance(target.vx - f->pos.vx, 0,
		                              target.vz - f->pos.vz) /
		                  8;
		f->timeThisMove = (f->timeThisMove < 0) ? 0
		                                        : ((f->timeThisMove > 0x32) ? 0x32 : f->timeThisMove);
		f->timeThisMove = (f->timeThisMove < 0) ? 0
		                                        : ((f->size < f->timeThisMove) ? f->size : f->timeThisMove);
		f->target = 0x14;
		f->unk28 = -1;
		break;
approach:
		sw2 = FISHING_DATA_PTR->swimmerLoc;
		toEulerAngles(&f->rotation, sw2.vx - f->pos.vx, 0, sw2.vz - f->pos.vz);
		f->timeThisMove = getDistance(sw2.vx - f->pos.vx, 0, sw2.vz - f->pos.vz) / 8;
		f->timeThisMove = (f->timeThisMove < 3) ? 3
		                                        : ((f->timeThisMove > 0x32) ? 0x32 : f->timeThisMove);
		f->timeThisMove = (f->timeThisMove < 3) ? 3
		                                        : ((f->size < f->timeThisMove) ? f->size : f->timeThisMove);
		f->target = 0x14;
		f->unk28 = -1;
		break;

	case 0x14:
		step.vx = 0;
		step.vy = 0;
		step.vz = f->timeThisMove;
		rotateVectorYXZ(&f->rotation, &step, &out);
		f->pos.vx = f->pos.vx + out.vx;
		f->pos.vy = f->pos.vy + out.vy;
		f->pos.vz = f->pos.vz + out.vz;
		e = f->timeThisMove;
		d = e;
		e = e >> 3;
		f->timeThisMove = d - e;
		if (f->timeThisMove < 8) {
			goto pick;
		}
		goto check;

tamer:
		pd = TAMER_ENTITY.entity.posData;
		toEulerAngles(&f->rotation, pd->location.vx - f->pos.vx, 0,
		              pd->location.vz - f->pos.vz);
		f->rotation.vy = f->rotation.vy + 0x800;
		f->target = 0x1E;
		f->unk28 = -1;
		break;

	case 0x1E:
		step.vx = 0;
		step.vy = 0;
		step.vz = 0x28;
		rotateVectorYXZ(&f->rotation, &step, &out);
		f->pos.vx = f->pos.vx + out.vx;
		f->pos.vy = f->pos.vy + out.vy;
		f->pos.vz = f->pos.vz + out.vz;
		f->transparency = f->transparency - 8;
		f->transparency = (f->transparency < 0) ? 0
		                                        : ((f->transparency > 0x80) ? 0x80 : f->transparency);
		if (f->transparency == 0) {
			f->alive = 0;
			FISHING_DATA_PTR->pool.count = FISHING_DATA_PTR->pool.count - 1;
		}
		break;
	}

	f->unk28 = f->unk28 + 1;
	f->unk20 = f->unk20 + 1;
}

void FISH_drawFishSprite(int32_t kind, VECTOR *pos, SVECTOR *rot, int32_t size, int32_t phase, int32_t transparency, int32_t flag)
{
	SVECTOR r;
	VECTOR scale;
	uint8_t *desc;
	int32_t t;
	int32_t shade;

	if (kind == 6) {
		FISH_drawSeadramon(pos, rot);
		return;
	}

	if (flag == 0) {
		if (size >= 0x64) {
			desc = (uint8_t *)(FISH_SPRITES + MAIN_D_801349A4[(phase / 3) & 7]);
			t = size * FISHING_DATA_PTR->mapDataPtr->unk10;
			scale.vz = t;
			scale.vy = t;
			scale.vx = t;
		} else {
			desc = (uint8_t *)(FISH_SPRITES + MAIN_D_801349AC[(phase / 4) & 3]);
			t = size * FISHING_DATA_PTR->mapDataPtr->unkC;
			scale.vz = t;
			scale.vy = t;
			scale.vx = t;
		}
	} else {
		if (size >= 0x64) {
			desc = (uint8_t *)(FISH_SPRITES + MAIN_D_801349B0[(uint32_t)phase % 6]);
			t = size * 170;
			scale.vz = t;
			scale.vy = t;
			scale.vx = t;
		} else {
			desc = (uint8_t *)(FISH_SPRITES + MAIN_D_801349B8[(uint32_t)phase % 6]);
			t = size * 0x155;
			scale.vz = t;
			scale.vy = t;
			scale.vx = t;
		}
	}

	shade = transparency * 30 / 128;

	r.vx = -0x400;
	r.vy = rot->vy;
	r.vz = 0;

	FISH_drawSprite3D(pos, &r, &scale, desc,
	                  (shade & 0xFF) | ((shade & 0xFF) << 8) | ((shade & 0xFF) << 16), 6, 0x64);
}

void FISH_drawFish(Fish *f)
{
	FISH_drawFishSprite(f->kind, &f->pos, &f->rotation, f->size, f->unk20, f->transparency, 0);
}

Fish *FISH_allocFish(void)
{
	FishPool *pool = &FISHING_DATA_PTR->pool;
	Fish *end;
	Fish *start;
	Fish *p;

	if (pool->count >= 33U) {
		return NULL;
	}

	end = &pool->items[33];
	p = pool->next;
	start = p;

	do {
		if (p->alive == 0) {
			pool->next = p;
			return p;
		}

		p++;
		if (p >= end) {
			p = pool->items;
		}
	} while (p != start);

	return NULL;
}

int32_t FISH_isSeadramonAlive(void)
{
	FishingData *d = FISHING_DATA_PTR;
	Fish *f = d->pool.items;
	Fish *end = &d->pool.items[33];

	while (f < end) {
		if (f->alive != 0) {
			if (f->kind == 6) {
				return 1;
			}
		}
		f++;
	}

	return 0;
}

INCLUDE_ASM("asm/fish/nonmatchings/fish", FISH_tickFishPopulation);

void FISH_drawFishPool(void)
{
	FishPool *pool = &FISHING_DATA_PTR->pool;
	int32_t i;

	for (i = 0; i < 33U; i++) {
		if (pool->items[i].alive) {
			FISH_drawFish(&pool->items[i]);
		}
	}
}

void FISH_initFishPool(void)
{
	FishPool *pool = &FISHING_DATA_PTR->pool;
	int32_t i;

	for (i = 0; i < 33U; i++) {
		pool->items[i].alive = 0;
	}

	pool->next = &pool->items[1];
	pool->count = 0;
	pool->unk740 = 0;
	pool->frameCount = 0;
	pool->escapeTimer = 0;
}

void FISH_fishPoolStub(void)
{
}

Fish *FISH_findFishToBite(Fish *pool, VECTOR *loc, int32_t depth)
{
	Fish *f;
	int32_t i;
	int32_t chance;
	int32_t rowOff;

	if (FISHING_DATA_PTR->hooked.fish.alive) {
		return NULL;
	}

	i = 0;
	f = pool;
	rowOff = (depth - 0x26) * 6;
	for (; i < 0x21U; i++, f = (Fish *)((int32_t)f + 56)) {
		if (!f->alive) {
			continue;
		}
		if (getDistance(f->pos.vx - loc->vx, 0, f->pos.vz - loc->vz) >= 0xC9) {
			continue;
		}
		if (f->kind == 6) {
			if ((VSync(-1) / 3) % 8 != 0) {
				continue;
			} else {
			}
			chance = 0x14;
		} else {
			if ((VSync(-1) / 3) % 20 != 0) {
				continue;
			}
			if ((f->target != 0xA) && (f->target != 0x14)) {
				continue;
			}
			chance = 0;
			if ((depth >= 0x26) && (depth < 0x47)) {
				chance = (FISH_D_8007A3EC + rowOff)[f->kind];
			}
		}
		FISH_spawnMarker((chance == 0) ? 0x30 : 0x2F, &f->pos);
		if (chance < customRandom(1, 0x64)) {
			continue;
		}
		return f;
	}

	return NULL;
}

void FISH_clearHookedFish(void)
{
	FISHING_DATA_PTR->hooked.fish.alive = 0;
}

void FISH_hookFish(Fish *f)
{
	HookedFish *h = &FISHING_DATA_PTR->hooked;

	FISH_clearMarkers();

	h->fish.alive = f->alive;
	h->fish.kind = f->kind;
	h->fish.size = f->size;
	h->fish.transparency = f->transparency;
	h->fish.pos = f->pos;
	h->fish.rotation = f->rotation;
	h->fish.unk20 = f->unk20;
	h->fish.target = f->target;
	h->fish.unk28 = f->unk28;
	h->fish.lifetime = f->lifetime;
	h->fish.timeNextMove = f->timeNextMove;
	h->fish.timeThisMove = f->timeThisMove;

	f->alive = 0;

	FISHING_DATA_PTR->pool.count = FISHING_DATA_PTR->pool.count - 1;

	toEulerAngles(&FISHING_DATA_PTR->hooked.fish.rotation,
	              FISHING_DATA_PTR->swimmerLoc.vx - h->fish.pos.vx, 0,
	              FISHING_DATA_PTR->swimmerLoc.vz - h->fish.pos.vz);

	h->fish.target = 10;
	h->fish.unk28 = 0;
	h->fish.unk20 = 0;
	h->animMode = 0;
	h->unk4C = 0;

	if (h->fish.size < 0x96) {
		h->nibbleTime = customRandom(0x14, 0x28);
		h->catchingTime = 0x1E;
	} else {
		h->nibbleTime = customRandom(0x28, 0x3C);
		h->catchingTime = 0x3C;
	}

	h->powerLeft = h->fish.size + 0x96;
	h->powerMax = h->powerLeft + 0x64;
	h->escaped = 0;
}

void FISH_tickHookedFish(int32_t mode)
{
	HookedFish *h = &FISHING_DATA_PTR->hooked;
	VECTOR step;
	VECTOR probe;
	int32_t i;
	int32_t k;
	int32_t t;

	if (!h->fish.alive) {
		goto ret;
	}

	if (FISH_isTextBoxVisible() != 0) {
		goto ret;
	}

	if (mode == 0) {
		goto stateDispatch;
	}

	if (mode == 0x11) {
		goto caught;
	}

	if (mode == 0x12) {
		goto snapped;
	}

	if (mode == 0xF) {
		goto escape;
	}

	if (mode != 0x10) {
		goto checkKind;
	}

	goto grabbed;

caught:
	h->fish.alive = 0;
	goto checkKind;

stateDispatch:
	t = h->fish.target;
	if (t == 0x64) {
		goto seadraChase;
	}

	if (t == 0x5A) {
		goto sinking;
	}

	if (t == 0x32) {
		goto fighting;
	}

	if (t == 0x28) {
		goto struggling;
	}

	if (t == 0x14) {
		goto approaching;
	}

	if (t != 0xA) {
		goto tick;
	}

	if (h->fish.unk28 == h->nibbleTime) {
		goto biteTaken;
	}

	if (h->fish.size < 0x96) {
		if (customRandom(1, 0x14) == 1) {
			if (h->nibbleTime - h->fish.unk28 >= 0x14) {
				FISH_bobberCommand(&FISHING_DATA_PTR->swimmerLoc, 2);
			}
		}
	} else {
		if (customRandom(1, 0x14) == 1) {
			FISH_bobberCommand(&FISHING_DATA_PTR->swimmerLoc, 0xC);
		}
	}

	FISH_SCRATCH->v0.vx = FISHING_DATA_PTR->swimmerLoc.vx - h->fish.pos.vx;
	FISH_SCRATCH->v0.vy = 0;
	FISH_SCRATCH->v0.vz = FISHING_DATA_PTR->swimmerLoc.vz - h->fish.pos.vz;
	FISH_SCRATCH->unk18 = getDistance(FISH_SCRATCH->v0.vx, FISH_SCRATCH->v0.vy,
	                                  FISH_SCRATCH->v0.vz);
	if (FISH_SCRATCH->unk18 < 4) {
		goto tick;
	}

	toEulerAngles(&FISHING_DATA_PTR->hooked.fish.rotation, FISH_SCRATCH->v0.vx,
	              FISH_SCRATCH->v0.vy, FISH_SCRATCH->v0.vz);
	k = FISH_SCRATCH->unk18;
	k = (k <= 0) ? 1 : ((FISH_SCRATCH->unk18 >= 0xB) ? 0xA : k);
	FISH_SCRATCH->unk18 = k;
	FISH_SCRATCH->v0.vx = 0;
	FISH_SCRATCH->v0.vy = 0;
	FISH_SCRATCH->v0.vz = FISH_SCRATCH->unk18;
	rotateVectorYXZ(&FISHING_DATA_PTR->hooked.fish.rotation, &FISH_SCRATCH->v0,
	                &FISH_SCRATCH->v1);
	h->fish.pos.vx = h->fish.pos.vx + FISH_SCRATCH->v1.vx;
	h->fish.pos.vy = h->fish.pos.vy + FISH_SCRATCH->v1.vy;
	h->fish.pos.vz = h->fish.pos.vz + FISH_SCRATCH->v1.vz;
	goto tick;

biteTaken:
	FISH_bobberCommand(&FISHING_DATA_PTR->swimmerLoc, 0xB);
	FISHING_DATA_PTR->rod.itemTaken = 1;
	removeItem(INVENTORY.types.array[FISHING_DATA_PTR->rod.itemSlot], 1);
	h->fish.target = 0x14;
	h->fish.unk28 = -1;
	goto tick;

approaching:
	FISH_SCRATCH->v0.vx = FISHING_DATA_PTR->swimmerLoc.vx - h->fish.pos.vx;
	FISH_SCRATCH->v0.vy = 0;
	FISH_SCRATCH->v0.vz = FISHING_DATA_PTR->swimmerLoc.vz - h->fish.pos.vz;
	FISH_SCRATCH->unk18 = getDistance(FISH_SCRATCH->v0.vx, FISH_SCRATCH->v0.vy,
	                                  FISH_SCRATCH->v0.vz);
	if (FISH_SCRATCH->unk18 >= 4) {
		toEulerAngles(&FISHING_DATA_PTR->hooked.fish.rotation, FISH_SCRATCH->v0.vx,
		              FISH_SCRATCH->v0.vy, FISH_SCRATCH->v0.vz);
		k = FISH_SCRATCH->unk18;
		k = (k <= 0) ? 1 : ((FISH_SCRATCH->unk18 >= 0xB) ? 0xA : k);
		FISH_SCRATCH->unk18 = k;
		FISH_SCRATCH->v0.vx = 0;
		FISH_SCRATCH->v0.vy = 0;
		FISH_SCRATCH->v0.vz = FISH_SCRATCH->unk18;
		rotateVectorYXZ(&FISHING_DATA_PTR->hooked.fish.rotation, &FISH_SCRATCH->v0,
		                &FISH_SCRATCH->v1);
		h->fish.pos.vx = h->fish.pos.vx + FISH_SCRATCH->v1.vx;
		h->fish.pos.vy = h->fish.pos.vy + FISH_SCRATCH->v1.vy;
		h->fish.pos.vz = h->fish.pos.vz + FISH_SCRATCH->v1.vz;
	}

	if (h->fish.unk28 < h->catchingTime) {
		goto tick;
	}

	k = 1;
	goto setEscaped;

grabbed:
	k = 1;
setAnim:
	h->animMode = k;
	h->fish.target = 0x28;
	h->fish.unk28 = -1;
	goto tick;

struggling:
	if (customRandom(0, 0xA) == 0) {
		playSound(8, 5);
	}

	if (customRandom(1, 0xA) == 1) {
		toEulerAngles(&h->fish.rotation,
		              TAMER_ENTITY.entity.posData->location.vx - h->fish.pos.vx, 0,
		              TAMER_ENTITY.entity.posData->location.vz - h->fish.pos.vz);
		h->fish.rotation.vy = h->fish.rotation.vy +
		                      (int16_t)(customRandom(-0x384, 0x384) + 0x800);
	}

	if (customRandom(1, 4) == 1) {
		FISH_spawnRipple(0, &h->fish.pos, 0x2BC, h->fish.size / 2,
		                 (h->fish.size << 12) / 120);
		FISH_SCRATCH->v0.vx = 0;
		FISH_SCRATCH->v0.vy = 0;
		FISH_SCRATCH->v0.vz = h->fish.size;
		rotateVectorYXZ(&h->fish.rotation, &FISH_SCRATCH->v0, &FISH_SCRATCH->v1);
		FISH_SCRATCH->v1.vx = FISH_SCRATCH->v1.vx + h->fish.pos.vx;
		FISH_SCRATCH->v1.vy = FISH_SCRATCH->v1.vy + h->fish.pos.vy;
		FISH_SCRATCH->v1.vz = FISH_SCRATCH->v1.vz + h->fish.pos.vz;
		FISH_spawnRipple(0, &h->fish.pos, 0x2BC, 0, h->fish.size / 2);
	}

	i = 0;
	goto probeTest;
probeStep:
	step.vx = 0;
	step.vy = 0;
	step.vz = 0x12C;
	rotateVectorYXZ(&h->fish.rotation, &step, &probe);
	probe.vx = probe.vx + FISHING_DATA_PTR->swimmerLoc.vx;
	probe.vy = probe.vy + FISHING_DATA_PTR->swimmerLoc.vy;
	probe.vz = probe.vz + FISHING_DATA_PTR->swimmerLoc.vz;
	if (FISH_projectPoint(&probe, &FISH_SCRATCH->x, &FISH_SCRATCH->y) != 0) {
		if (FISH_isScreenPointVisible(FISH_SCRATCH->x, FISH_SCRATCH->y) != 0) {
			if (FISHING_DATA_PTR->mapDataPtr->unk20(FISH_SCRATCH->x,
			                                        FISH_SCRATCH->y) != 0) {
				goto probeNext;
			}
		}
	}

	FISH_randomPointInSpot(h->fish.kind, &FISH_SCRATCH->v0);
	toEulerAngles(&h->fish.rotation, FISH_SCRATCH->v0.vx - h->fish.pos.vx, 0,
	              FISH_SCRATCH->v0.vz - h->fish.pos.vz);
probeNext:
	i = i + 1;
probeTest:
	if (i < 0x1E) {
		goto probeStep;
	}

	step.vx = 0;
	step.vy = 0;
	t = (h->powerLeft <= h->powerMax / 10) ? 0xF : 8;
	step.vz = h->fish.timeThisMove / t;
	rotateVectorYXZ(&h->fish.rotation, &step, &probe);
	probe.vx = probe.vx + FISHING_DATA_PTR->swimmerLoc.vx;
	probe.vy = probe.vy + FISHING_DATA_PTR->swimmerLoc.vy;
	probe.vz = probe.vz + FISHING_DATA_PTR->swimmerLoc.vz;
	h->fish.pos = FISHING_DATA_PTR->swimmerLoc = probe;
	h->fish.pos = FISHING_DATA_PTR->swimmerLoc;
	if (FISHING_DATA_PTR->rod.pressedFrames == -1) {
		goto powerCheck;
	}

	if (FISHING_DATA_PTR->rod.pressedFrames >= 0x3D) {
		if (FISHING_DATA_PTR->rod.pressedFrames % 3 == 0) {
			goto powerCheck;
		}
	}

	if (customRandom(1, 0x14) >= 3) {
		goto stayStruggling;
	}

powerCheck:
	if (customRandom(1, 0x1000) <= (h->powerLeft << 12) / h->powerMax) {
		goto nextMove;
	}

stayStruggling:
	if (FISHING_DATA_PTR->rod.fishingState == 0xB) {
		goto tick;
	}

	k = 1;
	goto setEscaped;

nextMove:
	toEulerAngles(&h->fish.rotation,
	              TAMER_ENTITY.entity.posData->location.vx - h->fish.pos.vx, 0,
	              TAMER_ENTITY.entity.posData->location.vz - h->fish.pos.vz);
	h->fish.rotation.vy = h->fish.rotation.vy +
	                      (int16_t)(customRandom(-0x384, 0x384) + 0x800);
	h->fish.timeThisMove = customRandom(0x14, 0x32);
	h->nibbleTime = customRandom(9, 0x13);
	h->fish.target = 0x32;
	h->fish.unk28 = -1;
	goto tick;

fighting:
	t = h->fish.unk28 % 2;
	if (t == 0) {
		h->powerLeft = h->powerLeft - 1;
		h->powerLeft = (h->powerLeft < 0) ? 0
		                                  : ((h->powerMax < h->powerLeft) ? h->powerMax : h->powerLeft);
	}

	if (customRandom(0, 0xA) == 0) {
		playSound(8, 6);
	}

	FISH_spawnRipple(0, &h->fish.pos, 0x2BC, 1, (h->fish.size << 12) / 120);
	i = 0;
	goto probeTest2;
probeStep2:
	step.vx = 0;
	step.vy = 0;
	step.vz = 0x12C;
	rotateVectorYXZ(&h->fish.rotation, &step, &probe);
	probe.vx = probe.vx + FISHING_DATA_PTR->swimmerLoc.vx;
	probe.vy = probe.vy + FISHING_DATA_PTR->swimmerLoc.vy;
	probe.vz = probe.vz + FISHING_DATA_PTR->swimmerLoc.vz;
	if (FISH_projectPoint(&probe, &FISH_SCRATCH->x, &FISH_SCRATCH->y) != 0) {
		if (FISH_isScreenPointVisible(FISH_SCRATCH->x, FISH_SCRATCH->y) != 0) {
			if (FISHING_DATA_PTR->mapDataPtr->unk20(FISH_SCRATCH->x,
			                                        FISH_SCRATCH->y) != 0) {
				goto probeNext2;
			}
		}
	}

	FISH_randomPointInSpot(h->fish.kind, &FISH_SCRATCH->v0);
	toEulerAngles(&h->fish.rotation, FISH_SCRATCH->v0.vx - h->fish.pos.vx, 0,
	              FISH_SCRATCH->v0.vz - h->fish.pos.vz);
probeNext2:
	i = i + 1;
probeTest2:
	if (i < 0x1E) {
		goto probeStep2;
	}

	step.vx = 0;
	step.vy = 0;
	step.vz = h->fish.timeThisMove;
	rotateVectorYXZ(&h->fish.rotation, &step, &probe);
	probe.vx = probe.vx + FISHING_DATA_PTR->swimmerLoc.vx;
	probe.vy = probe.vy + FISHING_DATA_PTR->swimmerLoc.vy;
	probe.vz = probe.vz + FISHING_DATA_PTR->swimmerLoc.vz;
	h->fish.pos = FISHING_DATA_PTR->swimmerLoc = probe;
	if (FISHING_DATA_PTR->rod.fishingState != 0xB) {
		goto escape;
	}

	if (h->fish.unk28 >= h->nibbleTime) {
		goto grabbed;
	}

	goto tick;

escape:
	k = 1;
setEscaped:
	h->escaped = k;
	FISH_bobberCommand(&FISHING_DATA_PTR->swimmerLoc, 7);
	toEulerAngles(&h->fish.rotation,
	              TAMER_ENTITY.entity.posData->location.vx - h->fish.pos.vx, 0,
	              TAMER_ENTITY.entity.posData->location.vz - h->fish.pos.vz);
	h->fish.rotation.vy = h->fish.rotation.vy + 0x800;
	h->fish.target = 0x5A;
	h->fish.unk28 = -1;
	goto tick;

sinking:
	step.vx = 0;
	step.vy = 0;
	step.vz = 0x28;
	rotateVectorYXZ(&h->fish.rotation, &step, &probe);
	h->fish.pos.vx = h->fish.pos.vx + probe.vx;
	h->fish.pos.vy = h->fish.pos.vy + probe.vy;
	h->fish.pos.vz = h->fish.pos.vz + probe.vz;
	h->fish.transparency = h->fish.transparency - 8;
	h->fish.transparency = (h->fish.transparency < 0) ? 0
	                                                  : ((h->fish.transparency >= 0x81) ? 0x80 : h->fish.transparency);
	if (h->fish.transparency != 0) {
		goto tick;
	}

	h->fish.alive = 0;
	goto tick;

snapped:
	h->fish.target = 0x64;
	h->fish.unk28 = -1;
	goto tick;

seadraChase:
	FISH_SCRATCH->unk18 = getDistance(
		FISH_D_8007AA18[0] - FISHING_DATA_PTR->hooked.fish.pos.vx, 0,
		FISH_D_8007AA20[0] - FISHING_DATA_PTR->hooked.fish.pos.vz);
	k = FISH_SCRATCH->unk18;
	k = (k < 0) ? 0 : ((FISH_SCRATCH->unk18 >= 0x33) ? 0x32 : k);
	FISH_SCRATCH->unk18 = k;
	toEulerAngles(&h->fish.rotation, FISH_D_8007AA18[0] - h->fish.pos.vx, 0,
	              FISH_D_8007AA20[0] - h->fish.pos.vz);
	FISH_SCRATCH->v0.vx = 0;
	FISH_SCRATCH->v0.vy = 0;
	FISH_SCRATCH->v0.vz = FISH_SCRATCH->unk18;
	rotateVectorYXZ(&h->fish.rotation, &FISH_SCRATCH->v0, &FISH_SCRATCH->v1);
	h->fish.pos.vx = h->fish.pos.vx + FISH_SCRATCH->v1.vx;
	h->fish.pos.vy = h->fish.pos.vy + FISH_SCRATCH->v1.vy;
	h->fish.pos.vz = h->fish.pos.vz + FISH_SCRATCH->v1.vz;
	FISH_spawnRipple(0, &h->fish.pos, 0x2BC, 1, (h->fish.size << 12) / 120);
	if (customRandom(1, 0xA) < 6) {
		playSound(8, 6);
	}

tick:
	h->fish.unk28 = h->fish.unk28 + 1;
	h->fish.unk20 = h->fish.unk20 + 1;
	if (h->unk4C != 0) {
		h->unk4C -= 1;
	}

checkKind:
	if (h->fish.kind == 6) {
		FISH_pushSeadramonSegment(&h->fish.pos, (FishHist *)&h->fish.rotation);
	}

ret:;
}

int32_t FISH_tryStrike(void)
{
	HookedFish *h = &FISHING_DATA_PTR->hooked;

	if ((h->fish.target == 0x14) && (h->fish.unk28 < h->catchingTime / 2)) {
		h->unk4C = 0x14;
		FISH_tickHookedFish(0x10);
		return 1;
	}

	FISH_tickHookedFish(0xF);

	return 0;
}

void FISH_drawHookedFish(void)
{
	HookedFish *h = &FISHING_DATA_PTR->hooked;

	if (h->fish.alive) {
		FISH_drawFishSprite(h->fish.kind, &h->fish.pos, &h->fish.rotation, h->fish.size,
		                    h->fish.unk20, h->fish.transparency, (h->fish.target < 0x1F) ? 0 : 1);
	}
}

INCLUDE_ASM("asm/fish/nonmatchings/fish", FISH_updateRodPose);

void FISH_drawRodSegments(FishingRod *rod)
{
	GsDOBJ2 *obj;
	int32_t i;

	for (i = 0; i < 5U; i++) {
		GsMulCoord0(&GsWSMATRIX, &FISH_D_8007AB68[i], &FISH_SCRATCH->m[0]);
		if ((FISH_SCRATCH->m[0].t[2] <= 0) ||
		    (FISH_SCRATCH->m[0].t[2] >= 0x10000)) {
			FISH_SCRATCH->flag = 1;
		} else {
			if (i < 4) {
				obj = FISH_getModelObject(&FISHING_DATA_PTR->rodModel, i);
				obj->coord2 = NULL;
				obj->attribute = 0;
				FISH_SCRATCH->v0.vx = 0x1000;
				FISH_SCRATCH->v0.vy = 0x1000;
				FISH_SCRATCH->v0.vz = 0x1000;
				ScaleMatrix(&FISH_SCRATCH->m[0], &FISH_SCRATCH->v0);
				GsSetLsMatrix(&FISH_SCRATCH->m[0]);
				GsSetLightMatrix(&FISH_D_8007AB68[i]);
				GsSortObject4(obj, ACTIVE_ORDERING_TABLE, 2, FISH_SCRATCH->ot);
			}
			FISH_SCRATCH->cur.vx = (FISH_SCRATCH->m[0].t[0] * VIEWPORT_DISTANCE) /
			                       FISH_SCRATCH->m[0].t[2];
			FISH_SCRATCH->cur.vy = (FISH_SCRATCH->m[0].t[1] * VIEWPORT_DISTANCE) /
			                       FISH_SCRATCH->m[0].t[2];
			FISH_SCRATCH->cur.vz = FISH_SCRATCH->m[0].t[2];
			FISH_SCRATCH->flag = 0;
		}
		FISH_SCRATCH->unk40 = FISH_SCRATCH->flag;
		FISH_SCRATCH->prev = FISH_SCRATCH->cur;
	}
}

int32_t FISH_clipLineToRect(SVECTOR *pts, int32_t xMin, int32_t xMax, int32_t yMin, int32_t yMax)
{
	int32_t code[2];
	int32_t i;

	for (i = 0; i < 2; i++) {
		code[i] = ((pts[i].vx < xMin) ? 1 : 0) | ((xMax < pts[i].vx) ? 2 : 0);
	}

	if ((code[0] & code[1]) != 0) {
		return 0;
	}

	for (i = 0; i < 2; i++) {
		if ((code[i] & 1) != 0) {
			pts[i].vy = pts[1 - i].vy + (xMin - pts[1 - i].vx) *
			                                    (pts[i].vy - pts[1 - i].vy) / (pts[i].vx - pts[1 - i].vx);
			pts[i].vx = xMin;
		}
		if ((code[i] & 2) != 0) {
			pts[i].vy = pts[1 - i].vy + (xMax - pts[1 - i].vx) *
			                                    (pts[i].vy - pts[1 - i].vy) / (pts[i].vx - pts[1 - i].vx);
			pts[i].vx = xMax;
		}
	}

	for (i = 0; i < 2; i++) {
		code[i] = ((pts[i].vy < yMin) ? 1 : 0) | ((yMax < pts[i].vy) ? 2 : 0);
	}

	if ((code[0] & code[1]) != 0) {
		return 0;
	}

	for (i = 0; i < 2; i++) {
		if ((code[i] & 1) != 0) {
			pts[i].vx = pts[1 - i].vx + (yMin - pts[1 - i].vy) *
			                                    (pts[i].vx - pts[1 - i].vx) / (pts[i].vy - pts[1 - i].vy);
			pts[i].vy = yMin;
		}
		if ((code[i] & 2) != 0) {
			pts[i].vx = pts[1 - i].vx + (yMax - pts[1 - i].vy) *
			                                    (pts[i].vx - pts[1 - i].vx) / (pts[i].vy - pts[1 - i].vy);
			pts[i].vy = yMax;
		}
	}

	return 1;
}

void FISH_drawFishingLine(FishingRod *rod)
{
	VECTOR *ends[2];
	SVECTOR pts[2];
	VECTOR cur;
	VECTOR prev;
	int32_t i;
	int32_t j;
	int32_t n;
	int32_t k;
	int32_t lo;

	ends[0] = &rod->lineStart;
	ends[1] = (VECTOR *)&FISHING_DATA_PTR->swimmerLoc;

	for (i = 0; i < 2; i++) {
		FISH_SCRATCH->m[1].t[0] = ends[i]->vx;
		FISH_SCRATCH->m[1].t[1] = ends[i]->vy;
		FISH_SCRATCH->m[1].t[2] = ends[i]->vz;
		GsMulCoord0(&GsWSMATRIX, &FISH_SCRATCH->m[1], &FISH_SCRATCH->m[0]);
		if ((FISH_SCRATCH->m[0].t[2] <= 0) ||
		    (FISH_SCRATCH->m[0].t[2] >= 0x10000)) {
			pts[i].vz = -1;
		} else {
			pts[i].vx = (FISH_SCRATCH->m[0].t[0] * VIEWPORT_DISTANCE) /
			            FISH_SCRATCH->m[0].t[2];
			pts[i].vy = (FISH_SCRATCH->m[0].t[1] * VIEWPORT_DISTANCE) /
			            FISH_SCRATCH->m[0].t[2];
			pts[i].vz = FISH_SCRATCH->m[0].t[2];
		}
	}

	if (pts[0].vz <= 0) {
		return;
	}

	if (pts[1].vz <= 0) {
		return;
	}

	lo = 0;
	if (FISH_clipLineToRect(pts, -DRAWING_OFFSET_X + lo, -DRAWING_OFFSET_X + 0x140,
	                        -DRAWING_OFFSET_Y + lo, -DRAWING_OFFSET_Y + 0xF0) == 0) {
		return;
	}

	n = (getDistance(pts[1].vx - pts[0].vx, pts[1].vy - pts[0].vy, 0) + 0x40) / 0x40;
	for (j = 0; j <= n; j++) {
		cur.vx = (pts[0].vx * (n - j)) / n + (pts[1].vx * j) / n;
		cur.vy = (pts[0].vy * (n - j)) / n + (pts[1].vy * j) / n;
		cur.vz = (pts[0].vz * (n - j)) / n + (pts[1].vz * j) / n;
		if (j > 0) {
			renderLinePrimitive(*(uint32_t *)&FISHING_DATA_PTR->tension.color, cur.vx, cur.vy,
			                    prev.vx, prev.vy,
			                    ((cur.vz > prev.vz) ? cur.vz : prev.vz) >> 4, 0);
		}
		prev = cur;
	}
}

void FISH_resetRodState(FishingRod *rod)
{
	rod->fishingState = 0;
	rod->displayRod = 0;
	rod->displayLine = 0;
	rod->displaySwimmer = 0;
}

INCLUDE_ASM("asm/fish/nonmatchings/fish", FISH_tickRod);

void FISH_tickFishingWorld(int32_t instanceId)
{
	if (FISHING_DATA_PTR->fishingEnabled != 0) {
		return;
	}

	if (PARTNER_STATE == 0xD) {
		return;
	}

	FISH_updateRodPose(&FISHING_DATA_PTR->rod);

	if (FISH_isTextBoxVisible() != 0) {
		return;
	}

	FISH_tickRipples();
	FISH_tickMarkers();
	FISH_tickFishPopulation();
	FISH_tickHookedFish(0);
}

void FISH_renderFishingWorld(int32_t instanceId)
{
	if (FISHING_DATA_PTR->fishingEnabled != 0) {
		return;
	}

	if (PARTNER_STATE == 0xD) {
		return;
	}

	FISH_drawFishPool();
	FISH_drawHookedFish();
	FISH_drawStub();
	FISH_drawRipples();
	FISH_drawMarkers();

	if (FISHING_DATA_PTR->rod.displayRod == 0) {
		return;
	}

	if (FISHING_DATA_PTR->rod.displaySwimmer != 0) {
		FISH_drawRodSegments(&FISHING_DATA_PTR->rod);
		FISH_bobberCommand(&FISHING_DATA_PTR->swimmerLoc, 1);
	}

	if (FISHING_DATA_PTR->rod.displayLine != 0) {
		FISH_drawFishingLine(&FISHING_DATA_PTR->rod);
	}

	FISH_drawTensionGauge();
	FISH_drawBaitSprite();
}

int32_t FISH_lerpTableByX(int32_t x, int32_t base, DVECTOR *tbl, int32_t n)
{
	if ((x < tbl[0].vx) || (tbl[n - 1].vx < x)) {
		return 0;
	}
	for (n = n - 2; n >= 0; n--) {
		if (x >= tbl[n].vx) {
			break;
		}
	}

	{
		int32_t y0 = tbl[n].vy;
		int32_t x0 = tbl[n].vx;
		return base - (y0 + (((x - tbl[n].vx) * (tbl[n + 1].vy - y0)) / (tbl[n + 1].vx - x0)));
	}
}

int32_t FISH_lerpTableByY(int32_t base, int32_t y, DVECTOR *tbl, int32_t n)
{
	if ((y < tbl[0].vy) || (tbl[n - 1].vy < y)) {
		return 0;
	}
	for (n = n - 2; n >= 0; n--) {
		if (y >= tbl[n].vy) {
			break;
		}
	}

	{
		int32_t x0 = tbl[n].vx;
		int32_t y0 = tbl[n].vy;
		return base - (x0 + (((y - tbl[n].vy) * (tbl[n + 1].vx - x0)) / (tbl[n + 1].vy - y0)));
	}
}

int32_t FISH_isFishableWater1(int32_t a, int32_t y)
{
	if ((-DRAW_OFFSET_LIMIT_Y_MIN + 0xF0) < y) {
		return 0;
	}

	return FISH_lerpTableByX(a, y, FISH_D_8007A890, 0x10) >= 0;
}

void FISH_waterTestStub1(void)
{
}

int32_t FISH_isFishableWater2(int32_t x, int32_t y)
{
	if (x >= 0x1AD) {
		return 0;
	}

	if (y < 6) {
		return 1;
	}

	return FISH_lerpTableByY(x, y, FISH_D_8007A9B0, 7) <= 0;
}

void FISH_waterTestStub2(void)
{
}

void FISH_getBoneTransform(Entity *e, int32_t bone, VECTOR *outPos, SVECTOR *outRot)
{
	MATRIX m;
	long *t;

	if (!(bone < DIGIMON_DATA[e->type].boneCount)) {
		bone = 0;
	}

	RotMatrix(&e->posData->rotation, &e->posData->posMatrix.coord);
	ScaleMatrix(&e->posData->posMatrix.coord, &e->posData->scale);
	TransMatrix(&e->posData->posMatrix.coord, &e->posData->location);

	calculatePosition(&e->posData[bone].posMatrix, &m);

	if (outRot != NULL) {
		matrixToEuler2(&m, outRot);
	}

	if (outPos != NULL) {
		t = m.t;
		outPos->vx = t[0];
		outPos->vy = t[1];
		outPos->vz = t[2];
	}
}

void FISH_drawItemSprite(int32_t itemId, int32_t x, int32_t y, int32_t depth, int32_t e)
{
	renderItemSprite(itemId, x, y, depth);
}

void FISH_endFishing(FishingRod *rod)
{
	if (rod->displayRod != 0) {
		FISH_applySavedCameraScroll();
		setCameraFollowPlayer();
		startAnimation(&TAMER_ENTITY.entity, 0);
		FISH_unloadModel(&FISHING_DATA_PTR->rodModel);
		rod->fishingState = 0;
		rod->displayLine = 0;
		rod->displayRod = 0;
	}

	FISH_closeBaitMenuIfOpen();
	FISH_shutdownTextBox();
	FISH_resetTension();
	FISH_initBaitSprite();
	FISH_initBobberAnim((FishRipple *)&FISHING_DATA_PTR->swimmerLoc);
}

void FISH_loadFishing(FishingRod *rod)
{
	RECT rect;
	int32_t t;

	if (rod->displayRod != 0) {
		FISH_endFishing(rod);
	}

	unsetCameraFollowPlayer();
	FISH_enableCameraControl();
	rod->fishingState = 2;
	rod->displayRod = 1;
	rod->displaySwimmer = 0;
	rod->displayLine = 0;
	FISH_clearBobberAnim((FishRipple *)&FISHING_DATA_PTR->swimmerLoc);
	FISH_tickHookedFish(0xF);
	FISH_loadTmdModel(&FISHING_DATA_PTR->rodModel, FISH_D_8007A5E4,
	                  FISHING_DATA_PTR->rodModelBuffer, 0x1000);
	rod->swimmerType = getBestFishingRod();
	t = rod->swimmerType;
	if (t != 1) {
		if (t != 2) {
			rod->swimmerType = 2;
		}
		rect.x = 0x1C0;
		rect.y = 0x1E4;
		rect.w = 0x1C;
		rect.h = 8;
		MoveImage(&rect, 0x1C0, 0x1DC);
	}
}

void FISH_init(void)
{
	FISHING_DATA_PTR->fishingEnabled = 0;
	FISHING_DATA_PTR->unk2D08 = 0;

	FISH_loadSpritesAndSeadramon(&FISHING_DATA_PTR->rodModel);
	FISH_initBaitMenu();
	FISH_initTextBox();
	FISH_resetTension();
	FISH_hideBaitSprite();
	FISH_resetBobberAnim((FishRipple *)&FISHING_DATA_PTR->swimmerLoc);
	FISH_initFishPool();
	FISH_resetRodState(&FISHING_DATA_PTR->rod);
	FISH_clearHookedFish();
	FISH_initRipplePool();
	FISH_initMarkerPool();

	addObject(0x3001, 0, (TickFunction)FISH_tickFishingWorld, (RenderFunction)FISH_renderFishingWorld);
}

void FISH_deinitialize(void)
{
	FISHING_DATA_PTR->fishingEnabled = 0;

	removeObject(0x3001, 0);

	FISH_endFishing(&FISHING_DATA_PTR->rod);
	FISH_initRipplePool();
	FISH_initMarkerPool();
	FISH_clearHookedFish();
	FISH_fishPoolStub();
	FISH_onRodModelLoaded(&FISHING_DATA_PTR->rodModel);
}

int32_t FISH_tamerTick(void)
{
	FISH_tickRod(&FISHING_DATA_PTR->rod);

	if (isFishing() == 0) {
		setTamerState(0);
		startGameTime();
		return 0;
	}

	if (FISHING_DATA_PTR->unk2D08 != 0) {
		FISH_tickHookedFish(0x11);
		FISH_endFishing(&FISHING_DATA_PTR->rod);
		setTamerState(0);
		return 1;
	}

	return 0;
}

void FISH_scrollCameraTo(int32_t x, int32_t y)
{
	int32_t dx;
	int32_t dy;
	int32_t near;

	if (MAIN_D_801351E0 == 0) {
		return;
	}

	x = (x < -DRAW_OFFSET_LIMIT_X_MAX + 0xA0)
	            ? -DRAW_OFFSET_LIMIT_X_MAX + 0xA0
	            : ((-DRAW_OFFSET_LIMIT_X_MIN + 0xA0 < x) ? -DRAW_OFFSET_LIMIT_X_MIN + 0xA0 : x);
	y = (y < -DRAW_OFFSET_LIMIT_Y_MAX + 0x78)
	            ? -DRAW_OFFSET_LIMIT_Y_MAX + 0x78
	            : ((-DRAW_OFFSET_LIMIT_Y_MIN + 0x78 < y) ? -DRAW_OFFSET_LIMIT_Y_MIN + 0x78 : y);

	x = x - (-DRAWING_OFFSET_X + 0xA0);
	y = y - (-DRAWING_OFFSET_Y + 0x78);
	DRAWING_OFFSET_X = DRAWING_OFFSET_X - x;
	DRAWING_OFFSET_Y = DRAWING_OFFSET_Y - y;
	CAMERA_X[0] = CAMERA_X[0] + (int16_t)x;
	CAMERA_Y[0] = CAMERA_Y[0] + (int16_t)y;

	near = 0;
	if (((x > 0) ? x : -x) < 0x7F) {
		if (((y > 0) ? y : -y) < 0x7F) {
			near = 1;
		}
	}

	handleTileUpdate(((x < 0) ? 0x8000U : 0) |
	                         ((x > 0) ? 0x2000 : 0) |
	                         ((y < 0) ? 0x1000 : 0) | ((y > 0) ? 0x4000 : 0),
	                 (near != 0) ? 0 : 1);
}

void FISH_applySavedCameraScroll(void)
{
	if (MAIN_D_801351DC[2] != 0) {
		MAIN_D_801351DC[2] = 0;
		FISH_scrollCameraTo(MAIN_D_801351DC[0], MAIN_D_801351DC[1]);
	}
}

void FISH_enableCameraControl(void)
{
	FISH_applySavedCameraScroll();
	MAIN_D_801351E0 = 1;
	MAIN_D_801351DC[0] = -DRAWING_OFFSET_X + 0xA0;
	MAIN_D_801351DE = -DRAWING_OFFSET_Y + 0x78;
}

void FISH_unloadModel(TMDFileLoadingData *m)
{
}

#pragma optimize_for_size off
void FISH_loadTmdModel(TMDFileLoadingData *m, char *path, uint8_t *buf, uint32_t bufSize)
{
	uint8_t *end;
	GsDOBJ2 *obj;
	int32_t size;
	int32_t i;

	size = ((int32_t)lookupFileSize(path) + 3) & ~3;
	if (bufSize < (uint32_t)((size + 0x7FF) & ~0x7FF)) {
		exit(1);
	}

	m->targetBuffer = buf;
	readFile(path, m->targetBuffer);

	m->objCount = ((uint32_t *)m->targetBuffer)[2];
	m->bufferEnd = buf + size;

	if (bufSize < (uint32_t)size + (uint32_t)m->objCount * 16) {
		exit(1);
	}

	m->unk8 = m->bufferEnd + m->objCount * 16;

	if (m->bufferEnd == NULL) {
		exit(1);
	}

	GsMapModelingData((u_long *)(m->targetBuffer + 4));

	end = m->targetBuffer + 0xC;
	for (i = 0; i < m->objCount; i++) {
		GsLinkObject4((u_long)end, obj = (GsDOBJ2 *)(m->bufferEnd + i * 16), i);
		obj->attribute = 0;
	}
}

GsDOBJ2 *FISH_getModelObject(TMDFileLoadingData *m, int32_t idx)
{
	if ((m == NULL) || (idx < 0) || (idx >= m->objCount)) {
		exit(1);
	}

	return &((GsDOBJ2 *)m->bufferEnd)[idx];
}

int32_t FISH_remapClamped(int32_t x, int32_t x0, int32_t x1, int32_t y0, int32_t y1)
{
	int32_t t;
	int32_t d;

	if (x1 == x0) {
		return y0;
	}

	if (x1 < x0) {
		t = x0;
		x0 = x1;
		x1 = t;
	}

	x = (x < x0) ? x0 : ((x1 < x) ? x1 : x);

	d = x1 - x0;

	return y0 + ((y1 - y0) * (x - x0) + d / 2) / d;
}
