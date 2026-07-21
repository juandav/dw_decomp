#include <libgs.h>
#include <libgte.h>
#include <mwinline_n.h>

#include <dw/types.h>

#include "common.h"

extern _GsFCALL GS_TMD_MAP;

PACKET *GsTMDfastF3L();
PACKET *GsTMDfastG3L();
PACKET *GsTMDfastF4L();
PACKET *GsTMDfastF4NL();
PACKET *GsTMDfastG4L();
PACKET *GsTMDfastNF4();
PACKET *GsTMDfastTF3L();
PACKET *GsTMDfastTF3NL();
PACKET *GsTMDfastTNF3();
PACKET *GsTMDfastTF4L();
PACKET *GsTMDfastTF4NL();
PACKET *GsTMDfastTNF4();
PACKET *GsTMDfastTG3L();
PACKET *GsTMDfastTG3NL();
PACKET *GsTMDfastTNG3();
PACKET *GsTMDfastTG4L();
PACKET *GsTMDfastTG4NL();
PACKET *GsTMDfastTNG4();
PACKET *GsTMDdivTF3NL();
PACKET *GsTMDdivTNF3();
PACKET *GsTMDdivTG3NL();
PACKET *GsTMDdivTNG3();
PACKET *GsTMDdivTF4L();
PACKET *GsTMDdivTF4NL();
PACKET *GsTMDdivTNF4();
PACKET *GsTMDdivTG4NL();
PACKET *GsTMDdivTNG4();

void setRotTransMatrix(MATRIX *m);
void initializeGsTMDMap(void);
extern int32_t MAIN_D_80137BE8;
typedef struct {
	int32_t f0;
	int32_t f1;
	int32_t f2;
	int32_t f3;
	int32_t f4;
	int32_t f5;
	int32_t f6;
} AttackObjectData;
extern AttackObjectData MAIN_D_80122584;
extern AttackObjectData MAIN_D_80137A24[];
void initializeAttackObjects(void);
int32_t addAttackObject(int32_t a, int32_t b, int16_t *rect, int32_t d, int32_t e, int32_t f);
void popAttackObject(int32_t a);
void updateTMDTextureData(char *tmd, int32_t clutX, int32_t x, int32_t y, int32_t tpage);
void renderDropShadow(int32_t a, int32_t b, int32_t c);

void setRotTransMatrix(MATRIX *m)
{
	gte_SetRotMatrix(m);
	gte_SetTransMatrix(m);
}

void initializeGsTMDMap(void)
{
	GS_TMD_MAP.f3[GsDivMODE_NDIV][GsLMODE_NORMAL]	= GsTMDfastF3L;
	GS_TMD_MAP.tf3[GsDivMODE_NDIV][GsLMODE_NORMAL]	= GsTMDfastTF3L;
	GS_TMD_MAP.tf3[GsDivMODE_NDIV][GsLMODE_LOFF]	= GsTMDfastTF3NL;
	GS_TMD_MAP.ntf3[GsDivMODE_NDIV]			= GsTMDfastTNF3;
	GS_TMD_MAP.g3[GsDivMODE_NDIV][GsLMODE_NORMAL]	= GsTMDfastG3L;
	GS_TMD_MAP.tg3[GsDivMODE_NDIV][GsLMODE_NORMAL]	= GsTMDfastTG3L;
	GS_TMD_MAP.tg3[GsDivMODE_NDIV][GsLMODE_LOFF]	= GsTMDfastTG3NL;
	GS_TMD_MAP.ntg3[GsDivMODE_NDIV]			= GsTMDfastTNG3;
	GS_TMD_MAP.f4[GsDivMODE_NDIV][GsLMODE_NORMAL]	= GsTMDfastF4L;
	GS_TMD_MAP.tf4[GsDivMODE_NDIV][GsLMODE_NORMAL]	= GsTMDfastTF4L;
	GS_TMD_MAP.tf4[GsDivMODE_NDIV][GsLMODE_LOFF]	= GsTMDfastTF4NL;
	GS_TMD_MAP.ntf4[GsDivMODE_NDIV]			= GsTMDfastTNF4;
	GS_TMD_MAP.g4[GsDivMODE_NDIV][GsLMODE_NORMAL]	= GsTMDfastG4L;
	GS_TMD_MAP.tg4[GsDivMODE_NDIV][GsLMODE_NORMAL]	= GsTMDfastTG4L;
	GS_TMD_MAP.tg4[GsDivMODE_NDIV][GsLMODE_LOFF]	= GsTMDfastTG4NL;
	GS_TMD_MAP.ntg4[GsDivMODE_NDIV]			= GsTMDfastTNG4;
	GS_TMD_MAP.tf4[GsDivMODE_DIV][GsLMODE_NORMAL]	= GsTMDdivTF4L;
	GS_TMD_MAP.tf4[GsDivMODE_DIV][GsLMODE_LOFF]	= GsTMDdivTF4NL;
	GS_TMD_MAP.tf3[GsDivMODE_DIV][GsLMODE_LOFF]	= GsTMDdivTF3NL;
	GS_TMD_MAP.tg4[GsDivMODE_DIV][GsLMODE_LOFF]	= GsTMDdivTG4NL;
	GS_TMD_MAP.tg3[GsDivMODE_DIV][GsLMODE_LOFF]	= GsTMDdivTG3NL;
	GS_TMD_MAP.ntg4[GsDivMODE_DIV]			= GsTMDdivTNG4;
	GS_TMD_MAP.ntg3[GsDivMODE_DIV]			= GsTMDdivTNG3;
	GS_TMD_MAP.ntf4[GsDivMODE_DIV]			= GsTMDdivTNF4;
	GS_TMD_MAP.ntf3[GsDivMODE_DIV]			= GsTMDdivTNF3;
	GS_TMD_MAP.f4[GsDivMODE_NDIV][GsLMODE_LOFF]	= GsTMDfastF4NL;
	GS_TMD_MAP.nf4[GsDivMODE_NDIV]			= GsTMDfastNF4;
}



void initializeAttackObjects(void)
{
  int32_t i;
  AttackObjectData tmp;
  i = 0;
  tmp = MAIN_D_80122584;
  for (; i < 0x10; i++)
  {
    MAIN_D_80137A24[i] = MAIN_D_80122584;
  }

}

INCLUDE_ASM("asm/main/nonmatchings/graphics", addAttackObject);

INCLUDE_ASM("asm/main/nonmatchings/graphics", popAttackObject);

INCLUDE_ASM("asm/main/nonmatchings/graphics", updateTMDTextureData);

INCLUDE_ASM("asm/main/nonmatchings/graphics", renderDropShadow);
