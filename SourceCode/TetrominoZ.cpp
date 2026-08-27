#include "TetrominoZ.h"


TetrominoZ::TetrominoZ()
    : Tetromino(PieceType::Z, 3)
{
    m_color = QColor(220, 50, 50);   // Red 红色
    m_shape = {
        {1, 1, 0},
        {0, 1, 1},
        {0, 0, 0}
    };
}


void TetrominoZ::rotate()
{
    rotateMatrix();
}
