#ifndef DW_EAB_H
#define DW_EAB_H

#include <libgs.h>
#include <libgte.h>

#include <dw/entity.h>
#include <dw/graphics.h>
#include <dw/types.h>

typedef struct {
	int16_t frame;
	int16_t phase;
	Entity *entity;
	VECTOR location;
} EabState;

extern int16_t EAB_D_800617D0[][4];
extern EabState EAB_D_800617E8;
extern char EAB_D_80061800[];
extern GsRVIEW2 EAB_D_800619E4;
extern int8_t EAB_D_80061A04[12];
extern int32_t EAB_D_80061A10[100][4];

extern char *MAIN_D_80134C28;
extern SVECTOR MAIN_D_80134C2C;
extern SVECTOR MAIN_D_80134C34;
extern RGB8 MAIN_D_80134C3C;
extern SVECTOR MAIN_D_80134C40;
extern uint8_t MAIN_D_80134C48;

void EAB_startBuildup(Entity *entity);
int32_t EAB_tick(Entity *entity, int32_t isInitialized);

#endif
