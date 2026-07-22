#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/types.h>

#include "common.h"

#define CUSTOM_RNG_FACTOR	0x41c650ad
#define CUSTOM_RNG_VALUE	0x3039

/* Not dw/math.h's ABS: that one tests (x) < 0, which branches the other way. */
#define ABS(x)			(((x) > 0) ? (x) : -(x))
#define MAX(a, b)		(((a) > (b)) ? (a) : (b))

extern GsOT *ACTIVE_ORDERING_TABLE;

extern uint32_t CUSTOM_RNG_1;
extern uint32_t CUSTOM_RNG_2;

void calculatePosition(GsCOORDINATE2 *coord, MATRIX *matrix);

const uint8_t MAIN_D_80114D68[256] = {
	0x00, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02,
	0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
	0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
	0x05, 0x05, 0x05, 0x05, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
	0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
	0x09, 0x09, 0x09, 0x09, 0x0a, 0x0a, 0x0a, 0x0a,
	0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a,
	0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a,
	0x0a, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
	0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
	0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
	0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
	0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
	0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
	0x0c, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d,
	0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d,
	0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d,
	0x0d, 0x0d, 0x0d, 0x0d, 0x0e, 0x0e, 0x0e, 0x0e,
	0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e,
	0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e,
	0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e,
	0x0e, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
};

void setLineBlendingMode(int32_t mode, int32_t order);
void renderTrianglePrimitive(uint32_t color, int16_t x0, int16_t y0,
			     int16_t x1, int32_t y1, int32_t x2, int32_t y2,
			     int32_t order, uint32_t mode);
void renderLinePrimitive(uint32_t color, int16_t x0, int16_t y0, int16_t x1,
			 int32_t y1, int32_t order, uint32_t mode);
void rotateVectorYXZ(SVECTOR *rotation, VECTOR *input, VECTOR *output);
void matrixToEuler2(MATRIX *matrix, SVECTOR *output);
void MAIN_func_800E4470(MATRIX *matrix, SVECTOR *output);
void multiplyRotations(SVECTOR *rotation1, SVECTOR *rotation2);
int32_t customRandom(int32_t min, int32_t max);

void setLineBlendingMode(int32_t mode, int32_t order)
{
	DR_TPAGE *prim;

	if (mode == 0) {
		return;
	}

	prim = (DR_TPAGE *)GsGetWorkBase();
	setDrawTPage(prim, 1, 1, (mode & 3) << 5);

	addPrim(&ACTIVE_ORDERING_TABLE->org[order], prim);
	GsSetWorkBase((PACKET *)&prim[1]);
}

void renderTrianglePrimitive(uint32_t color, int16_t x0, int16_t y0,
			     int16_t x1, int32_t y1, int32_t x2, int32_t y2,
			     int32_t order, uint32_t mode)
{
	LINE_F3 *prim;

	prim = (LINE_F3 *)GsGetWorkBase();
	*(uint32_t *)&prim->r0 = color;
	setXY3(prim, x0, y0, x1, y1, x2, y2);
	setLineF3(prim);
	setSemiTrans(prim, mode >> 2);

	addPrim(&ACTIVE_ORDERING_TABLE->org[order], prim);
	GsSetWorkBase((PACKET *)&prim[1]);

	setLineBlendingMode(mode, order);
}

void renderLinePrimitive(uint32_t color, int16_t x0, int16_t y0, int16_t x1,
			 int32_t y1, int32_t order, uint32_t mode)
{
	LINE_F2 *prim;

	prim = (LINE_F2 *)GsGetWorkBase();

	*(uint32_t *)&prim->r0 = color;
	setXY2(prim, x0, y0, x1, y1);
	setLineF2(prim);
	setSemiTrans(prim, mode >> 2);

	addPrim(&ACTIVE_ORDERING_TABLE->org[order], prim);
	GsSetWorkBase((PACKET *)&prim[1]);

	setLineBlendingMode(mode, order);
}

extern int32_t MAIN_D_80136F98[];
void MAIN_func_800E3FB8(int16_t *pos, VECTOR *out);

void MAIN_func_800E3FB8(int16_t *pos, VECTOR *out)
{
  MATRIX m;
  int32_t *new_var;
  int32_t new_var2;
  SVECTOR v;
  int32_t *cam;
  cam = MAIN_D_80136F98;
  new_var2 = (new_var = cam)[0];
 do { v.vx = pos[0] - new_var2; v.vy = pos[1] - new_var[1]; v.vz = pos[2] - new_var[2]; } while (0);
  TransposeMatrix(&GsWSMATRIX, &m);
  ApplyMatrix(&m, &v, out);
}

INCLUDE_ASM("asm/main/nonmatchings/graphics2", MAIN_func_800E4038);

void rotateVectorYXZ(SVECTOR *rotation, VECTOR *input, VECTOR *output)
{
	MATRIX matrix;
	RotMatrixYXZ(rotation, &matrix);
	ApplyMatrixLV(&matrix, input, output);
}

INCLUDE_ASM("asm/main/nonmatchings/graphics2", toEulerAngles);

INCLUDE_ASM("asm/main/nonmatchings/graphics2", getDistance);

void MAIN_func_800E4470(MATRIX *matrix, SVECTOR *output)
{
	int32_t sinX;
	int32_t cosX;
	int32_t sinZ;
	int32_t cosZ;
	int32_t largest;
	int32_t scale;

	if (matrix->m[2][2] == 0 && matrix->m[0][0] == 0) {
		output->vy = (matrix->m[0][2] > 0) ? 0x400 : -0x400;
		output->vx = ratan2(matrix->m[2][1], matrix->m[1][1]);
		output->vz = 0;
		return;
	}

	output->vx = ratan2(-matrix->m[1][2], matrix->m[2][2]);
	output->vz = ratan2(-matrix->m[0][1], matrix->m[0][0]);

	sinX = rsin(output->vx);
	cosX = rcos(output->vx);
	sinZ = rsin(output->vz);
	cosZ = rcos(output->vz);

	largest = MAX(MAX(ABS(sinX), ABS(cosX)), MAX(ABS(sinZ), ABS(cosZ)));

	if (largest == ABS(sinX)) {
		scale = (-matrix->m[1][2] << 12) / sinX;
	} else if (largest == ABS(cosX)) {
		scale = (matrix->m[2][2] << 12) / cosX;
	} else if (largest == ABS(sinZ)) {
		scale = (-matrix->m[0][1] << 12) / sinZ;
	} else if (largest == ABS(cosZ)) {
		scale = (matrix->m[0][0] << 12) / cosZ;
	}

	output->vy = ratan2(matrix->m[0][2], scale);
}

void matrixToEuler2(MATRIX *matrix, SVECTOR *output)
{
	int32_t sinY;
	int32_t cosY;
	int32_t sinZ;
	int32_t cosZ;
	int32_t largest;
	int32_t scale;

	if (matrix->m[2][2] == 0 && matrix->m[0][2] == 0) {
		output->vx = (matrix->m[1][2] > 0) ? -0x400 : 0x400;
		output->vy = ratan2(-matrix->m[2][0], matrix->m[0][0]);
		output->vz = 0;
		return;
	}

	output->vy = ratan2(matrix->m[0][2], matrix->m[2][2]);
	output->vz = ratan2(matrix->m[1][0], matrix->m[1][1]);

	sinY = rsin(output->vy);
	cosY = rcos(output->vy);
	sinZ = rsin(output->vz);
	cosZ = rcos(output->vz);

	largest = MAX(MAX(ABS(sinY), ABS(cosY)), MAX(ABS(sinZ), ABS(cosZ)));

	if (largest == ABS(sinY)) {
		scale = (matrix->m[0][2] << 12) / sinY;
	} else if (largest == ABS(cosY)) {
		scale = (matrix->m[2][2] << 12) / cosY;
	} else if (largest == ABS(sinZ)) {
		scale = (matrix->m[1][0] << 12) / sinZ;
	} else if (largest == ABS(cosZ)) {
		scale = (matrix->m[1][1] << 12) / cosZ;
	}

	output->vx = ratan2(-matrix->m[1][2], scale);
}

void calculatePosition(GsCOORDINATE2 *coord, MATRIX *matrix)
{
	GsCOORDINATE2 *stack[100];
	GsCOORDINATE2 **ptr;

	ptr = stack;
	*ptr++ = coord;
	while (coord->super != NULL) {
		coord = coord->super;
		*ptr++ = coord;
	}

	ptr--;
	*matrix = (*ptr)->coord;

	while (stack < ptr) {
		ptr--;
		GsMulCoord3(matrix, &(*ptr)->coord);
	}
}

void multiplyRotations(SVECTOR *rotation1, SVECTOR *rotation2)
{
	MATRIX matrix1;
	MATRIX matrix2;
	MATRIX matrix3;

	if ((rotation2->vx == 0) &&
	    (rotation2->vy == 0) &&
	    (rotation2->vz == 0)) {
		return;
	}

	RotMatrixYXZ(rotation1, &matrix1);
	RotMatrixYXZ(rotation2, &matrix2);
	MulMatrix0(&matrix1, &matrix2, &matrix3);
	matrixToEuler2(&matrix3, rotation1);
}

int32_t customRandom(int32_t min, int32_t max)
{
	uint32_t combined;
	uint32_t range;
	int32_t tmp;

	if (max == min) {
		return min;
	}

	if (max < min) {
		tmp = min;
		min = max;
		max = tmp;
	}

	CUSTOM_RNG_1 = CUSTOM_RNG_1 * CUSTOM_RNG_FACTOR + CUSTOM_RNG_VALUE;
	CUSTOM_RNG_2 = CUSTOM_RNG_2 * CUSTOM_RNG_FACTOR + CUSTOM_RNG_VALUE;

	combined = (CUSTOM_RNG_1 >> 16) | (CUSTOM_RNG_2 << 16);

	range = max - min + 1;

	return min + (int32_t)(combined % range);
}
