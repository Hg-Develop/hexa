# HeXa

## 概要

六角形のパズルです。

![hexa](https://user-images.githubusercontent.com/52535984/156352394-082541bc-c0f7-4ba2-9a6c-371951e31f07.png)

## 操作方法

- 左クリックでピースを掴む・放す
- 右クリック、d キーでピースを右回転
- a キーでピースを左回転
- shift キー + 回転操作で逆回転
- w キーでピース一覧を上に移動
- s キーでピース一覧を下に移動
- wasd の代わりにカーソルキーも使えます

ピースをグリッドに配置すると、一覧から消えます。
配置したピースを掴んで適当に放すと、一覧の一番下に追加されます。

## 画像の変更

source.png を変更すると、パズルの画像を変更できます。
ただし、サイズは 512\*512px でなければなりません。
要件を満たしていないと、起動時にエラーメッセージがでて終了します。

同じ外見のピースが複数ある、または回転しても外見が変わらないピースがあると、見かけ上完成しているのに完成にならないことがあります。
これは、内部の位置情報・回転情報が、正解と異なるためです。
このため、広範囲(128\*112px 以上が目安)の単色領域を持つ画像は適していません。

## ビルド方法

このソフトウェアは、32bit 版 Cygwin を対象に作成されました。

- このソフトウェアは、OpenGL、GLUT、glpng を使用しています。導入していない場合は、先に導入してください。
- `gcc -I/usr/include/opengl -o hexa hexa.c -lglpng -lglut32 -lglu32 -lopengl32` でビルドできます。
- ビルド時に GLUT が警告を出すかもしれません。無視しても大丈夫です。
- (Makefile を用意したかったのですが、当方の環境だと make が動きませんでした…)

## その他

- 作成当時の私はマクロを書くのが面白くなってしまったようで、コードが混沌としています。

## 謝辞

glpng の作成者、Ben Wyatt 氏に特別な感謝を表明いたします。