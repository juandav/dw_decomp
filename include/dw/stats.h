#ifndef DW_STATS_H
#define DW_STATS_H

#include <dw/types.h>

typedef struct {
	int16_t off;
	int16_t def;
	int16_t speed;
	int16_t brain;
	uint8_t movesPrio[4];
	uint8_t moves[4];
	int16_t hp;
	int16_t mp;
	int16_t currentHP;
	int16_t currentMP;
} Stats;

#endif
