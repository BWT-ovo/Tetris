#include "Tetromino.h"


Tetromino::Tetromino(PieceType type, int matrixSize)
    : m_matrixSize(matrixSize)
    , m_type(type)
{
    // 分配正方形矩阵并用 0 填充
    m_shape.resize(matrixSize);
    for (int i = 0; i < matrixSize; ++i) {
        m_shape[i].resize(matrixSize);
        m_shape[i].fill(0);
    }
}

// 虚析构函数 —— 使用默认实现

Tetromino::~Tetromino() = default;


void Tetromino::rotateMatrix()
{
    // 创建临时矩阵存储旋转后的结果
    QVector<QVector<int>> rotated(m_matrixSize,
                                   QVector<int>(m_matrixSize, 0));
    for (int i = 0; i < m_matrixSize; ++i) {
        for (int j = 0; j < m_matrixSize; ++j) {
            rotated[j][m_matrixSize - 1 - i] = m_shape[i][j];
        }
    }
    m_shape = rotated;
}
