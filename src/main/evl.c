#include <dw/anim.h>
#include <dw/bubble.h>
#include <dw/entity.h>
#include <dw/evl.h>
#include <dw/graphics.h>
#include <dw/main.h>
#include <dw/utils.h>
#include <dw/world_object.h>

#include "common.h"

extern int8_t EVL_LOADING_COMPLETE;

void stopBGM(void);
void stopSound(void);
void loadMapSounds2();
void isSoundLoaded();
void loadVLALL();

void* evl_functions[] = {
	evoSequenceAlwaysTrue,
	getEvoSequenceState,
	renderEvoSequenceLoading,
	tickEvoSequenceLoading
};
void tickEvoSequenceLoading(int32_t instanceId)
{
	EvoSequenceData *data;

	data = &EVO_SEQUENCE_DATA;

	switch (data->state) {
	case 0:
		if ((evoSequenceAlwaysTrue(500) == 1) && (data->timer > 55)) {
			data->state = 1;
			data->timer = 0;
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 1);
			addConditionBubble(7, &data->partner->digimonEntity.entity);
		}
		break;
	case 1:
		if ((data->timer > 55) && (EVL_LOADING_COMPLETE == 0)) {
			removeObject(0x809, instanceId);
			stopBGM();
			EVL_initEvoSequence();
			return;
		}
		break;
	default:
		break;
	}

	++data->timer;
	if (data->timer > 30000) {
		data->timer = 30000;
	}
}

void renderEvoSequenceLoading(int32_t instanceId)
{
	EvoSequenceData *data;

	data = &EVO_SEQUENCE_DATA;
}

/* CodeWarrior retains scheduler state between functions. This unused function
 * reproduces the state required by getEvoSequenceState and is linker-GC'd. */
static int32_t primeGetEvoSequenceStateScheduler(int32_t a, int32_t b,
						 int32_t c, int32_t d)
{
	int32_t value = a;

	value = (value << 1) + b;
	value = value * 3 + c;
	value ^= d;
	value += EVO_GAINS_DATA[value & 7].targetDigimon;
	value += DIGIMON_DATA[value & 7].height;
	value = (value << 3) - value;
	value -= b;
	value += c + d;
	value = (value << 1) + b;
	value = value * 3 + c;
	value ^= d;

	return value;
}

int32_t getEvoSequenceState(partner, buffer, para, target, isInitialized)
PartnerEntity *partner;
void *buffer;
PartnerPara *para;
int16_t target;
int16_t isInitialized;
{
	EvoSequenceData *data;
	int32_t instanceId;

	data = &EVO_SEQUENCE_DATA;
	instanceId = 0;
	if (isInitialized != 0) {
		return data->timer;
	}

	data->timer = 0;
	data->partner = partner;
	data->unk_0x8 = 0;
	data->state = 0;
	data->digimonId = EVO_GAINS_DATA[target].targetDigimon;
	data->para = para;
	data->evoTarget = target;
	data->heightFactor = (DIGIMON_DATA[data->digimonId].height << 12) /
			     DIGIMON_DATA[partner->digimonEntity.entity.type].height;

	startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0);
	stopSound();
	loadMapSounds2(18);
	isSoundLoaded(0, 8);
	loadVLALL(data->digimonId, GENERAL_BUFFER_PTR);
	loadDynamicLibrary(EVL_REL, (uint8_t *)&EVL_LOADING_COMPLETE, 0, 0, 0);
	EVL_LOADING_COMPLETE = 0;
	addObject(0x809, instanceId, tickEvoSequenceLoading, renderEvoSequenceLoading);

	return (int32_t)buffer;
}

int evoSequenceAlwaysTrue(int32_t unused)
{
	return 1;
}
