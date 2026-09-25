#include <string.h>

#include <libgpu.h>
#include <libgs.h>

#include <dw/btl.h>
#include <dw/script.h>
#include <dw/world_object.h>

extern char MAIN_D_801346FC[];
extern char MAIN_D_80134704[];
extern char MAIN_D_8013470C[];
extern BtlCommandShout BTL_COMMAND_SHOUT;
extern int8_t BTL_D_80072E54[];
extern int8_t BTL_D_80072E68[];

int16_t entityGetTechFromAnim(Entity *entity, int32_t anim);
void BTL_drawCommandShout(uint32_t arg0);
void BTL_removeCommandShout(void);
void BTL_renderCommandShout(void);

const char BTL_D_80072DB8[] = "Your Call!";
const char BTL_D_80072DC4[] = "Moderate!";
const char BTL_D_80072DD0[] = "Distance!";
const char BTL_D_80072DDC[] = "Defense!";

const char *const BTL_D_80072DE8[7] = {
	MAIN_D_801346FC,
	MAIN_D_80134704,
	BTL_D_80072DB8,
	BTL_D_80072DC4,
	BTL_D_80072DD0,
	BTL_D_80072DDC,
	MAIN_D_8013470C,
};

void BTL_drawCommandShout(uint32_t arg0)
{
	RECT rect;
	int16_t screenPos[2];
	int16_t tech;
	int32_t length;

	if (BTL_COMMAND_SHOUT.frame != -1) {
		BTL_removeCommandShout();
	}

	BTL_COMMAND_SHOUT.frame = 0;
	getEntityScreenPos(ENTITY_TABLE[0], 4, screenPos);
	setRECT(&rect, 0, 0xcc, 0xa8, 0xc);
	clearTextSubArea(&rect);

	if ((arg0 >= 8) && (arg0 < 0xc)) {
		tech = entityGetTechFromAnim(ENTITY_TABLE[1], PARTNER_ENTITY.digimonEntity.stats.base.moves[(int32_t)arg0 - 8]);
		drawString(MOVE_NAMES[tech], 0, 0xcc);
		length = strlen(MOVE_NAMES[tech]);
	} else {
		drawString(BTL_D_80072DE8[(int32_t)arg0 - 1], 0, 0xcc);
		length = strlen(BTL_D_80072DE8[(int32_t)arg0 - 1]);
	}

	BTL_COMMAND_SHOUT.width = length * 12;
	if ((screenPos[0] - (BTL_COMMAND_SHOUT.width / 2)) < -0x8c) {
		screenPos[0] = (BTL_COMMAND_SHOUT.width / 2) - 0x8c;
	}

	if ((screenPos[0] + (BTL_COMMAND_SHOUT.width / 2)) >= 0x8d) {
		screenPos[0] = 0x8c - (BTL_COMMAND_SHOUT.width / 2);
	}

	if (screenPos[1] < -0x64) {
		screenPos[1] = -0x64;
	}

	if (screenPos[1] >= 0x65) {
		screenPos[1] = 0x64;
	}

	BTL_COMMAND_SHOUT.x = screenPos[0];
	BTL_COMMAND_SHOUT.y = screenPos[1];
	addObject(0x199, 0, NULL, (RenderFunction)BTL_renderCommandShout);
}

void BTL_removeCommandShout(void)
{
	if (BTL_COMMAND_SHOUT.frame != -1) {
		removeObject(0x199, 0);
		BTL_COMMAND_SHOUT.frame = -1;
	}
}

void BTL_renderCommandShout(void)
{
	GsSPRITE sprite;
	int32_t y;

	sprite.attribute = 0;
	sprite.tpage = getTPage(0, 0, 704, 256);
	sprite.cx = 0xd0;
	sprite.cy = 0x1e8;
	sprite.r = sprite.g = sprite.b = 0x80;
	setWH(&sprite, BTL_COMMAND_SHOUT.width, 0xc);
	sprite.mx = sprite.w / 2;
	sprite.my = 6;
	sprite.u = 0;
	sprite.v = 0xcc;

	if (BTL_COMMAND_SHOUT.frame < 4L) {
		sprite.scaley = sprite.scalex = (((BTL_COMMAND_SHOUT.frame * 2) + 2) << 12) / 10;
	} else {
		sprite.scaley = sprite.scalex = 0x1000;
	}

	sprite.x = BTL_COMMAND_SHOUT.x;
	y = BTL_COMMAND_SHOUT.y;
	if (y < -0x28) {
		sprite.y = y + BTL_D_80072E68[BTL_COMMAND_SHOUT.frame++];
	} else {
		sprite.y = y + BTL_D_80072E54[BTL_COMMAND_SHOUT.frame++];
	}

	sprite.rotate = 0;
	GsSortSprite(&sprite, ACTIVE_ORDERING_TABLE, 7);

	sprite.b = 0;
	sprite.g = 0;
	sprite.r = 0;
	sprite.x++;
	sprite.y++;
	GsSortSprite(&sprite, ACTIVE_ORDERING_TABLE, 7);

	if (BTL_COMMAND_SHOUT.frame >= 0x14) {
		BTL_removeCommandShout();
	}
}
