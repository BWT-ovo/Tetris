#ifndef TETROMINO_I_H
#define TETROMINO_I_H

#include "Tetromino.h"

// I 型方块 —— 4×1 水平长条（青色）
// 使用 4×4 矩阵表示，初始形状为第 2 行全填充的水平条。
// 旋转后在水平和竖直两种状态之间切换。
class TetrominoI : public Tetromino {
public:
    // 构造函数 —— 初始化 4×4 矩阵和青色
    TetrominoI();

    // 顺时针旋转 90°（调用基类 rotateMatrix）
    void rotate() override;
};

#endif // TETROMINO_I_H
