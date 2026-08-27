#ifndef BLOCKCOLORS_H
#define BLOCKCOLORS_H

#include <QColor>
#include "Tetromino.h"

// 根据颜色 ID（1~7）返回对应的 QColor
inline QColor colorForId(int id)
{
    switch (id) {
        case 1: return QColor(0,   240, 240);   // I — 青色
        case 2: return QColor(255, 255, 0);     // O — 黄色
        case 3: return QColor(160, 0,   200);   // T — 紫色
        case 4: return QColor(255, 160, 0);     // L — 橙色
        case 5: return QColor(0,   100, 220);   // J — 蓝色
        case 6: return QColor(0,   220, 80);    // S — 绿色
        case 7: return QColor(220, 50,  50);    // Z — 红色
        default: return QColor(60, 60, 60);     // 空 / 未知 — 深灰
    }
}

// 根据方块类型枚举返回对应颜色
inline QColor colorForType(PieceType type)
{
    return colorForId(static_cast<int>(type) + 1);
}

#endif // BLOCKCOLORS_H
