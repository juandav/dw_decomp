# Working notes

Personal notes for this fork: how to stand the project up somewhere else, what
was learned matching functions, and where to pick up. Kept in English to match
the rest of the repository.

## State

`decomp-work` is the branch to build on. It is green - `make compare` matches
all 16 binaries - and merged with upstream `main` as of 2026-07-21, through
`9a148de "Partially match script*.c"`. That merge alone took the count from
432 to 643: eleven new `script_*.c` files and a `script` rodata subsegment.

```
decomp-work            green, 649/3003 functions, 15.4605% of code
wip/view               view_init, one instruction
wip/tournament         initTournamentInfo, one instruction
wip/drafts             checkMapCollisionX, modifySomeImage
wip/three-near-misses  superseded, see below
main, pr16             historical reference
```

`wip/efedat` and `wip/collision` are merged. `matrixToEuler2` came off
`wip/three-near-misses` and is merged too, so what is left of that branch is
the `initTournamentInfo` and `renderTournamentSchedule` drafts. Both were
written before upstream renamed the `TextBoxData` fields off their `unk_`
names, so neither compiles as-is; `wip/tournament` carries the rebased
`initTournamentInfo`.

Two are one instruction away, and in both cases it is a slot, not a value:

- `view_init` - the target hoists the `lui` for the `&GS_VIEWPOINT` argument
  to `GsSetRefView2` up to just after the first store, and this build sinks it
  to the call. Taking the address into a local early does not move it; the
  compiler sinks that too.
- `initTournamentInfo` - the target fills the load delay after
  `lbu $v0, %gp_rel(TOURNAMENT_SELECTED_ROW)` with the `sra` that finishes
  `posX`, this build puts `posY`'s there. Both are legal. Nothing in the
  source moves it.

`renderTournamentSchedule` is further out than its 209/216 suggested: the
target holds one more value in a callee-saved register - it saves `s8` and
this build does not - so the whole allocation is shifted, not just scheduled.

`wip/drafts` holds two that are structurally right and a dozen instructions
out. `checkMapCollisionX` needs three live copies in the prologue that no
arrangement of the doubled-assignment trick reproduces - and closing it also
closes `checkMapCollisionY`, the same function on the other axis.
`modifySomeImage` needs all four channel extractions hoisted above the first
multiply; the compiler sinks each one back to its use whatever the source
says.

Every `wip/` branch carries its analysis in the commit message. They are all
one or two instructions short, which is register allocation and scheduling
rather than structure.

## Remotes

The first machine had these reversed - `origin` was `jype0` because it was
already configured, and the fork was `fork`. A fresh clone names the fork
`origin`, which is the usual convention, so keep it and add the original as
`upstream`:

```
origin    git@github.com:juandav/dw_decomp.git      ours, tracked by decomp-work
upstream  https://github.com/jype0/dw_decomp.git    the original, for git fetch
```

A bare `git push` goes to the fork. Pull upstream work with
`git fetch upstream && git merge upstream/main`.

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

# Cloning names the fork "origin", which is what we want. Add the original.
git remote add upstream https://github.com/jype0/dw_decomp.git

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

`tools/setup.sh` does the lot: system packages, submodules, the virtualenv,
the tool downloads, graphify, and the container image when the host glibc is
too old for the prebuilt binaries. It is idempotent, so re-run it after
dropping the DLL and the disc dump in place and it will tell you the build
commands - prefixed with `tools/docker_run.sh` if this host needs the
container. `--no-graphify` skips the graph tooling.

The only things it cannot do are the two proprietary ones: the compiler DLL
and the disc dump.

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
- Read the permuter's best candidate even when it never reaches zero. On
  `setRectangleImpassable` it stalled at 10 from a base of 115, but the
  candidate contained `col = (col = y)` - the doubled assignment that survives
  folding - and that was the whole trick. Its output lives in
  `permuter/<function>/output-<score>-<n>/source.c`.
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
- `addu $r, $r, $zero` after a `lui`/`addiu` pair is an *index*, not a store
  through a pointer. The compiler proved the index was zero, kept it in
  `$zero` and still emitted the addition, so write `X[i]` with `i = 0` on the
  line above. Reaching for a pointer local instead - plain, `volatile` or
  self-assigned - gets folded straight back to a direct store through `$at`.
  This closed `findEFEDATFile`.
- A redundant `move` of a parameter into another register means the source
  read that parameter through a local, and the local survived. `rc = r` is
  folded straight back to the parameter; `rc = (rc = r)` survives. Then check
  which expressions read the copy and which read the parameter - in
  `setRectangleImpassable` the row start and column end read the parameters
  and the other two bounds read the copies.
- The declaration order of locals decides which registers the loop variables
  get. Swapping `row` and `col` was what moved them into `t0` and `a3`.
- Write `MAX` and `ABS` with `>`, not `<`. `((a) > (b)) ? (a) : (b)` and
  `((x) > 0) ? (x) : -(x)` are what CodeWarrior compiled here; the `<` forms
  are the same functions but emit the `slt` operands the other way round and
  branch `bgez` where the target branches `blez`. `dw/math.h`'s `ABS` is the
  `<` form, so a local one is needed. That was the last 21 diff lines of
  `matrixToEuler2`, a 206-instruction function that was otherwise correct.
- A load into one register and the mask into another - `lbu $a1, 0($v0)` then
  `andi $v0, $a1, 0x3f` - means the load and the mask were separate statements.
  `x = p[i] & 0x3F` compiles to a single register.
- Where a comparison names the same two values either way round, the operand
  order in the source decides which value lands in which register. `HOUR <= X`
  and `X >= HOUR` are the same `slt` with its inputs swapped. When the values
  are right but the registers are crossed, try the other form before anything
  else - it closed both the loop test and the tail of `setFoodTimer`.
- Reading a global twice makes the compiler copy it into a second register for
  the second read. If the target tests the same register both times, the
  source had a local.
- Before starting a function, look for one already matched that does the same
  thing. `MAIN_func_800E4470` is `matrixToEuler2` on different matrix
  elements, and went in almost as written once the macros were there.
  `renderPressStartToContinue` is `renderMainMenuBackground` with a third
  quad, and `applyRootMomentum` steps the same counters as `applyMomentum`.
- Declaration order sets the frame layout as well as the registers. Locals are
  laid out in the order they are declared, so if every stack offset is out by
  the size of one local, reorder the declarations rather than hunting the
  code.
- The third clause of a `for` runs before anything the body's tail does. If
  the target bumps the loop counter and then the pointers, they were all in
  the `for` statement: `for (i = 0; i < 3; i++, p++, q++)`.
- Inside a branch the compiler knows the index, write the constant. `root[0]`
  where `i` is provably 0 is one instruction; `root[i]` is two.
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

## The `sdata` audit is done

It was worth less than it looked. The 22 `sdata` subsegments hold 643 symbols,
but only 31 are declared in C at all - the rest are referenced solely from
functions still in asm. Of those 31, exactly two were incomplete arrays, both
in `efe.c`: `MAIN_D_80134220`, which closed `findEFEDATFile`, and
`MAIN_D_8013421C`, now `[4]`, which `renderEFEFlash` reaches through `$gp` and
will need when that function is written.

So there is no backlog of missing sizes waiting to unblock the near misses.
The check is cheap to repeat as more of each file lands - collect the symbols
from `asm/main/data/*.sdata.s` and grep the C for `extern` declarations of them
that end in `[]`.

## Next

The worklist:

```
tools/graphify_asm.py --started            # gaps in files already started
tools/graphify_asm.py --started --by-size  # biggest leaves, for a percentage
```

`renderParticleFlash` is 3400 bytes, the largest single jump available.
