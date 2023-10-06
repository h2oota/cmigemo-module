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
```

デフォルトインストールは~/.emacs.d/lispです。
環境変数`INSTALL_GLOBAL'を設定して、meson setupを実行すると、emacsのインストールディレクトリのsite-lispにインストールします。
```sh
INSTALL_GLOBAL=yes meson setup builddir
```

# Usage

カスタマイズ変数migemo-cmigemo-moduleのデフォルト設定はダイナミックモジュールのファイル名"cmigemo-module"です。
この変数がnil以外の場合、これをロードして利用します。何らかの原因でmigemo-cmigemo-moduleがロードできない場合は
従来通りの動作になります。

# Note


# Author

* 作成者 HIROSHI OOTA
* E-mail 2571874+h2oota@users.noreply.github.com

# License
cmigemo-module is under [MIT license](https://en.wikipedia.org/wiki/MIT_License).
