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
git clone --recursive git@github.com:h2oota/cmigemo-module.git
cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DINSTALL_FLAT=1
cmake --build build
cmake --install build
```
 -DINSTALL_FLAT=1を設定すると{migemo.el,migemo.elc,migemo-cmigemo.so}を${CMAKE_INSTALL_PREFIX}の直下にインストールします。
未設定の場合は${CMAKE_INSTALL_PREFIX}/migemo/にインストールします。
CMAKE_INSTALL_PREFIXのデフォルトは/usr/local/share/emacs/site-lispです変更する場合は
cmake -B buildの実行に--install-prefix=/path/to/installを追加します。

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
