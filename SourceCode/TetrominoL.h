#ifndef TETROMINO_L_H
#define TETROMINO_L_H

#include "Tetromino.h"

// L 型方块 —— L 字形（橙色），3×3 矩阵
class TetrominoL : public Tetromino {
public:
    // 构造函数 —— 初始化 L 形矩阵和橙色
    TetrominoL();

    // 顺时针旋转 90°
    void rotate() override;
};

#endif // TETROMINO_L_H
