#include "TetrominoI.h"


TetrominoI::TetrominoI()
    : Tetromino(PieceType::I, 4)
{
    m_color = QColor(0, 240, 240);   // Cyan 青色
    m_shape = {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };
}


void TetrominoI::rotate()
{
    rotateMatrix();
}
