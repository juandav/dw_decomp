#include <libgpu.h>

#include <dw/font.h>
#include <dw/graphics.h>

extern RECT MAIN_D_80134328;
extern char *MAIN_D_80124814[];
extern char FULLWIDTH_DIGITS[];
extern int8_t MENU_SUB_STATE;
extern int8_t SELECTED_CARD;

int32_t drawCardViewStrings(void);

int32_t drawCardViewStrings(void)
{
	RECT rect;
	int32_t state;

	rect = MAIN_D_80134328;
	state = MENU_SUB_STATE;
	if (state != 1) {
		if (state != 0) {
			goto ret0;
		}
		clearTextSubArea(&rect);
		drawString(FULLWIDTH_DIGITS, 0, 0xF0);
		MENU_SUB_STATE = 1;
		goto ret0;
	}
	drawString(MAIN_D_80124814[0], 0, 0xC);
	SELECTED_CARD = 0;
	return 1;
ret0:
	return 0;
}
