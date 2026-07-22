#include <libgpu.h>

#include <dw/graphics.h>

extern RECT MAIN_D_80134328;
extern char *MAIN_D_80124814[];
extern char MAIN_D_80124C54[];
extern int8_t MENU_SUB_STATE;
extern int8_t SELECTED_CARD;

void drawString(char *text, int32_t color, int32_t pos);
void clearTextSubArea(RECT *area);
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
		drawString(MAIN_D_80124C54, 0, 0xF0);
		MENU_SUB_STATE = 1;
		goto ret0;
	}
	drawString(MAIN_D_80124814[0], 0, 0xC);
	SELECTED_CARD = 0;
	return 1;
ret0:
	return 0;
}
