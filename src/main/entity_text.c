#include <libgs.h>
#include <dw/entity.h>
#include <dw/params.h>
#include <dw/types.h>
#include <dw/world_object.h>

#include "common.h"

extern uint8_t MAIN_D_80150CD0[];
void getEntityScreenPos(int32_t id, int32_t mode, int16_t *out);
void convertValueToDigits();
extern int8_t GAME_STATE;

void MAIN_func_800DF804();
void addEntityText(int32_t entityId, int32_t slot, int8_t color, int32_t value, uint8_t flag);
void setCombatTextPosition(int32_t id, int16_t *t);
void renderEntityText(int32_t instanceId);
void removeEntityText(int32_t id);

void *entity_text_order_anchor[] = {
	removeEntityText,
	renderEntityText,
	setCombatTextPosition,
	addEntityText,
	MAIN_func_800DF804,
};

INCLUDE_ASM("asm/main/nonmatchings/entity_text", MAIN_func_800DF804);

INCLUDE_ASM("asm/main/nonmatchings/entity_text", addEntityText);

void setCombatTextPosition(int32_t id, int16_t *t)
{
	int16_t xy[2];

	getEntityScreenPos(id, 0, xy);
	t[4] = xy[0];
	t[5] = xy[1] - 8;
}

INCLUDE_ASM("asm/main/nonmatchings/entity_text", renderEntityText);

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

