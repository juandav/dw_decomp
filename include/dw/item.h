#ifndef DW_ITEM_H
#define DW_ITEM_H

#include <libgte.h>

#include <dw/entity.h>
#include <dw/types.h>

typedef struct
{
	char name[20];
	int32_t value;
	uint16_t meritValue;
	int16_t sortingValue;
	uint8_t itemColor;
	uint8_t droppable;
	uint16_t unk;
} Item;

typedef struct {
	SVECTOR spriteLocation;
	int32_t type;
} WorldItem;

typedef struct {
	WorldItem worldItem;
	int32_t time;
} TamerItem;

typedef struct {
	WorldItem worldItem;
	int16_t tileX;
	int16_t tileY;
} DroppedItem;

extern Item ITEM_PARA[];
extern TamerItem TAMER_ITEM;
extern DroppedItem DROPPED_ITEMS[11];

void initializeInventory(void);
void setInventorySize(uint8_t size);
int32_t giveItem(uint32_t type, uint8_t amount);
void removeItem(int32_t type, uint32_t amount);
int32_t getItemCount(int32_t type);

void initializeDroppedItems(void);
void clearDroppedItems(void);
void spawnItem(int32_t type, int16_t tileX, int16_t tileY);
void spawnDroppedItems(Entity *e, int32_t type);
int32_t pickupItem(int16_t itemId);
void removeTamerItem(void);
void renderOverworldItem(WorldItem *item);

void addWithLimit(/* int16_t *value, int16_t amount, int16_t limit */);

#endif
