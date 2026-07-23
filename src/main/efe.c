#include <libgte.h>
#include <libgs.h>
#include <dw/entity.h>
#include <dw/params.h>
#include <dw/types.h>
#include <libcd.h>

extern int16_t MAIN_D_80138AA4[];
extern int16_t MAIN_D_801389B4[];
extern char *EFE_FLASH_DATA;
extern uint32_t MAIN_D_8012343C[];
extern char MAIN_D_8012342C[];
extern char MAIN_D_80134220[4];
extern int16_t EFE_LOADED_MOVE_DATA[];
extern char EFE_SCRIPT_MEM1_DATA;
extern char *EFE_DATA_STACK;
extern u_long SOME_IMAGE_DATA[];
void setShortWithStride();
void removeObject(int32_t objectId, int32_t instanceId);
char *strcpy(char *dst, char *src);
char *strcat(char *dst, char *src);
int32_t rand(void);
extern uint8_t MAIN_D_801387B8[];
typedef struct {
	int16_t v;
	int16_t pad1;
	int16_t pad2;
} EfeParticleField;
extern EfeParticleField MAIN_D_80138888[];
extern EfeParticleField MAIN_D_8013888A[];
extern EfeParticleField MAIN_D_8013888C[];
int32_t _sin(int32_t a);
extern int32_t FLASH_INSTANCE;
extern GsOT *ACTIVE_ORDERING_TABLE;
int32_t abs(int16_t value);
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
void getDrawingOffsetCopy(int32_t *x, int32_t *y);
int32_t isTamerOnScreen(void);
extern uint8_t MAIN_D_8013421C[4];
extern int32_t VIEWPORT_DISTANCE;
int32_t lerp(int16_t a, int16_t b, int32_t c, int16_t d, int32_t e);
void renderParticleFlash();
typedef struct {
	int16_t x;
	int16_t y;
	int16_t depth;
	uint32_t size;
	int16_t w;
	int16_t h;
	int16_t tpage;
	uint8_t clut;
	int8_t attr;
	int16_t clut2;
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} FlashParams;
int32_t _cos(int32_t a);
extern GsSPRITE CLOUD_FX_SPRITE;
extern int8_t MAIN_D_801233CC[];
extern int16_t MAIN_D_801233DC[];
extern int16_t MAIN_D_80123400[];
extern uint8_t MAIN_D_8012341C[];
int32_t worldPosToScreenPos(int16_t *world, int16_t *screen);
void renderSprite(GsSPRITE *spr, int16_t x, int16_t y, int32_t depth,
		  int32_t sx, int32_t sy);
typedef void (*EfeFn)(int32_t);
int32_t addObject(int32_t objectId, int32_t instanceId, EfeFn tick, EfeFn render);

#include "common.h"

void initializeParticleFX();
void createParticleFX();
void tickParticleFX(int32_t id);
void renderParticleFX();
int32_t addEntityParticleFX(int32_t *typePtr, int16_t timer);
void initializeEntityParticleFX();
void tickEntityParticleFX(int32_t id);
void renderEntityParticleFX(int32_t id);
void removeEntityParticleFX();
void initializeCloudFXData();
void removeAllCloudFX();
void createCloudFX(int32_t *pos);
void tickCloudFX();
void renderCloudFX(int32_t id);
void rotateVector();
char *initializeFlashData(char *base);
static inline int16_t *efe_s16ptr(char *arg0)
{
	return (int16_t *)arg0;
}
void createFlash();
void tickEFEFlash();
void renderEFEFlash(int32_t layer);
int32_t setEFEFlashOffset(int32_t id, int16_t x, int16_t y);
void downloadSomeImage();
void modifySomeImage(int32_t dim, int16_t stp);
void findEFEDATFile(void);
void initializeEFE();
void getEFEDATEntry();
void renderParticleFlash();

void *efe_order_anchor[] = {
	renderParticleFlash,
	getEFEDATEntry,
	initializeEFE,
	findEFEDATFile,
	modifySomeImage,
	downloadSomeImage,
	setEFEFlashOffset,
	renderEFEFlash,
	tickEFEFlash,
	createFlash,
	initializeFlashData,
	rotateVector,
	renderCloudFX,
	tickCloudFX,
	createCloudFX,
	removeAllCloudFX,
	initializeCloudFXData,
	removeEntityParticleFX,
	renderEntityParticleFX,
	tickEntityParticleFX,
	initializeEntityParticleFX,
	addEntityParticleFX,
	renderParticleFX,
	tickParticleFX,
	createParticleFX,
	initializeParticleFX,
};

void initializeParticleFX(void)
{
	int32_t i;
	SVECTOR ang;
	SVECTOR vec;
	MATRIX m;

	setShortWithStride(MAIN_D_801387B8, -1, 4, 0x34);
	for (i = 0; i < 0x32; i++) {
		ang.vx = (((rand() & 0x7F) - 0x40) << 12) / 64;
		ang.vy = (((rand() & 0x7F) - 0x40) << 12) / 64;
		ang.vz = (((rand() & 0x7F) - 0x40) << 12) / 64;
		vec.vy = 0;
		vec.vx = 0;
		vec.vz = ((rand() % 150) + 0xFA) * 0x10;
		RotMatrixZYX(&ang, &m);
		ApplyMatrixSV(&m, &vec, &vec);
		MAIN_D_80138888[i].v = vec.vx;
		MAIN_D_8013888A[i].v = vec.vy;
		MAIN_D_8013888C[i].v = vec.vz;
	}
}

INCLUDE_ASM("asm/main/nonmatchings/efe", createParticleFX);

INCLUDE_ASM("asm/main/nonmatchings/efe", tickParticleFX);

INCLUDE_ASM("asm/main/nonmatchings/efe", renderParticleFX);

INCLUDE_ASM("asm/main/nonmatchings/efe", addEntityParticleFX);

void initializeEntityParticleFX(void)
{
	setShortWithStride(MAIN_D_801389B4, -1, 0x14, 0xC);
}

void tickEntityParticleFX(int32_t id)
{
	char *e;
	int32_t bc;

	e = (char *)&MAIN_D_801389B4[id * 6];
	if (*(int16_t *)e <= 0) {
		removeEntityParticleFX(id);
		return;
	}
	if ((*(int16_t *)e % 4) == 0) {
		bc = DIGIMON_DATA[**(int32_t **)(e + 4)].boneCount - 1;
		*(int16_t *)(e + 2) = (rand() % bc) + 1;
		*(int16_t *)(e + 8) = rand() % 9;
	}
	*(int16_t *)e = *(int16_t *)e - 1;
}

INCLUDE_ASM("asm/main/nonmatchings/efe", renderEntityParticleFX);

void removeEntityParticleFX(int32_t id)
{
	MAIN_D_801389B4[id * 6] = -1;
	removeObject(0x502, id);
}

void initializeCloudFXData(void)
{
	setShortWithStride(MAIN_D_80138AA4, -1, 0x3C, 6);
}

INCLUDE_ASM("asm/main/nonmatchings/efe", removeAllCloudFX);

INCLUDE_ASM("asm/main/nonmatchings/efe", createCloudFX);

void tickCloudFX(int32_t id)
{
	int16_t *p;

	p = &MAIN_D_80138AA4[id * 3];
	*p += 1;
	if (*p >= 0xE) {
		*p = -1;
		removeObject(0x601, id);
	}
}

INCLUDE_ASM("asm/main/nonmatchings/efe", renderCloudFX);

INCLUDE_ASM("asm/main/nonmatchings/efe", rotateVector);

char *initializeFlashData(char *base)
{
	int32_t i;
	char *new_var;
	int32_t ofs;

	EFE_FLASH_DATA = base;
	i = 0;
	ofs = 0;
	while (i < 0xC) {
		new_var = EFE_FLASH_DATA;
		*efe_s16ptr((ofs + new_var) + 8) = -1;
		i += 1;
		ofs += 0x28;
		do { } while (0);
	}
	return base + 0x1E0;
}

INCLUDE_ASM("asm/main/nonmatchings/efe", createFlash);

void tickEFEFlash(int32_t id)
{
	char *e;

	e = EFE_FLASH_DATA + (id * 0x28);
	((int16_t *)e)[4] = ((int16_t *)e)[4] + 1;
	if (((int16_t *)e)[4] >= ((int16_t *)e)[5]) {
		((int16_t *)e)[4] = -1;
		removeObject(0x602, id);
	}
}

INCLUDE_ASM("asm/main/nonmatchings/efe", renderEFEFlash);

int32_t setEFEFlashOffset(int32_t id, int16_t x, int16_t y)
{
	int16_t *e;

	/* The self-assignment is load-bearing: without it the compiler keeps id
	   in a different register and the schedule shifts. */
	id = id;
	e = (int16_t *)(EFE_FLASH_DATA + (id * 0x28));
	e[18] = x;
	e[19] = y;
}

void downloadSomeImage(void)
{
	RECT r;

	r.x = 0x200;
	r.y = 0xF8;
	r.w = 0x100;
	r.h = 7;
	StoreImage(&r, SOME_IMAGE_DATA);
	DrawSync(0);
}

INCLUDE_ASM("asm/main/nonmatchings/efe", modifySomeImage);

void findEFEDATFile(void)
{
	char path[64];
	CdlFILE file;
	uint8_t mode;
	int32_t i;

	while (CdReadSync(1, NULL) != 0) {
	}

	mode = 0x80;
	path[0] = '\\\\';
	strcpy(&path[1], MAIN_D_8012342C);
	strcat(path, MAIN_D_80134220);

	while (CdSearchFile(&file, path) == (CdlFILE *)-1) {
	}

	CdControl(0xE, &mode, NULL);
	i = 0;
	MAIN_D_8012343C[i] = CdPosToInt(&file.pos);
}

INCLUDE_ASM("asm/main/nonmatchings/efe", initializeEFE);

void getEFEDATEntry(int32_t id)
{
	CdlLOC loc;

	id -= 0x100;
	CdIntToPos(MAIN_D_8012343C[0] + (id * 0xA), &loc);
}

void renderParticleFlash(FlashParams *params)
{
	POLY_FT4 *prim;
	GsOT_TAG *ot;
	int32_t r;
	int32_t g;
	int32_t b;
	int32_t alpha;
	int32_t cw;
	int32_t ch;
	int32_t sz;
	int32_t halfW;
	int32_t halfH;
	int32_t cols;
	int32_t rows;
	int32_t colStep;
	int32_t rowStep;
	int32_t uStep;
	int32_t vStep;
	int32_t t;
	int32_t row1;
	int32_t col1;
	int32_t x01;
	int32_t x11;
	int32_t y01;
	int32_t y11;
	int32_t rowY1;
	int32_t rowV1;
	int32_t colX1;
	int32_t colU1;
	int32_t uMul1;
	int32_t v0base1;
	int32_t v2base1;
	int32_t row2;
	int32_t col2;
	int32_t x02;
	int32_t x12;
	int32_t y02;
	int32_t y12;
	int32_t rowY2;
	int32_t rowV2;
	int32_t colX2;
	int32_t colU2;
	int32_t uMul2;
	int32_t v0base2;
	int32_t v2base2;
	int32_t xNeg2;
	int32_t row3;
	int32_t col3;
	int32_t x03;
	int32_t x13;
	int32_t y03;
	int32_t y13;
	int32_t rowY3;
	int32_t rowV3;
	int32_t colX3;
	int32_t colU3;
	int32_t uMul3;
	int32_t v0base3;
	int32_t v2base3;
	int32_t yNeg3;
	int32_t row4;
	int32_t col4;
	int32_t x04;
	int32_t x14;
	int32_t y04;
	int32_t y14;
	int32_t rowY4;
	int32_t rowV4;
	int32_t colX4;
	int32_t colU4;
	int32_t uMul4;
	int32_t v0base4;
	int32_t v2base4;
	int32_t xNeg4;
	int32_t yNeg4;
	if (params->depth >= 0x21 && params->depth < 0x1000) {
		alpha = params->a;
		ot = ACTIVE_ORDERING_TABLE->org;
		r = (int16_t)((params->r * alpha) >> 7);
		g = (int16_t)((params->g * alpha) >> 7);
		b = (int16_t)((params->b * alpha) >> 7);
		cw = (params->w - 1) & 0xFF;
		ch = (params->h - 1) & 0xFF;
		if (abs(params->x) < 0x400 && abs(params->y) < 0x200) {
			sz = params->size;
			halfW = (sz * cw) >> 0xA;
			if (halfW > 0) {
				halfW += 1;
				halfH = (sz * ch) >> 0xA;
				if (halfH > 0) {
					halfH += 1;
					if (halfW < 0x400 && halfH < 0x200) {
						cols = 0;
					} else {
						t = (halfW + 1) >> 0xA;
						if ((halfW + 1) < 0) {
							t = (halfW + 1 + 0x3FF) >> 0xA;
						}
						cols = t + 1;
						if (cols != 0) {
							cols = (cols & ~3) + 3;
						}
						if (!(cols & 1)) {
							cols += 1;
						}
					}
					t = (halfH + 1) >> 9;
					if ((halfH + 1) < 0) {
						t = (halfH + 1 + 0x1FF) >> 9;
					}
					rows = t + 1;
					colStep = (halfW + 1) / (cols + 1);
					rowStep = (halfH + 1) / (rows + 1);
					uStep = (cw + 1) / (cols + 1);
					vStep = (ch + 1) / (rows + 1);
					prim = (POLY_FT4 *)GsGetWorkBase();
					/* quadrant 1 */
					rowV1 = 1;
					row1 = 0;
					rowY1 = 0;
					while (rows >= row1) {
						y01 = (params->y - halfH) + rowY1;
						y11 = y01 + rowStep;
						if (y11 >= -DRAWING_OFFSET_Y && (-DRAWING_OFFSET_Y + 0xF0) >= y01) {
							uMul1 = 1;
							col1 = 0;
							v0base1 = vStep * row1;
							colX1 = 0;
							colU1 = 0;
							v2base1 = vStep * rowV1;
							while (cols >= col1) {
								x01 = (params->x - halfW) + colX1;
								x11 = x01 + colStep;
								if (x11 >= -DRAWING_OFFSET_X && (-DRAWING_OFFSET_X + 0x140) >= x01) {
									SetPolyFT4(prim);
									SetSemiTrans(prim, 1);
									prim->tpage = params->tpage;
									prim->clut = params->clut2;
									prim->x0 = x01;
									prim->y0 = y01;
									prim->x1 = x11;
									prim->y1 = y01;
									prim->x2 = x01;
									prim->y2 = y11;
									prim->x3 = x11;
									prim->y3 = y11;
									if (col1 == cols) {
										prim->x3 = params->x;
										prim->x1 = params->x;
									}
									if (row1 == rows) {
										prim->y3 = params->y;
										prim->y2 = params->y;
									}
									prim->r0 = r;
									prim->g0 = g;
									prim->b0 = b;
									prim->u2 = params->clut + colU1;
									prim->u0 = params->clut + colU1;
									prim->u3 = (params->clut + (uStep * uMul1)) - 1;
									prim->u1 = (params->clut + (uStep * uMul1)) - 1;
									prim->v1 = params->attr + v0base1;
									prim->v0 = params->attr + v0base1;
									prim->v3 = (params->attr + v2base1) - 1;
									prim->v2 = (params->attr + v2base1) - 1;
									AddPrim(ot + params->depth, prim);
									prim++;
								}
								uMul1 += 1;
								col1 += 1;
								colU1 += uStep;
								colX1 += colStep;
							}
						}
						rowV1 += 1;
						row1 += 1;
						rowY1 += rowStep;
					}
					/* quadrant 2 */
					rowV2 = 1;
					row2 = 0;
					rowY2 = 0;
					while (rows >= row2) {
						y02 = (params->y - halfH) + rowY2;
						y12 = y02 + rowStep;
						if (y12 >= -DRAWING_OFFSET_Y && (-DRAWING_OFFSET_Y + 0xF0) >= y02) {
							xNeg2 = -colStep;
							col2 = 0;
							colX2 = 0;
							v0base2 = vStep * row2;
							colU2 = 0;
							uMul2 = 1;
							v2base2 = vStep * rowV2;
							while (cols >= col2) {
								x02 = (params->x + halfW) - colX2;
								if (x02 >= -DRAWING_OFFSET_X && (-DRAWING_OFFSET_X + 0x140) >= (x02 - colStep)) {
									SetPolyFT4(prim);
									SetSemiTrans(prim, 1);
									prim->tpage = params->tpage;
									prim->clut = params->clut2;
									prim->x0 = x02;
									x12 = x02 + xNeg2;
									prim->y0 = y02;
									prim->x1 = x12;
									prim->y1 = y02;
									prim->x2 = x02;
									prim->y2 = y12;
									prim->x3 = x12;
									prim->y3 = y12;
									if (col2 == cols) {
										prim->x3 = params->x;
										prim->x1 = params->x;
									}
									if (row2 == rows) {
										prim->y3 = params->y;
										prim->y2 = params->y;
									}
									prim->r0 = r;
									prim->g0 = g;
									prim->b0 = b;
									prim->u2 = params->clut + colU2;
									prim->u0 = params->clut + colU2;
									prim->u3 = (params->clut + (uStep * uMul2)) - 1;
									prim->u1 = (params->clut + (uStep * uMul2)) - 1;
									prim->v1 = params->attr + v0base2;
									prim->v0 = params->attr + v0base2;
									prim->v3 = (params->attr + v2base2) - 1;
									prim->v2 = (params->attr + v2base2) - 1;
									AddPrim(ot + params->depth, prim);
									prim++;
								}
								colU2 += uStep;
								col2 += 1;
								uMul2 += 1;
								colX2 += colStep;
							}
						}
						rowV2 += 1;
						row2 += 1;
						rowY2 += rowStep;
					}
					/* quadrant 3 */
					rowV3 = 1;
					row3 = 0;
					rowY3 = 0;
					yNeg3 = -rowStep;
					while (rows >= row3) {
						y03 = (params->y + halfH) - rowY3;
						if (y03 >= -DRAWING_OFFSET_Y && (-DRAWING_OFFSET_Y + 0xF0) >= (y03 - rowStep)) {
							uMul3 = 1;
							y13 = y03 + yNeg3;
							col3 = 0;
							colX3 = 0;
							v0base3 = vStep * row3;
							colU3 = 0;
							v2base3 = vStep * rowV3;
							while (cols >= col3) {
								x03 = (params->x - halfW) + colX3;
								x13 = x03 + colStep;
								if (x13 >= -DRAWING_OFFSET_X && (-DRAWING_OFFSET_X + 0x140) >= x03) {
									SetPolyFT4(prim);
									SetSemiTrans(prim, 1);
									prim->tpage = params->tpage;
									prim->clut = params->clut2;
									prim->x0 = x03;
									prim->y0 = y03;
									prim->x1 = x13;
									prim->y1 = y03;
									prim->x2 = x03;
									prim->y2 = y13;
									prim->x3 = x13;
									prim->y3 = y13;
									if (col3 == cols) {
										prim->x3 = params->x;
										prim->x1 = params->x;
									}
									if (row3 == rows) {
										prim->y3 = params->y;
										prim->y2 = params->y;
									}
									prim->r0 = r;
									prim->g0 = g;
									prim->b0 = b;
									prim->u2 = params->clut + colU3;
									prim->u0 = params->clut + colU3;
									prim->u3 = (params->clut + (uStep * uMul3)) - 1;
									prim->u1 = (params->clut + (uStep * uMul3)) - 1;
									prim->v1 = params->attr + v0base3;
									prim->v0 = params->attr + v0base3;
									prim->v3 = (params->attr + v2base3) - 1;
									prim->v2 = (params->attr + v2base3) - 1;
									AddPrim(ot + params->depth, prim);
									prim++;
								}
								uMul3 += 1;
								col3 += 1;
								colU3 += uStep;
								colX3 += colStep;
							}
						}
						rowV3 += 1;
						row3 += 1;
						rowY3 += rowStep;
					}
					/* quadrant 4 */
					rowV4 = 1;
					row4 = 0;
					rowY4 = 0;
					yNeg4 = -rowStep;
					while (rows >= row4) {
						y04 = (params->y + halfH) - rowY4;
						if (y04 >= -DRAWING_OFFSET_Y && (-DRAWING_OFFSET_Y + 0xF0) >= (y04 - rowStep)) {
							xNeg4 = -colStep;
							col4 = 0;
							y14 = y04 + yNeg4;
							colX4 = 0;
							colU4 = 0;
							v0base4 = vStep * row4;
							uMul4 = 1;
							v2base4 = vStep * rowV4;
							while (cols >= col4) {
								x04 = (params->x + halfW) - colX4;
								if (x04 >= -DRAWING_OFFSET_X && (-DRAWING_OFFSET_X + 0x140) >= (x04 - colStep)) {
									SetPolyFT4(prim);
									SetSemiTrans(prim, 1);
									prim->tpage = params->tpage;
									prim->clut = params->clut2;
									prim->x0 = x04;
									x14 = x04 + xNeg4;
									prim->y0 = y04;
									prim->x1 = x14;
									prim->y1 = y04;
									prim->x2 = x04;
									prim->y2 = y14;
									prim->x3 = x14;
									prim->y3 = y14;
									if (col4 == cols) {
										prim->x3 = params->x;
										prim->x1 = params->x;
									}
									if (row4 == rows) {
										prim->y3 = params->y;
										prim->y2 = params->y;
									}
									prim->r0 = r;
									prim->g0 = g;
									prim->b0 = b;
									prim->u2 = params->clut + colU4;
									prim->u0 = params->clut + colU4;
									prim->u3 = (params->clut + (uStep * uMul4)) - 1;
									prim->u1 = (params->clut + (uStep * uMul4)) - 1;
									prim->v1 = params->attr + v0base4;
									prim->v0 = params->attr + v0base4;
									prim->v3 = (params->attr + v2base4) - 1;
									prim->v2 = (params->attr + v2base4) - 1;
									AddPrim(ot + params->depth, prim);
									prim++;
								}
								colU4 += uStep;
								col4 += 1;
								uMul4 += 1;
								colX4 += colStep;
							}
						}
						rowV4 += 1;
						row4 += 1;
						rowY4 += rowStep;
					}
					GsSetWorkBase((PACKET *)prim);
				}
			}
		}
	}
}

