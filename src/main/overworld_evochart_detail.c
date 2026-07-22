#include <dw/evl.h>
#include <dw/ui.h>

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

typedef struct {
	int16_t posX;
	int16_t posY;
	uint8_t u;
	uint8_t v;
	uint8_t clut;
	uint8_t pad;
} EvoChartEntry;

typedef struct {
	int16_t m[5];
} EvoClutTable;

typedef struct {
	int16_t x1;
	int16_t x2;
	int16_t x3;
	int16_t x4;
	int16_t y1;
	int16_t y2;
	int16_t y3;
	int16_t y4;
} Line4Points;

typedef struct {
	int16_t posX;
	int16_t posY;
	int16_t unk4;
	int16_t unk6;
} ChartSprite;

typedef struct {
	int16_t posX;
	int16_t posY;
	int16_t width;
	int16_t height;
} Inset;

extern int16_t MAP_OBJECT_INSTANCE_COUNT;
extern LocalMapObject LOCAL_MAP_OBJECTS[];
extern int32_t MAP_OBJECT_MOVE_TO_DATA[];
extern int8_t MENU_SUB_STATE;
extern EvoChartEntry MAIN_D_80124544[];
extern int16_t MAIN_D_80134D40;
extern Line4Points MAIN_D_80124944[];
extern Line4Points MAIN_D_80124984[];
extern ChartSprite MAIN_D_80124AA8[];
extern ChartSprite MAIN_D_80124AC8[];
extern Line4Points MAIN_D_801249D4[];
extern Line4Points MAIN_D_80124A34[];
extern ChartSprite MAIN_D_80124AF0[];
extern ChartSprite MAIN_D_80124B20[];
extern RGB8 MAIN_D_80124A84[];
extern RGB8 MAIN_D_80124A85[];
extern RGB8 MAIN_D_80124A86[];
extern Inset MAIN_D_80124B48[];
extern EvoClutTable MAIN_D_80123E6C;

int32_t drawEvoChartStrings(int32_t arg);
void renderRectPolyFT4(int16_t posX, int16_t posY, int32_t width,
		       int32_t height, uint8_t texX, uint8_t texY,
		       int16_t texturePage, int16_t clut, int32_t zIndex,
		       int8_t flag);
void renderSpriteBox(int16_t x, int16_t y, int16_t w, int16_t h, int32_t c1,
		     int32_t c2, uint8_t r, uint8_t g, uint8_t b, int32_t a10);
void renderString();
int32_t hasDigimonRaised(int32_t id);
void renderTrianglePrimitive(int32_t color, int32_t x0, int32_t y0, int32_t x1,
			     int32_t y1, int32_t x2, int32_t y2, int32_t otz,
			     int32_t flag);
void renderLinePrimitive(int32_t color, int32_t x0, int32_t y0, int32_t x1,
			 int32_t y1, int32_t otz, int32_t flag);
int32_t strlen(char *s);
void renderInsetBox(int16_t a, int16_t b, int16_t c, int16_t d, int32_t otz);
void renderEvoChartDetail(void);
int32_t random(int32_t max);

static void renderEvoChartDetail__garbage__(LocalMapObjectInstance *mapObjects,
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

void renderEvoChartDetail(void)
{
	int32_t uVar3;
	int32_t uVar4;
	Line4Points *psVar6;
	ChartSprite *psVar11;
	Line4Points *psVar7;
	ChartSprite *psVar10;
	int32_t iVar5;
	int32_t iVar8;
	int32_t iVar9;
	int32_t iVar13;
	unsigned short local_10;
	int32_t new_var;
	int32_t limit;
	int32_t bound;
	int8_t count2;
	int8_t count1;
	EvoClutTable local_c;
	local_c = MAIN_D_80123E6C;
	MENU_SUB_STATE = 2;
	drawEvoChartStrings((int8_t)MAIN_D_80134D40);
	count1 = (count2 = 0);
	iVar5 = MAIN_D_80134D40 - 1;
	for (iVar13 = 0; iVar13 < 5; iVar13++) {
		if (EVO_PATHS_DATA[iVar5].from[iVar13] != (-1)) {
			++count1;
		}
	}
	for (iVar13 = 0; iVar13 < 6; iVar13++) {
		if (EVO_PATHS_DATA[(MAIN_D_80134D40 - 1)].to[iVar13] != (-1)) {
			++count2;
		}
	}
	uVar3 = count1 % 2;
	if (uVar3 == 0) {
		psVar6 = MAIN_D_80124944;
		psVar11 = MAIN_D_80124AA8;
		local_10 = 4;
	} else {
		psVar6 = MAIN_D_80124984;
		psVar11 = MAIN_D_80124AC8;
		local_10 = 5;
	}
	uVar4 = count2 % 2;
	if (uVar4 == 0) {
		psVar7 = MAIN_D_801249D4;
		psVar10 = MAIN_D_80124AF0;
		limit = 6;
	} else {
		psVar7 = MAIN_D_80124A34;
		psVar10 = MAIN_D_80124B20;
		limit = 5;
	}
	for (iVar8 = 0; iVar8 < local_10; iVar8++) {
		if (EVO_PATHS_DATA[MAIN_D_80134D40 - 1].from[iVar8] > 0) {
			renderTrianglePrimitive(0x65db,
						psVar6->x1, psVar6->y1 - 1,
						psVar6->x2, psVar6->y2 - 1,
						psVar6->x3, psVar6->y3 - 1,
						4, 0);
			renderLinePrimitive(0x65db,
					    psVar6->x3, psVar6->y3 - 1,
					    psVar6->x4, psVar6->y4 - 1,
					    4, 0);
			renderTrianglePrimitive(0x794e3,
						psVar6->x1, psVar6->y1,
						psVar6->x2, psVar6->y2,
						psVar6->x3, psVar6->y3,
						4, 0);
			renderLinePrimitive(0x794e3,
					    psVar6->x3, psVar6->y3,
					    psVar6->x4, psVar6->y4,
					    4, 0);
			renderTrianglePrimitive(0x65db,
						psVar6->x1, psVar6->y1 + 1,
						psVar6->x2, psVar6->y2 + 1,
						psVar6->x3, psVar6->y3 + 1,
						4, 0);
			renderLinePrimitive(0x65db,
					    psVar6->x3, psVar6->y3 + 1,
					    psVar6->x4, psVar6->y4 + 1,
					    4, 0);
		}
		psVar6++;
	}
	iVar8 = 0;
	iVar9 = 0;
	bound = limit;
	limit = bound;
	for (; iVar8 < bound; iVar8++, iVar9 += 2) {
		new_var = iVar9;
		if (EVO_PATHS_DATA[MAIN_D_80134D40 - 1].to[iVar8] > 0) {
			int32_t color1;
			int32_t color2;
			color1 = (MAIN_D_80124A84[new_var].r |
				  (MAIN_D_80124A84[new_var].g << 8) |
				  (MAIN_D_80124A84[new_var].b << 16));
			color2 = (MAIN_D_80124A84[new_var + 1].r |
				  (MAIN_D_80124A84[new_var + 1].g << 8) |
				  (MAIN_D_80124A84[new_var + 1].b << 16));
			renderTrianglePrimitive(color2,
						psVar7->x1, psVar7->y1 - 1,
						psVar7->x2, psVar7->y2 - 1,
						psVar7->x3, psVar7->y3 - 1,
						4, 0);
			renderLinePrimitive(color2,
					    psVar7->x3, psVar7->y3 - 1,
					    (long)psVar7->x4, psVar7->y4 - 1,
					    4, 0);
			renderTrianglePrimitive(color1,
						psVar7->x1, psVar7->y1,
						psVar7->x2, psVar7->y2,
						psVar7->x3, psVar7->y3,
						4, 0);
			renderLinePrimitive(color1,
					    psVar7->x3, psVar7->y3,
					    psVar7->x4, psVar7->y4,
					    4, 0);
			renderTrianglePrimitive(color2,
						psVar7->x1, psVar7->y1 + 1,
						psVar7->x2, psVar7->y2 + 1,
						psVar7->x3, psVar7->y3 + 1,
						4, 0);
			renderLinePrimitive(color2,
					    psVar7->x3, psVar7->y3 + 1,
					    psVar7->x4, psVar7->y4 + 1,
					    4, 0);
		}
		psVar7++;
	}
	iVar8 = (MAIN_D_80134D40 - 1) * 8;
	renderRectPolyFT4(-8, -0x14, 0x10, 0x10,
			  MAIN_D_80124544[MAIN_D_80134D40 - 1].u,
			  MAIN_D_80124544[MAIN_D_80134D40 - 1].v, 0x18,
			  local_c.m[MAIN_D_80124544[MAIN_D_80134D40 - 1].clut],
			  4, 0);
	renderSpriteBox(0x97, 99, 0x12, 0x12, 0xbebebe, 0x3c3c3c, 0x87, 0x87,
			0x87, 4);
	for (iVar8 = 0; iVar8 < local_10; iVar8++) {
		int32_t from;
		from = EVO_PATHS_DATA[MAIN_D_80134D40 - 1].from[iVar8];
		if (from > 0) {
			if (hasDigimonRaised(from & 0xffff) == 1) {
				renderRectPolyFT4(psVar11->posX, psVar11->posY,
						  0x10, 0x10,
						  MAIN_D_80124544[from - 1].u,
						  MAIN_D_80124544[from - 1].v,
						  0x18,
						  local_c.m[MAIN_D_80124544[from - 1].clut],
						  4, 0);
			}
			renderSpriteBox((int16_t)(psVar11->posX + 0x9f),
					(int16_t)(psVar11->posY + 0x77),
					0x12, 0x12, 0xbebebe, 0x3c3c3c, 0x87,
					0x87, 0x87, 4);
		}
		psVar11++;
	}
	for (iVar8 = 0; iVar8 < limit; iVar8++) {
		int32_t to;
		to = EVO_PATHS_DATA[MAIN_D_80134D40 - 1].to[iVar8];
		if (to > 0) {
			if (hasDigimonRaised(to & 0xffff) == 1) {
				renderRectPolyFT4(psVar10->posX, psVar10->posY,
						  0x10, 0x10,
						  MAIN_D_80124544[to - 1].u,
						  MAIN_D_80124544[to - 1].v,
						  0x18,
						  local_c.m[MAIN_D_80124544[to - 1].clut],
						  4, 0);
			}
			renderSpriteBox((int16_t)(psVar10->posX + 0x9f),
					(int16_t)(psVar10->posY + 0x77),
					0x12, 0x12, 0xbebebe, 0x3c3c3c, 0x87,
					0x87, 0x87, 4);
		}
		psVar10++;
	}
	renderString(3, -0x14, -0x4f, 0x24, 0xc, 0, 0x18, 4);
	switch (DIGIMON_DATA[MAIN_D_80134D40].level) {
	case 1:
		renderString(0, -0x5b, 0x3a, 0x2e, 0xc, 0x24, 0x18, 4);
		break;
	case 2:
		renderString(0, -0x63, 0x3a, 0x3e, 0xc, 0x52, 0x18, 4);
		break;
	case 3:
		renderString(0, -0x5b, 0x3a, 0x2f, 0xc, 0, 0x24, 4);
		break;
	case 4:
		renderString(0, -0x64, 0x3a, 0x41, 0xc, 0x2f, 0x24, 4);
		break;
	case 5:
		renderString(0, -0x64, 0x3a, 0x41, 0xc, 0, 0x3c, 4);
	}
	new_var = strlen(DIGIMON_DATA[MAIN_D_80134D40].name) / 2;
	renderString(0, -0x5c - ((new_var - 4) * 6), 0x4d, 0x78, 0xc, 0, 0x30, 4);
	for (iVar13 = 0; iVar13 < 6; ++iVar13) {
		renderInsetBox(MAIN_D_80124B48[iVar13].posX,
			       MAIN_D_80124B48[iVar13].posY,
			       MAIN_D_80124B48[iVar13].width,
			       MAIN_D_80124B48[iVar13].height, 4);
	}
}
