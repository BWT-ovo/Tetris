#ifndef TETROMINO_J_H
#define TETROMINO_J_H

#include "Tetromino.h"

// J 型方块 —— J 字形（蓝色），3×3 矩阵
class TetrominoJ : public Tetromino {
public:
    // 构造函数 —— 初始化 J 形矩阵和蓝色
    TetrominoJ();

    // 顺时针旋转 90°
    void rotate() override;
};

#endif // TETROMINO_J_H
