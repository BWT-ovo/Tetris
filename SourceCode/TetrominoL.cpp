#include "TetrominoL.h"


TetrominoL::TetrominoL()
    : Tetromino(PieceType::L, 3)
{
    m_color = QColor(255, 160, 0);   // Orange 橙色
    m_shape = {
        {0, 0, 1},
        {1, 1, 1},
        {0, 0, 0}
    };
}

void TetrominoL::rotate()
{
    rotateMatrix();
}
