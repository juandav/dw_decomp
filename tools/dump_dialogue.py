#!/usr/bin/env python3
"""Print the dialogue of the map scripts in SCN/DG.SCN.

DG.SCN starts with a table of offsets, one script per map id. A script starts
with the offset of its code, then a table of (section, offset) pairs ended by
0xFFFF: section 0xFE runs when the map loads, and the others run when the
player talks to the NPC with that script id. This follows the code from every
section, the way the interpreter in script_interp.c would, and prints each
text instruction (0x1A) and choice (0x10) it reaches. The text format is
described in include/dw/text.h.

Each script is padded to a multiple of 0x800 bytes, and the padding often
holds an older copy of the script. --unreachable also prints the text found
there, which the game never shows.

Script 0 is SCN/MAPHEAD.SCN, the script shared by every map, as getScript()
loads it. Its message tables are left out; tools/maphead_text.py prints them.

    tools/dump_dialogue.py             # every script
    tools/dump_dialogue.py 0 5         # MAPHEAD.SCN and script 5
"""

import argparse
import struct
import sys

from maphead_text import TABLES, decode_message, format_message

DEFAULT_PATH = "disks/us/SCN/DG.SCN"
MAPHEAD_PATH = "disks/us/SCN/MAPHEAD.SCN"
HEADER_ENTRIES = 0x800

OP_CHOICE = 0x10
OP_CALL = 0x13
OP_JUMP = 0x16
OP_SWITCH = 0x18
OP_CONDITION = 0x19
OP_TEXT = 0x1A
OP_SPEAKER = 0x1B
OP_SIZED_TEXTBOX = 0x26

SPEAKER_PLAYER = 0xFD
SPEAKER_NONE = 0xFF

TEXT_END = 0x00
TEXT_SKIP_3 = 0x03
TEXT_NEWLINE = 0x0D
# The codes that take one argument byte, TEXT_COLOR to TEXT_TOURNAMENT_LOSSES.
TEXT_CODES = set(range(0x01, 0x16)) - {TEXT_SKIP_3, TEXT_NEWLINE}


def _sizes():
    """Return the size in bytes of each opcode that has a fixed size."""
    table = {
        1: [0x6B, 0xFC, 0xFD, 0xFE, 0xFF],
        2: [0x15, 0x1B, 0x21, 0x22, 0x23, 0x25, 0x27, 0x2D, 0x2E, 0x2F,
            0x30, 0x37, 0x3D, 0x3E, 0x46, 0x48, 0x49, 0x4A, 0x54, 0x58,
            0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x64, 0x65, 0x66, 0x69, 0x76,
            0x78, 0x79, 0x7B],
        4: [0x13, 0x16, 0x18, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x24, 0x26,
            0x28, 0x29, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x3A, 0x3B,
            0x3C, 0x3F, 0x47, 0x4B, 0x4C, 0x4D, 0x50, 0x51, 0x53, 0x56,
            0x57, 0x5A, 0x67, 0x68, 0x6A, 0x6D, 0x6F, 0x70, 0x7A, 0x7E],
        6: [0x14, 0x17, 0x2A, 0x2B, 0x38, 0x39, 0x4F, 0x72, 0x73, 0x74,
            0x77, 0xFB],
        8: [0x4E, 0x52, 0x55, 0x6C, 0x6E, 0x7C],
        10: [0x2C, 0x71, 0x7D],
        12: [0x75],
    }
    return {op: size for size, ops in table.items() for op in ops}


SIZES = _sizes()

# Opcodes after which the next instruction does not run.
ENDS = {0x15, 0x16, 0x17, 0x18, 0xFB, 0xFE, 0xFF}

# Entries of a condition block (opcode 0x19), by bits 3-5 of their opcode.
# Group 4 calls one of six tests, which read arguments of their own.
CONDITION_SIZES = {0: 4, 1: 4, 2: 4, 3: 4}
TEST_SIZES = {0: 6, 1: 6, 2: 4, 3: 4, 4: 6, 5: 8}

# The ones below 0xC8 are the script ids of the NPCs of the map.
SPEAKERS = {
    0xC8: "Sign",
    0xC9: "Box",
    0xCA: "Betamon",
    0xCB: "Coelamon",
    0xCC: "Tanemon",
    0xCD: "Palmon",
    0xFC: "partner",
    0xFD: "player",
    0xFF: "narrator",
}


def speaker_name(speaker):
    return SPEAKERS.get(speaker, f"npc {speaker}")


def u16(data, pos):
    return struct.unpack_from("<H", data, pos)[0]


def text_end(scn, pos):
    """Return where the text that starts at pos ends."""
    while pos + 1 < len(scn):
        code = scn[pos]
        if code == TEXT_SKIP_3:
            pos += 4
        elif code == TEXT_NEWLINE:
            pos += 2
            if scn[pos] == TEXT_END:
                return pos + 2
        else:
            pos += 2
    return pos


class Script:
    def __init__(self, scn, start, end):
        self.scn = scn
        self.start = start
        self.end = end
        self.sections = []
        pos = start + 2
        while True:
            section, offset = struct.unpack_from("<HH", scn, pos)
            if section == 0xFFFF:
                break
            self.sections.append((section, offset))
            pos += 4
        # Position of each instruction reached, mapped to the next one.
        self.code = {}
        self.texts = {}
        self.speakers = {}

    def walk(self, offset):
        """Follow the code from offset; return the positions of new text."""
        found = []
        # callScriptSection() starts with the player as the speaker.
        work = [(self.start + offset, SPEAKER_PLAYER)]
        scn = self.scn
        while work:
            pos, speaker = work.pop()
            while self.start <= pos < self.end and pos not in self.code:
                op = scn[pos]
                if op == OP_SPEAKER:
                    speaker = scn[pos + 1]
                elif op == OP_SIZED_TEXTBOX:
                    speaker = SPEAKER_NONE
                if op == OP_TEXT:
                    nxt = text_end(scn, pos + 2)
                    self.texts[pos] = (pos + 2, None)
                    self.speakers[pos] = speaker
                    found.append(pos)
                elif op == OP_CHOICE:
                    # The offsets of the options, then a text instruction.
                    count = scn[pos + 1]
                    targets = [u16(scn, pos + 2 + 2 * i) for i in range(count)]
                    text = pos + 2 + 2 * (count + 1)
                    self.code[pos] = text_end(scn, text)
                    self.texts[pos] = (text, count)
                    found.append(pos)
                    work += [(self.start + t, speaker) for t in targets]
                    break
                elif op == OP_CONDITION:
                    jumps = []
                    nxt = self._condition(pos, jumps)
                    work += [(j, speaker) for j in jumps]
                    if nxt is None:
                        self.code[pos] = pos
                        break
                elif op in SIZES:
                    nxt = pos + SIZES[op]
                    if op in (OP_CALL, OP_JUMP):
                        work.append((self.start + u16(scn, pos + 2), speaker))
                    elif op == OP_SWITCH:
                        count = u16(scn, pos + 2)
                        work += [(self.start + u16(scn, nxt + 2 * i), speaker)
                                 for i in range(count)]
                else:
                    # Not code: some sections hold data read by the engine.
                    self.code[pos] = pos
                    break
                self.code[pos] = nxt
                if op in ENDS:
                    break
                pos = nxt
        return found

    def _condition(self, pos, jumps):
        """Skip a condition block and collect where it can jump to."""
        scn = self.scn
        pos += 2
        while scn[pos] != OP_CONDITION:
            group = (scn[pos] >> 3) & 7
            if group == 4:
                size = TEST_SIZES.get(scn[pos] & 7)
            else:
                size = CONDITION_SIZES.get(group)
            if size is None or pos >= self.end:
                return None
            if group in (2, 3):
                # Jump if the condition is true (2) or false (3).
                jumps.append(self.start + u16(scn, pos + 2))
            pos += size
        return pos + 2

    def is_text(self, pos):
        """Check that pos holds a whole message, for text outside the code."""
        scn = self.scn
        chars = 0
        while pos + 2 < self.end:
            code = scn[pos]
            if code == TEXT_NEWLINE:
                pos += 2
                if scn[pos] == TEXT_END:
                    return chars > 0
            elif code in TEXT_CODES:
                pos += 2
            elif 0x81 <= code <= 0x84:
                chars += 1
                pos += 2
            else:
                return False
        return False

    def unreachable_texts(self):
        """Yield text instructions outside the code that was reached."""
        scn = self.scn
        pos = self.start
        while pos < self.end - 2:
            if (scn[pos] == OP_TEXT and scn[pos + 1] == 0
                    and pos not in self.code and pos not in self.texts
                    and self.is_text(pos + 2)):
                yield pos
            pos += 2


def format_text(script, pos):
    text, count = script.texts[pos]
    speaker, rows = decode_message(script.scn, text)
    if count is not None:
        return "choice: " + " / ".join(rows)
    # Text that names its speaker, such as a narrator's line, keeps that name.
    if speaker is None:
        speaker = speaker_name(script.speakers[pos])
    return format_message(speaker, rows)


def read_scripts(scn):
    offsets = struct.unpack_from(f"<{HEADER_ENTRIES}I", scn, 0)
    scripts = {}
    for index in range(1, HEADER_ENTRIES - 1):
        start, end = offsets[index], offsets[index + 1]
        if start == 0 or end <= start or end > len(scn):
            break
        scripts[index] = Script(scn, start, end)
    return scripts


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("scripts", nargs="*", type=lambda x: int(x, 0))
    parser.add_argument("--scn", default=DEFAULT_PATH)
    parser.add_argument("--maphead", default=MAPHEAD_PATH)
    parser.add_argument("--unreachable", action="store_true",
                        help="also print text the game never reaches")
    args = parser.parse_args()

    with open(args.scn, "rb") as f:
        scn = f.read()

    scripts = read_scripts(scn)
    with open(args.maphead, "rb") as f:
        maphead = f.read()
    scripts[0] = Script(maphead, 0, len(maphead))

    for index in args.scripts or sorted(scripts):
        if index not in scripts:
            sys.exit(f"there is no script {index}")
        script = scripts[index]
        scn = script.scn
        name = "MAPHEAD.SCN" if index == 0 else f"{script.start:#x}"
        print(f"== script {index} ({name})")
        for section, offset in script.sections:
            if index == 0 and section in TABLES:
                continue
            found = script.walk(offset)
            if not found:
                continue
            print(f"-- section {section:#x}")
            for pos in sorted(found):
                print("  " + format_text(script, pos))
        if args.unreachable:
            # Skip the lines that the reachable code also has.
            known = {tuple(decode_message(scn, text)[1])
                     for text, _ in script.texts.values()}
            extra = []
            for pos in script.unreachable_texts():
                speaker, rows = decode_message(scn, pos + 2)
                if tuple(rows) not in known:
                    known.add(tuple(rows))
                    extra.append(format_message(speaker, rows))
            if extra:
                print("-- unreachable")
                for line in extra:
                    print("  " + line)
        print()


if __name__ == "__main__":
    main()
