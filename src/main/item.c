#include <dw/entity.h>
#include <dw/item.h>
#include <dw/params.h>
#include <dw/particle.h>
#include <dw/types.h>
#include <dw/world_object.h>

#include "common.h"

typedef struct {
	int8_t hungerTimes[8];
	uint8_t energyCap;
	uint8_t energyThreshold;
	int8_t energyUsage;
	int16_t poopTimer;
	int16_t unk2;
	uint8_t poopSize;
	uint8_t favoriteFood;
	int8_t sleepCycle;
	int8_t favoredRegion;
	int8_t trainingType;
	int8_t defaultWeight;
	int16_t viewX;
	int16_t viewY;
	int16_t viewZ;
} RaiseData;

extern RaiseData RAISE_DATA[66];
extern TamerItem TAMER_ITEM;
extern TamerItem DROPPED_ITEMS[];
extern volatile uint8_t INVENTORY_SIZE[];
extern uint8_t MAP_LAYER_ENABLED;
extern uint8_t INVENTORY_ITEM_TYPES[30];
extern uint8_t INVENTORY_ITEM_AMOUNTS[30];
extern uint8_t INVENTORY_ITEM_NAMES[30];

void setInventorySize(uint8_t size);
void deleteDroppedItem(int16_t itemId);
void removeTamerItem(void);
void decreasePoopLevel(void);
void addWithLimit(int16_t *value, int16_t amount, int16_t limit);
void modifyLifetime(int16_t delta);
void reduceTiredness(int16_t amount);
void setTrainingBoost(int32_t flag, int32_t value, int32_t duration);
void addEnergy(int16_t amount);
void addHappiness(int16_t amount);
void addDiscipline(int16_t amount);
void addWeight(int16_t amount);
int32_t getItemCount(uint8_t type);
int32_t giveItem(uint8_t type, uint8_t amount);
void removeItem(int32_t type, uint32_t amount);


void handleEvoItems(int16_t arg);
void handleFood(int16_t arg);
void handleChips(int16_t arg);
void handleStatusItems(int16_t arg);
void handleRestore(int16_t arg);
void handleDoubleFloppy(void);
void handleMPHealingItem(unsigned char idx);
void handleHPHealingItem(unsigned char idx);
void initializeDroppedItems(void);
void spawnDroppedItems(Entity *e, int32_t type);
void getModelTile(VECTOR *pos, int16_t *outTileX, int16_t *outTileY);
void renderDroppedItem(int32_t instanceId);
void spawnItem(int32_t type, int16_t tileX, int16_t tileY);
void clearDroppedItems(void);
void renderOverworldItem(TamerItem *item);
void renderDroppedItemShadow(TamerItem *item);
int32_t pickupItem(int16_t itemId);
void handlePoopWeightLoss(int32_t type);
int32_t random(int32_t limit);
void clearTextArea(void);
void drawString(char *text, int32_t color, int32_t pos);
void setTextColor(int32_t color);
void setTamerState(int32_t state);
void setPartnerState(int8_t state);
void closeInventoryBoxes(void);
void BTL_healStatusEffect(int32_t arg);
void BTL_removeDeathCountdown(void);
void startAnimation(Entity *entity, uint8_t animId);
int32_t sprintf(char *dst, char *fmt, ...);
char *strcat(char *dst, char *src);
extern char MAIN_D_80134368;
extern char MAIN_D_80125F64[];
extern uint8_t MAIN_D_80127C5C[];
extern int16_t EVOLUTION_TARGET;
extern uint8_t HAS_USED_EVOITEM;
void addEntityText(Entity *entity, int32_t a, int32_t b, int16_t value,
		   int32_t kind);
extern int16_t MAIN_D_8013435C[4];
extern uint8_t MAIN_D_80134364[8];
extern int8_t GAME_STATE;
void initializeInventory(void);
int32_t handleMedicineHealing(int32_t sickChance, int32_t injuryChance);
void handlePortaPotty(int16_t arg);
void handleItemSickness(int16_t arg);

/* Order anchor: forces symtab indices (= metrowrap section order) to match
 * address order despite forward references. Unreferenced, so the linker's
 * --gc-sections discards it - zero bytes in the final binary. */
void *item_order_anchor[] = {
	handleItemSickness,
	setTrainingBoost,
	decreasePoopLevel,
	addWeight,
	addDiscipline,
	addHappiness,
	reduceTiredness,
	addEnergy,
	modifyLifetime,
	handlePortaPotty,
	handleMedicineHealing,
	addWithLimit,
	removeTamerItem,
	initializeInventory,
	pickupItem,
	removeItem,
	giveItem,
	getItemCount,
	renderDroppedItemShadow,
	renderOverworldItem,
	clearDroppedItems,
	deleteDroppedItem,
	spawnItem,
	renderDroppedItem,
	spawnDroppedItems,
	initializeDroppedItems,
	setInventorySize,
	handleHPHealingItem,
	handleMPHealingItem,
	handleDoubleFloppy,
	handleRestore,
	handleStatusItems,
	handleChips,
	handleFood,
	handleEvoItems,
};

INCLUDE_ASM("asm/main/nonmatchings/item", handleEvoItems);

INCLUDE_ASM("asm/main/nonmatchings/item", handleFood);

INCLUDE_ASM("asm/main/nonmatchings/item", handleChips);

INCLUDE_ASM("asm/main/nonmatchings/item", handleStatusItems);

void handleRestore(int16_t type)
{
	int16_t amount;

	if (GAME_STATE == 1) {
		BTL_removeDeathCountdown();
	}

	if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP == 0) {
		startAnimation(ENTITY_TABLE[1], 0x2C);
	}

	if (type != 0xC) {
		if (type == 0xB) {
			amount = PARTNER_ENTITY.digimonEntity.stats.base.hp / 2;
		}
	} else {
		if (GAME_STATE == 1) {
			BTL_healStatusEffect(0);
		}

		amount = 0x270F;
	}

	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentHP,
		     amount, PARTNER_ENTITY.digimonEntity.stats.base.hp);
	if (GAME_STATE == 1) {
		addEntityText(ENTITY_TABLE[1], 0, 0xB, amount, 1);
	}

	addHealingParticleEffect(ENTITY_TABLE[1], 1);
}

void handleDoubleFloppy(void)
{
	if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP != 0) {
		addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentHP,
			     0x5DC, PARTNER_ENTITY.digimonEntity.stats.base.hp);
		addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentMP,
			     0x5DC, PARTNER_ENTITY.digimonEntity.stats.base.mp);
		if (GAME_STATE == 1) {
			addEntityText(ENTITY_TABLE[1], 0, 0xB, 0x5DC, 1);
			addEntityText(ENTITY_TABLE[1], 0, 0xB, 0x5DC, 2);
		}
		addHealingParticleEffect(ENTITY_TABLE[1], 0);
	}
}

void handleMPHealingItem(unsigned char idx)
{
  if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP != 0)
  {
    addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentMP, MAIN_D_8013435C[idx - 4], PARTNER_ENTITY.digimonEntity.stats.base.mp);
    if (GAME_STATE == 1)
    {
      int16_t new_var;
      addEntityText(ENTITY_TABLE[1], 0, 0xB, new_var = MAIN_D_8013435C[idx - 4], 2);
    }
    addHealingParticleEffect(ENTITY_TABLE[1], MAIN_D_80134364[idx - 4]);
  }
}

void handleHPHealingItem(unsigned char idx)
{
  if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP != 0)
  {
    addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentHP, MAIN_D_8013435C[idx], PARTNER_ENTITY.digimonEntity.stats.base.hp);
    if (GAME_STATE == 1)
    {
      int16_t new_var;
      addEntityText(ENTITY_TABLE[1], 0, 0xB, new_var = MAIN_D_8013435C[idx], 1);
    }
    addHealingParticleEffect(ENTITY_TABLE[1], MAIN_D_80134364[idx]);
  }
}

void setInventorySize(uint8_t size)
{
	INVENTORY_SIZE[0] = size;
}

void initializeDroppedItems(void)
{
	int32_t i;

	TAMER_ITEM.worldItem.type = 0xFF;
	for (i = 0; i < 11; i++) {
		DROPPED_ITEMS[i].worldItem.type = 0xFF;
	}
}

void spawnDroppedItems(Entity *e, int32_t type)
{
	int32_t i;
	TamerItem *it;
	VECTOR *loc;

	loc = &e->posData->location;
	for (i = 0; i < 0xB; i++) {
		it = &DROPPED_ITEMS[i];
		if (it->worldItem.type == 0xFF) {
			it->worldItem.type = type;
			break;
		}
	}
	if (i != 0xB) {
		it->worldItem.spriteLocation.vx = loc->vx;
		it->worldItem.spriteLocation.vy = 0;
		it->worldItem.spriteLocation.vz = loc->vz;
		getModelTile(loc, (int16_t *)((char *)it + 0xC),
			     (int16_t *)((char *)it + 0xE));
		addObject(0x195, i, 0, renderDroppedItem);
	}
}

void renderDroppedItem(int32_t instanceId)
{
	TamerItem *item;

	if (MAP_LAYER_ENABLED != 0) {
		renderOverworldItem(item = &DROPPED_ITEMS[instanceId]);
		renderDroppedItemShadow(item);
	}
}

INCLUDE_ASM("asm/main/nonmatchings/item", spawnItem);


void deleteDroppedItem(int16_t itemId)
{
	removeObject(0x195, itemId);
	DROPPED_ITEMS[itemId].worldItem.type = 0xFF;
}

void clearDroppedItems(void)
{
	int32_t i;

	for (i = 0; i < 11; i++) {
		if (DROPPED_ITEMS[i].worldItem.type != 0xFF) {
			deleteDroppedItem(i);
		}
	}
}

INCLUDE_ASM("asm/main/nonmatchings/item", renderOverworldItem);

INCLUDE_ASM("asm/main/nonmatchings/item", renderDroppedItemShadow);

int32_t getItemCount(uint8_t type)
{
	int32_t i;
	uint8_t size;

	size = INVENTORY_SIZE[0];
	for (i = 0; i < size; i++) {
		if (INVENTORY_ITEM_TYPES[i] == type) {
			return INVENTORY_ITEM_AMOUNTS[i];
		}
	}

	return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/item", giveItem);

INCLUDE_ASM("asm/main/nonmatchings/item", removeItem);

INCLUDE_ASM("asm/main/nonmatchings/item", pickupItem);

INCLUDE_ASM("asm/main/nonmatchings/item", initializeInventory);

void removeTamerItem(void)
{
	if (TAMER_ITEM.worldItem.type != 0xFF) {
		removeObject(0x194, 0);
		TAMER_ITEM.worldItem.type = 0xFF;
	}
}

void addWithLimit(int16_t *value, int16_t amount, int16_t limit)
{
	*value += amount;
	if (*value > limit) {
		*value = limit;
	}
}

INCLUDE_ASM("asm/main/nonmatchings/item", handleMedicineHealing);

void handlePortaPotty(int16_t arg)
{
	if (PARTNER_PARA.condition & 8) {
		PARTNER_PARA.poopLevel =
			RAISE_DATA[ENTITY_TABLE[1]->type].poopTimer;
		PARTNER_PARA.condition &= ~8;
		handlePoopWeightLoss(ENTITY_TABLE[1]->type);
	}
}

void modifyLifetime(int16_t delta)
{
	int16_t *lifetime = &PARTNER_PARA.remainingLifetime;

	*lifetime += delta;
	if (*lifetime < 0) {
		*lifetime = 0;
	}
}

void addEnergy(int16_t amount)
{
	PARTNER_PARA.energyLevel += amount;
	if (PARTNER_PARA.energyLevel >
	    RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyCap) {
		PARTNER_PARA.energyLevel =
			RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyCap;
	}
}

void reduceTiredness(int16_t amount)
{
	PARTNER_PARA.tiredness -= amount;
	if (PARTNER_PARA.tiredness <= 0) {
		PARTNER_PARA.tiredness = 0;
	}
}

void addHappiness(int16_t amount)
{
	PARTNER_PARA.happiness += amount;
	if (PARTNER_PARA.happiness >= 0x64) {
		PARTNER_PARA.happiness = 0x64;
	}
}

void addDiscipline(int16_t amount)
{
	PARTNER_PARA.discipline += amount;
	if (PARTNER_PARA.discipline >= 0x64) {
		PARTNER_PARA.discipline = 0x64;
	}
}

void addWeight(int16_t amount)
{
	PARTNER_PARA.weight += amount;
	if (PARTNER_PARA.weight >= 0x64) {
		PARTNER_PARA.weight = 0x63;
	}

	if (PARTNER_PARA.weight <= 0) {
		PARTNER_PARA.weight = 1;
	}
}

void decreasePoopLevel(void)
{
	PARTNER_PARA.poopLevel--;
}

void setTrainingBoost(int32_t flag, int32_t value, int32_t duration)
{
	PARTNER_PARA.trainBoostFlag = flag;
	PARTNER_PARA.trainBoostValue = value;
	PARTNER_PARA.trainBoostTimer = duration * 1200;
}

INCLUDE_ASM("asm/main/nonmatchings/item", handleItemSickness);
