# Working notes

Personal notes for this fork: how to stand the project up somewhere else, what
was learned matching functions, and where to pick up. Kept in English to match
the rest of the repository.

## State

`decomp-work` is the branch to build on. It is green - `make compare` matches
all 16 binaries - and merged with upstream `main` as of 2026-07-21.

```
decomp-work            green, 431/3004 functions, 9.0131% of code
wip/efedat             findEFEDATFile, 37 of 39 instructions
wip/collision          setRectangleImpassable, 30 of 32
wip/view               view_init, 36 of 36
wip/three-near-misses  initTournamentInfo, matrixToEuler2, renderTournamentSchedule
main, pr16             historical reference
```

Every `wip/` branch carries its analysis in the commit message. They are all
one or two instructions short, which is register allocation and scheduling
rather than structure.

## Remotes

```
origin  git@github.com:jype0/dw_decomp.git      upstream, for git fetch
fork    git@github.com:juandav/dw_decomp.git    ours
```

This is the reverse of the usual convention, where `origin` is your fork and
`upstream` is the original. It is that way round because `origin` was already
configured; `git remote rename origin upstream && git remote rename fork origin`
swaps it if you prefer.

`decomp-work` tracks `fork/decomp-work`, so a bare `git push` goes to the fork.
Everything except `pr16` is pushed - that one is a local copy of someone else's
PR branch and is already merged upstream.

## Moving to another machine

The history is on the fork, so this is a clone rather than a copy. Two things
still have to travel by hand, and neither is in the repository:

| what | size |
|---|---:|
| the disc images | 620 MB |
| `cc_mips_40.dll` | 744 KB |

Everything else rebuilds: `disks/` from `dumpsxiso`, `bin/` from
`tools/dl_deps.sh`, `external/` from the submodules, `asm/` and `build/` from
`make regenerate`, `graphify-out/` from graphify.

### Distribution

Use Ubuntu 24.04 (noble). The prebuilt `metrowrap` and `mkpsxiso` need glibc
2.39, which is what the Dockerfile provides and what noble ships. On anything
older - Ubuntu 22.04 has 2.35 - every command has to go through
`tools/docker_run.sh`. Building natively is the reason to bother with a
dedicated install at all.

### Setup

```
git clone git@github.com:juandav/dw_decomp.git && cd dw_decomp

# Cloning names the fork "origin". Reproduce the layout above so the remote
# names mean the same thing on every machine.
git remote rename origin fork
git remote add origin git@github.com:jype0/dw_decomp.git

git checkout decomp-work

tools/setup.sh              # packages, submodules, venv, downloads
. .venv/bin/activate

mkdir -p bin/cc_mips
cp /path/to/cc_mips.dll bin/cc_mips/cc_mips_40.dll

bin/mkpsxiso-2.20-Linux/bin/dumpsxiso -x disks/us -s disks/us/us.xml \
    "/path/to/Digimon World (USA).bin"

make -j$(nproc) regenerate
make -j$(nproc) && make compare
```

`tools/setup.sh` reports whatever is still missing and says so if the host
glibc is too old.

## Working faster

- Build one object, not the project: `make build/src/main/efe.c.o`. A full
  `make -j12` to check one function is minutes wasted.
- Use the objdiff GUI (`./bin/objdiff-linux-x86_64`). It rebuilds on save and
  shows the aligned diff with a match percentage; hand-rolled
  `objdump | sed | diff` is slower and easier to misread.
- If decomp-permuter does not move off its base score within a few hundred
  iterations, kill it. A flat base score means the change it needs is outside
  its search space, so go back to the source instead of waiting.
- Batch the permuter across many functions and leave it running. It is machine
  time, not yours.
- Check the m2c draft before committing to a function. If it comes back full
  of `M2C_ERROR` or `goto`, it will cost several times as much. GTE code
  (`lwc2`, `rtps`, `swc2`) is written with psyq macros and m2c cannot read it.

## Matching, what the disassembly is telling you

Every function closed so far was structurally correct on the first draft. The
gap was always types or ordering.

- `sll`/`sra` by 16 around an intermediate means the variable was a `short`,
  not an `int`.
- `add`, `addi`, `sub` - the trapping forms - mean pointer arithmetic.
  `addu`, `addiu`, `subu` mean integer arithmetic.
- A non-void return type reserves `v0` and shifts the whole allocation. If the
  target's register use looks shifted by one, try returning something.
- Branching *to* the case bodies with the bodies laid out after the tests is a
  `switch`, not an if chain. CodeWarrior emits the bodies in source order and
  the tests in reverse, so if it comes out mirrored, reverse the cases.
- `lui`/`addiu` where the target uses `$gp` means the symbol lives in `.sdata`
  and is declared `extern type X[]` with no size. The compiler cannot tell it
  fits under `-sdata 8`. Give it the size.
- Writing `0x80` into an `int8_t` array yields `li -128`. Cast through
  `uint8_t` the way `getTileTrigger` already does.
- Look for the idiom elsewhere in the same file before inventing one. The
  `&TOURNAMENT_ARRAY[x * 6]` pointer form was two functions up.

### `Handwritten function` is noise here

spimdisasm marks 2100 of 2741 functions - 77% of the binary - as handwritten.
It is a false positive: the flagged instructions are only `addi`, `add` and
`sub`, the trapping arithmetic CodeWarrior emits and GCC and IDO never do. The
heuristic was tuned for N64. `setPosDataPolyFT4` carries the flag and matched
from C on the first try.

## Traps

- `make compare` can pass with a broken build. If `make` failed, it aborts
  before relinking and `build/` still holds the previous good binaries. Check
  the build's exit code before trusting compare.
- After reverting a decompiled function back to `INCLUDE_ASM`, run
  `make regenerate` before building. splat only emits the per-function `.s`
  for functions that are not in C, so reverting leaves an undefined reference.
- Run git from Linux only. Over `\\wsl.localhost` from Windows it cannot see
  the executable bits and reports spurious mode changes on every script.

## Next

Audit the `sdata` declarations. `config/main.yaml` defines `sdata` subsegments
for model, file_table, partner_impl, tamer, efe, overworld, ui, item and sound.
Cross-checking those symbols against their `extern` declarations in the `.c`
files and filling in the sizes is mechanical work that can unblock several
functions at once - possibly some of the near misses above, where the permuter
never moved because the difference is in a declaration rather than in the body.

After that, the worklist:

```
tools/graphify_asm.py --started            # gaps in files already started
tools/graphify_asm.py --started --by-size  # biggest leaves, for a percentage
```

`renderParticleFlash` is 3400 bytes, the largest single jump available.
