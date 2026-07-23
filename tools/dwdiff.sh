#!/bin/sh
# dwdiff.sh - build one object and diff one function against expected/
#
# The per-function inner loop, made permanent. Builds a single object,
# disassembles one function from both expected/ and build/, and reports:
#   - a normalised unified diff (reloc addresses and branch targets stripped,
#     so only real instruction differences show)
#   - the instruction counts on each side
#   - the opcode-multiset delta (empty => same instructions, only order/regs
#     differ, i.e. a pure scheduling/allocation miss - permuter territory)
#
# usage:
#   tools/dwdiff.sh <src.c> <func>          diff one function
#   tools/dwdiff.sh --raw <src.c> <func>    unnormalised diff (keeps offsets)
#   tools/dwdiff.sh --verify <src.c> <func> confirm expected/ is not contaminated
#
# --verify rebuilds the committed (HEAD) version of <src.c> and checks that it
# reproduces the bytes in expected/ for <func>. When the function is still
# INCLUDE_ASM in HEAD (the normal green state) this must MATCH; if it does not,
# expected/ was captured after an edit and every diff against it is a lie.
# See the "expected/ is a snapshot" trap in NOTES.md.
set -e

ROOT=$(cd -- "$(dirname -- "$(readlink -f -- "$0")")/.." && pwd)
cd "$ROOT"
[ -d .venv ] && . .venv/bin/activate 2>/dev/null || true

OBJD=mipsel-linux-gnu-objdump
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT

MODE=diff
case "$1" in
	--raw)    MODE=raw;    shift ;;
	--verify) MODE=verify; shift ;;
	-h|--help)
		sed -n '2,20p' "$0"; exit 0 ;;
esac

SRC="$1"; FN="$2"
[ -z "$SRC" ] || [ -z "$FN" ] && { sed -n '2,20p' "$0"; exit 1; }
OBJ="$SRC.o"

dis() {  # dis <objfile> <outfile> : one function, prefixes stripped
	$OBJD -d -r --disassemble="$FN" "$1" \
		| sed -n '/>:/,/^$/p' \
		| sed 's/^ *[0-9a-f]*:\t[0-9a-f]* \t/\t/' > "$2"
}
norm() {  # strip reloc addrs, sp offsets and branch targets - real diffs only
	sed -E '/R_MIPS/d;
		s/[0-9]+\(sp\)/N(sp)/g;
		s/[0-9a-f]+ <[^>]*>/T/g' "$1"
}

if [ "$MODE" = verify ]; then
	git show "HEAD:$SRC" > "$TMP/head.c"
	# build HEAD's version into an isolated object
	cp "$SRC" "$TMP/live.c"
	cp "$TMP/head.c" "$SRC"
	trap 'cp "$TMP/live.c" "$SRC"; rm -rf "$TMP"' EXIT
	make "build/$OBJ" >/dev/null 2>&1
	dis "build/$OBJ" "$TMP/head.txt"
	cp "$TMP/live.c" "$SRC"
	dis "expected/$OBJ" "$TMP/exp.txt"
	if diff -q "$TMP/exp.txt" "$TMP/head.txt" >/dev/null; then
		echo "OK  expected/ reproduces HEAD for $FN - trustworthy"
	else
		echo "STALE  expected/ does NOT match a build of HEAD for $FN."
		echo "       expected/ is contaminated. Rebuild it from a clean tree:"
		echo "         git stash && make build/$OBJ && make expected && git stash pop"
	fi
	exit 0
fi

make "build/$OBJ" >/dev/null 2>&1 || { make "build/$OBJ" 2>&1 | tail -20; exit 1; }
dis "expected/$OBJ" "$TMP/exp.txt"
dis "build/$OBJ"    "$TMP/mine.txt"

if [ "$MODE" = raw ]; then
	diff -u "$TMP/exp.txt" "$TMP/mine.txt" && echo "MATCH $FN"
	exit 0
fi

norm "$TMP/exp.txt"  > "$TMP/e.txt"
norm "$TMP/mine.txt" > "$TMP/m.txt"

if diff -q "$TMP/exp.txt" "$TMP/mine.txt" >/dev/null; then
	echo "MATCH $FN"
	exit 0
fi

ec=$(grep -c '	' "$TMP/e.txt"); mc=$(grep -c '	' "$TMP/m.txt")
echo "== $FN : target=$ec  mine=$mc =="
echo "-- normalised diff (target < / mine >) --"
diff "$TMP/e.txt" "$TMP/m.txt" || true
echo "-- opcode multiset delta (empty => pure scheduling/allocation miss) --"
# opcode is field 2 when split on tab: <tab>opcode<tab>operands (nop has no operands)
awk -F'\t' 'NF>1{print $2}' "$TMP/e.txt" | sort | uniq -c > "$TMP/eop.txt"
awk -F'\t' 'NF>1{print $2}' "$TMP/m.txt" | sort | uniq -c > "$TMP/mop.txt"
diff "$TMP/eop.txt" "$TMP/mop.txt" || true
