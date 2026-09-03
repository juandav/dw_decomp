#include <libgte.h>
#include <dw/entity.h>
#include <dw/params.h>
#include <dw/types.h>

#include "common.h"

extern int8_t MAP_COLLISION_DATA[];
void getModelTile(VECTOR *pos, int16_t *outTileX, int16_t *outTileY);
void loadMapCollisionData(int8_t *src);
int32_t getTileTrigger(VECTOR *pos);
int32_t checkMapCollisionX(Entity *entity, int32_t direction);
int32_t checkMapCollisionY(Entity *entity, int32_t direction);
void setRectImpassible(int32_t x, int32_t y, int32_t w, int32_t h);
void setRectangleImpassable(int32_t x, int32_t y, int32_t r);

static inline int32_t copyValue(int32_t value)
{
	return value;
}

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

int32_t checkMapCollisionX(Entity *entity, int32_t direction)
{
	VECTOR *position;
	int16_t radius;
	int32_t halfRadius;
	int32_t rightRadius;
	int16_t leftPos;
	int16_t rightPos;
	int16_t edgePos;
	int16_t leftTile;
	int16_t rightTile;
	int16_t edgeTile;

	position = &entity->posData->location;
	radius = DIGIMON_DATA[entity->type].radius;
	halfRadius = radius / 2;
	rightRadius = copyValue(halfRadius);

	leftPos = position->vx - halfRadius;
	leftTile = leftPos / 100 + 0x32;
	if (leftPos < 0) {
		leftTile--;
	}

	rightPos = position->vx + rightRadius;
	rightTile = rightPos / 100 + 0x32;
	if (rightPos < 0) {
		rightTile--;
	}

	if (direction == 0) {
		edgePos = position->vz + radius;
	} else {
		edgePos = position->vz - radius;
	}
	edgeTile = 0x31 - edgePos / 100;
	if (edgePos < 0) {
		edgeTile++;
	}

	for (; leftTile <= rightTile; leftTile++) {
		if ((((uint8_t *)MAP_COLLISION_DATA)[leftTile + edgeTile * 100] &
		     0x80) != 0) {
			return 1;
		}
	}

	return 0;
}

int32_t checkMapCollisionY(Entity *entity, int32_t direction)
{
	int16_t edgePos;
	VECTOR *position;
	int16_t radius;
	int32_t halfRadius;
	int32_t bottomRadius;
	int32_t originalZ;
	int32_t topValue;
	int16_t topPos;
	int16_t edgeTile;
	int16_t topTile;
	int16_t bottomTile;
	int16_t bottomPos;

	position = &entity->posData->location;
	radius = DIGIMON_DATA[entity->type].radius;
	if (direction == 0) {
		edgePos = position->vx - radius;
	} else {
		edgePos = position->vx + radius;
	}
	edgeTile = edgePos / 100 + 0x32;
	if (edgePos < 0) {
		edgeTile--;
	}

	halfRadius = radius / 2;
	bottomRadius = copyValue(halfRadius);
	topValue = originalZ = position->vz;
	topValue += halfRadius;
	topPos = topValue;
	topTile = 0x31 - topPos / 100;
	if (topPos < 0) {
		topTile++;
	}

	bottomPos = originalZ - bottomRadius;
	bottomTile = 0x31 - bottomPos / 100;
	if (bottomPos < 0) {
		bottomTile++;
	}

	for (; topTile <= bottomTile; topTile++) {
		if ((((uint8_t *)MAP_COLLISION_DATA)[edgeTile + topTile * 100] &
		     0x80) != 0) {
			return 1;
		}
	}

	return 0;
}

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
