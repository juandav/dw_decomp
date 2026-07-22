#include <libgte.h>
#include <dw/types.h>

#include "common.h"

extern int8_t MAP_COLLISION_DATA[];
void getModelTile(VECTOR *pos, int16_t *outTileX, int16_t *outTileY);
void loadMapCollisionData(int8_t *src);
int32_t getTileTrigger(VECTOR *pos);
void setRectImpassible(int32_t x, uint8_t y, int32_t w, int32_t h);
void setRectangleImpassable(int32_t x, int32_t y, int32_t r);

void loadMapCollisionData(int8_t *src)
{
	int32_t i;

	for (i = 0; i < 0x2710; i++) {
		MAP_COLLISION_DATA[i] = *src++;
	}
}

int32_t getTileTrigger(VECTOR *pos)
{
	int16_t tx;
	int16_t tz;
	int32_t idx;

	tx = pos->vx / 100 + 0x32;
	tz = 0x32 - pos->vz / 100;
	if (pos->vx < 0) {
		tx = tx - 1;
	}
	if (pos->vz > 0) {
		tz = tz - 1;
	}
	idx = tx + tz * 0x64;
	if ((((uint8_t *)MAP_COLLISION_DATA)[idx] != 0) &&
	    (((uint8_t *)MAP_COLLISION_DATA)[idx] != 0x80) &&
	    (((uint8_t *)MAP_COLLISION_DATA)[idx] != 0xFF80)) {
		return (int8_t)((uint8_t *)MAP_COLLISION_DATA)[idx];
	}
	if (((uint8_t *)MAP_COLLISION_DATA)[idx] == 0) {
		return 0;
	} else {
		return -1;
	}
}

INCLUDE_ASM("asm/main/nonmatchings/map_collision", checkMapCollisionX);

INCLUDE_ASM("asm/main/nonmatchings/map_collision", checkMapCollisionY);

void getModelTile(VECTOR *pos, int16_t *outTileX, int16_t *outTileY)
{
	*outTileX = pos->vx / 100 + 0x32;
	*outTileY = 0x32 - pos->vz / 100;
	if (pos->vx < 0) {
		*outTileX -= 1;
	}
	if (pos->vz > 0) {
		*outTileY -= 1;
	}
}

void setRectangleImpassable(int32_t x, int32_t y, int32_t r)
{
	int32_t col;
	int32_t row;
	int32_t rc;
	int32_t yc;

	/* The doubled assignments are load-bearing: a plain "rc = r" is folded
	   back to the parameter and the two copies at the top disappear. */
	rc = (rc = r);
	yc = (yc = y);
	for (row = y - r; row < yc + rc; row++) {
		for (col = x - rc; col < x + r; col++) {
			((uint8_t *)MAP_COLLISION_DATA)[row * 0x64 + col] = 0x80;
		}
	}
}

void setRectImpassible(int32_t x, uint8_t y, int32_t w, int32_t h)
{
	int32_t row;
	int32_t col;

	for (row = y; row < y + h; row++) {
		for (col = x; col < x + w; col++) {
			((uint8_t *)MAP_COLLISION_DATA)[row * 0x64 + col] = 0x80;
		}
	}
}
