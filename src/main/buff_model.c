#include <libgs.h>

#include <dw/model.h>
#include <dw/world_object.h>

extern int32_t buffModelValue[2];
extern int32_t buffModelFrame;
extern TMDModel *BUFF_MODEL[];

void tickBuffModelObject(int32_t instanceId);
void MAIN_func_800F1794(void);
void MAIN_func_800F179C(int32_t model, int32_t compIdx, int32_t color);
void initializeBuffModel(TMDModel *model);
int32_t initializeBuffModelObject(void);
int32_t removeBuffModelObject(void);

static void *buff_model_functions[] = {
	removeBuffModelObject,
	initializeBuffModelObject,
	initializeBuffModel,
	MAIN_func_800F179C,
	MAIN_func_800F1794,
	tickBuffModelObject,
};

void tickBuffModelObject(int32_t instanceId)
{
	MAIN_func_800F179C((int32_t)BUFF_MODEL[0], 5, buffModelValue[buffModelFrame & 1]);
	buffModelFrame += 1;
}

void MAIN_func_800F1794(void)
{
}

void MAIN_func_800F179C(int32_t model, int32_t compIdx, int32_t color)
{
	char *p;
	int32_t i;
	int32_t count;
	struct TMD_STRUCT *obj;
	uint8_t attr;

	model += 12;
	obj = (struct TMD_STRUCT *)model;
	obj += compIdx;
	count = obj->primn;
	p = (char *)obj->primtop;
	for (i = 0; i < count; i++) {
		attr = *(int32_t *)p >> 24;
		if (attr & 4) {
			*(int16_t *)(p + 6) = color;
			if (attr & 8) {
				if (!(attr & 0x10)) {
					p += 0x20;
				} else {
					p += 0x2c;
				}
			} else if (!(attr & 0x10)) {
				p += 0x1c;
			} else {
				p += 0x24;
			}
		}
	}
}

void initializeBuffModel(TMDModel *model)
{
	BUFF_MODEL[0] = model;
	GsMapModelingData((unsigned long *)&BUFF_MODEL[0]->flags);
	buffModelValue[0] = 0x7acc;
	buffModelValue[1] = 0x7b0c;
}

int32_t initializeBuffModelObject(void)
{
	buffModelFrame = 0;
	return addObject(0x501, 0, tickBuffModelObject,
			 (RenderFunction)MAIN_func_800F1794);
}

int32_t removeBuffModelObject(void)
{
	return removeObject(0x501, 0);
}
