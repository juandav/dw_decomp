#ifndef DW_DOO_H
#define DW_DOO_H

#include <libgpu.h>
#include <libgte.h>

#include <dw/entity.h>
#include <dw/types.h>

typedef struct {
	int16_t targetY;
	int16_t pad;
	VECTOR pos;
} DooFlash;

typedef struct {
	int32_t frame;
	Entity *entity;
	int16_t phase;
	int16_t phaseInitPending;
	int16_t fadeLevel;
	int16_t eggSlot;
	DooFlash flash;
	int32_t modelData[6];
	int16_t sparkleIndex;
	int8_t isModelLoading;
	int8_t unk_3F;
} DooSequence;

typedef struct {
	int8_t icon[4];
} Doo2EggIcons;

extern DooSequence DOOA_REINCARNATION_SEQ;

extern RECT MAIN_D_80134B90;
extern RECT MAIN_D_80134B98;
extern Doo2EggIcons EGG_ICONS;
extern RECT MAIN_D_80134BA4;

int32_t DOOA_tick(PartnerEntity *partner, void *buffer, int32_t isInitialized);
int32_t DOOA_getSequenceState(int32_t unused, int32_t isInitialized);

void DOO2_openEggBox(void);
int32_t DOO2_tickEggInput(void);

#endif
