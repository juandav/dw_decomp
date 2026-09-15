#ifndef DW_MURD_H
#define DW_MURD_H

#include <libgpu.h>
#include <libgs.h>

#include <dw/entity.h>
#include <dw/types.h>

typedef struct {
	int16_t frame;
	int16_t state;
	PartnerEntity *partner;
} MurdLivesBox;

extern int8_t MURD_LOADING_COMPLETE;
extern int8_t MURD_ENTITIES_VISIBLE;
extern RECT MURD_LIVES_BOX_FINAL_POS;
extern RECT MURD_LIVES_BOX_START_POS;
extern RECT MURD_LIVES_BOX_TARGET_POS;
extern MurdLivesBox MURD_LIVES_BOX;

extern uint16_t MURD_TEXTURE_BUFFER[];
extern int16_t MURD_PALETTE_BACKUP[];
extern GsOT MURD_ORDERING_TABLES[];
extern int8_t MURD_ENTITY_VISIBILITY[];

int32_t MURD_tick(PartnerEntity *partner, int32_t isInitialized);

#endif
