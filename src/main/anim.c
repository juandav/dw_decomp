#include <stdlib.h>
#include <string.h>

#include <dw/clock.h>
#include <dw/entity.h>
#include <dw/model.h>
#include <dw/params.h>
#include <dw/sound.h>
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
void animateEntityTexture(Entity *entity, EntityAnim *anim);
void setupModelMatrix(PositionData *posData);
void startAnimation(Entity *entity, int32_t animId);
void tickAnimation(Entity *entity);
void tickMomentum(Entity *entity, MomentumData *momentumBase);
void readMomentumInstructions(MomentumData *momentum, int16_t **instrPtr);
void readMomentumInstruction(int16_t *delta, int16_t *reload1,
			     int16_t *subDelta, int16_t *reload2,
			     int8_t *sign, int16_t **instrPtr,
			     int16_t *divisor);
int32_t applyMomentum(int32_t base, int16_t reload, int16_t delta,
		      int16_t *counter, int8_t step, int32_t offset);
void applyRootMomentum(MomentumData *momentum, Entity *entity);

int32_t getEntityType(Entity *entity);

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

void animateEntityTexture(Entity *entity, EntityAnim *anim)
{
	int32_t type;
	int16_t texX;
	int16_t frame;
	RECT rect;

	type = entity->type;
	if (type == 0x7f)
		goto tex_2c;
	if (type == 0x45)
		goto tex_10;
	if (type == 0x65)
		goto tex_10;
	if (type == 0x8f)
		goto tex_28;
	if (type == 0x6a)
		goto tex_28;
	if (type == 0x15)
		goto tex_28;
	if (type == 0x68)
		goto tex_1e;
	if (type == 0x85)
		goto tex_1e;
	if (type != 0x09)
		return;

tex_1e:
	texX = 0x1e;
	goto tex_done;
tex_28:
	texX = 0x28;
	goto tex_done;
tex_10:
	texX = 0x10;
	goto tex_done;
tex_2c:
	texX = 0x2c;

tex_done:
	if (PLAYTIME_FRAMES % 2)
		return;

	if (PLAYTIME_FRAMES % 6 == 0) {
		frame = 2;
	} else if (PLAYTIME_FRAMES % 4 == 0) {
		frame = 1;
	} else {
		frame = 0;
	}

	rect.x = anim->textureX;
	rect.y = anim->textureY + 0x20 + frame * 0x20;
	rect.w = texX;
	rect.h = 0x20;
	MoveImage(&rect, anim->textureX, anim->textureY);
}

void setupModelMatrix(PositionData *posData)
{
	GsCOORDINATE2 *matrix;

	matrix = &posData->posMatrix;
	TransMatrix(&matrix->coord, &posData->location);
	RotMatrix(&posData->rotation, &matrix->coord);
	ScaleMatrix(&matrix->coord, &posData->scale);
	matrix->flg = 0;
}

inline int16_t *getAnimInt16Ptr(uint8_t *ptr)
{
	return (int16_t *)ptr;
}

void tickMomentum(Entity *entity, MomentumData *momentumBase)
{
	int32_t boneCount;
	PositionData *posData;
	int16_t *delta;
	int32_t updateScale;
	int32_t elemIndex;
	int32_t value;
	int32_t updateRot;
	int32_t updateLoc;
	struct {
		long *scale;
		long *location;
		int16_t *subDelta;
		int16_t *delta;
		int16_t *subScale;
		int8_t *subValue;
	} volatile pointers;
	int16_t *scale1;
	int16_t *subDelta;
	int16_t *subScale;
	int8_t *subValue;
	int32_t boneIndex;
	int32_t scaleOffset;
	int32_t rotOffset;
	int16_t *rotation;

	posData = entity->posData;
	boneCount = DIGIMON_DATA[entity->type].boneCount;
	pointers.scale = &posData->scale.vx;
	pointers.location = &posData->location.vx;
	rotation = &posData->rotation.vx;
	pointers.subDelta = &momentumBase->subDelta[0];
	pointers.delta = &momentumBase->delta[0];
	pointers.subScale = &momentumBase->subScale[0];
	pointers.subValue = &momentumBase->subValue[0];

	for (boneIndex = 0; boneIndex < boneCount;) {
		updateLoc = 0;
		updateRot = 0;
		updateScale = 0;

		scale1 = &momentumBase->scale1[0];
		delta = pointers.delta;
		subDelta = pointers.subDelta;
		subScale = pointers.subScale;
		subValue = pointers.subValue;
		elemIndex = 0;
		scaleOffset = 0;
		rotOffset = 0;

		for (; elemIndex < 9;) {
			value = *delta;
			if (value == 0 && *subDelta == 0)
				goto advance;

			if (elemIndex < 3) {
				updateScale = 1;
				*(long *)((uint8_t *)pointers.scale + scaleOffset) =
					applyMomentum(
						*delta, *scale1, *subDelta, subScale,
						*subValue,
						*(long *)((uint8_t *)pointers.scale +
							 scaleOffset));
			} else if (elemIndex < 6) {
				updateRot = 1;
				*getAnimInt16Ptr((uint8_t *)rotation + rotOffset - 6) =
					(int16_t)applyMomentum(
						*delta, *scale1, *subDelta, subScale,
						*subValue,
						*getAnimInt16Ptr((uint8_t *)rotation +
								    rotOffset - 6));
			} else {
				if (boneIndex == 0)
					break;

				updateLoc = 1;
				*(long *)((uint8_t *)((uintptr_t)scaleOffset +
							  (uintptr_t)pointers.location) -
					  24) =
					applyMomentum(
						*delta, *scale1, *subDelta, subScale,
						*subValue,
						*(long *)((uint8_t *)pointers.location +
							    scaleOffset - 24));
			}

		advance:
			scale1++;
			subDelta++;
			delta++;
			subScale++;
			subValue++;
			elemIndex++;
			rotOffset += 2;
			scaleOffset += 4;
		}

		if (boneIndex == 0) {
			calculatePosMatrix(posData, 1, 1, 0);
			applyRootMomentum(momentumBase, entity);
			setupModelMatrix(posData);
		} else {
			calculatePosMatrix(posData, updateScale, updateRot,
					   updateLoc);
		}

		boneIndex++;
		momentumBase++;
		rotation = getAnimInt16Ptr((uint8_t *)rotation +
					       sizeof(PositionData));
		pointers.subValue = (int8_t *)((uint8_t *)pointers.subValue +
						      sizeof(MomentumData));
		pointers.subScale =
			(int16_t *)((uint8_t *)pointers.subScale +
				    sizeof(MomentumData));
		pointers.delta = (int16_t *)((uint8_t *)pointers.delta +
						 sizeof(MomentumData));
		pointers.subDelta =
			(int16_t *)((uint8_t *)pointers.subDelta +
				    sizeof(MomentumData));
		posData++;
		pointers.location =
			(long *)((uint8_t *)pointers.location +
				 sizeof(PositionData));
		pointers.scale = (long *)((uint8_t *)pointers.scale +
					 sizeof(PositionData));
	}
}

/* CodeWarrior retains scheduler state between functions. Keep tickMomentum
 * before these two functions to reproduce the retail instruction ordering. */
void startAnimation(Entity *entity, int32_t animId)
{
	int16_t *animData;
	int16_t *instrPtr;
	MomentumData *momentum;
	PositionData *posData;
	EntityAnim *anim;
	ModelComponent *model;
	VECTOR *location;
	int32_t hasScale;
	int32_t boneCount;
	int32_t entityType;
	register int32_t i;

	{
		int32_t loadedOffset;
		int32_t *animTable;
		register int32_t animOffset;

		animTable = entity->animPtr;
		loadedOffset = animTable[animId];
		if ((animOffset = loadedOffset) == 0)
			return;

		animData = (int16_t *)((uint8_t *)animTable + animOffset);
	}
	instrPtr = animData;
	posData = entity->posData;
	anim = &entity->anim;

	boneCount = DIGIMON_DATA[entity->type].boneCount & 0xff;
	entityType = getEntityType(entity);
	model = getEntityModelComponent(entity->type, entityType);

	anim->textureX = (model->pixelPage - 0x10) * 64;
	anim->textureY = model->pixelOffsetY + 0x100;
	anim->loopEndFrame = 1;
	anim->animId = animId;
	anim->animFrame = 1;
	anim->animFlag = 1;
	anim->loopCount = 0;
	momentum = anim->momentum;
	anim->frameCount = *instrPtr++;

	if (anim->frameCount & 0x8000)
		hasScale = 1;
	else
		hasScale = 0;
	anim->frameCount &= 0x7fff;
	location = &posData->location;
	anim->locX = location->vx << 15;
	anim->locY = location->vy << 15;
	anim->locZ = location->vz << 15;

	resetMomentumData(momentum);
	momentum++;
	setupModelMatrix(posData);
	posData++;

	for (i = 1; i < boneCount; i++, momentum++, posData++) {
		if (!hasScale) {
			posData->scale.vx = 0x1000;
			posData->scale.vy = 0x1000;
			posData->scale.vz = 0x1000;
		} else {
			posData->scale.vx = *instrPtr++;
			posData->scale.vy = *instrPtr++;
			posData->scale.vz = *instrPtr++;
		}
		posData->rotation.vx = *instrPtr++;
		posData->rotation.vy = *instrPtr++;
		posData->rotation.vz = *instrPtr++;

		posData->location.vx = *instrPtr++;
		posData->location.vy = *instrPtr++;
		posData->location.vz = *instrPtr++;

		resetMomentumData(momentum);
		setupModelMatrix(posData);
	}

	anim->animInstrPtr = instrPtr;
}

inline int32_t peekAnimationTextureHighByte(int16_t **instrPtr)
{
	return (**instrPtr & 0xff00) >> 8;
}

inline int32_t readAnimationTextureLowByte(int16_t **instrPtr)
{
	return *(*instrPtr)++ & 0xff;
}

void tickAnimation(Entity *entity)
{
	EntityAnim *anim;
	MomentumData *momentum;
	int16_t **instrPtrPtr;
	int16_t *framePtr;
	int32_t opcode;
	int16_t *instrPtr;
	int32_t instruction;
	int32_t loopInstruction;

	anim = &entity->anim;
	momentum = anim->momentum;
	instrPtrPtr = &anim->animInstrPtr;
	framePtr = &anim->animFrame;

	if (!(anim->animFlag & 1))
		return;

	tickMomentum(entity, momentum);

	opcode = **instrPtrPtr;
	instruction = opcode;
	opcode &= 0x1000;
	if (opcode) {
		anim->loopCount = instruction;
		(*instrPtrPtr)++;
		anim->loopStart = *instrPtrPtr;
		anim->loopEndFrame = **instrPtrPtr & 0xfff;
	}
	goto frame_update;

loop:
	instrPtr = *instrPtrPtr;
	opcode = *instrPtr;
	loopInstruction = opcode;
	opcode &= 0xf000;

	if (opcode == 0x4000)
		goto op_4000;
	if (opcode == 0x3000)
		goto op_3000;
	if (opcode == 0x2000)
		goto op_2000;
	if (opcode == 0x1000)
		goto op_1000;
	if (opcode != 0x0000)
		goto op_end;

	*instrPtrPtr = instrPtr + 1;
	readMomentumInstructions(momentum, instrPtrPtr);
	goto op_end;

op_1000:
	anim->loopCount = loopInstruction;
	(*instrPtrPtr)++;
	anim->loopStart = *instrPtrPtr;
	goto op_end;

op_2000:
	if (anim->loopCount != 0xff)
		anim->loopCount--;
	if (anim->loopCount == 0) {
		*instrPtrPtr += 2;
	} else {
		(*instrPtrPtr)++;
		*framePtr = **instrPtrPtr;
		*instrPtrPtr = anim->loopStart;
	}
	goto op_end;

op_3000:
	{
		RECT rect;

		*instrPtrPtr = instrPtr + 1;
		rect.x = anim->textureX + ((**instrPtrPtr & 0xff00) >> 8);
		rect.y = anim->textureY + (*(*instrPtrPtr)++ & 0xff);
		rect.w = (**instrPtrPtr & 0xff00) >> 8;
		rect.h = *(*instrPtrPtr)++ & 0xff;
		/* Separate function bodies sequence the cursor accesses. The pinned
		 * compiler evaluates the high-byte peek before the advancing read. */
		MoveImage(&rect,
			  anim->textureX + peekAnimationTextureHighByte(instrPtrPtr),
			  anim->textureY + readAnimationTextureLowByte(instrPtrPtr));
	}
	goto op_end;

op_4000:
	*instrPtrPtr = instrPtr + 1;
	opcode = instruction = **instrPtrPtr;
	opcode &= 0xff00;
	{
		int16_t bankId;
		int32_t soundInstruction;
		int32_t vabId;

		soundInstruction = instruction;
		bankId = opcode >> 8;
		if (entity->isOnScreen == 1) {
			vabId = bankId != 4
					? bankId
					: ((DigimonEntity *)entity)->stats.current.vabId;
			playSound(vabId, soundInstruction & 0xff);
		}
	}
	(*instrPtrPtr)++;

op_end:
	anim->loopEndFrame = **instrPtrPtr & 0xfff;

frame_update:
	if (*framePtr == anim->loopEndFrame)
		goto loop;

	if (*framePtr == anim->frameCount)
		anim->animFlag &= 0xfe;
	else
		(*framePtr)++;

	animateEntityTexture(entity, anim);
}

void readMomentumInstructions(MomentumData *base, int16_t **instrPtr)
{
	MomentumData *momentum;
	volatile int32_t instruction;
	int16_t divisor;
	int16_t *reload1;
	int16_t *subDelta;
	int16_t *delta;
	int16_t *reload2;
	int8_t *sign;
	uint16_t flag;
	int32_t i;

	goto check;
	do {
		instruction = *(*instrPtr)++;
		momentum = &base[instruction & 0x3f];
		divisor = *(*instrPtr)++;

		subDelta = &momentum->subDelta[0];
		reload1 = &momentum->scale1[0];
		delta = &momentum->delta[0];
		reload2 = &momentum->subScale[0];
		sign = &momentum->subValue[0];

		flag = 0x4000;
		for (i = 0; i < 9; i++) {
			if (instruction & flag) {
				readMomentumInstruction(delta + i, reload1 + i,
							subDelta + i, reload2 + i,
							sign + i, instrPtr, &divisor);
			}
			flag = flag >> 1;
		}
	check:
		;
	} while (**instrPtr & 0x8000);
}

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
	int8_t *subValue;
	VECTOR input;
	VECTOR result;
	int32_t base[3];
	EntityAnim *anim;
	PositionData *posData;

	scale1 = &momentum->scale1[6];
	subDelta = &momentum->subDelta[6];
	delta = &momentum->delta[6];
	subScale = &momentum->subScale[6];
	anim = &entity->anim;
	subValue = &momentum->subValue[6];
	i = 0;

	for (; i < 3; i++, delta++, subScale++, subDelta++) {
		if (i == 0) {
			if (anim->animId == 0x24 || anim->animId == 0x23) {
				base[0] = 0;
				continue;
			}
		}

		if (*subDelta != 0) {
			*subScale -= *subDelta;
			if (*subScale <= 0) {
				base[i] = (*delta + subValue[i]) << 15;
				*subScale += scale1[i];
				continue;
			}
		}
		base[i] = *delta << 15;
	}

	if (!(entity->anim.animFlag & 2)) {
		posData = entity->posData;
		input.vx = base[0];
		input.vy = base[1];
		input.vz = base[2];
		ApplyMatrixLV(&posData->posMatrix.coord, &input, &result);

		if ((entity->anim.animFlag & 8) && result.vz < 0)
			result.vz = 0;
		if ((entity->anim.animFlag & 0x10) && result.vx < 0)
			result.vx = 0;
		if ((entity->anim.animFlag & 0x20) && result.vz > 0)
			result.vz = 0;
		if ((entity->anim.animFlag & 0x40) && result.vx > 0)
			result.vx = 0;

		anim->locX = anim->locX + result.vx;
		anim->locY = anim->locY + result.vy;
		anim->locZ = anim->locZ + result.vz;

		posData->location.vx = anim->locX >> 15;
		posData->location.vy = anim->locY >> 15;
		posData->location.vz = anim->locZ >> 15;
	}
}
