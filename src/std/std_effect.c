#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libcd.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/aabb.h>
#include <dw/clock.h>
#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/math.h>
#include <dw/model.h>
#include <dw/move.h>
#include <dw/params.h>
#include <dw/sound.h>
#include <dw/types.h>

#include "common.h"

#define EFE_POP(ptr, type) ((type) * --(ptr))
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
	int32_t rotation[3];
} EfeRot;

typedef struct {
	uint8_t pad0[0xc];
	int16_t frameCount;
	int16_t frame;
	int16_t timer;
	int16_t unk12;
	int16_t unk14;
	int16_t unk16;
	int32_t start;
	int32_t cur;
} EfeUvAnim;

typedef struct {
	int16_t state;
	int16_t pad;
	int8_t *isLoaded;
	int16_t *moves;
	int16_t *effectIds;
	ModelComponent *model;
} EfeLoad;

extern int32_t *MAIN_D_80134D0C;
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern int32_t MAIN_D_80134CEC;
extern int16_t *MAIN_D_80134D00;
extern int16_t MAIN_D_80134D04;
extern int32_t MAIN_D_80134D08;
extern int32_t MAIN_D_80134D14;
extern int32_t MAIN_D_80134CD4;
extern int16_t MAIN_D_80139AB0[];
extern int32_t *EFE_DATA_STACK;
extern SVECTOR STD_D_8007FE24[];
extern int32_t MAIN_D_801351D0;
extern VECTOR STD_D_8007B028;
extern VECTOR STD_D_8007B038;
extern int8_t MAIN_D_801348E8[4];
extern int8_t MAIN_D_801348EC[4];
extern int16_t STD_D_8007B058[];
extern SVECTOR MAIN_D_80134938;
extern SVECTOR MAIN_D_80134940;
extern SVECTOR MAIN_D_80134948;
extern SVECTOR MAIN_D_80134950;
extern SVECTOR MAIN_D_8013491C;
extern VECTOR STD_D_8007B018;
extern GsSPRITE STD_POISON_BUBBLE_SPRITE;
extern int8_t MAIN_D_80134914[6];
extern VECTOR STD_D_8007AF78;
extern int32_t EFE_SCRIPT_MEM1_DATA[];
extern int32_t MAIN_D_80139B54[];
extern int32_t MAIN_D_80134CD0;
extern int32_t (*STD_D_8007AF08[])(int32_t);
extern void (*STD_D_8007AE68[][8])(int32_t *);
extern int32_t MAIN_D_801351C8;
extern char *MAIN_D_801351D4;
extern int32_t MAIN_D_80134CF0;
extern int32_t MAIN_D_80134D18;
extern int32_t STD_D_8007AB18[];
extern void *STD_D_8007AB1C[];
extern int16_t EFE_LOADED_MOVE_DATA[];
extern int32_t *MAIN_D_80134CFC;
extern int32_t MAIN_D_80134CE0;
extern int16_t STD_D_8007FD10[][6];
extern int16_t STD_D_8007FE14[];
extern char *MAIN_D_801351CC;
extern int32_t EFE_PARENT_INSTANCE;
extern char *MAIN_D_80134D10;
extern int32_t MAIN_D_80134CE4;
extern int16_t STD_D_8007FCB0[][4];
extern EfeSubEffect *MAIN_D_80134CE8;
extern EfeInstance *EFE_INSTANCE;
extern int16_t STD_D_8007FCD0[][8];
extern int16_t STD_D_8007FD4C[][4];
extern int32_t MAIN_D_80139AD0[][2];
extern void (*STD_jtbl_8007FA7C[])(void);
extern char *MAIN_D_80134CCC;
extern int32_t MAIN_D_80134CD8;
extern char MAIN_D_801348E4[8];
extern int16_t STD_D_8007FC20[];
extern void (*STD_jtbl_8007AE20[])(void);
extern int32_t MAIN_D_801351B8;
extern int32_t STD_D_8007FC00[];
extern int32_t MAIN_D_801351C0;
extern int32_t MAIN_D_801351C4;
extern int16_t MAIN_D_80139B20[];
extern int8_t *MAIN_D_80139B24[];
extern int32_t MAIN_D_80139B28[];
extern int32_t MAIN_D_80139B2C[];
extern int32_t VIEWPORT_DISTANCE;
extern ModelComponent UNKNOWN_MODEL[16];
extern GsRVIEW2 MAIN_D_80139B34;
extern int32_t MAIN_D_801350F4;
extern PositionData STD_D_8007F528[];
extern PositionData STD_D_8007C7B0[4];
extern int16_t STD_D_8007FA1C[];
extern char STD_D_8007FA5C[];
extern int16_t STD_D_8007AA70[];
extern int16_t STD_D_8007AA40[];
extern int16_t STD_D_8007FA08[];
extern int16_t MAIN_D_801351A4;
extern GsOT STD_D_8007B714[];
extern int32_t STD_D_8007AA10[];
extern GsRVIEW2 GS_VIEWPOINT;
extern int32_t ACTIVE_FRAMEBUFFER;
extern int16_t STD_D_8007CC78[];
extern int16_t STD_D_8007CCA4[];
extern int16_t STD_D_8007CCD0[];
extern int16_t STD_D_8007CCFC[];

void startAnimation(Entity *entity, int32_t animId);
void entityLookAtLocation(Entity *entity, VECTOR *pos);
void setMapLayerEnabled(int32_t enabled);
void MAIN_func_80092B60(POLY_FT4 *prim);
void addScreenPolyFT3(void *prim, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2);
void renderLinePrimitive(uint32_t color, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t layer, int32_t flag);
int32_t add3DSpritePrim(POLY_FT4 *poly, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3);
void renderSprite(GsSPRITE *sprite, int16_t x, int16_t y, int32_t distance, int32_t width, int32_t height);
void translateConditionFXToEntity(Entity *entity, SVECTOR *out);
void unloadModel(int32_t id, int32_t flag);
int32_t addAttackObject(int32_t a, int32_t b, int16_t *rect, int32_t d, int32_t e, int32_t f);
int32_t MAIN_func_800DA740(int16_t *rect, DVECTOR *line);
char *initializeFlashData(char *base);
void renderParticleFlash(int16_t *params);
void calculatePosition(GsCOORDINATE2 *coord, MATRIX *matrix);
void MAIN_func_80092C18(PACKET *prim, RECT *rect);
int32_t lerp(int32_t start, int32_t end, int32_t t0, int32_t t1, int32_t t);
void STD_renderFinisherAura(int32_t idx);
void renderTMDModel(uint8_t *buffer, int32_t id, GsCOORDINATE2 *coord, GsCOORDINATE2 *super, VECTOR *trans, SVECTOR *rot, VECTOR *scale);
void calculateBoneMatrix(Entity *entity, int32_t boneId, MATRIX *out);
int32_t worldPosToScreenPos(SVECTOR *pos, DVECTOR *out);
void multiplyRotations(SVECTOR *a, SVECTOR *b);
int32_t getOriginalType(int32_t type);
void MAIN_func_800E4470(MATRIX *m, SVECTOR *out);
void toEulerAngles(SVECTOR *out, int32_t x, int32_t y, int32_t z);
void matrixToEuler2(MATRIX *m, SVECTOR *out);
int32_t customRandom(int32_t a, int32_t b);
int32_t getDistance(int32_t x, int32_t y, int32_t z);
void createCloudFX(int16_t *pos);
int32_t addEntityParticleFX(int32_t *typePtr, int32_t timer);
void setShortWithStride(int16_t *ptr, int16_t value, int32_t count, int32_t stride);
void setFileReadCallback2(void *callback, int32_t arg);
void STD_setupLoadedEFEFile(void);
void STD_renderParticleEmitters(void);
void STD_renderProjectedSprite(void);
void STD_renderParallelLines(SVECTOR *a, SVECTOR *b, int32_t n, SVECTOR *from, SVECTOR *to, int32_t *col);
void STD_renderStunEffect(int32_t idx);
void downloadSomeImage();
void modifySomeImage(int32_t dim);
void addObject(int32_t objectId, int32_t instanceId, void *tick, void *render);
void removeObject(int32_t objectId, int32_t instanceId);
void STD_addEnemyHPBars(void);
void STD_func_8006C6D0(void);
void STD_func_8006CCD4(void);
void STD_func_8006D15C(void);
char *STD_initializeParticleEmitters(char *base);
void STD_tickEFEEngine(void);
void STD_renderEFEEngine(void);
void STD_clearEFESoundChannels(void);
void STD_stopEFESounds(void);
void STD_unloadEFESlot(int32_t idx);
void STD_runEFESlotScript(int32_t idx);
void STD_handleEFEFileLoaded(int32_t arg);
void STD_tickParticleEmitters(void);
int16_t STD_offsetEFEPrimitiveUVs(char *base, int32_t idx, int32_t du, int32_t dv);
char *STD_initializeEFEEngine(char *base);
void STD_removeEFEEngine(void);
void STD_loadMoveEFE(int16_t *moves, int16_t *effectIds, int8_t *isLoaded);
void STD_unloadAllEFESlots(void);
int32_t STD_startEFE(int32_t i);
void STD_stopEFESubEffect(int32_t a, int32_t b);
int32_t STD_func_8006EA6C(void);
char *STD_func_8006EA78(char *p);
char *STD_func_8006EAAC(char *p);
int32_t STD_getEFEFileId(int32_t *p);
void STD_isTargetUnhit(void);
void STD_markEFEFinished(void);
void STD_func_8006EB58(void);
void STD_disableMapLayer(void);
void STD_renderScreenFade(void);
void STD_applyBoxAttackHit(void);
void STD_applyLineAttackHit(void);
void STD_faceTargetEntity(void);
void STD_renderScreenOverlay(void);
void STD_initializeRibbonPoints(void);
void STD_func_80070A40(void);
void STD_addClutLoadPrim(void);
void STD_drawTMDScreenSpace(void);
void STD_loadClutColors(void);
void STD_func_80070EA0(void);
void STD_getCameraRotation(void);
void STD_convertToViewSpace(void);
void STD_maskVectorByScalar(void);
void STD_divideVectorByScalar(void);
void STD_multiplyVectorByScalar(void);
void STD_render3DTexturedQuad(void);
void STD_discardEFEOperand(void);
void STD_renderWireframeGrid(void);
void STD_render2DTexturedQuad(void);
void STD_restoreCameraView(void);
void STD_getSourceBoneTransform(void);
void STD_copyToParentTransform(void);
void STD_renderEFELine(void);
void STD_combineRotations(void);
void STD_func_800728D4(void);
void STD_rotateVectorByAngles(void);
void STD_getTargetBoneTransform(void);
void STD_centerTransformOnEntities(void);
void STD_shiftVectorsRight(void);
void STD_maskVectors(void);
void STD_divideVectors(void);
void STD_multiplyVectors(void);
void STD_subtractVectors(void);
void STD_addVectors(void);
void STD_copyVector(void);
void STD_getVectorLength(void);
void STD_setTargetToHitEntity(void);
void STD_func_80072FF8(void);
void STD_findHitEntity(void);
void STD_getVectorEulerAngles(void);
void STD_printDebugValue(void);
void STD_getRandomInRange(void);
void STD_interpolateValue(void);
void STD_func_800733CC(void);
void STD_func_80073440(void);
void STD_func_800734B4(void);
void STD_getUVAnimTimer(void);
void STD_checkTargetCollision(void);
void STD_rotateTransformTowardPoint(void);
void STD_setTransformToSourceBone(void);
void STD_renderParallaxSprites(void);
void STD_playEFESound(void);
void STD_addSourceEntityParticleFX(void);
void STD_copyFromParentTransform(void);
void STD_calculatePolarOffset(void);
void STD_func_80074A70(void);
void STD_renderParticleFlashSprite(void);
void STD_projectPositionToScreen(void);
void STD_renderScreenSprite(void);
void STD_addCloudEffect(void);
void STD_selectNextTargetEntity(void);
void STD_addParticleEmitter(void);
void STD_setEFEModelObjectColor(void);
void STD_copyTargetEntityPosition(void);
void STD_steerTransformTowardPoint(void);
void STD_interpolateVector(void);
void STD_discardEFEOperandPair(void);
void STD_getScatteredSpawnPosition(void);
void STD_checkCollisionWithDefaultPower(void);
void STD_addAttackObjectToTarget(void);
void STD_setTransformToTargetBone(void);
void STD_renderCenteredSprite(void);
void STD_initializeEFETransform(void);
void STD_func_80075BF8(void);
void STD_initializeSubEffectInstructions(void);
void STD_initializeUVAnim(void);
void STD_checkTechCompatibility(void);
void STD_popEFEValueToVariable(void);
void STD_returnFromEFESubroutine(void);
void STD_dispatchEFESubOpcode(void);
void STD_callEFESubroutine(void);
void STD_pushEFEVariableAddress(void);
void STD_pushEFEVariable(void);
void STD_pushEFEImmediate(void);
void STD_jumpEFEScript(void);
void STD_stopEFEScript(void);
void STD_branchEFEOnComparison(void);
void STD_applyEFEVariableOperator(void);
void STD_loadEFEIndexedVariable(void);
void STD_loadEFERandomValue(void);
void STD_loadEFEVariable(void);
void STD_loadEFEImmediate(void);
int32_t STD_shiftRightInt32Variable(int32_t *p);
int32_t STD_shiftLeftInt32Variable(int32_t *p);
int32_t STD_moduloInt32Variable(int32_t *p);
int32_t STD_divideInt32Variable(int32_t *p);
int32_t STD_multiplyInt32Variable(int32_t *p);
int32_t STD_subtractInt32Variable(int32_t *p);
int32_t STD_addInt32Variable(int32_t *p);
int32_t STD_setInt32Variable(int32_t *p);
int32_t STD_shiftRightInt8Variable(int8_t *p);
int32_t STD_shiftLeftInt8Variable(int8_t *p);
int32_t STD_moduloInt8Variable(int8_t *p);
int32_t STD_divideInt8Variable(int8_t *p);
int32_t STD_multiplyInt8Variable(int8_t *p);
int32_t STD_subtractInt8Variable(int8_t *p);
int32_t STD_addInt8Variable(int8_t *p);
int32_t STD_setInt8Variable(int8_t *p);
int32_t STD_shiftRightInt16Variable(int16_t *p);
int32_t STD_shiftLeftInt16Variable(int16_t *p);
int32_t STD_moduloInt16Variable(int16_t *p);
int32_t STD_divideInt16Variable(int16_t *p);
int32_t STD_multiplyInt16Variable(int16_t *p);
int32_t STD_subtractInt16Variable(int16_t *p);
int32_t STD_addInt16Variable(int16_t *p);
int32_t STD_setInt16Variable(int16_t *p);
int32_t STD_compareGreaterOrEqual(int32_t x);
int32_t STD_compareGreater(int32_t x);
int32_t STD_compareLessOrEqual(int32_t x);
int32_t STD_compareLess(int32_t x);
int32_t STD_compareNotEqual(int32_t x);
int32_t STD_compareEqual(int32_t x);
int16_t STD_calculateAttackHitPosition(SVECTOR *out, int32_t *self, int32_t *other, int32_t y);
int32_t STD_func_800770C0(int32_t lo, int32_t hi, int32_t t, int32_t start, int32_t end);
void STD_initializeEFESubOpcodeTable(void);
void STD_dispatchEFEOpcode(int32_t op);
int32_t STD_runEFEScript(int32_t script);
void STD_resetPoisonBubbles(void);
int32_t STD_addPoisonBubble(int32_t arg);
void STD_tickPoisonBubble(int32_t i);
void STD_renderPoisonBubble(int32_t i);
void STD_tickPoisonEffect(int32_t i);
void STD_func_80077620(void);
void STD_initializePoisonBubble(void);
int32_t STD_func_80077664(int32_t arg);
void STD_removePoisonEffect(int32_t i, int32_t arg);
void STD_removeAllPoisonEffects(void);
void STD_tickConfusionEffect(int32_t i);
void STD_renderConfusionEffect(int32_t idx);
void STD_initializeConfusionEffect(char *base);
int32_t STD_addConfusionEffect(int32_t arg);
void STD_removeConfusionEffect(int32_t i, int32_t arg);
void STD_resetStunSubEffects(void);
void STD_tickStunEffect(int32_t i);
void STD_removeAllStunSubEffects(void);
int32_t STD_func_80078044(int32_t arg);
void STD_tickStunSubEffect(int32_t i);
void STD_renderStunSubEffect(int32_t i);
int32_t STD_addStunEffect(int32_t arg, int32_t val);
void STD_removeStunEffect(int32_t i, int32_t arg);
void STD_removeAllStunEffects(void);
void STD_setTMDObjectColor(int32_t idx, int32_t *color, char *base);
void STD_tickFinisherAura(int32_t i);
void STD_func_80078BBC(char *pos, int32_t scale, SVECTOR *dir, uint8_t *col);
void STD_initializeFinisherAuraModel(char *tim, char *base);
int32_t STD_addFinisherAura(int32_t arg, int32_t val);
void STD_removeFinisherAura(int32_t i);
void STD_func_800791E0(void);
void STD_renderAuraProjectile(int32_t i);
char *STD_initializeAuraProjectiles(char *base);
int32_t STD_addAuraProjectile(Entity *e);
void STD_func_8006BFD4(void);
void STD_func_8006C7D4(void);
void setupModelMatrix(PositionData *posData);
void STD_applyRadiusAttackHit(void);
void STD_renderRingTube(void);
void STD_renderRibbonStrip(void);
void STD_renderRadialWaves(void);
void STD_selectRandomTargetEntity(void);
void STD_renderWireframeBox(void);
void STD_setupFixedCamera(void);
void STD_applyHomingMovement(void);
void STD_renderScrollingBackground(void);
void STD_spawnEFESubEffect(void);
int32_t addFileReadRequest(char *path, uint8_t *buffer, uint8_t *isRunning, void *callback, void *callbackParam, CdlLOC *loc, int32_t size);
CdlLOC *getEFEDATEntry(int32_t id);
void STD_func_8006BFB4(void);
void STD_func_8006C630(void);
void STD_func_80079874(void);
void swapByte(char *a, char *b);
void STD_loadNextEFEFile(int16_t *arg);
void STD_initializeStunEffect(char *base);
void STD_tickEFEUVAnimation(int32_t idx);
void STD_setTransformToBoneMatrix(void);
void STD_setTransformToBoneOffset(void);
void STD_func_8006C67C(void);
void STD_tickAuraProjectile(int32_t id);
void STD_func_8006CCE0(void);
void STD_func_8006D018(void);
void STD_renderVersusModelScene2(void);
void STD_func_8006C6DC(void);
void STD_func_8006CB10(void);
void STD_func_8006CE68(void);
void STD_tickRibbonPoints(void);

static void *std_effect_functions[] = {
	STD_func_80079874,
	STD_addAuraProjectile,
	STD_initializeAuraProjectiles,
	STD_renderAuraProjectile,
	STD_tickAuraProjectile,
	STD_func_800791E0,
	STD_removeFinisherAura,
	STD_addFinisherAura,
	STD_initializeFinisherAuraModel,
	STD_func_80078BBC,
	STD_renderFinisherAura,
	STD_tickFinisherAura,
	STD_setTMDObjectColor,
	STD_removeAllStunEffects,
	STD_removeStunEffect,
	STD_addStunEffect,
	STD_initializeStunEffect,
	STD_renderStunSubEffect,
	STD_tickStunSubEffect,
	STD_func_80078044,
	STD_removeAllStunSubEffects,
	STD_renderStunEffect,
	STD_tickStunEffect,
	STD_resetStunSubEffects,
	STD_removeConfusionEffect,
	STD_addConfusionEffect,
	STD_initializeConfusionEffect,
	STD_renderConfusionEffect,
	STD_tickConfusionEffect,
	STD_removeAllPoisonEffects,
	STD_removePoisonEffect,
	STD_func_80077664,
	STD_initializePoisonBubble,
	STD_func_80077620,
	STD_tickPoisonEffect,
	STD_renderPoisonBubble,
	STD_tickPoisonBubble,
	STD_addPoisonBubble,
	STD_resetPoisonBubbles,
	STD_runEFEScript,
	STD_dispatchEFEOpcode,
	STD_initializeEFESubOpcodeTable,
	STD_func_800770C0,
	STD_renderParallelLines,
	STD_calculateAttackHitPosition,
	STD_compareEqual,
	STD_compareNotEqual,
	STD_compareLess,
	STD_compareLessOrEqual,
	STD_compareGreater,
	STD_compareGreaterOrEqual,
	STD_setInt16Variable,
	STD_addInt16Variable,
	STD_subtractInt16Variable,
	STD_multiplyInt16Variable,
	STD_divideInt16Variable,
	STD_moduloInt16Variable,
	STD_shiftLeftInt16Variable,
	STD_shiftRightInt16Variable,
	STD_setInt8Variable,
	STD_addInt8Variable,
	STD_subtractInt8Variable,
	STD_multiplyInt8Variable,
	STD_divideInt8Variable,
	STD_moduloInt8Variable,
	STD_shiftLeftInt8Variable,
	STD_shiftRightInt8Variable,
	STD_setInt32Variable,
	STD_addInt32Variable,
	STD_subtractInt32Variable,
	STD_multiplyInt32Variable,
	STD_divideInt32Variable,
	STD_moduloInt32Variable,
	STD_shiftLeftInt32Variable,
	STD_shiftRightInt32Variable,
	STD_loadEFEImmediate,
	STD_loadEFEVariable,
	STD_loadEFERandomValue,
	STD_loadEFEIndexedVariable,
	STD_applyEFEVariableOperator,
	STD_branchEFEOnComparison,
	STD_stopEFEScript,
	STD_jumpEFEScript,
	STD_pushEFEImmediate,
	STD_pushEFEVariable,
	STD_pushEFEVariableAddress,
	STD_callEFESubroutine,
	STD_dispatchEFESubOpcode,
	STD_returnFromEFESubroutine,
	STD_popEFEValueToVariable,
	STD_spawnEFESubEffect,
	STD_checkTechCompatibility,
	STD_initializeUVAnim,
	STD_initializeSubEffectInstructions,
	STD_func_80075BF8,
	STD_initializeEFETransform,
	STD_renderCenteredSprite,
	STD_setTransformToTargetBone,
	STD_addAttackObjectToTarget,
	STD_checkCollisionWithDefaultPower,
	STD_getScatteredSpawnPosition,
	STD_discardEFEOperandPair,
	STD_interpolateVector,
	STD_steerTransformTowardPoint,
	STD_copyTargetEntityPosition,
	STD_setEFEModelObjectColor,
	STD_addParticleEmitter,
	STD_selectNextTargetEntity,
	STD_addCloudEffect,
	STD_renderScreenSprite,
	STD_projectPositionToScreen,
	STD_renderParticleFlashSprite,
	STD_func_80074A70,
	STD_renderProjectedSprite,
	STD_calculatePolarOffset,
	STD_copyFromParentTransform,
	STD_addSourceEntityParticleFX,
	STD_playEFESound,
	STD_setTransformToBoneOffset,
	STD_renderScrollingBackground,
	STD_renderParallaxSprites,
	STD_setTransformToSourceBone,
	STD_rotateTransformTowardPoint,
	STD_checkTargetCollision,
	STD_getUVAnimTimer,
	STD_applyHomingMovement,
	STD_func_800734B4,
	STD_func_80073440,
	STD_func_800733CC,
	STD_interpolateValue,
	STD_getRandomInRange,
	STD_printDebugValue,
	STD_getVectorEulerAngles,
	STD_findHitEntity,
	STD_func_80072FF8,
	STD_setTargetToHitEntity,
	STD_getVectorLength,
	STD_copyVector,
	STD_addVectors,
	STD_subtractVectors,
	STD_multiplyVectors,
	STD_divideVectors,
	STD_maskVectors,
	STD_shiftVectorsRight,
	STD_centerTransformOnEntities,
	STD_getTargetBoneTransform,
	STD_rotateVectorByAngles,
	STD_func_800728D4,
	STD_combineRotations,
	STD_renderEFELine,
	STD_copyToParentTransform,
	STD_getSourceBoneTransform,
	STD_setupFixedCamera,
	STD_restoreCameraView,
	STD_render2DTexturedQuad,
	STD_renderWireframeGrid,
	STD_discardEFEOperand,
	STD_renderWireframeBox,
	STD_setTransformToBoneMatrix,
	STD_render3DTexturedQuad,
	STD_multiplyVectorByScalar,
	STD_divideVectorByScalar,
	STD_maskVectorByScalar,
	STD_convertToViewSpace,
	STD_selectRandomTargetEntity,
	STD_getCameraRotation,
	STD_func_80070EA0,
	STD_loadClutColors,
	STD_drawTMDScreenSpace,
	STD_addClutLoadPrim,
	STD_func_80070A40,
	STD_renderRadialWaves,
	STD_initializeRibbonPoints,
	STD_tickRibbonPoints,
	STD_renderRibbonStrip,
	STD_renderRingTube,
	STD_renderScreenOverlay,
	STD_faceTargetEntity,
	STD_applyLineAttackHit,
	STD_applyRadiusAttackHit,
	STD_applyBoxAttackHit,
	STD_renderScreenFade,
	STD_disableMapLayer,
	STD_func_8006EB58,
	STD_markEFEFinished,
	STD_isTargetUnhit,
	STD_getEFEFileId,
	STD_func_8006EAAC,
	STD_func_8006EA78,
	STD_func_8006EA6C,
	STD_stopEFESubEffect,
	STD_startEFE,
	STD_unloadAllEFESlots,
	STD_loadMoveEFE,
	STD_removeEFEEngine,
	STD_initializeEFEEngine,
	STD_offsetEFEPrimitiveUVs,
	STD_renderParticleEmitters,
	STD_tickParticleEmitters,
	STD_tickEFEUVAnimation,
	STD_handleEFEFileLoaded,
	STD_setupLoadedEFEFile,
	STD_runEFESlotScript,
	STD_unloadEFESlot,
	STD_loadNextEFEFile,
	STD_stopEFESounds,
	STD_clearEFESoundChannels,
	STD_renderEFEEngine,
	STD_tickEFEEngine,
	STD_initializeParticleEmitters,
	STD_func_8006D15C,
	STD_func_8006D018,
	STD_func_8006CE68,
	STD_func_8006CCE0,
	STD_func_8006CCD4,
	STD_func_8006CB10,
	STD_func_8006C7D4,
	STD_func_8006C6DC,
	STD_func_8006C6D0,
	STD_func_8006C67C,
	STD_addEnemyHPBars,
	STD_func_8006C630,
	STD_renderVersusModelScene2,
	STD_func_8006BFD4,
	STD_func_8006BFB4,
};

void STD_func_8006BFB4(void)
{
	MAIN_D_801350F4 = 0;
	addObject(0x19D, 0, STD_func_8006BFD4, STD_renderVersusModelScene2);
}

INCLUDE_ASM("asm/std/nonmatchings/std_effect", STD_func_8006BFD4);

void STD_renderVersusModelScene2(void)
{
	MATRIX lw;
	MATRIX ls;
	int32_t i;

	GsSetProjection(0x200);
	GsWSMATRIX = *(MATRIX *)STD_D_8007AA10;
	GsClearOt(0, 4, &STD_D_8007B714[ACTIVE_FRAMEBUFFER]);

	for (i = 0; i < 9; i++) {
		GsGetLws(STD_D_8007C7B0[i].obj.coord2, &lw, &ls);
		GsSetLightMatrix(&lw);
		GsSetLsMatrix(&ls);
		GsSortObject4(&STD_D_8007C7B0[i].obj, &STD_D_8007B714[ACTIVE_FRAMEBUFFER], 9, getScratchAddr(0));
	}

	GsSortOt(&STD_D_8007B714[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

void STD_func_8006C630(void)
{
	removeObject(0x19D, 0);
	stopBGM();
	stopSound();
}

void STD_addEnemyHPBars(void)
{
	addObject(0x1AE, 0, NULL, STD_func_8006C67C);
}

void STD_func_8006C67C(void)
{
	MATRIX lw;
	MATRIX ls;

	GsGetLws(STD_D_8007F528[0].obj.coord2, &lw, &ls);
	GsSetLightMatrix(&lw);
	GsSetLsMatrix(&ls);
	GsSortObject4(&STD_D_8007F528[0].obj, ACTIVE_ORDERING_TABLE, 2, getScratchAddr(0));
}
void STD_func_8006C6D0(void)
{
	removeObject(0x1AE, 0);
}

void STD_func_8006C6DC(void)
{
	int32_t i;

	for (i = 0; i < 0x20; i++) {
		STD_D_8007FA1C[i] = 0;
		STD_D_8007FA5C[i] = i;
	}

	for (i = 0; i < 0x20; i++) {
		swapByte(&STD_D_8007FA5C[i], &STD_D_8007FA5C[random(0x20)]);
	}

	MAIN_D_801351A4 = 0;

	for (i = 0; i < 4; i++) {
		STD_D_8007FA08[i] = 0;
	}

	addObject(0x19d, 0, STD_func_8006C7D4, STD_func_8006CB10);
}

void STD_func_8006C7D4(void)
{
	int32_t frame;
	int32_t i;
	int32_t j;
	int32_t k;
	PositionData *p;

	if (MAIN_D_801351A4 < 0x8c) {
		MAIN_D_801351A4++;
	}
	if ((frame = MAIN_D_801351A4) >= 0x3d && STD_D_8007FA08[0] < 0x16) {
		STD_D_8007FA08[0]++;
	}
	if (frame >= 0x40 && STD_D_8007FA08[1] < 0x16) {
		STD_D_8007FA08[1]++;
	}
	if (frame >= 0x43 && STD_D_8007FA08[2] < 0x16) {
		STD_D_8007FA08[2]++;
	}
	if (frame >= 0x46 && STD_D_8007FA08[3] < 0x16) {
		STD_D_8007FA08[3]++;
	}
	for (i = 0; i < 4; i++) {
		if (STD_D_8007FA08[i] >= 0xb) {
			for (j = 0; j < 8; j++) {
				k = ((uint8_t *)STD_D_8007FA5C + j)[i * 8];
				if (STD_D_8007FA1C[k] < 0xf0) {
					STD_D_8007FA1C[k] += 0x3c;
				}
			}
		}
	}
	if (frame >= 0x74) {
		if (frame < 0x82) {
			STD_D_8007F528[4].rotation.vx = STD_D_8007AA70[frame - 0x74];
			STD_D_8007F528[5].rotation.vx = STD_D_8007F528[4].rotation.vx;
			STD_D_8007F528[6].rotation.vx = STD_D_8007F528[4].rotation.vx;
			STD_D_8007F528[7].rotation.vx = STD_D_8007F528[4].rotation.vx;
		}
		if (frame == 0x7f) {
			STD_D_8007FA08[0] = 0;
		}
		if (frame >= 0x80) {
			STD_D_8007F528[4].location.vy = STD_D_8007AA40[STD_D_8007FA08[0]] + 0x320;
			STD_D_8007F528[5].location.vy = STD_D_8007F528[4].location.vy;
			STD_D_8007F528[6].location.vy = STD_D_8007F528[4].location.vy;
			STD_D_8007F528[7].location.vy = STD_D_8007F528[4].location.vy;
		}
	} else {
		STD_D_8007F528[4].location.vy = STD_D_8007AA40[STD_D_8007FA08[0]];
		STD_D_8007F528[5].location.vy = STD_D_8007AA40[STD_D_8007FA08[1]];
		STD_D_8007F528[6].location.vy = STD_D_8007AA40[STD_D_8007FA08[2]];
		STD_D_8007F528[7].location.vy = STD_D_8007AA40[STD_D_8007FA08[3]];
	}
	for (i = 4; i < 8; i++) {
		p = &STD_D_8007F528[i];
		setupModelMatrix(p);
	}
}
void STD_func_8006CB10(void)
{
	MATRIX lw;
	MATRIX ls;
	GsBOXF box;
	int32_t i;

	for (i = 0; i < 0x20; i++) {
		box.attribute = 0x40000000;
		box.b = 0;
		box.g = 0;
		box.r = 0;
		box.w = 0xa;
		box.h = STD_D_8007FA1C[i];
		box.x = (box.w * i) - 0xa0;
		box.y = -0x78;
		GsSortBoxFill(&box, ACTIVE_ORDERING_TABLE, 5);
	}

	GsSetProjection(0x15e);
	GsWSMATRIX = *(MATRIX *)STD_D_8007AA10;
	GsClearOt(0, 4, &STD_D_8007B714[ACTIVE_FRAMEBUFFER]);

	for (i = 4; i < 8; i++) {
		GsGetLws(STD_D_8007F528[i].obj.coord2, &lw, &ls);
		GsSetLightMatrix(&lw);
		GsSetLsMatrix(&ls);
		GsSortObject4(&STD_D_8007F528[i].obj, &STD_D_8007B714[ACTIVE_FRAMEBUFFER], 9, getScratchAddr(0));
	}

	GsSortOt(&STD_D_8007B714[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

void STD_func_8006CCD4(void)
{
	removeObject(0x19d, 0);
}

void STD_func_8006CCE0(void)
{
	PositionData *p;
	int32_t i;

	STD_D_8007F528[0].location.vx = -0xfa;
	STD_D_8007F528[0].location.vy = 0;
	STD_D_8007F528[0].location.vz = -0x30c;
	STD_D_8007F528[1].location.vx = -0x32;
	STD_D_8007F528[1].location.vy = 0;
	STD_D_8007F528[1].location.vz = -0x30c;
	STD_D_8007F528[2].location.vx = 0x64;
	STD_D_8007F528[2].location.vy = 0;
	STD_D_8007F528[2].location.vz = -0x30c;
	STD_D_8007F528[3].location.vx = 0x104;
	STD_D_8007F528[3].location.vy = 0;
	MAIN_D_801351A4 = 0;
	STD_D_8007F528[3].location.vz = -0x30c;
	for (i = 0; i < 4; i++) {
		p = &STD_D_8007F528[i];
		STD_D_8007FA08[i] = 0;
		STD_D_8007F528[i].scale.vx = 0x1000;
		STD_D_8007F528[i].scale.vy = 0x1000;
		STD_D_8007F528[i].scale.vz = 0x1000;
		STD_D_8007F528[i].rotation.vx = 0;
		STD_D_8007F528[i].rotation.vy = 0;
		STD_D_8007F528[i].rotation.vz = 0;
		setupModelMatrix(p);
	}
	addObject(0x19d, 0, STD_func_8006CE68, STD_func_8006D018);
}
void STD_func_8006CE68(void)
{
	int32_t i;
	int32_t threshold;

	if (MAIN_D_801351A4 < 0xa0) {
		MAIN_D_801351A4++;
	}

	for (i = 0, threshold = 0x3c; i < 4; i++, threshold += 6) {
		if (MAIN_D_801351A4 > threshold) {
			if (STD_D_8007F528[i].location.vz < 0xdc) {
				STD_D_8007F528[i].location.vz += 0x32;
			}
			if (STD_D_8007FA08[i] < 0x14) {
				STD_D_8007FA08[i]++;
			}
			STD_D_8007F528[i].rotation.vx = STD_D_8007CC78[STD_D_8007FA08[i]];
			STD_D_8007F528[i].rotation.vy = STD_D_8007CCA4[STD_D_8007FA08[i]];
			if (i == 0) {
				STD_D_8007F528[i].location.vx = STD_D_8007CCD0[STD_D_8007FA08[i]];
			}
			if (i == 3) {
				STD_D_8007F528[3].location.vx = STD_D_8007CCFC[STD_D_8007FA08[3]];
			}
		}
		setupModelMatrix(&STD_D_8007F528[i]);
	}
}

void STD_func_8006D018(void)
{
	MATRIX lw;
	MATRIX ls;
	int32_t i;

	GsSetProjection(0x15e);
	GsWSMATRIX = *(MATRIX *)STD_D_8007AA10;
	GsClearOt(0, 4, &STD_D_8007B714[ACTIVE_FRAMEBUFFER]);

	for (i = 3; i >= 0; i--) {
		GsGetLws(STD_D_8007F528[i].obj.coord2, &lw, &ls);
		GsSetLightMatrix(&lw);
		GsSetLsMatrix(&ls);
		GsSortObject4(&STD_D_8007F528[i].obj, &STD_D_8007B714[ACTIVE_FRAMEBUFFER], 9, getScratchAddr(0));
	}

	GsSortOt(&STD_D_8007B714[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

void STD_func_8006D15C(void)
{
	removeObject(0x19d, 0);
}

char *STD_initializeParticleEmitters(char *base)
{
	int32_t i;
	int32_t off;

	MAIN_D_80134CCC = base;
	for (i = 0, off = 0; i < 4; i++) {
		*(int32_t *)(MAIN_D_80134CCC + off) = 0;
		off += 0x1b4;
	}

	return base + 0x23c4;
}

void STD_tickEFEEngine(void)
{
	int32_t i;
	int32_t j;
	int32_t n;

	int32_t (*p)[2];
	int32_t v;
	int32_t t;

	setMapLayerEnabled(1);

	MAIN_D_80134CD4 = 0;
	while (MAIN_D_80134CD4 < 0x10) {
		if (MAIN_D_80139AB0[MAIN_D_80134CD4] == 0) {
			MAIN_D_80139AB0[MAIN_D_80134CD4] = 1;
		}
		MAIN_D_80134CD4++;
	}

	MAIN_D_80134CD4 = 0;
	MAIN_D_80134D0C = (int32_t *)MAIN_D_80134D10;
	while (MAIN_D_80134CD4 < 0x10) {
		t = MAIN_D_80134D0C[2];
		if (t != 0) {
			MAIN_D_80134D08 = t;
			n = ((int16_t *)MAIN_D_80134D0C)[10];
			MAIN_D_80134CD0 = 0;
			while (MAIN_D_80134CD0 < n) {
				if (MAIN_D_80134D0C == NULL) {
				}
				MAIN_D_80134CE8 = (EfeSubEffect *)((int32_t)((uint32_t)(int32_t)((uint32_t)(char *)(MAIN_D_80134CD0 * 28)) + (int32_t)MAIN_D_80134D0C[4]));
				if ((int32_t)MAIN_D_80134CE8->inst != 0) {
					EFE_INSTANCE = MAIN_D_80134CE8->instance;
					EFE_PARENT_INSTANCE = (int32_t)MAIN_D_80134CE8->parentInstance;
					EFE_INSTANCE->frame = EFE_INSTANCE->frame + 1;
					STD_runEFEScript((int32_t)MAIN_D_80134CE8->inst);
				}
				MAIN_D_80134CD0++;
			}
			n = MAIN_D_80134D0C[7];
			for (j = 0; j < n; j++) {
				STD_tickEFEUVAnimation(j);
			}
		}
		MAIN_D_80134D0C = (int32_t *)((char *)MAIN_D_80134D0C + 0x28);
		MAIN_D_80134CD4++;
	}

	STD_tickParticleEmitters();

	p = MAIN_D_80139AD0;
	for (i = 0; i < 0xa; i++) {
		v = (*p)[0];
		if ((v >= 0) && (**(int32_t **)((char *)(*p)[1] + 8) < 0)) {
			stopSoundMask(v);
			(*p)[0] = -1;
		}
		p++;
	}
}

void STD_renderEFEEngine(void)
{
	int32_t i;
	int32_t j;
	int32_t n;
	int32_t off;
	int32_t off2;
	int32_t t;

	MAIN_D_80134D0C = (int32_t *)MAIN_D_80134D10;
	for (i = 0; i < 0x10; i++) {
		n = MAIN_D_80134D0C[7];
		for (j = 0, off = 0; j < n; j++, off += 0x20) {
			*(int16_t *)((char *)off + MAIN_D_80134D0C[8] + 2) = 0;
		}
		t = MAIN_D_80134D0C[2];
		if (t != 0) {
			MAIN_D_80134D08 = t;
			n = ((int16_t *)MAIN_D_80134D0C)[10];
			for (j = 0, off2 = 0; j < n; j++, off2 += 0x1c) {
				MAIN_D_80134CE8 = (EfeSubEffect *)((char *)MAIN_D_80134D0C[4] + off2);
				if ((int32_t)MAIN_D_80134CE8->inst != 0) {
					EFE_INSTANCE = MAIN_D_80134CE8->instance;
					EFE_PARENT_INSTANCE = (int32_t)MAIN_D_80134CE8->parentInstance;
					STD_runEFEScript((int32_t)MAIN_D_80134CE8->someInst);
				}
			}
		}
		MAIN_D_80134D0C = (int32_t *)((char *)MAIN_D_80134D0C + 0x28);
	}

	STD_renderParticleEmitters();
}

void STD_clearEFESoundChannels(void)
{
	int32_t i;

	for (i = 0; i < 0xa; i++) {
		MAIN_D_80139AD0[i][0] = -1;
	}
}

void STD_stopEFESounds(void)
{
	int32_t i;
	int32_t *p;
	int32_t v;

	p = &MAIN_D_80139AD0[0][0];
	for (i = 0; i < 10; i++) {
		v = *p;
		if (v >= 0) {
			stopSoundMask(v);
			*p = -1;
		}
	}
}

void STD_unloadEFESlot(int32_t idx)
{
	int32_t i;
	int32_t n;
	int16_t *p;
	int32_t off;

	*(char **)&MAIN_D_80134D0C = MAIN_D_80134D10 + (idx * 40);
	n = MAIN_D_80134D0C[7];
	for (i = 0, off = 0; i < n; i++, off += 0x20) {
		p = (int16_t *)((char *)MAIN_D_80134D0C[8] + off);
		if (p[0] != 0) {
			p[0] = 0;
			p[6] = 0;
		}
	}

	unloadModel(MAIN_D_80134D0C[0], 1);
	MAIN_D_80134D0C[2] = 0;
}

void STD_runEFESlotScript(int32_t idx)
{
	EFE_INSTANCE = NULL;

	*(char **)&MAIN_D_80134D0C = MAIN_D_80134D10 + (idx * 40);
	MAIN_D_80134D08 = MAIN_D_80134D0C[2];
	MAIN_D_80134CE8 = (EfeSubEffect *)MAIN_D_80134D0C[4];
	MAIN_D_80134CE4 = -1;
	STD_runEFEScript(((int32_t *)(MAIN_D_80134D10 + (idx * 40)))[3]);
}

INCLUDE_ASM("asm/std/nonmatchings/std_effect", STD_setupLoadedEFEFile);

void STD_handleEFEFileLoaded(int32_t arg)
{
	MAIN_D_801351B8 = 0;
	setFileReadCallback2(STD_setupLoadedEFEFile, arg);
}

INCLUDE_ASM("asm/std/nonmatchings/std_effect", STD_tickEFEUVAnimation);

void STD_tickParticleEmitters(void)
{
	SVECTOR rot;
	MATRIX m;
	char *p;
	char *owner;
	int32_t i;
	int16_t *q;
	int32_t k;
	int32_t t;

	p = MAIN_D_80134CCC;
	for (i = 0; i < 4; p = (char *)((int32_t)((uint32_t)p + 0x1b4)), i++) {
		if (*(char **)p == NULL) {
			continue;
		}
		owner = *(char **)p;
		if (*(int32_t *)owner == -1) {
			*(int32_t *)p = 0;
			continue;
		}

		k = 0;
		while (1) {
			if (((int16_t (*)[10])p)[k][8] <= 0) {
				break;
			}
			k++;
		}

		if (k != 0x14) {
			q = &((int16_t (*)[10])p)[k][8];
			q[0] = ((int16_t *)p)[3];
			q[1] = ((int16_t *)p)[4];
			rot.vx = (((rand() & 0x7f) - 0x40) << 12) / 64;
			rot.vy = (((rand() & 0x7f) - 0x40) << 12) / 64;
			rot.vz = (((rand() & 0x7f) - 0x40) << 12) / 64;
			q[3] = 0;
			q[2] = 0;
			q[4] = ((int16_t *)p)[3];
			RotMatrixZYX(&rot, &m);
			ApplyMatrixSV(&m, (SVECTOR *)&q[2], (SVECTOR *)&q[2]);
			q[6] = *(int32_t *)((int32_t)((uint32_t)(char *)owner + 4));
			q[7] = *(int32_t *)((int32_t)((uint32_t)(char *)owner + 8));
			q[8] = *(int32_t *)((int32_t)((uint32_t)(char *)owner + 0xc));
		}

		for (k = 0; k < 0x14; k++) {
			if (((int16_t (*)[10])p)[k][8] > 0) {
				q = &((int16_t (*)[10])p)[k][8];
				q[0] = q[0] - q[1];
				q[1] -= ((int16_t *)p)[5];
			}
		}

		t = ((int16_t *)p)[2] - 1;
		((int16_t *)p)[2] = t;
		if ((int16_t)t <= 0) {
			*(int32_t *)p = 0;
		}
	}
}

INCLUDE_ASM("asm/std/nonmatchings/std_effect", STD_renderParticleEmitters);

int16_t STD_offsetEFEPrimitiveUVs(char *base, int32_t idx, int32_t du, int32_t dv)
{
	uint8_t code;
	char *p;
	int32_t i;
	int32_t n;

	base = (char *)((int32_t)base + 0xc + (idx * 28));
	n = ((int32_t *)base)[5];
	p = ((char **)base)[4];
	for (i = 0; i < n; i++) {
		code = (*(int32_t *)p >> 24) & 0xff;
		if (code & 1) {
			goto setpath;
		}
		if (code & 4) {
			p[4] = p[4] + du;
			p[5] = p[5] + dv;
			p[8] = p[8] + du;
			p[9] = p[9] + dv;
			p[0xc] = p[0xc] + du;
			p[0xd] = p[0xd] + dv;
			if (code & 8) {
				p[0x10] = p[0x10] + du;
				p[0x11] = p[0x11] + dv;
				if (code & 0x10) {
					goto s1;
				}
				p += 0x20;
				goto next;
s1:
				p += 0x24;
				goto next;
			} else {
				if (code & 0x10) {
					goto s2;
				}
				p += 0x18;
				goto next;
s2:
				p += 0x1c;
				goto next;
			}
		}
		goto next;
setpath:
		if (code & 4) {
			p[4] = p[4] + du;
			p[5] = p[5] + dv;
			p[8] = p[8] + du;
			p[9] = p[9] + dv;
			p[0xc] = p[0xc] + du;
			p[0xd] = p[0xd] + dv;
			if (code & 8) {
				p[0x10] = p[0x10] + du;
				p[0x11] = p[0x11] + dv;
				if (code & 0x10) {
					goto s3;
				}
				p += 0x20;
				goto next;
s3:
				p += 0x2c;
				goto next;
			} else {
				if (code & 0x10) {
					goto s4;
				}
				p += 0x1c;
				goto next;
s4:
				p += 0x24;
				goto next;
			}
		}
next:;
	}
}

char *STD_initializeEFEEngine(char *base)
{
	int32_t i;
	int32_t off;

	base = (char *)((int32_t)base + (4 - ((int32_t)base & 3)));
	MAIN_D_80134D10 = base;
	for (i = 0, off = 0; i < 0x10; i++, off += 0x28) {
		*(int32_t *)((char *)off + (int32_t)MAIN_D_80134D10 + 8) = 0;
		*(int32_t *)((char *)off + (int32_t)MAIN_D_80134D10 + 0x1c) = 0;
	}

	base = (char *)((int32_t)base + 0x280);
	base = (char *)((int32_t)base + (4 - ((int32_t)base & 3)));
	base = STD_initializeParticleEmitters(base);
	base = (char *)((int32_t)base + (4 - ((int32_t)base & 3)));
	base = initializeFlashData(base);
	base = STD_initializeAuraProjectiles(base);
	base = (char *)((int32_t)base + (4 - ((int32_t)base & 3)));
	MAIN_D_80134D18 = (int32_t)base;
	MAIN_D_80134D14 = (int32_t)base;
	addObject(0x500, 0, STD_tickEFEEngine, STD_renderEFEEngine);
	STD_initializeEFESubOpcodeTable();
	base = (char *)((int32_t)base + 0x41000);
	STD_clearEFESoundChannels();

	return base;
}

void STD_removeEFEEngine(void)
{
	STD_func_80079874();
	STD_stopEFESounds();
	removeObject(0x500, 0);
}

void STD_loadMoveEFE(int16_t *moves, int16_t *effectIds, int8_t *isLoaded)
{
	downloadSomeImage(moves);
	MAIN_D_80139B20[0] = -1;
	MAIN_D_80139B24[0] = isLoaded;
	*MAIN_D_80139B24[0] = 1;
	MAIN_D_80139B28[0] = (int32_t)moves;
	MAIN_D_80139B2C[0] = (int32_t)effectIds;
	STD_loadNextEFEFile(MAIN_D_80139B20);
}

void STD_unloadAllEFESlots(void)
{
	int32_t i;

	for (i = 0; i < 0x10; i++) {
		if (EFE_LOADED_MOVE_DATA[i] != -1) {
			STD_unloadEFESlot(i);
			EFE_LOADED_MOVE_DATA[i] = -1;
		}
	}

	MAIN_D_80134D14 = MAIN_D_80134D18;
}

int32_t STD_startEFE(int32_t i)
{
	if ((i < 0) || (i >= 0x10)) {
		return -1;
	}

	if (EFE_LOADED_MOVE_DATA[i] < 0) {
		return -1;
	}

	MAIN_D_80139AB0[i] = -1;
	STD_runEFESlotScript(i);
}

void STD_stopEFESubEffect(int32_t a, int32_t b)
{
	char *p;
	char *q;

	if ((b < 0) || (a < 0) || (a >= 0x10)) {
		return;
	}

	if (MAIN_D_80134D10 != NULL) {
	}

	p = (char *)(a * 40) + (int32_t)MAIN_D_80134D10;
	q = ((char **)p)[4];
	MAIN_D_80134CE8 = (EfeSubEffect *)(q + (b * 28));
	MAIN_D_80134CE8->inst = NULL;
	MAIN_D_80134CE8->instance->frame = -1;
}

int32_t STD_func_8006EA6C(void)
{
	return MAIN_D_80134D14;
}

void STD_loadNextEFEFile(int16_t *arg)
{
	EfeLoad *p;
	ModelComponent *m;
	int32_t id;
	int32_t i;
	char path[32];
	CdlLOC loc;

	p = (EfeLoad *)(int32_t)arg;
	p->state++;
	id = *p->moves++;
	if (id < 0) {
		*p->isLoaded = 0;
		return;
	}

	id -= 0x100;
	if ((id < 0) || (id >= 0x17a)) {
		*p->isLoaded = -4;
		return;
	}

	for (m = UNKNOWN_MODEL, i = 0; i < 16; m++, i++) {
		if (m->useCount == 0) {
			break;
		}
	}

	if (i == 16) {
		*p->isLoaded = -5;
		return;
	}

	m->useCount = id + 0x100;
	m->mmdPtr = (void *)STD_func_8006EA6C();
	p->model = m;
	loc = *getEFEDATEntry(id + 0x100);
	addFileReadRequest(path, (uint8_t *)m->mmdPtr, NULL, (void *)STD_handleEFEFileLoaded, arg, &loc, 0x5000);
}

char *STD_func_8006EA78(char *p)
{
	int32_t a;
	int32_t b;
	char *base;

	base = p;
	p = (char *)((uint32_t)p + 0x34);
	a = ((int32_t *)base)[7];
	b = ((int32_t *)base)[9];
	if ((b - a) == 0) {
		return NULL;
	}

	return p + ((int32_t *)base)[7];
}

char *STD_func_8006EAAC(char *p)
{
	int32_t a;
	int32_t b;
	char *base;

	base = p;
	p = (char *)((uint32_t)p + 0x34);
	a = ((int32_t *)base)[5];
	b = ((int32_t *)base)[6];
	if ((b - a) == 0) {
		return NULL;
	}

	return p + ((int32_t *)base)[5];
}

int32_t STD_getEFEFileId(int32_t *p)
{
	return p[12];
}

void STD_isTargetUnhit(void)
{
	int32_t *out;

	out = EFE_POP1(int32_t *);
	if (((int8_t *)(int32_t)MAIN_D_80134CE8->targetEntity)[0x53] == 0) {
		*out = 1;
	} else {
		*out = 0;
	}
}

void STD_markEFEFinished(void)
{
	MAIN_D_80139AB0[MAIN_D_80134CD4] = 0;
}

void STD_func_8006EB58(void)
{
	int32_t *out;

	out = EFE_POP1(int32_t *);
	*out = VIEWPORT_DISTANCE;
}

void STD_disableMapLayer(void)
{
	setMapLayerEnabled(0);
}

void STD_renderScreenFade(void)
{
	modifySomeImage(EFE_POP1(int32_t));
}

void STD_applyBoxAttackHit(void)
{
	SVECTOR center;
	AABB box;
	int32_t r;
	int32_t *out;
	int32_t *ext;
	int32_t id;
	int32_t idx;
	int32_t j;
	Entity *e;
	int32_t ent;

	r = EFE_POP1(int32_t);
	out = EFE_POP1(int32_t *);
	ext = EFE_POP1(int32_t *);
	*out = 0;
	if (r < 0) {
		r = MAIN_D_80134CD0;
	}

	center.vx = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[0];
	center.vy = *(int32_t *)((int32_t)EFE_INSTANCE + 8);
	center.vz = *(int32_t *)((int32_t)EFE_INSTANCE + 0xc);
	box.center = &center;
	box.extent.vx = ext[0];
	box.extent.vy = ext[1];
	box.extent.vz = ext[2];
	MAIN_D_80134CD8 = 1;
	while (1) {
		id = findAABBHitEntity(&box, MAIN_D_80134CE8->sourceEntity, MAIN_D_80134CD8);
		MAIN_D_80134CD8 = id;
		if (id == -1) {
			return;
		}
		idx = (*(int32_t *)&MAIN_D_80134CD8);
		e = ENTITY_TABLE[idx];
		if (((int8_t *)e)[0x53] == 0) {
			for (j = 1; j < 10; j++) {
				ent = (int32_t)ENTITY_TABLE[j];
				if (ent == (int32_t)MAIN_D_80134CE8->sourceEntity) {
					break;
				}
			}
			((int8_t *)ENTITY_TABLE[idx])[0x53] = 1;

			addAttackObject(MAIN_D_80134CD8, 1, (int16_t *)&center, (int32_t)((uint32_t)(char *)MAIN_D_80134CD4), r, j);
			*out = 1;
			return;
		}
		MAIN_D_80134CD8 = idx + 1;
	}
}

INCLUDE_ASM("asm/std/nonmatchings/std_effect", STD_applyRadiusAttackHit);

void STD_applyLineAttackHit(void)
{
	DVECTOR line[2];
	int16_t rect[4];
	SVECTOR pos;
	int32_t *out;
	int32_t *arg;
	int32_t j;
	int32_t r;
	Entity *e;
	int32_t ent;

	EFE_DROP1();
	out = EFE_POP1(int32_t *);
	arg = EFE_POP1(int32_t *);
	*out = 0;
	line[0].vx = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[0];
	line[0].vy = *(int32_t *)((int32_t)EFE_INSTANCE + 0xc);
	line[1].vx = *(int32_t *)((char *)((int32_t *)(int32_t)MAIN_D_80134CE8->sourceEntity)[1] + 0x78);
	line[1].vy = *(int32_t *)((char *)((int32_t *)(int32_t)MAIN_D_80134CE8->sourceEntity)[1] + 0x80);
	for (MAIN_D_80134CD8 = 1; MAIN_D_80134CD8 < 10; MAIN_D_80134CD8++) {
		e = ENTITY_TABLE[(*(int32_t *)&MAIN_D_80134CD8)];
		if (e == MAIN_D_80134CE8->sourceEntity) {
			continue;
		}
		if (e == NULL) {
			continue;
		}
		if (((int8_t *)e)[0x53] != 0) {
			continue;
		}
		if (((int8_t *)e)[0x34] == 0) {
			continue;
		}
		r = DIGIMON_DATA[*(int32_t *)e].radius;
		rect[0] = *(int32_t *)((char *)((int32_t *)e)[1] + 0x78) - r;
		rect[2] = *(int32_t *)((char *)((int32_t *)e)[1] + 0x78) + r;
		rect[1] = *(int32_t *)((char *)((int32_t *)e)[1] + 0x80) - r;
		rect[3] = *(int32_t *)((char *)((int32_t *)e)[1] + 0x80) + r;
		if (MAIN_func_800DA740(rect, line) != -1) {
			continue;
		}
		if (*(int32_t *)((int32_t)EFE_INSTANCE + 8) < (-DIGIMON_DATA[*(int32_t *)e].height - arg[1])) {
			continue;
		}
		for (j = 1; j < 10; j++) {
			ent = (int32_t)ENTITY_TABLE[j];
			if (ent == (int32_t)MAIN_D_80134CE8->sourceEntity) {
				break;
			}
		}
		((int8_t *)ENTITY_TABLE[MAIN_D_80134CD8])[0x53] = 1;
		STD_calculateAttackHitPosition(&pos, (int32_t *)e, ((int32_t **)MAIN_D_80134CE8)[4], DIGIMON_DATA[*(int32_t *)e].radius);
		pos.vy = *(int32_t *)((int32_t)EFE_INSTANCE + 8);
		addAttackObject(MAIN_D_80134CD8, 1, (int16_t *)&pos, (int32_t)((uint32_t)(char *)MAIN_D_80134CD4), MAIN_D_80134CD0, j);
		*out = 1;
		return;
	}
}

void STD_faceTargetEntity(void)
{
	PositionData *pd;
	Entity *ent;

	if (MAIN_D_80134CE8->targetEntity == NULL) {
		return;
	}

	ent = MAIN_D_80134CE8->targetEntity;
	entityLookAtLocation(MAIN_D_80134CE8->sourceEntity, &ent->posData->location);
	pd = MAIN_D_80134CE8->sourceEntity->posData;
	RotMatrix(&pd->rotation, &pd->posMatrix.coord);
	ScaleMatrix(&pd->posMatrix.coord, &pd->scale);
	TransMatrix(&pd->posMatrix.coord, &pd->location);
	pd->posMatrix.flg = 0;
}

void STD_renderScreenOverlay(void)
{
	POLY_F4 *p;
	DR_TPAGE *dr;
	int32_t *col;
	int32_t mode;
	int32_t layer;

	mode = EFE_POP1(int32_t);
	col = EFE_POP1(int32_t *);
	layer = EFE_POP1(int32_t);

	p = (POLY_F4 *)GsGetWorkBase();
	setXY4(p, -0xa0, -0x78, 0xa0, -0x78, -0xa0, 0x78, 0xa0, 0x78);
	setRGB0(p, col[0], col[1], col[2]);
	setPolyF4(p);
	setSemiTrans(p, mode >> 2);
	setShadeTex(p, 0);
	addPrim(ACTIVE_ORDERING_TABLE->org + layer, p);
	GsSetWorkBase((PACKET *)(p + 1));

	dr = (DR_TPAGE *)GsGetWorkBase();
	setDrawTPage(dr, 1, 1, getTPage(0, mode & 3, 0, 0));
	addPrim(ACTIVE_ORDERING_TABLE->org + layer, dr);
	GsSetWorkBase((PACKET *)(dr + 1));
}

INCLUDE_ASM("asm/std/nonmatchings/std_effect", STD_renderRingTube);

INCLUDE_ASM("asm/std/nonmatchings/std_effect", STD_renderRibbonStrip);

INCLUDE_ASM("asm/std/nonmatchings/std_effect", STD_tickRibbonPoints);

void STD_initializeRibbonPoints(void)
{
	SVECTOR *q;
	int32_t i;
	int32_t v;

	q = EFE_POP1(SVECTOR *);
	for (i = 0, v = -0x708; i < 10; i++, v += 0x190) {
		q[i].vx = v;
		q[i].vy = customRandom(-0x96, 0x96);
		q[i].pad = customRandom(-0x1e, 0x1e);
		q[i].vz = 0;
	}
}

INCLUDE_ASM("asm/std/nonmatchings/std_effect", STD_renderRadialWaves);

void STD_func_80070A40(void)
{
	int32_t *out;

	out = EFE_POP1(int32_t *);
	*out = VIEWPORT_DISTANCE;
}

void STD_addClutLoadPrim(void)
{
	RECT rect;
	int32_t idx;
	char *src;
	int32_t y;
	int32_t z;
	PACKET *prim;

	idx = EFE_POP1(int32_t);
	src = EFE_POP1(char *);
	y = EFE_POP1(int32_t);
	z = EFE_POP1(int32_t);
	src += idx * 2;
	prim = GsGetWorkBase();
	GsSetWorkBase(prim + 0x44);
	rect.x = (((uint16_t *)MAIN_D_80134D0C[6])[9] & 0x3f) << 4;
	rect.y = ((((uint16_t *)MAIN_D_80134D0C[6])[9] >> 6) & 0x1ff) + y;
	rect.w = 0x10;
	rect.h = 1;
	MAIN_func_80092C18(prim, &rect);
	memcpy(prim + 0x10, src, 0x20);
	AddPrim(ACTIVE_ORDERING_TABLE->org + (z >> 4), prim);
}

void STD_drawTMDScreenSpace(void)
{
	EFE_SCRATCH->scale = EFE_POP1(VECTOR *);
	EFE_SCRATCH->id = EFE_POP1(int32_t);
	EFE_SCRATCH->rot.vx = ((VECTOR *)((int32_t)EFE_INSTANCE + 0x10))->vx;
	EFE_SCRATCH->rot.vy = ((VECTOR *)((int32_t)EFE_INSTANCE + 0x10))->vy;
	EFE_SCRATCH->rot.vz = ((VECTOR *)((int32_t)EFE_INSTANCE + 0x10))->vz;
	RotMatrixYXZ(&EFE_SCRATCH->rot, &EFE_SCRATCH->m0);
	ScaleMatrix(&EFE_SCRATCH->m0, EFE_SCRATCH->scale);
	EFE_SCRATCH->m0.t[0] = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[0];
	EFE_SCRATCH->m0.t[1] = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[1];
	EFE_SCRATCH->m0.t[2] = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[2];
	EFE_SCRATCH->m0.t[0] += -DRAWING_OFFSET_X + 0xa0;
	EFE_SCRATCH->m0.t[1] += -DRAWING_OFFSET_Y + 0x78;
	TransposeMatrix(&GsWSMATRIX, &EFE_SCRATCH->m2);
	MulMatrix0(&EFE_SCRATCH->m2, &EFE_SCRATCH->m0, &EFE_SCRATCH->m1);
	GsSetLightMatrix(&EFE_SCRATCH->m1);
	GsSetLsMatrix(&EFE_SCRATCH->m0);
	GsLinkObject4((unsigned long)(((char **)MAIN_D_80134D0C[6])[1] + 0xc), &EFE_SCRATCH->obj, EFE_SCRATCH->id);
	EFE_SCRATCH->obj.coord2 = NULL;
	EFE_SCRATCH->obj.attribute = 0;
	GsSortObject4(&EFE_SCRATCH->obj, ACTIVE_ORDERING_TABLE, 2, getScratchAddr(0x2c));
}

void STD_loadClutColors(void)
{
	uint16_t clut[16];
	RECT rect;
	int32_t y;
	int32_t x;
	int32_t count;
	uint16_t *src;
	ModelComponent *m;
	int32_t i;

	src = EFE_POP1(uint16_t *);
	count = EFE_POP1(int32_t);
	x = EFE_POP1(int32_t);
	y = EFE_POP1(int32_t);

	if ((count > 0) && (count <= 16)) {
		m = (ModelComponent *)MAIN_D_80134D0C[6];
		for (i = 0; i < count; i++) {
			clut[i] = *src++;
			clut[i] += *src++ << 5;
			clut[i] += *src++ << 10;
			clut[i] += *src++ << 15;
		}
		rect.x = ((m->clutPage & 0x3f) << 4) + x;
		rect.y = (m->clutPage >> 6) + y;
		rect.w = count;
		rect.h = 1;
		LoadImage(&rect, (u_long *)clut);
		DrawSync(0);
	}
}

void STD_func_80070EA0(void)
{
	typedef struct {
		int32_t rotation[3];
	} EfeRot;

	EFE_SCRATCH->scale = EFE_POP1(VECTOR *);
	EFE_SCRATCH->id = EFE_POP1(int32_t);
	EFE_SCRATCH->rot.vx = ((EfeRot *)((int32_t)EFE_INSTANCE + 0x10))->rotation[0];
	EFE_SCRATCH->rot.vy = ((EfeRot *)((int32_t)EFE_INSTANCE + 0x10))->rotation[1];
	EFE_SCRATCH->rot.vz = ((EfeRot *)((int32_t)EFE_INSTANCE + 0x10))->rotation[2];
	RotMatrixYXZ(&EFE_SCRATCH->rot, &EFE_SCRATCH->m1);
	ScaleMatrix(&EFE_SCRATCH->m1, EFE_SCRATCH->scale);
	EFE_SCRATCH->m1.t[0] = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[0];
	EFE_SCRATCH->m1.t[1] = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[1];
	EFE_SCRATCH->m1.t[2] = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[2];
	GsMulCoord0(&GsWSMATRIX, &EFE_SCRATCH->m1, &EFE_SCRATCH->m0);
	if (EFE_SCRATCH->m0.t[2] < -0x12c) {
		return;
	}

	if (EFE_SCRATCH->m0.t[2] >= 0x10000) {
		return;
	}

	GsSetLightMatrix(&EFE_SCRATCH->m1);
	GsSetLsMatrix(&EFE_SCRATCH->m0);
	GsLinkObject4((unsigned long)(((char **)MAIN_D_80134D0C[6])[1] + 0xc), &EFE_SCRATCH->obj, EFE_SCRATCH->id);
	EFE_SCRATCH->obj.coord2 = NULL;
	EFE_SCRATCH->obj.attribute = 0;
	GsSortObject4(&EFE_SCRATCH->obj, ACTIVE_ORDERING_TABLE, 2, getScratchAddr(0x2c));
}

void STD_getCameraRotation(void)
{
	MATRIX m;
	SVECTOR rot;
	int32_t *out;

	out = EFE_POP1(int32_t *);
	TransposeMatrix(&GsWSMATRIX, &m);
	matrixToEuler2(&m, &rot);
	out[0] = rot.vx;
	out[1] = rot.vy;
	out[2] = rot.vz;
}

INCLUDE_ASM("asm/std/nonmatchings/std_effect", STD_selectRandomTargetEntity);

void STD_convertToViewSpace(void)
{
	MATRIX m1;
	MATRIX m2;
	SVECTOR rot;
	VECTOR *rotIn;
	VECTOR *trans;
	VECTOR *out2;
	VECTOR *out1;

	rotIn = EFE_POP1(VECTOR *);
	trans = EFE_POP1(VECTOR *);
	out2 = EFE_POP1(VECTOR *);
	out1 = EFE_POP1(VECTOR *);
	rot.vx = rotIn->vx;
	rot.vy = rotIn->vy;
	rot.vz = rotIn->vz;
	((VECTOR *)m1.t)->vx = trans->vx;
	((VECTOR *)m1.t)->vy = trans->vy;
	((VECTOR *)m1.t)->vz = trans->vz;
	RotMatrixYXZ(&rot, &m1);
	GsMulCoord0(&GsWSMATRIX, &m1, &m2);
	matrixToEuler2(&m2, &rot);
	out1->vx = ((VECTOR *)m2.t)->vx;
	out1->vy = ((VECTOR *)m2.t)->vy;
	out1->vz = ((VECTOR *)m2.t)->vz;
	out2->vx = rot.vx;
	out2->vy = rot.vy;
	out2->vz = rot.vz;
}

void STD_maskVectorByScalar(void)
{
	int32_t mask;
	int32_t *v;

	mask = EFE_POP1(int32_t);
	v = EFE_POP1(int32_t *);
	v[0] &= mask;
	v[1] &= mask;
	v[2] &= mask;
}

void STD_divideVectorByScalar(void)
{
	int32_t k;
	int32_t *v;

	k = EFE_POP1(int32_t);
	v = EFE_POP1(int32_t *);
	v[0] /= k;
	v[1] /= k;
	v[2] /= k;
}

void STD_multiplyVectorByScalar(void)
{
	int32_t k;
	int32_t *v;

	k = EFE_POP1(int32_t);
	v = EFE_POP1(int32_t *);
	v[0] *= k;
	v[1] *= k;
	v[2] *= k;
}

void STD_render3DTexturedQuad(void)
{
	SVECTOR a;
	SVECTOR b;
	SVECTOR c;
	SVECTOR d;
	POLY_FT4 *prim;
	ModelComponent *m;
	int32_t u0off;
	int32_t v0off;
	int32_t *col;
	int32_t semi;
	int32_t du;
	int32_t dv;
	int32_t clutY;
	int32_t *p1;
	int32_t *p2;
	int32_t *p3;
	int32_t *p4;

	semi = EFE_POP1(int32_t);
	clutY = EFE_POP1(int32_t);
	dv = EFE_POP1(int32_t);
	du = EFE_POP1(int32_t);
	v0off = EFE_POP1(int32_t);
	u0off = EFE_POP1(int32_t);
	col = EFE_POP1(int32_t *);
	p4 = EFE_POP1(int32_t *);
	p3 = EFE_POP1(int32_t *);
	p2 = EFE_POP1(int32_t *);
	p1 = EFE_POP1(int32_t *);
	m = (ModelComponent *)MAIN_D_80134D0C[6];

	a.vx = p1[0];
	a.vy = p1[1];
	a.vz = p1[2];
	b.vx = p2[0];
	b.vy = p2[1];
	b.vz = p2[2];
	c.vx = p3[0];
	c.vy = p3[1];
	c.vz = p3[2];
	d.vx = p4[0];
	d.vy = p4[1];
	d.vz = p4[2];

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	if (semi != 0) {
		setSemiTrans(prim, 1);
	}

	prim->r0 = col[0];
	prim->g0 = col[1];
	prim->b0 = col[2];
	prim->tpage = m->pixelPage | semi;
	prim->clut = GetClut((m->clutPage & 0x3f) << 4, (m->clutPage >> 6) + clutY);
	prim->u0 = m->pixelOffsetX + u0off;
	prim->v0 = m->pixelOffsetY + v0off;
	prim->u1 = du + (m->pixelOffsetX + u0off);
	prim->v1 = m->pixelOffsetY + v0off;
	prim->u2 = m->pixelOffsetX + u0off;
	prim->v2 = dv + (m->pixelOffsetY + v0off);
	prim->u3 = du + (m->pixelOffsetX + u0off);
	prim->v3 = dv + (m->pixelOffsetY + v0off);
	add3DSpritePrim(prim, &a, &b, &c, &d);
}

void STD_setTransformToBoneMatrix(void)
{
	MATRIX m;
	SVECTOR v;
	SVECTOR out;
	int32_t *p;
	int32_t *q;

	p = (int32_t *)((int32_t)EFE_INSTANCE + 4);
	calculateBoneMatrix(MAIN_D_80134CE8->sourceEntity, MAIN_D_80134CE8->boneOffset->boneId, &m);
	v.vx = MAIN_D_80134CE8->boneOffset->positionX;
	v.vy = MAIN_D_80134CE8->boneOffset->positionY;
	v.vz = MAIN_D_80134CE8->boneOffset->positionZ;
	ApplyMatrixSV(&m, &v, &out);
	p[0] = out.vx;
	p[1] = out.vy;
	p[2] = out.vz;
	q = p++;
	*q = *q + m.t[0];
	q = p++;
	*q = *q + m.t[1];
	q = p++;
	*q = *q + m.t[2];
	matrixToEuler2(&m, &out);
	*p++ = out.vx;
	*p++ = out.vy;
	*p = out.vz;
}

INCLUDE_ASM("asm/std/nonmatchings/std_effect", STD_renderWireframeBox);

void STD_discardEFEOperand(void)
{
	EFE_DROP1();
}

void STD_renderWireframeGrid(void)
{
	SVECTOR c0;
	SVECTOR c1;
	SVECTOR c2;
	SVECTOR p0;
	SVECTOR p1;
	SVECTOR p2;
	SVECTOR p3;
	MATRIX m;
	SVECTOR rot;
	int32_t *base;
	int32_t *rotSrc;
	int32_t y;
	int32_t *col;
	int32_t x1;
	int32_t x2;
	int32_t z1;
	int32_t z2;
	int32_t n1;
	int32_t n2;

	y = EFE_POP1(int32_t);
	n2 = EFE_POP1(int32_t);
	z2 = EFE_POP1(int32_t);
	z1 = EFE_POP1(int32_t);
	n1 = EFE_POP1(int32_t);
	x2 = EFE_POP1(int32_t);
	x1 = EFE_POP1(int32_t);
	col = EFE_POP1(int32_t *);
	rotSrc = EFE_POP1(int32_t *);
	base = EFE_POP1(int32_t *);

	PushMatrix();
	rot.vx = rotSrc[0];
	rot.vy = rotSrc[1];
	rot.vz = rotSrc[2];
	RotMatrixZYX(&rot, &m);
	p0.vx = x1;
	p0.vy = y;
	p0.vz = z1;
	ApplyMatrixSV(&m, &p0, &p0);
	p1.vx = x2;
	p1.vy = y;
	p1.vz = z1;
	ApplyMatrixSV(&m, &p1, &p1);
	p2.vx = x1;
	p2.vy = y;
	p2.vz = z2;
	ApplyMatrixSV(&m, &p2, &p2);
	p3.vx = x2;
	p3.vy = y;
	p3.vz = z2;
	ApplyMatrixSV(&m, &p3, &p3);
	PopMatrix();

	c0.vx = c2.vx = base[0] + p0.vx;
	c0.vy = c2.vy = base[1] + p0.vy;
	c0.vz = c2.vz = base[2] + p0.vz;
	c1.vx = base[0] + p2.vx;
	c1.vy = base[1] + p2.vy;
	c1.vz = base[2] + p2.vz;
	STD_renderParallelLines(&c0, &c1, (int16_t)n1, &p0, &p1, col);
	c1.vx = base[0] + p1.vx;
	c1.vy = base[1] + p1.vy;
	c1.vz = base[2] + p1.vz;
	STD_renderParallelLines(&c2, &c1, (int16_t)n2, &p0, &p2, col);
}

void STD_render2DTexturedQuad(void)
{
	int32_t depth;
	int32_t *col;
	int32_t du;
	int32_t dv;
	int32_t clutY;
	int32_t a;
	int32_t b;
	POLY_FT4 *prim;
	ModelComponent *m;
	int32_t semi;
	int32_t u0off;
	int32_t v0off;
	int32_t c;
	int32_t d;

	semi = EFE_POP1(int32_t);
	clutY = EFE_POP1(int32_t);
	dv = EFE_POP1(int32_t);
	du = EFE_POP1(int32_t);
	v0off = EFE_POP1(int32_t);
	u0off = EFE_POP1(int32_t);
	col = EFE_POP1(int32_t *);
	depth = EFE_POP1(int32_t);
	d = EFE_POP1(int32_t);
	c = EFE_POP1(int32_t);
	b = EFE_POP1(int32_t);
	a = EFE_POP1(int32_t);

	if ((depth > 0x20) && (depth < 0x1000)) {
		m = (ModelComponent *)MAIN_D_80134D0C[6];
		prim = (POLY_FT4 *)GsGetWorkBase();
		SetPolyFT4(prim);
		if (semi < 0) {
			semi = -semi & 0xffff;
			prim->x0 = ((int32_t *)a)[0];
			prim->y0 = ((int32_t *)a)[1];
			prim->x1 = ((int32_t *)b)[0];
			prim->y1 = ((int32_t *)b)[1];
			prim->x2 = ((int32_t *)c)[0];
			prim->y2 = ((int32_t *)c)[1];
			prim->x3 = ((int32_t *)d)[0];
			prim->y3 = ((int32_t *)d)[1];
		} else {
			prim->x0 = a;
			prim->y0 = b;
			prim->x1 = a + c;
			prim->y1 = b;
			prim->x2 = a;
			prim->y2 = b + d;
			prim->x3 = a + c;
			prim->y3 = b + d;
		}
		if (semi != 0) {
			setSemiTrans(prim, 1);
		}
		prim->r0 = col[0];
		prim->g0 = col[1];
		prim->b0 = col[2];
		prim->tpage = m->pixelPage | semi;
		prim->clut = GetClut((m->clutPage & 0x3f) << 4, (m->clutPage >> 6) + clutY);
		prim->u0 = m->pixelOffsetX + u0off;
		prim->v0 = m->pixelOffsetY + v0off;
		prim->u1 = du + (m->pixelOffsetX + u0off);
		prim->v1 = m->pixelOffsetY + v0off;
		prim->u2 = m->pixelOffsetX + u0off;
		prim->v2 = dv + (m->pixelOffsetY + v0off);
		prim->u3 = du + (m->pixelOffsetX + u0off);
		prim->v3 = dv + (m->pixelOffsetY + v0off);
		AddPrim(ACTIVE_ORDERING_TABLE->org + depth, prim);
		prim++;
		GsSetWorkBase((PACKET *)prim);
	}
}

void STD_restoreCameraView(void)
{
	GsSetProjection(VIEWPORT_DISTANCE);
	DRAWING_OFFSET_X = MAIN_D_801351C0;
	DRAWING_OFFSET_Y = MAIN_D_801351C4;
	GsWSMATRIX = *(MATRIX *)STD_D_8007FC00;
	SetRotMatrix(&GsWSMATRIX);
	SetTransMatrix(&GsWSMATRIX);
}

void STD_setupFixedCamera(void)
{
	*(MATRIX *)STD_D_8007FC00 = GsWSMATRIX;
	MAIN_D_801351C0 = DRAWING_OFFSET_X;
	MAIN_D_801351C4 = DRAWING_OFFSET_Y;
	MAIN_D_80139B34.vpx = 0;
	MAIN_D_80139B34.vpz = -0x7d0;
	MAIN_D_80139B34.vpy = 0;
	MAIN_D_80139B34.vrx = 0;
	MAIN_D_80139B34.vry = 0;
	MAIN_D_80139B34.vrz = 0;
	MAIN_D_80139B34.rz = 0;
	MAIN_D_80139B34.super = NULL;
	GsSetRefView2(&MAIN_D_80139B34);
	DRAWING_OFFSET_X = 0xa0;
	DRAWING_OFFSET_Y = 0x78;
}

void STD_getSourceBoneTransform(void)
{
	MATRIX m;
	SVECTOR rot;
	int32_t *rotOut;
	int32_t *posOut;
	int32_t bone;

	rotOut = EFE_POP1(int32_t *);
	posOut = EFE_POP1(int32_t *);
	bone = EFE_POP1(int32_t);
	calculateBoneMatrix(MAIN_D_80134CE8->sourceEntity, bone, &m);
	matrixToEuler2(&m, &rot);
	rotOut[0] = rot.vx;
	rotOut[1] = rot.vy;
	rotOut[2] = rot.vz;
	posOut[0] = m.t[0];
	posOut[1] = m.t[1];
	posOut[2] = m.t[2];
}

void STD_copyToParentTransform(void)
{
	int32_t *dst;
	int32_t src;
	int32_t off;
	int32_t size;

	size = EFE_POP1(int32_t);
	off = EFE_POP1(int32_t);
	src = EFE_POP1(int32_t);
	dst = (int32_t *)(EFE_PARENT_INSTANCE + off);
	switch (size) {
	case 0xc:
		*dst++ = *(int32_t *)src;
		*dst++ = *(int32_t *)(src + 4);
		*dst = *(int32_t *)(src + 8);
		break;
	case 4:
		*dst = *(int32_t *)src;
		break;
	case 8:
		*dst++ = *(int32_t *)src;
		*dst = *(int32_t *)(src + 4);
		break;
	case 2:
		*(int16_t *)dst = *(int16_t *)src;
		break;
	case 1:
		*(int8_t *)dst = *(int8_t *)src;
		break;
	}
}

void STD_renderEFELine(void)
{
	int32_t *col;
	int32_t i;
	int32_t depth;
	int32_t y0;
	int32_t x1;
	int32_t y1;
	int32_t x0;
	int32_t flags;

	flags = EFE_POP1(int32_t);
	depth = EFE_POP1(int32_t);
	col = EFE_POP1(int32_t *);
	x0 = EFE_POP1(int32_t);
	y1 = EFE_POP1(int32_t);
	x1 = EFE_POP1(int32_t);
	y0 = EFE_POP1(int32_t);

	if ((depth > 0x20) && (depth < 0x1000)) {
		if ((flags & 0x20) != 0) {
			for (i = 0; i < 4; i++) {
				renderLinePrimitive(((col[0] * 50 / 100) & 0xff) | (((col[1] * 50 / 100) & 0xff) << 8) | (((col[2] * 50 / 100) & 0xff) << 16), y0 + MAIN_D_801348E8[i], x1 + MAIN_D_801348EC[i], y1 + MAIN_D_801348E8[i], x0 + MAIN_D_801348EC[i], depth, 5);
			}
		} else {
			renderLinePrimitive((col[0] & 0xff) | ((col[1] & 0xff) << 8) | ((col[2] & 0xff) << 16), y0, x1, y1, x0, depth, 0);
		}
	}
}

void STD_combineRotations(void)
{
	SVECTOR r1;
	SVECTOR r2;
	VECTOR *b;
	VECTOR *a;

	b = EFE_POP1(VECTOR *);
	a = EFE_POP1(VECTOR *);
	r1.vx = a->vx;
	r1.vy = a->vy;
	r1.vz = a->vz;
	r2.vx = b->vx;
	r2.vy = b->vy;
	r2.vz = b->vz;
	multiplyRotations(&r1, &r2);
	a->vx = r1.vx;
	a->vy = r1.vy;
	a->vz = r1.vz;
}

void STD_func_800728D4(void)
{
	MATRIX m;
	SVECTOR rot;
	int32_t *v;
	int32_t *out;

	v = EFE_POP1(int32_t *);
	out = EFE_POP1(int32_t *);
	rot.vx = v[0];
	rot.vy = v[1];
	rot.vz = v[2];
	RotMatrixYXZ(&rot, &m);
	MAIN_func_800E4470(&m, &rot);
	out[0] = rot.vx;
	out[1] = rot.vy;
	out[2] = rot.vz;
}

void STD_rotateVectorByAngles(void)
{
	MATRIX m;
	VECTOR res;
	SVECTOR rot;
	int32_t *v;
	int32_t *w;

	v = EFE_POP1(int32_t *);
	w = EFE_POP1(int32_t *);
	rot.vx = w[0];
	rot.vy = w[1];
	rot.vz = w[2];
	RotMatrixYXZ(&rot, &m);
	ApplyMatrixLV(&m, (VECTOR *)v, &res);
	v[0] = res.vx;
	v[1] = res.vy;
	v[2] = res.vz;
}

void STD_getTargetBoneTransform(void)
{
	MATRIX m;
	SVECTOR rot;
	int32_t *rotOut;
	int32_t *posOut;
	int32_t idx;
	GsCOORDINATE2 *coord;
	GsCOORDINATE2 *matrix;

	rotOut = EFE_POP1(int32_t *);
	posOut = EFE_POP1(int32_t *);
	idx = EFE_POP1(int32_t);
	matrix = (GsCOORDINATE2 *)(((char **)(int32_t)MAIN_D_80134CE8->targetEntity)[1] + 0x10);
	coord = (GsCOORDINATE2 *)(((char **)(int32_t)MAIN_D_80134CE8->targetEntity)[1] + (idx * 136) + 0x10);
	RotMatrix((SVECTOR *)(((char **)(int32_t)MAIN_D_80134CE8->targetEntity)[1] + 0x70), &matrix->coord);
	ScaleMatrix(&matrix->coord, (VECTOR *)(((char **)(int32_t)MAIN_D_80134CE8->targetEntity)[1] + 0x60));
	TransMatrix(&matrix->coord, (VECTOR *)(((char **)(int32_t)MAIN_D_80134CE8->targetEntity)[1] + 0x78));
	calculatePosition(coord, &m);
	matrixToEuler2(&m, &rot);
	rotOut[0] = rot.vx;
	rotOut[1] = rot.vy;
	rotOut[2] = rot.vz;
	posOut[0] = m.t[0];
	posOut[1] = m.t[1];
	posOut[2] = m.t[2];
}

void STD_centerTransformOnEntities(void)
{
	EfeTransform *sum;
	int32_t i;
	int32_t count;
	int32_t *p;
	Entity *e;

	count = 0;
	sum = (EfeTransform *)((int32_t)EFE_INSTANCE + 4);
	sum->position[0] = 0;
	sum->position[1] = 0;
	sum->position[2] = 0;
	*(int32_t *)((int32_t)EFE_INSTANCE + 0x10) = 0;
	*(int32_t *)((int32_t)EFE_INSTANCE + 0x14) = 0;
	*(int32_t *)((int32_t)EFE_INSTANCE + 0x18) = 0;
	for (i = 1; i < 10; i++) {
		if (ENTITY_TABLE[i] == NULL) {
			continue;
		}
		e = ENTITY_TABLE[i];
		if (e == MAIN_D_80134CE8->sourceEntity) {
			continue;
		}
		if (((int8_t *)e)[0x34] == 0) {
			continue;
		}
		count++;
		p = (int32_t *)(((char **)e)[1] + 0x34);
		sum->position[0] = sum->position[0] + p[5];
		sum->position[1] = sum->position[1] + p[6];
		sum->position[2] = sum->position[2] + p[7];
	}

	sum->position[0] /= count;
	sum->position[1] /= count;
	sum->position[2] /= count;
}

void STD_shiftVectorsRight(void)
{
	int32_t *a;
	int32_t *b;

	b = EFE_POP1(int32_t *);
	a = EFE_POP1(int32_t *);
	a[0] >>= b[0];
	a[1] >>= b[1];
	a[2] >>= b[2];
}

void STD_maskVectors(void)
{
	int32_t *a;
	int32_t *b;

	b = EFE_POP1(int32_t *);
	a = EFE_POP1(int32_t *);
	a[0] &= b[0];
	a[1] &= b[1];
	a[2] &= b[2];
}

void STD_divideVectors(void)
{
	int32_t *a;
	int32_t *b;

	b = EFE_POP1(int32_t *);
	a = EFE_POP1(int32_t *);
	a[0] /= b[0];
	a[1] /= b[1];
	a[2] /= b[2];
}

void STD_multiplyVectors(void)
{
	int32_t *a;
	int32_t *b;

	b = EFE_POP1(int32_t *);
	a = EFE_POP1(int32_t *);
	a[0] *= b[0];
	a[1] *= b[1];
	a[2] *= b[2];
}

void STD_subtractVectors(void)
{
	int32_t *a;
	int32_t *b;

	b = EFE_POP1(int32_t *);
	a = EFE_POP1(int32_t *);
	a[0] -= b[0];
	a[1] -= b[1];
	a[2] -= b[2];
}

void STD_addVectors(void)
{
	int32_t *a;
	int32_t *b;

	b = EFE_POP1(int32_t *);
	a = EFE_POP1(int32_t *);
	a[0] += b[0];
	a[1] += b[1];
	a[2] += b[2];
}

void STD_copyVector(void)
{
	int32_t *dst;
	int32_t *src;

	src = EFE_POP1(int32_t *);
	dst = EFE_POP1(int32_t *);
	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[2];
}

void STD_getVectorLength(void)
{
	int32_t *v;
	int32_t *out;

	v = EFE_POP1(int32_t *);
	out = EFE_POP1(int32_t *);
	*out = getDistance(v[0], v[1], v[2]);
}

void STD_setTargetToHitEntity(void)
{
	MAIN_D_80134CE8->targetEntity = ENTITY_TABLE[MAIN_D_80134CD8];
}

void STD_func_80072FF8(void)
{
	SVECTOR in;
	SVECTOR res;
	MATRIX m;
	int32_t *v;
	int32_t *out;

	v = EFE_POP1(int32_t *);
	out = EFE_POP1(int32_t *);
	in.vx = v[0];
	in.vy = v[1];
	in.vz = v[2];
	RotMatrixYXZ(&in, &m);
	MAIN_func_800E4470(&m, &res);
	out[0] = res.vx;
	out[1] = res.vy;
	out[2] = res.vz;
}

void STD_findHitEntity(void)
{
	SVECTOR center;
	AABB box;
	int32_t *ext;
	int32_t mode;
	int32_t *out;
	int32_t id;
	int32_t idx;

	ext = EFE_POP1(int32_t *);
	mode = EFE_POP1(int32_t);
	out = EFE_POP1(int32_t *);
	*out = 0;
	center.vx = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[0];
	center.vy = *(int32_t *)((int32_t)EFE_INSTANCE + 8);
	center.vz = *(int32_t *)((int32_t)EFE_INSTANCE + 0xc);
	box.center = &center;
	box.extent.vx = ext[0];
	box.extent.vy = ext[1];
	box.extent.vz = ext[2];
	MAIN_D_80134CD8 = 1;
	while (1) {
		id = findAABBHitEntity(&box, MAIN_D_80134CE8->sourceEntity, MAIN_D_80134CD8);
		MAIN_D_80134CD8 = id;
		if (id == -1) {
			return;
		}
		switch (mode) {
		case 0:
			idx = (*(int32_t *)&MAIN_D_80134CD8);
			if (((int8_t *)ENTITY_TABLE[idx])[0x53] == 0) {
				*out = 1;
				return;
			}
			MAIN_D_80134CD8 = idx + 1;
			break;
		case 1:
			*out = 1;
			return;
		default:
			return;
		}
	}
}

void STD_getVectorEulerAngles(void)
{
	SVECTOR rot;
	int32_t *v;
	int32_t *out;

	v = EFE_POP1(int32_t *);
	out = EFE_POP1(int32_t *);
	toEulerAngles(&rot, v[0], v[1], v[2]);
	out[0] = rot.vx;
	out[1] = rot.vy;
	out[2] = rot.vz;
}

void STD_printDebugValue(void)
{
	printf(MAIN_D_801348E4, EFE_POP1(int32_t));
}

void STD_getRandomInRange(void)
{
	int32_t a;
	int32_t b;
	int32_t *out;

	a = EFE_POP1(int32_t);
	b = EFE_POP1(int32_t);
	out = EFE_POP1(int32_t *);
	*out = customRandom(b, a);
}

void STD_interpolateValue(void)
{
	int32_t *out;
	int32_t lo;
	int32_t hi;
	int32_t t;
	int32_t start;
	int32_t end;
	int32_t tmp;

	end = EFE_POP1(int32_t);
	start = EFE_POP1(int32_t);
	t = EFE_POP1(int32_t);
	hi = EFE_POP1(int32_t);
	lo = EFE_POP1(int32_t);
	out = EFE_POP1(int32_t *);
	if (hi < lo) {
		tmp = lo;
		lo = hi;
		hi = tmp;
	}

	if (t < lo) {
		t = lo;
	}

	if (hi < t) {
		t = hi;
	}

	if (lo == hi) {
		*out = start;
	} else {
		*out = start + ((t - lo) * (end - start) / (hi - lo));
	}
}

void STD_func_800733CC(void)
{
	int32_t *out;
	int32_t scale;
	int32_t angle;

	out = EFE_POP1(int32_t *);
	scale = EFE_POP1(int32_t);
	angle = EFE_POP1(int32_t);
	*out = (scale * rcos(angle & 0xfff)) >> 12;
}

void STD_func_80073440(void)
{
	int32_t *out;
	int32_t scale;
	int32_t angle;

	out = EFE_POP1(int32_t *);
	scale = EFE_POP1(int32_t);
	angle = EFE_POP1(int32_t);
	*out = (scale * rsin(angle & 0xfff)) >> 12;
}

void STD_func_800734B4(void)
{
	int32_t *out;
	int32_t type;
	int16_t radius;
	int16_t height;

	out = EFE_POP1(int32_t *);
	type = *(int32_t *)(int32_t)MAIN_D_80134CE8->sourceEntity;
	radius = DIGIMON_DATA[type].radius;
	height = DIGIMON_DATA[type].height;
	out[0] = radius;
	out[1] = height;
	out[2] = radius;
}

INCLUDE_ASM("asm/std/nonmatchings/std_effect", STD_applyHomingMovement);

void STD_getUVAnimTimer(void)
{
	int32_t off;
	int32_t *out;
	int32_t idx;
	char *base;

	idx = EFE_POP1(int32_t);
	out = EFE_POP1(int32_t *);
	off = (uint32_t)idx;
	base = (char *)MAIN_D_80134D0C[8];
	*out = ((int16_t *)(base + (off * 32)))[9];
}

void STD_checkTargetCollision(void)
{
	int32_t r;
	int32_t *flag;
	int32_t *out2;
	int32_t *out;
	char *tgt;
	int32_t d0;
	int32_t d1;
	int16_t ang;
	int32_t t;

	r = EFE_POP1(int32_t);
	flag = EFE_POP1(int32_t *);
	out2 = EFE_POP1(int32_t *);
	out = EFE_POP1(int32_t *);
	tgt = ((char **)(int32_t)MAIN_D_80134CE8->targetEntity)[1] + 0x48;
	d0 = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[0] - *(int32_t *)tgt;
	d1 = *(int32_t *)((int32_t)EFE_INSTANCE + 0xc) - *(int32_t *)(tgt + 8);
	if ((r * r) < ((d0 * d0) + (d1 * d1))) {
		goto zero;
	}

	*flag = 1;
	ang = _atan(d0, d1);
	ang = ang - 0x400;
	ang = -ang;
	ang = ang & 0xfff;
	t = ang + 0x800;
	*out2 = t;
	ang = t;
	ang = ang >> 3;
	out[2] = (r * _sin(0x80 - ang)) >> 12;
	out[0] = (r * _cos(0x80 - ang)) >> 12;
	return;
zero:
	*flag = 0;
}

void STD_rotateTransformTowardPoint(void)
{
	int32_t *p;
	EfeTransform *q;
	int32_t angle;

	p = EFE_POP1(int32_t *);
	q = (EfeTransform *)((int32_t)EFE_INSTANCE + 4);
	angle = _atan(p[0] - q->position[0], p[2] - q->position[2]);
	angle = -(angle - 0x400) & 0xfff;
	*(int32_t *)((int32_t)EFE_INSTANCE + (int32_t)&((EfeInstance *)0)->transform.rotation[1]) = angle;
}

void STD_setTransformToSourceBone(void)
{
	int32_t idx;
	int32_t *p;
	char *src;
	int32_t s1;
	int32_t s2;

	idx = EFE_POP1(int32_t);
	p = &((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[0];
	src = ((char **)(int32_t)MAIN_D_80134CE8->sourceEntity)[1] + (idx * 136) + 0x34;
	*p++ = *(int32_t *)(src + 0x14);
	*p++ = *(int32_t *)(src + 0x18);
	*p++ = *(int32_t *)(src + 0x1c);
	*p++ = 0;
	*p++ = ((int16_t *)((char **)(int32_t)MAIN_D_80134CE8->sourceEntity)[1])[0x39];
	*p = 0;
	s1 = _sin(-(p[-1] >> 3));
	s2 = _sin(-(((p[-1] >> 3) + 0x80) & 0x1ff));
	p[-5] = p[-5] + ((s1 * 25) >> 12);
	p[-3] = p[-3] + ((s2 * 25) >> 12);
}

void STD_renderParallaxSprites(void)
{
	POLY_FT4 *prim;
	int16_t *p;
	ModelComponent *m;
	int32_t ox;
	int32_t oy;
	int16_t x;
	int16_t y;
	int16_t sz;

	oy = EFE_POP1(int32_t);
	ox = EFE_POP1(int32_t);
	p = EFE_POP1(int16_t *);
	m = (ModelComponent *)MAIN_D_80134D0C[6];
	prim = (POLY_FT4 *)GsGetWorkBase();

	while (1) {
		if (p[0] < 0) {
			break;
		}
		SetPolyFT4(prim);
		prim->r0 = prim->g0 = prim->b0 = 0x80;
		prim->tpage = m->pixelPage | 0x20;
		prim->clut = m->clutPage + 0x40;
		prim->u0 = m->pixelOffsetX + p[4];
		prim->v0 = m->pixelOffsetY;
		prim->u1 = m->pixelOffsetX + p[4] + 0x1f;
		prim->v1 = m->pixelOffsetY;
		prim->u2 = m->pixelOffsetX + p[4];
		prim->v2 = m->pixelOffsetY + 0x1f;
		prim->u3 = m->pixelOffsetX + p[4] + 0x1f;
		prim->v3 = m->pixelOffsetY + 0x1f;
		x = (p[3] * (p[1] + ox)) >> 7;
		y = (p[3] * (p[2] + oy)) >> 7;
		x = x % 400;
		y = y % 320;
		if (x < 0) {
			x = x + 360;
		}
		if (y < 0) {
			y = y + 280;
		}
		y = y - 0xa0;
		x = x - 0xc8;
		sz = (p[0] * p[3]) >> 8;
		prim->x0 = x;
		prim->y0 = y;
		prim->x1 = x + sz;
		prim->y1 = y;
		prim->x2 = x;
		prim->y2 = y + sz;
		prim->x3 = x + sz;
		prim->y3 = y + sz;
		AddPrim(ACTIVE_ORDERING_TABLE->org + 0x1e, prim);
		prim++;
		p += 5;
	}

	GsSetWorkBase((PACKET *)prim);
}

INCLUDE_ASM("asm/std/nonmatchings/std_effect", STD_renderScrollingBackground);

void STD_setTransformToBoneOffset(void)
{
	MATRIX m;
	SVECTOR in;
	SVECTOR out;
	int32_t *p;
	int32_t *q;

	p = (int32_t *)((int32_t)EFE_INSTANCE + 4);
	calculateBoneMatrix(MAIN_D_80134CE8->sourceEntity, MAIN_D_80134CE8->boneOffset->boneId, &m);
	in.vx = MAIN_D_80134CE8->boneOffset->positionX;
	in.vy = MAIN_D_80134CE8->boneOffset->positionY;
	in.vz = MAIN_D_80134CE8->boneOffset->positionZ;
	ApplyMatrixSV(&m, &in, &out);
	p[0] = out.vx;
	p[1] = out.vy;
	p[2] = out.vz;
	q = p++;
	*q = *q + m.t[0];
	q = p++;
	*q = *q + m.t[1];
	q = p++;
	*q = *q + m.t[2];
	*p++ = 0;
	*p++ = MAIN_D_80134CE8->sourceEntity->posData->rotation.vy;
	*p = 0;
}
void STD_playEFESound(void)
{
	int32_t id;
	int32_t i;

	int32_t (*p)[2];

	id = EFE_POP1(int32_t);
	if (id < 0) {
		return;
	}

	if ((id != 0x12) && (id != 0x20) && (id != 0x21) && (id != 0x22)) {
		playSound(8, id);
		return;
	}

	p = MAIN_D_80139AD0;
	for (i = 0; i < 10; i++) {
		if ((*p)[0] < 0) {
			(*p)[0] = playSound2(8, id);
			(*p)[1] = (int32_t)MAIN_D_80134CE8;
			return;
		}
		p++;
	}
}

void STD_addSourceEntityParticleFX(void)
{
	int32_t timer;

	timer = EFE_POP1(int32_t);
	addEntityParticleFX((int32_t *)(int32_t)MAIN_D_80134CE8->sourceEntity, timer);
}

void STD_copyFromParentTransform(void)
{
	int32_t *src;
	int32_t dst;
	int32_t off;
	int32_t size;

	size = EFE_POP1(int32_t);
	off = EFE_POP1(int32_t);
	dst = EFE_POP1(int32_t);
	src = (int32_t *)(EFE_PARENT_INSTANCE + off);
	switch (size) {
	case 0xc:
		*(int32_t *)dst = *src++;
		*(int32_t *)(dst + 4) = *src++;
		*(int32_t *)(dst + 8) = *src;
		break;
	case 4:
		*(int32_t *)dst = *src;
		break;
	case 2:
		*(int16_t *)dst = *(int16_t *)src;
		break;
	case 1:
		*(int8_t *)dst = *(int8_t *)src;
		break;
	}
}

void STD_calculatePolarOffset(void)
{
	int32_t r;
	int32_t ang;
	int32_t *out;

	r = EFE_POP1(int32_t);
	ang = EFE_POP1(int32_t);
	out = EFE_POP1(int32_t *);
	out[2] = (r * _sin(0x80 - ang)) >> 12;
	out[0] = (r * _cos(0x80 - ang)) >> 12;
}

INCLUDE_ASM("asm/std/nonmatchings/std_effect", STD_renderProjectedSprite);

void STD_func_80074A70(void)
{
	int32_t *out;
	int32_t type;
	int16_t radius;
	int16_t height;

	out = EFE_POP1(int32_t *);
	type = *(int32_t *)(int32_t)MAIN_D_80134CE8->targetEntity;
	radius = DIGIMON_DATA[type].radius;
	height = DIGIMON_DATA[type].height;
	out[0] = radius;
	out[1] = height;
	out[2] = radius;
}

void STD_renderParticleFlashSprite(void)
{
	int16_t p[14];
	char *hdr;
	int32_t n;

	hdr = (char *)MAIN_D_80134D0C[6];
	n = EFE_POP1(int32_t);
	p[2] = EFE_POP1(int32_t);
	*(int32_t *)&p[4] = EFE_POP1(int16_t);
	p[7] = EFE_POP1(int32_t);
	p[6] = EFE_POP1(int32_t);
	((int8_t *)p)[0x13] = EFE_POP1(int32_t) + ((uint8_t *)hdr)[0x15];
	((int8_t *)p)[0x12] = EFE_POP1(int32_t) + ((uint8_t *)hdr)[0x14];
	p[1] = EFE_POP1(int32_t);
	p[0] = EFE_POP1(int32_t);
	if (p[2] < 0xa) {
		return;
	}

	if (p[2] >= 0x1000) {
		return;
	}

	p[8] = ((uint16_t *)hdr)[8] | 0x20;
	p[10] = ((uint16_t *)hdr)[9] + (n << 6);

	((uint8_t *)p)[0x16] = ((uint8_t *)p)[0x17] = ((uint8_t *)p)[0x18] = ((uint8_t *)p)[0x19] = 0x80;
	renderParticleFlash(p);
}

void STD_projectPositionToScreen(void)
{
	DVECTOR screen;
	SVECTOR pos;
	int32_t *depthOut;
	int32_t *yOut;
	int32_t *xOut;
	int32_t *src;

	depthOut = EFE_POP1(int32_t *);
	yOut = EFE_POP1(int32_t *);
	xOut = EFE_POP1(int32_t *);
	src = EFE_POP1(int32_t *);
	pos.vx = src[0];
	pos.vy = src[1];
	pos.vz = src[2];
	*depthOut = worldPosToScreenPos(&pos, &screen);
	*depthOut >>= 4;
	*xOut = screen.vx;
	*yOut = screen.vy;
}

void STD_renderScreenSprite(void)
{
	GsSPRITE sprite;
	ModelComponent *m;
	int32_t flip;
	int32_t depth;

	m = (ModelComponent *)MAIN_D_80134D0C[6];
	flip = EFE_POP1(int32_t);
	sprite.scaley = EFE_POP1(int32_t);
	sprite.scalex = EFE_POP1(int32_t);
	depth = EFE_POP1(int32_t);
	sprite.rotate = EFE_POP1(int32_t) << 12;
	sprite.my = EFE_POP1(int32_t);
	sprite.mx = EFE_POP1(int32_t);
	sprite.h = EFE_POP1(int32_t);
	sprite.w = EFE_POP1(int32_t);
	sprite.v = EFE_POP1(int32_t) + m->pixelOffsetY;
	sprite.u = EFE_POP1(int32_t) + m->pixelOffsetX;
	sprite.y = EFE_POP1(int32_t);
	sprite.x = EFE_POP1(int32_t);

	if (flip < 0) {
		sprite.x += (int16_t)(0xa0 - DRAWING_OFFSET_X);
		sprite.y += (int16_t)(0x78 - DRAWING_OFFSET_Y);
		sprite.cy = -flip + (m->clutPage >> 6);
	} else {
		sprite.cy = flip + (m->clutPage >> 6);
	}

	sprite.attribute = 0x50000000;
	sprite.tpage = m->pixelPage | 0x20;
	sprite.cx = (m->clutPage & 0x3f) << 4;
	sprite.r = sprite.g = sprite.b = 0x80;

	if ((depth >= 0) && (depth < 0x1000)) {
		GsSortSprite(&sprite, ACTIVE_ORDERING_TABLE, (uint16_t)depth);
	}
}

void STD_addCloudEffect(void)
{
	int16_t pos[3];
	int32_t *v;

	v = EFE_POP1(int32_t *);
	pos[0] = v[0];
	pos[1] = v[1];
	pos[2] = v[2];
	createCloudFX(pos);
}

void STD_selectNextTargetEntity(void)
{
	int32_t *out;
	Entity *e;

	out = EFE_POP1(int32_t *);
	if (MAIN_D_80134CE0 >= 10) {
		*out = -1;
		return;
	}

	for (; MAIN_D_80134CE0 < 10; MAIN_D_80134CE0++) {
		if (ENTITY_TABLE[MAIN_D_80134CE0] == NULL) {
			continue;
		}
		e = ENTITY_TABLE[MAIN_D_80134CE0];
		if (e == MAIN_D_80134CE8->sourceEntity) {
			continue;
		}
		if (((int8_t *)e)[0x34] == 0) {
			continue;
		}
		if (((int8_t *)e)[0x35] == 0) {
			continue;
		}
		if (((int16_t *)e)[0x26] > 0) {
			break;
		}
	}

	if (MAIN_D_80134CE0 >= 10) {
		*out = -1;
		return;
	}

	MAIN_D_80134CE8->targetEntity = ENTITY_TABLE[*(int32_t *)&MAIN_D_80134CE0];
	*out = MAIN_D_80134CE0++;
}

void STD_addParticleEmitter(void)
{
	int32_t n;
	int32_t *vec;
	int32_t a;
	int32_t b;
	int32_t i;
	char *p;

	a = EFE_POP1(int32_t);
	vec = EFE_POP1(int32_t *);
	n = EFE_POP1(int32_t);
	b = EFE_POP1(int32_t);
	for (i = 0; i < 4; i++) {
		if (((int32_t (*)[0x6d])MAIN_D_80134CCC)[i][0] == 0) {
			break;
		}
	}

	if (i == 4) {
		return;
	}

	p = *(char **)&MAIN_D_80134CCC + (i * 0x1b4);
	p[0xf] = b;
	for (i = 0; i < 0x15; i++) {
		((int16_t (*)[10])p + i)[0][8] = 0;
	}

	*(int32_t *)p = (int32_t)EFE_INSTANCE;
	*(int16_t *)(p + 4) = a;
	p[0xc] = vec[0];
	p[0xd] = vec[1];
	p[0xe] = vec[2];
	*(int16_t *)(p + 6) = n * 16;
	*(int16_t *)(p + 8) = n / 8 * 16;
	*(int16_t *)(p + 0xa) = n / 160 * 16;
}

void STD_setEFEModelObjectColor(void)
{
	int32_t *rec;
	int32_t *color;
	int32_t idx;
	int32_t i;
	int32_t count;
	int32_t t;

	char (*pr)[0x20];
	char (*pg)[0x20];
	char (*pb)[0x20];
	int32_t *ent;

	color = EFE_POP1(int32_t *);
	idx = EFE_POP1(int32_t);
	ent = (int32_t *)((uint32_t)((char **)MAIN_D_80134D0C[6])[1] + 0xc);
	ent = (int32_t *)((int32_t)ent + (idx * 28));
	rec = (int32_t *)ent[4];
	count = ent[5];
	pr = (char (*)[0x20])((char *)rec + 0x14);
	i = 0;
	pg = (char (*)[0x20])((char *)rec + 0x15);
	pb = (char (*)[0x20])((char *)rec + 0x16);
	for (; i < count; i++) {
		t = (*rec >> 24) & 0xff;
		if ((t == 0x2f) || (t == 0x2d)) {
			(*pr)[0] = (int16_t)color[0];
			(*pg)[0] = (int16_t)color[1];
			(*pb)[0] = (int16_t)color[2];
			rec = (int32_t *)((int32_t)rec + 0x20);
			pr++;
			pg++;
			pb++;
		}
	}
}

void STD_copyTargetEntityPosition(void)
{
	int32_t *out;
	int32_t *m;

	out = EFE_POP1(int32_t *);
	m = (int32_t *)(((char **)(int32_t)MAIN_D_80134CE8->targetEntity)[1] + 0x34);
	out[0] = m[5];
	out[1] = m[6];
	out[2] = m[7];
}

void STD_steerTransformTowardPoint(void)
{
	MATRIX m;
	SVECTOR in;
	SVECTOR rot;
	SVECTOR out;
	EfeTransform *pos;
	int32_t turn;
	int32_t speed;
	int32_t *target;
	int32_t d;

	turn = EFE_POP1(int32_t);
	speed = EFE_POP1(int32_t);
	target = EFE_POP1(int32_t *);
	pos = (EfeTransform *)((int32_t)EFE_INSTANCE + 4);
	rot.vx = 0;
	rot.vy = -*(int32_t *)((int32_t)EFE_INSTANCE + 0x14);
	rot.vz = 0;
	in.vx = target[0] - pos->position[0];
	in.vy = 0;
	in.vz = target[2] - pos->position[2];
	RotMatrixZYX(&rot, &m);
	ApplyMatrixSV(&m, &in, &out);
	d = _atan(out.vx, out.vz);
	d = -(d - 0x400) & 0xfff;
	if (d >= 0x801) {
		d -= 0x1000;
	}

	if (d > 0) {
		if (d < turn) {
			turn = d;
		}
	} else if (-d < turn) {
		turn = d;
	} else {
		turn = -turn;
	}

	in.vx = 0;
	in.vy = 0;
	in.vz = -speed * 8;
	rot.vx = *(int32_t *)((int32_t)EFE_INSTANCE + 0x10);
	*(int32_t *)((int32_t)EFE_INSTANCE + 0x14) += turn;
	rot.vy = *(int32_t *)((int32_t)EFE_INSTANCE + 0x14);
	rot.vz = *(int32_t *)((int32_t)EFE_INSTANCE + 0x18);
	RotMatrixZYX(&rot, &m);
	ApplyMatrixSV(&m, &in, &out);
	pos->position[0] += out.vx >> 3;
	pos->position[1] += out.vy >> 3;
	pos->position[2] += out.vz >> 3;
}

void STD_interpolateVector(void)
{
	int32_t *out;
	int32_t t0;
	int32_t t1;
	int32_t t;
	int32_t *b;
	int32_t *a;

	out = EFE_POP1(int32_t *);
	t = EFE_POP1(int32_t);
	t1 = EFE_POP1(int32_t);
	t0 = EFE_POP1(int32_t);
	b = EFE_POP1(int32_t *);
	a = EFE_POP1(int32_t *);
	out[0] = lerp(a[0], b[0], t0, t1, t);
	out[1] = lerp(a[1], b[1], t0, t1, t);
	out[2] = lerp(a[2], b[2], t0, t1, t);
}

void STD_discardEFEOperandPair(void)
{
	EFE_DROP1();
	EFE_DROP1();
}

void STD_getScatteredSpawnPosition(void)
{
	int32_t r;
	int32_t *out;
	int32_t *p;
	int32_t *src;

	r = EFE_POP1(int32_t);
	out = EFE_POP1(int32_t *);

	if (EFE_PARENT_INSTANCE == 0) {
	}

	if (EFE_PARENT_INSTANCE == 0) {
		p = (int32_t *)(((char **)(int32_t)MAIN_D_80134CE8->sourceEntity)[1] + (*(int16_t *)(int32_t)MAIN_D_80134CE8->boneOffset * 136) + 0x34);
		out[0] = p[5];
		out[1] = p[6];
		out[2] = p[7];
	} else {
		src = (int32_t *)(EFE_PARENT_INSTANCE + 4);
		out[0] = *src++;
		out[1] = *src++;
		out[2] = *src;
	}

	if (r > 0) {
		out[0] = out[0] + ((rand() % r) - (r >> 1));
		out[1] = out[1] + ((rand() % r) - (r >> 1));
		out[2] = out[2] + ((rand() % r) - (r >> 1));
	}
}

void STD_checkCollisionWithDefaultPower(void)
{
	EFE_PUSH1(int32_t, -1);
	STD_applyBoxAttackHit();
}

void STD_addAttackObjectToTarget(void)
{
	int16_t pos[4];
	int32_t i;
	int32_t j;
	int32_t e;

	if (((int8_t *)(int32_t)MAIN_D_80134CE8->targetEntity)[0x53] != 0) {
		return;
	}

	pos[0] = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[0];
	pos[1] = *(int32_t *)((int32_t)EFE_INSTANCE + 8);
	pos[2] = *(int32_t *)((int32_t)EFE_INSTANCE + 0xc);
	for (i = 1; i < 10; i++) {
		if ((int32_t)ENTITY_TABLE[i] == (int32_t)MAIN_D_80134CE8->targetEntity) {
			break;
		}
	}

	((int8_t *)(int32_t)MAIN_D_80134CE8->targetEntity)[0x53] = 1;
	for (j = 1; j < 10; j++) {
		e = (int32_t)ENTITY_TABLE[j];
		if (e == (int32_t)MAIN_D_80134CE8->sourceEntity) {
			break;
		}
	}

	addAttackObject(i, 1, pos, MAIN_D_80134CD4, MAIN_D_80134CD0, j);
}

void STD_setTransformToTargetBone(void)
{
	int32_t r;
	int32_t idx;
	int32_t *p;
	char *src;
	int32_t s1;
	int32_t s2;

	r = EFE_POP1(int32_t);
	idx = EFE_POP1(int32_t);
	p = &((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[0];
	src = ((char **)(int32_t)MAIN_D_80134CE8->targetEntity)[1] + (idx * 136) + 0x34;
	*p++ = *(int32_t *)(src + 0x14);
	*p++ = *(int32_t *)(src + 0x18);
	*p++ = *(int32_t *)(src + 0x1c);
	*p++ = 0;
	*p++ = ((int16_t *)((char **)(int32_t)MAIN_D_80134CE8->targetEntity)[1])[0x39];
	*p = 0;
	s1 = _sin(-(p[-1] >> 3));
	s2 = _sin(-(((p[-1] >> 3) + 0x80) & 0x1ff));
	p[-5] = p[-5] + ((r * s1) >> 12);
	p[-3] = p[-3] + ((r * s2) >> 12);
}

void STD_renderCenteredSprite(void)
{
	int32_t y;
	int32_t x;

	y = EFE_POP1(int32_t);
	x = EFE_POP1(int32_t);
	EFE_PUSH1(int32_t, x + 1);
	EFE_PUSH1(int32_t, y + 1);
	EFE_PUSH1(int32_t, x / 2);
	EFE_PUSH1(int32_t, y / 2);
	EFE_PUSH1(int32_t, 0);
	EFE_PUSH1(int32_t, 5);
	EFE_PUSH1(int32_t, 0);
	STD_renderProjectedSprite();
}

void STD_initializeEFETransform(void)
{
	int32_t *src;
	int32_t *dst;
	int32_t *chk;

	dst = &((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[0];
	chk = (int32_t *)EFE_PARENT_INSTANCE;
	if (chk == NULL) {
		STD_setTransformToBoneOffset();
		return;
	}

	src = (int32_t *)((int32_t)chk + 4);
	*dst++ = *src++;
	*dst++ = *src++;
	*dst++ = *src++;
	*dst++ = *src++;
	*dst++ = *src++;
	*dst = *src;
}

void STD_func_80075BF8(void)
{
	EFE_SCRATCH->scale = EFE_POP1(VECTOR *);
	EFE_SCRATCH->id = EFE_POP1(int32_t);
	EFE_SCRATCH->rot.vx = ((VECTOR *)((int32_t)EFE_INSTANCE + 0x10))->vx;
	EFE_SCRATCH->rot.vy = ((VECTOR *)((int32_t)EFE_INSTANCE + 0x10))->vy;
	EFE_SCRATCH->rot.vz = ((VECTOR *)((int32_t)EFE_INSTANCE + 0x10))->vz;
	RotMatrix(&EFE_SCRATCH->rot, &EFE_SCRATCH->m1);
	ScaleMatrix(&EFE_SCRATCH->m1, EFE_SCRATCH->scale);
	EFE_SCRATCH->m1.t[0] = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[0];
	EFE_SCRATCH->m1.t[1] = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[1];
	EFE_SCRATCH->m1.t[2] = ((EfeTransform *)((int32_t)EFE_INSTANCE + 4))->position[2];
	GsMulCoord0(&GsWSMATRIX, &EFE_SCRATCH->m1, &EFE_SCRATCH->m0);
	if (EFE_SCRATCH->m0.t[2] < -0x12c) {
		return;
	}

	if (EFE_SCRATCH->m0.t[2] >= 0x10000) {
		return;
	}

	GsSetLightMatrix(&EFE_SCRATCH->m1);
	GsSetLsMatrix(&EFE_SCRATCH->m0);
	GsLinkObject4((unsigned long)(((char **)MAIN_D_80134D0C[6])[1] + 0xc), &EFE_SCRATCH->obj, EFE_SCRATCH->id);
	EFE_SCRATCH->obj.coord2 = NULL;
	EFE_SCRATCH->obj.attribute = 0;
	GsSortObject4(&EFE_SCRATCH->obj, ACTIVE_ORDERING_TABLE, 2, getScratchAddr(0x2c));
}

void STD_initializeSubEffectInstructions(void)
{
	int32_t b;
	int32_t a;

	a = EFE_POP1(int32_t);
	b = EFE_POP1(int32_t);
	EFE_INSTANCE->frame = 0;
	MAIN_D_80134CE8->inst = (int16_t *)b;
	MAIN_D_80134CE8->someInst = (int16_t *)a;
}

void STD_initializeUVAnim(void)
{
	int32_t val;
	int32_t ptr;
	int32_t idx;
	int32_t nv;
	int32_t *rec;
	int32_t i;
	int32_t *q;
	int32_t t;

	ptr = EFE_POP1(int32_t);
	val = EFE_POP1(int32_t);
	idx = EFE_POP1(int32_t);
	if ((idx < 0) || (idx >= MAIN_D_80134D0C[7])) {
		MAIN_D_80134CE4 = -2;
		MAIN_D_80134D00 = NULL;
		return;
	}

	nv = 8;
	rec = (int32_t *)((char *)MAIN_D_80134D0C[nv] + (nv * (4 * idx)));
	((int16_t *)rec)[0] = val;
	if (ptr == -1) {
		return;
	}

	((int16_t *)rec)[6] = *(int32_t *)ptr;
	rec[6] = ptr + 4;
	rec[7] = rec[6];
	((int16_t *)rec)[8] = 0;
	((int16_t *)rec)[7] = 0;
	((int16_t *)rec)[9] = 1;
	((int16_t *)rec)[10] = 0;
	q = (int32_t *)rec[6];
	for (i = 0; i < ((int16_t *)rec)[6]; i++) {
		t = *(int16_t *)q;
		((int16_t *)rec)[10] += t;
		q++;
	}
}

void STD_checkTechCompatibility(void)
{
	int16_t *p;
	int32_t type;

	p = EFE_POP1(int16_t *);
	type = (int16_t)getOriginalType(*(int32_t *)MAIN_D_80134CE8->sourceEntity);
	while (*p != type) {
		if (*p < 0) {
			MAIN_D_80134CE4 = -1;
			MAIN_D_80134D00 = NULL;
			break;
		}
		p = (int16_t *)((int32_t)p + 0xa);
	}

	MAIN_D_80134CF0 = (int32_t)(p + 1);
	MAIN_D_80134CE8->boneOffset = (EfeBoneOffset *)MAIN_D_80134CF0;
}

INCLUDE_ASM("asm/std/nonmatchings/std_effect", STD_spawnEFESubEffect);

void STD_popEFEValueToVariable(void)
{
	int32_t val;

	if (MAIN_D_80134D00[2] == 0) {
		val = EFE_POP1(int32_t);
		*(int32_t *)(MAIN_D_80134D00[1] + (int32_t)EFE_INSTANCE) = val;
	} else {
		val = EFE_POP1(int32_t);
		*(int32_t *)(MAIN_D_80134D00[1] + MAIN_D_80134D08) = val;
	}

	MAIN_D_80134D00 = (int16_t *)((int32_t)MAIN_D_80134D00 + 6);
}

void STD_returnFromEFESubroutine(void)
{
	MAIN_D_80134D00 = EFE_POP2(int16_t *);
	if (MAIN_D_80134D00 != NULL) {
		MAIN_D_80134CE8 = EFE_POP2(EfeSubEffect *);
		EFE_INSTANCE = MAIN_D_80134CE8->instance;
		EFE_PARENT_INSTANCE = (int32_t)MAIN_D_80134CE8->parentInstance;
		MAIN_D_80134CE0 = EFE_POP2(int32_t);
	}
}

void STD_dispatchEFESubOpcode(void)
{
	MAIN_D_80134D00 = (int16_t *)((int32_t)MAIN_D_80134D00 + 2);
	STD_jtbl_8007FA7C[MAIN_D_80134D04 >> 8]();
}

void STD_callEFESubroutine(void)
{
	int16_t *ip;

	ip = MAIN_D_80134D00;
	EFE_PUSH2(int32_t, MAIN_D_80134CE0);
	EFE_PUSH2(EfeSubEffect *, MAIN_D_80134CE8);
	EFE_PUSH2(int16_t *, MAIN_D_80134D00 + 3);
	MAIN_D_80134D00 = (int16_t *)(ip[1] + MAIN_D_80134D08);
}

void STD_pushEFEVariableAddress(void)
{
	int32_t ip;

	ip = (int32_t)MAIN_D_80134D00;

	*EFE_DATA_STACK = ((int16_t *)ip)[1];
	if (((int16_t *)ip)[2] == 0) {
		*EFE_DATA_STACK++ += (int32_t)EFE_INSTANCE;
	} else {
		*EFE_DATA_STACK++ += MAIN_D_80134D08;
	}

	MAIN_D_80134D00 = (int16_t *)((int32_t)MAIN_D_80134D00 + 6);
}

void STD_pushEFEVariable(void)
{
	int32_t *p;

	if (MAIN_D_80134D00[2] == 0) {
		p = (int32_t *)(MAIN_D_80134D00[1] + (int32_t)EFE_INSTANCE);
	} else {
		p = (int32_t *)(MAIN_D_80134D00[1] + MAIN_D_80134D08);
	}

	if ((MAIN_D_80134D04 & 0xf00) == 0x400) {
		EFE_PUSH1(int32_t, *p);
	} else if ((MAIN_D_80134D04 & 0xf00) == 0x200) {
		EFE_PUSH1(int32_t, *(int16_t *)p);
	} else {
		EFE_PUSH1(int32_t, *(int8_t *)p);
	}

	MAIN_D_80134D00 = (int16_t *)((int32_t)MAIN_D_80134D00 + 6);
}

void STD_pushEFEImmediate(void)
{
	EFE_PUSH1(int32_t, MAIN_D_80134D00[1]);
	MAIN_D_80134D00 = (int16_t *)((int32_t)MAIN_D_80134D00 + 6);
}

void STD_jumpEFEScript(void)
{
	MAIN_D_80134D00 = (int16_t *)(MAIN_D_80134D00[1] + MAIN_D_80134D08);
}

void STD_stopEFEScript(void)
{
	MAIN_D_80134D04 &= 0xff;
	MAIN_D_80134D00 = NULL;
}

void STD_branchEFEOnComparison(void)
{
	int16_t *ip;
	int32_t res;

	ip = MAIN_D_80134D00;
	if ((MAIN_D_80134D04 & 0xf00) == 0x400) {
		if (ip[2] == 0) {
			res = STD_D_8007AF08[MAIN_D_80134D04 >> 12](*(int32_t *)(ip[1] + (int32_t)EFE_INSTANCE));
		} else {
			res = STD_D_8007AF08[MAIN_D_80134D04 >> 12](*(int32_t *)(ip[1] + MAIN_D_80134D08));
		}
	} else if ((MAIN_D_80134D04 & 0xf00) == 0x200) {
		if (ip[2] == 0) {
			res = STD_D_8007AF08[MAIN_D_80134D04 >> 12](*(int16_t *)(ip[1] + (int32_t)EFE_INSTANCE));
		} else {
			res = STD_D_8007AF08[MAIN_D_80134D04 >> 12](*(int16_t *)(ip[1] + MAIN_D_80134D08));
		}
	} else {
		if (ip[2] == 0) {
			res = STD_D_8007AF08[MAIN_D_80134D04 >> 12](*(int8_t *)(ip[1] + (int32_t)EFE_INSTANCE));
		} else {
			res = STD_D_8007AF08[MAIN_D_80134D04 >> 12](*(int8_t *)(ip[1] + MAIN_D_80134D08));
		}
	}

	if (res == 0) {
		MAIN_D_80134D00 = (int16_t *)(ip[3] + MAIN_D_80134D08);
	} else {
		MAIN_D_80134D00 = (int16_t *)((int32_t)MAIN_D_80134D00 + 0xa);
	}
}

void STD_applyEFEVariableOperator(void)
{
	if (MAIN_D_80134D00[2] == 0) {
		STD_D_8007AE68[(MAIN_D_80134D04 >> 8) & 0xf][MAIN_D_80134D04 >> 12]((int32_t *)(MAIN_D_80134D00[1] + (int32_t)EFE_INSTANCE));
	} else {
		STD_D_8007AE68[(MAIN_D_80134D04 >> 8) & 0xf][MAIN_D_80134D04 >> 12]((int32_t *)(MAIN_D_80134D00[1] + MAIN_D_80134D08));
	}

	MAIN_D_80134D00 = (int16_t *)((int32_t)MAIN_D_80134D00 + 6);
}

void STD_loadEFEIndexedVariable(void)
{
	int16_t *ip;
	int32_t idx;

	ip = MAIN_D_80134D00;
	if ((MAIN_D_80134D04 & 0xf000) == 0x4000) {
		if (ip[4] == 0) {
			idx = *(int32_t *)(ip[3] + (int32_t)EFE_INSTANCE);
		} else {
			idx = *(int32_t *)(ip[3] + MAIN_D_80134D08);
		}
	} else if ((MAIN_D_80134D04 & 0xf000) == 0x2000) {
		if (ip[4] == 0) {
			idx = *(int16_t *)(ip[3] + (int32_t)EFE_INSTANCE);
		} else {
			idx = *(int16_t *)(ip[3] + MAIN_D_80134D08);
		}
	} else {
		if (ip[4] == 0) {
			idx = *(int8_t *)(ip[3] + (int32_t)EFE_INSTANCE);
		} else {
			idx = *(int8_t *)(ip[3] + MAIN_D_80134D08);
		}
	}

	if ((MAIN_D_80134D04 & 0xf00) == 0x400) {
		(*(int32_t *)&MAIN_D_80134CEC) = *(int32_t *)(ip[1] + (idx * 4) + MAIN_D_80134D08);
	} else if ((MAIN_D_80134D04 & 0xf00) == 0x200) {
		MAIN_D_80134CEC = *(int16_t *)(ip[1] + (idx * 2) + MAIN_D_80134D08);
	} else {
		MAIN_D_80134CEC = *(int8_t *)(ip[1] + idx + MAIN_D_80134D08);
	}

	MAIN_D_80134D00 = (int16_t *)((int32_t)MAIN_D_80134D00 + 0xa);
}

void STD_loadEFERandomValue(void)
{
	MAIN_D_80134CEC = rand();
	MAIN_D_80134D00 = (int16_t *)((int32_t)MAIN_D_80134D00 + 2);
}

void STD_loadEFEVariable(void)
{
	int16_t *ip;

	ip = MAIN_D_80134D00;
	if ((MAIN_D_80134D04 & 0xf00) == 0x400) {
		if (ip[2] == 0) {
			(*(int32_t *)&MAIN_D_80134CEC) = *(int32_t *)(ip[1] + (int32_t)EFE_INSTANCE);
		} else {
			MAIN_D_80134CEC = *(int32_t *)(ip[1] + MAIN_D_80134D08);
		}
	} else if ((MAIN_D_80134D04 & 0xf00) == 0x200) {
		if (ip[2] == 0) {
			MAIN_D_80134CEC = *(int16_t *)(ip[1] + (int32_t)EFE_INSTANCE);
		} else {
			MAIN_D_80134CEC = *(int16_t *)(ip[1] + MAIN_D_80134D08);
		}
	} else {
		if (ip[2] == 0) {
			MAIN_D_80134CEC = *(int8_t *)(ip[1] + (int32_t)EFE_INSTANCE);
		} else {
			MAIN_D_80134CEC = *(int8_t *)(ip[1] + MAIN_D_80134D08);
		}
	}

	MAIN_D_80134D00 = (int16_t *)((int32_t)MAIN_D_80134D00 + 6);
}

void STD_loadEFEImmediate(void)
{
	MAIN_D_80134CEC = MAIN_D_80134D00[1];
	MAIN_D_80134D00 = (int16_t *)((int32_t)MAIN_D_80134D00 + 6);
}

int32_t STD_shiftRightInt32Variable(int32_t *p)
{
	*p >>= MAIN_D_80134CEC;
}

int32_t STD_shiftLeftInt32Variable(int32_t *p)
{
	*p <<= MAIN_D_80134CEC;
}

int32_t STD_moduloInt32Variable(int32_t *p)
{
	*p %= MAIN_D_80134CEC;
}

int32_t STD_divideInt32Variable(int32_t *p)
{
	*p /= MAIN_D_80134CEC;
}

int32_t STD_multiplyInt32Variable(int32_t *p)
{
	*p *= MAIN_D_80134CEC;
}

int32_t STD_subtractInt32Variable(int32_t *p)
{
	*p -= MAIN_D_80134CEC;
}

int32_t STD_addInt32Variable(int32_t *p)
{
	*p += MAIN_D_80134CEC;
}

int32_t STD_setInt32Variable(int32_t *p)
{
	*p = MAIN_D_80134CEC;
}

int32_t STD_shiftRightInt8Variable(int8_t *p)
{
	*p >>= MAIN_D_80134CEC;
}

int32_t STD_shiftLeftInt8Variable(int8_t *p)
{
	*p <<= MAIN_D_80134CEC;
}

int32_t STD_moduloInt8Variable(int8_t *p)
{
	*p %= MAIN_D_80134CEC;
}

int32_t STD_divideInt8Variable(int8_t *p)
{
	*p /= MAIN_D_80134CEC;
}

int32_t STD_multiplyInt8Variable(int8_t *p)
{
	*p *= MAIN_D_80134CEC;
}

int32_t STD_subtractInt8Variable(int8_t *p)
{
	*p -= MAIN_D_80134CEC;
}

int32_t STD_addInt8Variable(int8_t *p)
{
	*p += MAIN_D_80134CEC;
}

int32_t STD_setInt8Variable(int8_t *p)
{
	*p = MAIN_D_80134CEC;
}

int32_t STD_shiftRightInt16Variable(int16_t *p)
{
	*p >>= MAIN_D_80134CEC;
}

int32_t STD_shiftLeftInt16Variable(int16_t *p)
{
	*p <<= MAIN_D_80134CEC;
}

int32_t STD_moduloInt16Variable(int16_t *p)
{
	*p %= MAIN_D_80134CEC;
}

int32_t STD_divideInt16Variable(int16_t *p)
{
	*p /= MAIN_D_80134CEC;
}

int32_t STD_multiplyInt16Variable(int16_t *p)
{
	*p *= MAIN_D_80134CEC;
}

int32_t STD_subtractInt16Variable(int16_t *p)
{
	*p -= MAIN_D_80134CEC;
}

int32_t STD_addInt16Variable(int16_t *p)
{
	*p += MAIN_D_80134CEC;
}

int32_t STD_setInt16Variable(int16_t *p)
{
	*p = MAIN_D_80134CEC;
}

int32_t STD_compareGreaterOrEqual(int32_t x)
{
	if (x >= MAIN_D_80134CEC) {
		return 0;
	}

	return -1;
}

int32_t STD_compareGreater(int32_t x)
{
	if (x > MAIN_D_80134CEC) {
		return 0;
	}

	return -1;
}

int32_t STD_compareLessOrEqual(int32_t x)
{
	if (x <= MAIN_D_80134CEC) {
		return 0;
	}

	return -1;
}

int32_t STD_compareLess(int32_t x)
{
	if (x < MAIN_D_80134CEC) {
		return 0;
	}

	return -1;
}

int32_t STD_compareNotEqual(int32_t x)
{
	if (x != MAIN_D_80134CEC) {
		return 0;
	}

	return -1;
}

int32_t STD_compareEqual(int32_t x)
{
	if (x == MAIN_D_80134CEC) {
		return 0;
	}

	return -1;
}

int16_t STD_calculateAttackHitPosition(SVECTOR *out, int32_t *self, int32_t *other, int32_t y)
{
	SVECTOR rot;
	MATRIX m;
	char *sp2;
	int32_t dx;
	int32_t dz;

	other = (int32_t *)other[1];
	sp2 = (char *)self[1];
	dx = (int16_t)(*(int32_t *)((char *)other + 0x78) - *(int32_t *)(sp2 + 0x78));
	dz = (int16_t)(*(int32_t *)((char *)other + 0x80) - *(int32_t *)(sp2 + 0x80));
	rot.vx = 0;
	rot.vy = (_atan(dz, dx) + 0x800) & 0xfff;
	rot.vz = 0;
	RotMatrixZYX(&rot, &m);
	out->vx = 0;
	out->vy = -DIGIMON_DATA[self[0]].height;
	out->vz = y;
	ApplyMatrixSV(&m, out, out);
	out->vx += (int16_t)*(int32_t *)((char *)self[1] + 0x78);
	out->vz += (int16_t)*(int32_t *)((char *)self[1] + 0x80);
}

INCLUDE_ASM("asm/std/nonmatchings/std_effect", STD_renderParallelLines);

int32_t STD_func_800770C0(int32_t lo, int32_t hi, int32_t t, int32_t start, int32_t end)
{
	int32_t tmp;

	if (hi < lo) {
		tmp = lo;
		lo = hi;
		hi = tmp;
	}

	t = t < lo ? lo : (hi < t ? hi : t);
	if (lo >= t) {
		return start;
	}

	return start + ((end - start) * (t - lo) / (hi - lo));
}

void STD_initializeEFESubOpcodeTable(void)
{
	int32_t i;

	for (i = 0; (uint32_t)i < 0x61; i++) {
		if ((uint32_t)((int32_t (*)[2])STD_D_8007AB18)[i][0] >= 0x61) {
			exit(1);
		}
		STD_jtbl_8007FA7C[((int32_t (*)[2])STD_D_8007AB18)[i][0]] = (void (*)(void))((void *(*)[2])STD_D_8007AB1C)[i][0];
	}
}

void STD_dispatchEFEOpcode(int32_t op)
{
	STD_jtbl_8007AE20[op]();
}

int32_t STD_runEFEScript(int32_t script)
{
	EFE_DATA_STACK = EFE_SCRIPT_MEM1_DATA;
	MAIN_D_80134CFC = MAIN_D_80139B54;
	MAIN_D_80134D00 = (int16_t *)script;
	EFE_PUSH2(int32_t, 0);
	while (MAIN_D_80134D00 != NULL) {
		MAIN_D_80134D04 = **(int16_t **)&MAIN_D_80134D00;
		STD_jtbl_8007AE20[MAIN_D_80134D04 & 0xff]();
	}

	if (MAIN_D_80134CE8->instance->frame == -1) {
		MAIN_D_80134CE8->inst = NULL;
	}

	return MAIN_D_80134CE4;
}

void STD_resetPoisonBubbles(void)
{
	setShortWithStride(STD_D_8007FC20, -1, 0xc, 0xc);
}

int32_t STD_addPoisonBubble(int32_t arg)
{
	int32_t i;
	int16_t *p;

	for (i = 0; i < 0xc; i++) {
		if (((int16_t (*)[6])STD_D_8007FC20)[i][0] == -1) {
			break;
		}
	}

	if (i == 0xc) {
		return -1;
	}

	p = ((int16_t (*)[6])STD_D_8007FC20)[i];
	p[0] = 0;
	*(int32_t *)&p[4] = arg;
	p[2] = (rand() % 100) - 0x32;
	p[3] = (rand() % 100) - 0x32;
	addObject(0x805, i, STD_tickPoisonBubble, STD_renderPoisonBubble);

	return i;
}

void STD_tickPoisonBubble(int32_t i)
{
	int16_t *p;

	p = &STD_D_8007FC20[i * 6];
	p[0]++;
	if (p[0] >= 0x28) {
		p[0] = -1;
		removeObject(0x805, i);
	}
}

void STD_renderPoisonBubble(int32_t i)
{
	SVECTOR pos;
	DVECTOR screen;
	int16_t *p;
	int16_t frame;
	int32_t otz;
	int32_t d;

	p = &STD_D_8007FC20[i * 6];
	frame = p[0];
	translateConditionFXToEntity((Entity *)*(int32_t *)&p[4], &pos);
	pos.vx += p[2];
	pos.vy -= (int16_t)lerp(0x32, 0xc8, 0, 0x28, frame);
	pos.vz += p[3];
	otz = worldPosToScreenPos(&pos, &screen);
	d = _sin((int16_t)lerp(0, 0x500, 0, 0x28, frame)) * 0x14 / 4096;
	screen.vx += (int16_t)(d * VIEWPORT_DISTANCE / otz);
	if ((otz > 0x200) && (otz < 0x10000)) {
		STD_POISON_BUBBLE_SPRITE.u = MAIN_D_80134914[(frame >> 1) % 6] + 0x20;
		renderSprite(&STD_POISON_BUBBLE_SPRITE, screen.vx, screen.vy, otz, 0x4ea4, 0x4ea4);
	}
}

void STD_tickPoisonEffect(int32_t i)
{
	int16_t *p;

	p = STD_D_8007FCB0[i];
	p[0]++;
	p[0] %= 0x1e;
	if (p[0] == 1) {
		STD_addPoisonBubble(*(int32_t *)&p[2]);
	}
}

void STD_func_80077620(void)
{
}

void STD_initializePoisonBubble(void)
{
	int32_t i;

	for (i = 0; i < 4; i++) {
		STD_D_8007FCB0[i][0] = -1;
	}

	STD_resetPoisonBubbles();
}

int32_t STD_func_80077664(int32_t arg)
{
	int32_t i;
	int16_t *p;

	for (i = 0; i < 4; i++) {
		if (STD_D_8007FCB0[i][0] == -1) {
			break;
		}
	}

	if (i == 4) {
		return -1;
	}

	p = STD_D_8007FCB0[i];
	p[0] = 0;
	*(int32_t *)&p[2] = arg;
	addObject(0x808, i, STD_tickPoisonEffect, STD_func_80077620);

	return i;
}

void STD_removePoisonEffect(int32_t i, int32_t arg)
{
	int16_t *p;

	p = STD_D_8007FCB0[i];
	if ((i >= 0) && (i < 4) && (*(int32_t *)&p[2] == arg)) {
		p[0] = -1;
		removeObject(0x808, i);
	}
}

void STD_removeAllPoisonEffects(void)
{
	int32_t i;

	for (i = 0; i < 4; i++) {
		removeObject(0x808, i);
	}

	for (i = 0; i < 0xc; i++) {
		removeObject(0x805, i);
	}
}

void STD_tickConfusionEffect(int32_t i)
{
	int16_t *p;

	p = STD_D_8007FCD0[i];
	p[1] += 7;
	p[2] += 0x5b;
}

void STD_renderConfusionEffect(int32_t idx)
{
	SVECTOR pos;
	GsCOORDINATE2 coord;
	SVECTOR rot;
	VECTOR trans;
	int32_t ang;
	int16_t *p;
	int32_t i;
	int32_t a;
	int32_t b;

	p = STD_D_8007FCD0[idx];
	for (i = 0, a = 0, b = 0; i < 3; i++, b += 0x555, a += 0xaa) {
		translateConditionFXToEntity((Entity *)((int32_t *)p)[3], &pos);
		ang = p[1] + a;
		trans.vx = pos.vx + (_sin(ang) * 0x78 / 4096);
		trans.vy = pos.vy - 0x78;
		trans.vz = pos.vz + (_cos(ang) * 0x78 / 4096);
		rot.vx = 0;
		rot.vy = (p + i)[3] + (p[2] - 0x400 + b);
		rot.vz = 0xe3;
		renderTMDModel((uint8_t *)MAIN_D_801351C8, 0, &coord, NULL, &trans, &rot, &STD_D_8007AF78);
	}
}

void STD_initializeConfusionEffect(char *base)
{
	MAIN_D_801351C8 = (int32_t)base;
	GsMapModelingData((unsigned long *)((char *)MAIN_D_801351C8 + 4));
	setShortWithStride((int16_t *)STD_D_8007FCD0, -1, 4, 0x10);
}

int32_t STD_addConfusionEffect(int32_t arg)
{
	int32_t i;
	int16_t *p;

	for (i = 0; i < 4; i++) {
		if (STD_D_8007FCD0[i][0] == -1) {
			break;
		}
	}

	if (i == 4) {
		return -1;
	}

	p = STD_D_8007FCD0[i];
	p[0] = 0;
	p[1] = 0;
	p[2] = 0;
	*(int32_t *)&p[6] = arg;
	p[3] = rand();
	p[4] = rand();
	p[5] = rand();
	p[2] = 0;
	addObject(0x806, i, STD_tickConfusionEffect, STD_renderConfusionEffect);

	return i;
}

void STD_removeConfusionEffect(int32_t i, int32_t arg)
{
	int16_t *p;

	p = STD_D_8007FCD0[i];
	if ((i >= 0) && (i < 4) && (*(int32_t *)&p[6] == arg)) {
		p[0] = -1;
		removeObject(0x806, i);
	}
}

void STD_initializeStunEffect(char *base)
{
	int32_t i;

	for (i = 0; i < 5; i++) {
		STD_D_8007FD10[i][0] = -1;
	}

	STD_resetStunSubEffects();
	MAIN_D_801351CC = base;
	GsMapModelingData((unsigned long *)(base + 4));
}

void STD_resetStunSubEffects(void)
{
	int32_t i;

	for (i = 0; i < 0x19; i++) {
		STD_D_8007FD4C[i][0] = -1;
	}
}

void STD_tickStunEffect(int32_t i)
{
	int16_t *p;

	p = STD_D_8007FD10[i];
	p[0] = p[0] + 1;
	switch (p[2]) {
	case 0:
		if ((p[0] % 6) == 0) {
			STD_func_80078044(*(int32_t *)&p[4]);
		}
		if (p[0] >= p[1]) {
			p[2] = 1;
			p[0] = 1;
		}
		break;
	case 1:
		if (p[0] >= 8) {
			p[2] = 2;
			p[0] = 1;
		}
		break;
	case 2:
		if (p[0] >= 5) {
			STD_removeStunEffect(i, *(int32_t *)&p[4]);
		}
		break;
	}
}

INCLUDE_ASM("asm/std/nonmatchings/std_effect", STD_renderStunEffect);

void STD_removeAllStunSubEffects(void)
{
	int32_t i;

	for (i = 0; i < 0x19; i++) {
		STD_D_8007FD4C[i][0] = -1;
		removeObject(0x810, i);
	}
}

int32_t STD_func_80078044(int32_t arg)
{
	int32_t i;
	int16_t *p;

	for (i = 0; i < 0x19; i++) {
		if (STD_D_8007FD4C[i][0] == -1) {
			break;
		}
	}

	if (i == 0x19) {
		return -1;
	}

	p = STD_D_8007FD4C[i];
	p[0] = 0;
	*(int32_t *)&p[2] = arg;
	addObject(0x810, i, STD_tickStunSubEffect, STD_renderStunSubEffect);

	return i;
}

void STD_tickStunSubEffect(int32_t i)
{
	int16_t *p;

	p = STD_D_8007FD4C[i];
	p[0]++;
	if (p[0] >= 0x10) {
		p[0] = -1;
		removeObject(0x810, i);
	}
}

void STD_renderStunSubEffect(int32_t i)
{
	GsCOORDINATE2 coord;
	VECTOR trans;
	SVECTOR rot;
	VECTOR scale;
	int16_t *p;
	int32_t z;
	int32_t y;
	int32_t x;
	int32_t s;

	p = STD_D_8007FD4C[i];
	x = (int16_t)((Entity **)p)[1]->posData->location.vx;
	y = (int16_t)lerp(-DIGIMON_DATA[((Entity **)p)[1]->type].height * 113 / 100, ((Entity **)p)[1]->posData->location.vy, 0, 0xf, p[0]);
	z = (int16_t)((Entity **)p)[1]->posData->location.vz;
	s = DIGIMON_DATA[((Entity **)p)[1]->type].radius * 0x4000 / 350;
	if (p[0] < 4) {
		s = lerp(s * 10 / 100, s, 0, 4, p[0]);
	}

	rot = MAIN_D_8013491C;
	scale = STD_D_8007B018;
	trans.vx = x;
	trans.vy = y;
	trans.vz = z;
	scale.vx = scale.vz = s;
	renderTMDModel((uint8_t *)MAIN_D_801351CC, 0, &coord, NULL, &trans, &rot, &scale);
}

int32_t STD_addStunEffect(int32_t arg, int32_t val)
{
	int32_t i;
	int16_t *p;

	for (i = 0; i < 5; i++) {
		if (STD_D_8007FD10[i][0] == -1) {
			break;
		}
	}

	if (i == 5) {
		return -1;
	}

	p = STD_D_8007FD10[i];
	p[0] = 0;
	p[1] = val;
	p[2] = 0;
	*(int32_t *)&p[4] = arg;
	addObject(0x80f, i, STD_tickStunEffect, STD_renderStunEffect);

	return i;
}

void STD_removeStunEffect(int32_t i, int32_t arg)
{
	int16_t *p;

	p = STD_D_8007FD10[i];
	if ((i >= 0) && (i < 5) && (*(int32_t *)&p[4] == arg)) {
		p[0] = -1;
		removeObject(0x80f, i);
	}
}

void STD_removeAllStunEffects(void)
{
	int32_t i;

	STD_removeAllStunSubEffects();
	for (i = 0; i < 5; i++) {
		STD_D_8007FD10[i][0] = -1;
		removeObject(0x80f, i);
	}
}

void STD_setTMDObjectColor(int32_t idx, int32_t *color, char *base)
{
	int32_t *rec;
	int32_t i;
	int32_t count;
	int32_t t;

	char (*pr)[0x20];
	char (*pg)[0x20];
	char (*pb)[0x20];

	rec = (int32_t *)((int32_t)((uint32_t)base + 0xc) + (idx * 28));
	count = rec[5];
	rec = (int32_t *)rec[4];
	pr = (char (*)[0x20])((char *)rec + 0x14);
	pg = (char (*)[0x20])((char *)rec + 0x15);
	pb = (char (*)[0x20])((char *)rec + 0x16);
	for (i = 0; i < count; i++) {
		t = (*rec >> 24) & 0xff;
		if ((t == 0x2f) || (t == 0x2d)) {
			(*pr)[0] = (int16_t)color[0];
			(*pg)[0] = (int16_t)color[1];
			(*pb)[0] = (int16_t)color[2];
			rec = (int32_t *)((int32_t)rec + 0x20);
			pr++;
			pg++;
			pb++;
		}
	}
}

void STD_tickFinisherAura(int32_t i)
{
	int16_t *p;

	p = &STD_D_8007FE14[i * 4];
	p[0]++;
	if (p[0] > p[1]) {
		removeObject(0x80d, i);
		p[0] = -1;
	}
}

INCLUDE_ASM("asm/std/nonmatchings/std_effect", STD_renderFinisherAura);

void STD_func_80078BBC(char *pos, int32_t scale, SVECTOR *dir, uint8_t *col)
{
	POLY_FT4 *prim;
	SVECTOR c;
	SVECTOR a;
	SVECTOR b;

	prim = (POLY_FT4 *)GsGetWorkBase();
	MAIN_func_80092B60(prim);
	SetSemiTrans(prim, 1);
	prim->r0 = col[0];
	prim->g0 = col[1];
	prim->b0 = col[2];
	prim->tpage = 0x3c;
	prim->clut = 0x7a4c;
	prim->u0 = 0x30;
	prim->v0 = 0xa8;
	prim->u1 = 0x37;
	prim->v1 = 0xa8;
	prim->u2 = 0x30;
	prim->v2 = 0xaf;
	a.vx = dir[0].vx * scale / 4096;
	a.vy = dir[0].vy * scale / 4096;
	a.vz = dir[0].vz * scale / 4096;
	b.vx = dir[1].vx * scale / 4096;
	b.vy = dir[1].vy * scale / 4096;
	b.vz = dir[1].vz * scale / 4096;
	c.vx = ((int32_t *)pos)[0];
	c.vy = ((int32_t *)pos)[1];
	c.vz = ((int32_t *)pos)[2];
	a.vx += (int16_t)((int32_t *)pos)[0];
	a.vy += (int16_t)((int32_t *)pos)[1];
	a.vz += (int16_t)((int32_t *)pos)[2];
	b.vx += (int16_t)((int32_t *)pos)[0];
	b.vy += (int16_t)((int32_t *)pos)[1];
	b.vz += (int16_t)((int32_t *)pos)[2];
	setSemiTrans(prim, 1);
	addScreenPolyFT3(prim, &c, &a, &b);
}

void STD_initializeFinisherAuraModel(char *tim, char *base)
{
	MATRIX m;
	SVECTOR v;
	SVECTOR rot;
	GsIMAGE image;
	RECT rect;
	VECTOR ca;
	VECTOR cb;
	int32_t j;
	int32_t i;
	SVECTOR *p;

	GsGetTimInfo((unsigned long *)tim + 1, &image);
	rect.x = image.px;
	rect.y = image.py;
	rect.w = image.pw;
	rect.h = image.ph;
	LoadImage(&rect, image.pixel);
	GetTPage(image.pmode & 3, 0, image.px, image.py);
	if (((image.pmode >> 3) & 1) != 0) {
		rect.x = image.cx;
		rect.y = image.cy;
		rect.w = image.cw;
		rect.h = image.ch;
		LoadImage(&rect, image.clut);
		GetClut(image.cx, image.cy);
	}

	MAIN_D_801351D0 = (int32_t)base;
	GsMapModelingData((unsigned long *)((char *)MAIN_D_801351D0 + 4));
	ca = STD_D_8007B028;
	cb = STD_D_8007B038;
	STD_setTMDObjectColor(0, (int32_t *)&ca, (char *)MAIN_D_801351D0);
	STD_setTMDObjectColor(1, (int32_t *)&ca, (char *)MAIN_D_801351D0);
	STD_setTMDObjectColor(2, (int32_t *)&ca, (char *)MAIN_D_801351D0);
	STD_setTMDObjectColor(3, (int32_t *)&cb, (char *)MAIN_D_801351D0);
	STD_setTMDObjectColor(4, (int32_t *)&ca, (char *)MAIN_D_801351D0);

	for (j = 0; j < 2; j++) {
		((int16_t (*)[4])STD_D_8007FE14)[j][0] = -1;
	}

	p = STD_D_8007FE24;
	for (i = 0; i < 0xa; i++) {
		v.vx = (rand() % 30) + 30;
		v.vy = 0;
		v.vz = (rand() % 0x190) + 0x320;
		rot.vx = rand() % 0x1000 * 180 / 360;
		rot.vy = rand();
		rot.vz = 0;
		RotMatrixZYX(&rot, &m);
		ApplyMatrixSV(&m, &v, p++);
		v.vx = -v.vx;
		ApplyMatrixSV(&m, &v, p++);
	}
}

int32_t STD_addFinisherAura(int32_t arg, int32_t val)
{
	int32_t i;
	int16_t *p;

	for (i = 0; i < 2; i++) {
		if (((int16_t (*)[4])STD_D_8007FE14)[i][0] < 0) {
			break;
		}
	}

	if (i == 2) {
		return -1;
	}

	p = &STD_D_8007FE14[i * 4];
	p[0] = 0;
	p[1] = val;
	*(int32_t *)&p[2] = arg;
	addObject(0x80d, i, STD_tickFinisherAura, STD_renderFinisherAura);

	return i;
}

void STD_removeFinisherAura(int32_t i)
{
	removeObject(0x80d, i);
	STD_D_8007FE14[i * 4] = -1;
}

void STD_func_800791E0(void)
{
	int32_t i;

	for (i = 0; i < 2; i++) {
		STD_removeFinisherAura(i);
	}
}

void STD_tickAuraProjectile(int32_t id)
{
	AABB box;
	EfeAura *a;
	int32_t hit;
	uint32_t idx;
	int32_t j;
	Entity *e;
	int32_t ent;

	a = &((EfeAura *)MAIN_D_801351D4)[id];
	a->frame++;
	if (a->frame >= 0x3a) {
		a->frame = -1;
		removeObject(0x179, (int16_t)id);
		return;
	}
	a->position.vx += a->velocity.vx;
	a->position.vy += a->velocity.vy;
	a->position.vz += a->velocity.vz;
	box.center = &a->position;
	box.extent.vx = 0x2d;
	box.extent.vy = 0xc8;
	box.extent.vz = 0x2d;
	hit = findAABBHitEntity(&box, a->owner, 1);
	idx = hit;
	if (hit == -1) {
		return;
	}
	e = ENTITY_TABLE[idx];
	if (((int8_t *)e)[0x53] != 0) {
		return;
	}
	for (j = 1; j < 10; j++) {
		ent = (int32_t)ENTITY_TABLE[j];
		if (ent == (int32_t)a->owner) {
			break;
		}
	}
	((int8_t *)e)[0x53] = 1;
	addAttackObject(idx, 1, (int16_t *)&a->position, 0x179, 0, j);
	a->frame = -1;
	removeObject(0x179, (int16_t)id);
}
void STD_renderAuraProjectile(int32_t i)
{
	MATRIX m;
	SVECTOR a;
	SVECTOR b;
	SVECTOR c;
	SVECTOR d;
	char *p;
	POLY_FT4 *prim;

	p = MAIN_D_801351D4 + (i * 36);
	prim = (POLY_FT4 *)GsGetWorkBase();
	RotMatrix((SVECTOR *)(p + 0xc), &m);
	ApplyMatrixSV(&m, &MAIN_D_80134938, &a);
	ApplyMatrixSV(&m, &MAIN_D_80134940, &b);
	ApplyMatrixSV(&m, &MAIN_D_80134948, &c);
	ApplyMatrixSV(&m, &MAIN_D_80134950, &d);
	a.vx += ((int16_t *)p)[2];
	a.vy += ((int16_t *)p)[3];
	a.vz += ((int16_t *)p)[4];
	b.vx += ((int16_t *)p)[2];
	b.vy += ((int16_t *)p)[3];
	b.vz += ((int16_t *)p)[4];
	c.vx += ((int16_t *)p)[2];
	c.vy += ((int16_t *)p)[3];
	c.vz += ((int16_t *)p)[4];
	d.vx += ((int16_t *)p)[2];
	d.vy += ((int16_t *)p)[3];
	d.vz += ((int16_t *)p)[4];
	SetPolyFT4(prim);
	setSemiTrans(prim, 1);
	prim->r0 = prim->g0 = prim->b0 = 0x80;
	prim->tpage = 0x3c;
	prim->clut = 0x7b0c;
	prim->u0 = ((int16_t *)((char **)p)[7])[5] + 0x60;
	prim->v0 = 0xa0;
	prim->u1 = ((int16_t *)((char **)p)[7])[5] + 0x67;
	prim->v1 = 0xa0;
	prim->u2 = ((int16_t *)((char **)p)[7])[5] + 0x60;
	prim->v2 = 0xa7;
	prim->u3 = ((int16_t *)((char **)p)[7])[5] + 0x67;
	prim->v3 = 0xa7;
	add3DSpritePrim(prim, &a, &b, &c, &d);
}

char *STD_initializeAuraProjectiles(char *base)
{
	int32_t i;
	int32_t off;

	base = (char *)((int32_t)base + (4 - ((int32_t)base & 3)));
	MAIN_D_801351D4 = base;
	base = (char *)((int32_t)base + 0x120);
	for (i = 0, off = 0; i < 8; i++, off += 0x24) {
		*(int32_t *)(MAIN_D_801351D4 + off) = -1;
	}

	return base;
}

int32_t STD_addAuraProjectile(Entity *e)
{
	EfeAura *p;
	MATRIX *q;
	char *tbl;
	int32_t i;

	p = (EfeAura *)MAIN_D_801351D4;
	tbl = (char *)STD_D_8007B058 + (getOriginalType(e->type) * 12);
	for (i = 0; i < 8; i++) {
		if (p[i].frame < 0) {
			break;
		}
	}

	if (i == 8) {
		return -1;
	}

	p = (EfeAura *)((int32_t)p + (i * 36));
	p->frame = 0;
	p->owner = e;
	p->typeData = tbl;
	q = (MATRIX *)((char *)e->posData + 0x34);
	p->position.vx = 0;
	p->position.vy = -DIGIMON_DATA[e->type].height * 50 / 100;
	p->position.vz = -DIGIMON_DATA[e->type].height * 50 / 100;
	ApplyMatrixSV(q, &p->position, &p->position);
	p->position.vx += (int16_t)q->t[0];
	p->position.vy += (int16_t)q->t[1];
	p->position.vz += (int16_t)q->t[2];
	p->rotation.vx = 0;
	p->rotation.vy = *(int16_t *)((char *)e->posData + 0x72);
	p->rotation.vz = 0;
	p->velocity.vx = 0;
	p->velocity.vy = 0;
	p->velocity.vz = -0x23;
	ApplyMatrixSV(q, &p->velocity, &p->velocity);
	addObject(0x179, (int16_t)i, STD_tickAuraProjectile, STD_renderAuraProjectile);

	return i;
}

void STD_func_80079874(void)
{
	int32_t i;
	int32_t off;

	for (i = 0, off = 0; i < 8; i++, off += 0x24) {
		if (*(int32_t *)(MAIN_D_801351D4 + off) >= 0) {
			*(int32_t *)(MAIN_D_801351D4 + off) = -1;
			removeObject(0x179, (int16_t)i);
		}
	}
}
