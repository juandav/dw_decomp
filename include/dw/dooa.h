#ifndef DW_DOOA_H
#define DW_DOOA_H

#include <libgs.h>
#include <libgte.h>

#include <dw/entity.h>
#include <dw/types.h>

typedef struct {
	int16_t state;
	int16_t prevState;
	Entity *entity;
	int32_t unk_08;
	void *shardBuffer;
	void *shardWrite;
	int32_t shardBytes;
	uint8_t colorR;
	uint8_t colorG;
	uint8_t colorB;
	int8_t flash;
} DooaShardEffect;

extern char DOOA_FLASH_DATA[];
extern GsRVIEW2 DOOA_CAMERA_START_VIEW;
extern VECTOR DOOA_SAVED_LOCATION;
extern uint8_t DOOA_SAVED_ENTITY_CLUT[];
extern int16_t DOOA_MODEL_CLUT[];
extern int16_t DOOA_SHARD_CLUT[];
extern GsRVIEW2 DOOA_SAVED_VIEW;
extern VECTOR DOOA_PARTNER_POSITION;
extern int16_t DOOA_SCENE_CLUT[];
extern int16_t DOOA_FADED_CLUT[];
extern GsOT DOOA_ORDERING_TABLE[];
extern int8_t DOOA_SAVED_ENTITY_VISIBILITY[];
extern DooaShardEffect DOOA_SHARD_EFFECT;

#endif
