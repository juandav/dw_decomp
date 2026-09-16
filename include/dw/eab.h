#ifndef DW_EAB_H
#define DW_EAB_H

#include <libgte.h>

#include <dw/entity.h>
#include <dw/graphics.h>
#include <dw/types.h>

extern char *MAIN_D_80134C28;
extern SVECTOR MAIN_D_80134C2C;
extern SVECTOR MAIN_D_80134C34;
extern RGB8 MAIN_D_80134C3C;
extern SVECTOR MAIN_D_80134C40;
extern uint8_t MAIN_D_80134C48;

void EAB_startBuildup(Entity *entity);
int32_t EAB_tick(Entity *entity, int32_t isInitialized);

#endif
