#ifndef DW_KAR_H
#define DW_KAR_H

#include <libgpu.h>

#include <dw/types.h>

typedef struct {
	int16_t weight[4];
} KarWeightTbl;

typedef struct {
	char *text[2];
} KarStrPair;

typedef struct {
	int8_t value[4];
} KarTallyValues;

typedef struct {
	int8_t id[3];
} KarPeggedModelIds;

extern KarWeightTbl MAIN_D_80134A08;
extern RECT MAIN_D_80134A10;
extern RECT MAIN_D_80134A18;
extern KarStrPair MAIN_D_80134A20;
extern RECT MAIN_D_80134A28;
extern RECT MAIN_D_80134A30;
extern KarTallyValues MAIN_D_80134A44;
extern KarPeggedModelIds MAIN_D_80134A4C;
extern int8_t MAIN_D_80134A4F;

void checkCurlingMap(int32_t mapId);

void KAR_start(void);
void KAR_tick(void);

#endif
