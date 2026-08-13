#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libcd.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>
#include <malloc.h>

#include <dw/aabb.h>
#include <dw/clock.h>
#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/main.h>
#include <dw/math.h>
#include <dw/model.h>
#include <dw/move.h>
#include <dw/params.h>
#include <dw/sound.h>
#include <dw/std.h>
#include <dw/types.h>

#include "common.h"

extern int32_t MAIN_D_801350EC;
extern void *MAIN_D_80135130;
extern int8_t GAME_STATE;
extern int32_t VIEWPORT_DISTANCE;
extern int16_t MAIN_D_801B1C0C[];
extern int32_t MAIN_D_801B1C14[];
extern int32_t MAIN_D_801B1B98[];
extern int32_t MAIN_D_801B1BB8[];
extern int32_t MAIN_D_801B1BBC[];
extern GsF_LIGHT LIGHT_DATA[];
extern GsOT *ACTIVE_ORDERING_TABLE;
extern int32_t ACTIVE_FRAMEBUFFER;
extern GsDOBJ2 STD_D_8007B97C[];
extern GsOT STD_D_8007B684[];
extern GsOT STD_D_8007B6AC[];
extern GsCOORDINATE2 STD_D_8007B83C[];
extern GsOT_TAG STD_D_8007B664[];
extern GsOT_TAG STD_D_8007B674[];
extern GsOT_TAG STD_D_8007B6D4[];
extern GsOT_TAG STD_D_8007B6E4[];
extern char *STD_D_80079C24[];
extern char STD_D_80079C80[];
extern char STD_D_80079C94[];
extern uint8_t MAIN_D_801350F8;
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
extern char STD_D_80079CA8[];

void removeObject(int32_t objectId, int32_t instanceId);
void STD_func_80058488(void);
void STD_func_80056E2C(int32_t a, int16_t b, int32_t c);
void STD_loadTIMToVRAM(char *path);
void STD_func_8005D9F4(uint8_t *out, uint8_t *arg);
void STD_initializeCamera(void);
void STD_func_80058254(void);
void STD_func_80057FC4(void);
void STD_func_80057778(void);
void STD_func_80057FD0(int32_t which);
void addObject(int32_t objectId, int32_t instanceId, void *tick, void *render);
int16_t STD_func_80057510(int32_t a, int32_t b);
void STD_func_80058E28(int32_t arg);
int32_t STD_func_8006314C(Entity *entity, int32_t arg);
void STD_removeCameraIntro(void);
void GsGetTimInfo(unsigned long *tim, GsIMAGE *img);
void readFile(char *path, void *dest);
void STD_loadArenaTIMToVRAM(char *path, int32_t count);
void STD_func_800577B4();
int32_t STD_func_8005DFF8(void);
void STD_func_80058030(void);
void STD_func_80057E5C(int32_t id);
void STD_initializeFinisherAuraModel(char *tim, char *base);
void STD_initializePoisonBubble(void);
void STD_initializeConfusionEffect(char *base);
void STD_initializeStunEffect(char *base);
void initializeBuffModel(TMDModel *model);
void STD_battleTickFrame(void);
int32_t loadTIMFile(char *path, void *buffer);
void removeMapEntities(void);
void STD_func_8006B6F4(void);
void STD_func_80056CA8(int32_t arena, uint8_t *arg);
void STD_func_8005A550(void);
void fadeToBlack(int32_t mode);
void STD_func_80057628(void);

static void *std_setup_functions[] = {
	STD_func_80058488,
	STD_func_80058254,
	STD_func_80058030,
	STD_func_80057FD0,
	STD_func_80057FC4,
	STD_func_80057E5C,
	STD_loadArenaTIMToVRAM,
	STD_func_800579D8,
	STD_loadTIMToVRAM,
	STD_func_800577B4,
	STD_func_80057778,
	STD_func_80057628,
	STD_initializeCamera,
	STD_func_80057510,
	STD_func_80056E2C,
	STD_func_80056CA8,
};

void STD_func_80056CA8(int32_t arena, uint8_t *arg)
{
	if (arena == 0) {
		MAIN_D_801350F8 = 0x1e;
	} else {
		MAIN_D_801350F8 = 0x20;
	}

	GAME_STATE = 5;
	DRAWING_OFFSET_X = 0xa0;
	DRAWING_OFFSET_Y = 0x78;
	STD_D_8007B684[0].length = 2;
	STD_D_8007B684[0].org = STD_D_8007B664;
	STD_D_8007B684[1].length = 2;
	STD_D_8007B684[1].org = STD_D_8007B674;
	STD_D_8007B6AC[0].length = 2;
	STD_D_8007B6AC[0].org = STD_D_8007B6D4;
	STD_D_8007B6AC[1].length = 2;
	STD_D_8007B6AC[1].org = STD_D_8007B6E4;
	removeMapEntities();
	ENTITY_TABLE[0]->isOnScreen = 0;
	STD_func_80057778();
	STD_initializeCamera();
	STD_func_800577B4(arena);
	STD_func_80057E5C(arena);
	STD_func_8006B6F4();
	loadTIMFile(STD_D_80079C24[arg[0]], GENERAL_BUFFER_PTR);
	STD_loadTIMToVRAM(STD_D_80079C80);
	loadTIMFile(STD_D_80079C94, (void *)0x80010000);
	STD_initializeFinisherAuraModel((char *)0x80052ae0, (char *)0x80053800);
	STD_initializePoisonBubble();
	STD_initializeConfusionEffect((char *)0x80054838);
	STD_initializeStunEffect((char *)0x80054d00);
	initializeBuffModel((TMDModel *)0x80055328);
}

INCLUDE_ASM("asm/std/nonmatchings/std_setup", STD_func_80056E2C);

int16_t STD_func_80057510(int32_t a, int32_t b)
{
	int16_t result;

	ENTITY_TABLE[1]->isOnScreen = 1;
	GAME_STATE = 5;
	STD_func_80058E28(b);
	MAIN_D_801350EC = 1;
	result = STD_func_8006314C(ENTITY_TABLE[0], a);
	STD_removeCameraIntro();
	MAIN_D_801350EC = 10;
	return result;
}

void STD_initializeCamera(void)
{
	VIEWPORT_DISTANCE = 500;
	GsSetProjection(500);
	MAIN_D_801B1C0C[0] = 100;
	MAIN_D_801B1C0C[1] = 0;
	MAIN_D_801B1C0C[2] = 0;
	MAIN_D_801B1C14[0] = 0;
	MAIN_D_801B1C14[1] = 500;
	MAIN_D_801B1C14[2] = 3000;
	MAIN_D_801B1BB8[0] = 0;
	RotMatrix((SVECTOR *)MAIN_D_801B1C0C, (MATRIX *)MAIN_D_801B1B98);
	TransMatrix((MATRIX *)MAIN_D_801B1B98, (VECTOR *)MAIN_D_801B1C14);
	MAIN_D_801B1BBC[0] = 0;
	GsSetView2((GsVIEW2 *)MAIN_D_801B1B98);
}
void STD_func_80057628(void)
{
	RECT rect;
	int32_t i;

	i = 0;
	stopBGM();
	stopSound();
	STD_func_80058488();
	STD_func_8005A550();
	STD_func_80057FC4();
	rect.x = 0;
	rect.y = 0;
	rect.w = 0x140;
	rect.h = 0x1e0;
	ClearImage(&rect, 0, 0, 0);
	rect.x = 0x300;
	rect.y = 0;
	rect.w = 0xff;
	rect.h = 0x180;
	ClearImage(&rect, 0, 0, 0);
	rect.x = 0x180;
	rect.y = 0;
	rect.w = 0x180;
	rect.h = 0x100;
	ClearImage(&rect, 0, 0, 0);
	DrawSync(0);
	ENTITY_TABLE[1]->isOnScreen = 0;
	loadTIMFile(STD_D_80079CA8, GENERAL_BUFFER_PTR);
	fadeToBlack(1);

	for (; i < 0xb; i++) {
		STD_battleTickFrame();
	}

	ENTITY_TABLE[1]->posData->location.vy = 0;
	ENTITY_TABLE[1]->anim.locY = 0;
	ENTITY_TABLE[0]->isOnScreen = 1;
	GAME_STATE = 0;
}

void STD_func_80057778(void)
{
	removeObject(0xFA2, 0);
	removeObject(0xFA0, 0);
	removeObject(0xFA8, 0);
}

void STD_func_800577B4(void)
{
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
	LIGHT_DATA[2].vx = -20;
	LIGHT_DATA[2].vy = 20;
	LIGHT_DATA[2].vz = 200;
	LIGHT_DATA[2].r = 96;
	LIGHT_DATA[2].g = 96;
	LIGHT_DATA[2].b = 96;
	GsSetFlatLight(2, &LIGHT_DATA[2]);
	GsSetAmbient(0x400, 0x400, 0x400);
	GsSetLightMode(0);
}
void STD_loadTIMToVRAM(char *path)
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

INCLUDE_ASM("asm/std/nonmatchings/std_setup", STD_func_800579D8);

void STD_loadArenaTIMToVRAM(char *path, int32_t count)
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

INCLUDE_ASM("asm/std/nonmatchings/std_setup", STD_func_80057E5C);

void STD_func_80057FC4(void)
{
	free3(MAIN_D_80135130);
}

void STD_func_80057FD0(int32_t which)
{
	switch (which) {
	case 0:
		addObject(0x1A7, 0, 0, STD_func_80058030);
		break;
	case 1:
		addObject(0x1A7, 0, 0, STD_func_80058254);
		break;
	}
}

void STD_func_80058030(void)
{
	MATRIX m;
	int32_t i;

	GsGetLw(&STD_D_8007B83C[1], &m);
	GsSetLightMatrix(&m);
	GsGetLs(&STD_D_8007B83C[1], &m);
	GsSetLsMatrix(&m);
	GsSortObject4(&STD_D_8007B97C[1], ACTIVE_ORDERING_TABLE, 2, getScratchAddr(0));
	if (STD_func_8005DFF8() == 1) {
		STD_D_8007B97C[0].attribute |= 0x200;
	} else {
		STD_D_8007B97C[0].attribute = 0;
	}
	GsClearOt(0, 0xfff, &STD_D_8007B684[ACTIVE_FRAMEBUFFER]);
	GsClearOt(0, 0xffe, &STD_D_8007B6AC[ACTIVE_FRAMEBUFFER]);
	for (i = 2; i >= 0; i--) {
		if (i == 1) {
			continue;
		}
		GsGetLw(&STD_D_8007B83C[i], &m);
		GsSetLightMatrix(&m);
		GsGetLs(&STD_D_8007B83C[i], &m);
		GsSetLsMatrix(&m);
		switch (i) {
		case 0:
			GsSortObject4(&STD_D_8007B97C[i], &STD_D_8007B6AC[ACTIVE_FRAMEBUFFER], 0xc, getScratchAddr(0));
			break;
		case 2:
			GsSortObject4(&STD_D_8007B97C[i], &STD_D_8007B684[ACTIVE_FRAMEBUFFER], 0xc, getScratchAddr(0));
			break;
		}
	}
	GsSortOt(&STD_D_8007B684[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
	GsSortOt(&STD_D_8007B6AC[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
}
void STD_func_80058254(void)
{
	MATRIX m;
	int32_t i;

	if (STD_func_8005DFF8() == 1) {
		GsGetLw(&STD_D_8007B83C[3], &m);
		GsSetLightMatrix(&m);
		GsGetLs(&STD_D_8007B83C[3], &m);
		GsSetLsMatrix(&m);
		GsSortObject4(&STD_D_8007B97C[3], ACTIVE_ORDERING_TABLE, 2, getScratchAddr(0));
	}
	if (STD_func_8005DFF8() == 1) {
		STD_D_8007B97C[0].attribute |= 0x200;
	} else {
		STD_D_8007B97C[0].attribute = 0;
	}
	GsClearOt(0, 0xfff, &STD_D_8007B684[ACTIVE_FRAMEBUFFER]);
	GsClearOt(0, 0xffe, &STD_D_8007B6AC[ACTIVE_FRAMEBUFFER]);
	for (i = 2; i >= 0; i--) {
		GsGetLw(&STD_D_8007B83C[i], &m);
		GsSetLightMatrix(&m);
		GsGetLs(&STD_D_8007B83C[i], &m);
		GsSetLsMatrix(&m);
		switch (i) {
		case 0:
			GsSortObject4(&STD_D_8007B97C[i], &STD_D_8007B6AC[ACTIVE_FRAMEBUFFER], 0xc, getScratchAddr(0));
			break;
		case 1:
		case 2:
			GsSortObject4(&STD_D_8007B97C[i], &STD_D_8007B684[ACTIVE_FRAMEBUFFER], 0xc, getScratchAddr(0));
			break;
		}
	}
	GsSortOt(&STD_D_8007B684[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
	GsSortOt(&STD_D_8007B6AC[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
}
void STD_func_80058488(void)
{
	removeObject(0x1a7, 0);
}
