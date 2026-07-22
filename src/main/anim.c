#include <stdlib.h>
#include <string.h>

#include <dw/entity.h>
#include <dw/types.h>

#include "common.h"

typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
} AnimVec;

void calculatePosMatrix(PositionData *posData, int32_t unused1,
			int32_t unused2, int32_t translate);
void resetMomentumData(MomentumData *momentum);
void animateEntityTexture();
void setupModelMatrix(PositionData *posData);
void startAnimation();
void tickAnimation();
void tickMomentum();
void readMomentumInstructions(MomentumData *momentum, int16_t **instrPtr);
void readMomentumInstruction(int16_t *delta, int16_t *reload1,
			     int16_t *subDelta, int16_t *reload2,
			     int8_t *sign, int16_t **instrPtr,
			     int16_t *divisor);
int32_t applyMomentum(int32_t base, int16_t reload, int16_t delta,
		      int16_t *counter, int8_t step, int32_t offset);
void applyRootMomentum(MomentumData *momentum, Entity *entity);

void *anim_order_anchor[] = {
	applyRootMomentum,
	applyMomentum,
	readMomentumInstruction,
	readMomentumInstructions,
	tickMomentum,
	tickAnimation,
	startAnimation,
	setupModelMatrix,
	animateEntityTexture,
	resetMomentumData,
	calculatePosMatrix,
};

void calculatePosMatrix(PositionData *posData, int32_t unused1,
			int32_t unused2, int32_t translate)
{
	GsCOORDINATE2 *matrix;

	matrix = &posData->posMatrix;
	if (translate != 0) {
		TransMatrix(&matrix->coord, &posData->location);
	}

	RotMatrix(&posData->rotation, &matrix->coord);
	ScaleMatrix(&matrix->coord, &posData->scale);
	matrix->flg = 0;
}

void resetMomentumData(MomentumData *momentum)
{
	AnimVec zero;

	zero.z = 0;
	zero.y = 0;
	zero.x = 0;
	memcpy(&momentum->delta[3], &zero, 6);
	memcpy(&momentum->delta[6], &zero, 6);
	memcpy(&momentum->delta[0], &zero, 6);
	memcpy(&momentum->subDelta[3], &zero, 6);
	memcpy(&momentum->subDelta[6], &zero, 6);
	memcpy(&momentum->subDelta[0], &zero, 6);
}

INCLUDE_ASM("asm/main/nonmatchings/anim", animateEntityTexture);

void setupModelMatrix(PositionData *posData)
{
	GsCOORDINATE2 *matrix;

	matrix = &posData->posMatrix;
	TransMatrix(&matrix->coord, &posData->location);
	RotMatrix(&posData->rotation, &matrix->coord);
	ScaleMatrix(&matrix->coord, &posData->scale);
	matrix->flg = 0;
}

INCLUDE_ASM("asm/main/nonmatchings/anim", startAnimation);

INCLUDE_ASM("asm/main/nonmatchings/anim", tickAnimation);

INCLUDE_ASM("asm/main/nonmatchings/anim", tickMomentum);

INCLUDE_ASM("asm/main/nonmatchings/anim", readMomentumInstructions);

void readMomentumInstruction(int16_t *delta, int16_t *reload1,
			     int16_t *subDelta, int16_t *reload2,
			     int8_t *sign, int16_t **instrPtr,
			     int16_t *divisor)
{
	int16_t *ptr;
	int16_t value;
	int16_t reload;

	ptr = *instrPtr;
	*instrPtr = ptr + 1;
	value = *ptr;
	*delta = value / *divisor;
	*subDelta = value % *divisor;
	if (*subDelta != 0) {
		if (*subDelta > 0) {
			*sign = 1;
		} else {
			*sign = -1;
		}

		*subDelta = abs(*subDelta);
		reload = *divisor;
		*reload1 = reload;
		*reload2 = reload;
	}
}

int32_t applyMomentum(int32_t base, int16_t reload, int16_t delta,
		      int16_t *counter, int8_t step, int32_t offset)
{
	if (delta != 0) {
		*counter -= delta;
		if (*counter <= 0) {
			*counter += reload;
			return step + (offset + base);
		}
	}

	return offset + base;
}

void applyRootMomentum(MomentumData *momentum, Entity *entity)
{
	int32_t i;
	int16_t *scale1;
	int16_t *subDelta;
	int16_t *delta;
	int16_t *subScale;
	char *subValue;
	EntityAnim *anim;
	PositionData *posData;
	VECTOR in;
	VECTOR out;
	int32_t root[3];

	scale1 = &momentum->scale1[6];
	subDelta = &momentum->subDelta[6];
	delta = &momentum->delta[6];
	subScale = &momentum->subScale[6];
	anim = &entity->anim;
	subValue = &momentum->subValue[6];

	for (i = 0; i < 3; i++, delta++, subScale++, subDelta++) {
		if (i == 0 && (anim->animId == 0x24 || anim->animId == 0x23)) {
			root[0] = 0;
		} else if (*subDelta != 0 && (*subScale -= *subDelta, *subScale <= 0)) {
			root[i] = (*delta + subValue[i]) << 15;
			*subScale += scale1[i];
		} else {
			root[i] = *delta << 15;
		}
	}

	if ((entity->anim.animFlag & 2) == 0) {
		posData = entity->posData;

		in.vx = root[0];
		in.vy = root[1];
		in.vz = root[2];
		ApplyMatrixLV(&posData->posMatrix.coord, &in, &out);

		if ((entity->anim.animFlag & 8) && out.vz < 0) {
			out.vz = 0;
		}
		if ((entity->anim.animFlag & 0x10) && out.vx < 0) {
			out.vx = 0;
		}
		if ((entity->anim.animFlag & 0x20) && out.vz > 0) {
			out.vz = 0;
		}
		if ((entity->anim.animFlag & 0x40) && out.vx > 0) {
			out.vx = 0;
		}

		anim->locX = anim->locX + out.vx;
		anim->locY = anim->locY + out.vy;
		anim->locZ = anim->locZ + out.vz;

		posData->location.vx = anim->locX >> 15;
		posData->location.vy = anim->locY >> 15;
		posData->location.vz = anim->locZ >> 15;
	}
}
