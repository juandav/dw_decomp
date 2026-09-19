#include <dw/font.h>
#include <dw/graphics.h>
#include <dw/params.h>
#include <dw/ui.h>

extern int8_t MENU_SUB_STATE;
extern char MAIN_D_80124C54[];
extern int16_t MAIN_D_80134D42;
extern int16_t MAIN_D_80134D44;
extern RECT MAIN_D_80134308;
extern RECT MAIN_D_80134310;
extern char *MAIN_D_80124814[];

int32_t drawEvoChartStrings(int32_t arg);

int32_t drawEvoChartStrings(int32_t arg)
{
	RECT rect1;
	RECT rect2;
	int32_t result;

	rect1 = MAIN_D_80134308;
	rect2 = MAIN_D_80134310;
	switch (MENU_SUB_STATE) {
	case 0:
		clearTextSubArea(&rect1);
		drawString(MAIN_D_80124C54, 0, 0xf0);
		drawString(MAIN_D_80124814[1], 0, 0xc);
		MAIN_D_80134D44 = 0;
		MAIN_D_80134D42 = 0;
		MENU_SUB_STATE = 1;
		/* fall through */
	case 1:
		drawString(MAIN_D_80124814[2], 0, 0x18);
		drawString(MAIN_D_80124814[3], 0, 0x24);
		drawString(MAIN_D_80124814[4], 0, 0x3c);
		result = 1;
		break;
	case 2:
		clearTextSubArea(&rect2);
		drawString((char *)(DIGIMON_DATA + arg), 0, 0x30);
		/* fall through */
	default:
		result = 0;
		break;
	}
	return result;
}
