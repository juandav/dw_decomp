#include <dw/graphics.h>
#include <dw/map.h>

extern GsOT *ACTIVE_ORDERING_TABLE;

void renderSpriteBox(int16_t x, int16_t y, int16_t w, int16_t h, int32_t c1,
		     int32_t c2, uint8_t r, uint8_t g, uint8_t b, int32_t a10);
void renderBoxBar(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t r,
		  uint8_t g, uint8_t b, uint8_t flags, int32_t otz);
void renderTrianglePrimitive(int32_t color, int32_t x0, int32_t y0, int32_t x1,
			     int32_t y1, int32_t x2, int32_t y2, int32_t otz,
			     int32_t flag);
void renderInsetBox(int16_t a, int16_t b, int16_t c, int16_t d, int32_t otz);
void renderMedalView(void);
void renderCardsView(void);
void drawDigimonStatsStrings(void);
void renderDigimonStatsBar(int32_t a, int32_t b, int32_t c, int16_t d, int16_t e);
void renderDigimonStatusConditions(void);
void renderDigiviceEntity(void);

static void *overworld_status_boxes_functions[] = {
	renderDigiviceEntity,
	renderDigimonStatusConditions,
	renderDigimonStatsBar,
	drawDigimonStatsStrings,
	renderSpriteBox,
	renderBoxBar,
	renderInsetBox,
	renderCardsView,
	renderMedalView,
};

INCLUDE_ASM("asm/main/nonmatchings/overworld_status_boxes", renderMedalView);

INCLUDE_ASM("asm/main/nonmatchings/overworld_status_boxes", renderCardsView);

void renderInsetBox(int16_t a, int16_t b, int16_t c, int16_t d, int32_t otz)
{
	void renderBoxBar(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t r,
			  uint8_t g, uint8_t b, uint8_t flags, int32_t otz);
	int32_t x;
	int32_t y;
	int32_t y0;
	int32_t x0;
	int32_t cc;
	int32_t dd;

	y = b;
	x = a;
	x = (int16_t)(x - 0xa0);
	y = (int16_t)(y - 0x78);
	y = y0 = y;
	x = x0 = x;
	dd = d;
	cc = c;
	renderTrianglePrimitive(0x20202, x0, y0 + dd, x0, y0, x0 + cc, y0, otz, 0);
	renderTrianglePrimitive(0xa08769, x0, y0 + dd, x0 + cc, y0 + dd, x0 + cc, y0,
				otz, 0);
	renderBoxBar(x, y, cc, dd, 0x35, 0x4b, 0x5c, 0, otz);
}

void renderBoxBar(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t r,
		  uint8_t g, uint8_t b, uint8_t flags, int32_t otz)
{
	GsBOXF box;

	if ((flags & 1) != 0) {
		if ((flags & 2) != 0) {
			box.attribute = 0x60000000;
		} else {
			box.attribute = 0x40000000;
		}
	} else {
		box.attribute = 0;
	}

	box.x = x;
	box.y = y;
	box.w = w;
	box.h = h;
	box.r = r;
	box.g = g;
	box.b = b;

	GsSortBoxFill(&box, ACTIVE_ORDERING_TABLE, otz);

	if ((flags & 0x80) != 0) {
		renderTrianglePrimitive(0x20202,
					x - 1, y - 1,
					x + 0xd, y - 1,
					x + 0xd, y + 0xc,
					otz, 0);
		renderTrianglePrimitive(0x20202,
					x - 1, y - 1,
					x - 1, y + 0xc,
					x + 0xd,
					y + 0xc,
					otz, 0);
	}
}

INCLUDE_ASM("asm/main/nonmatchings/overworld_status_boxes", renderSpriteBox);

INCLUDE_ASM("asm/main/nonmatchings/overworld_status_boxes", drawDigimonStatsStrings);

void renderDigimonStatsBar(int32_t a, int32_t b, int32_t c, int16_t d, int16_t e)
{
	renderBoxBar(d, e, (uint8_t)(c * a / b), 2, 0x32, 0xC8, 0xC8, 0, 5);
}

INCLUDE_ASM("asm/main/nonmatchings/overworld_status_boxes", renderDigimonStatusConditions);

INCLUDE_ASM("asm/main/nonmatchings/overworld_status_boxes", renderDigiviceEntity);
