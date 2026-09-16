#ifndef DW_BTL_H
#define DW_BTL_H

#include <libgs.h>
#include <libgte.h>

#include <dw/entity.h>
#include <dw/font.h>
#include <dw/graphics.h>
#include <dw/math.h>
#include <dw/script.h>
#include <dw/types.h>

extern int16_t ENEMY_COUNT;
extern int16_t MAIN_D_80134D66;
extern int16_t MAIN_D_80134D68;
extern Entity *MAIN_D_80134D60;
extern int32_t MAIN_D_80134D70;
extern int32_t MAIN_D_80134D74;
extern int32_t MAIN_D_80134D7C[2];
extern uint8_t MAIN_D_80135078;
extern int32_t MAIN_D_8013507C;
extern int32_t MAIN_D_80135080;
extern StatsGains INITIAL_COMBAT_STATS[];
extern int8_t GAME_STATE;
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern uint32_t POLLED_INPUT;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern char *MOVE_NAMES[];
extern uint8_t MAIN_D_80134730[8];
extern int32_t MAIN_D_80135098;
extern int16_t MAIN_D_801350AE;
extern int16_t MAIN_D_801350B0;
extern int16_t MAIN_D_801350B2;
extern int16_t MAIN_D_801350B4;
extern int16_t MAIN_D_801350B6;
extern int16_t MAIN_D_801350B8;

extern int16_t MAIN_D_801346D8[4];
extern uint8_t MAIN_D_801346E0[5];
extern uint8_t MAIN_D_801346E8[4];
extern uint8_t MAIN_D_801346EC[4];
extern uint8_t MAIN_D_801346F0[4];
extern uint8_t MAIN_D_801346F4[4];
extern int8_t MAIN_D_801346F8;
extern uint8_t MAIN_D_80134728[5];
extern char MAIN_D_80134740[4];
extern char MAIN_D_80134744[6];
extern char MAIN_D_8013474C[3];
extern char MAIN_D_80134750[3];
extern char MAIN_D_80134754[3];
extern char MAIN_D_80134758[2];
extern char MAIN_D_8013475C[5];
extern uint8_t MAIN_D_80134764[8];
extern uint8_t MAIN_D_8013476C[8];
extern uint8_t MAIN_D_80134774[8];
extern char MAIN_D_8013477C[4];
extern int8_t MAIN_D_80134780[4];
extern int8_t MAIN_D_80134784[4];
extern int8_t MAIN_D_80134788[8];
extern int8_t MAIN_D_80134790[8];
extern int8_t MAIN_D_80134798[8];
extern int32_t MAIN_D_801347A0;
extern uint8_t MAIN_D_801347A4[8];
extern int8_t MAIN_D_801347AC[6];
extern SVECTOR MAIN_D_801347B4;
extern int16_t MAIN_D_801347BC[4];
extern SVECTOR MAIN_D_801347C4;
extern RGB8 MAIN_D_801347CC;
extern SVECTOR MAIN_D_801347D0;
extern SVECTOR MAIN_D_801347D8;
extern SVECTOR MAIN_D_801347E0;
extern SVECTOR MAIN_D_801347E8;
extern uint8_t MAIN_D_801347F0[4];
extern uint8_t MAIN_D_801347F4[4];
extern uint8_t MAIN_D_801347F8[4];

void BTL_initializeDeathCountdown(void);
void BTL_initializePartnerTile(void);
void BTL_initializeEnemyHPBarSprites(void);
void BTL_addEnemyHPBars(void);
void BTL_initializePartnerStatusBars(void);
void BTL_initializeCommandMenu(void);
void BTL_addDeathCountdown(Entity *entity);
void entityLookAtLocation(Entity *entity, VECTOR *pos);
int32_t entityCheckCollision(Entity *a, Entity *entity, int32_t c, int32_t d);
int32_t entityIsOffScreen(Entity *entity, int32_t width, int32_t height);
void startBattleIdleAnimation(DigimonEntity *entity, Stats *stats, int32_t flags);
void BTL_battleTickFrame(void);
void BTL_initializeBattleStartText(void);
void BTL_removeBattleStartText(void);
void BTL_initializeBattleStartTextBurst(void);
void BTL_removeBattleStartTextBurst(void);
int32_t BTL_isBattleStartTextFinished(void);
void BTL_handleBattleIntro(void);
void BTL_initializeCombat(void);
int32_t BTL_isBattleFinished(void);
void BTL_removeDeathCountdown(void);
int32_t BTL_isCommandMenuClosed(void);
void BTL_removePartnerStatusBars(void);
void BTL_removeFinisherChargeup(void);
void BTL_removeAllFinisherAuras(void);
void BTL_removeAllPoisonEffects(void);
void BTL_removeAllStunEffects(void);
void BTL_removeAllBuffRingsEffects(void);
void BTL_removeAllBuffDiskEffects(void);
void BTL_removeAllItemParticles(void);
void BTL_removeAllAuraProjectiles(void);
void BTL_unloadAllEFESlots(void);
void BTL_removeEFEEngine(void);
void BTL_drawHoveredCommandName(void);

#endif
