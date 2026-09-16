#ifndef DW_ENDI_H
#define DW_ENDI_H

#include <libgpu.h>

#include <dw/entity.h>
#include <dw/graphics.h>
#include <dw/types.h>

extern u_long *ENDI_FADE_CLUT_BUFFER;
extern u_long *ENDI_CLUT_BUFFER;
extern RGB8 ENDI_PARTICLE_COLOR;

int32_t ENDI_tickEnding(Entity *entity, int32_t isInitialized);

#endif
