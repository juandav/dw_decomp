#!/bin/sh
# Bootstrap everything a fresh clone needs. Safe to re-run; each step is
# skipped if already done.
#
#   --force        redo the dependency download
#   --no-graphify  skip the knowledge graph tooling

set -e

TOP="$(dirname "$(dirname "$(readlink -f -- "$0")")")"
cd "$TOP"

FORCE=0
GRAPHIFY=1
for arg in "$@"; do
	case "$arg" in
		--force) FORCE=1 ;;
		--no-graphify) GRAPHIFY=0 ;;
		*) echo "unknown option: $arg" >&2; exit 1 ;;
	esac
done

APT_PKGS='binutils-mipsel-linux-gnu gcc-mipsel-linux-gnu git make python3 python3-venv unzip wget'

step() {
	printf '\n==> %s\n' "$1"
}

# The prebuilt metrowrap and mkpsxiso binaries are built against the glibc in
# ubuntu:noble, which is also what the Dockerfile uses.
GLIBC_MIN='2.39'
HOST_OK=1

step 'Host compatibility'
GLIBC="$(ldd --version | head -1 | awk '{print $NF}')"
if [ "$(printf '%s\n%s\n' "$GLIBC_MIN" "$GLIBC" | sort -V | head -1)" = "$GLIBC_MIN" ]; then
	echo "glibc $GLIBC — OK."
else
	HOST_OK=0
	echo "glibc $GLIBC is older than $GLIBC_MIN."
	echo 'metrowrap and mkpsxiso will not run directly on this host.'
	echo 'The rest of this script still applies, but build in the container:'
	echo '  tools/docker_build.sh'
	echo '  tools/docker_run.sh make -j$(nproc)'
fi

step 'System packages'
MISSING=''
for pkg in $APT_PKGS; do
	dpkg -s "$pkg" >/dev/null 2>&1 || MISSING="$MISSING $pkg"
done
if [ -n "$MISSING" ]; then
	echo "Installing:$MISSING"
	sudo apt-get update
	# shellcheck disable=SC2086
	sudo apt-get install -y $MISSING
else
	echo 'All present.'
fi

step 'Submodules'
git submodule update --init --recursive

step 'Python virtualenv'
[ -d .venv ] || python3 -m venv .venv
.venv/bin/pip install -q --upgrade pip
.venv/bin/pip install -q -r requirements.txt
echo 'Ready: . .venv/bin/activate'

step 'Downloaded tools'
if [ "$FORCE" = 1 ] || [ ! -d bin/metrowrap ]; then
	tools/dl_deps.sh
else
	echo 'Already downloaded (--force to redo).'
fi

step 'Knowledge graph tooling'
if [ "$GRAPHIFY" = 0 ]; then
	echo 'Skipped (--no-graphify).'
elif .venv/bin/python -c 'import graphify' 2>/dev/null; then
	echo 'Already installed.'
else
	# In the venv rather than user-wide, so tools/graphify_asm.py and the
	# graphify CLI agree on an interpreter.
	.venv/bin/pip install -q graphifyy
	echo 'Installed. Rebuild the graph with:'
	echo '  .venv/bin/graphify update .'
	echo '  tools/graphify_asm.py --merge'
	echo '  .venv/bin/graphify cluster-only .'
fi

# The prebuilt tools need a newer glibc than this host has, so everything gets
# built inside the container instead. Have it ready.
if [ "$HOST_OK" = 0 ]; then
	step 'Container image'
	if ! command -v docker >/dev/null 2>&1; then
		echo 'docker not installed - it is the only way to build on this host.'
	elif ! docker info >/dev/null 2>&1; then
		echo 'docker is installed but the daemon is not reachable.'
	elif [ -n "$(docker images -q dw-build:latest 2>/dev/null)" ]; then
		echo 'dw-build:latest already built.'
	else
		tools/docker_build.sh
	fi
fi

# Prefix for anything that has to touch the prebuilt tools.
if [ "$HOST_OK" = 0 ]; then
	RUN='tools/docker_run.sh '
else
	RUN=''
fi

step 'What is left'
STATUS=0

if [ ! -f bin/cc_mips/cc_mips_40.dll ]; then
	echo 'MISSING bin/cc_mips/cc_mips_40.dll'
	echo '  Obtain CodeWarrior for PlayStation Release 4 and copy its'
	echo '  cc_mips.dll to that path. It is proprietary, so nothing here'
	echo '  can fetch it for you.'
	STATUS=1
fi

if [ ! -f disks/us/SLUS_010.32 ]; then
	echo 'MISSING disks/us/'
	echo '  Dump an original Digimon World (USA) disc, then extract it:'
	echo "  ${RUN}bin/mkpsxiso-2.20-Linux/bin/dumpsxiso -x disks/us \\"
	echo '      -s disks/us/us.xml "/path/to/Digimon World (USA).bin"'
	STATUS=1
fi

if [ "$STATUS" = 0 ]; then
	echo 'Nothing. Build with:'
	echo "  ${RUN}make -j\$(nproc) regenerate   # disassemble the originals"
	echo "  ${RUN}make -j\$(nproc)"
	echo "  ${RUN}make compare                  # must say binaries match"
else
	echo ''
	echo 'Everything else is ready. Re-run this script once those are in place.'
fi

exit "$STATUS"
