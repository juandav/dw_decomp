# CLAUDE.md

Decompilation of **Digimon World (USA)**, PSX, CodeWarrior (`cc_mips_40.dll`)
via metrowrap/wibo. Goal: replace `INCLUDE_ASM(...)` stubs with C that
assembles to identical bytes, keeping `decomp-work` green (`make compare`
matches all 16 binaries).

`NOTES.md` is the durable knowledge base — read it for state, the matching
heuristics in depth, the traps, and where to pick up. This file is the quick
operational map.

## Environment

```
cd /home/juandav/code/dw_decomp && . .venv/bin/activate
```

- Branch to build on: **`decomp-work`** (green). Near-misses go on `wip/`
  branches, each carrying its analysis in the commit message.
- Assets (disc images, DLL) are proprietary and live outside the repo; setup is
  in NOTES.md ("Moving to another machine"). `tools/setup.sh` is idempotent.
- Build **one object**, never the project: `make build/src/main/<file>.c.o`.
  A full `make -j` to check one function is minutes wasted.

## Workflow toolkit

For any "match / decompile / continue the decomp" task, use these — they encode
what this project has already learned so it isn't re-derived:

- **Skill `match-function`** — the end-to-end loop: pick candidate → triage →
  draft → diff → escalate to permuter → land or park. Start here.
- **Agent `decomp-triage`** — reads one function's raw asm in its own context
  and returns a compact report (size, calls, frame, opcode signals, difficulty,
  sibling). Spawn it before drafting so the asm dump never enters the main
  thread.
- **Skill `decomp-heuristics`** — the CodeWarrior-asm→C idiom catalog (signal →
  cause → fix). Load when reading a target or explaining a diff.
- **Skill `quarantine-match`** — when a function byte-matches but breaks
  `make compare` downstream (overlays, symbol shifts), preserve it on a
  blocker-grouped branch instead of discarding it. See `NOTES.md` → Quarantine.
- **`tools/dwdiff.sh <file.c> <func>`** — build one object and diff one function
  against `expected/`: normalised diff, instruction counts, and the opcode
  multiset delta (empty ⇒ pure scheduling/allocation miss ⇒ permuter). Use
  `--verify` to prove `expected/` isn't contaminated.

## Candidate selection

```
tools/graphify_asm.py --started --by-size   # biggest leaves in started files
tools/graphify_asm.py --started             # all gaps in started files
```

Prefer leaves (DEPS 0) and functions with a matched sibling in the same file.

## Escalation

Structure right, only registers/scheduling off → the **permuter**
(`tools/permute.sh` + `external/decomp-permuter/permuter.py`). It's machine
time; batch several and background them. Kill any run whose base score is flat
after a few hundred iterations, and read the best candidate even if it never
hits zero — the idiom is often in it.

## Traps

- `expected/` is a snapshot from the last `make expected`; run it only on a
  clean tree. A count that jumps for no reason ⇒ `tools/dwdiff.sh --verify`.
- `make compare` can pass on a broken build (keeps old binaries) — check the
  build exit code. And a function can `dwdiff` MATCH yet break `make compare`
  (overlay symbol shifts) — always compare all 16 before committing.
- After reverting C→`INCLUDE_ASM`, `make regenerate` before building.
- Run git from Linux only (WSL loses the exec bit).
