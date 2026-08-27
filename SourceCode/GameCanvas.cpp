#include "GameCanvas.h"
#include "GameController.h"
#include "BlockColors.h"

#include <QPainter>
#include <QKeyEvent>
#include <QLinearGradient>

// ============================================================================
//  构造与尺寸
// ============================================================================

GameCanvas::GameCanvas(GameController* controller, QWidget* parent)
    : QWidget(parent)
    , m_controller(controller)
{
    setFixedSize(COLS * CELL_SIZE + 1, ROWS * CELL_SIZE + 1);
    setFocusPolicy(Qt::StrongFocus);
}

QSize GameCanvas::sizeHint() const
{
    return {COLS * CELL_SIZE + 1, ROWS * CELL_SIZE + 1};
}

// ============================================================================
//  绘制事件
// ============================================================================

void GameCanvas::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 背景
    painter.fillRect(rect(), QColor(30, 30, 30));

    // 网格线
    painter.setPen(QPen(QColor(50, 50, 50), 1));
    for (int c = 0; c <= COLS; ++c)
        painter.drawLine(c * CELL_SIZE, 0, c * CELL_SIZE, ROWS * CELL_SIZE);
    for (int r = 0; r <= ROWS; ++r)
        painter.drawLine(0, r * CELL_SIZE, COLS * CELL_SIZE, r * CELL_SIZE);

    // 固定方块 + 当前活动方块
    QVector<QVector<int>> grid = m_controller->getDisplayGrid();
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c)
            if (grid[r][c] != 0)
                drawCell(painter, c, r, grid[r][c]);

    // 消行动画：逐帧收缩白光
    if (m_controller->isClearAnimActive()) {
        float prog = m_controller->clearAnimProgress();
        const QVector<int>& animRows = m_controller->animatingRows();
        for (int rowIdx : animRows) {
            int y = rowIdx * CELL_SIZE;
            for (int cx = 0; cx < COLS; ++cx) {
                int cellX = cx * CELL_SIZE;
                QColor flash(255, 255, 255,
                             static_cast<int>(255 * (1.0f - prog)));
                painter.fillRect(cellX + 1, y + 1, CELL_SIZE - 2,
                                 CELL_SIZE - 2, flash);
                int shrink = static_cast<int>(prog * CELL_SIZE / 2);
                int sw = qMax(0, CELL_SIZE - 2 - shrink * 2);
                int sh = qMax(0, CELL_SIZE - 2 - shrink * 2);
                if (sw > 0 && sh > 0) {
                    painter.setPen(QPen(
                        QColor(200, 220, 255,
                               static_cast<int>(200 * (1.0f - prog))), 2));
                    painter.setBrush(Qt::NoBrush);
                    painter.drawRect(cellX + 1 + shrink, y + 1 + shrink, sw, sh);
                }
            }
        }
    }

    // 幽灵块（半透明 + 虚线边框）
    if (m_controller->state() == GameController::GameState::PLAYING) {
        const Tetromino* piece = m_controller->currentPiece();
        if (piece) {
            int ghostY = m_controller->getGhostY();
            if (ghostY != m_controller->currentY()) {
                const auto& shape = piece->shape();
                int size = piece->matrixSize();
                int px = m_controller->currentX();
                QColor base = colorForType(piece->type());
                const int r = 6;
                for (int row = 0; row < size; ++row) {
                    for (int col = 0; col < size; ++col) {
                        if (shape[row][col] == 0) continue;
                        int bx = px + col, by = ghostY + row;
                        if (by >= 0 && by < ROWS && bx >= 0 && bx < COLS) {
                            int x = bx * CELL_SIZE, y = by * CELL_SIZE;
                            painter.setPen(Qt::NoPen);
                            painter.setBrush(QColor(base.red(), base.green(),
                                                    base.blue(), 50));
                            painter.drawRoundedRect(x + 1, y + 1,
                                    CELL_SIZE - 2, CELL_SIZE - 2, r, r);
                            QPen ghostPen(base.lighter(150));
                            ghostPen.setStyle(Qt::DotLine);
                            painter.setPen(ghostPen);
                            painter.setBrush(Qt::NoBrush);
                            painter.drawRoundedRect(x + 1, y + 1,
                                    CELL_SIZE - 2, CELL_SIZE - 2, r, r);
                        }
                    }
                }
            }
        }
    }

    // 游戏结束遮罩
    if (m_controller->state() == GameController::GameState::GAME_OVER) {
        painter.fillRect(rect(), QColor(0, 0, 0, 140));
        painter.setPen(Qt::white);
        QFont f = painter.font();
        f.setPointSize(22); f.setBold(true);
        painter.setFont(f);
        painter.drawText(rect(), Qt::AlignCenter,
                         QStringLiteral("GAME OVER"));
    }
    // 暂停遮罩
    else if (m_controller->state() == GameController::GameState::PAUSED) {
        painter.fillRect(rect(), QColor(0, 0, 0, 120));
        painter.setPen(Qt::white);
        QFont f = painter.font();
        f.setPointSize(22); f.setBold(true);
        painter.setFont(f);
        painter.drawText(rect(), Qt::AlignCenter,
                         QStringLiteral("PAUSED"));
    }
}

// ============================================================================
//  方块单元格绘制（圆角 + 渐变高光）
// ============================================================================

void GameCanvas::drawCell(QPainter& painter, int col, int row, int colorId)
{
    int x = col * CELL_SIZE, y = row * CELL_SIZE;
    QColor base = colorForId(colorId);
    const int r = 6;

    painter.setPen(Qt::NoPen);
    painter.setBrush(base);
    painter.drawRoundedRect(x + 1, y + 1, CELL_SIZE - 2, CELL_SIZE - 2, r, r);

    // 顶部高光 → 底部阴影渐变
    QLinearGradient grad(x, y, x, y + CELL_SIZE);
    grad.setColorAt(0.0, QColor(255, 255, 255, 70));
    grad.setColorAt(0.4, QColor(255, 255, 255, 0));
    grad.setColorAt(0.6, QColor(0, 0, 0, 0));
    grad.setColorAt(1.0, QColor(0, 0, 0, 50));
    painter.setBrush(grad);
    painter.drawRoundedRect(x + 1, y + 1, CELL_SIZE - 2, CELL_SIZE - 2, r, r);

    // 深色描边
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(base.darker(140), 1));
    painter.drawRoundedRect(x + 1, y + 1, CELL_SIZE - 2, CELL_SIZE - 2, r, r);
}

// ============================================================================
//  键盘事件
// ============================================================================

void GameCanvas::keyPressEvent(QKeyEvent* event)
{
    if (!m_controller) { QWidget::keyPressEvent(event); return; }

    switch (event->key()) {
        case Qt::Key_Left:   m_controller->moveLeft();     break;
        case Qt::Key_Right:  m_controller->moveRight();    break;
        case Qt::Key_Down:   m_controller->moveDown();     break;
        case Qt::Key_Up:     m_controller->rotatePiece();  break;
        case Qt::Key_Space:  m_controller->hardDrop();     break;
        default:             QWidget::keyPressEvent(event); break;
    }
}
