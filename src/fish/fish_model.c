#include <stdlib.h>

#include <libgs.h>

#include <dw/file.h>
#include <dw/fish.h>
#include <dw/types.h>

static void *fish_model_functions[] = {
	FISH_remapClamped,
	FISH_getModelObject,
	FISH_loadTMDModel,
};

void FISH_loadTMDModel(TMDFileLoadingData *model, char *path, uint8_t *buf, uint32_t bufSize)
{
	uint8_t *end;
	GsDOBJ2 *obj;
	int32_t size;
	int32_t i;

	size = ((int32_t)lookupFileSize(path) + 3) & ~3;
	if (bufSize < ((size + 0x7ff) & ~0x7ff)) {
		exit(1);
	}

	model->targetBuffer = buf;
	readFile(path, model->targetBuffer);

	model->objCount = ((uint32_t *)model->targetBuffer)[2];
	model->objects = (GsDOBJ2 *)(buf + size);

	if (bufSize < (size + (model->objCount * sizeof(GsDOBJ2)))) {
		exit(1);
	}

	model->objectsEnd = model->objects + model->objCount;

	if (model->objects == NULL) {
		exit(1);
	}

	GsMapModelingData((u_long *)(model->targetBuffer + 4));

	end = model->targetBuffer + 0xc;
	for (i = 0; i < model->objCount; i++) {
		GsLinkObject4((u_long)end, obj = &model->objects[i], i);
		obj->attribute = 0;
	}
}

GsDOBJ2 *FISH_getModelObject(TMDFileLoadingData *model, int32_t idx)
{
	if ((model == NULL) || (idx < 0) || (idx >= model->objCount)) {
		exit(1);
	}

	return &model->objects[idx];
}

int32_t FISH_remapClamped(int32_t x, int32_t x0, int32_t x1, int32_t y0, int32_t y1)
{
	int32_t t;
	int32_t d;

	if (x1 == x0) {
		return y0;
	}

	if (x1 < x0) {
		t = x0;
		x0 = x1;
		x1 = t;
	}

	x = (x < x0) ? x0 : ((x1 < x) ? x1 : x);
	d = x1 - x0;

	return y0 + ((((y1 - y0) * (x - x0)) + (d / 2)) / d);
}
