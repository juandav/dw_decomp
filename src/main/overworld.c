#include <mwinline_n.h>

#include <dw/anim.h>
#include <dw/clock.h>
#include <dw/evl.h>
#include <dw/fish.h>
#include <dw/item.h>
#include <dw/map.h>
#include <dw/model.h>
#include <dw/move.h>
#include <dw/partner.h>
#include <dw/std.h>
#include <dw/sound.h>
#include <dw/tamer.h>
#include <dw/ui.h>

typedef struct {
	int16_t typeId;
	int16_t padding0;
	VECTOR waypoints[8];
	int16_t aiSections[8];
	int16_t activeSection;
	int16_t padding2;
	VECTOR targetLocation;
	int16_t posX;
	int16_t posY;
	int16_t posZ;
	int16_t rotX;
	int16_t rotY;
	int16_t rotZ;
	int16_t trackingRange;
	int16_t targetAngle;
	int16_t ccDiff;
	int16_t cwDiff;
	int8_t followMode;
	int8_t waypointWaitTimer;
	int8_t animation;
	int8_t hasWaypointTarget;
	int8_t lookAtTamerState;
	int8_t stopAnim;
	uint8_t pad[2];
} MapDigimonEntity;

typedef struct {
	int16_t orderValue;
	int16_t x;
	int16_t y;
	int16_t animSprites[8];
	uint8_t animTimes[8];
	uint8_t timer;
	uint8_t pad;
	int8_t currentFrame;
	int8_t flag;
} LocalMapObjectInstance;

typedef struct {
	int16_t texX;
	int16_t texY;
	int16_t someX;
	int16_t someY;
	int16_t someZ;
	uint8_t width;
	uint8_t height;
	int8_t clut;
	int8_t transparency;
} LocalMapObject;

extern int32_t IS_IN_MENU;
extern int32_t IS_SCRIPT_PAUSED;
extern uint16_t CURRENT_SCRIPT_ID;
extern int32_t LOADED_DIGIMON_MODELS[8];
extern int8_t GAME_STATE;
extern int8_t MAIN_D_80134D20[8];
extern MapDigimonEntity MAP_DIGIMON_TABLE[];
extern int16_t NPC_COLLISION_STATE[];
extern int32_t NPC_IS_WALKING_TOWARDS[];
extern int8_t TALKED_TO_ENTITY;
extern int16_t MIST_CLUT_Y[2];
extern int8_t NPC_ACTIVE_ANIM;
extern LocalMapObjectInstance LOCAL_MAP_OBJECT_INSTANCE[];
extern int16_t MAIN_D_8013CA38[];
extern int16_t MAIN_D_8013CA8C[];
extern int8_t MAIN_D_8013CAE0[];
extern int8_t MAIN_D_8013CB0C[];
extern int16_t MAP_OBJECT_INSTANCE_COUNT;
extern LocalMapObject LOCAL_MAP_OBJECTS[];
extern int16_t CAMERA_X[];
extern int16_t CAMERA_Y[];
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
extern int32_t MAP_OBJECT_MOVE_TO_DATA[];
extern int8_t MAIN_D_8013CB38[];
extern int8_t MAIN_D_8013CB44[];
extern uint8_t MAP_LAYER_ENABLED;
extern uint8_t CURRENT_SCREEN;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern int16_t CAMERA_X_PREVIOUS;
extern int16_t CAMERA_Y_PREVIOUS;
extern int16_t MIST_X_OFFSETS[4];
extern int16_t MIST_Y_OFFSETS[2];
extern int16_t MAIN_D_80134226;
extern int16_t MAIN_D_80134228;
extern int16_t MAIN_D_8013422A;
extern int16_t MAIN_D_8013422E;
extern int8_t MENU_SUB_STATE;
extern int16_t MAIN_D_80123E88[];
extern int16_t MAIN_D_80123E8A[];
extern int16_t MAIN_D_80123E8C[5];
extern char *MAIN_D_80124800[];
extern int32_t TRIANGLE_MENU_STATE;
extern int8_t MAIN_D_80123E8E[];
extern RECT MAIN_D_80134290;
extern char MAIN_D_80134298[8];
extern int32_t MAIN_D_80134D2C;
extern int8_t MENU_STATE;
extern int8_t MAIN_D_80134D36;
extern int8_t MAIN_D_80134D37;
extern int16_t MAIN_D_80134D38;
extern int16_t MAIN_D_80134D3A;
extern uint8_t MAIN_D_80134234[4];
extern int32_t CHANGED_INPUT;
typedef struct {
  uint8_t digimon;
  uint8_t moves;
} MenuTabPair;

extern uint8_t MAIN_D_801342A0[2];
typedef struct {
	int8_t tab[4];
} PlayerTabs;

extern PlayerTabs MAIN_D_801342A4;
extern char *MAIN_D_801247B8[];
extern uint8_t GAME_MENU_SPRITES[];
extern uint8_t GAME_MENU_LABELS[];
extern int16_t GAME_MENU_LINES[];
extern uint8_t INVENTORY_POINTER;
extern uint32_t POLLED_INPUT;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern int32_t MAIN_D_80134D28;
extern uint8_t MAIN_D_80123F48[];

typedef struct {
	uint8_t data[8];
} TriangleCursorUVData;

typedef struct {
	int8_t data[8];
} TriangleCursorOffsetData;

typedef struct {
	int16_t x;
	int16_t y;
	int16_t unknown;
	int8_t disabled;
	int8_t clutY;
	uint8_t width;
	uint8_t height;
	uint8_t texX;
	uint8_t texY;
} GameMenuSprite;

typedef struct {
	int16_t x;
	int16_t y;
	uint8_t width;
	uint8_t height;
	uint8_t texX;
	uint8_t texY;
} GameMenuLabel;

typedef struct {
	int16_t posX;
	int16_t posY;
	uint8_t u;
	uint8_t v;
	uint8_t clut;
	uint8_t pad;
} EvoChartEntry;

extern TriangleCursorUVData MAIN_D_80134250;
extern TriangleCursorUVData MAIN_D_80134258;
extern TriangleCursorUVData MAIN_D_80134260;
extern TriangleCursorUVData MAIN_D_80134268;
extern TriangleCursorOffsetData MAIN_D_80134270;
extern TriangleCursorOffsetData MAIN_D_80134278;
extern TriangleCursorOffsetData MAIN_D_80134280;
extern TriangleCursorOffsetData MAIN_D_80134288;
extern EvoChartEntry MAIN_D_80124544[];
extern EvoChartEntry MAIN_D_80124546[];
extern int8_t SELECTED_MEDAL;
extern int8_t MEDAL_SELECTOR_INDEX;
extern int8_t SELECTED_CARD;
extern int16_t MAIN_D_80134D40;
extern int16_t MAIN_D_80134D42;
extern int16_t MAIN_D_80134D44;
extern int8_t MAIN_D_80134D46;
extern int16_t MAIN_D_80134D48;
extern char *MOVE_NAMES[];

void clearTextSubArea(RECT *area);
int32_t isTriggerSet(int32_t triggerId);
void callScriptSection(uint16_t scriptId, uint32_t scriptSection,
		       uint32_t param);
void startNPCAnimation(uint8_t scriptId, int32_t animId);
void drawString(char *text, int32_t color, int32_t pos);
void drawInventoryText(void);
void closeTriangleMenu(void);
void closeInventoryBoxes2();
void renderDigimonMovesView(void);
void renderDigimonStatsView(void);
void renderCardsView(void);
void renderMedalView(void);
void renderEvoChartView(void);
void renderPlayerInfoView(void);
void renderString();
void renderMenuTab(int32_t a, int32_t b, int32_t c);

void setCameraFollowPlayer(void);
void handleGameMenuSelection(int32_t selection);
int32_t createMenuBox(int32_t id, int16_t x, int16_t y, int16_t width,
		      int16_t height, int8_t features, void (*tick)(void),
		      void (*render)(void));
void closeUIBoxIfOpen(int32_t arg);
void getEntityScreenPos(Entity *entity, int32_t flag, int16_t *outPos);
void initializeInventoryObject(void);
void clearTextArea(void);
void tickGameMenu(void);
void renderGameMenu(void);
void tickDigimonMenu(void);
void renderDigimonMenu(void);
void tickPlayerMenu(void);
void renderPlayerMenu(void);
void tickTriangleMenu(void);
void renderRectPolyFT4(int16_t posX, int16_t posY, int32_t width,
		       int32_t height, uint8_t texX, uint8_t texY,
		       int16_t texturePage, int16_t clut, int32_t zIndex,
		       int8_t flag);
void renderSeperatorLines(int16_t *lines, int8_t count, int32_t zIndex);
void renderMist(void);
void renderMapOverlays(LocalMapObjectInstance *instances, int32_t screenX,
		       int16_t screenY);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY,
		       int32_t width, int32_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t uPos, int32_t vPos,
		      int32_t width, int32_t height);
void buildMapOverlayPrim(POLY_FT4 *prim, LocalMapObjectInstance *inst,
			 LocalMapObject *obj, int32_t arg3, int16_t arg4,
			 int8_t mode);
void buildSnowflakePrim(POLY_FT4 *prim, LocalMapObjectInstance *inst,
			LocalMapObject *obj);
void tickNPCBattle(int32_t instanceId);
void tickWaypointAI(MapDigimonEntity *mapDigimon, Entity *entity,
		    int32_t instanceId);
void tickWaypointWait(MapDigimonEntity *mapDigimon, Entity *entity);
void tickWaypointWalk(MapDigimonEntity *mapDigimon, Entity *entity,
		      int32_t animation, int32_t instanceId);
int32_t isInTrackingRect(MapDigimonEntity *mapDigimon, VECTOR *location);
void tickLookingAtTamer(MapDigimonEntity *mapDigimon, Entity *entity,
			TamerEntity *tamer);
void clearMapObjects(LocalMapObjectInstance *instances);
void loadMapObjects(LocalMapObjectInstance *mapObjects, uint8_t *data,
		    int32_t mapId);
void getRotationDifference(PositionData *posData, VECTOR *targetLoc,
			   int16_t *outAngle, int16_t *outCcDiff,
			   int16_t *outCwDiff);
int32_t rotateEntity(SVECTOR *rotation, int16_t *targetAngle, int16_t *ccDiff,
		     int16_t *cwDiff, int16_t speed);
void getModelTile(VECTOR *position, int16_t *outTileX, int16_t *outTileY);
void tickTrackingTamer(MapDigimonEntity *mapDigimon, Entity *entity,
		       TamerEntity *tamer, int32_t instanceId);
void tickTrackingTamer3(MapDigimonEntity *mapDigimon, Entity *entity,
			int32_t instanceId);
int32_t isInTrackingRadius(Entity *entity, Entity *otherEntity,
			   MapDigimonEntity *mapDigimon);
void tickTrackingTamer2(MapDigimonEntity *mapDigimon, Entity *entity,
			TamerEntity *tamer, int32_t instanceId,
			uint8_t animId);
void tickTrackingTamer4(MapDigimonEntity *mapDigimon, Entity *entity,
			TamerEntity *tamer, int32_t instanceId);
int32_t entityCheckCollision(Entity *a, Entity *entity, int32_t c, int32_t d);
void removeTriangleMenu(void);
void closeInventoryBoxes(void);
void removeUIBox1(void);
void collisionGrace(int32_t a, Entity *entity, int32_t c, int32_t d);
uint8_t entityIsOffScreen(Entity *entity, int32_t w, int32_t h);
void tickNPC(int32_t instanceId);
void tickNPCOverworld(int32_t instanceId, MapDigimonEntity *mapDigimon);
void scriptUnloadEntity(uint8_t scriptId);
void setLoopCountToOne(uint32_t scriptId);
void loadNPCModel(int32_t digimonId);
void unloadDigimonModel(int32_t digimonType);
void setPartnerIdling(void);
int32_t tickRemoveMist(void);
void setActiveAnim(uint8_t scriptId, uint8_t animId);
void spawnSpriteAtEntity(int32_t scriptId, int32_t nodeId, uint8_t sprite);
void spawnSpriteAtLocation(int16_t x, int16_t y, int16_t z, int16_t sprite,
			   int16_t flag);
void loadMapImage1(u_long *tim);
void loadMapImage2(u_long *tim, int32_t id);
void renderNinjamonEffect(int32_t instanceId);
int32_t random(int32_t max);
int32_t _atan(int32_t dy, int32_t dx);
void createNinjamonEffect(void);
void calcMapObjectOrder(LocalMapObjectInstance *instances);
void getDrawPosition(SVECTOR *worldPos, int16_t *outX, int16_t *outY);
void storeMapObjectPosition(int16_t *outX, int16_t *outY, uint8_t a,
			    int32_t count);
void loadMapObjectPosition(int16_t *xData, int16_t *yData, int16_t startIndex,
			   int32_t count);
void moveMapObjects(uint8_t startIndex, int32_t count, int16_t dx,
		    int16_t dy);
int32_t moveMapObjectsWithLimit(uint8_t startIndex, int32_t count, int16_t dx,
				int16_t dy, int16_t limitX, int16_t limitY);
void setMapObjectsFlag(uint8_t start, int32_t count, int32_t flag);
void resetMapObjectAnimation(uint8_t startIndex, int32_t count);
void clearMapAITable(int32_t index);
void removeMapEntities(void);
void clearMapDigimon(void);
void resetEntityOrigin(int32_t scriptId);
void loadMapDigimon(int16_t *data, int32_t mapId);
void initializeLoadedNPCModels(void);
int32_t scriptSetDigimon(uint8_t type, uint8_t slot, uint8_t autotalk);
int32_t tickMoveObjectTo(uint8_t objectIndex, uint8_t moveIndex,
			 int8_t steps, int16_t targetX, int16_t targetY);
void setMovementEnabled(int32_t id, int32_t enabled);
void addGameMenu(void);
void renderDateDigits(void);
void renderTriangleCursor(int32_t selection, int16_t yOffset);
int32_t isUIBoxAvailable(int32_t id);
void setSleepDisabled(int32_t arg);
void startFeedingItem(int32_t arg);
void removeOneSelectedItem(void);
void renderFeedingItem(int32_t arg);
int32_t getEquippedSlot(void);
void equipMove(void);
int32_t isKeyDown(int32_t mask);
void convertValueToDigits(int32_t digits, int32_t value, int32_t *outCount,
			  int32_t *outDigits);
void renderLinePrimitive(int32_t color, int32_t x0, int32_t y0, int32_t x1,
			 int32_t y1, int32_t zIndex, int32_t flag);
void renderTrianglePrimitive(int32_t color, int32_t x0, int32_t y0,
			     int32_t x1, int32_t y1, int32_t x2,
			     int32_t y2, int32_t zIndex, int32_t flag);
uint8_t entityGetTechFromAnim(Entity *entity, int32_t anim);
int32_t hasDigimonRaised(int32_t digimonId);
int32_t hasMedal(uint16_t medal);
int32_t getCardAmount(int32_t cardId);
void activateMedalTexture(int32_t medalId, int32_t previousMedalId);
int32_t loadCardImage(int32_t id);
void renderEvoChartDetail(void);
void renderCardImage(void);
void renderCardCount(void);
int32_t hasMove(int32_t moveId);

static void *overworld_functions[] = {
	equipMove,
	getEquippedSlot,
	renderFeedingItem,
	removeOneSelectedItem,
	startFeedingItem,
	setSleepDisabled,
	handleGameMenuSelection,
	isUIBoxAvailable,
	renderPlayerMenu,
	tickPlayerMenu,
	renderDigimonMenu,
	tickDigimonMenu,
	createMenuBox,
	tickGameMenu,
	renderRectPolyFT4,
	renderTriangleCursor,
	renderDateDigits,
	renderSeperatorLines,
	renderGameMenu,
	closeUIBoxIfOpen,
	closeTriangleMenu,
	tickTriangleMenu,
	addGameMenu,
	setPartnerIdling,
	setMovementEnabled,
	resetEntityOrigin,
	setLoopCountToOne,
	startNPCAnimation,
	setActiveAnim,
	tickWaypointWalk,
	tickWaypointWait,
	tickTrackingTamer4,
	tickTrackingTamer3,
	rotateEntity,
	getRotationDifference,
	tickTrackingTamer2,
	isInTrackingRadius,
	tickTrackingTamer,
	tickLookingAtTamer,
	isInTrackingRect,
	tickWaypointAI,
	tickNPCOverworld,
	clearMapAITable,
	removeMapEntities,
	clearMapDigimon,
	unloadDigimonModel,
	scriptUnloadEntity,
	tickNPC,
	scriptSetDigimon,
	loadNPCModel,
	loadMapDigimon,
	tickRemoveMist,
	resetMapObjectAnimation,
	spawnSpriteAtEntity,
	spawnSpriteAtLocation,
	getDrawPosition,
	setMapObjectsFlag,
	moveMapObjects,
	tickMoveObjectTo,
	moveMapObjectsWithLimit,
	loadMapObjectPosition,
	renderNinjamonEffect,
	storeMapObjectPosition,
	createNinjamonEffect,
	buildMapOverlayPrim,
	buildSnowflakePrim,
	renderMist,
	renderMapOverlays,
	calcMapObjectOrder,
	loadMapImage2,
	loadMapImage1,
	loadMapObjects,
	clearMapObjects,
};

void clearMapObjects(LocalMapObjectInstance *instances)
{
	LocalMapObjectInstance *data;
	int32_t i;
	int32_t j;

	data = instances;
	for (i = 0; i < 188; i++) {
		for (j = 0; j < 8; j++) {
			data->animSprites[j] = -1;
		}
		for (j = 0; j < 8; j++) {
			data->animTimes[j] = 0;
		}
		data->orderValue = 0;
		data->timer = 0;
		data->currentFrame = -1;
		data->flag = 1;
		data++;
	}
}

void loadMapObjects(LocalMapObjectInstance *mapObjects, uint8_t *data,
		    int32_t mapId)
{
	LocalMapObjectInstance *obj;
	int16_t *src;
	int32_t i;
	int32_t j;
	int32_t k;
	int16_t count;

	obj = mapObjects;
	src = (int16_t *)data;
	count = *src++;
	for (i = 0; i < count; i++) {
		LOCAL_MAP_OBJECTS[i].texX = *src++;
		LOCAL_MAP_OBJECTS[i].texY = *src++;
		LOCAL_MAP_OBJECTS[i].width = *src++;
		LOCAL_MAP_OBJECTS[i].height = *src++;
		LOCAL_MAP_OBJECTS[i].someX = *src++;
		LOCAL_MAP_OBJECTS[i].someY = *src++;
		LOCAL_MAP_OBJECTS[i].someZ = *src++;
		LOCAL_MAP_OBJECTS[i].clut = *src++;
		LOCAL_MAP_OBJECTS[i].transparency = *src++;
	}
	MAP_OBJECT_INSTANCE_COUNT = *src++;
	for (k = 0; k < MAP_OBJECT_INSTANCE_COUNT; k++) {
		for (j = 0; j < 8; j++) {
			obj->animSprites[j] = *src++;
		}
		for (j = 0; j < 8; j++) {
			obj->animTimes[j] = *src++;
		}
		obj->x = *src++;
		obj->y = *src++;
		obj->flag = *src++;
		if (((mapId >= 0x58 && mapId < 0x61) ||
		     (mapId >= 0x84 && mapId < 0x88)) &&
		    k < 0x23) {
			if (k >= 0x14) {
				obj->x = random(320);
			}
			obj->y = random(240);
			obj->flag |= 0x80;
		}
		obj++;
	}
	for (k = 0; k < 10; k++) {
		MAP_OBJECT_MOVE_TO_DATA[k] = 0;
	}
}

void loadMapImage1(u_long *tim)
{
	TIM_IMAGE image;

	OpenTIM(tim);
	ReadTIM(&image);
	LoadImage(image.prect, image.paddr);
	DrawSync(0);
	if (image.crect->y != 0x1e0) {
		LoadImage(image.crect, image.caddr);
		DrawSync(0);
	}
}

void loadMapImage2(u_long *tim, int32_t id)
{
	TIM_IMAGE image;
	RECT rect;
	u_long *caddr;
	int32_t i;

	OpenTIM(tim);
	ReadTIM(&image);
	LoadImage(image.prect, image.paddr);
	DrawSync(0);
	if (id != 0) {
		return;
	}

	caddr = image.caddr;
	for (i = 0; i < image.crect->h; i++) {
		rect.x = i * 16;
		rect.y = 486;
		rect.w = 16;
		rect.h = 1;
		LoadImage(&rect, caddr);
		caddr += 8;
	}
}

void calcMapObjectOrder(LocalMapObjectInstance *instances)
{
	LocalMapObjectInstance *data;
	SVECTOR worldPos;
	SVECTOR screen;
	int32_t depth;
	int32_t i;
	int32_t j;
	int16_t val;

	SetRotMatrix(&GsWSMATRIX);
	SetTransMatrix(&GsWSMATRIX);
	data = instances;
	for (i = 0; i < MAP_OBJECT_INSTANCE_COUNT; i++) {
		for (j = 0; j < 8; j++) {
			val = data->animSprites[j];
			if (val == -1) {
				continue;
			}
			if (val == -2) {
				continue;
			}
			worldPos.vx = LOCAL_MAP_OBJECTS[val].someX;
			worldPos.vy = LOCAL_MAP_OBJECTS[val].someY;
			worldPos.vz = LOCAL_MAP_OBJECTS[val].someZ;
			break;
		}
		gte_ldv0(&worldPos);
		gte_rtps();
		gte_stsxy(&screen);
		gte_stszotz(&depth);
		depth = depth >> 2;
		if (depth < 0x23) {
			depth = 0x23;
		}
		if (depth >= 0x1000) {
			depth = 0xff6;
		}
		if (LOCAL_MAP_OBJECTS[val].someY == 0x2710) {
			depth = 0xffe;
		}
		data->orderValue = depth;
		if (data->flag & 0x80) {
			data->orderValue = 0x1e;
		}
		data->currentFrame = 0;
		data++;
	}
}

void renderMapOverlays(LocalMapObjectInstance *instances, int32_t screenX,
		       int16_t screenY)
{
	LocalMapObjectInstance *inst;
	LocalMapObject *obj;
	POLY_FT4 *prim;
	GsOT_TAG *org;
	int32_t i;
	int32_t objId;
	int16_t startFrame;
	int32_t sx;
	int32_t xMin;
	int32_t xMax;
	int32_t yMin;
	int32_t yMax;

	if (!MAP_LAYER_ENABLED) {
		return;
	}

	if ((CURRENT_SCREEN >= 115 && CURRENT_SCREEN < 120) ||
	    CURRENT_SCREEN == 121 || CURRENT_SCREEN == 163 ||
	    CURRENT_SCREEN == 220) {
		renderMist();
	}

	sx = screenX;
	org = ACTIVE_ORDERING_TABLE->org;
	screenX = sx;

	xMin = screenX - 40;
	xMax = sx + 360;
	i = 0;
	yMin = screenY - 60;
	yMax = screenY + 300;

	inst = instances;
	for (; i < 188; i++) {
		startFrame = inst->currentFrame;

		if (inst->animSprites[startFrame] == -1 || startFrame == -1) {
			inst++;
			continue;
		}

		objId = inst->animSprites[startFrame];

		if (objId != -2 && inst->flag != 1) {
			obj = &LOCAL_MAP_OBJECTS[objId];

			if (((xMin < (inst->x + obj->width)) &&
			     (inst->x < xMax) &&
			     (yMin < (inst->y + obj->height)) &&
			     (inst->y < yMax)) ||
			    (inst->flag & 0x80) ||
			    (inst->orderValue < 20)) {
				prim = (POLY_FT4 *)GsGetWorkBase();
				SetPolyFT4(prim);
				if (obj->transparency == 4) {
					SetSemiTrans(prim, 0);
				} else {
					SetSemiTrans(prim, 1);
				}

				if (inst->flag & 0x80) {
					buildSnowflakePrim(prim, inst,
							   obj);
				} else if (inst->orderValue < 20) {
					buildMapOverlayPrim(prim, inst,
							    obj, screenX,
							    screenY, 1);
				} else {
					buildMapOverlayPrim(prim, inst,
							    obj, screenX,
							    screenY, 0);
				}

				AddPrim(&org[inst->orderValue], prim);
				prim++;
				GsSetWorkBase((PACKET *)prim);
			}

			inst->timer++;
		if (inst->timer == inst->animTimes[startFrame]) {
			inst->currentFrame++;
			inst->timer = 0;
			if (inst->animSprites[inst->currentFrame] == -1 ||
			    inst->currentFrame >= 8) {
				inst->currentFrame = 0;
			}
		}
		} else {
			inst->timer++;
		if (inst->timer == inst->animTimes[startFrame]) {
			inst->currentFrame++;
			inst->timer = 0;
			if (inst->animSprites[inst->currentFrame] == -1 ||
			    inst->currentFrame >= 8) {
				inst->currentFrame = 0;
			}
		}
		}
		inst++;
	}
}

void renderMist(void)
{
	POLY_FT4 *prim;
	int16_t cameraDeltaX;
	int16_t cameraDeltaY;
	int32_t i;
	int32_t yIndex;
	int32_t evenFrame;

	if (((CURRENT_SCREEN != 0xa3) && (CURRENT_SCREEN != 0xdc)) ||
	    (isTriggerSet(0x155) != 1)) {
		if ((CURRENT_SCREEN != 0xdc) && (isTriggerSet(0x94) == 1)) {
			MIST_CLUT_Y[0] = 0xc0;
			MIST_CLUT_Y[1] = 0x80;
		}

		cameraDeltaX = CAMERA_X_PREVIOUS;
		cameraDeltaX -= CAMERA_X[0];
		/* Preserve the retail load order with the pinned compiler. */
		cameraDeltaY = CAMERA_Y_PREVIOUS - *(volatile int16_t *)CAMERA_Y;
		MIST_X_OFFSETS[0] += cameraDeltaX;
		if ((evenFrame = PLAYTIME_FRAMES % 2) == 0) {
			MIST_X_OFFSETS[0]--;
		}
		if (MIST_X_OFFSETS[0] >= 0xa0) {
			MIST_X_OFFSETS[0] -= 0x280;
		}
		if (MIST_X_OFFSETS[0] < -0x1df) {
			MIST_X_OFFSETS[0] += 0x280;
		}

		MAIN_D_80134226 = MIST_X_OFFSETS[0] + 0x140;
		if (MAIN_D_80134226 >= 0xa0) {
			MAIN_D_80134226 -= 0x280;
		}
		if (MAIN_D_80134226 < -0x1df) {
			MAIN_D_80134226 += 0x280;
		}

		MAIN_D_80134228 += (int16_t)(cameraDeltaX * 12 / 10);
		if (evenFrame == 0) {
			MAIN_D_80134228++;
		}
		if (MAIN_D_80134228 >= 0x1e0) {
			MAIN_D_80134228 -= 0x280;
		}
		if (MAIN_D_80134228 < -0x9f) {
			MAIN_D_80134228 += 0x280;
		}

		MAIN_D_8013422A = MAIN_D_80134228 + 0x140;
		if (MAIN_D_8013422A >= 0x1e0) {
			MAIN_D_8013422A -= 0x280;
		}
		if (MAIN_D_8013422A < -0x9f) {
			MAIN_D_8013422A += 0x280;
		}
		if (MAIN_D_8013422A >= 0x1e0) {
			MAIN_D_8013422A -= 0x280;
		}
		if (MAIN_D_8013422A < -0x9f) {
			MAIN_D_8013422A += 0x280;
		}

		MIST_Y_OFFSETS[0] += cameraDeltaY;
		if (MIST_Y_OFFSETS[0] >= 0x78) {
			MIST_Y_OFFSETS[0] -= 0x1e0;
		}
		if (MIST_Y_OFFSETS[0] < -0x167) {
			MIST_Y_OFFSETS[0] += 0x1e0;
		}
		MAIN_D_8013422E = MIST_Y_OFFSETS[0] + 0xf0;
		if (MAIN_D_8013422E >= 0x78) {
			MAIN_D_8013422E -= 0x1e0;
		}
		if (MAIN_D_8013422E < -0x167) {
			MAIN_D_8013422E += 0x1e0;
		}

		for (i = 0, yIndex = -4; i < 8; i++, yIndex++) {
			prim = (POLY_FT4 *)GsGetWorkBase();
			SetPolyFT4(prim);
			SetSemiTrans(prim, 1);
			if (i < 4) {
				prim->tpage = GetTPage(0, 3, 0x2c0, 0);
				prim->clut = GetClut(0, 0x1e6);
				setPosDataPolyFT4(prim, MIST_X_OFFSETS[i % 2],
						   MIST_Y_OFFSETS[i / 2], 0x140,
						   0xf0);
				prim->r0 = 0x96;
				prim->g0 = 0x96;
				prim->b0 = 0x96;
			} else {
				prim->tpage = GetTPage(0, 1, 0x2c0, 0);
				if (CURRENT_SCREEN == 0xa3 || CURRENT_SCREEN == 0xdc) {
					prim->clut = GetClut(0, 0x1e6);
				} else if (CURRENT_SCREEN == 0x77) {
					prim->clut = GetClut(MIST_CLUT_Y[0],
							     0x1e6);
				} else {
					prim->clut = GetClut(MIST_CLUT_Y[1],
							     0x1e6);
				}
				prim->x0 = (&MAIN_D_80134228)[i % 2];
				prim->x1 = (&MAIN_D_80134228)[i % 2] - 0x140;
				prim->x2 = (&MAIN_D_80134228)[i % 2];
				prim->x3 = (&MAIN_D_80134228)[i % 2] - 0x140;
				prim->y0 = MIST_Y_OFFSETS[yIndex / 2];
				prim->y1 = MIST_Y_OFFSETS[yIndex / 2];
				prim->y2 = MIST_Y_OFFSETS[yIndex / 2] + 0xf0;
				prim->y3 = MIST_Y_OFFSETS[yIndex / 2] + 0xf0;
				prim->r0 = 0x50;
				prim->g0 = 0x50;
				prim->b0 = 0x50;
			}
			setUVDataPolyFT4(prim, 0, 0, 0xff, 0xc8);
			AddPrim(&ACTIVE_ORDERING_TABLE->org[20], prim);
			prim++;
			GsSetWorkBase((PACKET *)prim);
		}
	}
}

void buildSnowflakePrim(POLY_FT4 *prim, LocalMapObjectInstance *inst,
			LocalMapObject *obj)
{
	int32_t randomRange;
	int32_t fallSpeed;
	int8_t randomValue;
	int8_t chance;
	int8_t horizontalMovement;

	if ((inst->animSprites[0] == 0) || (inst->animSprites[0] == 3) ||
	    (inst->animSprites[0] == 4)) {
		randomRange = 6;
		fallSpeed = 3;
	}
	if ((inst->animSprites[0] == 1) || (inst->animSprites[0] == 5) ||
	    (inst->animSprites[0] == 6)) {
		randomRange = 2;
		fallSpeed = 1;
	}
	if (inst->animSprites[0] == 2) {
		randomRange = 4;
		fallSpeed = 10;
	}

	randomValue = horizontalMovement = random(randomRange);
	if ((randomValue > (randomRange / 2)) != 0) {
		horizontalMovement = -(horizontalMovement % (randomRange / 2));
	}

	if ((inst->animSprites[0] == 0) || (inst->animSprites[0] == 3) ||
	    (inst->animSprites[0] == 4)) {
		if ((CURRENT_FRAME % 3) == 0) {
			inst->x += horizontalMovement;
		}
		if (inst->x >= 0x141) {
			inst->x = 0;
		}
		if (inst->x < 0) {
			inst->x = 0x140;
		}
	}

	setPosDataPolyFT4(prim, inst->x - 0xa0 + horizontalMovement,
			   inst->y - 0x78, obj->width, obj->height);
	prim->r0 = 0x80;
	prim->g0 = 0x80;
	prim->b0 = 0x80;
	setUVDataPolyFT4(prim, obj->texX % 256, obj->texY % 256,
			 obj->width - 1, obj->height - 1);
	prim->tpage = GetTPage(0, obj->transparency,
			       (obj->texX / 256 << 6) + 0x180, 0);
	prim->clut = GetClut(obj->clut * 16, 0x1e6);

	inst->y = inst->y + fallSpeed;
	if ((inst->y >= 0x83) && (inst->animSprites[0] != 2)) {
		chance = random(10);
		if ((chance < 2) && (inst->currentFrame == 0)) {
			inst->currentFrame++;
			inst->timer = 0;
		}
		if ((inst->currentFrame >= 2) &&
		    (inst->animSprites[inst->currentFrame + 1] == -1)) {
			inst->currentFrame = 0;
			inst->y = 0;
		}
	}
	if (inst->y >= 0xf1) {
		inst->y = 0;
	}
}

void buildMapOverlayPrim(POLY_FT4 *prim, LocalMapObjectInstance *inst,
			 LocalMapObject *obj, int32_t arg3, int16_t arg4,
			 int8_t mode)
{

	if (mode == 0) {
		setPosDataPolyFT4(prim,
				  ((inst->x - 160) - (arg3 - (160 - DRAWING_OFFSET_X))),
				  ((inst->y - 120) - (arg4 - (120 - DRAWING_OFFSET_Y))),
				  obj->width, obj->height);
	} else {
		setPosDataPolyFT4(prim, inst->x, inst->y, obj->width,
				  obj->height);
	}

	prim->r0 = 128;
	prim->g0 = 128;
	prim->b0 = 128;

	if (((obj->texX % 256) + obj->width) < 256) {
		if (((obj->texY % 256) + obj->height) < 256) {
			goto fit;
		}
	}
	setUVDataPolyFT4(prim, obj->texX % 256, obj->texY % 256,
			 (obj->width - 1), (obj->height - 1));
	goto clut;

fit:
	setUVDataPolyFT4(prim, obj->texX % 256, obj->texY % 256, obj->width,
			 obj->height);

clut:
	if (obj->clut == -1) {
		prim->tpage = GetTPage(1, obj->transparency,
				       ((obj->texX / 256) << 7) + 384, 0);
		prim->clut = GetClut(0, 480);
	} else if (obj->clut < 16) {
		prim->tpage = GetTPage(0, obj->transparency,
				       ((obj->texX / 256) << 6) + 384, 0);
		prim->clut = GetClut(obj->clut << 4, 486);
	} else {
		prim->tpage = GetTPage(1, obj->transparency,
				       ((obj->texX / 256) << 7) + 384, 0);
		prim->clut = GetClut(0, obj->clut + 468);
	}
}

void createNinjamonEffect(void)
{
	int32_t i;

	NPC_ACTIVE_ANIM = 0;
	storeMapObjectPosition(MAIN_D_8013CA38, MAIN_D_8013CA8C, 0, 0x29);
	for (i = 0; i < 0x29; i++) {
		MAIN_D_8013CAE0[i] = random(10) + 12;
		MAIN_D_8013CB0C[i] = random(10) + 3;
	}
	addObject(0xfba, 0, NULL, renderNinjamonEffect);
}

void storeMapObjectPosition(int16_t *outX, int16_t *outY, uint8_t startIndex,
			    int32_t count)
{
	long idx;
	int32_t i;

	for (i = 0; i < count; i++) {
		idx = startIndex + i;
		*outX++ = LOCAL_MAP_OBJECT_INSTANCE[idx].x;
		*outY++ = LOCAL_MAP_OBJECT_INSTANCE[idx].y;
	}
}

void renderNinjamonEffect(int32_t instanceId)
{
	LocalMapObjectInstance *data;
	LocalMapObjectInstance *base;
	int32_t i;

	base = LOCAL_MAP_OBJECT_INSTANCE;
	data = base;
	for (i = 0; i < 0x29; i++) {
		data->x += MAIN_D_8013CAE0[i];
		data->y += MAIN_D_8013CB0C[i];
		if (data->x > 0xa0) {
			data->x = MAIN_D_8013CA38[i];
			data->y = MAIN_D_8013CA8C[i];
			MAIN_D_8013CAE0[i] = random(10) + 12;
			MAIN_D_8013CB0C[i] = random(10) + 3;
		}
		data->orderValue = 10;
		data++;
	}
	NPC_ACTIVE_ANIM++;
	if (NPC_ACTIVE_ANIM >= 0x3c) {
		data = base;
		i = 0;
		while (i < 0x29) {
			data->flag = 1;
			i++;
			data++;
		}
		removeObject(0xfba, 0);
	}
}

void loadMapObjectPosition(int16_t *xData, int16_t *yData, int16_t startIndex,
			   int32_t count)
{
	long idx;
	int32_t i;

	for (i = 0; i < count; i++) {
		idx = startIndex + i;
		LOCAL_MAP_OBJECT_INSTANCE[idx].x = *xData++;
		LOCAL_MAP_OBJECT_INSTANCE[idx].y = *yData++;
		LOCAL_MAP_OBJECT_INSTANCE[idx].flag &= ~0x10;
	}
}

int32_t moveMapObjectsWithLimit(uint8_t startIndex, int32_t count, int16_t dx,
				int16_t dy, int16_t limitX, int16_t limitY)
{
	long idx;
	int32_t i;

	for (i = 0; i < count; i++) {
		idx = startIndex + i;
		LOCAL_MAP_OBJECT_INSTANCE[idx].x += dx;
		LOCAL_MAP_OBJECT_INSTANCE[idx].y += dy;
	}
	if (dx > 0) {
		if (LOCAL_MAP_OBJECT_INSTANCE[startIndex].x >= limitX) {
			return 1;
		}
	} else if (dx < 0) {
		if (LOCAL_MAP_OBJECT_INSTANCE[startIndex].x <= limitX) {
			return 1;
		}
	}
	if (dy > 0) {
		if (LOCAL_MAP_OBJECT_INSTANCE[startIndex].y >= limitY) {
			return 1;
		}
	} else if (dy < 0) {
		if (LOCAL_MAP_OBJECT_INSTANCE[startIndex].y <= limitY) {
			return 1;
		}
	}
	return 0;
}

int32_t tickMoveObjectTo(uint8_t objectIndex, uint8_t moveIndex,
			 int8_t steps, int16_t targetX, int16_t targetY)
{
	int8_t moveX;
	int32_t moveY;
	int32_t currentX;
	int32_t currentY;

	if (MAP_OBJECT_MOVE_TO_DATA[moveIndex] == 0) {
		MAIN_D_8013CB38[moveIndex] =
			(targetX - LOCAL_MAP_OBJECT_INSTANCE[objectIndex].x) / steps;
		MAIN_D_8013CB44[moveIndex] =
			(targetY - LOCAL_MAP_OBJECT_INSTANCE[objectIndex].y) / steps;
		MAP_OBJECT_MOVE_TO_DATA[moveIndex] = 1;
	}

	moveX = MAIN_D_8013CB38[moveIndex];
	LOCAL_MAP_OBJECT_INSTANCE[objectIndex].x +=
		MAIN_D_8013CB38[moveIndex];
	moveY = MAIN_D_8013CB44[moveIndex];
	LOCAL_MAP_OBJECT_INSTANCE[objectIndex].y += moveY;

	if (moveX > 0) {
		if (LOCAL_MAP_OBJECT_INSTANCE[objectIndex].x >= targetX) {
			LOCAL_MAP_OBJECT_INSTANCE[objectIndex].x = targetX;
		}
	} else if (MAIN_D_8013CB38[moveIndex] < 0) {
		if (LOCAL_MAP_OBJECT_INSTANCE[objectIndex].x <= targetX) {
			LOCAL_MAP_OBJECT_INSTANCE[objectIndex].x = targetX;
		}
	} else {
		LOCAL_MAP_OBJECT_INSTANCE[objectIndex].x = targetX;
	}

	if (MAIN_D_8013CB44[moveIndex] > 0) {
		if (LOCAL_MAP_OBJECT_INSTANCE[objectIndex].y >= targetY) {
			LOCAL_MAP_OBJECT_INSTANCE[objectIndex].y = targetY;
		}
	} else if (MAIN_D_8013CB44[moveIndex] < 0) {
		if (LOCAL_MAP_OBJECT_INSTANCE[objectIndex].y <= targetY) {
			LOCAL_MAP_OBJECT_INSTANCE[objectIndex].y = targetY;
		}
	} else {
		LOCAL_MAP_OBJECT_INSTANCE[objectIndex].y = targetY;
	}

	currentX = LOCAL_MAP_OBJECT_INSTANCE[objectIndex].x;
	if (currentX == targetX) {
		currentY = LOCAL_MAP_OBJECT_INSTANCE[objectIndex].y;
		if (currentY == targetY) {
			MAP_OBJECT_MOVE_TO_DATA[moveIndex] = 0;
			return 1;
		}
	}
	return 0;
}

void moveMapObjects(uint8_t startIndex, int32_t count, int16_t dx, int16_t dy)
{
	long idx;
	int32_t i;

	for (i = 0; i < count; i++) {
		idx = startIndex + i;
		LOCAL_MAP_OBJECT_INSTANCE[idx].x += dx;
		LOCAL_MAP_OBJECT_INSTANCE[idx].y += dy;
	}
}

void setMapObjectsFlag(uint8_t start, int32_t count, int32_t flag)
{
	long idx;
	int32_t i;

	for (i = 0; i < count; i++) {
		idx = start + i;
		LOCAL_MAP_OBJECT_INSTANCE[idx].flag = flag;
	}
}

void getDrawPosition(SVECTOR *worldPos, int16_t *outX, int16_t *outY)
{
	int16_t screen[4];
	int16_t camX;

	SetRotMatrix(&GsWSMATRIX);
	SetTransMatrix(&GsWSMATRIX);
	gte_ldv0(worldPos);
	gte_rtps();
	gte_stsxy(screen);
	*outX = DRAWING_OFFSET_X + (screen[0] + (camX = CAMERA_X[0]));
	*outY = DRAWING_OFFSET_Y + (screen[1] + CAMERA_Y[0]);
}

void spawnSpriteAtLocation(int16_t x, int16_t y, int16_t z, int16_t sprite,
			   int16_t count)
{
	SVECTOR position;
	int16_t positionsX[30];
	int16_t positionsY[30];
	int16_t screenX;
	int16_t screenY;
	int32_t j;
	int32_t i;

	position.vx = x;
	position.vy = y;
	position.vz = z;
	getDrawPosition(&position, &screenX, &screenY);

	for (i = 0; i < count; i++) {
		for (j = 0; j < 8; j++) {
			if (LOCAL_MAP_OBJECT_INSTANCE[(long)sprite + i]
				    .animSprites[j] != -2) {
				break;
			}
		}
		positionsX[i] =
			screenX -
			LOCAL_MAP_OBJECTS[LOCAL_MAP_OBJECT_INSTANCE[(long)sprite + i]
						  .animSprites[j]]
				.width /
				2;
		positionsY[i] =
			screenY -
			LOCAL_MAP_OBJECTS[LOCAL_MAP_OBJECT_INSTANCE[(long)sprite + i]
						  .animSprites[j]]
				.height /
				2;
	}

	loadMapObjectPosition(positionsX, positionsY, sprite, count);
}

void spawnSpriteAtEntity(int32_t scriptId, int32_t nodeId, uint8_t sprite)
{
	Entity *entity;
	MATRIX *m;

	entity = getEntityFromScriptId((uint8_t *)&scriptId);
	m = &entity->posData[nodeId].posMatrix.workm;
	spawnSpriteAtLocation(m->t[0], m->t[1], m->t[2], sprite, 1);
}

void resetMapObjectAnimation(uint8_t startIndex, int32_t count)
{
	long idx;
	int32_t i;

	for (i = 0; i < count; i++) {
		idx = startIndex + i;
		LOCAL_MAP_OBJECT_INSTANCE[idx].timer = 0;
		LOCAL_MAP_OBJECT_INSTANCE[idx].currentFrame = 0;
	}
}

int32_t tickRemoveMist(void)
{
	MIST_CLUT_Y[0] += 0x10;
	MIST_CLUT_Y[1] += 0x10;

	if (MIST_CLUT_Y[0] >= 0xc0) {
		MIST_CLUT_Y[0] = 0xc0;
		MIST_CLUT_Y[1] = 0x80;
		return 1;
	}

	return 0;
}

void loadMapDigimon(int16_t *data, int32_t mapId)
{
	Stats *stats;
	int16_t count;
	int32_t digimonCount;
	VECTOR *waypoints;
	int32_t i;
	int32_t j;

	digimonCount = *data++;
	if (MAP_ENTRIES[mapId].flags & 0x80) {
		for (i = 0; i < digimonCount; i++) {
			MAP_DIGIMON_TABLE[i].typeId = *data++;
			MAP_DIGIMON_TABLE[i].followMode = *data++;
			MAP_DIGIMON_TABLE[i].posX = *data++;
			MAP_DIGIMON_TABLE[i].posY = *data++;
			MAP_DIGIMON_TABLE[i].posZ = *data++;
			MAP_DIGIMON_TABLE[i].rotX = *data++;
			MAP_DIGIMON_TABLE[i].rotY = *data++;
			MAP_DIGIMON_TABLE[i].rotZ = *data++;
			MAP_DIGIMON_TABLE[i].trackingRange = *data++;
			NPC_ENTITIES[i].unk2 = *data++;
			NPC_ENTITIES[i].scriptId = *data++;
			stats = &NPC_ENTITIES[i].digimonEntity.stats;
			stats->base.hp = *data++;
			stats->base.mp = *data++;
			stats->current.currentHP = *data++;
			stats->current.currentMP = *data++;
			stats->base.off = *data++;
			stats->base.def = *data++;
			stats->base.speed = *data++;
			stats->base.brain = *data++;
			NPC_ENTITIES[i].bits = *data++;
			stats->current.chargeMode = *data++;
			NPC_ENTITIES[i].unk1 = *data++;
			stats->base.moves[0] = *data++;
			stats->base.moves[1] = *data++;
			stats->base.moves[2] = *data++;
			stats->base.moves[3] = *data++;
			stats->base.movesPrio[0] = *data++;
			stats->base.movesPrio[1] = *data++;
			stats->base.movesPrio[2] = *data++;
			stats->base.movesPrio[3] = *data++;
			NPC_ENTITIES[i].flee.vx = *data++;
			NPC_ENTITIES[i].flee.vy = *data++;
			NPC_ENTITIES[i].flee.vz = *data++;
			MAP_DIGIMON_TABLE[i].animation = 0;
			waypoints = MAP_DIGIMON_TABLE[i].waypoints;
			count = *data++;
			for (j = 0; j < 8; j++) {
				*(int16_t *)((uint8_t *)(j * 2) + (uint32_t)waypoints + 0x80) = *data++;
			}
			for (j = 0; j < count; j++) {
				waypoints[j].vx = *data++;
				waypoints[j].vy = *data++;
				waypoints[j].vz = *data++;
			}
			*(int16_t *)((uint8_t *)waypoints + 0x90) = 0;
		}
	}
}

void loadNPCModel(int32_t digimonId)
{
	thunkLoadMMD(digimonId, 0);
}

int32_t scriptSetDigimon(uint8_t type, uint8_t slot, uint8_t autotalk)
{
	Entity *entity;
	int32_t entityId;

	if (type != MAP_DIGIMON_TABLE[slot].typeId) {
		return 0;
	}
	entityId = slot + 2;
	if ((entity = ENTITY_TABLE[entityId]) != NULL) {
		removeEntity(entity->type, slot + 2);
	}
	ENTITY_TABLE[slot + 2] = &NPC_ENTITIES[slot].digimonEntity.entity;
	initializeDigimonObject(type, slot + 2, tickNPC);
	setEntityPosition(slot + 2, MAP_DIGIMON_TABLE[slot].posX,
			  MAP_DIGIMON_TABLE[slot].posY,
			  MAP_DIGIMON_TABLE[slot].posZ);
	setEntityRotation(slot + 2, MAP_DIGIMON_TABLE[slot].rotX,
			  MAP_DIGIMON_TABLE[slot].rotY,
			  MAP_DIGIMON_TABLE[slot].rotZ);
	setupEntityMatrix(slot + 2);
	startAnimation(ENTITY_TABLE[slot + 2],
		       MAP_DIGIMON_TABLE[slot].animation);
	NPC_ENTITIES[slot].autotalk = autotalk;
	NPC_IS_WALKING_TOWARDS[slot] = 0;
	ENTITY_TABLE[slot + 2]->isOnMap = 1;
	ENTITY_TABLE[slot + 2]->isOnScreen =
		entityIsOffScreen(ENTITY_TABLE[slot + 2], 0x140, 0xf0) ^ 1;
	return 1;
}

void tickNPC(int32_t instanceId)
{
	if (ENTITY_TABLE[instanceId]->isOnMap == 0) {
		return;
	}

	switch (GAME_STATE) {
	case 0:
		tickNPCOverworld(instanceId, &MAP_DIGIMON_TABLE[instanceId - 2]);
		break;
	case 1:
	case 2:
	case 3:
		tickNPCBattle(instanceId);
		break;
	case 4:
	case 5:
		STD_tickNPCTournament(instanceId);
		break;
	}
}

void scriptUnloadEntity(uint8_t scriptId)
{
	Entity *entity;

	entity = getEntityFromScriptId(&scriptId);
	entity->isOnMap = 0;
	removeEntity(entity->type, scriptId);
}

void unloadDigimonModel(int32_t digimonType)
{
	thunkUnloadModel(digimonType, 0);
}

void clearMapDigimon(void)
{
	int32_t i;
	int32_t j;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			MAP_DIGIMON_TABLE[i].aiSections[j] = -1;
			MAP_DIGIMON_TABLE[i].waypoints[j].vx = 0;
			MAP_DIGIMON_TABLE[i].waypoints[j].vy = 0;
			MAP_DIGIMON_TABLE[i].waypoints[j].vz = 0;
		}
		MAP_DIGIMON_TABLE[i].activeSection = 0;
		ENTITY_TABLE[i + 2] = NULL;
		ENTITY_TABLE[i + 2]->isOnMap = 0;
		MAP_DIGIMON_TABLE[i].typeId = -1;
		MAP_DIGIMON_TABLE[i].waypointWaitTimer = 0;
		MAP_DIGIMON_TABLE[i].cwDiff = 0;
		MAP_DIGIMON_TABLE[i].ccDiff = 0;
		MAP_DIGIMON_TABLE[i].targetAngle = 0;
		MAP_DIGIMON_TABLE[i].hasWaypointTarget = 0;
		MAP_DIGIMON_TABLE[i].lookAtTamerState = 0;
		MAIN_D_80134D20[i] = 0;
	}
}

void removeMapEntities(void)
{
	volatile int32_t unloaded[8];
	Entity *entity;
	int32_t *models;
	int32_t i;
	int32_t j;

	for (j = 0; j < 8; j++) {
		unloaded[j] = -1;
	}
	for (i = 0; i < 8; i++) {
		entity = ENTITY_TABLE[i + 2];
		if (entity != NULL) {
			entity = ENTITY_TABLE[i + 2];
			removeEntity(entity->type, i + 2);
			ENTITY_TABLE[i + 2] = NULL;
		}
	}
	for (i = 0; i < 8; i++) {
		if (LOADED_DIGIMON_MODELS[i] != -1) {
			thunkUnloadModel((models = LOADED_DIGIMON_MODELS)[i], 0);
		}
	}
	initializeLoadedNPCModels();
	clearMapAITable(-1);
}

void clearMapAITable(int32_t index)
{
	int32_t i;

	if (index != -1) {
		MAP_DIGIMON_TABLE[index].lookAtTamerState =
			MAP_DIGIMON_TABLE[index].hasWaypointTarget = 0;
		MAP_DIGIMON_TABLE[index].activeSection = 0;
	} else {
		for (i = 0; i < 8; i++) {
			MAP_DIGIMON_TABLE[i].hasWaypointTarget = 0;
			MAP_DIGIMON_TABLE[i].lookAtTamerState = 0;
			MAP_DIGIMON_TABLE[i].activeSection = 0;
		}
	}
}

void tickNPCOverworld(int32_t instanceId, MapDigimonEntity *mapDigimon)
{
	Entity *entity;

	if ((entity = ENTITY_TABLE[instanceId]) == NULL) {
		return;
	}
	if (IS_IN_MENU == 1) {
		return;
	}

	if (MAIN_D_80134D20[instanceId - 2] != 0) {
	} else if (mapDigimon->stopAnim == 0) {
		if (mapDigimon->lookAtTamerState == 0) {
			tickWaypointAI(mapDigimon, entity, instanceId);
		}

		switch (mapDigimon->followMode) {
		case 2:
		case 11:
			if ((isInTrackingRect(mapDigimon, &TAMER_ENTITY.entity.posData->location) == 1) ||
			    (mapDigimon->lookAtTamerState != 0)) {
				tickLookingAtTamer(mapDigimon, entity,
						   &TAMER_ENTITY);
			}
			break;
		case 3:
		case 4:
		case 5:
		case 12:
		case 13:
		case 14:
			if ((isInTrackingRect(mapDigimon, &TAMER_ENTITY.entity.posData->location) == 1) ||
			    (mapDigimon->lookAtTamerState != 0)) {
				tickTrackingTamer(mapDigimon, entity,
						  &TAMER_ENTITY, instanceId);
			}
			break;
		case 6:
		case 15:
			if ((isInTrackingRadius(&NPC_ENTITIES[instanceId - 2].digimonEntity.entity,
						&TAMER_ENTITY.entity, mapDigimon) == 1) ||
			    (mapDigimon->lookAtTamerState != 0)) {
				tickTrackingTamer2(mapDigimon, entity,
						   &TAMER_ENTITY,
						   instanceId, 2);
			}
			break;
		case 7:
		case 16:
			if ((isInTrackingRadius(&NPC_ENTITIES[instanceId - 2].digimonEntity.entity,
						&TAMER_ENTITY.entity, mapDigimon) == 1) ||
			    (mapDigimon->lookAtTamerState != 0)) {
				tickTrackingTamer2(mapDigimon, entity,
						   &TAMER_ENTITY,
						   instanceId, 4);
			}
			break;
		case 8:
		case 9:
		case 17:
		case 18:
			if (isInTrackingRect(
				    mapDigimon,
				    &TAMER_ENTITY.entity.posData->location) == 1 ||
			    mapDigimon->lookAtTamerState != 0) {
				tickTrackingTamer2(mapDigimon, entity, &TAMER_ENTITY,
						   instanceId, 2);
			}
			break;
		}

		NPC_COLLISION_STATE[instanceId - 2] =
			entityCheckCollision(NULL, entity, 0, 0);
		if ((NPC_COLLISION_STATE[instanceId - 2] == 0) &&
		    (getTamerState() == 0) &&
		    (NPC_ENTITIES[instanceId - 2].autotalk == 1)) {
			entity->anim.animFlag |= 2;
			if (IS_SCRIPT_PAUSED == 1) {
				removeTriangleMenu();
				closeInventoryBoxes();
				removeUIBox1();
				TALKED_TO_ENTITY = instanceId;
				callScriptSection(CURRENT_SCRIPT_ID, NPC_ENTITIES[instanceId - 2].scriptId, 1);
			}
		}
		if ((NPC_COLLISION_STATE[instanceId - 2] != -1) &&
		    (NPC_IS_WALKING_TOWARDS[instanceId - 2] == 0) &&
		    (entity->anim.animId > 1 && entity->anim.animId < 5)) {
			collisionGrace(0, entity, 0, 0);
		}
	} else {
		if (mapDigimon->animation != 0) {
			mapDigimon->animation = 0;
			startAnimation(entity, mapDigimon->animation);
		}
	}

	entity->isOnScreen = entityIsOffScreen(entity, 0x140, 0xf0) ^ 1;
	tickAnimation(entity);
}

void tickWaypointAI(MapDigimonEntity *mapDigimon, Entity *entity,
		    int32_t instanceId)
{
	switch (mapDigimon->aiSections[mapDigimon->activeSection]) {
	case 0:
		tickWaypointWait(mapDigimon, entity);
		break;
	case 1:
		tickWaypointWalk(mapDigimon, entity, 2, instanceId);
		break;
	case 2:
		tickWaypointWalk(mapDigimon, entity, 4, instanceId);
		break;
	}

	if (mapDigimon->activeSection >= 8 ||
	    mapDigimon->aiSections[mapDigimon->activeSection] == -1) {
		mapDigimon->activeSection = 0;
	}
}

int32_t isInTrackingRect(MapDigimonEntity *mapDigimon, VECTOR *location)
{
	if ((mapDigimon->posX + mapDigimon->trackingRange > location->vx) &&
	    (mapDigimon->posX - mapDigimon->trackingRange < location->vx) &&
	    (mapDigimon->posZ + mapDigimon->trackingRange > location->vz) &&
	    (mapDigimon->posZ - mapDigimon->trackingRange < location->vz)) {
		return 1;
	}
	return 0;
}

void tickLookingAtTamer(MapDigimonEntity *mapDigimon, Entity *entity,
			TamerEntity *tamer)
{
	if (mapDigimon->lookAtTamerState == 0) {
		mapDigimon->animation = 0;
		startAnimation(entity, mapDigimon->animation);
		mapDigimon->lookAtTamerState = 1;
	} else {
		getRotationDifference(entity->posData,
				      &tamer->entity.posData->location,
				      &mapDigimon->targetAngle,
				      &mapDigimon->ccDiff,
				      &mapDigimon->cwDiff);
		rotateEntity(&entity->posData->rotation,
			     &mapDigimon->targetAngle, &mapDigimon->ccDiff,
			     &mapDigimon->cwDiff, 0x71);
		if (isInTrackingRect(mapDigimon, &TAMER_ENTITY.entity.posData->location) == 0) {
			mapDigimon->lookAtTamerState = 0;
			mapDigimon->hasWaypointTarget = 0;
		}
	}
}

void tickTrackingTamer(MapDigimonEntity *mapDigimon, Entity *entity,
		       TamerEntity *tamer, int32_t instanceId)
{
	if (mapDigimon->lookAtTamerState == 0) {
		mapDigimon->targetLocation.vx =
			tamer->entity.posData->location.vx;
		mapDigimon->targetLocation.vy =
			tamer->entity.posData->location.vy;
		mapDigimon->targetLocation.vz =
			tamer->entity.posData->location.vz;
		mapDigimon->animation = 2;
		startAnimation(entity, mapDigimon->animation);
		mapDigimon->lookAtTamerState = 1;
	} else {
		tickTrackingTamer3(mapDigimon, entity, instanceId);
	}
}

int32_t isInTrackingRadius(Entity *entity, Entity *otherEntity,
			   MapDigimonEntity *mapDigimon)
{
	int32_t dx;
	int32_t dz;

	dx = entity->posData->location.vx - otherEntity->posData->location.vx;
	dz = entity->posData->location.vz - otherEntity->posData->location.vz;
	if ((dx + dz) < ((mapDigimon->trackingRange * 6 / 10) * (mapDigimon->trackingRange * 6 / 10))) {
		return 1;
	}
	return 0;
}

void tickTrackingTamer2(MapDigimonEntity *mapDigimon, Entity *entity,
			TamerEntity *tamer, int32_t instanceId,
			uint8_t animId)
{
	if (mapDigimon->lookAtTamerState == 0) {
		if (mapDigimon->animation != animId) {
			mapDigimon->animation = animId;
			startAnimation(entity, mapDigimon->animation);
		}
		mapDigimon->lookAtTamerState = 1;
	} else {
		tickTrackingTamer4(mapDigimon, entity, tamer, instanceId);
	}
}

void getRotationDifference(PositionData *posData, VECTOR *targetLoc,
			   int16_t *outAngle, int16_t *outCcDiff,
			   int16_t *outCwDiff)
{
	*outAngle = _atan((int16_t)(targetLoc->vz - posData->location.vz),
			  (int16_t)(targetLoc->vx - posData->location.vx));
	if (*outAngle > posData->rotation.vy) {
		*outCwDiff = *outAngle - posData->rotation.vy;
		*outCcDiff = posData->rotation.vy + (4096 - *outAngle);
	} else {
		*outCwDiff = *outAngle + (4096 - posData->rotation.vy);
		*outCcDiff = posData->rotation.vy - *outAngle;
	}
}

int32_t rotateEntity(SVECTOR *rotation, int16_t *targetAngle, int16_t *ccDiff,
		     int16_t *cwDiff, int16_t speed)
{
	int16_t target;
	int16_t cw;
	int16_t cc;

	target = *targetAngle;
	cc = *ccDiff;
	cw = *cwDiff;
	if (rotation->vy < target) {
		if (cc < cw) {
			rotation->vy -= speed;
			if ((target - 4096) > rotation->vy) {
				rotation->vy = target;
				return 1;
			}
		} else if (cw < cc) {
			rotation->vy += speed;
			if (target < rotation->vy) {
				rotation->vy = target;
				return 1;
			}
		}
	} else if (target < rotation->vy) {
		if (cc < cw) {
			rotation->vy -= speed;
			if (rotation->vy < target) {
				rotation->vy = target;
				return 1;
			}
		} else if (cw < cc) {
			rotation->vy += speed;
			if ((target + 4096) < rotation->vy) {
				rotation->vy = target;
				return 1;
			}
		}
	} else {
		rotation->vy = target;
		return 1;
	}
	return 0;
}

void tickTrackingTamer3(MapDigimonEntity *mapDigimon, Entity *entity,
			int32_t instanceId)
{
	int16_t currentTileX;
	int16_t currentTileY;
	int16_t targetTileX;
	int16_t targetTileY;

	switch (mapDigimon->lookAtTamerState) {
	case 1:
		mapDigimon->animation = 4;
		startAnimation(entity, mapDigimon->animation);
		mapDigimon->lookAtTamerState = 2;
		break;
	case 2:
		if (NPC_COLLISION_STATE[instanceId - 2] == -1) {
			getRotationDifference(entity->posData,
					      &mapDigimon->targetLocation,
					      &mapDigimon->targetAngle,
					      &mapDigimon->ccDiff,
					      &mapDigimon->cwDiff);
			rotateEntity(&entity->posData->rotation,
				     &mapDigimon->targetAngle,
				     &mapDigimon->ccDiff, &mapDigimon->cwDiff,
				     0x71);
		}
		getModelTile(&entity->posData->location, &currentTileX,
			     &currentTileY);
		getModelTile(&mapDigimon->targetLocation, &targetTileX,
			     &targetTileY);
		if (((currentTileX == targetTileX) &&
		     (currentTileY == targetTileY)) ||
		    (NPC_COLLISION_STATE[instanceId - 2] == 0)) {
			mapDigimon->animation = 0;
			startAnimation(entity, mapDigimon->animation);
			mapDigimon->waypointWaitTimer = 0;
			mapDigimon->lookAtTamerState = 3;
		}
		break;
	case 3:
		mapDigimon->waypointWaitTimer++;
		if (mapDigimon->waypointWaitTimer >= 40) {
			mapDigimon->animation = 2;
			startAnimation(entity, mapDigimon->animation);
			mapDigimon->targetLocation.vx = mapDigimon->posX;
			mapDigimon->targetLocation.vy = mapDigimon->posY;
			mapDigimon->targetLocation.vz = mapDigimon->posZ;
			mapDigimon->waypointWaitTimer = 0;
			mapDigimon->lookAtTamerState = 4;
		}
		break;
	case 4:
		if (NPC_COLLISION_STATE[instanceId - 2] == -1) {
			getRotationDifference(entity->posData,
					      &mapDigimon->targetLocation,
					      &mapDigimon->targetAngle,
					      &mapDigimon->ccDiff,
					      &mapDigimon->cwDiff);
			rotateEntity(&entity->posData->rotation,
				     &mapDigimon->targetAngle,
				     &mapDigimon->ccDiff, &mapDigimon->cwDiff,
				     0x71);
		}
		getModelTile(&entity->posData->location, &currentTileX,
			     &currentTileY);
		getModelTile(&mapDigimon->targetLocation, &targetTileX,
			     &targetTileY);
		if ((currentTileX == targetTileX) &&
		    (currentTileY == targetTileY)) {
			mapDigimon->animation = 0;
			startAnimation(entity, mapDigimon->animation);
			mapDigimon->waypointWaitTimer = 0;
			mapDigimon->hasWaypointTarget = 0;
			mapDigimon->lookAtTamerState = 0;
		}
		break;
	}
}

void tickTrackingTamer4(MapDigimonEntity *mapDigimon, Entity *entity,
			TamerEntity *tamer, int32_t instanceId)
{
	int16_t currentTileX;
	int16_t currentTileY;
	int16_t targetTileX;
	int16_t targetTileY;

	switch (mapDigimon->lookAtTamerState) {
	case 1:
		if (isInTrackingRadius(&NPC_ENTITIES[instanceId - 2].digimonEntity.entity,
				       &tamer->entity, mapDigimon) == 1) {
			if (NPC_COLLISION_STATE[instanceId - 2] == -1) {
				getRotationDifference(
					entity->posData,
					&tamer->entity.posData->location,
					&mapDigimon->targetAngle,
					&mapDigimon->ccDiff,
					&mapDigimon->cwDiff);
				rotateEntity(&entity->posData->rotation,
					     &mapDigimon->targetAngle,
					     &mapDigimon->ccDiff,
					     &mapDigimon->cwDiff, 0x71);
			}
		} else {
			mapDigimon->lookAtTamerState = 2;
			mapDigimon->targetLocation.vx = mapDigimon->posX;
			mapDigimon->targetLocation.vy = mapDigimon->posY;
			mapDigimon->targetLocation.vz = mapDigimon->posZ;
		}
		break;
	case 2:
		if (NPC_COLLISION_STATE[instanceId - 2] == -1) {
			getRotationDifference(entity->posData,
					      &mapDigimon->targetLocation,
					      &mapDigimon->targetAngle,
					      &mapDigimon->ccDiff,
					      &mapDigimon->cwDiff);
			rotateEntity(&entity->posData->rotation,
				     &mapDigimon->targetAngle,
				     &mapDigimon->ccDiff,
				     &mapDigimon->cwDiff, 0x71);
		}
		getModelTile(&entity->posData->location, &currentTileX,
			     &currentTileY);
		getModelTile(&mapDigimon->targetLocation, &targetTileX,
			     &targetTileY);
		if ((currentTileX == targetTileX) &&
		    (currentTileY == targetTileY)) {
			mapDigimon->animation = 0;
			startAnimation(entity, mapDigimon->animation);
			mapDigimon->waypointWaitTimer = 0;
			mapDigimon->lookAtTamerState = 3;
		}
		break;
	case 3:
		mapDigimon->waypointWaitTimer++;
		if (mapDigimon->waypointWaitTimer >= 80) {
			mapDigimon->hasWaypointTarget = 0;
			mapDigimon->lookAtTamerState = 0;
			mapDigimon->waypointWaitTimer = 0;
		}
		break;
	}
}

void tickWaypointWait(MapDigimonEntity *mapDigimon, Entity *entity)
{
	switch (mapDigimon->hasWaypointTarget) {
	case 0:
		if (mapDigimon->animation != 0) {
			mapDigimon->animation = 0;
			startAnimation(entity, mapDigimon->animation);
		}
		mapDigimon->waypointWaitTimer = 0;
		mapDigimon->hasWaypointTarget = 1;
		break;
	case 1:
		mapDigimon->waypointWaitTimer++;
		if (mapDigimon->waypointWaitTimer >=
		    mapDigimon->waypoints[mapDigimon->activeSection].vx) {
			mapDigimon->hasWaypointTarget = 0;
			mapDigimon->waypointWaitTimer = 0;
			mapDigimon->activeSection++;
		}
		break;
	}
}

void tickWaypointWalk(MapDigimonEntity *mapDigimon, Entity *entity,
		      int32_t animation, int32_t instanceId)
{
	int16_t currentTileX;
	int16_t currentTileY;
	int16_t targetTileX;
	int16_t targetTileY;

	switch (mapDigimon->hasWaypointTarget) {
	case 0:
		mapDigimon->targetLocation.vx =
			mapDigimon->waypoints[mapDigimon->activeSection].vx;
		mapDigimon->targetLocation.vy =
			mapDigimon->waypoints[mapDigimon->activeSection].vy;
		mapDigimon->targetLocation.vz =
			mapDigimon->waypoints[mapDigimon->activeSection].vz;
		if (mapDigimon->animation != animation) {
			mapDigimon->animation = animation;
			startAnimation(entity, mapDigimon->animation);
		}
		mapDigimon->hasWaypointTarget = 1;
		break;
	case 1:
		if (NPC_COLLISION_STATE[instanceId - 2] == -1) {
			getRotationDifference(entity->posData,
					      &mapDigimon->targetLocation,
					      &mapDigimon->targetAngle,
					      &mapDigimon->ccDiff,
					      &mapDigimon->cwDiff);
			rotateEntity(&entity->posData->rotation,
				     &mapDigimon->targetAngle,
				     &mapDigimon->ccDiff, &mapDigimon->cwDiff,
				     0x71);
		}
		getModelTile(&entity->posData->location, &currentTileX,
			     &currentTileY);
		getModelTile(&mapDigimon->targetLocation, &targetTileX,
			     &targetTileY);
		if ((currentTileX == targetTileX) &&
		    (currentTileY == targetTileY)) {
			mapDigimon->hasWaypointTarget = 0;
			mapDigimon->activeSection++;
		}
		break;
	}
}

void setActiveAnim(uint8_t scriptId, uint8_t animId)
{
	NPCEntity *npc;
	int32_t i;

	npc = NPC_ENTITIES;
	for (i = 0; i < 8; i++) {
		if (ENTITY_TABLE[i + 2] == NULL) {
			npc++;
			continue;
		}
		if (npc->scriptId == scriptId) {
			MAIN_D_80134D20[i] = animId;
			return;
		}
		npc++;
	}
}

void startNPCAnimation(uint8_t scriptId, int32_t animId)
{
	NPCEntity *npc;
	int32_t i;

	npc = NPC_ENTITIES;
	for (i = 0; i < 8; i++) {
		if (ENTITY_TABLE[i + 2] == NULL) {
			npc++;
			continue;
		}
		if (npc->scriptId == scriptId) {
			startAnimation((Entity *)npc, animId);
			return;
		}
		npc++;
	}
}

void setLoopCountToOne(uint32_t scriptId)
{
	Entity *entity;

	entity = getEntityFromScriptId((uint8_t *)&scriptId);
	entity->anim.loopCount = 1;
}

void resetEntityOrigin(int32_t scriptId)
{
	NPCEntity *npc;
	VECTOR *waypoints;
	int16_t deltaX;
	int16_t deltaZ;
	int32_t entityIndex;
	int32_t i;
	int32_t j;

	npc = NPC_ENTITIES;
	for (i = 0; i < 8; i++) {
		entityIndex = i + 2;
		if (ENTITY_TABLE[entityIndex] == NULL) {
			npc++;
			continue;
		}
		if (npc->scriptId == scriptId) {
			break;
		}
		npc++;
	}
	deltaX = npc->digimonEntity.entity.posData->location.vx -
		 MAP_DIGIMON_TABLE[i].posX;
	deltaZ = npc->digimonEntity.entity.posData->location.vz -
		 MAP_DIGIMON_TABLE[i].posZ;
	MAP_DIGIMON_TABLE[i].posX =
		npc->digimonEntity.entity.posData->location.vx;
	MAP_DIGIMON_TABLE[i].posZ =
		npc->digimonEntity.entity.posData->location.vz;
	waypoints = MAP_DIGIMON_TABLE[i].waypoints;
	for (j = 0; j < 8; j++) {
		if (*(int16_t *)((uint8_t *)(j * 2) +
				   (uint32_t)waypoints + 0x80) != 0) {
			if (*(int16_t *)((uint8_t *)(j * 2) +
					   (uint32_t)waypoints + 0x80) == -1) {
				break;
			}
			waypoints[j].vx += deltaX;
			waypoints[j].vz += deltaZ;
		}
	}
}

void setMovementEnabled(int32_t id, int32_t enabled)
{
	int32_t i;

	if (id != -1) {
		if (id == 0) {
			if (enabled == 0) {
				setTamerState(0);
			} else {
				setTamerState(6);
			}
		} else if (id == 1) {
			if (enabled == 0) {
				setPartnerState(1);
			} else {
				setPartnerState(11);
			}
		} else {
			MAP_DIGIMON_TABLE[id - 2].stopAnim = enabled;
			if (enabled == 1) {
				clearMapAITable((int16_t)(id - 2));
			}
		}
	} else {
		if (enabled == 0) {
			setTamerState(0);
			setPartnerState(1);
		} else {
			setTamerState(6);
			setPartnerState(11);
		}
		for (i = 0; i < 8; i++) {
			MAP_DIGIMON_TABLE[i].stopAnim = enabled;
			if (enabled == 1) {
				clearMapAITable(-1);
			}
		}
	}
}

void setPartnerIdling(void)
{
	startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0);
}

void addGameMenu(void)
{
	MAIN_D_80123E8C[0] = 1;
	MAIN_D_80134D28 = 7;
	GAME_MENU_SPRITES[0x4e] = 0;
	MAIN_D_80134D2C = hasFishingRod();
	if (MAIN_D_80134D2C != 0) {
		MAIN_D_80134D28++;
		if (MAIN_D_80134D2C == 1) {
			GAME_MENU_SPRITES[0x4e] = 1;
		}
		MAIN_D_80123E8C[0] = 7;
	}
	if (PARTNER_PARA.condition & 1) {
		GAME_MENU_SPRITES[0x42] = 0;
	} else {
		GAME_MENU_SPRITES[0x42] = 1;
	}
	TRIANGLE_MENU_STATE = 0;
	addObject(0xfa4, 0, (void (*)(int32_t))tickTriangleMenu, NULL);
}

void tickTriangleMenu(void)
{
	RECT rect;

	rect = MAIN_D_80134290;
	switch (TRIANGLE_MENU_STATE) {
	case 0:
		if (MAIN_D_80134D2C != 0) {
			createMenuBox(0, -0x42, -0x50, 0x84, 0x95, 2,
				      tickGameMenu, renderGameMenu);
		} else {
			createMenuBox(0, -0x42, -0x50, 0x84, 0x6e, 2,
				      tickGameMenu, renderGameMenu);
		}
		if (UI_BOX_DATA[0].frame == 4) {
			TRIANGLE_MENU_STATE = -1;
		}
		clearTextSubArea(&rect);
		drawString(MAIN_D_80134298, 0, 0xe8);
		break;
	case 1:
		closeUIBoxIfOpen(0);
		removeObject(0xfa4, 0);
		break;
	case 2:
		closeUIBoxIfOpen(0);
		if (UI_BOX_DATA[0].frame == 0) {
			initializeInventoryObject();
			TRIANGLE_MENU_STATE = -1;
		}
		break;
	case 3:
		closeUIBoxIfOpen(0);
		if (UI_BOX_DATA[0].frame == 0) {
			createMenuBox(1, -0x96, -0x59, 300, 0xbe, 0,
				      tickDigimonMenu, renderDigimonMenu);
			clearTextArea();
			drawString(MAIN_D_801247B8[0], 0, 0);
			MENU_STATE = 0;
			MENU_SUB_STATE = 0;
			TRIANGLE_MENU_STATE = -1;
			MAIN_D_80134D36 = 0;
		}
		break;
	case 4:
		TAMER_ENTITY.entity.isOnScreen = 1;
		PARTNER_ENTITY.digimonEntity.entity.isOnScreen = 1;
		closeUIBoxIfOpen(1);
		if (UI_BOX_DATA[1].frame == 0) {
			TRIANGLE_MENU_STATE = 0;
		}
		break;
	case 5:
		closeUIBoxIfOpen(0);
		if (UI_BOX_DATA[0].frame == 0) {
			createMenuBox(1, -0x96, -0x59, 300, 0xbe, 0,
				      tickPlayerMenu, renderPlayerMenu);
			clearTextArea();
			drawString(MAIN_D_80124800[0], 0, 0);
			MENU_STATE = 0;
			MENU_SUB_STATE = 0;
			TRIANGLE_MENU_STATE = -1;
			MAIN_D_80134D37 = 0;
		}
		break;
	case 6:
		TAMER_ENTITY.entity.isOnScreen = 1;
		PARTNER_ENTITY.digimonEntity.entity.isOnScreen = 1;
		closeUIBoxIfOpen(1);
		if (UI_BOX_DATA[1].frame == 0) {
			TRIANGLE_MENU_STATE = 0;
		}
	}
}

void closeTriangleMenu(void)
{
	closeUIBoxIfOpen(1);
	closeInventoryBoxes2();
	closeUIBoxIfOpen(0);
	removeObject(0xfa4, 0);
}

void closeUIBoxIfOpen(int32_t id)
{
	int16_t pos[2];

	if (UI_BOX_DATA[id].frame >= 5 && UI_BOX_DATA[id].state == 1) {
		getEntityScreenPos(ENTITY_TABLE[0], 0, pos);
		removeAnimatedUIBox(id, 0);
	}
}

void renderGameMenu(void)
{
	GameMenuSprite *sprite;
	int32_t digitCount;
	int32_t digits[6];
	int8_t disabled;
	int32_t highlight;
	int16_t yOffset;
	int32_t i;

	renderSeperatorLines(GAME_MENU_LINES, 2, 5);
	renderDateDigits();
	yOffset = 0;
	if (MAIN_D_80134D28 == 7) {
		yOffset = -0x28;
	}
	sprite = (GameMenuSprite *)GAME_MENU_SPRITES;
	renderTriangleCursor((int8_t)MAIN_D_80123E8C[0], yOffset);
	for (i = 1; i < MAIN_D_80134D28; sprite++, i++) {
		disabled = 0;
		if (sprite->disabled == 1) {
			disabled = 1;
		}
		highlight = 0;
		if ((i == MAIN_D_80123E8C[0]) &&
		    ((PLAYTIME_FRAMES % 10) < 5)) {
			highlight = 0x14;
		}
		renderRectPolyFT4(sprite->x - 0x42,
				  sprite->y - 0x50 + yOffset,
				  sprite->width, sprite->height,
				  sprite->texX + highlight,
				  sprite->texY + 0xc0, 0x1e,
				  GetClut(0x100, sprite->clutY + 0x1f0), 6,
				  disabled);
		renderRectPolyFT4(((GameMenuLabel *)GAME_MENU_LABELS)[i].x - 0x42,
				  yOffset + (((GameMenuLabel *)GAME_MENU_LABELS)[i].y - 0x50),
				  ((GameMenuLabel *)GAME_MENU_LABELS)[i].width,
				  ((GameMenuLabel *)GAME_MENU_LABELS)[i].height,
				  ((GameMenuLabel *)GAME_MENU_LABELS)[i].texX,
				  ((GameMenuLabel *)GAME_MENU_LABELS)[i].texY + 0xbf, 0x1e, 0x7f50, 6,
				  disabled);
	}
	renderString(3, -0x3c, -0x49, 0x24, 0xc, 0, 0xe7, 6, 1);
	renderString(3, 6, -0x49, 0x1c, 0xc, 0x24, 0xe7, 6, 1);
	convertValueToDigits(3, DAY, &digitCount, digits);
}

void renderSeperatorLines(int16_t *lines, int8_t count, int32_t zIndex)
{
	uint8_t *color;
	int32_t i;

	for (i = 0; i < count; lines += 5ULL, i++) {
		color = &MAIN_D_80123F48[((uint8_t *)lines)[8] * 3];
		renderLinePrimitive(color[0] | (color[1] << 8) |
				    (color[2] << 16),
				    lines[0], lines[1], lines[2], lines[3],
				    zIndex, 0);
	}
}

void renderDateDigits(void)
{
	int32_t digitCount;
	int32_t digits[6];
	int32_t digit;
	int32_t i;
	int32_t j;
	int16_t texX;
	int16_t texY;

	convertValueToDigits(2, DAY + 1, &digitCount, digits);
	for (i = digitCount - 1, j = 0; i >= 0; i--, j++) {
		texY = 0x34;
		if ((digit = *(int32_t *)&digits[i]) == 0) {
			texX = 0x78;
		} else if (digit < 5) {
			texX = (digits[i] - 1) * 8 + 0x60;
			texY = 0x28;
		} else {
			texX = (digits[i] - 5) * 8 + 0x50;
		}
		renderRectPolyFT4(0x25 + j * 9, -0x4a, 7, 0xc, texX + 1,
				  texY + 0xc0, 0x1e, 0x7f10, 5, 0);
	}

	convertValueToDigits(3, YEAR + 1, &digitCount, digits);
	for (i = digitCount - 1, j = 0; i >= 0; i--, j++) {
		texY = 0x34;
		if ((digit = *(int32_t *)&digits[i]) == 0) {
			texX = 0x78;
		} else if (digit < 5) {
			texX = (digits[i] - 1) * 8 + 0x60;
			texY = 0x28;
		} else {
			texX = (digits[i] - 5) * 8 + 0x50;
		}
		renderRectPolyFT4(-0x16 + j * 9, -0x4a, 7, 0xc, texX + 1,
				  texY + 0xc0, 0x1e, 0x7f10, 5, 0);
	}
}

void renderTriangleCursor(int32_t selection, int16_t yOffset)
{
	TriangleCursorUVData u0;
	TriangleCursorUVData u1;
	TriangleCursorUVData v0;
	TriangleCursorUVData v1;
	TriangleCursorOffsetData xOffset;
	TriangleCursorOffsetData yOffsetData;
	TriangleCursorOffsetData width;
	TriangleCursorOffsetData height;
	POLY_FT4 *prim;
	GsOT_TAG *tag;
	int16_t baseX;
	int16_t baseY;
	int32_t menuYOffset;
	int32_t i;

	u0 = MAIN_D_80134250;
	u1 = MAIN_D_80134258;
	v0 = MAIN_D_80134260;
	v1 = MAIN_D_80134268;
	xOffset = MAIN_D_80134270;
	yOffsetData = MAIN_D_80134278;
	width = MAIN_D_80134280;
	height = MAIN_D_80134288;
	baseX = *(int16_t *)((uint8_t *)MAIN_D_80123E88 + selection * 12) -
		0x46;
	baseY = *(int16_t *)((uint8_t *)MAIN_D_80123E8A + selection * 12) -
		0x53;
	i = 0;
	menuYOffset = yOffset;
	tag = &ACTIVE_ORDERING_TABLE->org[5];
	for (; i < 8; i++) {
		prim = (POLY_FT4 *)GsGetWorkBase();
		SetPolyFT4(prim);
		prim->u0 = (uint8_t)u0.data[i];
		prim->v0 = (uint8_t)v0.data[i];
		prim->u1 = (uint8_t)u1.data[i];
		prim->v1 = (uint8_t)v0.data[i];
		prim->u2 = (uint8_t)u0.data[i];
		prim->v2 = (uint8_t)v1.data[i];
		prim->u3 = (uint8_t)u1.data[i];
		prim->v3 = (uint8_t)v1.data[i];
		setPosDataPolyFT4(prim, baseX + xOffset.data[i],
				   baseY + yOffsetData.data[i] + menuYOffset,
				   width.data[i], height.data[i]);
		prim->r0 = 0x80;
		prim->g0 = 0x80;
		prim->b0 = 0x80;
		prim->tpage = GetTPage(0, 0, 0x380, 0x1c0);
		prim->clut = GetClut(0x100, 0x1fc);
		AddPrim(tag, prim);
		prim++;
		GsSetWorkBase((PACKET *)prim);
	}
}

void renderRectPolyFT4(int16_t posX, int16_t posY, int32_t width,
		       int32_t height, uint8_t texX, uint8_t texY,
		       int16_t texturePage, int16_t clut, int32_t zIndex,
		       int8_t flag)
{
	POLY_FT4 *prim;
	GsOT_TAG *tags;
	int32_t doubledWidth;
	uint32_t drawWidth;
	uint32_t drawHeight;
	int32_t x;
	int32_t y;
	int32_t bottom;
	int32_t left;
	int32_t right;
	int32_t top;

	tags = ACTIVE_ORDERING_TABLE->org;
	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	if (flag & 0x40) {
		SetSemiTrans(prim, 1);
	}
	if ((texY + height) >= 0x100) {
		height = (height - 1u) & 0xffu;
	}
	if ((texX + width) >= 0x100) {
		width = (width - 1u) & 0xffu;
	}
	setUVDataPolyFT4(prim, texX, texY, drawWidth = width, drawHeight = height);
	if (flag & 2) {
		setPosDataPolyFT4(prim, posX, posY, drawWidth * 2, drawHeight * 2);
	} else if (flag & 4) {
		setPosDataPolyFT4(prim, posX, posY, doubledWidth = drawWidth * 2, drawHeight * 2);
		prim->x0 += (int16_t)doubledWidth;
		prim->x1 -= (int16_t)doubledWidth;
		prim->x2 += (int16_t)doubledWidth;
		prim->x3 -= (int16_t)doubledWidth;
	} else {
		setPosDataPolyFT4(prim, posX, posY, drawWidth, drawHeight);
	}
	if (flag & 1) {
		prim->r0 = 0x32;
		prim->g0 = 0x32;
		prim->b0 = 0x32;
	} else {
		prim->r0 = 0x80;
		prim->g0 = 0x80;
		prim->b0 = 0x80;
	}
	prim->tpage = texturePage;
	prim->clut = clut;
	AddPrim(&tags[zIndex], prim);
	prim++;
	GsSetWorkBase((PACKET *)prim);
	if (flag & 0x80) {
		x = posX;
		y = posY;
		top = y - 1;
		right = x + 0xd;
		renderTrianglePrimitive(0x20202, left = x - 1, top, right, top, right,
					bottom = y + 0xc, 2, 0);
		renderTrianglePrimitive(0x20202, left, top, left, bottom,
					right, bottom, 2, 0);
	}
}

void tickGameMenu(void)
{
	int32_t selection;
	int32_t previousSelection;
	register int32_t input;
	int32_t pressed;

	if (PARTNER_PARA.condition & 1) {
		GAME_MENU_SPRITES[0x42] = 0;
	}
	selection = MAIN_D_80123E8C[0];
	previousSelection = selection;
	pressed = input = POLLED_INPUT & ~POLLED_INPUT_PREVIOUS;
	if (pressed & 0x1000) {
		if ((selection -= 3) <= 0) {
			selection += ((MAIN_D_80134D28 + 1) / 3) * 3;
		}
		if (selection >= MAIN_D_80134D28) {
			selection -= 3;
		}
	} else if (input & 0x4000) {
		if ((selection += 3) >= MAIN_D_80134D28) {
			selection -= ((MAIN_D_80134D28 + 1) / 3) * 3;
		}
		if (selection <= 0) {
			selection += 3;
		}
	} else if (input & 0x8000) {
		if ((selection -= 1) <= 0) {
			selection = MAIN_D_80134D28 - 1;
		}
	} else if (input & 0x2000) {
		if ((selection += 1) >= MAIN_D_80134D28) {
			selection = 1;
		}
	}
	if (selection != previousSelection) {
		MAIN_D_80123E8C[0] = selection;
		playSound(0, 2);
	}
	if (TRIANGLE_MENU_STATE == -1) {
		if (isKeyDown(0x40) != 0) {
			if (MAIN_D_80123E8E[MAIN_D_80123E8C[0] * 12] & 1) {
				playSound(0, 4);
			} else {
				playSound(0, 3);
			}
			handleGameMenuSelection(MAIN_D_80123E8C[0]);
		}
		if ((isKeyDown(0x10) != 0) &&
		    ((UI_BOX_DATA[0].state == 1) ||
		     (UI_BOX_DATA[0].frame == 0))) {
			playSound(0, 4);
			closeTriangleMenu();
			setTamerState(0);
			setCameraFollowPlayer();
			IS_IN_MENU = 0;
			startGameTime();
		}
	}
}

int32_t createMenuBox(int32_t id, int16_t x, int16_t y, int16_t width,
		      int16_t height, int8_t features, void (*tick)(void),
		      void (*render)(void))
{
	RECT finalPos;
	RECT startPos;
	int16_t entityPos[2];

	if (UI_BOX_DATA[id].state == 1) {
		return 1;
	}
	if (UI_BOX_DATA[id].frame == 0) {
		finalPos.x = x;
		finalPos.y = y;
		finalPos.w = width;
		finalPos.h = height;
		getEntityScreenPos(ENTITY_TABLE[0], 1, entityPos);
		startPos.x = entityPos[0] - 5;
		startPos.y = entityPos[1] - 5;
		startPos.w = 10;
		startPos.h = 10;
		createAnimatedUIBox(id, 1, features, &finalPos, &startPos,
				    (TickFunction)tick, (RenderFunction)render);
	}
	return 0;
}

void tickDigimonMenu(void)
{
	int8_t equippedSlot;
	int32_t slotResult;
	int16_t previousX;
	int16_t previousY;
	int32_t i;

	if ((MAIN_D_80134D36 != 1) ||
	    ((MAIN_D_80134D36 == 1) && (MENU_STATE == 1))) {
		if ((CHANGED_INPUT & 0x2000) && (MENU_STATE != 0)) {
			MAIN_D_80134D36++;
			if (MAIN_D_80134D36 >= 2) {
				MAIN_D_80134D36 = 1;
			} else {
				MENU_STATE = 0;
				MENU_SUB_STATE = 0;
				playSound(0, 2);
			}
		}
		if ((CHANGED_INPUT & 0x8000) && (MENU_STATE != 0)) {
			MAIN_D_80134D36--;
			if (MAIN_D_80134D36 < 0) {
				MAIN_D_80134D36 = 0;
			} else {
				MENU_STATE = 0;
				MENU_SUB_STATE = 0;
				playSound(0, 2);
			}
		}
		if (isKeyDown(0x10) != 0) {
			if (MENU_STATE == 1) {
				TRIANGLE_MENU_STATE = 4;
			}
			playSound(0, 4);
		}
		if ((MAIN_D_80134D36 == 1) && (isKeyDown(0x40) != 0)) {
			if (MENU_STATE == 1) {
				MENU_STATE = 2;
			}
			playSound(0, 3);
		}
	} else if (MENU_STATE == 6) {
		if (isKeyDown(0x10) != 0) {
			for (i = 0; i < 3; i++) {
				if ((PARTNER_ENTITY.digimonEntity.stats.base.moves[i] !=
				     0xff) &&
				    (MOVE_DATA[entityGetTechFromAnim(
					     &PARTNER_ENTITY.digimonEntity.entity,
					     PARTNER_ENTITY.digimonEntity.stats.base
						     .moves[i])]
					     .power != 0)) {
					break;
				}
			}
			if (((PARTNER_ENTITY.digimonEntity.stats.base.moves[0] !=
			      0xff) ||
			     (PARTNER_ENTITY.digimonEntity.stats.base.moves[1] !=
			      0xff) ||
			     (PARTNER_ENTITY.digimonEntity.stats.base.moves[2] !=
			      0xff)) &&
			    (i != 3)) {
				MENU_STATE = 4;
			}
			playSound(0, 4);
		} else if (isKeyDown(0x80) != 0) {
			MENU_STATE = 7;
			MENU_SUB_STATE = 0;
			playSound(0, 3);
		} else if (isKeyDown(0x40) != 0) {
			slotResult = getEquippedSlot();
			equippedSlot = (int8_t)slotResult;
			if ((int8_t)slotResult != -1) {
				MAIN_D_80134234[equippedSlot] = 0xff;
				PARTNER_ENTITY.digimonEntity.stats.base
					.moves[equippedSlot] = 0xff;
				playSound(0, 3);
			} else {
				equipMove();
			}
		}
		previousY = MAIN_D_80134D38;
		previousX = MAIN_D_80134D3A;
		if (CHANGED_INPUT & 0x1000) {
			MAIN_D_80134D38 -= 0xf;
		}
		if (CHANGED_INPUT & 0x4000) {
			MAIN_D_80134D38 += 0xf;
		}
		if (CHANGED_INPUT & 0x8000) {
			MAIN_D_80134D3A -= 0x12;
		}
		if (CHANGED_INPUT & 0x2000) {
			MAIN_D_80134D3A += 0x12;
		}
		if (MAIN_D_80134D3A < 0x73) {
			MAIN_D_80134D3A = 0x73;
		}
		if (MAIN_D_80134D3A >= 0xf2) {
			MAIN_D_80134D3A = 0xf1;
		}
		if (MAIN_D_80134D38 < 0x6f) {
			MAIN_D_80134D38 = 0x6f;
		}
		if (MAIN_D_80134D38 >= 0xca) {
			MAIN_D_80134D38 = 0xc9;
		}
		if ((previousX != MAIN_D_80134D3A) ||
		    (previousY != MAIN_D_80134D38)) {
			playSound(0, 2);
		}
	} else if ((MENU_STATE == 8) && (isKeyDown(0x10) != 0)) {
		MENU_STATE = 9;
		MENU_SUB_STATE = 0;
		playSound(0, 4);
	}
	TAMER_ENTITY.entity.isOnScreen = 0;
	PARTNER_ENTITY.digimonEntity.entity.isOnScreen = 0;
}

void renderDigimonMenu(void)
{
  int8_t tabs[2];
  int32_t v;
  MenuTabPair *tp;
  long nv;
  tp = (MenuTabPair *)MAIN_D_801342A0;
  nv = tp->digimon;
  v = tp->moves;
  tabs[0] = nv;
  tabs[1] = v;
  v = MAIN_D_80134D36;
  if (v != 1) {
    if (v == 0) {
      renderDigimonStatsView();
    }
  } else {
    renderDigimonMovesView();
  }
  tabs[MAIN_D_80134D36] = 0;
  renderString(tabs[0], -0x8A, -((short) 0x65), 0x3C, 0xC, 0, 0, 5, 1);
  renderString(tabs[1], -0x3F, -0x65, 0x30, 0xC, 0x3C, 0, 5, 1);
  nv = tabs[0];
  renderMenuTab(-0x91, 0x4C, nv);
  renderMenuTab(-0x46, 0x40, tabs[1]);
}

int32_t isUIBoxAvailable(int32_t id)
{
	if (UI_BOX_DATA[id].state == 1) {
		return 1;
	}

	if (UI_BOX_DATA[id].frame == 0) {
		return 1;
	}

	return 0;
}

void tickPlayerMenu(void)
{
	RECT finalPos;
	RECT secondFinalPos;
	RECT startPos;
	int16_t previousRow;
	int16_t previousColumn;
	int32_t maxColumn;
	int16_t selectorX;
	int32_t previousCard;
	int32_t i;

	if (MENU_STATE < 2) {
		if (CHANGED_INPUT & 0x2000) {
			MAIN_D_80134D37++;
			if (MAIN_D_80134D37 >= 4) {
				MAIN_D_80134D37 = 3;
			} else {
				MENU_STATE = 0;
				MENU_SUB_STATE = 0;
				playSound(0, 2);
			}
		}
		if (CHANGED_INPUT & 0x8000) {
			MAIN_D_80134D37--;
			if (MAIN_D_80134D37 < 0) {
				MAIN_D_80134D37 = 0;
			} else {
				MENU_STATE = 0;
				MENU_SUB_STATE = 0;
				playSound(0, 2);
			}
		}
		if (isKeyDown(0x10) != 0) {
			TRIANGLE_MENU_STATE = 6;
			playSound(0, 4);
		}
		if ((isKeyDown(0x40) != 0) && (MENU_STATE == 1) &&
		    (MAIN_D_80134D37 != 0) && (MAIN_D_80134D37 != 4)) {
			MENU_STATE = 2;
			playSound(0, 3);
			SELECTED_MEDAL = 0;
			MEDAL_SELECTOR_INDEX = 0;
			SELECTED_CARD = 0;
		}
	} else {
		if ((isKeyDown(0x10) != 0) && (MENU_STATE == 2)) {
			playSound(0, 4);
			MENU_STATE = 1;
			MAIN_D_80134D40 = -1;
		}

		if (MAIN_D_80134D37 == 1) {
			if (MENU_STATE == 2) {
				previousRow = MAIN_D_80134D42;
				previousColumn = MAIN_D_80134D44;
				if (CHANGED_INPUT & 0x8000) {
					MAIN_D_80134D42--;
				}
				if (CHANGED_INPUT & 0x2000) {
					MAIN_D_80134D42++;
				}
				if (MAIN_D_80134D42 < 0) {
					MAIN_D_80134D42 = 0;
				}
				if (MAIN_D_80134D42 >= 9) {
					MAIN_D_80134D42 = 8;
				}
				if ((MAIN_D_80134D42 == 0) ||
				    (MAIN_D_80134D42 == 1)) {
					if (CHANGED_INPUT & 0x1000) {
						MAIN_D_80134D44 -= 2;
					}
					if (CHANGED_INPUT & 0x4000) {
						MAIN_D_80134D44 += 2;
					}
					if (MAIN_D_80134D42 < 2) {
						MAIN_D_80134D44 =
							(MAIN_D_80134D44 / 2) * 2;
					}
					maxColumn = 6;
				} else {
					if (CHANGED_INPUT & 0x1000) {
						MAIN_D_80134D44--;
					}
					if (CHANGED_INPUT & 0x4000) {
						MAIN_D_80134D44++;
					}
					if (MAIN_D_80134D42 == 2) {
						maxColumn = 8;
					} else if (((MAIN_D_80134D42 >= 4) &&
						    (MAIN_D_80134D42 < 7)) ||
						   (MAIN_D_80134D42 == 8)) {
						maxColumn = 6;
					} else {
						maxColumn = 7;
					}
				}
				if (MAIN_D_80134D44 < 0) {
					MAIN_D_80134D44 = 0;
				}
				if (maxColumn < MAIN_D_80134D44) {
					MAIN_D_80134D44 = maxColumn;
				}
				if ((previousRow != MAIN_D_80134D42) ||
				    (previousColumn != MAIN_D_80134D44)) {
					playSound(0, 2);
				}

				if (MAIN_D_80134D42 < 3) {
					selectorX = MAIN_D_80134D42 * 0x25 + 0x1c;
				} else if (MAIN_D_80134D42 < 7) {
					selectorX =
						(MAIN_D_80134D42 - 3) * 0x18 + 0x8b;
				} else {
					selectorX =
						(MAIN_D_80134D42 - 7) * 0x18 + 0xf8;
				}
				MAIN_D_80134D40 = -1;
				for (i = 0; i < 0x3e; i++) {
					if (((selectorX + 2) ==
					     MAIN_D_80124544[i].posX) &&
					    ((MAIN_D_80134D44 * 0x13 + 0x2b) ==
					     MAIN_D_80124546[i].posX)) {
						break;
					}
				}
				MAIN_D_80134D40 = i + 1;
				if (isKeyDown(0x40) != 0) {
					if (hasDigimonRaised(
						    (uint16_t)MAIN_D_80134D40) != 0) {
						if (MAIN_D_80134D42 < 3) {
							selectorX =
								MAIN_D_80134D42 * 0x25 + 0x1c;
						} else if (MAIN_D_80134D42 < 7) {
							selectorX =
								(MAIN_D_80134D42 - 3) * 0x18 + 0x8b;
						} else {
							selectorX =
								(MAIN_D_80134D42 - 7) * 0x18 + 0xf8;
						}
						if (isUIBoxAvailable(2) == 1) {
							finalPos.x = -0x96;
							finalPos.y = -0x59;
							finalPos.w = 0x12c;
							finalPos.h = 0xbe;
							startPos.x = selectorX - 0x99;
							startPos.y =
								MAIN_D_80134D44 * 0x13 - 0x45;
							startPos.w = 10;
							startPos.h = 10;
							MAIN_D_80134D46 = 0;
							createAnimatedUIBox(
								2, 1, 0, &finalPos, &startPos,
								NULL,
								(RenderFunction)renderEvoChartDetail);
							playSound(0, 3);
							MENU_STATE = 3;
						}
					} else {
						playSound(0, 4);
					}
				}
			} else if ((MENU_STATE == 3) &&
				   (isKeyDown(0x10) != 0)) {
				if (MENU_STATE == 3) {
					if (MAIN_D_80134D42 < 3) {
						selectorX = MAIN_D_80134D42 * 0x25 + 0x1c;
					} else if (MAIN_D_80134D42 < 7) {
						selectorX =
							(MAIN_D_80134D42 - 3) * 0x18 + 0x8b;
					} else {
						selectorX =
							(MAIN_D_80134D42 - 7) * 0x18 + 0xf8;
					}
					finalPos.x = selectorX - 0x99;
					finalPos.y = MAIN_D_80134D44 * 0x13 - 0x45;
					finalPos.w = 10;
					finalPos.h = 10;
					removeAnimatedUIBox(2, &finalPos);
					playSound(0, 3);
					MENU_STATE = 2;
				}
			}
		}

		if (MAIN_D_80134D37 == 2) {
			if ((CHANGED_INPUT & 0x8000) &&
			    ((MEDAL_SELECTOR_INDEX % 5) != 0)) {
				MEDAL_SELECTOR_INDEX--;
			}
			if ((CHANGED_INPUT & 0x2000) &&
			    ((MEDAL_SELECTOR_INDEX % 5) != 4)) {
				MEDAL_SELECTOR_INDEX++;
			}
			if ((CHANGED_INPUT & 0x1000) &&
			    (MEDAL_SELECTOR_INDEX >= 5)) {
				MEDAL_SELECTOR_INDEX -= 5;
			}
			if ((CHANGED_INPUT & 0x4000) &&
			    (MEDAL_SELECTOR_INDEX < 10)) {
				MEDAL_SELECTOR_INDEX += 5;
			}
			if (SELECTED_MEDAL != MEDAL_SELECTOR_INDEX) {
				if (hasMedal(MEDAL_SELECTOR_INDEX) != 0) {
					activateMedalTexture(MEDAL_SELECTOR_INDEX,
							     SELECTED_MEDAL);
					MENU_STATE = 3;
				}
				playSound(0, 2);
			}
			SELECTED_MEDAL = MEDAL_SELECTOR_INDEX;
		}

		if (MAIN_D_80134D37 == 3) {
			previousCard = (uint8_t)SELECTED_CARD;
			if (MENU_STATE == 2) {
				if ((isKeyDown(0x40) != 0) &&
				    (getCardAmount((uint8_t)SELECTED_CARD) != 0)) {
					playSound(0, 3);
					MENU_STATE = 3;
					loadCardImage(SELECTED_CARD);
					startPos.x = (SELECTED_CARD % 11) * 0x18 - 0x79;
					startPos.y = (SELECTED_CARD / 11) * 0x18 - 0x36;
					startPos.w = 1;
					startPos.h = 1;
					finalPos.x = -0x4b;
					finalPos.y = -0x53;
					finalPos.w = 0x96;
					finalPos.h = 0xb4;
					secondFinalPos.x = 0x4a;
					secondFinalPos.y = 0x45;
					secondFinalPos.w = 0x36;
					secondFinalPos.h = 0x18;
					createAnimatedUIBox(
						2, 1, 0, &finalPos, &startPos, NULL,
						(RenderFunction)renderCardImage);
					createAnimatedUIBox(
						3, 1, 0, &secondFinalPos, &startPos,
						NULL, (RenderFunction)renderCardCount);
				}
				if (MENU_STATE == 2) {
					if ((CHANGED_INPUT & 0x8000) &&
					    ((SELECTED_CARD % 11) != 0)) {
						SELECTED_CARD--;
					}
					if ((CHANGED_INPUT & 0x2000) &&
					    ((SELECTED_CARD % 11) != 10)) {
						SELECTED_CARD++;
					}
					if ((CHANGED_INPUT & 0x1000) &&
					    (SELECTED_CARD >= 11)) {
						SELECTED_CARD -= 11;
					}
					if ((CHANGED_INPUT & 0x4000) &&
					    (SELECTED_CARD < 0x37)) {
						SELECTED_CARD += 11;
					}
					if (previousCard != SELECTED_CARD) {
						playSound(0, 2);
					}
				}
			}
			if ((isKeyDown(0x10) != 0) && (MENU_STATE == 3)) {
				playSound(0, 4);
				MENU_STATE = 2;
				startPos.x = (SELECTED_CARD % 11) * 0x18 - 0x79;
				startPos.y = (SELECTED_CARD / 11) * 0x18 - 0x36;
				startPos.w = 1;
				startPos.h = 1;
				removeAnimatedUIBox(2, &startPos);
				removeAnimatedUIBox(3, &startPos);
			}
		}
	}

	if (POLLED_INPUT == 0) {
		MAIN_D_80134D48 = 0;
	} else {
		MAIN_D_80134D48++;
	}
	TAMER_ENTITY.entity.isOnScreen = 0;
	PARTNER_ENTITY.digimonEntity.entity.isOnScreen = 0;
}

void renderPlayerMenu(void);

void renderPlayerMenu(void)
{
	int8_t tabs[4];
	int32_t v;
	int32_t t1;
	int32_t t2;
	int32_t t3;

	*(PlayerTabs *)tabs = MAIN_D_801342A4;
	v = MAIN_D_80134D37;
	if (v != 3) {
		if (v != 2) {
			if (v != 1) {
				if (v == 0) {
					renderPlayerInfoView();
				}
			} else {
				renderEvoChartView();
			}
		} else {
			renderMedalView();
		}
	} else {
		renderCardsView();
	}
	tabs[MAIN_D_80134D37] = 0;
	renderString(tabs[0], -0x89, -0x65, 0x3C, 0xC, 0, 0, 5, 1);
	renderString(t1 = tabs[1], -0x3E, -0x65, 0x3C, 0xC, 0x3C, 0, 5, 1);
	renderString(t2 = tabs[2], 0xD, -0x65, 0x24, 0xC, 0x78, 0, 5, 1);
	renderString(t3 = tabs[3], 0x40, -0x65, 0x24, 0xC, 0x9C, 0, 5, 1);
	renderMenuTab(-0x91, 0x4C, tabs[0]);
	renderMenuTab(-0x46, 0x4C, t1);
	renderMenuTab(5, 0x34, t2);
	renderMenuTab(0x38, 0x34, t3);
}

void handleGameMenuSelection(int32_t selection)
{
	switch (selection) {
	case 1:
		if (MAIN_D_80123E8E[selection * 12] & 1) {
			return;
		}
		TRIANGLE_MENU_STATE = 2;
		drawInventoryText();
		break;
	case 6:
		if (MAIN_D_80123E8E[selection * 12] & 1) {
			return;
		}
		closeTriangleMenu();
		setPartnerState(3);
		IS_IN_MENU = 0;
		startGameTime();
		break;
	case 5:
		if (MAIN_D_80123E8E[selection * 12] & 1) {
			return;
		}
		closeTriangleMenu();
		setPartnerState(4);
		IS_IN_MENU = 0;
		startGameTime();
		break;
	case 2:
		TRIANGLE_MENU_STATE = 3;
		break;
	case 3:
		TRIANGLE_MENU_STATE = 5;
		break;
	case 4:
		if (MAIN_D_80123E8E[selection * 12] & 1) {
			return;
		}
		closeTriangleMenu();
		setPartnerState(0xf);
		IS_IN_MENU = 0;
		startGameTime();
		break;
	case 7:
		if (MAIN_D_80123E8E[selection * 12] & 1) {
			return;
		}
		closeTriangleMenu();
		setCameraFollowPlayer();
		initializeFishing();
		setTamerState(0xb);
		IS_IN_MENU = 0;
	}
}

void setSleepDisabled(int32_t arg)
{
	GAME_MENU_SPRITES[0x42] = arg;
}

void startFeedingItem(int32_t arg)
{
	if (TAMER_ITEM.worldItem.type == 0xff) {
		TAMER_ITEM.worldItem.type = arg;
		setTamerState(6);
		setPartnerState(5);
		removeObject(0xfa4, 0);
		IS_IN_MENU = 0;
		startGameTime();
	}
}

void removeOneSelectedItem(void)
{
	removeItem(INVENTORY.types.array[INVENTORY_POINTER], 1);
}

void renderFeedingItem(int32_t arg)
{
	MATRIX *m;

	if (arg == 0) {
		m = &TAMER_ENTITY.entity.posData[9].posMatrix.workm;
	}

	TAMER_ITEM.worldItem.spriteLocation.vx = m->t[0];
	TAMER_ITEM.worldItem.spriteLocation.vy = m->t[1];
	TAMER_ITEM.worldItem.spriteLocation.vz = m->t[2];

	renderOverworldItem(&TAMER_ITEM.worldItem);
}

int32_t getEquippedSlot(void)
{
	uint8_t moveId;
	uint8_t column;
	uint8_t row;
	int32_t slot;

	column = (MAIN_D_80134D3A - 0x73) / 18;
	row = (MAIN_D_80134D38 - 0x6f) / 15;
	if (row == 1) {
		row = 5;
	} else if (row == 2) {
		row = 1;
	} else if (row == 3) {
		row = 4;
	} else if (row == 4) {
		row = 2;
	} else if (row == 5) {
		row = 3;
	}
	moveId = column + row * 8;
	if (moveId >= 0x30) {
		moveId++;
	}
	for (slot = 0; slot < 3; slot++) {
		if ((moveId == 0x2c) && (MAIN_D_80134234[slot] == 0x30)) {
			return (int8_t)slot;
		}
		if ((moveId == 0x37) && (MAIN_D_80134234[slot] == 0x39)) {
			return (int8_t)slot;
		}
		if (MAIN_D_80134234[slot] == moveId) {
			return (int8_t)slot;
		}
	}
	return -1;
}

void equipMove(void)
{
	RECT textArea;
	uint8_t column;
	uint8_t row;
	uint8_t moveId;
	int32_t slot;
	int32_t animation;
	int32_t textY;

	column = (MAIN_D_80134D3A - 0x73) / 18;
	row = (MAIN_D_80134D38 - 0x6f) / 15;
	if (row == 1) {
		row = 5;
	} else if (row == 2) {
		row = 1;
	} else if (row == 3) {
		row = 4;
	} else if (row == 4) {
		row = 2;
	} else if (row == 5) {
		row = 3;
	}
	moveId = column + row * 8;
	if (row == 6) {
		moveId++;
	}
	if (hasMove(moveId) == 0) {
		playSound(0, 4);
		return;
	}

	for (animation = 0; animation < 16; animation++) {
		if (moveId == 0x2c) {
			if (DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type]
				    .moves[animation] == 0x2c) {
				break;
			}
			if (DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type]
				    .moves[animation] == 0x30) {
				moveId = 0x30;
				break;
			}
		}
		if (moveId == 0x37) {
			if (DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type]
				    .moves[animation] == 0x37) {
				break;
			}
			if (DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type]
				    .moves[animation] == 0x39) {
				moveId = 0x39;
				break;
			}
		}
		if (moveId ==
		    DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type]
			    .moves[animation]) {
			break;
		}
		if (animation == 15) {
			playSound(0, 4);
			return;
		}
	}

	for (slot = 0; slot < 3; slot++) {
		if (MAIN_D_80134234[slot] == 0xff) {
			break;
		}
		if (slot == 2) {
			playSound(0, 4);
			return;
		}
	}
	MAIN_D_80134234[slot] = moveId;
	PARTNER_ENTITY.digimonEntity.stats.base.moves[slot] = animation + 0x2e;
	textArea.x = 0;
	textArea.y = textY = slot * 12 + 0x18;
	textArea.w = 0x84;
	textArea.h = 0xc;
	clearTextSubArea(&textArea);
	drawString(MOVE_NAMES[(uint8_t)moveId], 0, textY);
	playSound(0, 3);
}
