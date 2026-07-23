#!/bin/sh
# check-upstream.sh - is there new work on upstream main to pull in?
#
# Read-only. Fetches upstream/main and reports what it has that the current
# branch (default decomp-work) does not: the new commits, the changed source
# files, and a count of functions upstream newly matched (INCLUDE_ASM removed).
# It never merges - run `git fetch upstream && git merge upstream/main` yourself.
#
# usage:
#   tools/check-upstream.sh              compare decomp-work to upstream/main
#   tools/check-upstream.sh <branch>     compare <branch> instead
#
# Assumes the `upstream` remote points at the original repo (see NOTES.md):
#   git remote add upstream https://github.com/jype0/dw_decomp.git
set -e

ROOT=$(cd -- "$(dirname -- "$(readlink -f -- "$0")")/.." && pwd)
cd "$ROOT"

BRANCH="${1:-decomp-work}"

if ! git remote | grep -qx upstream; then
	echo "no 'upstream' remote. Add it:"
	echo "  git remote add upstream https://github.com/jype0/dw_decomp.git"
	exit 1
fi

echo "Fetching upstream/main ..."
git fetch --quiet upstream main

AHEAD=$(git rev-list --count "$BRANCH..upstream/main")   # upstream has, we don't
BEHIND=$(git rev-list --count "upstream/main..$BRANCH")  # we have, upstream doesn't

echo
echo "== $BRANCH vs upstream/main =="
echo "upstream is $AHEAD commit(s) ahead of $BRANCH; $BRANCH has $BEHIND of its own"

if [ "$AHEAD" -eq 0 ]; then
	echo
	echo "Up to date - nothing new on upstream/main."
	exit 0
fi

echo
echo "-- new upstream commits --"
git log --oneline --no-decorate "$BRANCH..upstream/main"

echo
echo "-- changed source files --"
git diff --stat "$BRANCH...upstream/main" -- 'src/**/*.c' 'include/**' | sed '$d' || true

# Functions upstream newly turned from INCLUDE_ASM into C: an INCLUDE_ASM count
# that dropped between our branch and upstream, per changed file.
echo
echo "-- functions upstream newly matched (INCLUDE_ASM count dropped) --"
FOUND=0
for f in $(git diff --name-only "$BRANCH...upstream/main" -- 'src/**/*.c'); do
	ours=$(git show "$BRANCH:$f" 2>/dev/null | grep -c INCLUDE_ASM || true)
	theirs=$(git show "upstream/main:$f" 2>/dev/null | grep -c INCLUDE_ASM || true)
	ours=${ours:-0}; theirs=${theirs:-0}
	if [ "$theirs" -lt "$ours" ]; then
		echo "  $f: $ours -> $theirs  (+$((ours - theirs)) matched)"
		FOUND=1
	fi
done
[ "$FOUND" -eq 0 ] && echo "  (none - upstream changes are elsewhere)"

echo
echo "To integrate:  git checkout $BRANCH && git merge upstream/main"
echo "Then rebuild:  make -j\$(nproc) && make compare   (check build exit code)"
