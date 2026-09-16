#ifndef DW_TRN2_H
#define DW_TRN2_H

#include <libgpu.h>

#include <dw/types.h>

extern char MAIN_D_80134BE0[7];
extern RECT MAIN_D_80134BE8;
extern RECT MAIN_D_80134BF0;
extern RECT MAIN_D_80134BF8;

void TRN2_setupHpTraining(int32_t mapId);
void TRN2_setupOffenseTraining(int32_t mapId);
void TRN2_setupSpeedTraining(int32_t mapId);
void TRN2_setupDefenseTraining(int32_t mapId);
void TRN2_setupMpTraining(int32_t mapId);

#endif
