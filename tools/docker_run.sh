#!/bin/sh

# Allocate a tty only when there is one, so this also works from scripts.
if [ -t 0 ]; then
	TTY='-it'
else
	TTY='-i'
fi

docker run $TTY --rm -v "$PWD":/dw -v dw_venv:/dw/.venv dw-build "$@"
