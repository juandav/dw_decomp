#include <dw/graphics.h>
#include <dw/ui.h>

extern int8_t MENU_SUB_STATE;
extern int8_t MEDAL_SELECTOR_INDEX;
extern RECT MAIN_D_80134318;
extern RECT MAIN_D_80134320;
extern char MAIN_D_80124C54[];
extern char *MAIN_D_80124804[];
extern char *MAIN_D_80124828[];
extern char *MAIN_D_80124864[];

void clearTextSubArea(RECT *area);
void drawString(char *text, int32_t color, int32_t pos);
int32_t drawMedalViewStrings(void);

int32_t drawMedalViewStrings(void)
{
	RECT rect1;
	RECT rect2;
	int32_t i;

	rect1 = MAIN_D_80134318;
	rect2 = MAIN_D_80134320;
	switch (MENU_SUB_STATE) {
	case 0:
		clearTextSubArea(&rect1);
		drawString(MAIN_D_80124C54, 0, 0xf0);
		MENU_SUB_STATE = 1;
		break;
	case 1:
		drawString(MAIN_D_80124804[0], 0, 0xc);
		MENU_SUB_STATE = 2;
		break;
	case 2:
		drawString(MAIN_D_80124828[MEDAL_SELECTOR_INDEX], 0, 0x18);
		MENU_SUB_STATE = 3;
		break;
	case 3:
	case 4:
	case 5:
		i = MENU_SUB_STATE - 3;
		drawString(MAIN_D_80124864[i + MEDAL_SELECTOR_INDEX * 3], 0,
			   i * 0xc + 0x24);
		MENU_SUB_STATE = MENU_SUB_STATE + 1;
		if (MENU_SUB_STATE == 6) {
			return 1;
		}
		break;
	case 6:
		clearTextSubArea(&rect2);
		MENU_SUB_STATE = 2;
	}
	return 0;
}
