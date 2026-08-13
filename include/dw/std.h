#ifndef DW_STD_H
#define DW_STD_H

#include <dw/types.h>

extern uint8_t STD_D_80052AE0[];
extern char STD_D_8007AAB0[];

int16_t STD_func_800579D8(uint8_t *arg);
void STD_tickNPCTournament(int32_t instanceId);
void STD_tickPartnerTournament(int32_t instanceId);
void STD_tickTamerTournament(int32_t instanceId);

#endif
