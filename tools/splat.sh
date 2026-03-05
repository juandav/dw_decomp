#!/bin/sh
# See LICENSE file for copyright and license details.

set -e

TOP="$(dirname "$(dirname "$(readlink -f -- "$0")")")"

for f in "$TOP"/config/*.yaml; do
	splat split "$f"
done
