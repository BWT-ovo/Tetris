#ifndef TETROMINO_H
#define TETROMINO_H

#include <QVector>
#include <QColor>

// 方块类型枚举 —— 标识 7 种标准俄罗斯方块
enum class PieceType {
    I, O, T, L, J, S, Z
};

// 所有方块的抽象基类
// 定义方块的公共属性
class Tetromino {
public:
    // 构造函数 —— 初始化方块类型和矩阵尺寸
    // type  方块类型枚举值
    // matrixSize  正方形矩阵的边长（2、3 或 4）
    Tetromino(PieceType type, int matrixSize);

    // 虚析构函数 —— 确保派生类正确析构
    virtual ~Tetromino();

    // ========== 纯虚函数接口 ==========

    // 旋转方块（纯虚函数）
    // 每个具体方块类必须重写此函数，实现各自的旋转行为。
    // O 型方块重写为空操作（旋转不变），其余类型调用 rotateMatrix()。
    virtual void rotate() = 0;

    // ========== 属性访问器 ==========

    // 获取当前形状矩阵（只读引用）
    const QVector<QVector<int>>& shape() const { return m_shape; }

    // 获取矩阵边长
    int matrixSize() const { return m_matrixSize; }

    // 获取方块颜色
    QColor color() const { return m_color; }

    // 获取方块类型
    PieceType type() const { return m_type; }

protected:
    QVector<QVector<int>> m_shape;   // 二维形状矩阵（1 = 填充，0 = 空）
    int m_matrixSize;                // 正方形矩阵维度（2、3 或 4）
    QColor m_color;                  // 渲染颜色
    PieceType m_type;                // 方块类型标识

    // 通用矩阵旋转辅助函数 —— 将 m_shape 顺时针旋转 90°
    // 算法：原坐标 (i, j) 映射到新坐标 (j, N-1-i)
    // 除 O 型方块外，所有派生类的 rotate() 均调用此函数。
    void rotateMatrix();
};

#endif // TETROMINO_H
