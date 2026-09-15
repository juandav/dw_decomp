#include <libgpu.h>

#include <dw/murd.h>
#include <dw/types.h>

int8_t MURD_LOADING_COMPLETE = 1;
int8_t MURD_ENTITIES_VISIBLE = 1;
RECT MURD_LIVES_BOX_FINAL_POS = { -95, -50, 190, 44 };
RECT MURD_LIVES_BOX_START_POS = { -8, -6, 16, 12 };
RECT MURD_LIVES_BOX_TARGET_POS = { -8, -6, 16, 12 };

MurdLivesBox MURD_LIVES_BOX;
