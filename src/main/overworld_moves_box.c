#include <dw/clock.h>
#include <dw/evl.h>
#include <dw/move.h>

typedef struct {
	int16_t orderValue;
	int16_t x;
	int16_t y;
	int16_t animSprites[8];
	uint8_t animTimes[8];
	uint8_t timer;
	uint8_t pad;
	int8_t currentFrame;
	int8_t flag;
} LocalMapObjectInstance;

typedef struct {
	int16_t texX;
	int16_t texY;
	int16_t someX;
	int16_t someY;
	int16_t someZ;
	uint8_t width;
	uint8_t height;
	int8_t clut;
	int8_t transparency;
} LocalMapObject;

extern int16_t MAP_OBJECT_INSTANCE_COUNT;
extern LocalMapObject LOCAL_MAP_OBJECTS[];
extern int32_t MAP_OBJECT_MOVE_TO_DATA[];
extern uint8_t MAIN_D_80134234[4];

void renderRectPolyFT4(int16_t posX, int16_t posY, int32_t width,
		       int32_t height, uint8_t texX, uint8_t texY,
		       int16_t texturePage, int16_t clut, int32_t zIndex,
		       int8_t flag);
void renderBoxBar(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t r,
		  uint8_t g, uint8_t b, uint8_t flags, int32_t otz);
int32_t hasMove(int32_t move);
void sortArray(int16_t *arr, uint8_t count);
void renderDigimonMoveBox(void);
void renderDigimonMovesSelected(int32_t arg);
int32_t drawMoveViewHelpStrings(void);
void removeTriangleMenu(void);
void removeUIBox1(void);
int32_t random(int32_t max);
void removeStaticUIBox(int32_t id);
void removeObject(int32_t objectId, int32_t arg1);

void renderDigiviceMedals(void);

static void renderDigimonMoveBox__garbage__(LocalMapObjectInstance *mapObjects,
					    uint8_t *data, int32_t mapId)
{
	LocalMapObjectInstance *obj;
	int16_t *src;
	int32_t i;
	int32_t j;
	int32_t k;
	int16_t count;

	obj = mapObjects;
	src = (int16_t *)data;
	count = *src++;
	for (i = 0; i < count; i++) {
		LOCAL_MAP_OBJECTS[i].texX = *src++;
		LOCAL_MAP_OBJECTS[i].texY = *src++;
		LOCAL_MAP_OBJECTS[i].width = *src++;
		LOCAL_MAP_OBJECTS[i].height = *src++;
		LOCAL_MAP_OBJECTS[i].someX = *src++;
		LOCAL_MAP_OBJECTS[i].someY = *src++;
		LOCAL_MAP_OBJECTS[i].someZ = *src++;
		LOCAL_MAP_OBJECTS[i].clut = *src++;
		LOCAL_MAP_OBJECTS[i].transparency = *src++;
	}
	MAP_OBJECT_INSTANCE_COUNT = *src++;
	for (k = 0; k < MAP_OBJECT_INSTANCE_COUNT; k++) {
		for (j = 0; j < 8; j++) {
			obj->animSprites[j] = *src++;
		}
		for (j = 0; j < 8; j++) {
			obj->animTimes[j] = *src++;
		}
		obj->x = *src++;
		obj->y = *src++;
		obj->flag = *src++;
		if (((mapId >= 0x58 && mapId < 0x61) ||
		     (mapId >= 0x84 && mapId < 0x88)) &&
		    k < 0x23) {
			if (k >= 0x14) {
				obj->x = random(320);
			}
			obj->y = random(240);
			obj->flag |= 0x80;
		}
		obj++;
	}
	for (k = 0; k < 10; k++) {
		MAP_OBJECT_MOVE_TO_DATA[k] = 0;
	}
}

static void *overworld_moves_box_functions[] = {
	removeUIBox1,
	removeTriangleMenu,
	sortArray,
	renderDigiviceMedals,
	drawMoveViewHelpStrings,
	renderDigimonMoveBox,
	renderDigimonMovesSelected,
};

INCLUDE_ASM("asm/main/nonmatchings/overworld_moves_box", renderDigimonMovesSelected);

void renderDigimonMoveBox(void)
{
	uint8_t bVar1;
	uint8_t uVar2;
	uint8_t uVar3;
	uint8_t uVar6;
	uint8_t cVar4;
	int16_t iVar5;
	int32_t iVar6;
	int16_t unaff_s2;
	int32_t iVar10;
	int32_t iVar11;
	int32_t iVar12;
	int32_t iVar13;
	int16_t local_20[16];
	for (iVar11 = 0; iVar11 < 3; iVar11++) {
		uVar2 = (uVar6 = MAIN_D_80134234[iVar11]);
		if (uVar2 < 0x3a) {
			uVar3 = uVar6 % 8;
			if (uVar6 < 8) {
				iVar13 = 0;
			} else if (uVar6 < 0x10) {
				iVar13 = 2;
			} else if (uVar6 < 0x18) {
				iVar13 = 4;
			} else if (uVar6 < 0x20) {
				iVar13 = 5;
			} else if (uVar6 < 0x28) {
				iVar13 = 3;
			} else if (uVar6 < 0x30) {
				iVar13 = 1;
			} else {
				uVar3 = (uVar6 - 1) % 8;
				iVar13 = 6;
			}
			if (uVar6 == 0x30) {
				uVar3 = 4;
				iVar13 = 1;
			}
			if (uVar6 == 0x39) {
				iVar13 = uVar3 = 6;
			}
			renderBoxBar(uVar3 * 0x12 - 0x2a, iVar13 * 0xf - 7, 0xc, 0xc, 200, 0, 0x28, 1, 4);
		}
	}
	for (iVar11 = 0; iVar11 < 0x10; iVar11++) {
		local_20[iVar11] = DIGIMON_DATA[ENTITY_TABLE[1]->type].moves[iVar11];
	}
	sortArray(local_20, 0x10);
	for (iVar11 = 0, iVar12 = 0; iVar12 < 7; iVar12++) {
		for (iVar10 = 0; iVar10 < 8; iVar10++) {
			iVar5 = iVar12 * 8 + iVar10;
			if (0x2f < iVar5) {
				++iVar5;
			}
			if (iVar5 == 0x2c) {
				for (iVar6 = 0; iVar6 < 0x10; iVar6++) {
					if (local_20[iVar6] == 0x2c || local_20[iVar6] == 0x30) {
						break;
					}
					if (iVar6 == 0xf) {
						renderBoxBar(0x1e, 8, 0xc, 0xc, 0x68, 0x68, 0x68, 3, 4);
					}
				}
			}
			if (iVar5 == 0x37) {
				for (iVar6 = 0; iVar6 < 0x10; iVar6++) {
					if (local_20[iVar6] == 0x37 || local_20[iVar6] == 0x39) {
						break;
					}
					if (iVar6 == 0xf) {
						renderBoxBar(0x42, 0x53, 0xc, 0xc, 0x68, 0x68, 0x68, 3, 4);
					}
				}
			}
			if (iVar5 == local_20[iVar11]) {
				++iVar11;
			} else if (iVar5 != 0x2c && iVar5 != 0x37 && iVar5 != 0x30 &&
				   iVar5 != 0x39) {
				if (iVar5 < 8) {
					iVar13 = 0;
				} else if (iVar5 < 0x10) {
					iVar13 = 2;
				} else if (iVar5 < 0x18) {
					iVar13 = 4;
				} else if (iVar5 < 0x20) {
					iVar13 = 5;
				} else if (iVar5 < 0x28) {
					iVar13 = 3;
				} else if (iVar5 < 0x30) {
					iVar13 = 1;
				} else {
					iVar13 = 6;
				}
				renderBoxBar(iVar10 * 0x12 - 0x2a, iVar13 * 0xf - 7, 0xc,
					     0xc, 0x68, 0x68, 0x68, 3, 4);
			}
			if (hasMove(iVar5) == 1) {
				bVar1 = MOVE_DATA[iVar5].special;
				switch (bVar1) {
					case 0:
						unaff_s2 = 0x7a06;
						break;
					case 1:
						unaff_s2 = 0x7a06;
						break;
					case 2:
						unaff_s2 = 0x7a46;
						break;
					case 3:
						unaff_s2 = 0x7a46;
						break;
					case 4:
						unaff_s2 = 0x7a46;
						break;
					case 5:
						unaff_s2 = 0x7a86;
						break;
					case 6:
						unaff_s2 = 0x7a06;
				}
				if (PLAYTIME_FRAMES % 0x14 < 10) {
					cVar4 = 0;
				} else {
					cVar4 = 0xc;
				}
				renderRectPolyFT4(iVar10 * 0x12 - 0x2a, bVar1 * 0xf - 7, 0xc,
						  0xc, cVar4 + (bVar1 * 0x18 + 0x24), 0x80, 5,
						  unaff_s2, 4, 0);
			}
		}
	}
}

INCLUDE_ASM("asm/main/nonmatchings/overworld_moves_box", drawMoveViewHelpStrings);

INCLUDE_ASM("asm/main/nonmatchings/overworld_moves_box", renderDigiviceMedals);

void sortArray(int16_t *arr, uint8_t count)
{
	int32_t i;
	int32_t j;
	int16_t tmp;

	for (i = 0; i < count - 1; ++i) {
		for (j = i + 1; j < count; ++j) {
			if (arr[i] > arr[j]) {
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}

void removeTriangleMenu(void)
{
	removeStaticUIBox(0);
	removeObject(0xfa4, 0);
}

void removeUIBox1(void)
{
	removeStaticUIBox(1);
}
