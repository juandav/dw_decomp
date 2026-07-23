# Working notes

Personal notes for this fork: how to stand the project up somewhere else, what
was learned matching functions, and where to pick up. Kept in English to match
the rest of the repository.

## State

`decomp-work` is the branch to build on. It is green - `make compare` matches
all 16 binaries - and merged with upstream `main` as of 2026-07-22, through
`7a87687 "Match more inventory.c functions"`. Upstream's item.c and
inventory.c work brought the count to 656.

```
decomp-work            green, 658/3003 functions, 15.7635% of code
wip/view               view_init, one instruction
wip/tournament         initTournamentInfo, one instruction
wip/drafts             checkMapCollisionX, modifySomeImage, popAttackObject,
                       initializeClockData, checkEatDistance
wip/particle-flash     renderParticleFlash, full draft, 985/851 instructions
wip/rotate-vector      rotateVector, 48/50 instructions, EFE_DATA_STACK pop
wip/intToStringSJIS    intToStringSJIS, 55/56, loop body exact, reg-alloc
wip/overlay-blocked    getEntityFromScriptId, matches but breaks overlays
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
says. `popAttackObject` is the closest of the three - every instruction bar
the first loop, where three registers are rotated against the target.
`initializeClockData` matches to the opcode - only the live range of the
constant `8`, shared between `HOUR` and the sprite width, is scheduled
differently; the colour-store half fell to the permuter's `g = (r = 0x80)`.
`checkEatDistance` also matches to the opcode: the argument register frees up
and the target reuses it for a pointer, giving a register rotation this build
cannot reproduce without breaking the load scheduling instead.

Three of these five - `initializeClockData`, `checkEatDistance`, and
`popAttackObject` - match to the opcode multiset and differ only in register
naming or one value's live range. That is the wall this stretch of the worklist
keeps hitting: the structure is easy, the register allocation is not, and the
permuter's source-level randomisation reaches it only slowly. The ones that did
close (`getPartnerTamerCloseness`, `removeItem`) turned on a specific idiom -
squaring each value the moment it is computed, deriving a pointer from the
index rather than walking it - not on grinding the allocation.

`rotateVector` (efe.c) is the first function to pop the EFE_DATA_STACK, the
script VM's argument stack. It is structurally exact - the RotMatrixZYX /
ApplyMatrixSV body matches `initializeParticleFX` - and 48/50 instructions. The
two-instruction gap is the per-pop reload: the target loads the global once per
pop and reuses the register for the deref; every source form either reloads the
deref too (`char*` direct, +1) or CSE-folds the second pop's read (temp
pointer, -2). `char* -= 4` also emits trapping `addi`; scaled arithmetic
`p = (char*)((int32_t*)EFE_DATA_STACK - 1)` gives the target's `addiu`. The
permuter stalled at base score 260. Draft saved on `wip/rotate-vector`.

`intToStringSJIS` (script_common.c, int -> full-width SJIS digits) is 55/56 with
the loop body matching instruction-for-instruction. Five idioms closed the
structure: the local array initializer `int32_t divisors[6] = {1,10,...}` is the
`MAIN_D_80130344` copy; `uint8_t digits` gives the `addiu`+`andi` decrement;
`base = (base = 0x824F)` hoists the SJIS '0' into a register (a plain constant
splits into `addi 592`+`addi 32767`); recomputing `i = digits - 1` at the loop
top kills a strength-reduced pointer walk; and `goto` mirrors the leading-zero
suppression. The last instruction is register allocation - the target reuses
ch's register for the output pointer and copies ch first; permuter plateaus at
435. Draft on `wip/intToStringSJIS`.

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

## Tooling (for the AI workflow)

Encoded so the loop isn't re-derived each session - see `CLAUDE.md` for the map:

- **`tools/dwdiff.sh <file.c> <func>`** - build one object, diff one function
  vs `expected/`: normalised diff + instruction counts + opcode multiset delta
  (empty delta ⇒ pure scheduling/allocation miss ⇒ permuter). `--verify` proves
  `expected/` isn't contaminated; `--raw` keeps offsets.
- **`tools/check-upstream.sh`** - fetch upstream/main and report new commits,
  changed source, and functions upstream newly matched. Read-only; never merges.
- **Skill `match-function`** - the end-to-end procedure (candidate → triage →
  draft → diff → escalate → land/park).
- **Skill `quarantine-match`** - when a function byte-matches but breaks
  `make compare` downstream, preserve it on a blocker-grouped branch instead of
  discarding it, keep decomp-work green, and register the resume condition.
- **Skill `decomp-heuristics`** - the signal→cause→fix idiom catalog distilled
  from the section below.
- **Agent `decomp-triage`** - reads one function's raw asm in its own context
  and returns a compact report, keeping the disassembly out of the main thread.

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
- The same rule runs backwards, and that direction comes up more often. `$gp`
  where the target uses `lui`/`addiu` means the declaration is *too* precise:
  a complete type under eight bytes makes the compiler assume `.sdata`. Take
  the size away - `extern int8_t X[]`, read as `X[0]` - and it addresses the
  symbol absolutely. Anything in `.bss` needs this; `PARTNER_AREA_RESPONSE`
  sits at 0x80150C3E, which is not within reach of `$gp` at all.
- A `b` into the middle of a loop, with the increment at the top of the body,
  is `for (i = 0; cond; i++)`. Test first and increment at the bottom, with no
  branch to enter, is `for (i = 0;; i++)` with the exit written as an `if` and
  a `break` inside.
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
- The compiler squares (or otherwise consumes) a value the moment it has it, so
  feed it the values in the order it wants them used. Two coordinate
  differences left inline in a distance expression are subtracted-and-squared
  one at a time; lifted into `dx`/`dz` locals first they are both subtracted
  and then both squared. And two thresholds compared later still want their
  squares materialised in source order now - `getPartnerTamerCloseness`
  needed `inner2 = inner * inner` right after `inner`, before the `outer`
  the first comparison actually uses.
- Reaching a parallel array through the wrong base costs a symbol load. Three
  consecutive tables (types, amounts, names, 30 bytes each) - the target
  reads the amount as `&types[i] + 30`, not `amounts[i]`. Materialise the
  pointer from the index inside the match block and deref the local; a pointer
  walked alongside `i` as `p[30]` gets strength-reduced into a second
  induction variable instead. This closed `removeItem`.
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
- `expected/` is a snapshot, not a build product: `make expected` copies
  whatever `build/` holds *now*. Run it only while the working tree is clean -
  captured after editing, every diff against it reports a false MATCH, and
  the mistake surfaces later as an inexplicable regression. When a diff
  count jumps for no reason, suspect the snapshot before the source.
  `tools/dwdiff.sh --verify <file> <func>` checks it for you.
- A function can `dwdiff` MATCH and still break `make compare`. Functions
  referenced by overlay data tables shift the overlay `_REL.BIN` symbol
  resolution when converted (see Quarantine). Always `make compare` all 16
  before committing, and check the build's exit code - a link error leaves the
  old green binaries in `build/` and compare passes on stale output.

## Quarantine (matches blocked downstream)

Byte-exact matches that can't land on `decomp-work` because converting them
regresses something the function itself doesn't touch. Preserved, not discarded;
see the `quarantine-match` skill. Revisit when the blocker column clears.

| branch | function | blocker | resume when |
|---|---|---|---|
| `wip/overlay-blocked` | `getEntityFromScriptId` (tamer.c) | overlay refs to `ENTITY_TABLE`/`startAnimation` resolve 8 bytes low in `TRN_REL`/`VS_REL` when it's C; main stays green | the overlay reloc-pairing mechanism is understood |

The risk class is **functions referenced by overlay data tables**. Before
converting a `main` function, check `grep -rln <func> asm/{vs,trn,trn2,btl,std}`
and run `make compare` (all 16), not just `dwdiff`, before committing. dwdiff
MATCH is necessary but not sufficient.

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

`renderParticleFlash` (3400 bytes, the largest single jump) is reverse-
engineered in full on `wip/particle-flash` - a complete, compiling draft at
975 instructions to the target's 850. The structure is done and confirmed;
what remains is matching the target's register economy so params fields stop
reloading. Resume from that branch, not from m2c. The four quadrant bodies are
near-identical, so a fix found in one carries to the other three.
