#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "Tetromino.h"

// 管理 20×10 固定方块网格的类
// 负责碰撞检测、将方块合并到网格中、清除已完成的行。
class GameBoard {
public:
    static constexpr int WIDTH  = 10;   // 棋盘宽度（列数）
    static constexpr int HEIGHT = 20;   // 棋盘高度（行数）

    // 构造函数 —— 初始化一个空棋盘（全部清零）
    GameBoard();

    // 清空棋盘 —— 将所有单元格重置为 0（空）
    void clear();

    // 碰撞检测 —— 检查方块能否放置在指定位置
    bool canPlace(const Tetromino& piece, int px, int py) const;

    // 将方块固定到棋盘上，将方块形状矩阵中的填充单元格写入棋盘对应位置。
    void place(const Tetromino& piece, int px, int py);

    // 扫描并清除所有已填满的行
    // 从底部向顶部逐行检查，满行则消除并将上方所有行下移。
    int clearLines();

    // 读取单个单元格的值
    int cell(int row, int col) const { return m_grid[row][col]; }

private:
    int m_grid[HEIGHT][WIDTH];   // 棋盘网格 [行][列]，0 表示空

    // 检查指定行是否已填满
    bool isRowFull(int row) const;

    // 删除指定行，将上方所有行下移一行
    // 第 0 行以上的行变为空行（全 0）。
    void removeRow(int row);
};

#endif // GAMEBOARD_H
