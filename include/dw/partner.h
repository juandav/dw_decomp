#ifndef DW_PARTNER_H
#define DW_PARTNER_H

#include <dw/types.h>

typedef struct {
	int8_t hungerTimes[8];
	uint8_t energyCap;
	uint8_t energyThreshold;
	uint8_t energyUsage;
	int16_t poopTimer;
	int16_t unk2;
	uint8_t poopSize;
	uint8_t favoriteFood;
	int8_t sleepCycle;
	uint8_t favoredRegion;
	uint8_t trainingType;
	uint8_t defaultWeight;
	int16_t viewX;
	int16_t viewY;
	int16_t viewZ;
} RaiseData;

typedef struct
{
	uint8_t map;
	uint8_t x;
	uint8_t y;
	int8_t size;
} PoopPile;

extern RaiseData RAISE_DATA[66];
extern PoopPile WORLD_POOP[];

void tickPartner(int32_t instanceId);
void tickPartnerOverworld(int32_t instanceId);
void tickNormal(void);
void partnerSleep(void);
void partnerPraiseScold(int32_t partnerState);
void partnerFeedItem(void);
void tickPartnerToilet(void);
void partnerWildPoop(void);
void tickPartnerDying(void);
void partnerEatShit(void);
void tickPartnerIdle(void);
void tickPartnerEvolving(void);
void tickPartnerDying2(void);
void tickPartnerWalking(void);
int32_t getPartnerTamerCloseness(void);
void setPartnerSlowWalking(void);
void updateConditionAnimation(void);
void setPartnerIdle(void);
void setPartnerState(int8_t state);
int32_t checkEatDistance(int32_t distance);
void MAIN_func_800DF5A0(void);
int32_t getPartnerState(void);
void startPartnerAnimation(int32_t animId);
void callDigimonRoutine(int32_t routine);
int32_t MAIN_func_800DF7F8(void);

#endif
