#include <dw/clock.h>

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

extern int8_t MENU_STATE;
extern EvoChartEntry MAIN_D_80124544[];
extern EvoClutTable MAIN_D_80123E1C;
extern int16_t MAIN_D_80134D40;
extern int16_t MAIN_D_80134D42;
extern int16_t MAIN_D_80134D44;

int32_t drawEvoChartStrings(int32_t arg);
void renderRectPolyFT4(int16_t posX, int16_t posY, int32_t width,
		       int32_t height, uint8_t texX, uint8_t texY,
		       int16_t texturePage, int16_t clut, int32_t zIndex,
		       int8_t flag);
void renderSpriteBox(int16_t x, int16_t y, int16_t w, int16_t h,
		     int32_t c1, int32_t c2, uint8_t r, uint8_t g, uint8_t b,
		     int32_t a10);
void renderString();
int32_t hasDigimonRaised(int32_t id);
void renderEvoChartView(void);

void renderEvoChartView(void)
{
	int32_t iVar1;
	int32_t cVar2;
	int32_t uVar4;
	int32_t iVar5;
	int32_t iVar7;
	EvoClutTable local_c;

	local_c = MAIN_D_80123E1C;
	switch (MENU_STATE) {
	case 0:
		if (drawEvoChartStrings(0) == 1) {
			MENU_STATE = 1;
		}
		break;
	case 2:
	case 3:
	case 4:
		if ((iVar5 = MAIN_D_80134D42) < 3) {
			iVar5 = (int16_t)(iVar5 * 0x25 + 0x1c);
		} else if (iVar5 < 7) {
			iVar5 = (int16_t)((iVar5 - 3) * 0x18 + 0x8b);
		} else {
			iVar5 = (int16_t)((iVar5 - 7) * 0x18 + 0xf8);
		}
		renderRectPolyFT4((int16_t)(iVar5 - 0xa2),
			(int16_t)(MAIN_D_80134D44 * 0x13 - 0x4e), 0x18, 0x14,
			0, 0xe8, 0x18, 0x7dc7, 5, 0);
		/* fall through */
	case 1:
		for (iVar7 = 0, uVar4 = 1; iVar7 < 0x3e; iVar7++, uVar4++) {
			iVar1 = hasDigimonRaised(uVar4 & 0xffff);
			if (iVar1 != 0) {
				cVar2 = 0;
				if ((uVar4 == MAIN_D_80134D40) &&
				    (1 < MENU_STATE) &&
				    ((PLAYTIME_FRAMES % 10) < 5)) {
					cVar2 = 0x10;
				}
				renderRectPolyFT4(
					(int16_t)(MAIN_D_80124544[iVar7].posX - 0xa0),
					(int16_t)(MAIN_D_80124544[iVar7].posY - 0x78),
					0x10, 0x10,
					(uint8_t)(cVar2 + MAIN_D_80124544[iVar7].u),
					MAIN_D_80124544[iVar7].v, 0x18,
					local_c.m[MAIN_D_80124544[iVar7].clut],
					5, 0);
			}
		}
		for (iVar7 = 0; iVar7 < 0x3e; iVar7++) {
			renderSpriteBox((MAIN_D_80124544[iVar7].posX - 1),
				(MAIN_D_80124544[iVar7].posY - 1), 0x12, 0x12,
				0xbebebe, 0x3c3c3c, 0x87, 0x87, 0x87, 5);
		}
		renderSpriteBox(0x15, 0x27, 0x22, 0x9f, 0xaaa0c8, 0x5a3c8c,
				0xa5, 0x5a, 0x73, 5);
		renderSpriteBox(0x3a, 0x27, 0x22, 0x9f, 0xa078a5, 0x46144b,
				0x6e, 0x2d, 0x5a, 5);
		renderSpriteBox(0x5f, 0x27, 0x22, 0xb0, 0xc88250, 0x5f370a,
				0xf, 0x50, 0x82, 5);
		renderSpriteBox(0x84, 0x27, 0x6a, 0x9f, 0x6eaf5a, 0x285a00,
				0, 0x87, 0x41, 5);
		renderSpriteBox(0xf1, 0x27, 0x38, 0x9f, 0x6996d2, 0x1e4178,
				0xaf, 0x64, 0x2d, 5);
		renderString(0xf, -0x1a, 0x54, 0xc, 0xc, 0, 0xc, 5, 1);
		renderString(0, -0xe, 0x54, 0x3c, 0xc, 0xc, 0xc, 5, 1);
		renderString(7, 0x3a, 0x54, 0xc, 0xc, 0x48, 0xc, 5, 1);
		renderString(0, 0x46, 0x54, 0x3c, 0xc, 0x54, 0xc, 5, 1);
	}
}
