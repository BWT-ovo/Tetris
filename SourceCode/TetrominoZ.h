#ifndef TETROMINO_Z_H
#define TETROMINO_Z_H

#include "Tetromino.h"

// Z 型方块 —— Z 字形（红色），3×3 矩阵
class TetrominoZ : public Tetromino {
public:
    // 构造函数 —— 初始化 Z 形矩阵和红色
    TetrominoZ();

    // 顺时针旋转 90°
    void rotate() override;
};

#endif // TETROMINO_Z_H
