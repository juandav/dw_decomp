#include <stdio.h>

#include <dw/clock.h>
#include <dw/params.h>
#include <dw/ui.h>

extern int8_t MENU_SUB_STATE;
extern char MAIN_D_80124C54[];
extern RECT MAIN_D_801342F8;
extern char MAIN_D_80134300[8];
extern char *MAIN_D_80124800[];
extern char *MAIN_D_80124918[];

void clearTextSubArea(RECT *area);
void drawString(char *text, int32_t color, int32_t pos);
int32_t drawPlayerInfoStrings(void);

int32_t drawPlayerInfoStrings(void)
{
	RECT rect;
	char buf[8];
	int32_t i;

	rect = MAIN_D_801342F8;
	switch (MENU_SUB_STATE) {
	case 0:
		clearTextSubArea(&rect);
		drawString(MAIN_D_80124C54, 0, 0xf0);
		MENU_SUB_STATE = 1;
		break;
	case 1:
		sprintf(buf, MAIN_D_80134300, (int)PLAYTIME_HOURS, (int)PLAYTIME_MINUTES);
		drawString(buf, 0, 0xe4);
	case 2:
	case 3:
		drawString(MAIN_D_80124800[MENU_SUB_STATE + 1], 0,
		    (MENU_SUB_STATE - 1) * 0xc + 0xc);
		MENU_SUB_STATE = MENU_SUB_STATE + 1;
		break;
	case 4:
		drawString((char *)DIGIMON_DATA, 0, 0x30);
		for (i = 0; i < 4; i++) {
			drawString(MAIN_D_80124918[i], 0, i * 0xc + 0x40);
		}
		MENU_SUB_STATE = 5;
		break;
	case 5:
		for (i = 4; i < 0xb; i++) {
			drawString(MAIN_D_80124918[i], 0, i * 0xc + 0x40);
		}
		return 1;
	}
	return 0;
}
