# -*- mode: meson;-*-
project('cmigemo-module','c',
        meson_version: '>=1.2.0',
        default_options: run_command(
          ['scripts/compute-default.py'],
          check: true).stdout().strip().split('\n'),
        version: '1.0')
fsmod = import('fs')

emacs = get_option('emacs')
lisp_dir = get_option('lispdir')

if meson.is_cross_build()
  error('cross build is not allowed')
else
  gzip = find_program('gzip', disabler: true, required: false)
  incdir = run_command(emacs, '-q', '-batch',
                       '--eval', '(let ((h (expand-file-name "../include/emacs-module.h" data-directory))) (if (file-exists-p h) (princ (file-name-directory h)) (error "emacs-module: not found")))', check: true).stdout().strip()
endif

el_files = []
elc_files = []
foreach el: ['migemo/migemo.el']
  el_files += fsmod.copyfile(
    el,
    install: true,
    install_dir: lisp_dir,
    install_mode: 'rw-r--r--')
  elc_files += fsmod.name(fsmod.replace_suffix(el, '.elc'))
endforeach

migemo = custom_target(
  'migemo',
  input: el_files,
  output: elc_files,
  command: [emacs, '-q', '--batch',
            '--eval', '(require \'cl-extra)',
            '--eval', '(setq load-path (cons "@OUTDIR@" load-path))',
            '--eval', '(setq depends nil)',
            '--eval', '(dolist (el argv) (let ((pred (lambda (f) (file-newer-than-file-p (concat (file-name-sans-extension el) ".elc") f)))) (unless (cl-every pred (cons el depends)) (message "compiling %s" el) (byte-compile-file el))))', '@INPUT@'],
  install: true,
  install_dir: lisp_dir,
  install_mode: 'rw-r--r--',
  build_by_default: true)

cmigemo_module = shared_module(
  'cmigemo-module',
  files('src/emacs-module.c'),
  include_directories: incdir,
  dependencies: meson.get_compiler('c').find_library('migemo', required : true),
  install_dir: lisp_dir,
  install_mode: 'rw-r--r--',
  name_prefix: '',
  install: true)
