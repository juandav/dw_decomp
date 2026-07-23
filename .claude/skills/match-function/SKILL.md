---
name: match-function
description: End-to-end procedure for matching one Digimon World function from C to the target binary, token-efficiently. Load when the task is "match/decompile function X", "continue the decomp", or picking the next function to close. Covers candidate choice, the triage→draft→diff→escalate loop, the expected/ safety guard, and when to stop hand-tuning and hand off to the permuter.
---

# Match one function

Goal: turn an `INCLUDE_ASM(...)` stub into C that assembles to the identical
bytes, keeping `decomp-work` green. Spend tokens on the draft and the idioms,
not on re-reading raw asm or re-deriving the setup.

## 0. Setup (once per session)

```
cd /home/juandav/code/dw_decomp && . .venv/bin/activate
```

Build **one object**, never the project: `make build/src/main/<file>.c.o`.
`CLAUDE.md` has the environment details if something is missing.

## 1. Pick a candidate

```
tools/graphify_asm.py --started --by-size   # biggest leaves in started files
```

Prefer **leaves** (DEPS 0). Small math/geometry functions and any function with
an **already-matched sibling in the same file** are the shortest path. Skip
functions full of GTE macros (`lwc2`, `rtps`, `swc2`) for a quick win — m2c
can't read them.

## 2. Triage before drafting (delegate the asm read)

Spawn the **`decomp-triage`** agent with the function name. It reads the raw
asm (expensive in tokens) in its own context and returns a compact report:
size, calls, frame, opcode-signal flags, difficulty class, and the nearest
matched sibling. Draft from that report — do **not** pull the full `.s` into the
main thread unless triage flags something it couldn't resolve.

Optional: `sh tools/m2c.sh <asm.s>` for a starting skeleton, but only if triage
says it's not GTE and not `goto`-soup.

## 3. Draft

Write the C in place of the `INCLUDE_ASM`. Apply the **`decomp-heuristics`**
skill for every signal triage flagged. Keep the forward-declaration/order rules
in mind (a typeless proto if the struct is defined later in the file).

## 4. Diff loop

```
tools/dwdiff.sh src/main/<file>.c <func>
```

Reads: `target=N mine=M`, the normalised diff, and the **opcode-multiset
delta**. Interpret:

- **MATCH** → go to step 6.
- Multiset delta **non-empty** → wrong instructions: a type, a cast, an
  addressing or control-flow choice. Fixable from source. Apply heuristics,
  re-diff. This is where hand-work pays off.
- Multiset delta **empty**, only order/registers differ → pure
  scheduling/allocation miss. One or two such iterations of source-level
  nudging (declaration order, operand order, doubled assignment) are worth it;
  beyond that, **escalate** (step 5).

## 5. Escalate to the permuter (machine time, not yours)

When the structure is right and only register naming / scheduling remains:

```
sh tools/permute.sh --clean src/main/<file>.c \
    asm/main/nonmatchings/<seg>/<func>.s <func>
nohup python3 external/decomp-permuter/permuter.py -j6 --stop-on-zero \
    permuter/<func>/ > /tmp/perm_<func>.log 2>&1 &
```

- If the base score doesn't move within a few hundred iterations, **kill it** —
  the change it needs is outside its search space; go back to source.
- **Read the best candidate even if it never hits zero** — the trick is often
  in it (`col = (col = y)` doubled assignment on `setRectangleImpassable`).
  Lives in `permuter/<func>/output-<score>-<n>/source.c`.
- Batch several functions and leave them running in the background.

## 6. Land it / park it

**Matched:**
```
make build/src/main/<file>.c.o && make compare   # check build exit code first!
git add -A && git commit    # message = the analysis: what idiom closed it
```

**Structurally right but 1–2 instructions out (a near-miss):** don't wedge it
into `decomp-work` (that breaks green). Revert the file to `INCLUDE_ASM`, save
the draft as a patch, and record the finding. Near-misses live on `wip/`
branches; each carries its analysis in the commit message.

## Traps (cost hours if missed)

- **`expected/` is a snapshot.** `make expected` copies whatever `build/` holds
  *now*. If you ran it after editing, every diff is a false MATCH. When a count
  jumps for no reason, run `tools/dwdiff.sh --verify <file> <func>` — it
  rebuilds HEAD and confirms `expected/` is honest.
- **`make compare` can pass on a broken build** (it aborts before relinking and
  keeps the old binaries). Check the build's exit code.
- After reverting C→`INCLUDE_ASM`, run `make regenerate` before building, or the
  per-function `.s` is missing.
- Run git from **Linux only** (WSL mounts lose the exec bit and show phantom
  mode changes).
