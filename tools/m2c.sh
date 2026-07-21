#!/bin/sh
# Produce a first-draft C decompilation of a function with m2c. The output is
# a starting point for hand-matching, never a match on its own.

set -e

usage() {
	echo "usage: $0 [-c|--context] <asm-file> [<m2c-args>]"
	echo "  --context   : regenerate build/ctx.c before running"
	echo "  <asm-file>  : path to assembly source in asm/*/*matchings"
	exit 1
}

CONTEXT=0

while [ "$#" -gt 0 ]; do
	case "$1" in
		-h|--help)
			usage
			;;
		-c|--context)
			CONTEXT=1
			shift
			;;
		--)
			shift
			break
			;;
		-*)
			echo "Unknown option: $1"
			usage
			;;
		*)
			break
			;;
	esac
done

[ "$#" -lt 1 ] && usage

ASM_FILE="$1"
shift

[ -f "$ASM_FILE" ] || usage

TOP="$(cd -- "$(dirname -- "$(readlink -f -- "$0")")/.." && pwd)"
cd "$TOP"

PYTHON="${PYTHON:-python3}"
CTX="${CTX:-build/ctx.c}"

if [ "$CONTEXT" = 1 ] || [ ! -f "$CTX" ]; then
	echo "Generating $CTX" >&2
	$PYTHON tools/m2ctx.py -o "$CTX" >&2
fi

# The files under asm/*/*matchings are bare function bodies, so give m2c the
# same preamble the assembler gets. Same treatment as tools/permute.sh.
TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT

{
	echo '.set noat'
	echo '.set noreorder'
	echo ''
	echo '.section .text, "ax"'
	echo ''
	cat "$ASM_FILE"
} > "$TMP/input.s"

$PYTHON external/m2c/m2c.py \
	--target mipsel-gcc-c \
	--context "$CTX" \
	"$@" \
	"$TMP/input.s"
