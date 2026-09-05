#include <stdio.h>

#include <libetc.h>
#include <mwinline_n.h>

#include <dw/btl.h>
#include <dw/combat.h>
#include <dw/ui.h>
#include <dw/utils.h>

void damageTick(FighterData* fighter, Stats* stats);
void sortItemsById(uint8_t *data, int32_t count);
void initStringFT4(POLY_FT4* poly);
void renderNumber(int32_t color, int32_t x, int16_t y, int32_t n,
		  int32_t value, int32_t layer);
void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount,
			  int32_t *digits);
void pauseFrame(void);
void setEntityTextDigit(POLY_FT4* poly, int32_t x, int32_t y);
void setPosDataPolyFT4(POLY_FT4 *prim, int16_t posX, int16_t posY, int16_t width, int16_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int16_t xPos, int16_t yPos, int16_t width, int16_t height);

extern char MAIN_D_8012B94C[];
extern int32_t ACTIVE_FRAMEBUFFER;
extern GsOT GS_ORDERING_TABLE[];
extern PACKET GS_WORK_BASES[];
extern char DRAW_OFFSETS[];
extern RGB8 TEXT_COLORS[];

void pauseFrame(void)
{
	ACTIVE_FRAMEBUFFER = GsGetActiveBuff();
	ACTIVE_ORDERING_TABLE = &GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER];
	GsSetWorkBase(&GS_WORK_BASES[ACTIVE_FRAMEBUFFER * 0x14000]);
	GsClearOt(0, 0, ACTIVE_ORDERING_TABLE);
	tickObjects();
	renderObjects();
	AddPrim((char *)ACTIVE_ORDERING_TABLE->org + 0x80,
		&DRAW_OFFSETS[ACTIVE_FRAMEBUFFER * 0xC]);
	DrawSync(0);
	VSync(3);
	GsSetOrign(DRAWING_OFFSET_X, DRAWING_OFFSET_Y);
	GsSwapDispBuff();
	GsSortClear(0, 0, 0, ACTIVE_ORDERING_TABLE);
	GsDrawOt(ACTIVE_ORDERING_TABLE);
}

void damageTick(FighterData* fighter, Stats* stats)
{
	if (fighter->hpDamageBuffer > 999) {
		stats->current.currentHP -= 900;
		fighter->hpDamageBuffer -= 900;
	}

	if (fighter->hpDamageBuffer > 99) {
		stats->current.currentHP -= 80;
		fighter->hpDamageBuffer -= 80;
	}

	if (fighter->hpDamageBuffer > 9) {
		stats->current.currentHP -= 6;
		fighter->hpDamageBuffer -= 6;
	}

	if (fighter->hpDamageBuffer > 0) {
		stats->current.currentHP -= 1;
		fighter->hpDamageBuffer -= 1;
	}

	if (stats->current.currentHP <= 0) {
		stats->current.currentHP = 0;
		fighter->hpDamageBuffer = 0;
	}
}

void sortItemsById(uint8_t *data, int32_t count)
{
	int32_t i;
	int32_t j;
	int16_t minIdx;
	uint8_t minVal;
	uint8_t *dst;

	for (i = 0, dst = data; i < count; ++i, ++dst) {
		minVal = data[i];
		minIdx = i;

		for (j = i; j < count; ++j) {
			if (data[j] < minVal) {
				minIdx = j;
				minVal = data[j];
			}
		}

		swapByte(dst, &data[minIdx]);
	}
}

void swapByte(uint8_t *a, uint8_t *b)
{
	uint8_t tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void swapShort(int16_t *a, int16_t *b)
{
	int16_t tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void swapInt(int32_t *a, int32_t *b)
{
	int32_t tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void getEntityScreenPos(Entity *e, int32_t boneId, int16_t *out)
{
	MATRIX *w;
	SVECTOR v;
	int16_t ox;

	GsSetLsMatrix(&GsWSMATRIX);
	w = &e->posData[boneId].posMatrix.workm;
	v.vx = w->t[0];
	v.vy = w->t[1];
	v.vz = w->t[2];
	gte_ldv0(&v);
	gte_rtps();
	gte_stsxy((long *)out);
	ox = 0xA0 - DRAWING_OFFSET_X;
	out[0] = out[0] - ox;
	out[1] -= 0x78 - DRAWING_OFFSET_Y;
}

void setEntityTextDigit(POLY_FT4* poly, int32_t x, int32_t y)
{
	SetPolyFT4(poly);
	poly->tpage = 0x1E;
	setClut(poly, x, y);
}

void initStringFT4(POLY_FT4* poly)
{
	SetPolyFT4(poly);
	poly->tpage = 0x1B;
	setClut(poly, 0xD0, 0x1E8);
}

void renderNumber(int32_t color, int32_t x, int16_t y, int32_t n,
		  int32_t value, int32_t layer)
{
	POLY_FT4 *prim;
	int32_t i;
	int32_t count;
	int32_t buf[6];

	prim = (POLY_FT4 *)GsGetWorkBase();

	convertValueToDigits(n, value, &count, buf);

	for (i = count - 1; i >= 0; i--) {
		initStringFT4(prim);
		setRGB0(prim, TEXT_COLORS[color].r, TEXT_COLORS[color].g,
			TEXT_COLORS[color].g);
		setUVDataPolyFT4(prim, buf[i] * 8, 0xf0, 8, 12);
		setPosDataPolyFT4(prim, x + (((n - 1) - i) * 8), y, 8, 12);
		AddPrim(ACTIVE_ORDERING_TABLE->org + layer, prim++);
	}

	GsSetWorkBase((PACKET *)prim);
}

void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount,
			  int32_t *digits)
{
	char buf[8];
	int32_t i;
	int32_t j;
	long long nv;
	int32_t off;
	char *base;
	int32_t cnt;

	sprintf(buf, &MAIN_D_8012B94C[(j = n - 1) * 5], value);
	i = 0;
	off = 0;
	n = cnt = n;
	base = (char *)(digits + n) - 4;
	for (; i < n; i++) {
		*(int32_t *)(base - off) = buf[i] - '0';
		off += 4;
	}
	nv = j;
	*outCount = cnt;
	for (i = nv; i >= 0; i--) {
		if (digits[i] != 0) {
			break;
		}
		if (i != 0) {
			(*outCount)--;
		}
	}
}

void setUVDataPolyFT4(POLY_FT4 *p, int16_t u, int16_t v, int16_t w, int16_t h)
{
	setUVWH(p, u, v, w, h);
}

void setPosDataPolyFT4(POLY_FT4 *p, int16_t x, int16_t y, int16_t w, int16_t h)
{
	setXYWH(p, x, y, w, h);
}
