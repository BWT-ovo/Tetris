#include "GameWidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFont>
#include <QApplication>

// ============================================================================
//  构造、UI 构建、样式
// ============================================================================

GameWidget::GameWidget(GameController* controller, QWidget* parent)
    : QWidget(parent)
    , m_controller(controller)
{
    setupUI();
    applyStyles();

    // 控制器信号 → UI 更新槽
    connect(m_controller, &GameController::boardChanged,
            this, &GameWidget::onBoardChanged);
    connect(m_controller, &GameController::scoreChanged,
            this, &GameWidget::onScoreChanged);
    connect(m_controller, &GameController::nextPieceChanged,
            this, &GameWidget::onNextPieceChanged);
    connect(m_controller, &GameController::gameOver,
            this, &GameWidget::onGameOver);
    connect(m_controller, &GameController::linesClearing,
            this, [this](int) {
        m_gameOverLabel->hide();
        m_canvas->update();
    });

    // 按钮 → 控制器槽
    connect(m_startBtn, &QPushButton::clicked, m_controller, &GameController::start);
    connect(m_pauseBtn, &QPushButton::clicked, m_controller, &GameController::pause);
    connect(m_resetBtn, &QPushButton::clicked, m_controller, &GameController::reset);

    setWindowTitle(QStringLiteral("Tetris"));
    setFixedSize(sizeHint());
}

void GameWidget::setupUI()
{
    m_canvas = new GameCanvas(m_controller, this);

    // 右侧信息面板
    QWidget* side = new QWidget(this);
    QVBoxLayout* sl = new QVBoxLayout(side);
    sl->setSpacing(8);
    sl->setContentsMargins(10, 10, 10, 10);

    // NEXT 标题 + 预览
    QLabel* nextTitle = new QLabel(QStringLiteral("NEXT"), side);
    nextTitle->setAlignment(Qt::AlignCenter);
    nextTitle->setStyleSheet("color: #aaa; font-weight: bold; font-size: 14px;");
    sl->addWidget(nextTitle);

    m_preview = new NextPiecePreview(side);
    sl->addWidget(m_preview, 0, Qt::AlignCenter);
    sl->addSpacing(10);

    // 统计标签（标题 + 数值）
    auto addStat = [&](const QString& t, QLabel*& out) {
        QLabel* tl = new QLabel(t, side);
        tl->setStyleSheet("color: #888; font-size: 11px;");
        sl->addWidget(tl);
        out = new QLabel(QStringLiteral("0"), side);
        out->setStyleSheet("color: #fff; font-size: 18px; font-weight: bold;");
        sl->addWidget(out);
    };
    addStat(QStringLiteral("SCORE"),      m_scoreLabel);
    addStat(QStringLiteral("LEVEL"),      m_levelLabel);
    addStat(QStringLiteral("LINES"),      m_linesLabel);
    addStat(QStringLiteral("HIGH SCORE"), m_highScoreLabel);
    sl->addSpacing(10);

    // 游戏结束标签（默认隐藏）
    m_gameOverLabel = new QLabel(side);
    m_gameOverLabel->setAlignment(Qt::AlignCenter);
    m_gameOverLabel->setStyleSheet("color: #f44; font-weight: bold; font-size: 14px;");
    m_gameOverLabel->hide();
    sl->addWidget(m_gameOverLabel);
    sl->addStretch();

    // 按钮
    m_startBtn = new QPushButton(QStringLiteral("▶  Start"), side);
    m_pauseBtn = new QPushButton(QStringLiteral("⏸  Pause"), side);
    m_resetBtn = new QPushButton(QStringLiteral("↺  Reset"), side);
    sl->addWidget(m_startBtn);
    sl->addWidget(m_pauseBtn);
    sl->addWidget(m_resetBtn);

    // 主布局
    QHBoxLayout* ml = new QHBoxLayout(this);
    ml->setSpacing(8);
    ml->setContentsMargins(10, 10, 10, 10);
    ml->addWidget(m_canvas);
    ml->addWidget(side);
}

void GameWidget::applyStyles()
{
    setStyleSheet(QStringLiteral(
        "QWidget { background-color: #1a1a1a; color: #ddd;"
        "  font-family: 'Segoe UI', 'Microsoft YaHei', sans-serif; }"
        "QPushButton { background-color: #3a3a3a; color: #eee;"
        "  border: 1px solid #555; border-radius: 4px; padding: 6px 14px;"
        "  font-size: 13px; min-width: 100px; }"
        "QPushButton:hover { background-color: #4a4a4a; border-color: #777; }"
        "QPushButton:pressed { background-color: #2a2a2a; }"
    ));
}

// ============================================================================
//  槽函数
// ============================================================================

void GameWidget::onBoardChanged()
{
    m_canvas->update();
}

void GameWidget::onScoreChanged(int score, int level, int lines, int high)
{
    m_gameOverLabel->hide();
    m_scoreLabel->setText(QString::number(score));
    m_levelLabel->setText(QString::number(level));
    m_linesLabel->setText(QString::number(lines));
    m_highScoreLabel->setText(QString::number(high));
    m_canvas->update();
}

void GameWidget::onNextPieceChanged(PieceType type)
{
    m_preview->setPieceType(type);
}

void GameWidget::onGameOver(int finalScore)
{
    m_gameOverLabel->setText(
        QStringLiteral("GAME OVER\nFinal Score: %1").arg(finalScore));
    m_gameOverLabel->show();
    m_canvas->update();
}
