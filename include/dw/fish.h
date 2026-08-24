#ifndef DW_FISH_H
#define DW_FISH_H

#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/types.h>

typedef struct {
	DVECTOR *path;
	int32_t unk4;
	int32_t unk8;
} FishSeadra;

typedef struct {
	uint8_t unk0;
	uint8_t unk1;
	uint8_t unk2;
	uint8_t unk3;
	int32_t unk4;
	DVECTOR *unk8;
	uint8_t unkC;
	uint8_t unkD;
	uint8_t unkE;
	uint8_t unkF;
} FishingSpot;

typedef struct {
	int16_t mapId;
	int16_t unk2;
	int32_t unk4;
	int32_t unk8;
	int32_t unkC;
	int32_t unk10;
	int16_t seadramonTime1;
	int16_t seadramonTime2;
	FishingSpot *fishingSpots;
	int32_t unk1C;
	int32_t (*unk20)(int32_t x, int32_t y);
	void *unk24;
	int16_t unk28;
	int16_t unk2A;
	char *spriteFilePath;
	DVECTOR *seadramonWaypoints;
	int32_t unk34;
} FishingMapData;

typedef struct {
	int32_t objCount;
	uint8_t *bufferEnd;
	uint8_t *unk8;
	uint8_t *targetBuffer;
} TMDFileLoadingData;

typedef struct {
	uint8_t alive : 1;
	uint8_t kind : 4;
	uint8_t unk5 : 3;
	uint8_t unk1;
	int16_t size;
	int16_t transparency;
	uint16_t unk6;
	VECTOR pos;
	SVECTOR rotation;
	int32_t unk20;
	int32_t target;
	int32_t unk28;
	int32_t lifetime;
	int32_t timeNextMove;
	int32_t timeThisMove;
} Fish;

typedef struct {
	Fish items[33];
	Fish *next;
	int32_t count;
	int32_t unk740;
	int32_t frameCount;
	int32_t escapeTimer;
} FishPool;

typedef struct {
	VECTOR unk0;
	int32_t unk10;
	int32_t unk14;
	int32_t unk18;
	uint32_t unk1C;
	int32_t unk20;
} FishRipple;

typedef struct {
	int8_t unk0;
	int8_t unk1;
	int16_t unk2;
	VECTOR unk4;
} FishMarker;

typedef struct {
	FishRipple items[16];
	int32_t count;
	uint32_t unk244;
	FishRipple *next;
	int32_t unk24C;
} FishRipplePool;

typedef struct {
	FishMarker items[16];
	int32_t count;
	FishMarker *next;
} FishMarkerPool;

typedef union {
	int32_t word;
	uint8_t rgb[4];
} FishColor;

typedef struct {
	int32_t display;
	uint32_t unk4[2];
	int32_t level;
	int32_t unk10;
	int32_t limit;
	FishColor color;
} FishingTension;

typedef struct {
	int32_t shown;
	int16_t boxId;
	RECT startPos;
	RECT finalPos;
	int16_t unk16;
} FishingBox;

typedef struct {
	int32_t shown;
	int32_t unk4;
	int32_t unk8;
	int32_t unkC;
	int32_t unk10;
	int16_t unk14[30];
	int32_t unk50;
	int32_t unk54;
	RECT unk58;
	RECT unk60;
	int16_t boxId;
	int16_t unk6A;
	int32_t unk6C;
} FishingBaitMenu;

typedef struct {
	FishingBox box;
	uint32_t unk18[4];
	VECTOR scrollTo;
	int32_t isScrolling;
	int32_t unk3C;
} FishingView;

typedef struct {
	int32_t shown;
	VECTOR pos;
	int32_t sprite;
} FishingItem;

typedef struct {
	Fish fish;
	int32_t nibbleTime;
	int32_t catchingTime;
	int32_t animMode;
	int32_t powerLeft;
	int32_t powerMax;
	int32_t unk4C;
	int32_t escaped;
} HookedFish;

typedef struct {
	int32_t displayRod;
	int32_t fishingState;
	int32_t framesOnState;
	int32_t itemSlot;
	int32_t bait;
	int32_t throwPower;
	int32_t itemTaken;
	int32_t swimmerType;
	int32_t displaySwimmer;
	VECTOR lineStart;
	VECTOR lineStartMirror;
	int32_t displayLine;
	int32_t pressedFrames;
} FishingRod;

typedef struct {
	uint32_t header;
	FishingBaitMenu msg;
	FishPool pool;
	FishingRod rod;
	HookedFish hooked;
	FishingView view;
	FishingTension tension;
	FishingItem item;
	VECTOR swimmerLoc;
	uint32_t unk8E4[3];
	int32_t unk8F0;
	VECTOR unk8F4;
	int32_t unk904;
	int32_t unk908;
	int32_t unk90C;
	int32_t unk910;
	uint32_t unk914;
	uint8_t messageBuffer[32];
	TMDFileLoadingData rodModel;
	TMDFileLoadingData seadraModel;
	int32_t fishingEnabled;
	uint32_t mapId;
	FishingMapData *mapDataPtr;
	FishRipplePool pool1;
	FishMarkerPool pool2;
	FishSeadra seadra;
	uint8_t rodModelBuffer[4096];
	uint8_t seadraModelBuffer[4096];
	int32_t unk2D08;
	uint32_t trailer;
} FishingData;

extern FishingData *FISHING_DATA_PTR;
extern FishingMapData FISHING_MAP_DATA[];
extern FishingData FISHING_DATA;

void MAIN_func_800FF900(VECTOR *v);
int32_t getBestFishingRod(void);
int32_t hasFoodItem(void);
void setFishingDisabled(void);
void setFishingEnabled(void);
int32_t hasFishingRod(void);
void initializeFishing(void);
void tickFishing(void);
int32_t isFishing(void);
void deinitializeFishing(void);
void checkFishingMap(uint32_t mapId);

void FISH_loadFishing(FishingRod *fishingRod);
void FISH_init(void);
void FISH_deinitialize(void);
int32_t FISH_tamerTick(void);

#endif
