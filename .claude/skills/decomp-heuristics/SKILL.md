---
name: decomp-heuristics
description: CodeWarrior MIPS disassembly→C idiom catalog for the Digimon World decomp. Load when reading an asm target or explaining why a build diff differs, to map a signal (addi vs addiu, sll/sra 0x10, $gp vs lui, redundant move, b into loop) to its C cause and fix without re-deriving it. Recall over reasoning.
---

# Reading CodeWarrior asm → C

Every function closed here was **structurally correct on the first draft**; the
gap was always types, ordering, or register economy. Match the signal, apply
the cause. War stories and the full list live in `NOTES.md` ("Matching, what
the disassembly is telling you") — this is the fast lookup.

## Types & casts

| Signal in the target | C cause | Fix |
|---|---|---|
| `sll`/`sra` by `0x10` around a value | the variable is `short`, not `int` | declare `int16_t` |
| `add` / `addi` / `sub` (trapping) | **pointer** arithmetic | pointer/`+=` on a typed ptr |
| `addu` / `addiu` / `subu` | **integer** arithmetic | plain int math |
| `lbu`/`lhu` (unsigned load) + `andi` in a 2nd reg | load and mask were **separate statements** | fold: `x = p[i] & 0x3F` in one |
| `li -128` storing into a byte array | `0x80` into `int8_t` | cast through `uint8_t` (see `getTileTrigger`) |

## Addressing (.sdata vs .bss — runs both ways)

- `lui`/`addiu` where target uses **`$gp`** → symbol is in `.sdata`; declared
  `extern T X[]` with **no size**. Give it the size so it fits under `-sdata 8`.
- **`$gp`** where target uses `lui`/`addiu` → declaration is **too precise**; a
  complete type ≤8 bytes makes CW assume `.sdata`. Remove the size
  (`extern int8_t X[]`, read `X[0]`). Anything in `.bss` needs this.

## Control flow

- Branch **to** case bodies, bodies after the tests → `switch` (CW lays bodies
  in source order, tests reversed). If mirrored, reverse the cases.
- `b` into the middle of a loop, increment at top of body → `for(i=0; cond; i++)`.
  Test-first + increment-at-bottom, no entry branch → `for(i=0;;i++)` with the
  exit as `if(...) break;`.
- Write `MAX`/`ABS` with `>`, not `<`: `((a)>(b))?(a):(b)`, `((x)>0)?(x):-(x)`.
  The `<` forms swap the `slt` operands and branch the other way. `dw/math.h`'s
  `ABS` is the `<` form — write a local one.
- Comparison naming the same two values either way (`HOUR<=X` vs `X>=HOUR`) is
  the same `slt` with inputs swapped. Registers crossed but values right → try
  the other operand order first.

## Register economy (the wall)

- A **redundant `move`** of a parameter → source read it through a **local that
  survived**. `rc = r` folds back; `rc = (rc = r)` survives. Then check which
  expressions read the copy vs the parameter.
- **Non-void return** reserves `v0` and shifts the whole allocation. If register
  use looks shifted by one, try returning something.
- **Declaration order** sets both which registers loop variables get **and** the
  stack frame layout. Every offset out by one local's size → reorder decls, not
  code. Swapping two decls moves the variables between registers.
- Reading a **global twice** copies it into a second register. If the target
  tests the **same** register both times, the source used a **local**.
- Reaching a parallel array through the wrong base costs a symbol load.
  Materialise the pointer from the index **inside** the block and deref the
  local; a pointer walked alongside `i` gets strength-reduced into a second
  induction variable instead (closed `removeItem`).

## Scheduling (feed values in the order CW consumes them)

- CW squares/consumes a value the moment it has it. Two coord diffs left inline
  are subtracted-and-squared one at a time; lifted into `dx`/`dz` locals first
  they are both subtracted then both squared. Materialise squares in source
  order **now** even if compared later (`inner2 = inner*inner` right after
  `inner`) — closed `getPartnerTamerCloseness`.
- The 3rd `for` clause runs before the body tail. Target bumps counter then
  pointers → they were all in the `for`: `for(i=0;i<3;i++,p++,q++)`.
- Inside a branch where CW knows the index, write the constant: `root[0]` (1
  insn) not `root[i]` (2). This also closed `findEFEDATFile` via the
  `addu $r,$r,$zero` "proven-zero index, still emitted" signal — write `X[i]`
  with `i=0` on the line above, not a pointer local.

## EFE_DATA_STACK pop (efe.c script VM)

The VM data stack pops one 4-byte pointer slot per arg. The target emits
`lw v0,gp; addiu v0,-4; sw v0,gp; lw sX,0(v0)` per pop — **`addiu`** (scaled
ptr), reload the global each pop, reuse the register for the deref. `char*`
`-= 4` gives trapping `addi`; scaled arithmetic
`p = (char*)((int32_t*)EFE_DATA_STACK - 1)` gives `addiu`. Documented near-miss
(scheduling of the per-pop reload) — see `rotateVector`.

## First move on any function

**Look for an already-matched sibling in the same file** before inventing an
idiom. `matrixToEuler2`↔`MAIN_func_800E4470`, `renderPressStartToContinue`↔
`renderMainMenuBackground`, `applyRootMomentum`↔`applyMomentum` went in almost
as written. The `&TOURNAMENT_ARRAY[x*6]` form was two functions up.

## Noise

`Handwritten function` from spimdisasm is a false positive here (77% of the
binary) — it only flags `addi/add/sub`, the trapping arithmetic CW emits.
Ignore it; write from C.
