#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QTimer>
#include <memory>
#include <random>

#include "GameBoard.h"
#include "ScoreManager.h"
#include "Tetromino.h"

// 游戏主控制器（MVC 中的 Control 层）
// 拥有 GameBoard、ScoreManager、当前/下一个方块和 QTimer。
// 通过信号/槽机制与视图层通信，核心逻辑与 UI 完全分离。
class GameController : public QObject {
    Q_OBJECT

public:
    // 游戏生命周期状态
    enum class GameState {
        IDLE,        // 未开始或已重置
        PLAYING,     // 计时器运行，接受输入
        PAUSED,      // 计时器停止
        GAME_OVER    // 棋盘冻结
    };

    explicit GameController(QObject* parent = nullptr);
    ~GameController() override;

    // --- 状态查询（视图层读取渲染数据） ---

    GameState state() const { return m_state; }
    const GameBoard& board() const { return m_board; }
    const Tetromino* currentPiece() const { return m_currentPiece.get(); }
    const Tetromino* nextPiece()    const { return m_nextPiece.get(); }
    int currentX() const { return m_pieceX; }
    int currentY() const { return m_pieceY; }
    const ScoreManager& scoreManager() const { return m_scoreManager; }

    // 构建合并后的 20×10 显示网格（固定方块 + 当前活动方块）
    QVector<QVector<int>> getDisplayGrid() const;

    // 计算幽灵块的行坐标（硬降落点）
    int getGhostY() const;

    // 获取正在播放消行动画的行索引列表
    const QVector<int>& animatingRows() const { return m_animatingRows; }

    // 消行动画是否正在播放
    bool isClearAnimActive() const { return m_clearAnimActive; }

    // 消行动画进度（0.0 开始 → 1.0 结束）
    float clearAnimProgress() const {
        return m_clearAnimActive
            ? qMin(1.0f, static_cast<float>(m_clearAnimTimer) / ClearAnimDuration)
            : 0.0f;
    }

public slots:
    // --- 生命周期 ---
    void start();          // 开始/重新开始游戏
    void pause();          // 暂停/继续游戏
    void reset();          // 清空所有数据，回到 IDLE

    // --- 方块移动 ---
    void moveLeft();       // 左移一格
    void moveRight();      // 右移一格
    void moveDown();       // 软降（下移一格，触底则锁定）
    void rotatePiece();    // 顺时针旋转 90°
    void hardDrop();       // 硬降（直落到底后锁定）

signals:
    void boardChanged();       // 棋盘视觉变化（移动/旋转/锁定/消行）
    void scoreChanged(int score, int level, int linesCleared, int highScore);
    void nextPieceChanged(PieceType nextType);
    void gameOver(int finalScore);
    void linesClearing(int rowCount);   // 消行动画开始

private slots:
    void onTick();                  // 重力下落步进（QTimer 超时回调）
    void onClearAnimationTimeout(); // 消行动画逐帧回调

private:
    // --- 内部辅助 ---
    void spawnPiece();                          // 生成新方块（next → current）
    void lockCurrentPiece();                    // 锁定当前方块 + 触发消行
    std::unique_ptr<Tetromino> createPiece(PieceType type) const;
    PieceType randomType();                     // 均匀随机选择方块类型
    int gravityInterval() const;                // 当前等级对应的下落间隔(ms)
    bool tryMove(int newX, int newY);           // 尝试移动，成功返回 true
    bool tryRotate();                           // 尝试旋转，碰撞则回退

    // --- 成员变量 ---
    GameBoard m_board;
    ScoreManager m_scoreManager;
    std::unique_ptr<Tetromino> m_currentPiece;
    std::unique_ptr<Tetromino> m_nextPiece;
    int m_pieceX = 0, m_pieceY = 0;
    GameState m_state = GameState::IDLE;

    QTimer* m_timer = nullptr;               // 重力下落计时器
    QTimer* m_animTimer = nullptr;           // 消行动画帧计时器（50ms）
    QVector<int> m_animatingRows;            // 动画中的行索引
    bool m_clearAnimActive = false;          // 动画激活标志
    int  m_clearAnimTimer = 0;               // 动画帧计数

    std::mt19937 m_rng;
    std::uniform_int_distribution<int> m_dist{0, 6};

    static constexpr int BASE_INTERVAL   = 800;
    static constexpr int INTERVAL_DELTA  = 75;
    static constexpr int MIN_INTERVAL    = 100;
    static constexpr int ANIM_TICK_MS    = 50;
    static constexpr int ClearAnimDuration = 8;   // 总帧数（8×50ms=400ms）
};

#endif // GAMECONTROLLER_H
