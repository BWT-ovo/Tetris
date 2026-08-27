#include "TetrominoO.h"

TetrominoO::TetrominoO()
    : Tetromino(PieceType::O, 2)
{
    m_color = QColor(255, 255, 0);   // Yellow 黄色
    m_shape = {
        {1, 1},
        {1, 1}
    };
}

void TetrominoO::rotate()
{
    // 空操作 —— O 型方块旋转不变
}
