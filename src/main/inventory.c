#include <libgs.h>

#include <dw/anim.h>
#include <dw/entity.h>
#include <dw/params.h>
#include <dw/types.h>
#include <dw/ui.h>
#include <dw/item.h>
#include <dw/world_object.h>

char CONFIRM_PROMPT[16] = "Are you sure?";
uint8_t MAIN_D_8012A9C4[3][6] = {
	{0, 1, 3, 2, 5, 4},
	{2, 5, 0, 1, 4, 3},
	{2, 0, 1, 3, 5, 4}
};
char ACTION_LABELS[4][8] = {"Use", "Move", "Sort", "Drop"};

char SORT_LABEL_BATTLE[8] = "Battle";
char SORT_LABEL_RAISE[8] = "Raise";
char SORT_LABEL_BASIC[8] = "Basic";
char CONFIRM_LABEL_YES[4] = "Yes";
char CONFIRM_LABEL_NO[4] = "No";

int32_t MAIN_D_80134E00;
int32_t INVENTORY_STATE;
int32_t INVENTORY_OPEN;
int16_t INVENTORY_SCROLL_ROW;
int16_t INVENTORY_ROW_OFFSET;
uint8_t INVENTORY_POINTER;
uint8_t INVENTORY_MOVE_SRC;
uint8_t INVENTORY_LAST_POINTER;
uint8_t ACTION_CURSOR;
uint8_t SORT_TYPE_CURSOR;
uint8_t CONFIRM_CURSOR;
int32_t INPUT_HOLD_FRAMES;

static void *inventory_sbss_order[] = {
	&INPUT_HOLD_FRAMES,
	&CONFIRM_CURSOR,
	&SORT_TYPE_CURSOR,
	&ACTION_CURSOR,
	&INVENTORY_LAST_POINTER,
	&INVENTORY_MOVE_SRC,
	&INVENTORY_POINTER,
	&INVENTORY_ROW_OFFSET,
	&INVENTORY_SCROLL_ROW,
	&INVENTORY_OPEN,
	&INVENTORY_STATE,
	&MAIN_D_80134E00,
};

extern int32_t POLLED_INPUT;
extern int32_t POLLED_INPUT_PREVIOUS;
void clearTextArea(void);
extern int8_t GAME_STATE;
extern char *COMBAT_DATA_PTR;
extern TamerEntity TAMER_ENTITY;
void addGameMenu(void);
void closeTriangleMenu(void);
void startFeedingItem(uint8_t type);
void startThrowingItem(void);
void getEntityScreenPos(Entity *e, int32_t mode, int16_t *out);
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e,
		  int32_t f, int32_t g, int32_t h, int32_t i);
void renderSelectionCursor(int32_t x, int32_t y, int16_t w, int16_t h, int32_t layer);
extern char *ITEM_DESC_PTR[];
void clearTextSubArea(RECT *area);
void drawString(char *text, int32_t color, int32_t pos);
void swapByte(uint8_t *a, uint8_t *b);
void playSound(int32_t vabId, uint32_t note);
void sortItems(int16_t mode);
void sortItemsById(uint8_t *data, int32_t count);
/* These legacy local declarations retain the retail O32 argument words.
 * The rendering helpers narrow their coordinate/value inputs. */
void renderItemSprite(int32_t type, int32_t x, int32_t y, int32_t layer);
void MAIN_func_800E58A4(int32_t color, int32_t n, int32_t x, int32_t y,
			int32_t value, int32_t layer);
void renderTrianglePrimitive(uint32_t color, int32_t x0, int32_t y0,
			     int32_t x1, int32_t y1, int32_t x2, int32_t y2,
			     int32_t layer, uint32_t mode);

int32_t getActionColor(int32_t mode);
void drawInventoryText(void);
void drawInventoryTextLine(int16_t startSlot);
void resetInventoryFlags();
void initializeInventoryObject();
void tickInventoryObject(int32_t instanceId);
void closeInventoryBoxes();
int32_t createInventoryUI(void);
void tickInventoryTop();
/* UI dispatch sign-extends the 16-bit instanceId. The explicit RenderFunction
 * casts retain the narrow callbacks' legacy MIPS ABI, not C type compatibility. */
void renderInventoryTop(int16_t boxId);
void renderInventoryBottom(int32_t arg);
void closeInventoryBoxes2();
void openActionMenu(void);
void tickActionMenu();
void renderActionMenu(int16_t boxId);
int32_t closeActionMenu(void);
void openSortTypeMenu();
void tickSortTypeMenu();
void renderSortTypeMenu(int16_t boxId);
void closeSubMenu();
void openDropConfirm();
void tickDropConfirm();
void renderDropConfirm(int16_t boxId);
void closeSubMenuThunk();
void updateItemNames();
void moveItem();
void selectSorting();
void confirmDrop();
void selectAction();
void selectInventoryItem();

void copyItemArray(uint8_t *src, uint8_t *dst, int32_t n);
void updateInputHold();
int32_t isInputTriggered(int32_t mask);
void moveMenuCursor(uint8_t *p, int32_t unused, int32_t max);

/* Definition placement preserves CodeWarrior scheduling; this anchor keeps
 * the retail function order independently of the definitions below. */
void *inventory_order_anchor[] = {
	moveMenuCursor,
	isInputTriggered,
	updateInputHold,
	copyItemArray,
	sortItems,
	selectInventoryItem,
	selectAction,
	confirmDrop,
	selectSorting,
	moveItem,
	updateItemNames,
	closeSubMenuThunk,
	renderDropConfirm,
	tickDropConfirm,
	openDropConfirm,
	closeSubMenu,
	renderSortTypeMenu,
	tickSortTypeMenu,
	openSortTypeMenu,
	closeActionMenu,
	renderActionMenu,
	tickActionMenu,
	openActionMenu,
	closeInventoryBoxes2,
	renderInventoryBottom,
	renderInventoryTop,
	tickInventoryTop,
	createInventoryUI,
	closeInventoryBoxes,
	tickInventoryObject,
	initializeInventoryObject,
	resetInventoryFlags,
	drawInventoryTextLine,
	drawInventoryText,
	getActionColor,
};

int32_t getActionColor(int32_t mode)
{
	Item *item;
	uint8_t t;

	if ((mode == 1) || (mode == 2)) {
		return 9;
	}
	t = INVENTORY.types.array[INVENTORY_POINTER];
	if (mode == 0) {
		if (t != 0xFF) {
			item = &ITEM_PARA[t];
			switch (GAME_STATE) {
			case 0:
				if ((item->itemColor == 0) ||
				    (item->itemColor == 2)) {
					return 9;
				}
				return 0xA;
			case 1:
			case 2:
			case 3:
				if ((item->itemColor == 1) ||
				    (item->itemColor == 2)) {
					return 9;
				}
				return 0xA;
			}
		}
		return 0xA;
	} else if (mode == 3) {
		if (GAME_STATE == 1) {
			return 0xA;
		}
		if (t != 0xFF) {
			item = &ITEM_PARA[t];
			if (item->droppable == 1) {
				return 9;
			}
			return 0xA;
		} else {
			return 0xA;
		}
	}
}

void drawInventoryText(void)
{
	int32_t i;
	int32_t slot;
	int32_t y;

	clearTextArea();
	for (i = 0, slot = 0; i < (INVENTORY.size / 2); ++i, slot += 2) {
		drawInventoryTextLine((int16_t)slot);
	}
	for (i = 0, y = 0; i < 4; ++i, y += 0xc) {
		drawString(ACTION_LABELS[i], 0xc0, y);
	}
}

void openActionMenu(void)
{
	RECT finalPos;
	RECT startPos;
	int32_t x;
	int32_t y;
	int16_t row;

	if ((UI_BOX_DATA[1].state == 1) &&
	    (UI_BOX_DATA[2].frame == 0) &&
	    (INVENTORY_STATE != 5)) {
		UI_BOX_DATA[2].features = 0;
		ACTION_CURSOR = 0;
		if ((INVENTORY_POINTER & 1) != 0) {
			x = UI_BOX_DATA[0].finalPos.x + 0x9a;
		} else {
			x = UI_BOX_DATA[0].finalPos.x + 0xa;
		}
		row = (INVENTORY_POINTER / 2) - INVENTORY_SCROLL_ROW;
		if (row < 5) {
			y = UI_BOX_DATA[0].finalPos.y + 0x18 + row * 0x12;
		} else {
			y = UI_BOX_DATA[0].finalPos.y + 0x20 + (row - 6) * 0x12;
		}
		finalPos.x = x;
		finalPos.y = y;
		finalPos.w = 0x3a;
		finalPos.h = 0x54;
		startPos.x = x - 3;
		startPos.y = UI_BOX_DATA[0].finalPos.y + 7 +
			     (INVENTORY_POINTER / 2 - INVENTORY_SCROLL_ROW) * 0x12;
		startPos.w = 0x8a;
		startPos.h = 0x12;
		createAnimatedUIBox(2, 1, 0, &finalPos, &startPos,
				    (TickFunction)tickActionMenu,
				    (RenderFunction)renderActionMenu);
	}
}

void drawInventoryTextLine(int16_t startSlot)
{
	int32_t slot;
	int32_t i;

	for (i = 0, slot = (int32_t)startSlot + i; i < 2; ++i, ++slot) {
		if ((INVENTORY.types.array + (int32_t)startSlot)[i] != 0xFF) {
			(INVENTORY.names.array + (int32_t)startSlot)[i] = slot;
			drawString(ITEM_PARA[INVENTORY.types.array[startSlot + i]].name,
				   (slot & 1) * 0x60, (slot / 2) * 0xC);
		}
	}
	DrawSync(0);
}

void resetInventoryFlags(void)
{
	INVENTORY_STATE = 0;
	INVENTORY_OPEN = 0;
}

void initializeInventoryObject(void)
{
	if ((INVENTORY_OPEN != 1) && (UI_BOX_DATA[0].state == 0) &&
	    (TAMER_ITEM.worldItem.type == 0xFF)) {
		INVENTORY_OPEN = 1;
		INVENTORY_STATE = 1;
		addObject(0x1A5, 0, tickInventoryObject, 0);
	}
}


static void tickInventoryObject__garbage__(void)
{
        int32_t v0;
        int32_t v1;
        int32_t v2;
        int32_t v3;

        v0 = INVENTORY.amounts.array[0] + 0;
        v1 = INVENTORY.amounts.array[1] + 1;
        v2 = INVENTORY.amounts.array[2] + 2;
        v3 = INVENTORY.amounts.array[3] + 3;
        INVENTORY.amounts.array[0] = (uint8_t)((v0 * v1) + v2);
        INVENTORY.amounts.array[1] = (uint8_t)((v1 * v2) + v3);
        INVENTORY.amounts.array[2] = (uint8_t)((v2 * v3) + v0);
        INVENTORY.amounts.array[3] = (uint8_t)((v3 * v0) + v1);
}


void tickInventoryObject(int32_t instanceId)
{
  TamerEntity *tam;
  int new_var;
  uint8_t *it;
  if ((*((uint8_t *) (COMBAT_DATA_PTR + 0x64E))) != 1)
  {
    tam = &TAMER_ENTITY;
    if ((POLLED_INPUT & (~POLLED_INPUT_PREVIOUS)) & 0x40)
    {
      if (UI_BOX_DATA[3].state == 1)
      {
        if (ACTION_CURSOR == 2)
        {
          selectSorting();
        }
        else
        {
          confirmDrop();
        }
      }
      else
        if (UI_BOX_DATA[2].state == 1)
      {
        selectAction();
      }
      else
      {
        selectInventoryItem();
      }
    }
    if ((POLLED_INPUT & (~POLLED_INPUT_PREVIOUS)) & 0x10)
    {
      if (UI_BOX_DATA[3].state == 1)
      {
        if (ACTION_CURSOR == 2)
        {
          INVENTORY_STATE = 9;
        }
        else
        {
          INVENTORY_STATE = 0xF;
        }
      }
      else
        if (UI_BOX_DATA[2].state == 0)
      {
        new_var = 1;
        if (UI_BOX_DATA[0].state == 4)
        {
          playSound(0, 3);
          UI_BOX_DATA[0].state = new_var;
          INVENTORY_STATE = 0;
        }
        else
          if (UI_BOX_DATA[1].state == 1)
        {
          INVENTORY_STATE = 2;
        }
      }
      else
        if (UI_BOX_DATA[2].state == 1)
      {
        INVENTORY_STATE = 4;
      }
    }
    switch (INVENTORY_STATE)
    {
      case 1:
        createInventoryUI();
        if (tam->entity.anim.animId != 4)
      {
        startAnimation(&tam->entity, 4);
      }
        break;

      case 2:
        closeInventoryBoxes2();
        if ((GAME_STATE == 0) && (UI_BOX_DATA[0].frame == 0))
      {
        closeTriangleMenu();
        addGameMenu();
        startAnimation(&(&TAMER_ENTITY)->entity, 0);
      }
        break;

      case 3:
        openActionMenu();
        break;

      case 4:

      case 6:
        closeActionMenu();
        break;

      case 5:
        if (closeActionMenu() != 0)
      {
        closeInventoryBoxes2();
      }
        if (UI_BOX_DATA[0].state == 0)
      {
        if (GAME_STATE == 0)
        {
          startFeedingItem(INVENTORY.types.array[INVENTORY_POINTER]);
          INVENTORY_STATE = 0;
        }
        else
          if (GAME_STATE == 1)
        {
          startThrowingItem();
          INVENTORY_STATE = 0;
        }
      }
        break;

      case 7:
        if (UI_BOX_DATA[3].state != 0)
      {
        closeSubMenuThunk();
      }
      else
        if (UI_BOX_DATA[2].state != 0)
      {
        closeActionMenu();
      }
      else
      {
        INVENTORY_STATE = 0;
        it = &INVENTORY.types.array[INVENTORY_POINTER];
        removeItem(it[0], it[0x1E]);
      }
        break;

      case 8:
        openSortTypeMenu();
        break;

      case 9:
        closeSubMenu();
        break;

      case 11:

      case 12:

      case 13:
        if (UI_BOX_DATA[3].state != 0)
      {
        closeSubMenu();
      }
      else
        if (UI_BOX_DATA[2].state != 0)
      {
        closeActionMenu();
      }
      else
      {
        INVENTORY_STATE = 0;
      }
        break;

      case 14:
        openDropConfirm();
        break;

      case 15:
        closeSubMenuThunk();
        break;

    }

  }
}

void closeInventoryBoxes(void)
{
  int16_t new_var;
  int i;
  if (INVENTORY_OPEN != 0)
  {
    INVENTORY_OPEN = 0;
    INVENTORY_STATE = 0;
    for (i = 0; i < 4; i++)
    {
      if (UI_BOX_DATA[i].state != 0)
      {
        removeStaticUIBox(new_var = i);
      }
    }

    removeObject(0x1A5, 0);
  }
}

int32_t createInventoryUI(void)
{
	int16_t xy[2];
	RECT finalPos;
	RECT startPos;
	uint8_t features;
	UIBoxData *box;

	if (UI_BOX_DATA[1].state == 1) {
		return 1;
	}
	box = &UI_BOX_DATA[0];
	if (UI_BOX_DATA[0].frame == 0) {
		INVENTORY_SCROLL_ROW = 0;
		INVENTORY_ROW_OFFSET = 0;
		MAIN_D_80134E00 = 0;
		INVENTORY_POINTER = 0;
		box->rowOffset = 0;
		box->visibleRows = 9;
		box->totalRows = 5;
		features = 2;
		if (INVENTORY.size > 10) {
			features |= 4;
			box->totalRows = INVENTORY.size == 20 ? 10 : 15;
		}
		finalPos.x = -0x98;
		finalPos.y = -0x68;
		finalPos.w = 0x130;
		finalPos.h = INVENTORY.size == 10 ? 0x6E : 0xB6;
		getEntityScreenPos(ENTITY_TABLE[0], 1, xy);
		startPos.x = xy[0] - 5;
		startPos.y = xy[1] - 5;
		startPos.w = 10;
		startPos.h = 10;
		createAnimatedUIBox(0, 0, features, &finalPos, &startPos,
				    (TickFunction)tickInventoryTop, (RenderFunction)renderInventoryTop);
	}
	if (box->state != 1) {
		return 0;
	}
	if (UI_BOX_DATA[1].frame == 0) {
		INVENTORY_LAST_POINTER = 0xFF;
		finalPos.x = -0x98;
		finalPos.y = 0x4D;
		finalPos.w = 0x130;
		UI_BOX_DATA[1].features = 2;
		finalPos.h = 0x1C;
		startPos.x = box->finalPos.x + 8;
		startPos.y = box->finalPos.y + 0xE;
		startPos.w = 0x10;
		startPos.h = 0x10;
		createAnimatedUIBox(1, 0, 2, &finalPos, &startPos,
				    NULL, renderInventoryBottom);
	}
	return 0;
}

void tickInventoryTop(void)
{
	if (UI_BOX_DATA[2].state == 0 && UI_BOX_DATA[1].state == 1) {
		updateInputHold();
		if (isInputTriggered(0x1000) && INVENTORY_POINTER >= 2) {
			playSound(0, 2);
			INVENTORY_POINTER -= 2;
		}
		if (isInputTriggered(0x4000) && INVENTORY_POINTER < INVENTORY.size - 2) {
			playSound(0, 2);
			INVENTORY_POINTER += 2;
		}
		if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x8000) &&
		    (INVENTORY_POINTER & 1)) {
			playSound(0, 2);
			--INVENTORY_POINTER;
		}
		if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x2000) &&
		    !(INVENTORY_POINTER & 1)) {
			playSound(0, 2);
			++INVENTORY_POINTER;
		}
		if (INVENTORY_POINTER < INVENTORY_SCROLL_ROW * 2) {
			--INVENTORY_SCROLL_ROW;
		}
		/* Retail reloads this cursor byte after the preceding scroll update. */
		if (*(volatile uint8_t *)&INVENTORY_POINTER >
		    (INVENTORY_SCROLL_ROW + UI_BOX_DATA[0].visibleRows - 1) * 2 + 1) {
			++INVENTORY_SCROLL_ROW;
		}
	}
}

void renderInventoryTop(int16_t boxId)
{
	UIBoxData *box;
	int32_t i;
	int32_t slot;
	int32_t count;
	int32_t row;
	uint8_t type;
	uint8_t color;
	int16_t x;
	int16_t y;

	box = &UI_BOX_DATA[boxId];
	slot = INVENTORY_SCROLL_ROW * 2;
	x = box->finalPos.x;
	y = box->finalPos.y;
	count = INVENTORY.size == 10 ? 10 : 18;
	for (i = 0; i < count; ++i, ++slot) {
		type = INVENTORY.types.array[slot];
		if (type != 0xFF) {
			renderItemSprite(type, (i & 1) ? x + 0x9A : x + 0xA,
					 y + 8 + (i / 2) * 0x12, 6 - boxId);
			if (ITEM_PARA[type].itemColor == 0xFF) {
				color = 8;
			} else {
				color = ITEM_PARA[type].itemColor + 5;
			}
			renderString(color, (i & 1) ? x + 0xAD : x + 0x1D,
				     y + 0xA + (i / 2) * 0x12, 0x60, 0xC,
				     ((int32_t)INVENTORY.names.array[slot] & 1) * 0x60,
				     ((int32_t)INVENTORY.names.array[slot] / 2) * 0xC,
				     6 - boxId, 1);
			MAIN_func_800E58A4(0, 2, (i & 1) ? x + 0x113 : x + 0x83,
					  y + 0xC + (i / 2) * 0x12,
					  INVENTORY.amounts.array[slot], 6 - boxId);
		}
	}
	if (box->state == 4) {
		row = (int32_t)INVENTORY_MOVE_SRC / 2 - INVENTORY_SCROLL_ROW;
		if (0 <= row && row < box->visibleRows) {
			if (INVENTORY_MOVE_SRC & 1) {
			x = box->finalPos.x + 0x97;
		} else {
			x = box->finalPos.x + 7;
		}
			y = box->finalPos.y + 7 + row * 0x12;
			renderTrianglePrimitive(0x5D4AF1, x, y + 0x12, x, y,
						x + 0x8A, y, 6 - boxId, 0);
			renderTrianglePrimitive(0x5D4AF1, x + 0x8A, y,
						x + 0x8A, y + 0x12, x, y + 0x12,
						6 - boxId, 0);
		}
	}
	if (INVENTORY_POINTER & 1) {
			x = box->finalPos.x + 0x97;
		} else {
			x = box->finalPos.x + 7;
		}
	y = box->finalPos.y + 7 + ((int32_t)INVENTORY_POINTER / 2 - INVENTORY_SCROLL_ROW) * 0x12;
	renderSelectionCursor(x, y, 0x8A, 0x12, 6 - boxId);
	box->rowOffset = INVENTORY_SCROLL_ROW;
	INVENTORY_ROW_OFFSET = box->rowOffset;
}

void renderInventoryBottom(int32_t arg)
{
	if (INVENTORY.types.array[INVENTORY_POINTER] != 0xFF) {
		if (INVENTORY_POINTER != INVENTORY_LAST_POINTER) {
			updateItemNames();
		}
		INVENTORY_LAST_POINTER = INVENTORY_POINTER;
		renderString(9, UI_BOX_DATA[1].finalPos.x + 0x1A,
			     UI_BOX_DATA[1].finalPos.y + 8, 0xFC, 0xC, 0, 0xB4,
			     6 - arg, 1);
	}
}

void closeInventoryBoxes2(void)
{
	int16_t xy[2];
	RECT r;
	int16_t x;

	if (UI_BOX_DATA[0].frame == 0) {
		closeInventoryBoxes();
		return;
	}
	if (UI_BOX_DATA[1].state == 1) {
		if (INVENTORY_POINTER & 1) {
			x = UI_BOX_DATA[0].finalPos.x + 0x9A;
		} else {
			x = UI_BOX_DATA[0].finalPos.x + 0xA;
		}
		r.x = x;
		r.y = UI_BOX_DATA[0].finalPos.y + 8 +
		      ((int16_t)((INVENTORY_POINTER / 2) - INVENTORY_SCROLL_ROW) * 0x12);
		r.w = 0x10;
		r.h = 0x10;
		removeAnimatedUIBox(1, &r);
	}
	if ((UI_BOX_DATA[1].frame <= 0) && (UI_BOX_DATA[0].state == 1)) {
		getEntityScreenPos(ENTITY_TABLE[0], 1, xy);
		r.x = xy[0] - 5;
		r.y = xy[1] - 5;
		r.w = 0xA;
		r.h = 0xA;
		removeAnimatedUIBox(0, &r);
	}
}


void tickActionMenu(void)
{
	if (UI_BOX_DATA[3].state == 0) {
		moveMenuCursor(&ACTION_CURSOR, 2, 4);
	}
}



int32_t closeActionMenu(void)
{
	if (UI_BOX_DATA[2].frame == 0) {
		return 1;
	}
	if (UI_BOX_DATA[2].state == 1) {
		removeAnimatedUIBox(2, NULL);
	}
	return 0;
}

void openSortTypeMenu(void)
{
	RECT r1;
	RECT r2;

	if ((UI_BOX_DATA[2].state == 1) && (UI_BOX_DATA[3].state == 0)) {
		drawString(SORT_LABEL_BATTLE, 0xC0, 0x30);
		drawString(SORT_LABEL_RAISE, 0xC0, 0x3C);
		drawString(SORT_LABEL_BASIC, 0xC0, 0x48);
		SORT_TYPE_CURSOR = 0;
		r1.x = UI_BOX_DATA[2].finalPos.x + UI_BOX_DATA[2].finalPos.w;
		r1.y = UI_BOX_DATA[2].finalPos.y;
		r1.w = 0x48;
		r1.h = 0x42;
		r2.x = UI_BOX_DATA[2].finalPos.x + 9;
		r2.y = UI_BOX_DATA[2].finalPos.y + 6 + (ACTION_CURSOR * 0x12);
		r2.w = 0x28;
		r2.h = 0x10;
		createAnimatedUIBox(3, 1, 0, &r1, &r2,
				    (TickFunction)tickSortTypeMenu,
				    (RenderFunction)renderSortTypeMenu);
	}
}

void renderActionMenu(int16_t boxId)
{
	UIBoxData *box;
	int16_t x;
	int16_t y;
	int32_t i;

	box = &UI_BOX_DATA[boxId];
	x = box->finalPos.x + 9;
	i = 0;
	y = box->finalPos.y + 6 + ACTION_CURSOR * 0x12;

	for (; i < 4; ++i) {
		renderString(getActionColor(i), x + 3,
			     box->finalPos.y + 8 + i * 0x12, 0x24, 0xC,
			     0xC0, i * 0xC, 6 - boxId, 1);
	}
	renderSelectionCursor(x, y, 0x28, 0x10, 6 - boxId);
}

void tickSortTypeMenu(void)
{
	moveMenuCursor(&SORT_TYPE_CURSOR, 3, 3);
}

void renderSortTypeMenu(int16_t boxId)
{
	int32_t i;
	int16_t x;
	int16_t y;
	RECT *pos;

	pos = &UI_BOX_DATA[boxId].finalPos;
	x = pos->x + 9;
	y = pos->y + 6 + SORT_TYPE_CURSOR * 0x12;

	for (i = 0; i < 3; ++i) {
		renderString(9, x + 3, pos->y + 8 + i * 0x12, 0x30,
			     0xC, 0xC0, 0x30 + i * 0xC, 6 - boxId, 1);
	}
	renderSelectionCursor(x, y, 0x36, 0x10, 6 - boxId);
}

void closeSubMenu(void)
{
	if (UI_BOX_DATA[3].state == 1) {
		removeAnimatedUIBox(3, NULL);
	}
}

void openDropConfirm(void)
{
	RECT r1;
	RECT r2;

	if ((UI_BOX_DATA[2].state == 1) && (UI_BOX_DATA[3].state == 0)) {
		drawString(CONFIRM_LABEL_YES, 0xC5, 0x54);
		drawString(CONFIRM_LABEL_NO, 0xC0, 0x60);
		drawString(CONFIRM_PROMPT, 0, 0xC0);
		r1.x = -0x40;
		r1.y = -0x26;
		r1.w = 0x80;
		r1.h = 0x35;
		r2.x = UI_BOX_DATA[2].finalPos.x + 9;
		CONFIRM_CURSOR = 1;
		r2.y = UI_BOX_DATA[2].finalPos.y + 6 + (ACTION_CURSOR * 0x12);
		r2.w = 0x28;
		r2.h = 0x10;
		createAnimatedUIBox(3, 1, 0, &r1, &r2,
				    (TickFunction)tickDropConfirm,
				    (RenderFunction)renderDropConfirm);
	}
}

void tickDropConfirm(void)
{
	updateInputHold();
	if ((isInputTriggered(0x8000) != 0) && (CONFIRM_CURSOR != 0)) {
		playSound(0, 2);
		--CONFIRM_CURSOR;
	}
	if ((isInputTriggered(0x2000) != 0) && (CONFIRM_CURSOR == 0)) {
		playSound(0, 2);
		++CONFIRM_CURSOR;
	}
}

void renderDropConfirm(int16_t boxId)
{
	int32_t i;
	int32_t labelX;
	int16_t x;
	int16_t y;
	UIBoxData *box;

	box = &UI_BOX_DATA[boxId];
	x = box->finalPos.x + 0x12 + CONFIRM_CURSOR * 0x35;
	y = box->finalPos.y + 0x1E;

	renderString(9, -0x34, -0x1E, 0x6C, 0xC, 0, 0xC0, 6 - boxId, 1);
	for (i = 0; i < 2; ++i) {
		labelX = box->finalPos.x + 0x12 + i * 0x34;
		renderString(9, labelX + 5,
			     y + 2, 0x24, 0xC, 0xC0, 0x54 + i * 0xC,
			     6 - boxId, 1);
	}
	renderSelectionCursor(x, y, 0x2A, 0x10, 6 - boxId);
}

void closeSubMenuThunk(void)
{
	closeSubMenu();
}

void updateItemNames(void)
{
	RECT area;

	area.x = 0;
	area.y = 0xB4;
	area.w = 0xFC;
	area.h = 0xC;
	clearTextSubArea(&area);
	drawString(ITEM_DESC_PTR[INVENTORY.types.array[INVENTORY_POINTER]], 0, 0xB4);
	DrawSync(0);
}

void moveItem(void)
{
	uint8_t *p;

	p = &INVENTORY.types.array[INVENTORY_POINTER];
	swapByte(p, &INVENTORY.types.array[INVENTORY_MOVE_SRC]);
	swapByte(&INVENTORY.types.array[INVENTORY_POINTER] + 0x1E,
		 &INVENTORY.types.array[INVENTORY_MOVE_SRC] + 0x1E);
	swapByte(&INVENTORY.types.array[INVENTORY_POINTER] + 0x3C,
		 &INVENTORY.types.array[INVENTORY_MOVE_SRC] + 0x3C);
	if (*p != 0xFF) {
		updateItemNames();
	}
}

void selectSorting(void)
{
	if (INVENTORY_STATE != SORT_TYPE_CURSOR + 0xB) {
		sortItems(SORT_TYPE_CURSOR + 0xB);
		INVENTORY_STATE = SORT_TYPE_CURSOR + 0xB;
	}
}

void confirmDrop(void)
{
	if (CONFIRM_CURSOR == 0) {
		INVENTORY_STATE = 7;
	} else {
		INVENTORY_STATE = 0xF;
	}
}

void selectAction(void)
{
	Item *item;
	uint8_t t;

	if ((INVENTORY_STATE != 0xB) && (INVENTORY_STATE != 0xC) &&
	    (INVENTORY_STATE != 0xD) && (UI_BOX_DATA[3].state == 0)) {
		switch (ACTION_CURSOR) {
		case 0:
			t = INVENTORY.types.array[INVENTORY_POINTER];
			if (t != 0xFF) {
				item = &ITEM_PARA[INVENTORY.types.array[INVENTORY_POINTER]];
				switch (GAME_STATE) {
				case 0:
					if ((item->itemColor == 0) ||
					    (item->itemColor == 2)) {
						INVENTORY_STATE = 5;
					}
					break;
				case 1:
					if ((item->itemColor == 1) ||
					    (item->itemColor == 2)) {
						INVENTORY_STATE = 5;
					}
					break;
				}
			}
			break;
		case 1:
			INVENTORY_STATE = 6;
			INVENTORY_MOVE_SRC = INVENTORY_POINTER;
			UI_BOX_DATA[0].state = 4;
			return;
		case 2:
			INVENTORY_STATE = 8;
			break;
		case 3:
			if (GAME_STATE == 0) {
				t = INVENTORY.types.array[INVENTORY_POINTER];
				if ((t != 0xFF) && ITEM_PARA[INVENTORY.types.array[INVENTORY_POINTER]].droppable == 1) {
					INVENTORY_STATE = 0xE;
				}
			}
			break;
		}
	}
}

void selectInventoryItem(void)
{
	if ((INVENTORY_STATE != 0xB) && (INVENTORY_STATE != 0xC) &&
	    (INVENTORY_STATE != 0xD) && (INVENTORY_STATE != 5) &&
	    (UI_BOX_DATA[2].state == 0)) {
		if (UI_BOX_DATA[0].state == 0) {
			INVENTORY_STATE = 1;
			return;
		}
		if (INVENTORY_STATE == 6) {
			playSound(0, 4);
			UI_BOX_DATA[0].state = 1;
			INVENTORY_STATE = 0;
			moveItem();
			return;
		}
		if ((INVENTORY_STATE < 5) && (TAMER_ITEM.worldItem.type == 0xFF) &&
		    (UI_BOX_DATA[1].state == 1)) {
			INVENTORY_STATE = 3;
		}
	}
}

void sortItems(int16_t mode)
{
	int32_t counts[6];
	uint8_t desired[30];
	uint8_t categories[6][30];
	int32_t i;
	int32_t j;
	uint8_t type;
	uint8_t *other;
	uint8_t *order;

	INVENTORY_LAST_POINTER = 0xFF;
	for (i = 0; i < INVENTORY.size; ++i) {
		desired[i] = 0xFF;
	}
	for (i = 0; i < 6; ++i) {
		counts[i] = 0;
		for (j = 0; j < INVENTORY.size; ++j) {
			if (INVENTORY.types.array[j] != 0xFF) {
				type = INVENTORY.types.array[j];
				if (i == ITEM_PARA[type].sortingValue) {
					categories[i][counts[i]] = type;
					++counts[i];
				}
			}
		}
		sortItemsById(categories[i], counts[i]);
	}
	i = 0;
	order = MAIN_D_8012A9C4[mode - 11];
	for (j = 0; j < 6; ++j, ++order) {
		copyItemArray(categories[*order],
			      desired + i, counts[*order]);
		i += counts[*order];
	}
	for (i = 0; i < INVENTORY.size; ++i) {
		if (desired[i] == 0xFF) {
			break;
		}
		if (INVENTORY.types.array[i] != desired[i]) {
			for (j = 0; j < INVENTORY.size; ++j) {
				if (INVENTORY.types.array[j] == desired[i]) {
					break;
				}
			}
			swapByte(((uint8_t *)&INVENTORY + i),
				 other = ((uint8_t *)&INVENTORY + j));
			swapByte(((uint8_t *)&INVENTORY + i) + 0x1E, other + 0x1E);
			swapByte(((uint8_t *)&INVENTORY + i) + 0x3C, other + 0x3C);
		}
	}
}

void copyItemArray(uint8_t *src, uint8_t *dst, int32_t n)
{
	int32_t i;

	for (i = 0; i < n; ++i) {
		*dst++ = *src++;
	}
}

void updateInputHold(void)
{
	if (POLLED_INPUT == POLLED_INPUT_PREVIOUS) {
		++INPUT_HOLD_FRAMES;
		return;
	}
	INPUT_HOLD_FRAMES = 0;
}

int32_t isInputTriggered(int32_t mask)
{
	if ((mask & (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS)) ||
	    ((INPUT_HOLD_FRAMES >= 0xB) && (mask & POLLED_INPUT))) {
		return 1;
	}
	return 0;
}

void moveMenuCursor(uint8_t *p, int32_t unused, int32_t max)
{
	updateInputHold();
	if ((isInputTriggered(0x1000) != 0) && (*p != 0)) {
		playSound(0, 2);
		--*p;
	}
	if ((isInputTriggered(0x4000) != 0) && (*p < max - 1)) {
		playSound(0, 2);
		++*p;
	}
}
