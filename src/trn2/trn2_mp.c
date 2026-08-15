#include <dw/entity.h>
#include <dw/partner.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/types.h>
#include <dw/ui.h>

extern int8_t MAIN_D_801353BC;
extern int8_t MAIN_D_801353BD;
extern int32_t TRN2_D_8008DC1C[];
extern int32_t TRN2_D_8008DC2C[];
extern int32_t TRAINING_COMPLETE;
extern uint32_t POLLED_INPUT;
extern int16_t MAIN_D_801353C2;
extern int16_t MAIN_D_801353C4;

void createCameraMovement(VECTOR *pos, int32_t speed);
void startAnimation(Entity *entity, int32_t animId);
int32_t tickEntityWalkTo(uint8_t scriptId1, uint8_t scriptId2, int32_t targetX, int32_t targetZ, int8_t withCamera);
void TRN_tickMpTraining(int32_t instanceId);
void TRN2_tickMpTraining(int32_t instanceId);
void TRN2_startSlotSessionIfEnabled(int16_t arg);
int32_t TRN2_statGainsAreZero(void);
void TRN2_awardMpTrainingGains(int32_t a, int16_t b, int32_t c);
void TRN2_applyBaseStats(void);
void TRN2_closeUIBox(int32_t id);
int16_t TRN2_getSlotSessionResult(void);
void TRN2_startSlotSpin(void);

static void *trn2_mp_functions[] = {
	TRN2_tickMpTraining,
};

void TRN2_tickMpTraining(int32_t instanceId)
{
	int32_t r;

	switch (MAIN_D_801353BD) {
	case 0:
		setTamerState(8);
		setPartnerState(10);
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 2);
		createCameraMovement((VECTOR *)TRN2_D_8008DC1C, 10);
		playSound(8, 9);
		MAIN_D_801353C2 = 0;
		MAIN_D_801353BD = 1;
		TRN2_startSlotSessionIfEnabled(1);
		break;
	case 1:
		if (tickEntityWalkTo(0xfc, 0xff, TRN2_D_8008DC2C[0], TRN2_D_8008DC2C[2], 0) == 1) {
			MAIN_D_801353BD = 2;
		}
		break;
	case 2:
		if (tickEntityWalkTo(0xfc, 0xff, TRN2_D_8008DC1C[0], TRN2_D_8008DC1C[2], 0) == 1) {
			PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy = 0x400;
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0xc);
			MAIN_D_801353BD = 3;
			if (MAIN_D_801353BC == 1) {
				TRN2_startSlotSpin();
			}
		}
		break;
	case 3:
		MAIN_D_801353C2++;
		r = 10;
		if (PARTNER_ENTITY.digimonEntity.entity.anim.animFrame <= PARTNER_ENTITY.digimonEntity.entity.anim.frameCount) {
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0xc);
		}
		if (MAIN_D_801353BC == 1) {
			r = TRN2_getSlotSessionResult();
		}
		if ((((MAIN_D_801353C2 >= 0x4b0) || (POLLED_INPUT & 0x10)) && (MAIN_D_801353BC == 0)) || ((MAIN_D_801353BC == 1) && (r >= 0))) {
			playSound(8, 0xa);
			TRN2_awardMpTrainingGains(PARTNER_ENTITY.digimonEntity.entity.type, 8, r);
			createCameraMovement(&TAMER_ENTITY.entity.posData->location, 10);
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 2);
			MAIN_D_801353BD = 4;
			MAIN_D_801353C4 = 0;
		}
		break;
	case 4:
		if (tickEntityWalkTo(0xfc, 0xff, TRN2_D_8008DC2C[0], TRN2_D_8008DC2C[2], 0) == 1) {
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x12);
			MAIN_D_801353BD = 5;
		}
		break;
	case 5:
		if (TRN2_statGainsAreZero() == 1) {
			MAIN_D_801353C4 = 0;
			MAIN_D_801353BD = 6;
		}
		break;
	case 6:
		MAIN_D_801353C4++;
		if (MAIN_D_801353C4 >= 0x14) {
			TRN2_applyBaseStats();
			TRN2_closeUIBox(1);
			setTamerState(0);
			setPartnerState(1);
			MAIN_D_801353C2 = 0;
			MAIN_D_801353BD = 0;
			/*
			 * BUG: reference to TRN_tickMpTraining in TRN
			 * overlay. The function only takes two parameters so
			 * the parameter is unused.
			 */
			removeObject(0xfaf, instanceId, TRN_tickMpTraining, NULL);
			TRAINING_COMPLETE = 1;
		}
		break;
	}
}
