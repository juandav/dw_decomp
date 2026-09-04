#include <stdio.h>
#include <strings.h>

#include <libgs.h>
#include <mwinline_n.h>

#include <dw/btl.h>
#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/item.h>
#include <dw/math.h>
#include <dw/params.h>
#include <dw/particle.h>
#include <dw/partner.h>
#include <dw/script.h>
#include <dw/types.h>
#include <dw/world_object.h>

#include "common.h"

typedef struct {
	uint8_t array[30];
} InventoryTable;

void deleteDroppedItem(int16_t itemId);
void setUVDataPolyFT4(POLY_FT4 *p, int32_t u, int32_t v, int32_t w, int32_t h);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY, int32_t width,
		       int32_t height);
void setItemTexture(POLY_FT4 *prim, int32_t type);
void decreasePoopLevel(void);
void modifyLifetime(int16_t delta);
void reduceTiredness(int16_t amount);
void setTrainingBoost(int32_t flag, int32_t value, int32_t duration);
void addEnergy(int16_t amount);
void addHappiness(int16_t amount);
void addDiscipline(int16_t amount);
void addWeight(int16_t amount);
void handleEvoItems(int16_t arg);
void handleFood(int16_t arg);
void handleChips(int32_t chipId);
void handleStatusItems(int32_t itemId);
void handleRestore(int16_t arg);
void handleDoubleFloppy(int32_t itemId);
void handleMPHealingItem(unsigned char idx);
void handleHPHealingItem(unsigned char idx);
void getModelTile(VECTOR *pos, int16_t *outTileX, int16_t *outTileY);
void renderDroppedItem(int32_t instanceId);
void renderDroppedItemShadow(WorldItem *item);
void handlePoopWeightLoss(int32_t type);
void clearTextArea(void);
void closeInventoryBoxes(void);
void BTL_healStatusEffect(int32_t arg);
void addEntityText(Entity *entity, int32_t a, int32_t b, int16_t value,
		   int32_t kind);
int32_t handleMedicineHealing(int32_t injuryChance, int32_t sicknessChance);
void handlePortaPotty(void);
void handleItemSickness(int16_t arg);
void addTamerLevel(int32_t chance, int32_t amount);

extern uint8_t MAP_LAYER_ENABLED;
extern InventoryTable INVENTORY_ITEM_TYPES;
extern InventoryTable INVENTORY_ITEM_AMOUNTS;
extern InventoryTable INVENTORY_ITEM_NAMES;
extern InventoryTable DEFAULT_ITEM_AMOUNTS;
extern InventoryTable DEFAULT_ITEM_TYPES;
extern int32_t VIEWPORT_DISTANCE;
extern char MAIN_D_80134368;
extern char MAIN_D_80125F64[];
extern uint8_t MAIN_D_80127C5C[];
extern int16_t EVOLUTION_TARGET;
extern uint8_t HAS_USED_EVOITEM;
extern int16_t MAIN_D_8013435C[4];
extern uint8_t MAIN_D_80134364[8];

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

static void handleEvoItems__garbage__(void)
{
	int32_t t0;
	int32_t t1;
	int32_t t2;
	int32_t t3;
	int32_t t4;
	int32_t t5;

	t0 = DROPPED_ITEMS[0].tileX;
	t1 = DROPPED_ITEMS[1].tileX;
	t2 = DROPPED_ITEMS[2].tileX;
	t3 = DROPPED_ITEMS[3].tileX;
	t4 = DROPPED_ITEMS[4].tileX;
	t5 = DROPPED_ITEMS[5].tileX;
	DROPPED_ITEMS[0].tileX = t0 + 1;
	DROPPED_ITEMS[1].tileX = t1 + 2;
	DROPPED_ITEMS[2].tileX = t2 + 3;
	DROPPED_ITEMS[3].tileX = t3 + 4;
	DROPPED_ITEMS[4].tileX = t4 + 5;
	DROPPED_ITEMS[5].tileX = t5 + 6;
}

void handleEvoItems(int16_t item)
{
	int32_t target;
	int16_t level;

	if (item >= 0x7d) {
		if (item == 0x7d) {
			EVOLUTION_TARGET = 0x40;
		}
		if (item == 0x7e) {
			EVOLUTION_TARGET = 0x3f;
		}
		if (item == 0x7f) {
			EVOLUTION_TARGET = 0x41;
		}
	} else {
		target = MAIN_D_80127C5C[item - 0x47];
		level = DIGIMON_DATA[target].level - 1;
		if (level != DIGIMON_DATA[ENTITY_TABLE[1]->type].level) {
			return;
		}
		EVOLUTION_TARGET = MAIN_D_80127C5C[item - 0x47];
	}
	HAS_USED_EVOITEM = 1;
	removeTamerItem();
	closeInventoryBoxes();
	setTamerState(6);
	setPartnerState(0xd);
}

INCLUDE_ASM("asm/main/nonmatchings/item", handleFood);

void handleStatusItems(int32_t itemId)
{
	int32_t cured;

	if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP != 0) {
		switch (itemId) {
		case 8:
			BTL_healStatusEffect(1);
			return;
		case 9:
			if (GAME_STATE == 1) {
				BTL_healStatusEffect(0);
			}
			handleDoubleFloppy(itemId);
			return;
		case 10:
			COMBAT_DATA_PTR->fighter[0].flags |= 0x100;
			break;
		case 0xd:
			cured = handleMedicineHealing(3, 2);
			if (cured == 1) {
				addHealingParticleEffect(ENTITY_TABLE[1], 0);
				return;
			}
			break;
		case 0xe:
			cured = handleMedicineHealing(3, 10);
			if (cured == 1) {
				addHealingParticleEffect(ENTITY_TABLE[1], 0);
			}
		}
	}
}

void handleChips(int32_t chipId)
{
	int16_t lifetime;
	int16_t off;
	int16_t hp;
	int16_t mp;
	int16_t def;
	int16_t speed;
	int16_t brain;
	int16_t s;

	lifetime = 0;
	mp = s = 0U;
	hp = s = 0U;
	brain = s = 0U;
	speed = s = 0U;
	def = s = 0U;
	off = s = 0U;
	switch (chipId) {
	case 0x16:
		if (IS_SCRIPT_PAUSED == 1) {
			removeTamerItem();
			callScriptSection(0, 0x4dd, 0);
		}
		break;
	case 0x17:
		off = 0x32;
		break;
	case 0x18:
		def = 0x32;
		break;
	case 0x19:
		brain = 0x32;
		break;
	case 0x1a:
		speed = 0x32;
		break;
	case 0x1b:
		hp = 500;
		break;
	case 0x1c:
		mp = 500;
		break;
	case 0x1d:
		off = 100;
		brain = 100;
		lifetime = 0xffffffe8;
		break;
	case 0x1e:
		def = 100;
		speed = 100;
		lifetime = 0xffffffe8;
		break;
	case 0x1f:
		hp = 1000;
		mp = 1000;
		lifetime = 0xffffffe8;
		break;
	case 0x20:
		handlePortaPotty();
		return;
	}
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.hp, hp, 9999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.mp, mp, 9999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.off, off, 999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.def, def, 999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.speed, speed, 999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.brain, brain, 999);
	modifyLifetime(lifetime);
	if ((0x1c < chipId) && (chipId < 0x20)) {
		addTamerLevel(10, 0xffffffff);
	}
}

void handleRestore(int16_t type)
{
	int16_t amount;

	if (GAME_STATE == 1) {
		BTL_removeDeathCountdown();
	}

	if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP == 0) {
		startAnimation(ENTITY_TABLE[1], 0x2c);
	}

	if (type != 0xc) {
		if (type == 0xb) {
			amount = PARTNER_ENTITY.digimonEntity.stats.base.hp / 2;
		}
	} else {
		if (GAME_STATE == 1) {
			BTL_healStatusEffect(0);
		}

		amount = 0x270f;
	}

	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentHP,
		     amount, PARTNER_ENTITY.digimonEntity.stats.base.hp);
	if (GAME_STATE == 1) {
		addEntityText(ENTITY_TABLE[1], 0, 0xb, amount, 1);
	}

	addHealingParticleEffect(ENTITY_TABLE[1], 1);
}

void handleDoubleFloppy(int32_t itemId)
{
	if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP != 0) {
		addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentHP,
			     0x5dc, PARTNER_ENTITY.digimonEntity.stats.base.hp);
		addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentMP,
			     0x5dc, PARTNER_ENTITY.digimonEntity.stats.base.mp);
		if (GAME_STATE == 1) {
			addEntityText(ENTITY_TABLE[1], 0, 0xb, 0x5dc, 1);
			addEntityText(ENTITY_TABLE[1], 0, 0xb, 0x5dc, 2);
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
			addEntityText(ENTITY_TABLE[1], 0, 0xb, new_var = MAIN_D_8013435C[idx - 4], 2);
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
			addEntityText(ENTITY_TABLE[1], 0, 0xb, new_var = MAIN_D_8013435C[idx], 1);
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

	TAMER_ITEM.worldItem.type = 0xff;
	for (i = 0; i < 11; i++) {
		DROPPED_ITEMS[i].worldItem.type = 0xff;
	}
}

void spawnDroppedItems(Entity *e, int32_t type)
{
	int32_t i;
	DroppedItem *it;
	VECTOR *loc;

	loc = &e->posData->location;
	for (i = 0; i < 0xb; i++) {
		it = &DROPPED_ITEMS[i];
		if (it->worldItem.type == 0xff) {
			it->worldItem.type = type;
			break;
		}
	}
	if (i != 0xb) {
		it->worldItem.spriteLocation.vx = loc->vx;
		it->worldItem.spriteLocation.vy = 0;
		it->worldItem.spriteLocation.vz = loc->vz;
		getModelTile(loc, (int16_t *)((char *)it + 0xc),
			     (int16_t *)((char *)it + 0xe));
		addObject(0x195, i, 0, renderDroppedItem);
	}
}

void renderDroppedItem(int32_t instanceId)
{
	WorldItem *item;

	if (MAP_LAYER_ENABLED != 0) {
		renderOverworldItem(item = &DROPPED_ITEMS[instanceId].worldItem);
		renderDroppedItemShadow(item);
	}
}

static void spawnItem__garbage__(void)
{
	int32_t t0;
	int32_t t1;
	int32_t t2;
	int32_t t3;
	int32_t t4;
	int32_t t5;
	int32_t t6;
	int32_t t7;
	int32_t t8;
	int32_t t9;

	t0 = DROPPED_ITEMS[0].tileX;
	t1 = DROPPED_ITEMS[1].tileX;
	t2 = DROPPED_ITEMS[2].tileX;
	t3 = DROPPED_ITEMS[3].tileX;
	t4 = DROPPED_ITEMS[4].tileX;
	t5 = DROPPED_ITEMS[5].tileX;
	t6 = DROPPED_ITEMS[6].tileX;
	t7 = DROPPED_ITEMS[7].tileX;
	t8 = DROPPED_ITEMS[8].tileX;
	t9 = DROPPED_ITEMS[9].tileX;
	DROPPED_ITEMS[0].tileX = t0 + 1;
	DROPPED_ITEMS[1].tileX = t1 + 2;
	DROPPED_ITEMS[2].tileX = t2 + 3;
	DROPPED_ITEMS[3].tileX = t3 + 4;
	DROPPED_ITEMS[4].tileX = t4 + 5;
	DROPPED_ITEMS[5].tileX = t5 + 6;
	DROPPED_ITEMS[6].tileX = t6 + 7;
	DROPPED_ITEMS[7].tileX = t7 + 8;
	DROPPED_ITEMS[8].tileX = t8 + 9;
	DROPPED_ITEMS[9].tileX = t9 + 10;
}

void spawnItem(int32_t itemId, int16_t tileX, int16_t tileY)
{
	int32_t i;

	for (i = 0; i < 0xb; i++) {
		if (DROPPED_ITEMS[i].worldItem.type == 0xff) {
			DROPPED_ITEMS[i].worldItem.type = itemId;
			DROPPED_ITEMS[i].tileX = tileX;
			DROPPED_ITEMS[i].tileY = tileY;
			DROPPED_ITEMS[i].worldItem.spriteLocation.vx =
				(tileX - 0x32) * 100 + 0x32;
			DROPPED_ITEMS[i].worldItem.spriteLocation.vy =
				ENTITY_TABLE[0]->posData->location.vy;
			DROPPED_ITEMS[i].worldItem.spriteLocation.vz =
				(0x32 - tileY) * 100 - 0x32;
			addObject(0x195, i, 0, renderDroppedItem);
			return;
		}
	}
}

void deleteDroppedItem(int16_t itemId)
{
	removeObject(0x195, itemId);
	DROPPED_ITEMS[itemId].worldItem.type = 0xff;
}

void clearDroppedItems(void)
{
	int32_t i;

	for (i = 0; i < 11; i++) {
		if (DROPPED_ITEMS[i].worldItem.type != 0xff) {
			deleteDroppedItem(i);
		}
	}
}

void renderOverworldItem(WorldItem *item)
{
	DVECTOR screen;
	int32_t otz;
	POLY_FT4 *prim;
	int16_t width;
	uint32_t depth;

	GsSetLsMatrix(&GsWSMATRIX);
	gte_ldv0(&item->spriteLocation);
	gte_rtps();
	gte_stsxy(&screen);
	gte_stszotz(&otz);
	width = (uint32_t)(VIEWPORT_DISTANCE << 7) / (depth = (uint32_t)(otz * 4));
	otz = otz >> 2;
	if ((0 < otz) && (otz < 0x1000)) {
		prim = (POLY_FT4 *)GsGetWorkBase();
		SetPolyFT4(prim);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->tpage = getTPage(0, 0, 320, 0);
		setItemTexture(prim, item->type);
		if (width >= 0x20) {
			setUVWH(prim, prim->u0, prim->v0, 0xf, 0xf);
		}
		setPosDataPolyFT4(prim, screen.vx - (width >> 1),
				  screen.vy - (width >> 1), width, width);
		AddPrim(&ACTIVE_ORDERING_TABLE->org[otz], prim++);
		GsSetWorkBase((PACKET *)prim);
	}
}

void renderDroppedItemShadow(WorldItem *item)
{
	SVECTOR p0;
	SVECTOR p1;
	SVECTOR p2;
	SVECTOR p3;
	int32_t otz;
	POLY_FT4 *prim;
	int16_t px;
	int16_t pz;

	GsSetLsMatrix(&GsWSMATRIX);
	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	SetSemiTrans(prim, 1);
	prim->tpage = getTPage(1, 2, 832, 256);
	setClut(prim, 0, 0x1e7);
	setUVDataPolyFT4(prim, 0x40, 0x80, 0x3f, 0x3f);
	setRGB0(prim, 0x30, 0x30, 0x30);
	px = item->spriteLocation.vx;
	pz = item->spriteLocation.vz;
	p0.vx = px - 100;
	p0.vy = 0;
	p0.vz = pz - 100;
	p1.vx = px + 100;
	p1.vy = 0;
	p1.vz = pz - 100;
	p2.vx = px - 100;
	p2.vy = 0;
	p2.vz = pz + 100;
	p3.vx = px + 100;
	p3.vy = 0;
	p3.vz = pz + 100;
	gte_ldv3(&p0, &p1, &p2);
	gte_rtpt();
	gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
	gte_stszotz(&otz);
	gte_ldv0(&p3);
	gte_rtps();
	gte_stsxy(&prim->x3);
	AddPrim(&ACTIVE_ORDERING_TABLE->org[0xffd], prim++);
	GsSetWorkBase((PACKET *)prim);
}

int32_t getItemCount(int32_t type)
{
	int32_t i;
	uint8_t size;

	size = INVENTORY_SIZE[0];
	for (i = 0; i < size; i++) {
		if (INVENTORY_ITEM_TYPES.array[i] == type) {
			return INVENTORY_ITEM_AMOUNTS.array[i];
		}
	}

	return 0;
}

int32_t giveItem(uint32_t item, uint8_t amount)
{
	int16_t used[30];
	int32_t i;
	int32_t j;
	int32_t n;
	uint8_t *p;
	uint8_t *q;

	for (i = 0; i < (n = *(volatile uint8_t *)INVENTORY_SIZE); i++) {
		if (INVENTORY_ITEM_TYPES.array[i] == item) {
			q = &INVENTORY_ITEM_TYPES.array[i] + 0x1e;
			p = q - 0x1e;
			if (q[0] != 0x63) {
				q[0] += amount;
				if (q[0] >= 0x64) {
					q[0] = 0x63;
				}
				return 1;
			}
			return 0;
		}
	}
	for (i = 0; i < n; i++) {
		if (INVENTORY_ITEM_TYPES.array[i] == 0xff) {
			p = &INVENTORY_ITEM_TYPES.array[i];
			p[0] = item;
			INVENTORY_ITEM_AMOUNTS.array[i] = amount;
			for (j = 0; j < INVENTORY_SIZE[0]; j++) {
				used[j] = 0;
			}
			for (j = 0; j < INVENTORY_SIZE[0]; j++) {
				if (INVENTORY_ITEM_NAMES.array[j] != 0xff) {
					used[INVENTORY_ITEM_NAMES.array[j]] = 1;
				}
			}
			for (j = 0; j < INVENTORY_SIZE[0]; j++) {
				if (used[j] == 0) {
					INVENTORY_ITEM_NAMES.array[i] = j;
					break;
				}
			}
			return 1;
		}
	}
	return 0;
}

void removeItem(int32_t type, uint32_t amount)
{
	int32_t new_var;
	int32_t i;
	uint8_t *new_var2;
	uint8_t *amt;
	if (type == 0xff)
	{
		return;
	}
	for (i = 0; i < INVENTORY_SIZE[0]; i++)
	{
		if (INVENTORY_ITEM_TYPES.array[i] == type)
		{
			amt = (&INVENTORY_ITEM_TYPES.array[i]) + 0x1e;
			new_var2 = amt;
			new_var = amount < (*new_var2);
			if (new_var)
			{
				*amt = (*new_var2) - amount;
			}
			else
			{
				*new_var2 = 0;
				INVENTORY_ITEM_TYPES.array[i] = 0xff;
				INVENTORY_ITEM_NAMES.array[i] = 0xff;
			}
		}
	}

}

int32_t pickupItem(int16_t itemId)
{
	int32_t *tp;
	int32_t got;

	tp = &DROPPED_ITEMS->worldItem.type;
	got = giveItem((uint8_t)tp[itemId << 2], 1);
	if (got != 0) {
		deleteDroppedItem(itemId);
	}
	return got;
}

void initializeInventory(void)
{
	InventoryTable amounts;
	InventoryTable types;
	int32_t i;

	for (i = 0; i < 0x1e; ++i) {
		INVENTORY_ITEM_TYPES.array[i] = 0xff;
		INVENTORY_ITEM_AMOUNTS.array[i] = 0;
		INVENTORY_ITEM_NAMES.array[i] = 0xff;
	}

	INVENTORY_SIZE[0] = 10;
	amounts = DEFAULT_ITEM_AMOUNTS;
	types = DEFAULT_ITEM_TYPES;

	for (i = 0; i < 0x1e; ++i) {
		INVENTORY_ITEM_TYPES.array[i] = types.array[i];
		INVENTORY_ITEM_AMOUNTS.array[i] = amounts.array[i];
		INVENTORY_ITEM_NAMES.array[i] = i;
	}

	INVENTORY_SIZE[0] = 0x1e;
}

void removeTamerItem(void)
{
	if (TAMER_ITEM.worldItem.type != 0xff) {
		removeObject(0x194, 0);
		TAMER_ITEM.worldItem.type = 0xff;
	}
}

void addWithLimit(value, amount, limit)
	int16_t *value;
	int16_t amount;
	int16_t limit;
{
	*value += amount;
	if (*value > limit) {
		*value = limit;
	}
}

int32_t handleMedicineHealing(int32_t injuryChance, int32_t sicknessChance)
{
	int32_t roll;
	int32_t cured;

	if (((PARTNER_PARA.condition & 0x20) != 0) &&
	    (roll = random(3), (int16_t)roll < injuryChance)) {
		PARTNER_PARA.condition &= 0xffffffdf;
		PARTNER_PARA.injuryTimer = 0;
	}
	if (((PARTNER_PARA.condition & 0x40) != 0) &&
	    (roll = random(10), (int16_t)roll < sicknessChance)) {
		PARTNER_PARA.condition &= 0xffffffbf;
		PARTNER_PARA.sicknessTimer = 0;
		PARTNER_PARA.areaEffectTimer = 0;
		cured = 1;
	} else {
		cured = 0;
	}
	return cured;
}

void handlePortaPotty(void)
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

void handleItemSickness(int16_t chance)
{
	int32_t new_var;
	int16_t r;
	char buf[0x18];
	r = (int16_t) random(0x64);
	new_var = PARTNER_PARA.condition & 0x40;
	if ((r < chance) && (!new_var))
	{
		PARTNER_PARA.condition |= 0x40;
		PARTNER_PARA.timesBeingSick++;
		PARTNER_PARA.sicknessTimer = 1;
		if (PARTNER_PARA.condition & 0x20)
		{
			PARTNER_PARA.condition &= ~0x20;
			PARTNER_PARA.injuryTimer = 0;
		}
		setTamerState(0x14);
		clearTextArea();
		setTextColor(0xa);
		sprintf(buf, &MAIN_D_80134368, PARTNER_ENTITY.name);
		strcat(buf, MAIN_D_80125F64);
		drawString(buf, 0, 0x78);
	}
}
