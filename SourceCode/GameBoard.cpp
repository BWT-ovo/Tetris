#include "GameBoard.h"
#include <cstring>

// 构造函数 —— 将整个棋盘网格清零
GameBoard::GameBoard()
{
    clear();
}

// 清空棋盘 —— 用 memset 快速将整个网格置零
void GameBoard::clear()
{
    std::memset(m_grid, 0, sizeof(m_grid));
}

// 碰撞检测 —— 验证方块是否能放在棋盘位置 (px, py)
// 遍历方块形状矩阵的每个单元格：
// - 空单元格跳过
// - 填充单元格检查是否越界（左/右/下方）
// - 位于棋盘上方的单元格（boardY < 0）允许通过（尚未进入可见区）
// - 可见区内的单元格检查是否与已有方块重叠
bool GameBoard::canPlace(const Tetromino& piece, int px, int py) const
{
    const auto& shape = piece.shape();
    int size = piece.matrixSize();

    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            if (shape[row][col] == 0)
                continue;   // 空单元格跳过

            int boardX = px + col;
            int boardY = py + row;

            // 越界检查：左侧、右侧、下方出界
            if (boardX < 0 || boardX >= WIDTH)
                return false;
            if (boardY >= HEIGHT)        // 超出底部边界
                return false;

            // 位于棋盘上方的单元格允许通过（尚未进入可见区，不会碰撞）
            if (boardY < 0)
                continue;

            // 与已有方块重叠检查
            if (m_grid[boardY][boardX] != 0)
                return false;
        }
    }
    return true;
}

// 将方块合并到棋盘上
// 遍历方块形状矩阵，将填充单元格写入棋盘的对应位置。
// 仅写入棋盘可见范围内的单元格。
// 存储值为方块类型 ID + 1（以便 0 表示空）。
void GameBoard::place(const Tetromino& piece, int px, int py)
{
    const auto& shape = piece.shape();
    int size = piece.matrixSize();

    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            if (shape[row][col] == 0)
                continue;

            int boardX = px + col;
            int boardY = py + row;

            // 只写入可见区域内的单元格
            if (boardX >= 0 && boardX < WIDTH &&
                boardY >= 0 && boardY < HEIGHT) {
                m_grid[boardY][boardX] = static_cast<int>(piece.type()) + 1;
            }
        }
    }
}

// 消除所有满行
// 从底部向顶部扫描每一行：
// - 如果某行全部填满，调用 removeRow() 删除该行
// - 删除后使用 ++row 重新检查同一索引（因为上方行已下移）
// - 返回总共消除的行数
int GameBoard::clearLines()
{
    int cleared = 0;

    // 从底部向顶部扫描（行索引越大越靠近底部）
    for (int row = HEIGHT - 1; row >= 0; --row) {
        if (isRowFull(row)) {
            removeRow(row);
            ++cleared;
            // 重新检查当前行（原来上一行的内容已下移到这里）
            ++row;
        }
    }
    return cleared;
}

// 判断某一行是否全部填满
// 检查该行的每一列，只要有任一列为空（0）则返回 false。
bool GameBoard::isRowFull(int row) const
{
    for (int col = 0; col < WIDTH; ++col) {
        if (m_grid[row][col] == 0)
            return false;
    }
    return true;
}

// 删除指定行，将上方内容整体下移
// 从被删除行开始，逐行将上一行的内容复制到当前行。
// 最顶行（第 0 行）变为全空。
void GameBoard::removeRow(int row)
{
    // 将 row 上方的每一行下移一行
    for (int r = row; r > 0; --r) {
        for (int col = 0; col < WIDTH; ++col) {
            m_grid[r][col] = m_grid[r - 1][col];
        }
    }
    // 最顶行变为空
    for (int col = 0; col < WIDTH; ++col) {
        m_grid[0][col] = 0;
    }
}
