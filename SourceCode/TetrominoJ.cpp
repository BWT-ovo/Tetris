#include "TetrominoJ.h"


TetrominoJ::TetrominoJ()
    : Tetromino(PieceType::J, 3)
{
    m_color = QColor(0, 100, 220);   // Blue 蓝色
    m_shape = {
        {1, 0, 0},
        {1, 1, 1},
        {0, 0, 0}
    };
}


void TetrominoJ::rotate()
{
    rotateMatrix();
}
