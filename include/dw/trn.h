#ifndef DW_TRN_H
#define DW_TRN_H

#include <libgpu.h>

#include <dw/types.h>

extern char MAIN_D_80134BC0[7];
extern RECT MAIN_D_80134BC8;
extern RECT MAIN_D_80134BD0;
extern RECT MAIN_D_80134BD8;

void TRN_setupHpTraining(int32_t mapId);
void TRN_setupOffenseTraining(int32_t mapId);
void TRN_setupSpeedTraining(int32_t mapId);
void TRN_setupDefenseTraining(int32_t mapId);
void TRN_setupMpTraining(int32_t mapId);
void TRN_setupBrainsTraining(int32_t mapId);

#endif
