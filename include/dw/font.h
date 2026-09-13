#ifndef DW_FONT_H
#define DW_FONT_H

#include <libgpu.h>

#include <dw/types.h>

void initializeFontCLUT(void);
void clearTextArea(void);
void clearTextSubArea(RECT *rect);
void setTextColor(int32_t color);
int32_t drawGlyph(uint16_t codepoint, int32_t x, int32_t y);
void drawString(/* char *str, uint16_t x, int32_t y */);

#endif
