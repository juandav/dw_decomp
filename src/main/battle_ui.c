#include <libgs.h>
#include <dw/entity.h>
#include <dw/params.h>
#include <dw/types.h>
#include <dw/graphics.h>
#include <dw/world_object.h>

typedef struct {
	RECT startPos;
	RECT finalPos;
	int16_t frame;
	int16_t state;
	int16_t rowOffset;
	int16_t visibleRows;
	int16_t totalRows;
	uint8_t features;
	uint8_t color;
	TickFunction tick;
	RenderFunction render;
} UIBoxData;

extern UIBoxData UI_BOX_DATA[6];
void createAnimatedUIBox(int32_t instanceId, uint8_t color, uint8_t features,
			 RECT *finalPos, RECT *startPos,
			 TickFunction tickFunc, RenderFunction renderFunc);
void removeAnimatedUIBox();
extern uint16_t MAIN_D_80134E90;
extern int32_t MAIN_D_80134E9C;
extern int32_t MONEY;
extern int32_t POLLED_INPUT;
extern int32_t POLLED_INPUT_PREVIOUS;
void BTL_func_80062F58();
void playSound(int32_t vabId, uint32_t note);
extern GsOT *ACTIVE_ORDERING_TABLE;
void renderNumber(int32_t a, int32_t x, int32_t y, int32_t digits,
		  int32_t value, int32_t layer);
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e,
		  int32_t f, int32_t g, int32_t h, int32_t i);

#include "common.h"

extern POLY_FT4 BIT_BOX;
extern uint8_t MAIN_D_801555AC[];
extern uint8_t MAIN_D_801555AD[];
extern uint8_t MAIN_D_801555AE[];
extern int16_t INITIAL_COMBAT_STATS[];
extern int16_t STATS_GAINS[];
extern int16_t MAIN_D_80134EA0;
void BTL_func_8006306C(int32_t layer);
void renderLinePrimitive(uint32_t color, int16_t x0, int16_t y0, int16_t x1,
			 int32_t y1, int32_t order, uint32_t mode);
extern char MAIN_D_801344D4;
extern uint8_t BTL_D_80073EA0[];
extern Entity *ENTITY_TABLE[10];
void initStringFT4(POLY_FT4 *p);
void setUVDataPolyFT4(POLY_FT4 *p, int32_t u, int32_t v, int32_t w, int32_t h);
void getEntityScreenPos(Entity *e, int32_t mode, int16_t *out);
void drawString(char *text, int32_t color, int32_t pos);
int16_t random(int32_t max);

void initBitBox();
void battleStatsGainsAndDrops();
void handleBattleInjury();
void battleMoveLearning();
void createBitBox();
void createFinalBalanceBox();
void handleBattleEndBox();
void tickBitBox(int32_t instanceId);
void renderBitBox(int32_t layer);
void renderFinalBalance(int32_t layer);
void resetStatsAfterCombat();
void createPostBattleStatsBox();
void MAIN_func_800EE088();
void MAIN_func_800EE1FC();
void MAIN_func_800EE7B4(int32_t id);

void *battle_ui_order_anchor[] = {
	MAIN_func_800EE7B4,
	MAIN_func_800EE1FC,
	MAIN_func_800EE088,
	createPostBattleStatsBox,
	resetStatsAfterCombat,
	renderFinalBalance,
	renderBitBox,
	tickBitBox,
	handleBattleEndBox,
	createFinalBalanceBox,
	createBitBox,
	battleMoveLearning,
	handleBattleInjury,
	battleStatsGainsAndDrops,
	initBitBox,
};

INCLUDE_ASM("asm/main/nonmatchings/battle_ui", initBitBox);

INCLUDE_ASM("asm/main/nonmatchings/battle_ui", battleStatsGainsAndDrops);

INCLUDE_ASM("asm/main/nonmatchings/battle_ui", handleBattleInjury);

INCLUDE_ASM("asm/main/nonmatchings/battle_ui", battleMoveLearning);

INCLUDE_ASM("asm/main/nonmatchings/battle_ui", createBitBox);

INCLUDE_ASM("asm/main/nonmatchings/battle_ui", createFinalBalanceBox);

INCLUDE_ASM("asm/main/nonmatchings/battle_ui", handleBattleEndBox);

INCLUDE_ASM("asm/main/nonmatchings/battle_ui", tickBitBox);

INCLUDE_ASM("asm/main/nonmatchings/battle_ui", renderBitBox);

INCLUDE_ASM("asm/main/nonmatchings/battle_ui", renderFinalBalance);

void resetStatsAfterCombat(void)
{
	PARTNER_ENTITY.digimonEntity.stats.base.hp = INITIAL_COMBAT_STATS[0];
	PARTNER_ENTITY.digimonEntity.stats.base.mp = INITIAL_COMBAT_STATS[1];
	PARTNER_ENTITY.digimonEntity.stats.base.off = INITIAL_COMBAT_STATS[2];
	PARTNER_ENTITY.digimonEntity.stats.base.def = INITIAL_COMBAT_STATS[3];
	PARTNER_ENTITY.digimonEntity.stats.base.speed = INITIAL_COMBAT_STATS[4];
	PARTNER_ENTITY.digimonEntity.stats.base.brain = INITIAL_COMBAT_STATS[5];
}

INCLUDE_ASM("asm/main/nonmatchings/battle_ui", createPostBattleStatsBox);

INCLUDE_ASM("asm/main/nonmatchings/battle_ui", MAIN_func_800EE088);

INCLUDE_ASM("asm/main/nonmatchings/battle_ui", MAIN_func_800EE1FC);

INCLUDE_ASM("asm/main/nonmatchings/battle_ui", MAIN_func_800EE7B4);

