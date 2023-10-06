# cmigemo-module

[C/Migemo](https://github.com/koron/cmigemo)をGNU Emacsのダイナミックモジュールにします。
それを利用する[migemo.el](https://github.com/emacs-jp/migemo)の修正も含みます。

# Features

C/MigemoをEmacsのダイナミックモジュールとしてロードします。
検索を始めた瞬間に引っかかる感じを低減できているような気がします。

# Requirement

- GNU Emacs >= 27.1
- C/Migemo >= rel-1_2
- meson >= 1.2
- ninja >= 1.11
# Installation

```sh
meson setup builddir
meson compile -C builddir
meson install --no-rebuild -C builddir
git clone --recursive git@github.com:h2oota/cmigemo-module.git
cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DINSTALL_FLAT=1
cmake --build build
cmake --install build
```
 -DINSTALL_FLAT=1を設定すると{migemo.el,migemo.elc,migemo-cmigemo.so}を${CMAKE_INSTALL_PREFIX}の直下にインストールします。
未設定の場合は${CMAKE_INSTALL_PREFIX}/migemo/にインストールします。
${CMAKE_INSTALL_PREFIX}のデフォルトは/usr/local/share/emacs/site-lispです変更する場合は
cmake -B buildの実行に--install-prefix=/path/to/installを追加します。

# Usage

migemo-cmigemo-moduleのデフォルト設定はダイナミックモジュールのファイル名"cmigemo-module"になっていて、
これをロードして利用します。何らかの原因でmigemo-cmigemo-moduleがロードできない場合は従来通りの動作に
なります。migemo-cmigemo-moduleがnilに設定されている場合も従来の動作です。
migemo-cmigemo-moduleをカスタマイズするなどの方法で設定してください。

# Note


# Author

* 作成者 HIROSHI OOTA
* E-mail 2571874+h2oota@users.noreply.github.com

# License
cmigemo-module is under [MIT license](https://en.wikipedia.org/wiki/MIT_License).
