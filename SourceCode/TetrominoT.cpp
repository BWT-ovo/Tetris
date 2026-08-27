#include "TetrominoT.h"


TetrominoT::TetrominoT()
    : Tetromino(PieceType::T, 3)
{
    m_color = QColor(160, 0, 200);   // Purple 紫色
    m_shape = {
        {0, 1, 0},
        {1, 1, 1},
        {0, 0, 0}
    };
}

void TetrominoT::rotate()
{
    rotateMatrix();
}
