#include <dw/clock.h>
#include <dw/entity.h>
#include <dw/partner.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/trn2.h>
#include <dw/types.h>
#include <dw/ui.h>

extern uint16_t MAIN_D_801353CE;
extern int16_t MAIN_D_801353D0;
extern int16_t MAIN_D_801353D2;
extern int16_t TRN2_D_8008DC54[];
extern int8_t MAIN_D_801353BC;
extern int8_t MAIN_D_801353BD;
extern int32_t TRN2_D_8008DC1C[];
extern int32_t TRN2_D_8008DC2C[];
extern int32_t TRAINING_COMPLETE;
extern uint32_t POLLED_INPUT;
extern int16_t MAIN_D_801353B4;
extern int16_t MAIN_D_801353B6;
extern int16_t MAIN_D_801353C2;
extern int16_t MAIN_D_801353C4;
extern int16_t TRN2_D_8008DC74[];

void createCameraMovement(VECTOR *pos, int32_t speed);
void startAnimation(Entity *entity, int32_t animId);
void removeAllCloudFX(void);
void storeMapObjectPosition(int16_t *outX, int16_t *outY, int16_t a, int16_t count);
void loadMapObjectPosition(int16_t *xData, int16_t *yData, int16_t startIndex, int16_t count);
int32_t moveMapObjectsWithLimit(int16_t startIndex, int16_t count, int16_t dx, int16_t dy, int16_t limitX, int16_t limitY);
int32_t tickEntityWalkTo(uint8_t scriptId1, uint8_t scriptId2, int32_t targetX, int32_t targetZ, int8_t withCamera);
void TRN2_awardHpTrainingGains(int32_t a, int32_t b, int32_t c);
void TRN2_tickHpTrainingMap107(int32_t instanceId);
void TRN2_saveTrainingStartTime(void);
void TRN2_tickMpTraining(int32_t instanceId);
void TRN2_startSlotSessionIfEnabled(int16_t arg);
void TRN2_func_8008AA84(int8_t arg);
int32_t TRN2_statGainsAreZero(void);
void TRN2_applyBaseStats(void);
void TRN2_closeUIBox(int32_t id);
int16_t TRN2_getSlotSessionResult(void);
void TRN2_startSlotSpin(void);

static void *trn2_hp_map107_functions[] = {
	TRN2_setupMpTraining,
	TRN2_saveTrainingStartTime,
	TRN2_tickHpTrainingMap107,
};

void TRN2_tickHpTrainingMap107(int32_t instanceId)
{
	int32_t r;

	switch (MAIN_D_801353BD) {
	case 0:
		storeMapObjectPosition(TRN2_D_8008DC54, TRN2_D_8008DC74, MAIN_D_801353B4, MAIN_D_801353B6);
		setTamerState(8);
		setPartnerState(10);
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
		createCameraMovement((VECTOR *)TRN2_D_8008DC1C, 10);
		playSound(8, 9);
		MAIN_D_801353C2 = 0;
		MAIN_D_801353BD = 1;
		TRN2_startSlotSessionIfEnabled(0);
		break;
	case 1:
		TRN2_func_8008AA84(4);
		if (tickEntityWalkTo(0xfc, 0xff, TRN2_D_8008DC1C[0], TRN2_D_8008DC1C[2], 0) == 1) {
			startAnimation(ENTITY_TABLE[1], 0x1b);
			PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy = 0x898;
			if (MAIN_D_801353BC == 1) {
				TRN2_startSlotSpin();
			}
			playSound(8, 0);
			MAIN_D_801353BD = 2;
		}
		break;
	case 2:
		PARTNER_ENTITY.digimonEntity.entity.anim.animFlag |= 2;
		if (((MAIN_D_801353C2 % 0x78) == 0) && (MAIN_D_801353C2 < 0x4b1)) {
			PARTNER_ENTITY.digimonEntity.entity.anim.animFlag &= 0xfd;
			moveMapObjectsWithLimit(MAIN_D_801353B4, MAIN_D_801353B6, 0, -1, 0, 0);
		}
		if ((MAIN_D_801353C2 % 2) == 0) {
			moveMapObjectsWithLimit(MAIN_D_801353B4, MAIN_D_801353B6, 2, 0, 0, 0);
		} else {
			moveMapObjectsWithLimit(MAIN_D_801353B4, MAIN_D_801353B6, -2, 0, 0, 0);
		}
		if ((MAIN_D_801353C2 % 10) == 0) {
			playSound(8, 0);
		}
		MAIN_D_801353C2++;
		r = 10;
		if (MAIN_D_801353BC == 1) {
			r = TRN2_getSlotSessionResult();
		}
		if ((((MAIN_D_801353C2 >= 0x4b0) || (POLLED_INPUT & 0x10)) && (MAIN_D_801353BC == 0)) || ((MAIN_D_801353BC == 1) && (0 <= r))) {
			MAIN_D_801353C2 %= 0x4b0;
			playSound(8, 0xa);
			MAIN_D_801353C2 = MAIN_D_801353C2 / 120;
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x12);
			TRN2_awardHpTrainingGains(PARTNER_ENTITY.digimonEntity.entity.type, 1, r);
			createCameraMovement(&TAMER_ENTITY.entity.posData->location, 10);
			MAIN_D_801353BD = 3;
		}
		break;
	case 3:
		if (TRN2_statGainsAreZero() == 1) {
			MAIN_D_801353C4 = 0;
			MAIN_D_801353BD = 4;
		}
		break;
	case 4:
		MAIN_D_801353C4++;
		if (MAIN_D_801353C4 >= 0x14) {
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
			MAIN_D_801353BD = 5;
		}
		break;
	case 5:
		MAIN_D_801353C2--;
		if (MAIN_D_801353C2 > 0) {
			moveMapObjectsWithLimit(MAIN_D_801353B4, MAIN_D_801353B6, -1, 2, 0, 0);
		}
		if ((tickEntityWalkTo(0xfc, 0xfd, 0, 0, 0) == 1) && (MAIN_D_801353C2 <= 0)) {
			TRN2_applyBaseStats();
			TRN2_closeUIBox(1);
			setPartnerState(1);
			loadMapObjectPosition(TRN2_D_8008DC54, TRN2_D_8008DC74, MAIN_D_801353B4, MAIN_D_801353B6);
			setTamerState(0);
			removeAllCloudFX();
			MAIN_D_801353C2 = 0;
			MAIN_D_801353BD = 0;
			removeObject(0xfab, instanceId);
			TRAINING_COMPLETE = 1;
		}
		break;
	}
}

void TRN2_saveTrainingStartTime(void)
{
	MAIN_D_801353CE = CURRENT_FRAME;
	MAIN_D_801353D0 = HOUR;
	MAIN_D_801353D2 = MINUTE;
}

static void trn2_hp_map107__garbage__(void)
{
	int32_t v0;
	int32_t v1;
	int32_t v2;
	int32_t v3;
	int32_t v4;
	int32_t v5;
	int32_t v6;
	int32_t v7;
	int32_t v8;
	int32_t v9;
	int32_t v10;
	int32_t v11;
	int32_t v12;
	int32_t v13;
	int32_t v14;
	int32_t v15;
	int32_t v16;
	int32_t v17;
	int32_t v18;
	int32_t v19;

	v0 = TRN2_D_8008DC54[0] + 0;
	v1 = TRN2_D_8008DC54[1] + 1;
	v2 = TRN2_D_8008DC54[2] + 2;
	v3 = TRN2_D_8008DC54[3] + 3;
	v4 = TRN2_D_8008DC54[4] + 4;
	v5 = TRN2_D_8008DC54[5] + 5;
	v6 = TRN2_D_8008DC54[6] + 6;
	v7 = TRN2_D_8008DC54[7] + 7;
	v8 = TRN2_D_8008DC54[0] + 8;
	v9 = TRN2_D_8008DC54[1] + 9;
	v10 = TRN2_D_8008DC54[2] + 10;
	v11 = TRN2_D_8008DC54[3] + 11;
	v12 = TRN2_D_8008DC54[4] + 12;
	v13 = TRN2_D_8008DC54[5] + 13;
	v14 = TRN2_D_8008DC54[6] + 14;
	v15 = TRN2_D_8008DC54[7] + 15;
	v16 = TRN2_D_8008DC54[0] + 16;
	v17 = TRN2_D_8008DC54[1] + 17;
	v18 = TRN2_D_8008DC54[2] + 18;
	v19 = TRN2_D_8008DC54[3] + 19;
	TRN2_D_8008DC54[0] = (v0 * v1) + v2;
	TRN2_D_8008DC54[1] = (v1 * v2) + v3;
	TRN2_D_8008DC54[2] = (v2 * v3) + v4;
	TRN2_D_8008DC54[3] = (v3 * v4) + v5;
	TRN2_D_8008DC54[4] = (v4 * v5) + v6;
	TRN2_D_8008DC54[5] = (v5 * v6) + v7;
	TRN2_D_8008DC54[6] = (v6 * v7) + v8;
	TRN2_D_8008DC54[7] = (v7 * v8) + v9;
	TRN2_D_8008DC54[0] = (v8 * v9) + v10;
	TRN2_D_8008DC54[1] = (v9 * v10) + v11;
	TRN2_D_8008DC54[2] = (v10 * v11) + v12;
	TRN2_D_8008DC54[3] = (v11 * v12) + v13;
	TRN2_D_8008DC54[4] = (v12 * v13) + v14;
	TRN2_D_8008DC54[5] = (v13 * v14) + v15;
	TRN2_D_8008DC54[6] = (v14 * v15) + v16;
	TRN2_D_8008DC54[7] = (v15 * v16) + v17;
	TRN2_D_8008DC54[0] = (v16 * v17) + v18;
	TRN2_D_8008DC54[1] = (v17 * v18) + v19;
	TRN2_D_8008DC54[2] = (v18 * v19) + v0;
	TRN2_D_8008DC54[3] = (v19 * v0) + v1;
}

void TRN2_setupMpTraining(int32_t arg)
{
	if (arg == 0xa5) {
		TRN2_D_8008DC2C[0] = 0x3e5;
		TRN2_D_8008DC2C[1] = 0;
		TRN2_D_8008DC2C[2] = 0x76;
		TRN2_D_8008DC1C[0] = 0x6f8;
		TRN2_D_8008DC1C[1] = 0;
		TRN2_D_8008DC1C[2] = 0x1d2;
		addObject(0xfaf, 8, (TickFunction)TRN2_tickMpTraining, NULL);
	}

	TRAINING_COMPLETE = 0;
	MAIN_D_801353BC = readPStat(0xf6);
	TRN2_saveTrainingStartTime();
	MAIN_D_801353BD = 0;
}
