#include <stdlib.h>

#include <__rts_info_t__.h>

#include <libcd.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>

#include <dw/anim.h>
#include <dw/attack_object.h>
#include <dw/clock.h>
#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/font.h>
#include <dw/item.h>
#include <dw/main.h>
#include <dw/fade.h>
#include <dw/model.h>
#include <dw/params.h>
#include <dw/tamer.h>
#include <dw/ui.h>
#include <dw/types.h>
#include <dw/world_object.h>

#include "common.h"

int32_t isTriggerSet(uint16_t trigger);
void unsetTrigger(uint16_t trigger);



extern GsRVIEW2 GS_VIEWPOINT;


extern uint8_t INVENTORY_POINTER;

extern StatsGains INITIAL_COMBAT_STATS[];

extern DVECTOR MAIN_D_801344DC;
extern DVECTOR MAIN_D_801344E0;

extern GsOT_TAG GS_ORDERING_TABLE_0[];
extern GsOT_TAG GS_ORDERING_TABLE_1[];
extern GsOT GS_ORDERING_TABLE[2];

extern int32_t ACTIVE_FRAMEBUFFER;
typedef void (*ItemFunction)(int16_t);

extern int32_t MAIN_D_80134EAC;
typedef struct {
	int16_t spawnX[10];
	int16_t spawnY[10];
	int16_t spawnZ[10];
	int16_t rotation[10];
	int16_t targetMap[10];
	int16_t targetExit[10];
} MapWarps;

extern MapWarps MAP_WARPS;
extern uint8_t CURRENT_EXIT;
extern int8_t TAMER_START_TILE_X;
extern int8_t TAMER_START_TILE_Y;
extern int8_t TAMER_WAYPOINT_COUNT;
extern int8_t TAMER_WAYPOINT_X[];
extern int8_t TAMER_WAYPOINT_Y[];
extern int8_t GAME_STATE;
extern Entity *MAIN_D_80134D60;
extern uint8_t MAIN_D_80134D64;
extern int32_t MAIN_D_80134D74;
extern int32_t MAIN_D_80134D78;
extern int16_t MAIN_D_80134D68;
extern int32_t MAIN_D_80134D7C[2];
void BTL_getRemainingEnemies(Entity *self, int16_t *out, int16_t *count);
void BTL_drawCommandShout(uint8_t command);
extern int32_t MAIN_D_80134F0C;
extern int16_t FADE_IN_CURRENT;
extern int32_t CHECKED_MEMORY_CARD;
extern int32_t CURRENT_MENU;
extern int32_t TARGET_MENU;
extern char MAIN_D_8012CEA0[];
extern int32_t IS_SCRIPT_PAUSED;
extern GsRVIEW2 GS_VIEWPOINT_COPY;
extern int32_t VIEWPORT_DISTANCE;
extern int32_t VIEWPORT_DISTANCE_COPY;
extern int32_t DRAWING_OFFSET_X_COPY;
extern int32_t DRAWING_OFFSET_Y_COPY;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern PACKET GS_WORK_BASES[];
extern int16_t FADE_OUT_CURRENT;
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;

extern DR_OFFSET DRAW_OFFSETS[];

extern uint8_t MAP_LAYER_ENABLED;


void GsSetNearClip(long clip);

uint16_t MAIN_func_800F19B0(int32_t param_1, int32_t param_2);

extern GsOT *ACTIVE_ORDERING_TABLE;
extern int16_t MAIN_D_80134F10;
extern GsF_LIGHT LIGHT_DATA[3];

void renderMainMenuBackground(void);
void checkShopMap(int32_t mapId);
void entityLookAtTile(Entity *entity, int32_t tileX, int32_t tileY);
void getModelTile(VECTOR *pos, int16_t *outTileX, int16_t *outTileY);
void entityLookAtLocation(Entity *entity, VECTOR *pos);
void initializeInventoryObject(void);
uint32_t playSound(int32_t vabId, int32_t val);
void tickMainMenu(void);
void renderMainMenu(void);
void handlePause(void);
int32_t tickScript(void);
void updateTournamentRegistration(void);
void initializeNamingBuffer(uint8_t flags);
void MAIN_func_8010020C(void);
void MAIN_func_80100258(int32_t flag);
int32_t newGameStateMachine(void);
void processInput(void);
void loadNewgameScene(void);
void tickNewGameJijimon(int32_t instanceId);
void writePStat(int32_t stat, int32_t value);

void initializeHeap(void);
void initializeFramebuffer(void);
void view_init(void);
void initializeEffectData(void);
void addThrownItem(int32_t type);
void startThrowingItem(void);
void tickThrownItem(int32_t instanceId);
void renderThrownItem(int32_t instanceId);
void handleBuffDisks(int32_t type);
void MAIN_func_800F1794(void);
int32_t removeBuffModelObject(void);
void tickPartnerBattle(int32_t instanceId);
void tickNPCBattle(int32_t instanceId);
void unloadNewGameScene(void);
void initializeBuffModel(TMDModel *model);
void tickBuffModelObject(int32_t instanceId);
int32_t initializeBuffModelObject(void);

extern int32_t buffModelValue[2];
extern int32_t buffModelFrame;
extern TMDModel *BUFF_MODEL[];

void resetInventoryFlags(void);
void MAIN_func_800DF804(void);
void initializeInventoryModules(void);

void initializeEFE(void);
void initializeParticleFX(void);
void initializeEntityParticleFX(void);
void initializeCloudFXData(void);
void initializeHealingParticles(void);

void BTL_buffStats(Entity *entity, int32_t entityId, int32_t amount,
		   int16_t* valuePtr, uint8_t color, uint8_t icon);
void BTL_addBuffDiskEffect(Entity *entity);

void SsInit(void);
void PadInit(int32_t mode);
long MemCardInit(long val);
long MemCardStart(void);
void initializeGsTMDMap(void);
void initializeScripts(void);
void initializeStatusObjects(void);
void initializeMusic(void);
void finalizeMusic(void);
void playMovie(int32_t movieId, dw_bool shouldPlay);
void loadStackedTIMFile(char *path);
void runLandingScreen(void);
void runMainMenu(void);
void newGameScene(void);
void MAIN_func_800EF38C(void);
void recalculatePPandArena(void);
void gameLoop(void);
void cleanupGame(void);
void initializePartner(int32_t type, int32_t posX, int32_t posY,
		       int32_t posZ, int32_t rotX, int32_t rotY,
		       int32_t rotZ);
void setDigimonRaised(uint16_t type);
void MAIN_func_800D56E0(void);
void runMapHeadScript(int32_t scriptId);
int32_t readPStat(int32_t id);
void fadeFromBlack(int32_t frames);
void initializeUIBoxData(void);
void initializeMedalModel(void);
void initializeChest(void);
void initializeFileReadQueue(void);
void fillEFEXTable(void);
void initializeFadeData(void);
int32_t loadTIMFile(char *path, void *buffer);

extern int8_t MAIN_STATE;
extern int32_t MAIN_D_80134EB0;
extern char MAIN_D_8012CE64[];
extern char MAIN_D_8012CE78[];
extern char MAIN_D_8012CE8C[];
extern int32_t MAIN_D_80155670[];
extern uint8_t MAIN_D_80155725[];

int32_t main(void);
void applyHUDOffset(int32_t offset);
extern int32_t POLLED_INPUT;
extern int32_t POLLED_INPUT_PREVIOUS;
extern int32_t CHANGED_INPUT;
extern int32_t MAIN_D_80134EA4;
extern int32_t MAIN_D_80134EA8;
void pollInputGame(void);
void pollInputMenu(void);
void renderPressStartToContinue(void);
void tickTamerBattle(int32_t instanceId);
void MAIN_func_800F0B2C(void);
void MAIN_func_800F1020(void);
void MAIN_func_800F179C(void *model, int32_t compIdx, int32_t color);

/* Order anchor (reversed): pins symtab/section order to address order.
 * Unreferenced; discarded by --gc-sections. */
void *main_order_anchor[] = {
	removeBuffModelObject,
	initializeBuffModelObject,
	initializeBuffModel,
	MAIN_func_800F179C,
	MAIN_func_800F1794,
	tickBuffModelObject,
	unloadNewGameScene,
	loadNewgameScene,
	tickNewGameJijimon,
	tickNPCBattle,
	tickPartnerBattle,
	tickTamerBattle,
	MAIN_func_800F1020,
	MAIN_func_800F0B2C,
	handleBuffDisks,
	renderThrownItem,
	tickThrownItem,
	startThrowingItem,
	addThrownItem,
	initializeInventoryModules,
	initializeEffectData,
	view_init,
	renderMainMenuBackground,
	renderPressStartToContinue,
	pollInputMenu,
	pollInputGame,
	applyHUDOffset,
	gameLoop,
	recalculatePPandArena,
	MAIN_func_800EF38C,
	newGameScene,
	runMainMenu,
	runLandingScreen,
	initializeFramebuffer,
	initializeHeap,
	main,
};

int32_t main(void)
{
	int32_t partnerId;

	initializeHeap();
	SsInit();
	ResetCallback();
	ResetGraph(0);
	SetGraphDebug(0);
	initializeFramebuffer();
	PadInit(0);
	CdInit();
	MemCardInit(1);
	MemCardStart();
	initializeModelComponents();
	initializeWorldObjects();
	initializeGsTMDMap();
	initializeFileReadQueue();
	initializeAttackObjects();
	initializeFontCLUT();
	fillEFEXTable();
	initializeFadeData();
	initializeScripts();
	view_init();
	initializeUIBoxData();
	initializeEffectData();
	initializeInventoryModules();
	initializeMedalModel();
	initializeClockData();
	initializeStatusObjects();
	loadTIMFile(MAIN_D_8012CE64, GENERAL_BUFFER_PTR);

	for (;;) {
		loadTIMFile(MAIN_D_8012CE78, GENERAL_BUFFER_PTR);
		MAIN_D_80134EB0 = 0;
		while (MAIN_D_80134EB0 == 0) {
			playMovie(0, 1);
			initializeMusic();
			runLandingScreen();
			finalizeMusic();
		}

		loadStackedTIMFile(MAIN_D_8012CE8C);
		initializeMusic();
		runMainMenu();

		switch (MAIN_STATE) {
		case 0:
			newGameScene();
			finalizeMusic();
			playMovie(1, 1);
			initializeMusic();
			loadStackedTIMFile(MAIN_D_8012CE8C);
			initializeTamer(0, 0, 0, 0, 0, 0, 0);
			if (readPStat(0xfe) == 0) {
				partnerId = 0x3;
			} else {
				partnerId = 0x11;
			}
			initializePartner(partnerId, 0, 0, 0, 0, 0, 0);
			setDigimonRaised(partnerId);
			MAIN_func_800D56E0();
			initializeChest();
			runMapHeadScript(0xcc);
			addClock();
			stopGameTime();
			break;
		case 1:
			loadStackedTIMFile(MAIN_D_8012CE8C);
			initializeTamer(0, 0, 0, 0, 0, 0, 0);
			initializePartner(MAIN_D_80155670[0], 0, 0, 0, 0, 0, 0);
			MAIN_func_800D56E0();
			initializeChest();
			runMapHeadScript(MAIN_D_80155725[0]);
			MAIN_func_800EF38C();
			addClock();
			if (getTamerState() != 0) {
				setTamerState(0);
			}
			break;
		case 2:
			finalizeMusic();
			playMovie(2, 1);
			initializeMusic();
			loadStackedTIMFile(MAIN_D_8012CE8C);
			initializeTamer(0, 0, 0, 0, 0, 0, 0);
			initializePartner(MAIN_D_80155670[0], 0, 0, 0, 0, 0, 0);
			MAIN_func_800D56E0();
			initializeChest();
			runMapHeadScript(MAIN_D_80155725[0]);
			MAIN_func_800EF38C();
			addClock();
			if (getTamerState() != 0) {
				setTamerState(0);
			}
			break;
		}

		fadeFromBlack(0x28);
		recalculatePPandArena();
		while (MAIN_STATE != 3) {
			gameLoop();
		}
		cleanupGame();
		finalizeMusic();
		playMovie(3, 1);
	}
}

void initializeHeap(void)
{
	unsigned long heap;
	unsigned long stackTop;
	unsigned long stackFrames;

	heap = (_end & 0xfffffff0) + 0x10;
	stackTop = (unsigned long)_stack_addr;
	stackFrames = _stack_size;

	InitHeap3((unsigned long *)heap,
		  ((stackTop - (stackFrames * 0x400)) & 0xfffffff0) - heap);
}

void initializeFramebuffer(void)
{
	DVECTOR ofs0;
	DVECTOR ofs1;
	RECT rect;

	ofs0 = MAIN_D_801344DC;
	ofs1 = MAIN_D_801344E0;

	SetDispMask(0);
	GsInitGraph(320, 240, 4, 0, 0);
	GsDefDispBuff(0, 0, 0, 240);

	rect.x = 0;
	rect.y = 0;
	rect.w = 320;
	rect.h = 480;
	ClearImage(&rect, 0, 0, 0);

	GS_ORDERING_TABLE[0].length = 12;
	GS_ORDERING_TABLE[0].org = GS_ORDERING_TABLE_0;
	GS_ORDERING_TABLE[1].length = 12;
	GS_ORDERING_TABLE[1].org = GS_ORDERING_TABLE_1;

	GsInit3D();

	DRAWING_OFFSET_X = 160;
	DRAWING_OFFSET_Y = 120;

	SetDrawOffset(&DRAW_OFFSETS[0], (u_short *)&ofs1);
	SetDrawOffset(&DRAW_OFFSETS[1], (u_short *)&ofs0);

	MAP_LAYER_ENABLED = 1;

	SetDispMask(1);
}

void runLandingScreen(void)
{
	int32_t timer;

	timer = 0x258;
	addObject(0xfa3, 0, NULL, (RenderFunction)renderPressStartToContinue);
	FADE_OUT_CURRENT = 0;
	fadeFromBlack(0x28);

	while (--timer > 0) {
		pollInputMenu();

		if ((POLLED_INPUT & 0x800) != 0) {
			if ((FADE_OUT_CURRENT == 0) && (FADE_IN_CURRENT == 0)) {
				playSound(0, 3);
				MAIN_D_80134EB0 = 1;
				goto fade;
			}
		}

		ACTIVE_FRAMEBUFFER = GsGetActiveBuff();
		GsSetWorkBase(&GS_WORK_BASES[ACTIVE_FRAMEBUFFER * 0x14000]);
		GsClearOt(0, 0, &GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER]);
		ACTIVE_ORDERING_TABLE = &GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER];
		tickObjects();
		renderObjects();
		applyHUDOffset(ACTIVE_FRAMEBUFFER);
		DrawSync(0);
		VSync(0);
		ResetGraph(1);
		GsSetOrign(DRAWING_OFFSET_X, DRAWING_OFFSET_Y);
		GsSwapDispBuff();
		GsDrawOt(&GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER]);
	}

fade:
	fadeToBlack(0x28);

	while (FADE_OUT_CURRENT < 0x28) {
		ACTIVE_FRAMEBUFFER = GsGetActiveBuff();
		GsSetWorkBase(&GS_WORK_BASES[ACTIVE_FRAMEBUFFER * 0x14000]);
		GsClearOt(0, 0, &GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER]);
		ACTIVE_ORDERING_TABLE = &GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER];
		tickObjects();
		renderObjects();
		applyHUDOffset(ACTIVE_FRAMEBUFFER);
		DrawSync(0);
		VSync(0);
		ResetGraph(1);
		GsSetOrign(DRAWING_OFFSET_X, DRAWING_OFFSET_Y);
		GsSwapDispBuff();
		GsDrawOt(&GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER]);
	}

	removeObject(0xfa3, 0);
}

void runMainMenu(void)
{
	CHECKED_MEMORY_CARD = 0x10;
	CURRENT_MENU = -1;
	TARGET_MENU = 0;
	loadTIMFile(MAIN_D_8012CEA0, GENERAL_BUFFER_PTR);
	addObject(0x1388, 0, (TickFunction)tickMainMenu, (RenderFunction)renderMainMenu);
	addObject(0xfa3, 0, NULL, (RenderFunction)renderMainMenuBackground);
	fadeFromBlack(0x28);

	do {
		ACTIVE_FRAMEBUFFER = GsGetActiveBuff();
		ACTIVE_ORDERING_TABLE = &GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER];
		GsSetWorkBase(&GS_WORK_BASES[ACTIVE_FRAMEBUFFER * 0x14000]);
		GsClearOt(0, 0, &GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER]);
		AddPrim((char *)ACTIVE_ORDERING_TABLE->org + 0x80,
			&DRAW_OFFSETS[ACTIVE_FRAMEBUFFER]);
		pollInputMenu();
		tickObjects();
		renderObjects();
		DrawSync(0);
		VSync(0);
		ResetGraph(1);
		GsSwapDispBuff();
		GsSortClear(0, 0, 0, &GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER]);
		GsDrawOt(&GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER]);

		if ((CURRENT_MENU == -1) && (FADE_OUT_CURRENT == 0)) {
			fadeToBlack(0x28);
			removeObject(0x1388, 0);
		}
	} while ((CURRENT_MENU != -1) && (FADE_OUT_CURRENT < 0x28));

	removeObject(0xfa3, 0);
}

void newGameScene(void)
{
	int32_t done;

	done = 0;
	checkShopMap(0xda);
	initializeNamingBuffer(0);
	MAIN_func_8010020C();
	fadeFromBlack(0x14);
	writePStat(0xfe, 0);
	writePStat(0xf3, 0xff);
	loadNewgameScene();

	do {
		pollInputGame();
		ACTIVE_FRAMEBUFFER = GsGetActiveBuff();
		ACTIVE_ORDERING_TABLE = &GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER];
		GsSetWorkBase(&GS_WORK_BASES[ACTIVE_FRAMEBUFFER * 0x14000]);
		GsClearOt(0, 0, ACTIVE_ORDERING_TABLE);
		processInput();
		MAIN_func_80100258(1);

		if (readPStat(0xf3) == 0) {
			done = newGameStateMachine();
		}

		tickObjects();
		renderObjects();
		applyHUDOffset(ACTIVE_FRAMEBUFFER);
		DrawSync(0);
		VSync(3);
		GsSetOrign(DRAWING_OFFSET_X, DRAWING_OFFSET_Y);
		GsSwapDispBuff();
		GsSortClear(0, 0, 0, &GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER]);
		GsDrawOt(ACTIVE_ORDERING_TABLE);

		if ((done == 1) && (FADE_OUT_CURRENT == 0)) {
			fadeToBlack(0x28);
		}
	} while ((done == 0) || (FADE_OUT_CURRENT < 0x28));

	unloadNewGameScene();
}

INCLUDE_ASM("asm/main/nonmatchings/main", MAIN_func_800EF38C);

void recalculatePPandArena(void)
{
	uint8_t pp;
	int32_t i;

	pp = 0;
	for (i = 3; i < 0x3b; i++) {
		if ((DIGIMON_DATA[i].level >= 3) && (isTriggerSet((uint16_t)(0xc8 + i)) != 0)) {
			if ((i == 0xb) || (i == 0x27) || (i == 0x35)) {
				pp++;
			} else {
				pp += (uint8_t)(DIGIMON_DATA[i].level - 2);
			}
		}
	}
	writePStat(1, pp);
	pp = readPStat(3);
	if (pp >= 0x17) {
		if (isTriggerSet(0x25) != 0) {
			unsetTrigger(0x25);
		}
		if (isTriggerSet(0x26) != 0) {
			unsetTrigger(0x26);
		}
		if (isTriggerSet(0x27) != 0) {
			unsetTrigger(0x27);
		}
	}
}

void gameLoop(void)
{
	pollInputGame();
	ACTIVE_FRAMEBUFFER = GsGetActiveBuff();
	GsSetWorkBase(&GS_WORK_BASES[ACTIVE_FRAMEBUFFER * 0x14000]);
	GsClearOt(0, 0, &GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER]);
	ACTIVE_ORDERING_TABLE = &GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER];
	processInput();
	updateTournamentRegistration();

	if (IS_SCRIPT_PAUSED == 0) {
		tickScript();
	}

	GsSetOrign(DRAWING_OFFSET_X, DRAWING_OFFSET_Y);
	GsSetRefView2(&GS_VIEWPOINT);
	GsSetProjection(VIEWPORT_DISTANCE);
	GS_VIEWPOINT_COPY = GS_VIEWPOINT;
	VIEWPORT_DISTANCE_COPY = VIEWPORT_DISTANCE;
	DRAWING_OFFSET_X_COPY = DRAWING_OFFSET_X;
	DRAWING_OFFSET_Y_COPY = DRAWING_OFFSET_Y;
	tickObjects();
	renderObjects();
	applyHUDOffset(ACTIVE_FRAMEBUFFER);
	DrawSync(0);
	VSync(3);
	GsSwapDispBuff();
	GsSortClear(0, 0, 0, &GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER]);
	GsDrawOt(&GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER]);
	handlePause();
}

void applyHUDOffset(int32_t offset)
{
	AddPrim((char *)GS_ORDERING_TABLE[offset].org + 0x80,
		&DRAW_OFFSETS[offset]);
}

void pollInputGame(void)
{
	int32_t held;

	POLLED_INPUT_PREVIOUS = POLLED_INPUT;
	POLLED_INPUT = PadRead(0);
	CHANGED_INPUT = POLLED_INPUT & ~POLLED_INPUT_PREVIOUS;
	held = POLLED_INPUT & POLLED_INPUT_PREVIOUS & 0xf000f000;
	if (held != 0) {
		int32_t count;

		count = MAIN_D_80134EA4 + 1;
		MAIN_D_80134EA4 = count;
		if (count >= 5) {
			MAIN_D_80134EA4 -= 2;
		} else {
			held = 0;
		}
	} else {
		MAIN_D_80134EA4 = 0;
	}
	CHANGED_INPUT |= held;
}

void pollInputMenu(void)
{
	int32_t held;

	POLLED_INPUT_PREVIOUS = POLLED_INPUT;
	POLLED_INPUT = PadRead(0);
	CHANGED_INPUT = POLLED_INPUT & ~POLLED_INPUT_PREVIOUS;
	held = POLLED_INPUT & POLLED_INPUT_PREVIOUS & 0xf000f000;
	if (held != 0) {
		int32_t count;

		count = MAIN_D_80134EA8 + 1;
		MAIN_D_80134EA8 = count;
		if (count >= 0xf) {
			MAIN_D_80134EA8 -= 6;
		} else {
			held = 0;
		}
	} else {
		MAIN_D_80134EA8 = 0;
	}
	CHANGED_INPUT |= held;
}

void renderPressStartToContinue(void)
{
	GsOT_TAG *ot;
	POLY_FT4 *prim;

	ot = ACTIVE_ORDERING_TABLE->org;
	prim = (POLY_FT4 *)GsGetWorkBase();
	MAIN_D_80134EAC = MAIN_D_80134EAC + 1;

	if (MAIN_D_80134EB0 == 1) {
		MAIN_D_80134EAC = MAIN_D_80134EAC + 0x1d;
	}

	MAIN_D_80134EAC = MAIN_D_80134EAC % 0x3c;

	if (MAIN_D_80134EAC < 0x1e) {
		SetPolyFT4(prim);
		setXY4(prim, -0x36, 0x32, 0x43, 0x32, -0x36, 0x3c, 0x43, 0x3c);
		setUV4(prim, 0, 0xf1, 0x79, 0xf1, 0, 0xfb, 0x79, 0xfb);
		setRGB0(prim, 0, 0x80, 0);
		setTPage(prim, 1, 0, 768, 0);
		setClut(prim, 0, 480);
		AddPrim(&ot[30], prim++);
	}

	SetPolyFT4(prim);
	setXY4(prim, -0xa0, -0x78, 0x60, -0x78, -0xa0, 0x78, 0x60, 0x78);
	setUV4(prim, 0, 0, 0xff, 0, 0, 0xf0, 0xff, 0xf0);
	setRGB0(prim, 0x80, 0x80, 0x80);
	setTPage(prim, 1, 0, 768, 0);
	setClut(prim, 0, 480);
	AddPrim(ot = (GsOT_TAG *)((uint32_t)ot + 0x78), prim++);

	SetPolyFT4(prim);
	setXY4(prim, 0x60, -0x78, 0xa0, -0x78, 0x60, 0x78, 0xa0, 0x78);
	setUV4(prim, 0, 0, 0x40, 0, 0, 0xf0, 0x40, 0xf0);
	setRGB0(prim, 0x80, 0x80, 0x80);
	setTPage(prim, 1, 0, 896, 0);
	setClut(prim, 0, 480);
	AddPrim(ot, prim++);
	GsSetWorkBase((PACKET *)prim);
}

void renderMainMenuBackground(void)
{
	GsOT_TAG *ot;
	POLY_FT4 *prim;

	ot = ACTIVE_ORDERING_TABLE->org;
	prim = (POLY_FT4 *)GsGetWorkBase();

	SetPolyFT4(prim);
	setXY4(prim, -0xa0, -0x78, 0x60, -0x78, -0xa0, 0x78, 0x60, 0x78);
	setUV4(prim, 0, 0, 0xff, 0, 0, 0xf0, 0xff, 0xf0);
	setRGB0(prim, 0x80, 0x80, 0x80);
	setTPage(prim, 1, 0, 768, 0);
	setClut(prim, 0, 480);
	AddPrim(ot = (GsOT_TAG *)((uint32_t)ot + 0x78), prim++);

	SetPolyFT4(prim);
	setXY4(prim, 0x60, -0x78, 0xa0, -0x78, 0x60, 0x78, 0xa0, 0x78);
	setUV4(prim, 0, 0, 0x40, 0, 0, 0xf0, 0x40, 0xf0);
	setRGB0(prim, 0x80, 0x80, 0x80);
	setTPage(prim, 1, 0, 896, 0);
	setClut(prim, 0, 480);
	AddPrim(ot, prim++);
	GsSetWorkBase((PACKET *)prim);
}

static void view_init__garbage__(void)
{
	int32_t v0;
	int32_t v1;
	int32_t v2;
	int32_t v3;

	v0 = MAIN_D_80155670[0] + 0;
	v1 = MAIN_D_80155670[1] + 1;
	v2 = MAIN_D_80155670[2] + 2;
	v3 = MAIN_D_80155670[0] + 3;
	MAIN_D_80155670[0] = ((v0 * v1) + v2);
	MAIN_D_80155670[1] = ((v1 * v2) + v3);
	MAIN_D_80155670[2] = ((v2 * v3) + v0);
	MAIN_D_80155670[0] = ((v3 * v0) + v1);
}
void view_init(void)
{
	GsSetProjection(0x400);
	GS_VIEWPOINT.vpx = 0;
	GS_VIEWPOINT.vpy = -0xb73;
	GS_VIEWPOINT.vpz = -0xb73;
	GS_VIEWPOINT.vpy = 0;
	GS_VIEWPOINT.vpz = -0x7d0;
	GS_VIEWPOINT.vrx = 0;
	GS_VIEWPOINT.vry = 0;
	GS_VIEWPOINT.vrz = 0;
	GS_VIEWPOINT.rz = 0;
	GS_VIEWPOINT.super = 0;
	GsSetRefView2(&GS_VIEWPOINT);
	GsSetNearClip(0x64);
}

void initializeEffectData(void)
{
	initializeEFE();
	initializeParticleFX();
	initializeEntityParticleFX();
	initializeCloudFXData();
	initializeHealingParticles();
}

void initializeInventoryModules(void)
{
	COMBAT_DATA_PTR = &COMBAT_DATA;
	resetFlattenGlobal();
	initializeDroppedItems();
	resetInventoryFlags();
	initializeInventory();
	MAIN_func_800DF804();
}

void addThrownItem(int32_t type)
{
	MATRIX *workm;

	if (TAMER_ITEM.worldItem.type == 0xff) {
		workm = &TAMER_ENTITY.entity.posData[9].posMatrix.workm;
		TAMER_ITEM.worldItem.spriteLocation.vx = workm->t[0];
		TAMER_ITEM.worldItem.spriteLocation.vy = workm->t[1];
		TAMER_ITEM.worldItem.spriteLocation.vz = workm->t[2];
		TAMER_ITEM.time = 0;
		TAMER_ITEM.worldItem.type = type;
		addObject(0x194, 0, tickThrownItem, renderThrownItem);
	}
}

void startThrowingItem(void)
{
	startAnimation(ENTITY_TABLE[0], 6);
	addThrownItem(INVENTORY.types.array[INVENTORY_POINTER]);
	removeItem(INVENTORY.types.array[INVENTORY_POINTER], 1);
}

INCLUDE_ASM("asm/main/nonmatchings/main", tickThrownItem);

void renderThrownItem(int32_t instanceId)
{
	renderOverworldItem(&TAMER_ITEM.worldItem);
}

void handleBuffDisks(int32_t type)
{
	BaseStats *stats;
	CurrentStats *current;
	int16_t offLimit;
	int16_t defLimit;
	int16_t speedLimit;
	int16_t gain;

	current = &PARTNER_ENTITY.digimonEntity.stats.current;

	if (current->currentHP == 0) {
		return;
	}

	offLimit = (INITIAL_COMBAT_STATS[0].offense +
		    ((INITIAL_COMBAT_STATS[0].offense * 3) / 10));
	if (offLimit > 999) {
		offLimit = 999;
	}

	defLimit = (INITIAL_COMBAT_STATS[0].defense +
		    ((INITIAL_COMBAT_STATS[0].defense * 3) / 10));
	if (defLimit > 999) {
		defLimit = 999;
	}

	speedLimit = (INITIAL_COMBAT_STATS[0].speed +
		      ((INITIAL_COMBAT_STATS[0].speed * 3) / 10));
	if (speedLimit > 999) {
		speedLimit = 999;
	}

	stats = &PARTNER_ENTITY.digimonEntity.stats.base;

	switch(type) {
	case 0xf:
		if (offLimit < (stats->off + 20)) {
			gain = offLimit - stats->off;
		}
		else {
			gain = 20;
		}
		BTL_buffStats(ENTITY_TABLE[1], 0, gain, &stats->off, 0xb, 3);
		break;
	case 0x10:
		if (defLimit < (stats->def + 20)) {
			gain = defLimit - stats->def;
		}
		else {
			gain = 20;
		}
		BTL_buffStats(ENTITY_TABLE[1], 0, gain, &stats->def, 0xb, 4);
		break;
	case 0x11:
		if (speedLimit < (stats->speed + 20)) {
			gain = speedLimit - stats->speed;
		}
		else {
			gain = 20;
		}
		BTL_buffStats(ENTITY_TABLE[1], 0, gain, &stats->speed, 0xb, 5);
		break;
	case 0x12:
		BTL_buffStats(ENTITY_TABLE[1], 0, 20, &stats->off, 0xb, 3);
		BTL_buffStats(ENTITY_TABLE[1], 0, 20, &stats->def, 0xb, 4);
		BTL_buffStats(ENTITY_TABLE[1], 0, 20, &stats->speed, 0xb, 5);
		break;
	case 0x13:
		if (offLimit < (stats->off + 50)) {
			gain = offLimit - stats->off;
		}
		else {
			gain = 50;
		}
		BTL_buffStats(ENTITY_TABLE[1], 0, gain, &stats->off, 0xb, 3);
		break;
	case 0x14:
		if (defLimit < (stats->def + 50)) {
			gain = defLimit - stats->def;
		}
		else {
			gain = 50;
		}
		BTL_buffStats(ENTITY_TABLE[1], 0, gain, &stats->def, 0xb, 4);
		break;
	case 0x15:
		if (speedLimit < (stats->speed + 50)) {
			gain = speedLimit - stats->speed;
		}
		else {
			gain = 50;
		}
		BTL_buffStats(ENTITY_TABLE[1], 0, gain, &stats->speed, 0xb, 5);
		break;
	}

	BTL_addBuffDiskEffect(ENTITY_TABLE[1]);
}

void MAIN_func_800F0B2C(void)
{
	SVECTOR rot;
	VECTOR target;
	VECTOR in;
	VECTOR out;
	MATRIX m;
	int16_t enemies[4];
	int16_t tileX;
	int16_t tileY;
	int16_t count;
	int32_t cmd;

	if (MAIN_D_80134D78 == 1) {
		return;
	}

	if (UI_BOX_DATA[0].state != 0) {
		return;
	}

	if (COMBAT_DATA_PTR->player.currentCommand[0] == 1) {
		if ((MAIN_D_80134D68 == 0x14) &&
		    ((PARTNER_ENTITY.digimonEntity.stats.current.currentHP -
		      COMBAT_DATA_PTR->fighter[0].hpDamageBuffer) > 0)) {
			fadeToBlack(0x14);
		}

		MAIN_func_800F1020();
		MAIN_D_80134D68 = MAIN_D_80134D68 + 1;

		return;
	}

	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x8000) {
		playSound(0, 2);
		COMBAT_DATA_PTR->player.hoveredCommand[0]++;

		if ((MAIN_D_80134D7C[0] != 0) &&
		    (COMBAT_DATA_PTR->player.hoveredCommand[0] == 1)) {
			COMBAT_DATA_PTR->player.hoveredCommand[0]++;
		}

		if ((COMBAT_DATA_PTR->player.numCommands[0] - 1) <
		    COMBAT_DATA_PTR->player.hoveredCommand[0]) {
			if (MAIN_D_80134D7C[0] != 0) {
				COMBAT_DATA_PTR->player.hoveredCommand[0] = 2;
			} else {
				COMBAT_DATA_PTR->player.hoveredCommand[0] = 1;
			}
		}
	}

	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x2000) {
		playSound(0, 2);
		COMBAT_DATA_PTR->player.hoveredCommand[0]--;

		if ((MAIN_D_80134D7C[0] != 0) &&
		    (COMBAT_DATA_PTR->player.hoveredCommand[0] == 1)) {
			COMBAT_DATA_PTR->player.hoveredCommand[0]--;
		}

		if (COMBAT_DATA_PTR->player.hoveredCommand[0] <= 0) {
			COMBAT_DATA_PTR->player.hoveredCommand[0] =
				COMBAT_DATA_PTR->player.numCommands[0] - 1;
		}
	}

	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x40) {
		playSound(0, 3);
		BTL_getRemainingEnemies(ENTITY_TABLE[1], enemies, &count);

		if (count == 0) {
			return;
		}

		if (ENTITY_TABLE[0]->anim.animId != 0xe) {
			startAnimation(ENTITY_TABLE[0], 0xe);
		}

		COMBAT_DATA_PTR->player.bufferedCommand[0] =
			COMBAT_DATA_PTR->player.availableCommands[0][COMBAT_DATA_PTR->player.hoveredCommand[0]];

		if (PARTNER_PARA.discipline < 0x46) {
			COMBAT_DATA_PTR->player.commandDelay[0] = 0xa0 - (PARTNER_PARA.discipline / 10);
		} else {
			COMBAT_DATA_PTR->player.commandDelay[0] = (0xa - (PARTNER_PARA.discipline / 10)) * 10;
		}

		COMBAT_DATA_PTR->player.commandDelay[0] = 0;
		cmd = COMBAT_DATA_PTR->player.bufferedCommand[0];

		if (cmd == 7) {
			goto changeTarget;
		}

		if (cmd == 1) {
			COMBAT_DATA_PTR->player.commandDelay[0] = 0;
			COMBAT_DATA_PTR->player.currentCommand[0] = 1;
			getModelTile(&ENTITY_TABLE[0]->posData->location, &tileX, &tileY);

			if ((tileX == TAMER_START_TILE_X) && (tileY == TAMER_START_TILE_Y)) {
				target.vx = MAP_WARPS.spawnX[CURRENT_EXIT];
				target.vy = 0;
				target.vz = MAP_WARPS.spawnZ[CURRENT_EXIT];
				in.vx = 0;
				in.vy = 0;
				in.vz = -0xbb8;
				rot.vx = 0;
				rot.vy = (MAP_WARPS.rotation[CURRENT_EXIT] + 0x800) & 0xfff;
				rot.vz = 0;
				RotMatrix(&rot, &m);
				ApplyMatrixLV(&m, &in, &out);
				target.vx = target.vx + out.vx;
				target.vz = target.vz + out.vz;
				entityLookAtLocation(ENTITY_TABLE[0], &target);
			}

			startAnimation(ENTITY_TABLE[0], 3);
		}

		goto shout;

	changeTarget:
		COMBAT_DATA_PTR->player.changeTarget = 1;

	shout:
		BTL_drawCommandShout(COMBAT_DATA_PTR->player.bufferedCommand[0]);
	}

	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x80) {
		if (COMBAT_DATA_PTR->fighter[0].finisherProgress ==
		    COMBAT_DATA_PTR->fighter[0].finisherGoal) {
			COMBAT_DATA_PTR->player.bufferedCommand[0] = 0xb;
			playSound(0, 3);
			COMBAT_DATA_PTR->player.commandDelay[0] = 0;
			COMBAT_DATA_PTR->player.currentCommand[0] = 0xb;
			BTL_drawCommandShout(COMBAT_DATA_PTR->player.bufferedCommand[0]);
		}
	}
}

void MAIN_func_800F1020(void)
{
	SVECTOR rot;
	MATRIX m;
	VECTOR target;
	VECTOR in;
	VECTOR out;
	int16_t tileX;
	int16_t tileY;
	int16_t i;

	i = TAMER_WAYPOINT_COUNT - 1;
	getModelTile(&ENTITY_TABLE[0]->posData->location, &tileX, &tileY);

	if (i >= 0) {
		entityLookAtTile(ENTITY_TABLE[0], TAMER_WAYPOINT_X[i], TAMER_WAYPOINT_Y[i]);

		if ((tileX == TAMER_WAYPOINT_X[i]) && (tileY == TAMER_WAYPOINT_Y[i])) {
			TAMER_WAYPOINT_COUNT = TAMER_WAYPOINT_COUNT - 1;
		}

		return;
	}

	entityLookAtTile(ENTITY_TABLE[0], TAMER_START_TILE_X, TAMER_START_TILE_Y);

	if ((tileX == TAMER_START_TILE_X) && (tileY == TAMER_START_TILE_Y)) {
		target.vx = MAP_WARPS.spawnX[CURRENT_EXIT];
		target.vy = 0;
		target.vz = MAP_WARPS.spawnZ[CURRENT_EXIT];
		in.vx = 0;
		in.vy = 0;
		in.vz = -0xbb8;
		rot.vx = 0;
		rot.vy = (MAP_WARPS.rotation[CURRENT_EXIT] + 0x800) & 0xfff;
		rot.vz = 0;
		RotMatrix(&rot, &m);
		ApplyMatrixLV(&m, &in, &out);
		target.vx = target.vx + out.vx;
		target.vz = target.vz + out.vz;
		entityLookAtLocation(ENTITY_TABLE[0], &target);
		ENTITY_TABLE[0]->anim.animFlag |= 2;
	}
}

void tickTamerBattle(int32_t instanceId)
{
	Entity *tamer;
	Entity *partner;
	int32_t anim;

	tamer = ENTITY_TABLE[instanceId];

	if (GAME_STATE == 1) {
		if (((uint8_t *)COMBAT_DATA_PTR)[0x64e] != 1) {
			partner = ENTITY_TABLE[1];

			if (MAIN_D_80134D78 == 0) {
				if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x10) != 0) {
					initializeInventoryObject();
				}
			}

			entityLookAtLocation(tamer, &partner->posData->location);

			if (UI_BOX_DATA[0].state == 0) {
				if ((MAIN_D_80134D74 == 0) || (MAIN_D_80134D60 != ENTITY_TABLE[1])) {
					if ((tamer->anim.animId == 6) || ((anim = tamer->anim.animId) == 0xe)) {
						if ((tamer->anim.animFlag & 1) == 0) {
							startAnimation(tamer, 1);
						}
					} else if (tamer->anim.animId != 1) {
						startAnimation(tamer, 1);
					}
				} else if (tamer->anim.animId != 0xa) {
					startAnimation(tamer, 0xa);
				}
			}
		}

		if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x100) != 0) {
			MAIN_D_80134D64 = (MAIN_D_80134D64 + 1) & 1;
		}

		MAIN_func_800F0B2C();
	}

	if (ENTITY_TABLE[0]->anim.animId == 1) {
		MAIN_D_80134F0C = MAIN_D_80134F0C + 1;
	} else {
		MAIN_D_80134F0C = 0;
	}

	if (MAIN_D_80134F0C >= 0xab) {
		startAnimation(ENTITY_TABLE[0], 1);
		MAIN_D_80134F0C = 0;
	}

	tickAnimation(tamer);
}

void tickPartnerBattle(int32_t instanceId)
{
	tickAnimation(ENTITY_TABLE[instanceId]);
}

void tickNPCBattle(int32_t instanceId)
{
	Entity *entity;

	tickAnimation(ENTITY_TABLE[instanceId]);
	entity = ENTITY_TABLE[instanceId];
	if (entity->anim.animFlag & 4) {
		tickAnimation(entity);
	}
}

void tickNewGameJijimon(int32_t instanceId)
{
	int16_t *p;

	p = &MAIN_D_80134F10;
	if (*p < 0x7530) {
		*p += 1;
	}
	if (*p == 0x23) {
		setEntityRotation(2, 0, 0x71, 0);
		setupEntityMatrix(2);
		startAnimation(ENTITY_TABLE[2], 0);
		writePStat(0xf3, 0);
	}
	tickAnimation(ENTITY_TABLE[2]);
}

void loadNewgameScene(void)
{
	GsRVIEW2 view;

	thunkLoadMMD(0x75, 0);
	ENTITY_TABLE[2] = &NPC_ENTITIES[0].digimonEntity.entity;
	initializeDigimonObject(0x75, 2, tickNewGameJijimon);
	ENTITY_TABLE[2]->isOnMap = 1;
	ENTITY_TABLE[2]->isOnScreen = 1;
	MAIN_D_80134F10 = 0;
	setEntityPosition(2, 0x320, 0x96, 0);
	setEntityRotation(2, 0, 0x400, 0);
	setupEntityMatrix(2);
	startAnimation(ENTITY_TABLE[2], 2);
	GsSetProjection(0x3e8);
	view.vpx = 0;
	view.vpz = -0x1068;
	view.vpy = 0;
	view.vrx = 0;
	view.vry = 0;
	view.vrz = 0;
	view.rz = 0;
	view.super = NULL;
	GsSetRefView2(&view);
	DRAWING_OFFSET_X = 0xa0;
	DRAWING_OFFSET_Y = 0xb9;
	LIGHT_DATA[0].vx = 0x1e;
	LIGHT_DATA[0].vy = 0x64;
	LIGHT_DATA[0].vz = 0x1e;
	LIGHT_DATA[0].r = 0x40;
	LIGHT_DATA[0].g = 0x40;
	LIGHT_DATA[0].b = 0x40;
	GsSetFlatLight(0, &LIGHT_DATA[0]);
	LIGHT_DATA[1].vx = -0x1e;
	LIGHT_DATA[1].vy = 0x64;
	LIGHT_DATA[1].vz = 0;
	LIGHT_DATA[1].r = 0x28;
	LIGHT_DATA[1].g = 0x28;
	LIGHT_DATA[1].b = 0x28;
	GsSetFlatLight(1, &LIGHT_DATA[1]);
	LIGHT_DATA[2].vx = 0;
	LIGHT_DATA[2].vy = 0x64;
	LIGHT_DATA[2].vz = -0x1e;
	LIGHT_DATA[2].r = 0x26;
	LIGHT_DATA[2].g = 0x26;
	LIGHT_DATA[2].b = 0x26;
	GsSetFlatLight(2, &LIGHT_DATA[2]);
	GsSetAmbient(0x800, 0x800, 0x800);
}

void unloadNewGameScene(void)
{
	removeEntity(0x75, 2);
	thunkUnloadModel(0x75, 0);
}

void tickBuffModelObject(int32_t instanceId)
{
	MAIN_func_800F179C(BUFF_MODEL[0], 5, buffModelValue[buffModelFrame & 1]);
	buffModelFrame += 1;
}

void MAIN_func_800F1794(void)
{
}

void MAIN_func_800F179C(void *model, int32_t compIdx, int32_t color)
{
  char *p;
  int32_t new_var;
  int32_t new_var2;
  char *comp;
  int32_t i;
  int32_t attr;
  int new_var3;
  comp = (char *) model;
  comp = comp + 0xc;
  comp = comp + (compIdx * 0x1c);
  new_var = *((int32_t *) (comp + 0x14));
  new_var3 = 0x10;
  p = *((char **) (comp + new_var3));
  for (i = 0; i < new_var; i++)
  {
    attr = ((*((int32_t *) p)) >> 0x18) & 0xff;
    if (attr & 4)
    {
      attr = ((*((int32_t *) p)) >> 0x18) & 0xff;
      *((int16_t *) (p + 6)) = (int16_t) color;
      new_var2 = attr;
      if (new_var2 & 8)
      {
        if (!(attr & new_var3))
        {
          p += 0x20;
        }
        else
        {
          p += 0x2c;
        }
      }
      else
        if (!(attr & new_var3))
      {
        p += 0x1c;
      }
      else
      {
        p += 0x24;
      }
    }
  }

}

void initializeBuffModel(TMDModel *model)
{
	BUFF_MODEL[0] = model;
	GsMapModelingData((unsigned long *)&BUFF_MODEL[0]->flags);
	buffModelValue[0] = 0x7acc;
	buffModelValue[1] = 0x7b0c;
}

int32_t initializeBuffModelObject(void)
{
	buffModelFrame = 0;
	return addObject(0x501, 0, tickBuffModelObject,
			 (RenderFunction)MAIN_func_800F1794);
}

int32_t removeBuffModelObject(void)
{
	return removeObject(0x501, 0);
}
