#include <libgs.h>
#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/params.h>
#include <dw/types.h>
#include <dw/world_object.h>

#include "common.h"

extern uint8_t MAIN_D_80150CD0[];
int8_t MAIN_D_8012AA28[24] = {
	0, -7, -5, -5, -2, -1, 1, 2, 5, 5, 7, -4,
	-3, -2, 0, -1, 1, 0, 2, 3, 0, 0, 0, 0,
};
uint8_t MAIN_D_801343EC[8] = { 0xB0, 0xC8, 0xB8, 0xC0, 0xD0, 0, 0, 0 };
void getEntityScreenPos(Entity *entity, int32_t mode, int16_t *out);
void convertValueToDigits(int32_t n, int32_t value, int32_t *outCount,
			  int32_t *digits);
extern int8_t GAME_STATE;

void MAIN_func_800E5724(int32_t color, int32_t n, int32_t x, int32_t y,
			int32_t value, int32_t layer);
void MAIN_func_800E60E0(int32_t x, int32_t y, int32_t u, int32_t layer);

void MAIN_func_800DF804();
void addEntityText(Entity *entity, int32_t slot, int8_t color, int32_t value, uint8_t flag);
void setCombatTextPosition(Entity *entity, int16_t *t);
void renderEntityText(int32_t instanceId);
void removeEntityText(int32_t id);

void *entity_text_order_anchor[] = {
	removeEntityText,
	renderEntityText,
	setCombatTextPosition,
	addEntityText,
	MAIN_func_800DF804,
};

void MAIN_func_800DF804()
{
	int32_t id;
	int32_t i;
	int32_t ofs;
	uint8_t *p;

	id = 0;
	p = &MAIN_D_80150CD0[id * 0x8C];
	while (id < 4) {
		i = 0;
		ofs = 0;
		while (i < 8) {
			(&p[ofs])[0x10] = 0xFF;
			(&p[i])[0x84] = 0xFF;
			i += 1;
			ofs += 0x10;
		}
		*(int32_t *)p = 0;
		id += 1;
		p += 0x8C;
	}
}

/* CodeWarrior retains scheduler state between functions. This unused function
 * primes the state needed by addEntityText and is linker-GC'd. */
static int32_t primeAddEntityTextScheduler(int32_t a, int32_t b, int32_t c,
					   int32_t d)
{
	int32_t value = a;

	value = (value << 1) + b;
	value = value * 3 + c;
	value ^= d;
	value += MAIN_D_80150CD0[value & 7];
	value += MAIN_D_80150CD0[(value & 7) + 8];
	value = (value << 3) - value;
	value -= b;
	value += c + d;
	value = (value << 1) + b;
	value = value * 3 + c;
	value ^= d;

	return value;
}

void addEntityText(Entity *entity, int32_t slot, int8_t color, int32_t value,
		   uint8_t flag)
{
	int32_t digits[4];
	int32_t *entry;
	int32_t ofs;
	int32_t j;
	int32_t i;
	uint8_t *data;

	data = &MAIN_D_80150CD0[slot * 0x8C];
	if (*(int32_t *)data == 8) {
		return;
	}

	i = 0;
	ofs = 0;
	while (i < 8) {
		if ((&data[ofs])[0x10] == 0xFF) {
			break;
		}
		i += 1;
		ofs += 0x10;
	}

	entry = (int32_t *)(&data[i * 0x10] + 4);
	convertValueToDigits(4, value, &entry[1], digits);
	entry[0] = value;
	((uint8_t *)entry)[0xD] = color;
	((uint8_t *)entry)[0xE] = flag;
	((uint8_t *)entry)[0xC] = 0;
	setCombatTextPosition(entity, (int16_t *)entry);
	if (GAME_STATE != 4) {
		((int16_t *)entry)[4] += *(int32_t *)data * 4;
		((int16_t *)entry)[5] += *(int32_t *)data * 4;
	} else {
		((int16_t *)entry)[4] = *(int32_t *)data * 4;
		((int16_t *)entry)[5] = *(int32_t *)data * 4;
	}

	for (j = 0; j < 8; j++) {
		if ((&data[j])[0x84] == 0xFF) {
			(&data[j])[0x84] = i;
			break;
		}
	}

	if (*(int32_t *)data == 0) {
		addObject(0x192, slot, 0, renderEntityText);
	}
	*(int32_t *)data += 1;
}

void setCombatTextPosition(Entity *entity, int16_t *t)
{
	int16_t xy[2];

	getEntityScreenPos(entity, 0, xy);
	t[4] = xy[0];
	t[5] = xy[1] - 8;
}

void renderEntityText(int32_t instanceId)
{
	int16_t screenPos[2];
	uint8_t *data;
	uint8_t *entryBase;
	uint8_t *entry;
	uint8_t *frame;
	uint8_t activeIndex;
	int32_t i;
	uint32_t combatId;
	int32_t id;
	int32_t index;

	id = instanceId;
	data = &MAIN_D_80150CD0[id * 0x8C];
	i = 0;
	combatId = id;
	while (i < 8) {
		activeIndex = (&data[i])[0x84];
		index = activeIndex;
		if (activeIndex == 0xFF) {
			break;
		}
		entryBase = &data[index * 0x10];
		entry = &entryBase[4];
		frame = &entry[0xC];
		activeIndex = *frame;
		index = activeIndex;
		if (activeIndex == 0xFF) {
			break;
		}
		if (i != 0 && (&data[(&data[i])[0x83] * 0x10])[0x10] < 11) {
			break;
		}
		if ((uint32_t)index < 21) {
			*(int16_t *)&entry[0xA] += MAIN_D_8012AA28[index];
		}

		if (GAME_STATE == 4) {
			getEntityScreenPos(ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[combatId]],
					   0, screenPos);
			MAIN_func_800E5724(entry[0xD], *(int32_t *)&entry[4],
					     screenPos[0] + *(int16_t *)&entry[8],
					     screenPos[1] - 8 + *(int16_t *)&entry[0xA],
					     *(int32_t *)entry, 14 - i);
		} else {
			MAIN_func_800E5724(entry[0xD], *(int32_t *)&entry[4],
					     *(int16_t *)&entry[8],
					     *(int16_t *)&entry[0xA],
					     *(int32_t *)entry, 14 - i);
		}

		if (entry[0xE] != 0) {
			if (GAME_STATE != 4) {
				MAIN_func_800E60E0(*(int16_t *)&entry[8] - 8,
						     *(int16_t *)&entry[0xA],
						     MAIN_D_801343EC[entry[0xE] - 1],
						     14 - i);
			} else {
				MAIN_func_800E60E0(screenPos[0] + *(int16_t *)&entry[8] - 8,
						     screenPos[1] - 8 +
							     *(int16_t *)&entry[0xA],
						     MAIN_D_801343EC[entry[0xE] - 1],
						     14 - i);
			}
		}

		*frame += 1;
		if (*frame >= 31) {
			entry[0xC] = 0xFF;
			(&data[i])[0x84] = 0xFF;
		}
		i += 1;
	}

	if (data[0x84] == 0xFF) {
		i = 1;
		while (i < 8) {
			activeIndex = (&data[i])[0x84];
			index = activeIndex;
			if (activeIndex != 0xFF) {
				(&data[i])[0x83] = index;
			} else {
				(&data[i])[0x83] = 0xFF;
				break;
			}
			i += 1;
		}
		data[0x8B] = 0xFF;
	}

	i = 0;
	while (i < 8) {
		if ((&data[i])[0x84] != 0xFF) {
			break;
		}
		i += 1;
	}
	if (i == 8) {
		removeEntityText(id);
	}
}

void removeEntityText(int32_t id)
{
	uint8_t *p;
	int32_t i;
	int32_t ofs;

	p = &MAIN_D_80150CD0[id * 0x8C];
	if (*(int32_t *)p != 0) {
		removeObject(0x192, id);
		*(int32_t *)p = 0;
		i = 0;
		ofs = 0;
		while (i < 8) {
			(&p[ofs])[0x10] = 0xFF;
			(&p[i])[0x84] = 0xFF;
			i += 1;
			ofs += 0x10;
		}
	}
}
