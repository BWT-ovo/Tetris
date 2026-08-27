#ifndef TETROMINO_O_H
#define TETROMINO_O_H

#include "Tetromino.h"

// O 型方块 —— 2×2 实心正方形（黄色）
// 旋转为无操作（空函数），因为 2×2 正方形在 90° 旋转下形状不变。
class TetrominoO : public Tetromino {
public:
    // 构造函数 —— 初始化 2×2 实心矩阵
    TetrominoO();

    // 旋转 —— 空操作（O 型方块旋转不变）
    void rotate() override;
};

#endif // TETROMINO_O_H
