#include <libgpu.h>

#include <dw/font.h>
#include <dw/graphics.h>

extern RECT CARD_VIEW_TEXT_AREA;
extern char *CARD_CHART_LABELS[];
extern char MAIN_D_80124C54[];
extern int8_t MENU_SUB_STATE;
extern int8_t SELECTED_CARD;

int32_t drawCardViewStrings(void);

int32_t drawCardViewStrings(void)
{
	RECT rect;

	rect = CARD_VIEW_TEXT_AREA;
	switch (MENU_SUB_STATE) {
	case 0:
		clearTextSubArea(&rect);
		drawString(MAIN_D_80124C54, 0, 0xF0);
		MENU_SUB_STATE = 1;
		break;
	case 1:
		drawString(CARD_CHART_LABELS[0], 0, 0xC);
		SELECTED_CARD = 0;
		return 1;
	}
	return 0;
}
