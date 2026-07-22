#include <dw/evl.h>

extern int8_t MENU_SUB_STATE;
extern char MAIN_D_80124C54[];
extern RECT MAIN_D_801342E8;
extern uint8_t MAIN_D_80134234[4];
extern int16_t MAIN_D_80134D38;
extern int16_t MAIN_D_80134D3A;
extern char *MAIN_D_801247C4[];
extern char *MAIN_D_801247C8[];
extern char *MAIN_D_801247CC[];
extern char *MAIN_D_801247D0[];
extern char *MAIN_D_801247D4[];
extern char *MAIN_D_801247D8[];
extern char *MOVE_NAMES[];

void clearTextSubArea(RECT *area);
void drawString(char *text, int32_t color, int32_t pos);
int32_t entityGetTechFromAnim(Entity *entity, int32_t anim);
int32_t drawDigimonMovesText(void);

int32_t drawDigimonMovesText(void)
{
	RECT rect;

	rect = MAIN_D_801342E8;
	switch (MENU_SUB_STATE) {
	case 0:
		clearTextSubArea(&rect);
		drawString(MAIN_D_80124C54, 0, 0xf0);
		if (PARTNER_ENTITY.digimonEntity.stats.base.moves[0] != 0xff) {
			MAIN_D_80134234[0] = entityGetTechFromAnim(ENTITY_TABLE[1],
				PARTNER_ENTITY.digimonEntity.stats.base.moves[0]);
			drawString(MOVE_NAMES[MAIN_D_80134234[0]], 0, 0x18);
		} else {
			MAIN_D_80134234[0] = 0xff;
		}
		MENU_SUB_STATE = 1;
		break;
	case 1:
		drawString(MAIN_D_801247C4[0], 0, 0x48);
		if (PARTNER_ENTITY.digimonEntity.stats.base.moves[1] != 0xff) {
			MAIN_D_80134234[1] = entityGetTechFromAnim(ENTITY_TABLE[1],
				PARTNER_ENTITY.digimonEntity.stats.base.moves[1]);
			drawString(MOVE_NAMES[MAIN_D_80134234[1]], 0, 0x24);
		} else {
			MAIN_D_80134234[1] = 0xff;
		}
		MENU_SUB_STATE = 2;
		DrawSync(0);
		return 0;
	case 2:
		drawString(MAIN_D_801247D0[0], 0x84, 0x6c);
		if (PARTNER_ENTITY.digimonEntity.stats.base.moves[2] != 0xff) {
			MAIN_D_80134234[2] = entityGetTechFromAnim(ENTITY_TABLE[1],
				PARTNER_ENTITY.digimonEntity.stats.base.moves[2]);
			drawString(MOVE_NAMES[MAIN_D_80134234[2]], 0, 0x30);
		} else {
			MAIN_D_80134234[2] = 0xff;
		}
		MENU_SUB_STATE = 3;
		DrawSync(0);
		return 0;
	case 3:
		if (PARTNER_ENTITY.digimonEntity.stats.base.moves[3] != 0xff) {
			MAIN_D_80134234[3] = entityGetTechFromAnim(ENTITY_TABLE[1],
				PARTNER_ENTITY.digimonEntity.stats.base.moves[3]);
			drawString(MOVE_NAMES[MAIN_D_80134234[3]], 0, 0x3c);
		} else {
			MAIN_D_80134234[3] = 0xff;
		}
		drawString(MAIN_D_801247C8[0], 0, 0x6c);
		MENU_SUB_STATE = 4;
		DrawSync(0);
		return 0;
	case 4:
		drawString(MAIN_D_801247CC[0], 0, 0x54);
		MENU_SUB_STATE = 5;
		break;
	case 5:
		drawString(MAIN_D_801247D4[0], 0, 0x60);
		drawString(MAIN_D_801247D8[0], 0, 0x78);
		MAIN_D_80134D3A = 0x73;
		MAIN_D_80134D38 = 0x6f;
		return 1;
	}
	return 0;
}
