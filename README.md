# Parallel Prime Finder (C, pthread)

このプログラムは、C言語と `pthread` を用いて **マルチスレッドで素数を計算**するサンプルです。  
並列処理、排他制御、動的メモリなど C の特徴的な機能を実装しています。

---

## 🚀 実行方法

### 1. コンパイル
```bash
gcc prime_parallel.c -o prime_parallel -lpthread -lm
```
-lpthread: pthreadライブラリをリンク
-lm: mathライブラリをリンク（sqrt関数で使用）

### 2. 実行
```bash
./prime_parallel 100000
```
→ 1〜100000 の範囲で素数を探索します。
