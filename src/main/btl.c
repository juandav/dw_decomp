#include <libgs.h>
#include <dw/entity.h>
#include <dw/params.h>
#include <dw/types.h>
typedef void (*TickFunction2)(int32_t);
typedef void (*RenderFunction2)(int32_t);
int32_t addObject(int32_t objectId, int32_t instanceId, TickFunction2 tick,
		  RenderFunction2 render);

#include "common.h"

extern uint8_t CURRENT_SCREEN;
extern uint8_t MAIN_D_8012BB20[];
extern int16_t MAIN_D_80154FCC[];
extern int16_t MAIN_D_80154FCE[];
void removeObject(int32_t objectId, int32_t instanceId);
extern int32_t POLLED_INPUT;
extern int32_t POLLED_INPUT_PREVIOUS;
extern int32_t ACTIVE_FRAMEBUFFER;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern GsOT GS_ORDERING_TABLE[];
extern PACKET GS_WORK_BASES[];
extern char DRAW_OFFSETS[];
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
long PadRead(long id);
long VSync(long mode);
void tickObjects(void);
void startAnimation(Entity *e, int32_t anim);
void renderObjects(void);

int32_t isScreenConcave(void);
void loadBattleData();
void loadCombatDataTick();
void handleBattleIdle();
void MAIN_func_800E7D9C(uint16_t id, int16_t v);
void MAIN_func_800E7DEC(int32_t instanceId);
void MAIN_func_800E8470(int32_t id);
void handleBattleStart();
void startBattleIdleAnimation();
extern int32_t MAIN_D_80134D74;
extern Entity *MAIN_D_80134D60;

void *btl_order_anchor[] = {
	startBattleIdleAnimation,
	handleBattleStart,
	MAIN_func_800E8470,
	MAIN_func_800E7DEC,
	MAIN_func_800E7D9C,
	handleBattleIdle,
	loadCombatDataTick,
	loadBattleData,
	isScreenConcave,
};

int32_t isScreenConcave(void)
{
	int32_t i;

	i = 0;
	while (i < 0x12) {
		if (CURRENT_SCREEN == MAIN_D_8012BB20[i]) {
			return 1;
		}
		i += 1;
	}
	return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/btl", loadBattleData);

void loadCombatDataTick(void)
{
	POLLED_INPUT = PadRead(1);
	ACTIVE_FRAMEBUFFER = GsGetActiveBuff();
	ACTIVE_ORDERING_TABLE = &GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER];
	GsSetWorkBase(&GS_WORK_BASES[ACTIVE_FRAMEBUFFER * 0x14000]);
	GsClearOt(0, 0, ACTIVE_ORDERING_TABLE);
	tickObjects();
	renderObjects();
	AddPrim((char *)ACTIVE_ORDERING_TABLE->org + 0x80,
		&DRAW_OFFSETS[ACTIVE_FRAMEBUFFER * 0xC]);
	DrawSync(0);
	VSync(3);
	POLLED_INPUT_PREVIOUS = POLLED_INPUT;
	GsSetOrign(DRAWING_OFFSET_X, DRAWING_OFFSET_Y);
	GsSwapDispBuff();
	GsSortClear(0, 0, 0, ACTIVE_ORDERING_TABLE);
	GsDrawOt(ACTIVE_ORDERING_TABLE);
}

void handleBattleIdle(Entity *e)
{
	Entity *p;

	p = e;
	if ((MAIN_D_80134D74 == 0) || (p != MAIN_D_80134D60)) {
		if ((*(uint8_t *)((char *)p + 0x2E) != 0x21) &&
		    (*(uint8_t *)((char *)p + 0x2E) != 0x22)) {
			startBattleIdleAnimation(p, (char *)p + 0x38);
		}
	}
}

void MAIN_func_800E7D9C(uint16_t id, int16_t v)
{
	MAIN_D_80154FCC[(id - 2) * 6] = 0;
	MAIN_D_80154FCE[(id - 2) * 6] = v;

	addObject(0x196, id, NULL, MAIN_func_800E7DEC);
}

INCLUDE_ASM("asm/main/nonmatchings/btl", MAIN_func_800E7DEC);

void MAIN_func_800E8470(int32_t id)
{
	removeObject(0x196, id);
}

INCLUDE_ASM("asm/main/nonmatchings/btl", handleBattleStart);

void startBattleIdleAnimation(Entity *e, int16_t *stats, int32_t flags)
{
  int new_var;
  int32_t f;
  int32_t anim;
  {
    f = 1;
    if (!(flags & 1))
    {
      if (stats[0xA] > (stats[8] / 5))
      {
 goto dummy_label_16793; dummy_label_16793: ;
        f = 0;
      }
    }
    new_var = f;
    if (new_var != 0U)
    {
      new_var = new_var == 0;
      anim = 0x21;
      anim = 0x22;
    }
    else
    {
      anim = 0x22;
      anim = 0x21;
    }
    startAnimation(e, (uint8_t) anim);
  }
}

