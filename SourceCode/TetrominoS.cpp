#include "TetrominoS.h"


TetrominoS::TetrominoS()
    : Tetromino(PieceType::S, 3)
{
    m_color = QColor(0, 220, 80);   // Green 绿色
    m_shape = {
        {0, 1, 1},
        {1, 1, 0},
        {0, 0, 0}
    };
}


void TetrominoS::rotate()
{
    rotateMatrix();
}
