# -*- mode: meson;-*-
project('cmigemo-module','c',
        meson_version: '>=1.2.0',
        default_options: run_command(
          ['scripts/compute-default.py'],
          check: true).stdout().strip().split('\n'),
        version: '1.0')
emacs = get_option('emacs')
lispdir = get_option('lispdir')

# find eemacs
if meson.is_cross_build()
  error('cross build is not allowed')
else
  gzip = find_program('gzip', disabler: true, required: false)
  incdir = run_command(emacs, '-q', '-batch',
                       '--eval', '(let ((h (expand-file-name "../include/emacs-module.h" data-directory))) (if (file-exists-p h) (princ (file-name-directory h)) (error "emacs-module: not found")))', check: true).stdout().strip()
endif

migemo = subproject(
  'migemo',
  default_options: {'emacs': emacs, 'lispdir': lispdir},
  required: true)

cmigemo_module = shared_module(
  'cmigemo-module',
  files('src/emacs-module.c'),
  include_directories: incdir,
  dependencies: meson.get_compiler('c').find_library('migemo', required : true),
  install_dir: lispdir,
  install_mode: 'rw-r--r--',
  name_prefix: '',
  install: true)
