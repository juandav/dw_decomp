# Digimon World decomp

A work in progress decompilation of Digimon World for PS1.

## Dependencies

```
tools/setup.sh
. .venv/bin/activate
```

This installs the system packages, checks out the submodules, creates the
virtualenv and downloads the prebuilt tools. It is safe to re-run and reports
whatever is still outstanding.

The prebuilt metrowrap and mkpsxiso binaries need glibc 2.39 or newer, which is
what the Dockerfile (ubuntu:noble) provides. On an older host, setup.sh says so
and you build in the container instead:

```
tools/docker_build.sh
tools/docker_run.sh make -j$(nproc)
```

Prefix any command in the Build section below with tools/docker_run.sh to run
it there. Every command is otherwise identical.

Two things it cannot fetch for you:

- CodeWarrior for PlayStation Release 4 — copy its cc_mips.dll to
  bin/cc_mips/cc_mips_40.dll.
- An original Digimon World (USA) ISO, dumped to disks/us (see Build below).

## Build

```
# Dump original PSX Digimon World (USA) ISO
bin/mkpsxiso-2.20-Linux/bin/dumpsxiso -x disks/us -s disks/us/us.xml "/path/to/Digimon World (USA).bin"

# Disassemble original binaries
make -j$(nproc) regenerate

# (Optional) Create file local.mk to override defaults
MWCCWRAP := /path/to/mwccwrap.exe
MWCCWRAP_FLAGS := -dll "/path/to/cc_mips.dll"
METROWRAP := /path/to/mw
METROWRAP_FLAGS := --use-wibo --wibo-path /path/to/wibo
TOOLCHAIN := /path/to/mipsel-linux-gnu-

# Build new binaries
make -j$(nproc)

# Compare original vs new binaries
make compare

# Generate objdiff config
make objdiff
```

## Decompiling a function

To pick one, ask for the worklist. It ranks what is still in asm by how little
it depends on other undecompiled functions, then by size — leaves match
fastest:

```
tools/graphify_asm.py
```

The same script merges the disassembly into the graphify knowledge graph, so
queries cover the whole binary and not just the parts already in C. The graph
lives in graphify-out/, which is not tracked - it is 22 MB of derived data that
rewrites itself on every run. Rebuild it in three steps, at no API cost:

```
graphify update .                     # re-extract the C (AST only)
tools/graphify_asm.py --merge         # add the disassembly
graphify cluster-only .               # communities, report, HTML
```

Get a first draft from the disassembly with m2c, then hand-match it against
objdiff. The draft compiles but will not match — it is a reading aid.

```
# Generate the C context (build/ctx.c) used by m2c and decomp.me
make ctx

# First draft for one function
tools/m2c.sh asm/main/nonmatchings/main/someFunction.s
```

Once a candidate is close, let decomp-permuter search for the last few
instruction differences:

```
tools/permute.sh --run src/main/main.c \
    asm/main/nonmatchings/main/someFunction.s someFunction
```

## Links

Symbols and reverse engineering is based on work by SydMontague:  
https://github.com/SydMontague/DW1-SydPatches  
https://github.com/SydMontague/DW1-Code
