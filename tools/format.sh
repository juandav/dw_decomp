#!/bin/sh

set -e

IGNORE='
include/__rts_info_t__.h
include/mwinline_n.h
src/main/_psstart.c
'

for FILE in "$@"; do
	for I in $IGNORE; do
		[ "$FILE" != "$I" ] || continue 2
	done
	clang-format -i "$FILE"
done
