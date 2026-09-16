#ifndef DW_VS_H
#define DW_VS_H

#include <libgte.h>

#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/graphics.h>
#include <dw/types.h>

extern char MAIN_D_80134A50[4];
extern char MAIN_D_80134A54[5];
extern uint8_t MAIN_D_80134A5C[4];
extern uint8_t MAIN_D_80134A60[4];
extern SVECTOR MAIN_D_80134A64;
extern SVECTOR MAIN_D_80134A6C;
extern SVECTOR MAIN_D_80134A74;
extern SVECTOR MAIN_D_80134A7C;
extern SVECTOR MAIN_D_80134A84;
extern SVECTOR MAIN_D_80134A8C;
extern SVECTOR MAIN_D_80134A94;
extern SVECTOR MAIN_D_80134A9C;
extern SVECTOR MAIN_D_80134AA4;
extern SVECTOR MAIN_D_80134AAC;
extern SVECTOR MAIN_D_80134AB4;
extern uint8_t MAIN_D_80134ABC[4];
extern uint8_t MAIN_D_80134AC0[4];
extern uint8_t MAIN_D_80134AC4[4];
extern int8_t MAIN_D_80134AC8[2];
extern uint8_t MAIN_D_80134AE8[5];
extern uint8_t MAIN_D_80134AF0[5];
extern uint8_t MAIN_D_80134AF8[8];
extern uint8_t MAIN_D_80134B00[8];
extern uint8_t MAIN_D_80134B08[8];
extern char *MAIN_D_80134B10;
extern int16_t MAIN_D_80134B14[4];
extern char MAIN_D_80134B1C[4];
extern int8_t MAIN_D_80134B20[4];
extern int8_t MAIN_D_80134B24[4];
extern int8_t MAIN_D_80134B28[8];
extern int8_t MAIN_D_80134B30[8];
extern int8_t MAIN_D_80134B38[8];
extern int32_t MAIN_D_80134B40;
extern uint8_t MAIN_D_80134B44[8];
extern int8_t MAIN_D_80134B4C[6];
extern SVECTOR MAIN_D_80134B54;
extern int16_t MAIN_D_80134B5C[4];
extern SVECTOR MAIN_D_80134B64;
extern RGB8 MAIN_D_80134B6C;
extern SVECTOR MAIN_D_80134B70;
extern SVECTOR MAIN_D_80134B78;
extern SVECTOR MAIN_D_80134B80;
extern SVECTOR MAIN_D_80134B88;
extern uint8_t VS_D_8006FF20[][14];
extern uint8_t VS_D_800716A8[];
extern uint8_t VS_D_800716AD[];
extern uint8_t VS_D_800716B2[];
extern uint8_t VS_D_800716B3[];
extern VECTOR VS_D_80071744;

int32_t VS_addAuraProjectile(Entity *e);
void VS_addCommandMenu(uint8_t index);
void VS_addFighterCounter(uint8_t arg);
void VS_addFighterStatusBars(int32_t id);
int32_t VS_addFinisherAura(int32_t arg, int32_t val);
void VS_addFinisherProgress(FighterData *fighter, int16_t amount);
void VS_addTargetCursor(/* int16_t id, int32_t tech */);
void VS_addVersusModelScene(void);
void VS_applyChargeRequirement(DigimonEntity *digimon, FighterData *fighter, int16_t tech);
void VS_applyMoveResult(void);
int32_t VS_getDistanceSquared(Entity *a, Entity *b);
void VS_initializeBattleStartText(void);
void VS_initializeBattleStartTextBurst(void);
char *VS_initializeEFEEngine(char *base);
void VS_initializeVS(void);
int32_t VS_isBattleStartTextFinished(void);
int32_t VS_isMoveUsable(DigimonEntity *digimon, FighterData *fighter, int16_t slot);
int32_t VS_isVersusModelSceneFinished(void);
void VS_loadMoveEFE(int16_t *moves, int16_t *effectIds, int8_t *isLoaded);
void VS_loadVersusSceneModel(void);
void VS_playMoveEffect(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
void VS_queueRandomMove(DigimonEntity *digimon, FighterData *fighter, int32_t tech);
void VS_removeAllAuraProjectiles(void);
void VS_removeAllFinisherAuras(void);
void VS_removeAllPoisonEffects(void);
void VS_removeAllStunEffects(void);
void VS_removeBattleStartText(void);
void VS_removeBattleStartTextBurst(void);
void VS_removeCommandMenu(int32_t i);
void VS_removeEFEEngine(void);
void VS_removeFighterCounter(void);
void VS_removeFighterStatusBars(int32_t i);
void VS_removeMoveEffect(DigimonEntity *digimon, FighterData *fighter);
void VS_removeResultModelScene(void);
void VS_removeStatusEffects(DigimonEntity *digimon, FighterData *fighter);
void VS_removeVersusModelScene(void);
void VS_resetFighterAction(FighterData *fighter);
void VS_resolveAttack(void);
int32_t VS_selectMoveByMpCost(/* int32_t arg0, int16_t *flags */);
int32_t VS_selectMoveByPower(/* int32_t arg0, int16_t *flags */);
int32_t VS_selectMoveTarget(Entity *entity, FighterData *fighter);
void VS_selectPartnerMove(DigimonEntity *digimon, FighterData *fighter, int16_t index);
void VS_selectRandomCamera(DigimonEntity *entity, int32_t mode, int32_t sub);
void VS_setRandomViewpoint(Entity *entity, int32_t idx);
void VS_setupQueuedMove(/* DigimonEntity *digimon, FighterData *fighter,
			int16_t arg2, int32_t moveIndex */);
void VS_startCameraChase(Entity *entity, int32_t dx, int32_t side);
void VS_startFighterMove(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
void VS_tickFrame(void);
void VS_unloadAllEFESlots(void);

#endif
