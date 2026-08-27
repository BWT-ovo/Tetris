#ifndef GAMECANVAS_H
#define GAMECANVAS_H

#include <QWidget>

class GameController;

// 游戏画布 —— 负责渲染游戏网格和处理键盘输入
// 所有绘制由 paintEvent() 中的 QPainter 完成。
// 键盘事件翻译为 GameController 的方法调用。
class GameCanvas : public QWidget {
    Q_OBJECT
public:
    static constexpr int CELL_SIZE  = 30;
    static constexpr int COLS       = 10;
    static constexpr int ROWS       = 20;

    explicit GameCanvas(GameController* controller, QWidget* parent = nullptr);

    QSize sizeHint() const override;

protected:
    // 绘制事件 —— 网格、方块、消行动画、幽灵块、结束/暂停遮罩
    void paintEvent(QPaintEvent* event) override;

    // 键盘按下事件 —— ← → ↓ ↑ Space 映射到控制器方法
    void keyPressEvent(QKeyEvent* event) override;

private:
    // 绘制单个圆角方块单元格（含渐变高光）
    void drawCell(QPainter& painter, int col, int row, int colorId);

    GameController* m_controller;
};

#endif // GAMECANVAS_H
