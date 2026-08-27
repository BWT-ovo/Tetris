#ifndef TETROMINO_T_H
#define TETROMINO_T_H

#include "Tetromino.h"

// T 型方块 —— T 字形（紫色），3×3 矩阵
class TetrominoT : public Tetromino {
public:
    // 构造函数 —— 初始化 T 形矩阵和紫色
    TetrominoT();

    // 顺时针旋转 90°
    void rotate() override;
};

#endif // TETROMINO_T_H
