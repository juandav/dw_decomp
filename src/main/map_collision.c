#include <libgte.h>
#include <dw/types.h>

#include "common.h"

extern int8_t MAP_COLLISION_DATA[];
void getModelTile(VECTOR *pos, int16_t *outTileX, int16_t *outTileY);
void loadMapCollisionData(int8_t *src);
int32_t getTileTrigger(VECTOR *pos);
void setRectImpassible(int32_t x, int32_t y, int32_t w, int32_t h);
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

static inline int32_t copyValue(int32_t value)
{
	return value;
}

void setRectangleImpassable(int32_t x, int32_t y, int32_t radius)
{
	int32_t originalRadius;
	int32_t originalY;
	int32_t tileX;
	int32_t tileY;

	originalRadius = copyValue(radius);
	originalY = copyValue(y);
	for (tileY = y - radius;
	     tileY < originalY + originalRadius;
	     tileY++) {
		for (tileX = x - originalRadius; tileX < x + radius; tileX++) {
			((uint8_t *)MAP_COLLISION_DATA)[tileX + tileY * 100] = 0x80;
		}
	}
}

void setRectImpassible(int32_t x, int32_t y, int32_t width, int32_t height)
{
	int32_t originalY;
	int32_t tileX;

	originalY = copyValue(y);
	for (; y < originalY + height; y++) {
		for (tileX = x; tileX < x + width; tileX++) {
			((uint8_t *)MAP_COLLISION_DATA)[tileX + y * 100] = 0x80;
		}
	}
}
