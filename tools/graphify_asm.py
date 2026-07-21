#!/usr/bin/env python3

"""Feed the disassembly into the graphify knowledge graph.

graphify parses the C in src/, which is the part that is already done. Every
function still in asm/ is invisible to it. This walks the disassembly, turns
each glabel into a node and each jal into a call edge, and merges the result
into graphify-out/graph.json so the graph covers the whole binary.

Calls that land on a symbol with no glabel anywhere are already decompiled (or
live in the psyq libraries), so they are pointed at the existing C node when
one exists.

  tools/graphify_asm.py            # worklist, nothing written
  tools/graphify_asm.py --merge    # merge into graphify-out/graph.json
"""

import argparse
import json
import re
import yaml
from collections import defaultdict
from pathlib import Path

TOP = Path(__file__).resolve().parent.parent

ASM_DIR = TOP / 'asm'
GRAPH = TOP / 'graphify-out' / 'graph.json'
OBJDIFF_CONFIG = TOP / 'tools' / 'objdiff' / 'config.yaml'

ORIGIN = 'asm'

GLABEL = re.compile(r'^\s*glabel\s+(\S+)')
NONMATCHING = re.compile(r'^\s*nonmatching\s+(\S+)\s*,\s*(0x[0-9A-Fa-f]+|\d+)')
JAL = re.compile(r'^\s*/\*.*?\*/\s*jal\s+(\S+)')

# spimdisasm splits out branch targets it cannot tell apart from a function
# start. They are a couple of instructions mid-function, not something to
# decompile on its own.
JLABEL = re.compile(r'jlabel_[0-9A-Fa-f]{8}$')


def _node_id(overlay, name):
    return 'asm_{}_{}'.format(overlay, name.lower())


def _ignored():
    """The psyq libraries and anything else objdiff already skips. Those are
    Sony's, not ours to decompile."""
    with open(OBJDIFF_CONFIG) as f:
        return tuple(yaml.safe_load(f)['ignored_files'])


def _parse_asm():
    """Return (functions, calls). functions maps name -> metadata, calls is a
    list of (caller, callee) pairs."""
    functions = {}
    calls = []
    ignored = _ignored()

    for path in sorted(ASM_DIR.rglob('*.s')):
        relative = path.relative_to(ASM_DIR)
        if 'data' in relative.parts:
            continue
        if str(relative.as_posix()).startswith(ignored):
            continue

        overlay = relative.parts[0]
        rel = path.relative_to(TOP).as_posix()

        current = None
        size = None

        for lineno, line in enumerate(path.read_text(errors='replace')
                                      .splitlines(), 1):
            match = NONMATCHING.match(line)
            if match:
                size = int(match.group(2), 0)
                continue

            match = GLABEL.match(line)
            if match:
                current = match.group(1)
                if JLABEL.search(current):
                    current = None
                    size = None
                    continue
                # splat emits a full-file disassembly and, for files already
                # being decompiled, a second copy per function under
                # nonmatchings/. Keep the latter: it is what tells us the
                # containing file has its headers and structs in place.
                previous = functions.get(current)
                if not (previous
                        and 'nonmatchings' in previous['source_file']):
                    functions[current] = {
                        'overlay': overlay,
                        'source_file': rel,
                        'source_location': 'L{}'.format(lineno),
                        'size_bytes': size,
                    }
                size = None
                continue

            match = JAL.match(line)
            if match and current:
                calls.append((current, match.group(1)))

    return functions, calls


def _load_graph():
    with open(GRAPH) as f:
        return json.load(f)


def _c_functions(graph):
    """Map a bare function name to the id of its existing C node."""
    result = {}
    for node in graph['nodes']:
        if node.get('_origin') == ORIGIN:
            continue
        label = node.get('label', '')
        if label.endswith('()'):
            result[label[:-2]] = node['id']
    return result


def _build(functions, calls, graph):
    c_funcs = _c_functions(graph)

    nodes = []
    links = []

    overlays = sorted({meta['overlay'] for meta in functions.values()})
    for overlay in overlays:
        nodes.append({
            'label': '{} (overlay)'.format(overlay),
            'file_type': 'code',
            'source_file': 'asm/{}'.format(overlay),
            'source_location': 'L1',
            '_origin': ORIGIN,
            'id': 'asm_overlay_{}'.format(overlay),
            'norm_label': '{} (overlay)'.format(overlay),
            'status': 'asm',
        })

    for name, meta in sorted(functions.items()):
        node_id = _node_id(meta['overlay'], name)
        nodes.append({
            'label': '{}()'.format(name),
            'file_type': 'code',
            'source_file': meta['source_file'],
            'source_location': meta['source_location'],
            '_origin': ORIGIN,
            'id': node_id,
            'norm_label': '{}()'.format(name.lower()),
            'overlay': meta['overlay'],
            'status': 'asm',
            'size_bytes': meta['size_bytes'],
        })
        links.append({
            'relation': 'contains',
            'context': 'overlay',
            'confidence': 'EXTRACTED',
            'source_file': meta['source_file'],
            'source_location': meta['source_location'],
            'weight': 1.0,
            '_origin': ORIGIN,
            'source': 'asm_overlay_{}'.format(meta['overlay']),
            'target': node_id,
            'confidence_score': 1.0,
        })

    seen = set()
    for caller, callee in calls:
        source = _node_id(functions[caller]['overlay'], caller)

        if callee in functions:
            target = _node_id(functions[callee]['overlay'], callee)
        elif callee in c_funcs:
            target = c_funcs[callee]
        else:
            # A psyq library routine or a symbol we have no node for.
            continue

        if (source, target) in seen or source == target:
            continue
        seen.add((source, target))

        links.append({
            'relation': 'calls',
            'context': 'call',
            'confidence': 'EXTRACTED',
            'source_file': functions[caller]['source_file'],
            'source_location': functions[caller]['source_location'],
            'weight': 1.0,
            '_origin': ORIGIN,
            'source': source,
            'target': target,
            'confidence_score': 1.0,
        })

    return nodes, links


def _worklist(functions, calls, limit, started_only, by_size):
    """Rank undecompiled functions by how little they depend on other
    undecompiled functions, then by size. Leaves match fastest."""
    pending = defaultdict(set)
    for caller, callee in calls:
        if callee in functions and callee != caller:
            pending[caller].add(callee)

    selected = functions
    if started_only:
        # Anything under nonmatchings/ is a hole in a file that already has
        # its headers and structs in place, so it is the cheapest to close.
        selected = {name: meta for name, meta in functions.items()
                    if 'nonmatchings' in meta['source_file']}

    if by_size:
        # Progress is measured in bytes, so when chasing a percentage the big
        # leaves are worth more than the easy ones.
        ranked = sorted(
            (kv for kv in selected.items() if not pending[kv[0]]),
            key=lambda kv: -(kv[1]['size_bytes'] or 0))
    else:
        ranked = sorted(
            selected.items(),
            key=lambda kv: (len(pending[kv[0]]),
                            kv[1]['size_bytes'] or 1 << 30))

    print('{:<34} {:<8} {:>7} {:>6}'.format(
        'FUNCTION', 'OVERLAY', 'BYTES', 'DEPS'))
    for name, meta in ranked[:limit]:
        print('{:<34} {:<8} {:>7} {:>6}'.format(
            name[:34], meta['overlay'],
            meta['size_bytes'] if meta['size_bytes'] else '?',
            len(pending[name])))

    leaves = sum(1 for name in selected if not pending[name])
    print('\n{} functions still in asm, {} of them leaves.'.format(
        len(selected), leaves))


def _merge(nodes, links, graph):
    graph['nodes'] = [n for n in graph['nodes'] if n.get('_origin') != ORIGIN]
    graph['links'] = [l for l in graph['links'] if l.get('_origin') != ORIGIN]

    known = {n['id'] for n in graph['nodes']} | {n['id'] for n in nodes}

    graph['nodes'].extend(nodes)
    graph['links'].extend(
        l for l in links if l['source'] in known and l['target'] in known)

    with open(GRAPH, 'w') as f:
        json.dump(graph, f)

    print('Merged: {} nodes, {} links total.'.format(
        len(graph['nodes']), len(graph['links'])))
    print('Run "graphify cluster-only ." to recluster.')


def _parse_args():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--merge', action='store_true',
                        help='write the result into graphify-out/graph.json')
    parser.add_argument('--limit', type=int, default=40,
                        help='worklist entries to show (default: %(default)s)')
    parser.add_argument('--started', action='store_true',
                        help='only the gaps in files already being decompiled')
    parser.add_argument('--by-size', action='store_true',
                        help='biggest leaves first, for chasing a percentage')

    return parser.parse_args()


def _main():
    args = _parse_args()

    functions, calls = _parse_asm()
    if not functions:
        raise SystemExit('No asm found. Run "make regenerate" first.')

    graph = _load_graph()

    if args.merge:
        nodes, links = _build(functions, calls, graph)
        _merge(nodes, links, graph)
    else:
        _worklist(functions, calls, args.limit, args.started,
                  args.by_size)


if __name__ == '__main__':
    _main()
