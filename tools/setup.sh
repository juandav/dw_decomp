#!/bin/sh
# Bootstrap the build environment. Safe to re-run; each step is skipped if
# already done. Pass --force to redo the dependency download.

set -e

TOP="$(dirname "$(dirname "$(readlink -f -- "$0")")")"
cd "$TOP"

FORCE=0
[ "$1" = "--force" ] && FORCE=1

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

step 'Manual steps'
STATUS=0

if [ "$HOST_OK" = 0 ]; then
	echo "glibc $GLIBC is too old for the prebuilt tools — use the container"
	echo '  (see Host compatibility above).'
	STATUS=1
fi

if [ ! -f bin/cc_mips/cc_mips_40.dll ]; then
	echo 'MISSING bin/cc_mips/cc_mips_40.dll'
	echo '  Obtain CodeWarrior for PlayStation Release 4 and copy its'
	echo '  cc_mips.dll to that path.'
	STATUS=1
fi

if [ ! -f disks/us/SLUS_010.32 ]; then
	echo 'MISSING disks/us/'
	echo '  Dump an original Digimon World (USA) ISO:'
	echo '  bin/mkpsxiso-2.20-Linux/bin/dumpsxiso -x disks/us \'
	echo '      -s disks/us/us.xml "/path/to/Digimon World (USA).bin"'
	STATUS=1
fi

if [ "$STATUS" = 0 ]; then
	echo 'None outstanding. Next:'
	echo '  make -j$(nproc) regenerate  # disassemble originals'
	echo '  make -j$(nproc) && make compare'
fi

exit "$STATUS"
