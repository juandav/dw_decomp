#!/usr/bin/env python3
"""Turn the script files SCN/DG.SCN and SCN/MAPHEAD.SCN into source and back.

    tools/scn.py disasm disks/us/SCN/DG.SCN build/scn/DG
    tools/scn.py asm build/scn/DG build/scn/DG.SCN

disasm writes one source file per script, plus layout.s with the bytes that
belong to no script. asm builds the file again from them. Unchanged source
gives the original file back byte for byte, and `make scn` checks that.

The source can also be edited. Instructions refer to each other through
labels, so text can grow or shrink: @sec_XX is where section XX starts and
@L1, @L2... are the other jump targets. Instructions use the opcode names and
arguments of include/dw/script.h, and text the codes of include/dw/text.h.
Ids are written in hex and other numbers in decimal. A ; starts a comment;
disasm uses them to say who speaks each text and to name the NPCs, items,
triggers and pstats, as tools/dump_dialogue.py does.

A condition block lists its tests in brackets, as scriptConditionBlock()
reads them:

    condition [trigger 0x118] [and not has_move 0x2a] [else @L3]

Each test replaces the result, or is combined with it by "and" or "or".
"not" negates a trigger, has_move or partner_condition test, and the others
compare a value (pstat, stat, card, item, money). [then @L] jumps if the
result is true and [else @L] if it is false; otherwise the script goes on
after the block. An entry the syntax can't express is kept as hex bytes.

Code that no section reaches, such as the old copies of scripts left in the
padding, stays as raw bytes. getScript() reads a DG.SCN script by whole
sectors, so its last bytes, after .padding, only fill the last sector. When
an edit changes the size of a script, asm cuts that padding or adds zeros to
keep whole sectors.
"""

import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dump_dialogue as dd  # noqa: E402

ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..")
TEXT_HEADER = os.path.join(ROOT, "include/dw/text.h")
SCRIPT_HEADER = os.path.join(ROOT, dd.HEADER_PATH)

TEXT_NEWLINE = 0x0D
TEXT_END = 0x00
TEXT_SKIP_3 = 0x03

SECTOR_SIZE = 0x800
# The size of SCRIPT_DATA, where getScript() reads a script.
SCRIPT_DATA_SIZE = 0x4000


def read_text_codes(path=TEXT_HEADER):
    """Return the names of the text control codes, by value."""
    with open(path) as f:
        text = f.read()
    codes = {}
    for m in re.finditer(r"#define TEXT_(\w+)\s+(0x[0-9A-F]{2})\b", text):
        value = int(m.group(2), 16)
        if value < 0x20:
            codes.setdefault(value, m.group(1))
    return codes


CODES = read_text_codes()
CODE_VALUES = {name: value for value, name in CODES.items()}
ESCAPED = {"{": "\\{", "}": "\\}", "\\": "\\\\", '"': '\\"'}

# The US font draws these as a period and an apostrophe, and the text uses
# them for that; the full-width "．" and "＇" never appear.
ASCII_AS = {".": "。", "'": "「"}


def decode_char(pair):
    """Return how a two-byte character is written in source, or None."""
    try:
        ch = pair.decode("cp932")
    except UnicodeDecodeError:
        return None
    if len(ch) != 1:
        return None
    for text in (ascii_form(ch), ch):
        if encode_char(text) == pair:
            return ESCAPED.get(text, text)
    return None


def ascii_form(ch):
    if ch == "　":
        return " "
    for ascii_ch, other in ASCII_AS.items():
        if ch == other:
            return ascii_ch
    if 0xFF01 <= ord(ch) <= 0xFF5E:
        # Full-width ASCII is written as ASCII.
        return chr(ord(ch) - 0xFEE0)
    return ch


def encode_char(ch):
    if ch == " ":
        return "　".encode("cp932")
    if ch in ASCII_AS:
        return ASCII_AS[ch].encode("cp932")
    if 0x21 <= ord(ch) <= 0x7E:
        return chr(ord(ch) + 0xFEE0).encode("cp932")
    return ch.encode("cp932")


def decode_text(data, pos):
    """Return the source of the text at pos.

    A text ends with TEXT_NEWLINE and TEXT_END. With zero arguments they are
    left out of the source, else they are written as {end xx yy}. A TEXT_END
    inside the text, which stops the drawing there, is written as {END}.
    """
    out = []
    while True:
        code = data[pos]
        if code == TEXT_NEWLINE:
            arg = data[pos + 1]
            pos += 2
            if data[pos] == TEXT_END:
                end_arg = data[pos + 1]
                if arg or end_arg:
                    out.append(f"{{end {arg:02x} {end_arg:02x}}}")
                return "".join(out)
            out.append("\\n" if arg == 0 else f"{{NEWLINE {arg:02x}}}")
        elif code == TEXT_SKIP_3:
            args = " ".join(f"{b:02x}" for b in data[pos + 1:pos + 4])
            out.append(f"{{SKIP_3 {args}}}")
            pos += 4
        elif code < 0x20:
            name = CODES.get(code, f"{code:02x}")
            arg = data[pos + 1]
            out.append(f"{{{name}}}" if arg == 0 else f"{{{name} {arg:02x}}}")
            pos += 2
        else:
            pair = bytes(data[pos:pos + 2])
            text = decode_char(pair)
            out.append(text if text is not None
                       else f"{{raw {pair[0]:02x} {pair[1]:02x}}}")
            pos += 2


def encode_text(text):
    out = bytearray()
    ended = False
    i = 0
    while i < len(text):
        ch = text[i]
        if ch == "\\":
            nxt = text[i + 1]
            if nxt == "n":
                out += bytes([TEXT_NEWLINE, 0])
            else:
                out += encode_char(nxt)
            i += 2
            continue
        if ch == "{":
            close = text.index("}", i)
            words = text[i + 1:close].split()
            i = close + 1
            name, args = words[0], [int(w, 16) for w in words[1:]]
            if name == "raw":
                out += bytes(args)
            elif name == "end":
                out += bytes([TEXT_NEWLINE, args[0], TEXT_END, args[1]])
                ended = True
            else:
                value = CODE_VALUES[name] if name in CODE_VALUES else int(name, 16)
                out += bytes([value] + (args or [0]))
            continue
        out += encode_char(ch)
        i += 1
    if not ended:
        out += bytes([TEXT_NEWLINE, 0, TEXT_END, 0])
    return bytes(out)


OPCODES = dd.read_opcodes(SCRIPT_HEADER)
OP_NAMES = {name.lower(): op for op, (name, _) in OPCODES.items()}

# Arguments written in hex: the ids, and the ones script.h leaves unnamed.
HEX_ARGS = {"item", "digimon", "move", "card", "trigger", "builtin", "stat",
            "map", "offset", "script", "section", "flag", "mask"}
# Arguments that get a name in a comment.
NAMED_ARGS = {"item", "digimon", "move", "card", "trigger", "builtin", "stat"}


def arg_label(kind, name):
    return kind if name == kind else name.split()[0]


def is_hex(kind, name):
    return name == kind or kind in ("id", "pstat") or arg_label(kind, name) in HEX_ARGS


# Condition entries: bits 7 and 6 combine the test with the result so far,
# bits 3-5 are the group and bits 0-2 the comparison or test.
COND_AND = 0x80
COND_OR = 0x40
COND_TRIGGER = 0
COND_PSTAT = 1
COND_THEN = 2
COND_ELSE = 3
COND_TEST = 4
COMPARE = ["==", "!=", ">=", "<=", ">", "<"]
# The scriptTest*() functions of group 4, with the name of their first
# argument: the ones with a comparison read an u8 id, the comparison and a
# value; the others an u8 id and a byte that negates them.
TESTS = ["stat", "card", "has_move", "partner_condition", "item", "money"]
NEGATED_TESTS = {"has_move", "partner_condition"}
SCRIPT_STAT_HAPPINESS = 0x09


def decode_condition_entry(e, ref):
    """Return the source of one entry of a condition block."""
    raw = "[" + " ".join(f"{b:02x}" for b in e) + "]"
    op = e[0]
    join, group, low = op & 0xC0, (op >> 3) & 7, op & 7
    if e[1] != 0 or join == COND_AND | COND_OR:
        return raw
    prefix = {0: "", COND_AND: "and ", COND_OR: "or "}[join]
    if group == COND_TRIGGER and low < 2:
        neg = "not " if low else ""
        return f"[{prefix}{neg}trigger 0x{e[2] | e[3] << 8:x}]"
    if group == COND_PSTAT and low < len(COMPARE):
        return f"[{prefix}pstat 0x{e[2]:x} {COMPARE[low]} {e[3]}]"
    if group in (COND_THEN, COND_ELSE) and join == 0 and low == 0:
        word = "then" if group == COND_THEN else "else"
        return f"[{word} {ref(e[2] | e[3] << 8)}]"
    if group != COND_TEST or low >= len(TESTS):
        return raw
    test = TESTS[low]
    if test in NEGATED_TESTS:
        if e[3] > 1:
            return raw
        neg = "not " if e[3] else ""
        return f"[{prefix}{neg}{test} 0x{e[2]:x}]"
    if e[3] >= len(COMPARE):
        return raw
    if test == "money":
        if e[2] != 0:
            return raw
        value = int.from_bytes(e[4:8], "little", signed=True)
        return f"[{prefix}money {COMPARE[e[3]]} {value}]"
    value = int.from_bytes(e[4:6], "little",
                           signed=test == "stat" and e[2] == SCRIPT_STAT_HAPPINESS)
    return f"[{prefix}{test} 0x{e[2]:x} {COMPARE[e[3]]} {value}]"


def encode_condition_entry(entry, value):
    words = entry[1:-1].split()
    if all(re.fullmatch(r"[0-9a-f]{2}|@\S+|0x[0-9a-f]+", w) for w in words):
        # Hex bytes, where a label or 0x... is an u16.
        out = bytearray()
        for w in words:
            out += value(w).to_bytes(2, "little") if len(w) != 2 else bytes([int(w, 16)])
        return bytes(out)
    join = 0
    if words[0] in ("and", "or"):
        join = COND_AND if words.pop(0) == "and" else COND_OR
    neg = 0
    if words[0] == "not":
        words.pop(0)
        neg = 1
    kind = words[0]
    if kind in ("then", "else"):
        group = COND_THEN if kind == "then" else COND_ELSE
        return bytes([group << 3, 0]) + value(words[1]).to_bytes(2, "little")
    if kind == "trigger":
        return bytes([join | COND_TRIGGER << 3 | neg, 0]) + value(words[1]).to_bytes(2, "little")
    if kind == "pstat":
        cmp = COMPARE.index(words[2])
        return bytes([join | COND_PSTAT << 3 | cmp, 0, value(words[1]), value(words[3])])
    test = TESTS.index(kind)
    head = bytes([join | COND_TEST << 3 | test, 0])
    if kind in NEGATED_TESTS:
        return head + bytes([value(words[1]), neg])
    if kind == "money":
        v = value(words[2])
        return head + bytes([0, COMPARE.index(words[1])]) + v.to_bytes(4, "little", signed=v < 0)
    v = value(words[3]) & 0xFFFF
    return head + bytes([value(words[1]), COMPARE.index(words[2])]) + v.to_bytes(2, "little")


def condition_entries(data, pos):
    """Return the entries of the condition block at pos, with its end."""
    entries = []
    pos += 2
    while True:
        op = data[pos]
        if op == dd.OP_CONDITION:
            entries.append(bytes(data[pos:pos + 2]))
            return entries
        group = (op >> 3) & 7
        size = dd.TEST_SIZES[op & 7] if group == COND_TEST else 4
        entries.append(bytes(data[pos:pos + size]))
        pos += size


class Disassembler:
    def __init__(self, script, names=None, sectors=False):
        self.s = script
        self.names = names
        self.sectors = sectors
        self.data = script.scn
        for _, offset in script.sections:
            script.walk(offset)
        self.base = script.start
        targets = {offset for _, offset in script.sections}
        targets.add(dd.u16(self.data, self.base))
        self.items = {}
        for pos, nxt in script.code.items():
            if nxt > pos:
                self.items[pos] = self.instruction_end(pos, nxt)
        for pos in self.items:
            targets.update(self.targets(pos))
        self.labels = self.name_labels(targets)

    def instruction_end(self, pos, nxt):
        if self.data[pos] == dd.OP_SWITCH:
            # The table of offsets follows the instruction.
            return pos + 4 + 2 * dd.u16(self.data, pos + 2)
        return nxt

    def targets(self, pos):
        data, op = self.data, self.data[pos]
        if op in (dd.OP_CALL, dd.OP_JUMP):
            return [dd.u16(data, pos + 2)]
        if op == dd.OP_SWITCH:
            return [dd.u16(data, pos + 4 + 2 * i) for i in range(dd.u16(data, pos + 2))]
        if op == dd.OP_CHOICE:
            return [dd.u16(data, pos + 2 + 2 * i) for i in range(data[pos + 1])]
        if op == dd.OP_CONDITION:
            out = []
            for entry in condition_entries(data, pos):
                if entry[0] != dd.OP_CONDITION and (entry[0] >> 3) & 7 in (COND_THEN, COND_ELSE):
                    out.append(entry[2] | entry[3] << 8)
            return out
        return []

    def name_labels(self, targets):
        """Name the targets that start an instruction.

        Section starts are named after their section and the others are
        numbered in order, so that editing a script doesn't rename them.
        """
        starts = {pos - self.base for pos in self.items}
        labels = {}
        for section, offset in self.s.sections:
            if offset in starts and offset not in labels:
                name = f"@sec_{section:02x}"
                while name in labels.values():
                    name += "_"
                labels[offset] = name
        header = dd.u16(self.data, self.base)
        if header in starts and header not in labels:
            labels[header] = "@start"
        others = sorted(t for t in targets if t in starts and t not in labels)
        for i, offset in enumerate(others, 1):
            labels[offset] = f"@L{i}"
        return labels

    def ref(self, offset):
        return self.labels.get(offset, f"0x{offset:x}")

    def lines(self):
        data, base = self.data, self.base
        table_end = base + 2 + 4 * len(self.s.sections) + 2
        out = [f"header {self.ref(dd.u16(data, base))}"]
        for section, offset in self.s.sections:
            out.append(f"section 0x{section:x} {self.ref(offset)}")
        out.append("")
        code_end = max(self.items.values())
        cuts = sorted(set(self.items) | {code_end, self.s.end})
        pos = table_end
        for cut in cuts:
            if cut < pos:
                continue
            if cut > pos:
                out += self.raw(pos, cut)
            pos = cut
            if pos >= self.s.end:
                break
            if pos == code_end and self.sectors:
                out.append(".padding")
            if pos - base in self.labels:
                out.append(f"{self.labels[pos - base]}:")
            if pos in self.items:
                line = "\t" + self.instruction(pos)
                if self.names is not None:
                    note = comment(self.s, pos, self.names)
                    if note:
                        line += "\t; " + note
                out.append(line)
                pos = self.items[pos]
        if pos < self.s.end:
            out += self.raw(pos, self.s.end)
        return out

    def raw(self, start, end):
        out = []
        for pos in range(start, end, 16):
            chunk = self.data[pos:min(pos + 16, end)]
            out.append("\t.bytes " + " ".join(f"{b:02x}" for b in chunk))
        return out

    def instruction(self, pos):
        data, op = self.data, self.data[pos]
        if op == dd.OP_TEXT:
            text = decode_text(data, pos + 2)
            pad = f" pad={data[pos + 1]}" if data[pos + 1] else ""
            return f'text "{text}"{pad}'
        if op == dd.OP_CHOICE:
            count = data[pos + 1]
            targets = " ".join(self.ref(dd.u16(data, pos + 2 + 2 * i)) for i in range(count))
            extra = dd.u16(data, pos + 2 + 2 * count)
            text = decode_text(data, pos + 4 + 2 * count)
            tail = "" if extra == 0x001A else f" extra=0x{extra:04x}"
            return f'choice [{targets}] "{text}"{tail}'
        if op == dd.OP_CONDITION:
            entries = condition_entries(data, pos)
            if entries[-1] == bytes([dd.OP_CONDITION, 0]):
                entries.pop()
            parts = [decode_condition_entry(e, self.ref) for e in entries]
            pad = f" pad={data[pos + 1]}" if data[pos + 1] else ""
            return "condition " + " ".join(parts) + pad
        if op == dd.OP_SWITCH:
            count = dd.u16(data, pos + 2)
            targets = " ".join(self.ref(dd.u16(data, pos + 4 + 2 * i)) for i in range(count))
            return f"switch 0x{data[pos + 1]:x} [{targets}]"
        name = OPCODES[op][0]
        words = [name.lower()]
        pads = []
        for kind, arg, value in read_args(data, pos):
            if kind == "_":
                pads.append(value)
            elif op in (dd.OP_CALL, dd.OP_JUMP) and kind == "u16":
                words.append(self.ref(value))
            elif is_hex(kind, arg) and value >= 0:
                words.append(f"0x{value:x}")
            else:
                words.append(str(value))
        if any(pads):
            words.append("pad=" + ",".join(str(p) for p in pads))
        return " ".join(words)


def read_args(data, pos):
    """Yield the kind, name and value of each argument of an instruction."""
    at = pos + 1
    for kind, arg in OPCODES[data[pos]][1]:
        size = dd.ARG_SIZES[kind]
        yield kind, arg, int.from_bytes(data[at:at + size], "little",
                                        signed=kind in ("s8", "s16", "s32"))
        at += size


def load_context(scn_dir):
    """Read both script files and work out the names the comments show.

    This is what tools/dump_dialogue.py does: find who says each text and
    name the NPCs, items, triggers and pstats from the executable and the
    headers. Without the executable and maps, the comments are left out.
    """
    with open(os.path.join(scn_dir, "DG.SCN"), "rb") as f:
        scripts = dd.read_scripts(f.read())
    with open(os.path.join(scn_dir, "MAPHEAD.SCN"), "rb") as f:
        maphead = f.read()
    scripts[0] = dd.Script(maphead, 0, len(maphead))
    for script in scripts.values():
        for _, offset in script.sections:
            texts, _, _ = script.walk(offset)
            for pos, speaker in texts.items():
                script.default_speakers.setdefault(pos, speaker)
    exe_path = os.path.join(ROOT, dd.EXE_PATH)
    map_dir = os.path.join(ROOT, dd.MAP_DIR)
    if not (os.path.exists(exe_path) and os.path.exists(map_dir)):
        return scripts, None
    exe = dd.Exe(exe_path, os.path.join(ROOT, dd.SYMBOLS_PATH))
    dd.trace_contexts(scripts)
    dd.name_npcs(scripts, dd.read_map_npcs(exe, map_dir), dd.read_map_files(exe))
    dd.LABELS["trigger"] = dd.Labels(os.path.join(ROOT, dd.TRIGGER_PATH), "TRIGGER")
    dd.LABELS["pstat"] = dd.Labels(os.path.join(ROOT, dd.PSTAT_PATH), "PSTAT")
    dd.LABELS["builtin"] = dd.Labels(SCRIPT_HEADER, "SCRIPT_BUILTIN")
    dd.LABELS["stat"] = dd.Labels(SCRIPT_HEADER, "SCRIPT_STAT")
    return scripts, dd.read_names(exe)


def name_of(script, kind, value, names):
    """Return the name of an id, or None if it has none."""
    if kind == "id":
        name = dd.speaker_name(value, script.npcs)
        return None if name.startswith("npc ") else name
    if kind in dd.LABELS:
        name = dd.LABELS[kind].label(value, names)
        return None if name.startswith("0x") else name
    table = names.get(kind)
    if table and 0 <= value < len(table):
        return table[value]
    return None


def comment(script, pos, names):
    """Return the names an instruction refers to, or who says its text."""
    data, op = script.scn, script.scn[pos]
    if op == dd.OP_TEXT:
        if pos not in script.default_speakers:
            return None
        speaker, _ = dd.decode_message(data, script.texts[pos][0])
        if speaker is None:
            speaker = script.speaker_names.get(pos)
        if speaker is None:
            speaker = dd.speaker_name(script.default_speakers[pos], script.npcs)
        return speaker
    if op == dd.OP_CHOICE:
        return None
    found = []
    if op == dd.OP_SWITCH:
        found.append(("pstat", "pstat", data[pos + 1]))
    elif op == dd.OP_CONDITION:
        for e in condition_entries(data, pos):
            group, low = (e[0] >> 3) & 7, e[0] & 7
            if e[0] == dd.OP_CONDITION:
                continue
            if group == COND_TRIGGER:
                found.append(("trigger", "trigger", e[2] | e[3] << 8))
            elif group == COND_PSTAT:
                found.append(("pstat", "pstat", e[2]))
            elif group == COND_TEST and low < len(TESTS) and TESTS[low] != "money":
                kind = TESTS[low].replace("has_", "")
                if kind in NAMED_ARGS:
                    found.append((kind, kind, e[2]))
    else:
        for kind, arg, value in read_args(data, pos):
            label = arg_label(kind, arg)
            if kind in ("id", "pstat"):
                found.append((kind, label, value))
            elif label in NAMED_ARGS:
                found.append((label, label, value))
    words = []
    for kind, label, value in found:
        name = name_of(script, kind, value, names)
        if name is None:
            continue
        if op == dd.OP_CONDITION and len(found) > 1:
            # Say which of the tests the name is for.
            words.append(f"0x{value:x}={name}")
        elif label in ("id", "pstat", "trigger", "card"):
            words.append(name)
        else:
            words.append(f"{label}={name}")
    return ", ".join(words) or None


def disassemble(path, out_dir):
    with open(path, "rb") as f:
        data = f.read()
    os.makedirs(out_dir, exist_ok=True)
    context, names = load_context(os.path.dirname(path))
    if os.path.basename(path).upper() == "MAPHEAD.SCN":
        scripts = {0: context[0]}
    else:
        scripts = {i: s for i, s in context.items() if i != 0}
    layout = []
    pos = 0
    if 0 not in scripts:
        layout.append(".offsets")
    for index, script in sorted(scripts.items(), key=lambda kv: kv[1].start):
        if script.start > pos:
            layout += [f".bytes {data[p:min(p + 16, script.start)].hex(' ')}"
                       for p in range(pos, script.start, 16)]
        name = f"{index:03d}.s"
        lines = Disassembler(script, names, sectors=0 not in scripts).lines()
        with open(os.path.join(out_dir, name), "w", encoding="utf-8") as f:
            f.write(f"; script {index}\n")
            f.write("\n".join(lines) + "\n")
        layout.append(f"script {index} {name}")
        pos = script.end
    layout += [f".bytes {data[p:min(p + 16, len(data))].hex(' ')}"
               for p in range(pos, len(data), 16)]
    with open(os.path.join(out_dir, "layout.s"), "w") as f:
        f.write("\n".join(layout) + "\n")


def split_words(line):
    """Split a line into words, keeping quoted text and [...] together."""
    return re.findall(r'"(?:[^"\\]|\\.)*"|\[[^\]]*\]|\S+', line)


def strip_comment(line):
    """Remove a ; comment, but not a ; inside quoted text."""
    quoted = False
    i = 0
    while i < len(line):
        if line[i] == "\\" and quoted:
            i += 2
            continue
        if line[i] == '"':
            quoted = not quoted
        elif line[i] == ";" and not quoted:
            return line[:i]
        i += 1
    return line


def assemble_script(lines, filename):
    body = []
    for number, line in enumerate(lines, 1):
        line = strip_comment(line).strip()
        if line:
            body.append((number, line))
    labels = {}
    for final in (False, True):
        header = None
        sections = []
        code = bytearray()
        padding = None
        for _, line in body:
            if line.startswith("header "):
                header = line.split()[1]
            elif line.startswith("section "):
                _, sid, ref = line.split()
                sections.append((int(sid, 0), ref))

        def value(ref):
            if not ref.startswith("@"):
                return int(ref, 0)
            if ref in labels:
                return labels[ref]
            if final:
                raise KeyError(f"no label {ref}")
            return 0

        pos = 2 + 4 * len(sections) + 2
        for number, line in body:
            if line.startswith(("header ", "section ")):
                continue
            if line.endswith(":"):
                labels[line[:-1]] = pos
                continue
            if line == ".padding":
                padding = len(code)
                continue
            try:
                enc = encode_line(line, value)
            except (KeyError, ValueError, IndexError) as e:
                sys.exit(f"{filename}:{number}: {e.args[0] if e.args else e}: {line}")
            code += enc
            pos += len(enc)
        out = value(header).to_bytes(2, "little")
        for sid, ref in sections:
            out += sid.to_bytes(2, "little") + value(ref).to_bytes(2, "little")
        out += b"\xff\xff" + code
    if padding is not None:
        out = fill_sectors(out, len(out) - len(code) + padding)
    return bytes(out)


def fill_sectors(out, used):
    """Make a script a whole number of sectors again after an edit.

    getScript() reads a script by whole sectors, so the bytes after its
    last instruction pad it to a multiple of SECTOR_SIZE. When an edit has
    changed the size, the old padding is cut or extended with zeros.
    """
    if len(out) % SECTOR_SIZE == 0:
        return out
    size = -(-used // SECTOR_SIZE) * SECTOR_SIZE
    return out[:size] + bytes(max(0, size - len(out)))


def encode_line(line, value):
    words = split_words(line)
    name = words[0]
    if name == ".bytes":
        return bytes(int(w, 16) for w in words[1:])
    options = dict(w.split("=", 1) for w in words[1:] if "=" in w and not w.startswith(('"', "[")))
    words = [w for w in words[1:] if not ("=" in w and not w.startswith(('"', "[")))]
    pad = int(options.get("pad", "0").split(",")[0])
    if name == "text":
        return bytes([dd.OP_TEXT, pad]) + encode_text(unquote(words[0]))
    if name == "choice":
        targets = words[0][1:-1].split()
        extra = int(options.get("extra", "0x001a"), 0)
        out = bytes([dd.OP_CHOICE, len(targets)])
        for t in targets:
            out += value(t).to_bytes(2, "little")
        return out + extra.to_bytes(2, "little") + encode_text(unquote(words[1]))
    if name == "condition":
        out = bytearray([dd.OP_CONDITION, pad])
        for entry in words:
            out += encode_condition_entry(entry, value)
        # The block ends with another 0x19, unless it is written out.
        if not words or not words[-1][1:].startswith("19"):
            out += bytes([dd.OP_CONDITION, 0])
        return bytes(out)
    if name == "switch":
        targets = words[1][1:-1].split()
        out = bytes([dd.OP_SWITCH, int(words[0], 0)]) + len(targets).to_bytes(2, "little")
        for t in targets:
            out += value(t).to_bytes(2, "little")
        return out
    if name not in OP_NAMES:
        raise KeyError(f"no opcode {name}")
    op = OP_NAMES[name]
    pads = [int(p) for p in options["pad"].split(",")] if "pad" in options else []
    out = bytearray([op])
    for kind, _ in OPCODES[op][1]:
        size = dd.ARG_SIZES[kind]
        v = (pads.pop(0) if pads else 0) if kind == "_" else value(words.pop(0))
        out += v.to_bytes(size, "little", signed=v < 0)
    return bytes(out)


def unquote(word):
    return word[1:-1].replace('\\"', '"')


def assemble(src_dir, path):
    with open(os.path.join(src_dir, "layout.s")) as f:
        layout = [line.split() for line in f if line.strip()]
    has_offsets = [".offsets"] in layout
    out = bytearray()
    starts = {}
    ends = {}
    for words in layout:
        if words[0] == ".bytes":
            out += bytes(int(w, 16) for w in words[1:])
        elif words[0] == "script":
            index, filename = int(words[1]), os.path.join(src_dir, words[2])
            with open(filename, encoding="utf-8") as f:
                code = assemble_script(f.read().splitlines(), filename)
            if has_offsets and len(code) > SCRIPT_DATA_SIZE:
                sys.exit(f"{filename}: {len(code):#x} bytes, "
                         f"more than the {SCRIPT_DATA_SIZE:#x} of SCRIPT_DATA")
            starts[index] = len(out)
            out += code
            ends[index] = len(out)
    if has_offsets:
        # The table of offsets of DG.SCN: script i starts at entry i, and
        # the entry after the last script is the end of the file.
        order = sorted(starts)
        for index in order:
            out[4 * index:4 * index + 4] = starts[index].to_bytes(4, "little")
        last = order[-1] + 1
        out[4 * last:4 * last + 4] = ends[order[-1]].to_bytes(4, "little")
    with open(path, "wb") as f:
        f.write(out)


def main():
    if len(sys.argv) != 4 or sys.argv[1] not in ("asm", "disasm"):
        sys.exit(__doc__)
    if sys.argv[1] == "disasm":
        disassemble(sys.argv[2], sys.argv[3])
    else:
        assemble(sys.argv[2], sys.argv[3])


if __name__ == "__main__":
    main()
