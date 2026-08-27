#ifndef TETROMINO_S_H
#define TETROMINO_S_H

#include "Tetromino.h"

// S 型方块 —— S 字形（绿色），3×3 矩阵
class TetrominoS : public Tetromino {
public:
    // 构造函数 —— 初始化 S 形矩阵和绿色
    TetrominoS();

    // 顺时针旋转 90°
    void rotate() override;
};

#endif // TETROMINO_S_H
