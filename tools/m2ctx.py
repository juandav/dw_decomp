#!/usr/bin/env python3

"""Generate a preprocessed C context file for m2c, decomp-permuter and
decomp.me. With no source arguments the context covers every header under
include/dw."""

import argparse
import os
import subprocess
import tempfile
from pathlib import Path

TOP = Path(__file__).resolve().parent.parent

INCLUDES = ['-Iexternal/psyq_headers/mw_lib41/include', '-Iinclude']

DEFAULT_HEADERS = sorted((TOP / 'include' / 'dw').glob('*.h'))


def __build_source(sources):
    if sources:
        return '\n'.join('#include "{}"'.format(Path(s).resolve())
                         for s in sources) + '\n'

    lines = ['#include "common.h"']
    lines += ['#include "dw/{}"'.format(h.name) for h in DEFAULT_HEADERS]

    return '\n'.join(lines) + '\n'


def __preprocess(source, cc):
    with tempfile.NamedTemporaryFile('w', suffix='.c', delete=False) as f:
        f.write(source)
        path = f.name

    try:
        return subprocess.run([cc, '-E', '-P'] + INCLUDES + [path],
                              cwd=TOP, check=True, capture_output=True,
                              text=True).stdout
    finally:
        os.unlink(path)


def __parse_args():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('sources', nargs='*',
                        help='headers or C files to build the context from')
    parser.add_argument('-o', '--output', default='build/ctx.c',
                        help='output path (default: %(default)s)')

    return parser.parse_args()


def __main():
    args = __parse_args()

    cc = os.environ.get('TOOLCHAIN', 'mipsel-linux-gnu-') + 'gcc'

    context = __preprocess(__build_source(args.sources), cc)

    output = Path(args.output)
    if not output.is_absolute():
        output = TOP / output

    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(context)

    print('Wrote {} ({} lines)'.format(output, context.count('\n')))


if __name__ == '__main__':
    __main()
