#include "NextPiecePreview.h"
#include "BlockColors.h"

#include <QPainter>
#include <QLinearGradient>

// 工厂方法需要的具体方块类
#include "TetrominoI.h"
#include "TetrominoO.h"
#include "TetrominoT.h"
#include "TetrominoL.h"
#include "TetrominoJ.h"
#include "TetrominoS.h"
#include "TetrominoZ.h"

NextPiecePreview::NextPiecePreview(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(130, 110);
}

QSize NextPiecePreview::sizeHint() const
{
    return {130, 110};
}

// 工厂方法：根据类型枚举创建对应的具体方块对象
void NextPiecePreview::setPieceType(PieceType type)
{
    switch (type) {
        case PieceType::I: m_previewPiece = std::make_unique<TetrominoI>(); break;
        case PieceType::O: m_previewPiece = std::make_unique<TetrominoO>(); break;
        case PieceType::T: m_previewPiece = std::make_unique<TetrominoT>(); break;
        case PieceType::L: m_previewPiece = std::make_unique<TetrominoL>(); break;
        case PieceType::J: m_previewPiece = std::make_unique<TetrominoJ>(); break;
        case PieceType::S: m_previewPiece = std::make_unique<TetrominoS>(); break;
        case PieceType::Z: m_previewPiece = std::make_unique<TetrominoZ>(); break;
    }
    update();
}

void NextPiecePreview::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 背景和边框
    painter.fillRect(rect(), QColor(40, 40, 40));
    painter.setPen(QPen(QColor(80, 80, 80), 1));
    painter.drawRoundedRect(rect().adjusted(0, 0, -1, -1), 4, 4);

    if (!m_previewPiece) return;

    const auto& shape = m_previewPiece->shape();
    int size = m_previewPiece->matrixSize();
    QColor color = colorForType(m_previewPiece->type());

    int previewCell = 22;
    int totalW = size * previewCell, totalH = size * previewCell;
    int offsetX = (width() - totalW) / 2, offsetY = (height() - totalH) / 2;
    const int pr = 4;

    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            if (shape[r][c] == 0) continue;
            int bx = offsetX + c * previewCell, by = offsetY + r * previewCell;

            // 圆角主体填充
            painter.setPen(Qt::NoPen);
            painter.setBrush(color);
            painter.drawRoundedRect(bx + 1, by + 1, previewCell - 2,
                                    previewCell - 2, pr, pr);

            // 渐变高光
            QLinearGradient grad(bx, by, bx, by + previewCell);
            grad.setColorAt(0.0, QColor(255, 255, 255, 60));
            grad.setColorAt(0.5, QColor(255, 255, 255, 0));
            grad.setColorAt(1.0, QColor(0, 0, 0, 40));
            painter.setBrush(grad);
            painter.drawRoundedRect(bx + 1, by + 1, previewCell - 2,
                                    previewCell - 2, pr, pr);

            // 深色描边
            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen(color.darker(140), 1));
            painter.drawRoundedRect(bx + 1, by + 1, previewCell - 2,
                                    previewCell - 2, pr, pr);
        }
    }
}
