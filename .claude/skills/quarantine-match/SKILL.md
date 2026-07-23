---
name: quarantine-match
description: Preserve a correct-but-unlandable function match instead of discarding it. Load when a function byte-matches (dwdiff MATCH / --verify OK) but breaks make compare downstream (overlays, symbol shifts), or is a structurally-correct near-miss that can't close yet. Moves it to a blocker-grouped quarantine branch with full analysis, keeps decomp-work green, registers it, and defines the resume condition so it is revisited - not lost.
---

# Quarantine a blocked match

A function that byte-matches in isolation but can't land on `decomp-work` is not
a failure to throw away - it is finished work waiting on a blocker. Quarantine it
so the match is preserved, `decomp-work` stays green, and it gets revisited when
the blocker clears. Never revert-and-discard a correct match.

## When to quarantine (vs. land vs. park)

- **Land** on decomp-work: `tools/dwdiff.sh` MATCH **and** `make compare` green.
- **Quarantine** (this skill): the function byte-matches (dwdiff MATCH,
  `--verify` OK) **but** `make compare` regresses something downstream - an
  overlay `_REL.BIN`, a symbol shift, a link error. The C is correct; the build
  can't take it yet.
- **Park** as a near-miss (`wip/<topic>`): structurally right but a few
  instructions out on register allocation/scheduling - not byte-exact. That is
  the `match-function` escalation path, not quarantine.

The distinguishing test: **run `make compare` (all 16 binaries), not just
dwdiff.** dwdiff MATCH is necessary, not sufficient. If compare regresses a
binary you did not touch, you have a quarantine candidate.

## Quarantine procedure

1. **Confirm it's a clean blocker, not a mistake.** Restore green
   (`git checkout <file> && make regenerate && make && make compare`), then
   re-apply *only* this function and rebuild **without** `make regenerate`. If
   compare still regresses, the block is real. (A stray edit or a stale
   regenerate can fake it - see the traps below.)

2. **Identify the blocker and group by it.** Functions with the same blocker
   share a branch so they unblock together. Current groups:
   - `wip/overlay-blocked` - matches that shift overlay (`TRN/VS/TRN2_REL`)
     symbol resolution. Risk class: functions referenced by overlay data tables.
     Detect with `grep -rln <func> asm/{vs,trn,trn2,btl,std}`.
   Use `wip/<blocker>` for a new blocker class; `wip/quarantine-<func>` only if
   the blocker is unique to one function.

3. **Commit the match with the full analysis** on the quarantine branch:
   ```
   git checkout -b wip/<blocker>          # or checkout the existing group branch
   git add <file> && git commit
   ```
   The message must carry: the winning idiom (so the match isn't re-derived),
   the exact compare regression (which binary, which bytes, which symbols), what
   you ruled out, and the **resume condition** - the concrete thing that must be
   true for it to land.

4. **Return decomp-work to green** and confirm:
   ```
   git checkout decomp-work && make -j$(nproc) && make compare
   ```

5. **Register it** so it isn't forgotten: add a line to the `## Quarantine`
   section of `NOTES.md` - branch, function, one-line blocker, resume condition.

## Resume procedure (revisit)

Run when the blocker may have cleared (e.g. the overlay reloc mechanism is now
understood, or a prerequisite function landed):

1. `git checkout wip/<blocker> && git rebase decomp-work` (pick up new work).
2. Rebuild and `make compare`. If green now, the blocker is gone - fast-forward
   the match onto decomp-work, commit, and drop it from the NOTES quarantine
   list.
3. If still blocked, note what you learned on the branch and leave it quarantined.

Check the register each session; a quarantined match is the cheapest possible
win once its blocker lifts - the hard part is already done.

## Traps

- `make regenerate` emits per-function `.s` only for functions **not** in C. If
  you regenerate while the function (or any function) is C, its `.s` vanishes and
  reverting leaves an undefined-reference link error. After quarantining, if the
  build breaks with `referencia a X sin definir`, run `make regenerate` on the
  clean tree to re-emit the stub.
- Don't trust a compare result taken while an unrelated edit is in the tree
  (`git status` first). A stray patch on another file will masquerade as this
  function's blocker.
