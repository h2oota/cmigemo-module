# cmigemo-module

[C/Migemo](https://github.com/koron/cmigemo)をGNU Emacsのダイナミックモジュールにします。
それを利用する[migemo.el](https://github.com/emacs-jp/migemo)の修正も含みます。

# DEMO


# Features

C/MigemoをEmacsのダイナミックモジュールとしてロードします。
検索を始めた瞬間に引っかかる感じを低減できているような気がします。

# Requirement

* GNU Emacs 28
* C/Migemo rel-1_2
* CMake 3.24

# Installation

```sh
cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DINSTALL_FLAT=1 --install-prefix=/usr/local/share/emacs/site-lisp
cmake --build build
cmake --install build
```
 -DINSTALL_FLAT=1を設定すると{migemo.el,migemo.elc,migemo-cmigemo.so}をinstall-prefixの直下に
インストールします。設定していない場合はinstall-prefixの下にmigemoディレクトリを作成し、その下に
インストールします。

# Usage

migemo-cmigemo-moduleにファイル名(migemo-cmigemo)を設定して、(migemo-init)を呼び出すと
migemo-cmigemoをロードして、利用します。何らかの原因でmigemo-cmigemoがロードできない場合は
従来通りの動作になります。
migemo-cmigemo-moduleをカスタマイズするなどの方法で設定してください。

# Note


# Author

* 作成者 HIROSHI OOTA
* E-mail 2571874+h2oota@users.noreply.github.com

# License
cmigemo-module is under [MIT license](https://en.wikipedia.org/wiki/MIT_License).
