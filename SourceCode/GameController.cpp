#include "GameController.h"
#include <chrono>

// ============================================================================
//  构造与析构
// ============================================================================

GameController::GameController(QObject* parent)
    : QObject(parent)
{
    m_rng.seed(static_cast<unsigned>(
        std::chrono::steady_clock::now().time_since_epoch().count()));

    m_timer = new QTimer(this);
    m_timer->setTimerType(Qt::PreciseTimer);
    connect(m_timer, &QTimer::timeout, this, &GameController::onTick);

    // 消行动画帧计时器（50ms 重复触发，非单次）
    m_animTimer = new QTimer(this);
    m_animTimer->setSingleShot(false);
    connect(m_animTimer, &QTimer::timeout,
            this, &GameController::onClearAnimationTimeout);

    m_scoreManager.loadHighScore();
}

GameController::~GameController() = default;

// ============================================================================
//  生命周期槽函数
// ============================================================================

void GameController::start()
{
    if (m_state == GameState::PLAYING)
        return;

    if (m_state == GameState::GAME_OVER || m_state == GameState::IDLE) {
        m_board.clear();
        m_scoreManager.reset();
        m_currentPiece.reset();
        m_nextPiece.reset();

        m_nextPiece = createPiece(randomType());
        emit nextPieceChanged(m_nextPiece->type());
        spawnPiece();

        emit boardChanged();
        emit scoreChanged(m_scoreManager.score(), m_scoreManager.level(),
                          m_scoreManager.linesCleared(), 
                          m_scoreManager.highScore());
    }

    m_state = GameState::PLAYING;
    m_timer->start(gravityInterval());
}

void GameController::pause()
{
    if (m_state != GameState::PLAYING) return;
    m_state = GameState::PAUSED;
    m_timer->stop();
}

void GameController::reset()
{
    m_timer->stop();
    m_board.clear();
    m_scoreManager.reset();
    m_currentPiece.reset();
    m_nextPiece.reset();
    m_state = GameState::IDLE;

    emit boardChanged();
    emit scoreChanged(m_scoreManager.score(), m_scoreManager.level(),
                      m_scoreManager.linesCleared(), m_scoreManager.highScore());
}

// ============================================================================
//  方块移动槽函数
// ============================================================================

void GameController::moveLeft()
{
    if (m_state != GameState::PLAYING || !m_currentPiece) return;
    if (tryMove(m_pieceX - 1, m_pieceY))
        emit boardChanged();
}

void GameController::moveRight()
{
    if (m_state != GameState::PLAYING || !m_currentPiece) return;
    if (tryMove(m_pieceX + 1, m_pieceY))
        emit boardChanged();
}

void GameController::moveDown()
{
    if (m_state != GameState::PLAYING || !m_currentPiece) return;
    if (!tryMove(m_pieceX, m_pieceY + 1))
        lockCurrentPiece();
    else
        emit boardChanged();
}

void GameController::rotatePiece()
{
    if (m_state != GameState::PLAYING || !m_currentPiece) return;
    if (tryRotate())
        emit boardChanged();
}

void GameController::hardDrop()
{
    if (m_state != GameState::PLAYING || !m_currentPiece) return;
    while (tryMove(m_pieceX, m_pieceY + 1)) { }
    lockCurrentPiece();
}

// ============================================================================
//  计时器回调
// ============================================================================

void GameController::onTick()
{
    if (m_state != GameState::PLAYING || !m_currentPiece) return;
    if (!tryMove(m_pieceX, m_pieceY + 1))
        lockCurrentPiece();
    else
        emit boardChanged();
}

// ============================================================================
//  移动与旋转辅助
// ============================================================================

bool GameController::tryMove(int newX, int newY)
{
    if (!m_currentPiece) return false;
    if (m_board.canPlace(*m_currentPiece, newX, newY)) {
        m_pieceX = newX;
        m_pieceY = newY;
        return true;
    }
    return false;
}

bool GameController::tryRotate()
{
    if (!m_currentPiece) return false;
    m_currentPiece->rotate();
    if (m_board.canPlace(*m_currentPiece, m_pieceX, m_pieceY))
        return true;
    // 旋转后碰撞 → 再旋转 3 次（共 360°）回退
    m_currentPiece->rotate();
    m_currentPiece->rotate();
    m_currentPiece->rotate();
    return false;
}
