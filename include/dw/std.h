#ifndef DW_STD_H
#define DW_STD_H

#include <libgte.h>

#include <dw/graphics.h>
#include <dw/types.h>

typedef struct {
	uint8_t modelCount[2];
	uint8_t timCount[2];
} StdArenaCfg;

extern uint8_t STD_D_80052AE0[];
extern char STD_D_8007AAB0[];

extern StdArenaCfg MAIN_D_801347FC;
extern uint8_t MAIN_D_80134800[4];
extern uint8_t MAIN_D_80134804[4];
extern char MAIN_D_80134808[5];
extern char MAIN_D_80134810[5];
extern SVECTOR MAIN_D_80134818;
extern SVECTOR MAIN_D_80134820;
extern SVECTOR MAIN_D_80134828;
extern SVECTOR MAIN_D_80134830;
extern SVECTOR MAIN_D_80134838;
extern SVECTOR MAIN_D_80134840;
extern SVECTOR MAIN_D_80134848;
extern SVECTOR MAIN_D_80134850;
extern SVECTOR MAIN_D_80134858;
extern SVECTOR MAIN_D_80134860;
extern SVECTOR MAIN_D_80134868;
extern int16_t MAIN_D_80134870[4];
extern char MAIN_D_80134878[7];
extern uint8_t MAIN_D_80134880[5];
extern uint8_t MAIN_D_80134888[4];
extern uint8_t MAIN_D_8013488C[4];
extern uint8_t MAIN_D_80134890[4];
extern uint8_t MAIN_D_801348B0[5];
extern uint8_t MAIN_D_801348B8[5];
extern uint8_t MAIN_D_801348C0[8];
extern uint8_t MAIN_D_801348C8[8];
extern uint8_t MAIN_D_801348D0[8];
extern char *MAIN_D_801348D8;
extern int16_t MAIN_D_801348DC[4];
extern char MAIN_D_801348E4[4];
extern int8_t MAIN_D_801348E8[4];
extern int8_t MAIN_D_801348EC[4];
extern int8_t MAIN_D_801348F0[8];
extern int8_t MAIN_D_801348F8[8];
extern int8_t MAIN_D_80134900[8];
extern int32_t MAIN_D_80134908;
extern uint8_t MAIN_D_8013490C[8];
extern int8_t MAIN_D_80134914[6];
extern SVECTOR MAIN_D_8013491C;
extern int16_t MAIN_D_80134924[4];
extern SVECTOR MAIN_D_8013492C;
extern RGB8 MAIN_D_80134934;
extern SVECTOR MAIN_D_80134938;
extern SVECTOR MAIN_D_80134940;
extern SVECTOR MAIN_D_80134948;
extern SVECTOR MAIN_D_80134950;

int16_t STD_func_800579D8(uint8_t *arg);
void STD_tickNPCTournament(int32_t instanceId);
void STD_tickPartnerTournament(int32_t instanceId);
void STD_tickTamerTournament(int32_t instanceId);

#endif
