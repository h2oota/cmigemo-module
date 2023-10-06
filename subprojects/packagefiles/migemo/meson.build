# -*- mode: meson;-*-
project('migemo')
fsmod = import('fs')

emacs = get_option('emacs')
lisp_dir = get_option('lispdir')

el_files = ['migemo.el']

cpied_el = []
elc_files = []
foreach el: el_files
  cpied_el += fsmod.copyfile(
    el,
    install: true,
    install_dir: lisp_dir,
    install_mode: 'rw-r--r--')
  elc_files += fsmod.name(fsmod.replace_suffix(el, '.elc'))
endforeach

elc = custom_target(
  'migemo-el',
  input: cpied_el,
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
