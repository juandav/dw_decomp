#include <dw/entity.h>
#include <dw/partner.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/trn2.h>
#include <dw/types.h>
#include <dw/ui.h>

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
extern int16_t MAIN_D_801353B8;
extern int16_t MAIN_D_801353BA;
extern int16_t MAIN_D_801353BE;
extern int16_t MAIN_D_801353C0;
extern int16_t TRN2_D_8008DC3C[];
extern int16_t TRN2_D_8008DC44[];
extern int16_t TRN2_D_8008DC4C[];
extern int16_t MAIN_D_801353CC;

void createCameraMovement(VECTOR *pos, int32_t speed);
void startAnimation(Entity *entity, int32_t animId);
void loadMapObjectPosition(int16_t *xData, int16_t *yData, int16_t startIndex, int16_t count);
int32_t tickEntityWalkTo(uint8_t scriptId1, uint8_t scriptId2, int32_t targetX, int32_t targetZ, int8_t withCamera);
void setMapObjectsFlag(int16_t startIndex, int16_t count, int32_t flag);
void setCameraFollowPlayer(void);
void createParticleFX(uint8_t kind, int32_t count, void *arg2, Entity *entity, int32_t arg4);
void resetMapObjectAnimation(int16_t startIndex, int16_t count);
void entityLookAtLocation(Entity *entity, VECTOR *pos);
void TRN2_awardDefenseTrainingGains(int32_t a, int32_t b, int32_t c);
void TRN2_saveTrainingStartTime(void);
void TRN2_tickDefenseTrainingMap108(int32_t instanceId);
void TRN2_tickSpeedTraining(int32_t instanceId);
void TRN2_startSlotSessionIfEnabled(int16_t arg);
int32_t TRN2_statGainsAreZero(void);
void TRN2_applyBaseStats(void);
void TRN2_closeUIBox(int32_t id);
int16_t TRN2_getSlotSessionResult(void);
void TRN2_startSlotSpin(void);

static void *trn2_def_map108_functions[] = {
	TRN2_setupSpeedTraining,
	TRN2_tickDefenseTrainingMap108,
};

void TRN2_tickDefenseTrainingMap108(int32_t instanceId)
{
	int32_t r;

	switch (MAIN_D_801353BD) {
	case 0:
		setTamerState(8);
		setPartnerState(10);
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
		createCameraMovement((VECTOR *)TRN2_D_8008DC1C, 10);
		MAIN_D_801353C2 = 0;
		MAIN_D_801353BD = 1;
		TRN2_startSlotSessionIfEnabled(3);
		break;
	case 1:
		if (tickEntityWalkTo(0xfc, 0xff, TRN2_D_8008DC1C[0], TRN2_D_8008DC1C[2], 0) == 1) {
			PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy = 0x800;
			setMapObjectsFlag(MAIN_D_801353B8, MAIN_D_801353BA, 1);
			setMapObjectsFlag(MAIN_D_801353BE, MAIN_D_801353C0, 0);
			resetMapObjectAnimation(MAIN_D_801353BE, MAIN_D_801353C0);
			MAIN_D_801353BD = 2;
			MAIN_D_801353CC = 0;
			if (MAIN_D_801353BC == 1) {
				TRN2_startSlotSpin();
			}
		}
		break;
	case 2:
		MAIN_D_801353C2++;
		MAIN_D_801353CC++;
		if (MAIN_D_801353CC == 4) {
			startAnimation(ENTITY_TABLE[1], 0x25);
			createParticleFX(0, 0, TRN2_D_8008DC3C, NULL, 0);
			createParticleFX(0, 0, TRN2_D_8008DC44, NULL, 0);
			createParticleFX(0, 0, TRN2_D_8008DC4C, NULL, 0);
			playSound(8, 4);
			MAIN_D_801353CC = 0;
			MAIN_D_801353BD = 3;
		}
		break;
	case 3:
		MAIN_D_801353CC++;
		if (MAIN_D_801353CC >= 0x1e) {
			resetMapObjectAnimation(MAIN_D_801353BE, MAIN_D_801353C0);
			MAIN_D_801353CC = 0;
			MAIN_D_801353BD = 2;
		}
		if (POLLED_INPUT & 0x10) {
			MAIN_D_801353C2 = 0x4b0;
		}
		MAIN_D_801353C2++;
		r = 10;
		if (MAIN_D_801353BC == 1) {
			r = TRN2_getSlotSessionResult();
		}
		if (((MAIN_D_801353C2 >= 0x4b0) && (MAIN_D_801353BC == 0)) || ((MAIN_D_801353BC == 1) && (r >= 0))) {
			setMapObjectsFlag(MAIN_D_801353B8, MAIN_D_801353BA, 0);
			setMapObjectsFlag(MAIN_D_801353BE, MAIN_D_801353C0, 1);
			TRN2_awardDefenseTrainingGains(PARTNER_ENTITY.digimonEntity.entity.type, instanceId, r);
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x12);
			createCameraMovement(&TAMER_ENTITY.entity.posData->location, 10);
			playSound(8, 0xa);
			MAIN_D_801353BD = 4;
		}
		break;
	case 4:
		if (TRN2_statGainsAreZero() == 1) {
			MAIN_D_801353C4 = 0;
			MAIN_D_801353BD = 5;
		}
		break;
	case 5:
		MAIN_D_801353C4++;
		if (MAIN_D_801353C4 >= 0x14) {
			entityLookAtLocation(ENTITY_TABLE[1], &TAMER_ENTITY.entity.posData->location);
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
			MAIN_D_801353BD = 6;
		}
		break;
	case 6:
		if (tickEntityWalkTo(0xfc, 0xfd, 0, 0, 0) == 1) {
			TRN2_applyBaseStats();
			TRN2_closeUIBox(1);
			loadMapObjectPosition(TRN2_D_8008DC54, TRN2_D_8008DC74, MAIN_D_801353B4, MAIN_D_801353B6);
			setTamerState(0);
			setCameraFollowPlayer();
			setPartnerState(1);
			MAIN_D_801353C2 = 0;
			MAIN_D_801353BD = 0;
			removeObject(0xfae, instanceId);
			TRAINING_COMPLETE = 1;
		}
		break;
	}
}

static void trn2_def_map108__garbage__(void)
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

void TRN2_setupSpeedTraining(int32_t arg)
{
	if (arg == 0x6c) {
		TRN2_D_8008DC2C[0] = 0xf7;
		TRN2_D_8008DC2C[1] = 0;
		TRN2_D_8008DC2C[2] = 0x226;
		TRN2_D_8008DC1C[0] = 0x372;
		TRN2_D_8008DC1C[1] = 0;
		TRN2_D_8008DC1C[2] = 0x3c5;
		MAIN_D_801353B8 = 0x38;
		MAIN_D_801353BA = 3;
		addObject(0xfad, 1, (TickFunction)TRN2_tickSpeedTraining, NULL);
	}

	MAIN_D_801353BC = readPStat(0xf6);
	TRAINING_COMPLETE = 0;
	TRN2_saveTrainingStartTime();
	MAIN_D_801353BD = 0;
}
