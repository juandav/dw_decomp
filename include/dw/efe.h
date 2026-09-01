#ifndef DW_EFE_H
#define DW_EFE_H

#include <libgs.h>
#include <libgte.h>

#include <dw/entity.h>
#include <dw/model.h>
#include <dw/types.h>

#define EFE_POP(ptr, type) ((type)*--(ptr))
#define EFE_PUSH(ptr, type, value) (*(ptr)++ = (int32_t)(type)(value))

#define EFE_POP1(type) EFE_POP(EFE_DATA_STACK, type)
#define EFE_PUSH1(type, value) EFE_PUSH(EFE_DATA_STACK, type, value)
#define EFE_DROP1() (--EFE_DATA_STACK)

#define EFE_POP2(type) EFE_POP(MAIN_D_80134CFC, type)
#define EFE_PUSH2(type, value) EFE_PUSH(MAIN_D_80134CFC, type, value)

#define EFE_SCRATCH ((EfeScratch *)getScratchAddr(0))

typedef struct {
	int32_t position[3];
	int32_t rotation[3];
} EfeTransform;

typedef struct {
	int32_t frame;
	EfeTransform transform;
} EfeInstance;

typedef struct {
	int16_t boneId;
	int16_t positionX;
	int16_t positionY;
	int16_t positionZ;
} EfeBoneOffset;

typedef struct {
	int16_t *inst;
	int16_t *someInst;
	EfeInstance *instance;
	EfeInstance *parentInstance;
	Entity *sourceEntity;
	Entity *targetEntity;
	EfeBoneOffset *boneOffset;
} EfeSubEffect;

typedef struct {
	GsDOBJ2 obj;
	SVECTOR rot;
	MATRIX m0;
	MATRIX m1;
	MATRIX m2;
	VECTOR *scale;
	int32_t id;
} EfeScratch;

typedef struct {
	int32_t frame;
	SVECTOR position;
	SVECTOR rotation;
	SVECTOR velocity;
	char *typeData;
	Entity *owner;
} EfeAura;

typedef struct {
	int16_t state;
	int16_t pad;
	int8_t *isLoaded;
	int16_t *moves;
	int16_t *effectIds;
	ModelComponent *model;
} EfeLoad;

typedef struct {
	SVECTOR worldPos;
	int16_t progress;
	int16_t tMax;
	int16_t mode;
	int16_t fixedDepth;
	int16_t redMin;
	int16_t greenMin;
	int16_t blueMin;
	int16_t redMax;
	int16_t greenMax;
	int16_t blueMax;
	int32_t scaleMin;
	int32_t scaleMax;
	int16_t offsetX;
	int16_t offsetY;
} EfeFlashData;

typedef struct {
	DVECTOR screenPos;
	int16_t depth;
	int16_t pad;
	int32_t scale;
	uint16_t sizeX;
	uint16_t sizeY;
	uint16_t tpage;
	uint8_t uBase;
	uint8_t vBase;
	uint16_t clut;
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t colorScale;
} ParticleFlashData;

extern EfeFlashData *EFE_FLASH_DATA;
extern int32_t *EFE_DATA_STACK;
extern int32_t EFE_SCRIPT_MEM1_DATA[];
extern int16_t EFE_LOADED_MOVE_DATA[];
extern EfeInstance *EFE_INSTANCE;
extern int32_t EFE_PARENT_INSTANCE;

extern char *MAIN_D_80134CCC;
extern int32_t MAIN_D_80134CD0;
extern int32_t MAIN_D_80134CD4;
extern int32_t MAIN_D_80134CD8;
extern int32_t MAIN_D_80134CE0;
extern int32_t MAIN_D_80134CE4;
extern EfeSubEffect *MAIN_D_80134CE8;
extern int32_t MAIN_D_80134CEC;
extern int32_t MAIN_D_80134CF0;
extern int32_t *MAIN_D_80134CFC;
extern int16_t *MAIN_D_80134D00;
extern int16_t MAIN_D_80134D04;
extern int32_t MAIN_D_80134D08;
extern int32_t *MAIN_D_80134D0C;
extern char *MAIN_D_80134D10;
extern int32_t MAIN_D_80134D14;
extern int32_t MAIN_D_80134D18;
extern int16_t MAIN_D_80139AB0[];
extern int16_t MAIN_D_80139B20[];
extern int32_t MAIN_D_80139B28[];
extern int32_t MAIN_D_80139B2C[];
extern GsRVIEW2 MAIN_D_80139B34;
extern int32_t MAIN_D_80139B54[];

#endif
