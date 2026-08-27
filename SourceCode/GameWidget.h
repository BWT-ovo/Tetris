#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

#include "GameController.h"
#include "GameCanvas.h"
#include "NextPiecePreview.h"

// 主游戏窗口 —— 组合画布、预览、分数面板和控制按钮
// 水平布局：左侧 GameCanvas（301×601）+ 右侧信息面板（160px）。
// 将 GameController 的 5 个信号连接到 UI 更新槽，
// 将 3 个按钮的 clicked 信号连接到控制器的对应槽。
class GameWidget : public QWidget {
    Q_OBJECT
public:
    explicit GameWidget(GameController* controller, QWidget* parent = nullptr);

private slots:
    void onBoardChanged();        // 触发画布重绘
    void onScoreChanged(int score, int level, int linesCleared, int highScore);
    void onNextPieceChanged(PieceType type);
    void onGameOver(int finalScore);

private:
    void setupUI();               // 构建 UI 布局
    void applyStyles();           // 应用暗色主题样式表

    GameController*   m_controller;
    GameCanvas*       m_canvas;
    NextPiecePreview* m_preview;

    QLabel*      m_scoreLabel;
    QLabel*      m_levelLabel;
    QLabel*      m_linesLabel;
    QLabel*      m_highScoreLabel;
    QLabel*      m_gameOverLabel;
    QPushButton* m_startBtn;
    QPushButton* m_pauseBtn;
    QPushButton* m_resetBtn;
};

#endif // GAMEWIDGET_H
