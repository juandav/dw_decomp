#ifndef DW_WORLD_OBJECT_H
#define DW_WORLD_OBJECT_H

#include <dw/types.h>

typedef void (*TickFunction)(int32_t);
typedef void (*RenderFunction)(int32_t);

int32_t addObject(int32_t objectId, int32_t instanceId, TickFunction tick,
		  RenderFunction render);
int32_t removeObject(int32_t objectId, int32_t instanceId);

#endif
