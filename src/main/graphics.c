#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>
#include <mwinline_n.h>

#include <dw/attack_object.h>
#include <dw/entity.h>
#include <dw/model.h>
#include <dw/params.h>
#include <dw/types.h>

#include "common.h"

typedef struct {
	SVECTOR *center;
	SVECTOR extent;
} AABB;

PACKET *GsTMDfastF3L();
PACKET *GsTMDfastG3L();
PACKET *GsTMDfastF4L();
PACKET *GsTMDfastF4NL();
PACKET *GsTMDfastG4L();
PACKET *GsTMDfastNF4();
PACKET *GsTMDfastTF3L();
PACKET *GsTMDfastTF3NL();
PACKET *GsTMDfastTNF3();
PACKET *GsTMDfastTF4L();
PACKET *GsTMDfastTF4NL();
PACKET *GsTMDfastTNF4();
PACKET *GsTMDfastTG3L();
PACKET *GsTMDfastTG3NL();
PACKET *GsTMDfastTNG3();
PACKET *GsTMDfastTG4L();
PACKET *GsTMDfastTG4NL();
PACKET *GsTMDfastTNG4();
PACKET *GsTMDdivTF3NL();
PACKET *GsTMDdivTNF3();
PACKET *GsTMDdivTG3NL();
PACKET *GsTMDdivTNG3();
PACKET *GsTMDdivTF4L();
PACKET *GsTMDdivTF4NL();
PACKET *GsTMDdivTNF4();
PACKET *GsTMDdivTG4NL();
PACKET *GsTMDdivTNG4();
void setRotTransMatrix(MATRIX *m);
void initializeGsTMDMap(void);
int32_t getTileTrigger(VECTOR *loc);
void unusedAABB(AABB *box);
void updateTMDTextureData(char *tmd, int32_t clutX, int32_t x, int32_t y, int32_t tpage);
void renderDropShadow(Entity *entity);

extern _GsFCALL GS_TMD_MAP;
extern int32_t MAIN_D_80137BE8[];
extern int32_t MAIN_D_80137BE4[];
extern GsOT *ACTIVE_ORDERING_TABLE;
extern AttackObject RESET_ATTACK_OBJECT;
extern AttackObject ATTACK_OBJECTS[];

void setRotTransMatrix(MATRIX *m)
{
	gte_SetRotMatrix(m);
	gte_SetTransMatrix(m);
}

void initializeGsTMDMap(void)
{
	GS_TMD_MAP.f3[GsDivMODE_NDIV][GsLMODE_NORMAL]	= GsTMDfastF3L;
	GS_TMD_MAP.tf3[GsDivMODE_NDIV][GsLMODE_NORMAL]	= GsTMDfastTF3L;
	GS_TMD_MAP.tf3[GsDivMODE_NDIV][GsLMODE_LOFF]	= GsTMDfastTF3NL;
	GS_TMD_MAP.ntf3[GsDivMODE_NDIV]			= GsTMDfastTNF3;
	GS_TMD_MAP.g3[GsDivMODE_NDIV][GsLMODE_NORMAL]	= GsTMDfastG3L;
	GS_TMD_MAP.tg3[GsDivMODE_NDIV][GsLMODE_NORMAL]	= GsTMDfastTG3L;
	GS_TMD_MAP.tg3[GsDivMODE_NDIV][GsLMODE_LOFF]	= GsTMDfastTG3NL;
	GS_TMD_MAP.ntg3[GsDivMODE_NDIV]			= GsTMDfastTNG3;
	GS_TMD_MAP.f4[GsDivMODE_NDIV][GsLMODE_NORMAL]	= GsTMDfastF4L;
	GS_TMD_MAP.tf4[GsDivMODE_NDIV][GsLMODE_NORMAL]	= GsTMDfastTF4L;
	GS_TMD_MAP.tf4[GsDivMODE_NDIV][GsLMODE_LOFF]	= GsTMDfastTF4NL;
	GS_TMD_MAP.ntf4[GsDivMODE_NDIV]			= GsTMDfastTNF4;
	GS_TMD_MAP.g4[GsDivMODE_NDIV][GsLMODE_NORMAL]	= GsTMDfastG4L;
	GS_TMD_MAP.tg4[GsDivMODE_NDIV][GsLMODE_NORMAL]	= GsTMDfastTG4L;
	GS_TMD_MAP.tg4[GsDivMODE_NDIV][GsLMODE_LOFF]	= GsTMDfastTG4NL;
	GS_TMD_MAP.ntg4[GsDivMODE_NDIV]			= GsTMDfastTNG4;
	GS_TMD_MAP.tf4[GsDivMODE_DIV][GsLMODE_NORMAL]	= GsTMDdivTF4L;
	GS_TMD_MAP.tf4[GsDivMODE_DIV][GsLMODE_LOFF]	= GsTMDdivTF4NL;
	GS_TMD_MAP.tf3[GsDivMODE_DIV][GsLMODE_LOFF]	= GsTMDdivTF3NL;
	GS_TMD_MAP.tg4[GsDivMODE_DIV][GsLMODE_LOFF]	= GsTMDdivTG4NL;
	GS_TMD_MAP.tg3[GsDivMODE_DIV][GsLMODE_LOFF]	= GsTMDdivTG3NL;
	GS_TMD_MAP.ntg4[GsDivMODE_DIV]			= GsTMDdivTNG4;
	GS_TMD_MAP.ntg3[GsDivMODE_DIV]			= GsTMDdivTNG3;
	GS_TMD_MAP.ntf4[GsDivMODE_DIV]			= GsTMDdivTNF4;
	GS_TMD_MAP.ntf3[GsDivMODE_DIV]			= GsTMDdivTNF3;
	GS_TMD_MAP.f4[GsDivMODE_NDIV][GsLMODE_LOFF]	= GsTMDfastF4NL;
	GS_TMD_MAP.nf4[GsDivMODE_NDIV]			= GsTMDfastNF4;
}

void initializeAttackObjects(void)
{
	int32_t i;
	AttackObject tmp;

	i = 0;
	tmp = RESET_ATTACK_OBJECT;
	for (; i < 0x10; i++) {
		ATTACK_OBJECTS[i] = RESET_ATTACK_OBJECT;
	}
}

int32_t addAttackObject(int32_t victimId, int32_t active, SVECTOR *pos, int32_t effectId, int32_t subEffectIndex, int32_t casterId)
{
	int32_t i;

	MAIN_D_80137BE8[0] = -1;
	for (i = 0;; i++) {
		if (ATTACK_OBJECTS[i].active == -1) {
			break;
		}
	}
	if (i == 0x10) {
		return 0;
	}
	ATTACK_OBJECTS[i].victimId = victimId;
	ATTACK_OBJECTS[i].active = active;
	ATTACK_OBJECTS[i].position = *pos;
	ATTACK_OBJECTS[i].effectId = effectId;
	ATTACK_OBJECTS[i].subEffectIndex = subEffectIndex;
	ATTACK_OBJECTS[i].casterId = casterId;
	return 1;
}

int32_t popAttackObject(uint8_t entityId, AttackObject *out)
{
	AttackObject tmp;
	int32_t i;
	int32_t k;
	int32_t m;

	MAIN_D_80137BE4[0] = entityId;
	for (i = 0;; i++) {
		if (ATTACK_OBJECTS[i].victimId == entityId) {
			break;
		}
	}
	if (i == 0x10) {
		return 0;
	}
	*out = ATTACK_OBJECTS[i];
	ATTACK_OBJECTS[i] = RESET_ATTACK_OBJECT;
	for (k = 0; k < 0xF; k++) {
		tmp = RESET_ATTACK_OBJECT;
		if (ATTACK_OBJECTS[k].active == -1) {
			for (m = k + 1; m < 0x10; m++) {
				if (ATTACK_OBJECTS[m].active != -1) {
					break;
				}
			}
			if (m == 0x10) {
				break;
			}
			ATTACK_OBJECTS[k] = ATTACK_OBJECTS[m];
			ATTACK_OBJECTS[m] = tmp;
		}
	}
	return 1;
}

void updateTMDTextureData(char *tmd, int32_t clutX, int32_t x, int32_t y, int32_t tpage)
{
	int32_t nObj;
	int32_t *hdr;
	uint32_t *prim;
	struct TMD_STRUCT *obj;
	int32_t i;
	int32_t j;
	int32_t primn;
	int32_t clut;
	int32_t tpages;
	int32_t clutXs;
	uint8_t mode;
	uint32_t len;

	hdr = (int32_t *)((int32_t)tmd + 8);
	nObj = *hdr++;
	obj = (struct TMD_STRUCT *)hdr;
	clut = (y << 8) + x;
	tpages = tpage << 16;
	clutXs = clutX << 16;
	for (i = 0; i < nObj; obj++, i++) {
		primn = obj->primn;
		prim = (uint32_t *)obj->primtop;
		for (j = 0; j < primn; j++) {
			mode = prim[0] >> 24;
			if ((mode & 4) == 0) {
				break;
			}
			len = (prim[0] & 0xFF00) >> 8;
			prim[1] += tpages + clut;
			prim[2] = clut + ((prim[2] & 0xFFE0FFFF) + clutXs);
			prim[3] += clut;
			len++;
			if (mode & 8) {
				prim[4] += clut;
			}
			prim += len;
		}
	}
}

void renderDropShadow(Entity *entity)
{
	SVECTOR p0;
	SVECTOR p1;
	SVECTOR p2;
	SVECTOR p3;
	long depth;
	long flag;
	AABB box;
	SVECTOR c;
	POLY_FT4 *prim;
	int16_t x;
	PositionData *posData;
	int16_t y;
	PositionData *pos;
	int16_t radiusX;
	int16_t radiusZ;
	int16_t z;

	if (getTileTrigger(&entity->posData->location) == -1) {
		return;
	}
	radiusX = DIGIMON_DATA[entity->type].radius;
	radiusZ = DIGIMON_DATA[entity->type].radius;
	posData = entity->posData;
	pos = posData;
	x = pos->location.vx;
	y = pos->location.vy;
	z = pos->location.vz;
	p0.vx = x - radiusX;
	p0.vy = y;
	p0.vz = z - radiusZ;
	p1.vx = x + radiusX;
	p1.vy = y;
	p1.vz = z - radiusZ;
	p2.vx = x - radiusX;
	p2.vy = y;
	p2.vz = z + radiusZ;
	p3.vx = x + radiusX;
	p3.vy = y;
	p3.vz = z + radiusZ;
	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	SetSemiTrans(prim, 1);
	prim->tpage = getTPage(1, 2, 832, 256);
	setClut(prim, 0, 0x1E7);
	setUV4(prim, 0x40, 0x80, 0x7F, 0x80, 0x40, 0xBF, 0x7F, 0xBF);
	setRotTransMatrix(&GsWSMATRIX);
	RotTransPers4(&p0, &p1, &p2, &p3, (long *)&prim->x0, (long *)&prim->x1, (long *)&prim->x2,
		      (long *)&prim->x3, &depth, &flag);
	prim->r0 = prim->g0 = prim->b0 = 0x30;
	AddPrim(ACTIVE_ORDERING_TABLE->org + 0xFFD, prim);
	prim++;
	GsSetWorkBase((PACKET *)prim);
	c.vx = entity->posData->location.vx;
	c.vy = -(DIGIMON_DATA[entity->type].height >> 1);
	c.vz = entity->posData->location.vz;
	box.center = &c;
	box.extent.vx = DIGIMON_DATA[entity->type].radius;
	box.extent.vy = DIGIMON_DATA[entity->type].height >> 1;
	box.extent.vz = DIGIMON_DATA[entity->type].radius;
	unusedAABB(&box);
}
