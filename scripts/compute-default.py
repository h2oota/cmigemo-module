#!/usr/bin/env python
# -*- mode: python; coding: utf-8; -*-
import platform
import pathlib
import typing
import sys
import os
import argparse
import subprocess

def find_darwin_apps(directory: pathlib.Path=pathlib.Path('/Applications')) -> typing.Generator[pathlib.Path, None, None]:
    for ent in directory.glob('*'):
        if ent.name.startswith('.'):
            continue
        elif ent.suffix == '.app':
            yield ent
        else:
            yield from find_darwin_apps(ent)

def error_exit(msg: str, *args) -> typing.NoReturn:
    print(msg.format(*args), file=sys.stderr)
    sys.exit()

def main():
    options = set(('prefix', 'emacs', 'lispdir',))
    parser = argparse.ArgumentParser()
    parser.add_argument(dest='opt_names', metavar='OPTION_NAMES', help='option names which would be computes', nargs='*')

    intstall_global = os.getenv('INSTALL_GLOBAL')

    args = parser.parse_args()
    opsys = platform.system().lower()

    if not options >= set(args.opt_names):
        for opt in args.opt_names:
            print('$$$', opt)
            if opt not in options:
                error_exit('{}: unknown option', opt)

    if opsys == 'windows':
        for dir in ('C:/Emacs', 'C:/Emacs/bin',
                    'C:/Program Files/Emacs/', 'C:/Program Files/Emacs/bin/',
                    'C:/Program Files/GNU/', 'C:/Program Files/GNU/bin/',):
            emacs  = pathlib.Path(dir) / "emacs.exe"
            if emacs.exists():
                break
        else:
            error_exit('emacs not found')
    elif opsys == 'darwin':
        for app in find_darwin_apps():
            emacs = app / 'Contents/MacOS/emacs'
            if emacs.exists():
                break
        else:
            error_exit('emacs not found')
    else:
        for dir in os.getenv('PATH').split(':'):
            emacs = pathlib.Path(dir) / 'emacs'
            if emacs.exists():
                break
        else:
            error_exit('emacs not found')

    assert(emacs is not None)

    # emacs, prefix, lispdir

    if len(args.opt_names) == 0 or 'emacs' in args.opt_names:
        print('emacs={}'.format(str(emacs)))

    if len(args.opt_names) == 0 or 'prefix' in args.opt_names:
        if intstall_global:
            exp = '(princ (expand-file-name ".." lisp-directory))'
        else:
            exp = '(princ (expand-file-name user-emacs-directory))'

        emacs_proc = subprocess.Popen(
            (str(emacs), '-q', '--batch', '--eval', exp,),
            stdout=subprocess.PIPE, encoding='utf-8')

        print('prefix={}'.format(emacs_proc.stdout.read()))

    if len(args.opt_names) == 0 or 'lispdir' in args.opt_names:
        if intstall_global:
            print('lispdir=site-lisp')
        else:
            print('lispdir=lisp')

main()
