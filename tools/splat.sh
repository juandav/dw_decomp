#!/bin/sh

set -e

TOP="$(dirname "$(dirname "$(readlink -f -- "$0")")")"

rm -rf -- "$TOP/asm"

for f in "$TOP"/config/*.yaml; do
	splat split "$f" --disassemble-all --make-full-disasm-for-code
done
