#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include <libgs.h>
#include <libgte.h>

#include <dw/anim.h>
#include <dw/entity.h>
#include <dw/evl.h>
#include <dw/file.h>
#include <dw/file_queue.h>
#include <dw/model.h>
#include <dw/params.h>
#include <dw/world_object.h>

extern ModelComponent NPC_MODEL[5];
extern ModelComponent TAMER_MODEL;
extern ModelComponent PARTNER_MODEL;
extern int32_t NPC_MODEL_TAKEN[5];
extern int32_t UNKNOWN_MODEL_TAKEN[16];
extern uint8_t PARTNER_MODEL_BUFFER[];
extern uint8_t TAMER_MODEL_BUFFER[];

extern char MAIN_D_8011D190[];
extern char MAIN_D_8011D484[];
extern char MAIN_D_801340E4[5];
extern char *PTR_DIGIMON_FILE_NAMES[];
extern GsOT *ACTIVE_ORDERING_TABLE;
void renderDropShadow(Entity *entity);
void setRotTransMatrix(MATRIX *m);
int32_t add3DSpritePrim(POLY_FT4 *poly, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3);
void updateTMDTextureData(char *tmd, int32_t clutX, int32_t x, int32_t y, int32_t tpage);
extern PositionData TAMER_POSITION_DATA[];
extern PositionData PARTNER_POSITION_DATA[];
extern MomentumData TAMER_MOMENTUM_DATA[];
extern MomentumData PARTNER_MOMENTUM_DATA[];
extern char MAIN_D_8011D46C[];
extern char MAIN_D_8011D478[];
extern char MAIN_D_801340EC[5];
extern char MAIN_D_801340F4[5];

void handleNullModel(void);
void concatStrings(char *dst, char *src1, char *src2);
void initializePosData(PositionData *posData);
void loadDigimonTexture(int32_t digiType, char *path,
			ModelComponent *component);
ModelComponent *loadMMD(int32_t digiType, int32_t modelType);
void uploadModelTexture(void *textureData, ModelComponent *component);

void renderFlatDigimon(Entity* entity);
void renderDigimon(/* int32_t instanceId */);
void renderWireframed(GsDOBJ2* obj, int32_t wireFrameShare);
void uploadModelTexture(void *textureData, ModelComponent *component);

static void *model_functions[] = {
	applyMMD,
	loadMMDAsync,
	uploadModelTexture,
	getEntityType,
	getEntityModelComponent,
	unloadModel,
	loadMMD,
	initializeModelComponents,
	handleNullModel,
	concatStrings,
	loadDigimonTexture,
	resetFlattenGlobal,
	renderWireframed,
	setEntityRotation,
	setEntityPosition,
	setupEntityMatrix,
	removeEntity,
	renderDigimon,
	initializeDigimonObject,
	thunkUnloadModel,
	thunkLoadMMD,
	renderFlatDigimon,
	initializePosData,
};

static inline int32_t applyTPageOffset(int32_t tpageOffset,
				       int32_t pixelOffset)
{
	return tpageOffset + pixelOffset;
}

static inline int8_t *model_s8ptr(uint8_t *arg0)
{
  return (int8_t *) arg0;
}

void initializePosData(PositionData *posData)
{
	MATRIX *m;

	RotMatrix(&posData->rotation, m = &posData->posMatrix.coord);
	ScaleMatrix(m, &posData->scale);
	TransMatrix(m, &posData->location);
	posData->posMatrix.flg = 0;
}

void renderFlatDigimon(Entity* entity)
{
	MATRIX m;
	SVECTOR in[4];
	SVECTOR out[4];
	ModelComponent *model;
	POLY_FT4 *prim;
	VECTOR *loc;
	int32_t entityType;
	int32_t i;
	int32_t height;
	int32_t half;

	entityType = getEntityType(entity);
	model = getEntityModelComponent(entity->type, entityType);
	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	setRGB0(prim, 0x80, 0x80, 0x80);
	prim->clut = model->clutPage + 0x3C0;
	prim->tpage = model->pixelPage;
	if (entity->flatSprite < 2) {
		if ((entity->flatTimer % 10) == 0) {
			entity->flatSprite = (entity->flatSprite + 1) & 1;
		}
		++entity->flatTimer;
	}
	setUVWH(prim,
		((entity->flatSprite % 2) == 0) ? 0xE0 : 0xF0,
		(entity->flatSprite < 2) ? model->pixelOffsetY + 0x60 : model->pixelOffsetY + 0x70,
		0xF, 0xF);
	half = (height = DIGIMON_DATA[entity->type].height) / 2;
	in[0].vx = 0;
	in[0].vy = -height;
	in[0].vz = -half;
	in[1].vx = 0;
	in[1].vy = -height;
	in[1].vz = half;
	in[2].vx = 0;
	in[2].vy = 0;
	in[2].vz = -half;
	in[3].vx = 0;
	in[3].vy = 0;
	in[3].vz = half;
	loc = &entity->posData->location;
	RotMatrix(&entity->posData->rotation, &m);
	ScaleMatrix(&m, &entity->posData->scale);
	for (i = 0; i < 4; i++) {
		ApplyMatrixSV(&m, &in[i], &out[i]);
		out[i].vx += (int16_t)loc->vx;
		out[i].vy += (int16_t)loc->vy;
		out[i].vz += (int16_t)loc->vz;
	}
	add3DSpritePrim(prim, &out[0], &out[1], &out[2], &out[3]);
}

ModelComponent *thunkLoadMMD(int32_t digiType, int32_t modelType)
{
	return loadMMD(digiType, modelType);
}

void thunkUnloadModel(int32_t digiType, int32_t modelType)
{
	unloadModel(digiType, modelType);
}

void initializeDigimonObject(int32_t type, int32_t instanceId,
			     TickFunction tick)
{
	Entity *entity;
	ModelComponent *model;
	PositionData *pos;
	SkeletonBone *bone;
	int16_t *anim;
	int32_t entityType;
	int32_t boneCount;
	int32_t i;
	int32_t parent;

	if (instanceId < 0 || instanceId >= ENTITY_MAX) {
		return;
	}
	boneCount = DIGIMON_DATA[type].boneCount;
	entity = ENTITY_TABLE[instanceId];
	entityType = getEntityType(entity);
	entity->type = type;
	switch (entityType) {
	case 2:
		entity->posData = TAMER_POSITION_DATA;
		entity->anim.momentum = TAMER_MOMENTUM_DATA;
		break;
	case 3:
		entity->posData = PARTNER_POSITION_DATA;
		entity->anim.momentum = PARTNER_MOMENTUM_DATA;
		break;
	case 0:
		entity->posData = malloc3(boneCount * sizeof(PositionData));
		entity->anim.momentum = malloc3(boneCount * sizeof(MomentumData));
		break;
	}
	model = getEntityModelComponent(type, entityType);
	entity->animPtr = model->animTablePtr;
	pos = entity->posData;
	bone = DIGIMON_SKELETONS[type];
	for (i = 0; i < boneCount; ++pos, ++bone, ++i) {
		if (bone->objIndex != -1) {
			GsLinkObject4((u_long)model->modelPtr->obj, &pos->obj, bone->objIndex);
		} else {
			pos->obj.tmd = NULL;
		}
		if ((parent = bone->parentIndex) != -1) {
			GsInitCoordinate2(&entity->posData[parent].posMatrix, &pos->posMatrix);
		} else {
			GsInitCoordinate2(NULL, &pos->posMatrix);
		}
		pos->obj.attribute = 0;
		pos->obj.coord2 = &pos->posMatrix;
	}
	pos = entity->posData;
	anim = (int16_t *)((char *)entity->animPtr + *entity->animPtr);
	++anim;
	if (type == 0x71) {
		pos->scale.vx = 0x1800, pos->scale.vy = 0x1800, pos->scale.vz = 0x1800;
	} else {
		pos->scale.vx = 0x1000, pos->scale.vy = 0x1000, pos->scale.vz = 0x1000;
	}
	pos->rotation.vx = 0;
	pos->rotation.vy = 0;
	pos->rotation.vz = 0;
	pos->location.vx = 0;
	pos->location.vy = 0;
	pos->location.vz = 0;
	initializePosData(pos);
	for (i = 1; i < boneCount; i++) {
		++pos;
		pos->scale.vx = 0x1000, pos->scale.vy = 0x1000, pos->scale.vz = 0x1000;
		pos->rotation.vx = *anim++;
		pos->rotation.vy = *anim++;
		pos->rotation.vz = *anim++;
		pos->location.vx = *anim++;
		pos->location.vy = *anim++;
		pos->location.vz = *anim++;
		initializePosData(pos);
	}
	startAnimation(entity, 0);
	addObject((int16_t)type, (int16_t)instanceId, tick, renderDigimon);
}

void renderDigimon(instanceId)
	int16_t instanceId;
{
	Entity *entity;
	PositionData *pos;
	MATRIX lw;
	MATRIX ls;
	int32_t boneCount;
	int32_t i;

	entity = ENTITY_TABLE[instanceId];
	if (entity->isOnMap != 2) {
		if (entity->isOnMap == 0) {
			return;
		}
		if (entity->isOnScreen == 0) {
			return;
		}
	}
	boneCount = DIGIMON_DATA[entity->type].boneCount;
	pos = entity->posData;
	for (i = 0; i < boneCount; pos++, i++) {
		if (pos->obj.tmd == NULL) {
			continue;
		}
		GsGetLws(pos->obj.coord2, &lw, &ls);
		GsSetLightMatrix(&lw);
		GsSetLsMatrix(&ls);
		if (ENTITY_TABLE[instanceId]->flatSprite != -1) {
			continue;
		}
		if (instanceId == 1) {
			if (PARTNER_WIREFRAME_TOTAL != 0x10) {
				renderWireframed(&pos->obj, PARTNER_WIREFRAME_TOTAL);
				continue;
			}
			if (PARTNER_WIREFRAME_SUB[i] != 0x10) {
				renderWireframed(&pos->obj, PARTNER_WIREFRAME_SUB[i]);
				continue;
			}
			GsSortObject4(&pos->obj, ACTIVE_ORDERING_TABLE, 2, (u_long *)0x1F800000);
			continue;
		}
		if (instanceId == 2) {
			if (ENTITY1_WIREFRAME_TOTAL != 0x10) {
				renderWireframed(&pos->obj, ENTITY1_WIREFRAME_TOTAL);
				continue;
			}
			GsSortObject4(&pos->obj, ACTIVE_ORDERING_TABLE, 2, (u_long *)0x1F800000);
			continue;
		}
		GsSortObject4(&pos->obj, ACTIVE_ORDERING_TABLE, 2, (u_long *)0x1F800000);
	}
	if (ENTITY_TABLE[instanceId]->flatSprite != -1) {
		renderFlatDigimon(ENTITY_TABLE[instanceId]);
	}
	if (instanceId == 0) {
		if (PLAYER_SHADOW_ENABLED == 0) {
			return;
		}
		renderDropShadow(ENTITY_TABLE[instanceId]);
		return;
	}
	renderDropShadow(ENTITY_TABLE[instanceId]);
}

void removeEntity(int32_t objectId, int32_t entityId)
{
	Entity *entity;

	removeObject((int16_t)objectId, (int16_t)entityId);
	if ((entityId != 1) && (entityId != 0)) {
		entity = ENTITY_TABLE[entityId];
		free3(entity->anim.momentum);
		free3(entity->posData);
		ENTITY_TABLE[entityId] = NULL;
	}
}

void setupEntityMatrix(int32_t entityId)
{
	MATRIX *m;
	PositionData *posData;

	if ((entityId >= 0) && (entityId < ENTITY_MAX)) {
		posData = ENTITY_TABLE[entityId]->posData;
		RotMatrix(&posData->rotation, m = &posData->posMatrix.coord);
		ScaleMatrix(m, &posData->scale);
		TransMatrix(m, &posData->location);
		posData->posMatrix.flg = 0;
	}
}

void setEntityPosition(int32_t entityId, int32_t x, int32_t y, int32_t z)
{
	VECTOR *location;

	if ((entityId >= 0) && (entityId < ENTITY_MAX)) {
		location = &ENTITY_TABLE[entityId]->posData->location;
		location->vx = x;
		location->vy = y;
		location->vz = z;
		ENTITY_TABLE[entityId]->anim.locX = location->vx << 15;
		ENTITY_TABLE[entityId]->anim.locY = location->vy << 15;
		ENTITY_TABLE[entityId]->anim.locZ = location->vz << 15;
	}
}

void setEntityRotation(entityId, x, y, z)
	int32_t entityId;
	int16_t x;
	int16_t y;
	int16_t z;
{
	SVECTOR *rotation;

	if ((entityId >= 0) && (entityId < ENTITY_MAX)) {
		rotation = &ENTITY_TABLE[entityId]->posData->rotation;
		rotation->vx = x;
		rotation->vy = y;
		rotation->vz = z;
	}
}

void renderWireframed(GsDOBJ2* obj, int32_t wireFrameShare)
{
	int32_t primn;
	CVECTOR col;
	long p;
	long flag;
	long otz;
	MATRIX m;
	struct TMD_STRUCT *tmd;
	SVECTOR *vert;
	SVECTOR *normal;
	u_char *prim;
	GsCOORDINATE2 *coord;
	u_char *pk;
	int32_t start;
	uint32_t cur;
	int32_t i;
	int8_t color;
	int16_t min;
	POLY_GT3 *gt3;
	POLY_GT4 *gt4;
	LINE_F4 *lf3;
	LINE_F2 *lf2;

	min = WIREFRAME_COLOR_MIN;
	color = min + rand() % (WIREFRAME_COLOR_MAX - min);
	tmd = (struct TMD_STRUCT *)obj->tmd;
	vert = (SVECTOR *)tmd->vertop;
	normal = (SVECTOR *)tmd->nortop;
	prim = (u_char *)tmd->primtop;
	primn = tmd->primn;
	pk = (u_char *)GsGetWorkBase();
	col.r = col.g = col.b = 0x80;
	coord = obj->coord2;
	if (coord->flg == 0) {
		coord->flg = 1;
		MulMatrix0(&coord->coord, &coord->super->workm, &coord->workm);
	}
	MulMatrix0(&GsLIGHTWSMATRIX, &coord->workm, &m);
	SetLightMatrix(&m);
	CompMatrix(&GsWSMATRIX, &coord->workm, &m);
	setRotTransMatrix(&m);
	for (i = 0; i < primn; i++) {
		if ((prim[3] & 0xFC) == 0x34) {
			cur = (uint32_t)prim;
			if (WIREFRAME_RNG_TABLE[i & 0xF] < wireFrameShare) {
				gt3 = (POLY_GT3 *)pk;
				start = (int32_t)pk;
				if (0 < RotNclip3(&vert[*(u_short *)(prim + 0x12)], &vert[*(u_short *)(prim + 0x16)],
					      &vert[*(u_short *)(prim + 0x1A)], (long *)&gt3->x0, (long *)&gt3->x1,
					      (long *)&gt3->x2, &p, &otz, &flag)) {
					NormalColorCol3(&normal[*(u_short *)(prim + 0x10)],
							&normal[*(u_short *)(prim + 0x14)],
							&normal[*(u_short *)(prim + 0x18)], &col, (CVECTOR *)&gt3->r0,
							(CVECTOR *)&gt3->r1, (CVECTOR *)&gt3->r2);
					setUV3(gt3, prim[4], prim[5], prim[8], prim[9], prim[0xC], prim[0xD]);
					gt3->clut = *(u_short *)(prim + 6);
					gt3->tpage = *(u_short *)(prim + 0xA);
					setPolyGT3(gt3);
					otz = otz >> 2;
					AddPrim(ACTIVE_ORDERING_TABLE->org + otz, gt3);
					pk = (u_char *)(start + sizeof(POLY_GT3));
				}
			} else {
				lf3 = (LINE_F4 *)pk;
				start = (int32_t)pk;
				if (0 < RotNclip3(&vert[*(u_short *)(prim + 0x12)], &vert[*(u_short *)(prim + 0x16)],
					      &vert[*(u_short *)(prim + 0x1A)], (long *)&lf3->x0, (long *)&lf3->x1,
					      (long *)&lf3->x2, &p, &otz, &flag)) {
					lf3->x3 = lf3->x0;
					lf3->y3 = lf3->y0;
					setlen(lf3, 6);
					setcode(lf3, 0x4C);
					lf3->pad = 0x55555555;
					lf3->b0 = color;
					lf3->g0 = color;
					lf3->r0 = color;
					otz = otz >> 2;
					AddPrim(ACTIVE_ORDERING_TABLE->org + otz, lf3);
					pk = (u_char *)(start + sizeof(LINE_F4));
				}
			}
			prim = (u_char *)(cur + 0x1C);
		} else if ((prim[3] & 0xFC) == 0x3C) {
			if (WIREFRAME_RNG_TABLE[i & 0xF] < wireFrameShare) {
				gt4 = (POLY_GT4 *)pk;
				start = (int32_t)pk;
				if (0 < RotNclip4(&vert[*(u_short *)(prim + 0x16)], &vert[*(u_short *)(prim + 0x1A)],
					      &vert[*(u_short *)(prim + 0x1E)], &vert[*(u_short *)(prim + 0x22)],
					      (long *)&gt4->x0, (long *)&gt4->x1, (long *)&gt4->x2, (long *)&gt4->x3, &p,
					      &otz, &flag)) {
					NormalColorCol3(&normal[*(u_short *)(prim + 0x14)],
							&normal[*(u_short *)(prim + 0x18)],
							&normal[*(u_short *)(prim + 0x1C)], &col, (CVECTOR *)&gt4->r0,
							(CVECTOR *)&gt4->r1, (CVECTOR *)&gt4->r2);
					NormalColorCol(&normal[*(u_short *)(prim + 0x20)], &col, (CVECTOR *)&gt4->r3);
					setUV4(gt4, prim[4], prim[5], prim[8], prim[9], prim[0xC], prim[0xD], prim[0x10], prim[0x11]);
					gt4->clut = *(u_short *)(prim + 6);
					gt4->tpage = *(u_short *)(prim + 0xA);
					setPolyGT4(gt4);
					otz = otz >> 2;
					AddPrim(ACTIVE_ORDERING_TABLE->org + otz, gt4);
					pk = (u_char *)(start + sizeof(POLY_GT4));
				}
			} else {
				lf3 = (LINE_F4 *)pk;
				if (0 < RotNclip4(&vert[*(u_short *)(prim + 0x16)], &vert[*(u_short *)(prim + 0x1A)],
					      &vert[*(u_short *)(prim + 0x1E)], &vert[*(u_short *)(prim + 0x22)],
					      (long *)&lf3->x0, (long *)&lf3->x1, (long *)&lf3->x3, (long *)&lf3->x2, &p,
					      &otz, &flag)) {
					setlen(lf3, 6);
					setcode(lf3, 0x4C);
					lf3->pad = 0x55555555;
					lf3->b0 = color;
					lf3->g0 = color;
					lf3->r0 = color;
					otz = otz >> 2;
					AddPrim(ACTIVE_ORDERING_TABLE->org + otz, lf3);
					lf2 = (LINE_F2 *)(pk + sizeof(LINE_F4));
					setLineF2(lf2);
					lf2->b0 = color;
					lf2->g0 = color;
					lf2->r0 = color;
					setXY2(lf2, lf3->x3, lf3->y3, lf3->x0, lf3->y0);
					AddPrim(ACTIVE_ORDERING_TABLE->org + otz, lf2);
					pk = (u_char *)lf2 + sizeof(LINE_F2);
				}
			}
			prim = (u_char *)((uint32_t)prim + 0x24);
		} else {
			break;
		}
	}
	GsSetWorkBase((PACKET *)pk);
}

void resetFlattenGlobal(void)
{
	int32_t i;

	TAMER_ENTITY.entity.flatSprite = -1;

	PARTNER_ENTITY.digimonEntity.entity.flatTimer = 0;
	PARTNER_ENTITY.digimonEntity.entity.flatSprite = -1;

	for (i = 0; i < 8; i++) {
		NPC_ENTITIES[i].digimonEntity.entity.flatTimer = 0;
		NPC_ENTITIES[i].digimonEntity.entity.flatSprite = -1;
	}

}

void loadDigimonTexture(int32_t digiType, char *path,
			ModelComponent *component)
{
	void *buffer;
	char fileName[32];

	strrchr(path, '\\');
	strcpy(fileName, MAIN_D_8011D190);

	buffer = malloc3(0x4800);

	readFileSectors(fileName, buffer, digiType * 9, 9);
	uploadModelTexture(buffer, component);

	free3(buffer);
}

void concatStrings(char *dst, char *src1, char *src2)
{
	while (*src1 != '\0') {
		*dst++ = *src1++;
	}

	while (*src2 != '\0') {
		*dst++ = *src2++;
	}

	*dst = '\0';
}

void handleNullModel(void)
{
}

void initializeModelComponents(void)
{
	ModelComponent t;
	int32_t i;

	t.useCount = 0;
	t.modelPtr = NULL;
	t.animTablePtr = NULL;
	t.mmdPtr = NULL;
	t.pixelPage = 0;
	t.clutPage = 0;
	t.pixelOffsetY = 0;
	t.pixelOffsetX = 0;
	t.modelId = -1;
	t.digiType = -1;

	for (i = 0; i < 5; ++i) {
		NPC_MODEL[i] = t;
	}

	for (i = 0; i < 5; ++i) {
		NPC_MODEL_TAKEN[i] = 0;
	}

	for (i = 0; i < 16; ++i) {
		UNKNOWN_MODEL[i] = t;
	}

	for (i = 0; i < 16; ++i) {
		UNKNOWN_MODEL_TAKEN[i] = 0;
	}
}

ModelComponent *loadMMD(int32_t digiType, int32_t modelType)
{
	ModelComponent *m;
	char *name;
	char path[32];
	int32_t i;
	int32_t slot;
	int32_t k;
	int32_t size;

	if (modelType == 1) {
		return 0;
	}
	if (digiType < 0 || digiType >= 0xB4) {
		return 0;
	}
	if (modelType == 0) {
		slot = -1;
		for (m = NPC_MODEL, i = 0; i < 5; m++, i++) {
			if (m->useCount == 0) {
				slot = i;
			} else if (m->digiType == digiType) {
				break;
			}
		}
		if (i == 5) {
			if (slot == -1) {
				return 0;
			}
			m = &NPC_MODEL[slot];
			m->digiType = digiType;
		}
		++m->useCount;
		if (m->useCount != 1) {
			return m;
		}
		k = 0;
		while (NPC_MODEL_TAKEN[k] != 0 && k < 5) {
			++k;
		}
		if (k == 5) {
			return 0;
		}
		NPC_MODEL_TAKEN[k] = 1;
		m->pixelPage = k / 2 + 0x16;
		m->clutPage = ((((k << 4) + 0x20) >> 4) & 0x3F) | 0x7A00;
		m->pixelOffsetX = 0;
		m->pixelOffsetY = (k % 2) << 7;
		m->modelId = k;
		name = PTR_DIGIMON_FILE_NAMES[digiType];
		loadDigimonTexture(digiType, name, m);
		concatStrings(path, MAIN_D_8011D484, name);
		concatStrings(path, path, MAIN_D_801340E4);
		path[9] = digiType / 30 + '0';
		size = lookupFileSize(path);
		m->mmdPtr = malloc3((size + 0x7FF) & ~0x7FF);
		if (m->mmdPtr == NULL) {
			handleNullModel();
		}
		readFile(path, m->mmdPtr);
		m->modelPtr = (TMDModel *)((char *)m->mmdPtr + ((int32_t *)m->mmdPtr)[0]);
		m->animTablePtr = (int32_t *)((char *)m->mmdPtr + ((int32_t *)m->mmdPtr)[1]);
		GsMapModelingData((u_long *)&m->modelPtr->flags);
		updateTMDTextureData((char *)m->modelPtr, m->pixelPage, m->pixelOffsetX, m->pixelOffsetY,
				     m->clutPage - 0x7A00);
		return m;
	}
	if (modelType == 2) {
		TAMER_MODEL.pixelPage = 0x15;
		TAMER_MODEL.clutPage = 0x7A00;
		TAMER_MODEL.pixelOffsetX = 0;
		TAMER_MODEL.pixelOffsetY = 0;
		TAMER_MODEL.modelId = 0;
		m = &TAMER_MODEL;
		TAMER_MODEL.mmdPtr = TAMER_MODEL_BUFFER;
	} else if (modelType == 3) {
		PARTNER_MODEL.pixelPage = 0x15;
		PARTNER_MODEL.clutPage = 0x7A01;
		PARTNER_MODEL.pixelOffsetX = 0;
		PARTNER_MODEL.pixelOffsetY = 0x80;
		PARTNER_MODEL.modelId = 0;
		m = &PARTNER_MODEL;
		PARTNER_MODEL.mmdPtr = PARTNER_MODEL_BUFFER;
	} else {
		return 0;
	}
	name = PTR_DIGIMON_FILE_NAMES[digiType];
	loadDigimonTexture(digiType, name, m);
	concatStrings(path, MAIN_D_8011D484, name);
	concatStrings(path, path, MAIN_D_801340E4);
	path[9] = digiType / 30 + '0';
	readFile(path, m->mmdPtr);
	m->modelPtr = (TMDModel *)((char *)m->mmdPtr + ((int32_t *)m->mmdPtr)[0]);
	m->animTablePtr = (int32_t *)((char *)m->mmdPtr + ((int32_t *)m->mmdPtr)[1]);
	GsMapModelingData((u_long *)m->modelPtr + 1);
	updateTMDTextureData((char *)m->modelPtr, m->pixelPage, m->pixelOffsetX, m->pixelOffsetY,
			     m->clutPage - 0x7A00);
	return m;
}

void unloadModel(int32_t digiType, int32_t modelType)
{
	int32_t i;
	ModelComponent *m;

	if (modelType == 0) {
		for (m = NPC_MODEL, i = 0; i < 5; ++m, ++i) {
			if (m->digiType == digiType) {
				break;
			}
		}
		if (i != 5) {
			--m->useCount;
			if (m->useCount == 0) {
				m->digiType = -1;
				if (m->mmdPtr != NULL) {
					free3(m->mmdPtr);
				}
				m->modelPtr = NULL;
				m->animTablePtr = NULL;
				m->mmdPtr = NULL;
				if (m->modelId != -1) {
					NPC_MODEL_TAKEN[m->modelId] = 0;
				}
				m->modelId = -1;
			}
		}
	} else if (modelType == 1) {
		for (m = UNKNOWN_MODEL, i = 0; i < 16; ++m, ++i) {
			if (m->useCount == digiType) {
				break;
			}
		}
		if (i != 16) {
			m->useCount = 0;
			if (m->modelId != -1) {
				UNKNOWN_MODEL_TAKEN[m->modelId] = 0;
			}
			m->modelId = -1;
		}
	}
}

ModelComponent *getEntityModelComponent(int32_t instance, int32_t type)
{
	ModelComponent *p;
	int32_t i;

	i = type;
	if (i == 2) {
		return &TAMER_MODEL;
	}
	if (i == 3) {
		return &PARTNER_MODEL;
	}
	if (i == 0) {
		i = instance;
		if ((i < 0) || (instance >= 0xB4)) {
			return 0;
		}
		p = NPC_MODEL;
		for (i = 0; i < 5; p++, i++) {
			if (p->digiType == instance) {
				break;
			}
		}
		if (i == 5) {
			return 0;
		}
		if (p->useCount != 0) {
			goto done;
		}
		return 0;
	}
	if (i == 1) {
		i = instance;
		if ((instance < 0) || (i >= 0x96)) {
			return 0;
		}
		p = &UNKNOWN_MODEL[i];
		if (p->useCount == 0) {
			return 0;
		}
	}
done:
	return p;
}

int32_t getEntityType(Entity* entity)
{
	int32_t i;
	int32_t v;

	for (i = 0; i < 10; i++) {
		if (ENTITY_TABLE[i] == entity) {
			break;
		}
	}
	switch (i) {
	case 0:
		v = 2;
		break;
	case 1:
		v = 3;
		break;
	case 10:
		v = -1;
		break;
	default:
		v = 0;
		break;
	}
	return v;
}


void uploadModelTexture(void *textureData, ModelComponent *component)
{
	GsIMAGE img;
	RECT rect;

	GsGetTimInfo((unsigned long *)textureData + 1, &img);

	img.px = applyTPageOffset((component->pixelPage % 16) * 64,
				  component->pixelOffsetX);
	img.py = applyTPageOffset((component->pixelPage / 16) * 256,
				  component->pixelOffsetY);
	img.cx = (component->clutPage & 0x3f) << 4;
	img.cy = component->clutPage >> 6;

	rect.x = img.px;
	rect.y = img.py;
	rect.w = img.pw;
	rect.h = img.ph;
	LoadImage(&rect, img.pixel);

	if ((img.pmode >> 3) & 1) {
		rect.x = img.cx;
		rect.y = img.cy;
		rect.w = img.cw;
		rect.h = img.ch;
		LoadImage(&rect, img.clut);
	}

	DrawSync(0);
}

uint8_t *loadMMDAsync(int32_t digimonType, int32_t entityType, uint8_t *buffer,
		      EvoModelData *modelData, uint8_t *readComplete)
{
	ModelComponent *m;
	char path[32];
	char tim[32];
	char *name;
	int32_t i;
	int32_t slot;
	int32_t align;

	if (digimonType < 0 || digimonType >= 0xB4) {
		return 0;
	}
	if (entityType == 0) {
		slot = -1;
		for (m = NPC_MODEL, i = 0; i < 5; m++, i++) {
			if (m->useCount == 0) {
				slot = i;
			} else if (m->digiType == digimonType) {
				break;
			}
		}
		if (i == 5) {
			if (slot == -1) {
				return 0;
			}
			m = &NPC_MODEL[slot];
			m->digiType = digimonType;
		}
		++m->useCount;
		if (m->useCount != 1) {
			return 0;
		}
	} else if (entityType != 2 && entityType != 3) {
		return 0;
	}
	name = PTR_DIGIMON_FILE_NAMES[digimonType];
	strcpy(tim, MAIN_D_8011D190);
	align = (int32_t)buffer & 3;
	if (align != 0) {
		buffer += 4 - align;
	}
	readFileSectors(tim, buffer, digimonType * 9, 9);
	modelData->imagePtr = buffer;
	modelData->imageSize = 0x4800;
	buffer += modelData->imageSize;
	concatStrings(path, MAIN_D_8011D484, name);
	concatStrings(path, path, MAIN_D_801340E4);
	path[9] = digimonType / 30 + '0';
	align = (int32_t)buffer & 3;
	if (align != 0) {
		buffer += 4 - align;
	}
	addFileReadRequestPath(path, buffer, readComplete, 0, 0);
	modelData->modelPtr = buffer;
	modelData->modelSize = lookupFileSize(path);
	buffer += modelData->modelSize;
	return buffer;
}

ModelComponent *applyMMD(int32_t digimonType, int32_t entityType,
			 EvoModelData *modelData)
{
	ModelComponent *m;
	char *name;
	char path[32];
	GsIMAGE img;
	RECT rect;
	int32_t i;
	int32_t k;
	int32_t size;
	int32_t digit;

	if (digimonType < 0 || digimonType >= 0xB4) {
		return 0;
	}
	if (entityType == 0) {
		for (m = NPC_MODEL, i = 0; i < 5; m++, i++) {
			if (m->digiType == digimonType) {
				break;
			}
		}
		if (i == 5) {
			return 0;
		}
		k = 0;
		while (NPC_MODEL_TAKEN[k] != 0 && k < 5) {
			++k;
		}
		if (k == 5) {
			return 0;
		}
		NPC_MODEL_TAKEN[k] = 1;
		m->pixelPage = k / 2 + 0x16;
		m->clutPage = ((((k << 4) + 0x20) >> 4) & 0x3F) | 0x7A00;
		m->pixelOffsetX = 0;
		m->pixelOffsetY = (k % 2) << 7;
		m->modelId = k;
		name = PTR_DIGIMON_FILE_NAMES[digimonType];
		loadDigimonTexture(digimonType, name, m);
		concatStrings(path, MAIN_D_8011D46C, name);
		concatStrings(path, path, MAIN_D_801340EC);
		path[9] = digit = digimonType / 30 + '0';
		size = lookupFileSize(path);
		m->modelPtr = malloc3((size + 0x7FF) & ~0x7FF);
		readFile(path, m->modelPtr);
		GsMapModelingData((u_long *)m->modelPtr + 1);
		updateTMDTextureData((char *)m->modelPtr, m->pixelPage, m->pixelOffsetX, m->pixelOffsetY,
				     m->clutPage - 0x7A00);
		concatStrings(path, MAIN_D_8011D478, name);
		concatStrings(path, path, MAIN_D_801340F4);
		path[9] = digit;
		size = lookupFileSize(path);
		m->animTablePtr = malloc3((size + 0x7FF) & ~0x7FF);
		readFile(path, m->animTablePtr);
		return m;
	}
	if (entityType == 2) {
	} else if (entityType == 3) {
		PARTNER_MODEL.pixelPage = 0x15;
		PARTNER_MODEL.clutPage = 0x7A01;
		PARTNER_MODEL.pixelOffsetX = 0;
		PARTNER_MODEL.pixelOffsetY = 0x80;
		PARTNER_MODEL.modelId = 0;
		PARTNER_MODEL.mmdPtr = PARTNER_MODEL_BUFFER;
		m = &PARTNER_MODEL;
		PARTNER_MODEL.useCount = 0;
	} else {
		return 0;
	}
	++m->useCount;
	if (m->useCount != 1) {
		return m;
	}
	GsGetTimInfo((unsigned long *)modelData->imagePtr + 1, &img);
	img.px = applyTPageOffset((m->pixelPage % 16) * 64, m->pixelOffsetX);
	img.py = applyTPageOffset((m->pixelPage / 16) * 256, m->pixelOffsetY);
	img.cx = (m->clutPage & 0x3f) << 4;
	img.cy = m->clutPage >> 6;
	rect.x = img.px;
	rect.y = img.py;
	rect.w = img.pw;
	rect.h = img.ph;
	LoadImage(&rect, img.pixel);
	if ((img.pmode >> 3) & 1) {
		rect.x = img.cx;
		rect.y = img.cy;
		rect.w = img.cw;
		rect.h = img.ch;
		LoadImage(&rect, img.clut);
	}
	memcpy(m->mmdPtr, modelData->modelPtr, modelData->modelSize);
	m->modelPtr = (TMDModel *)((char *)m->mmdPtr + ((int32_t *)m->mmdPtr)[0]);
	m->animTablePtr = (int32_t *)((char *)m->mmdPtr + ((int32_t *)m->mmdPtr)[1]);
	GsMapModelingData((u_long *)m->modelPtr + 1);
	updateTMDTextureData((char *)m->modelPtr, m->pixelPage, m->pixelOffsetX, m->pixelOffsetY,
			     m->clutPage - 0x7A00);
	return m;
}
