#include "GameController.h"

#include "TetrominoI.h"
#include "TetrominoO.h"
#include "TetrominoT.h"
#include "TetrominoL.h"
#include "TetrominoJ.h"
#include "TetrominoS.h"
#include "TetrominoZ.h"

// ============================================================================
//  方块生成与锁定
// ============================================================================

void GameController::spawnPiece()
{
    m_currentPiece = std::move(m_nextPiece);
    if (!m_currentPiece) return;

    m_nextPiece = createPiece(randomType());
    emit nextPieceChanged(m_nextPiece->type());

    // 居中放置在棋盘顶部
    m_pieceX = (GameBoard::WIDTH - m_currentPiece->matrixSize()) / 2;
    m_pieceY = 0;

    // 出生位置被占 → 游戏结束
    if (!m_board.canPlace(*m_currentPiece, m_pieceX, m_pieceY)) {
        m_state = GameState::GAME_OVER;
        m_timer->stop();
        m_scoreManager.saveHighScore();
        emit boardChanged();
        emit gameOver(m_scoreManager.score());
    }
}

void GameController::lockCurrentPiece()
{
    if (!m_currentPiece) return;

    // 合并到棋盘
    m_board.place(*m_currentPiece, m_pieceX, m_pieceY);

    // 检测满行（不立即清除，先播放动画）
    m_animatingRows.clear();
    for (int row = GameBoard::HEIGHT - 1; row >= 0; --row) {
        bool full = true;
        for (int col = 0; col < GameBoard::WIDTH; ++col) {
            if (m_board.cell(row, col) == 0) { full = false; break; }
        }
        if (full) m_animatingRows.append(row);
    }

    if (!m_animatingRows.isEmpty()) {
        // 启动逐帧消行动画
        m_timer->stop();
        m_clearAnimActive = true;
        m_clearAnimTimer = 0;
        emit linesClearing(m_animatingRows.size());
        emit boardChanged();
        m_animTimer->start(ANIM_TICK_MS);
    } else {
        emit boardChanged();
        spawnPiece();
    }
}

// ============================================================================
//  消行动画
// ============================================================================

void GameController::onClearAnimationTimeout()
{
    ++m_clearAnimTimer;

    if (m_clearAnimTimer < ClearAnimDuration) {
        // 动画进行中 → 刷新画面
        emit boardChanged();
        return;
    }

    // 动画结束 → 真正消行
    m_animTimer->stop();
    m_clearAnimActive = false;

    int cleared = m_board.clearLines();
    m_animatingRows.clear();

    if (cleared > 0) {
        m_scoreManager.addLinesCleared(cleared, m_scoreManager.level());
        emit scoreChanged(m_scoreManager.score(), m_scoreManager.level(),
                          m_scoreManager.linesCleared(), 
                          m_scoreManager.highScore());
        m_timer->start(gravityInterval());
    }

    emit boardChanged();
    spawnPiece();
}

// ============================================================================
//  工厂方法与工具函数
// ============================================================================

std::unique_ptr<Tetromino> GameController::createPiece(PieceType type) const
{
    switch (type) {
        case PieceType::I: return std::make_unique<TetrominoI>();
        case PieceType::O: return std::make_unique<TetrominoO>();
        case PieceType::T: return std::make_unique<TetrominoT>();
        case PieceType::L: return std::make_unique<TetrominoL>();
        case PieceType::J: return std::make_unique<TetrominoJ>();
        case PieceType::S: return std::make_unique<TetrominoS>();
        case PieceType::Z: return std::make_unique<TetrominoZ>();
    }
    return nullptr;
}

PieceType GameController::randomType()
{
    return static_cast<PieceType>(m_dist(m_rng));
}

int GameController::gravityInterval() const
{
    int interval = BASE_INTERVAL - (m_scoreManager.level() - 1) * INTERVAL_DELTA;
    return std::max(interval, MIN_INTERVAL);
}

// ============================================================================
//  显示网格与幽灵块
// ============================================================================

QVector<QVector<int>> GameController::getDisplayGrid() const
{
    QVector<QVector<int>> grid(GameBoard::HEIGHT,
                                QVector<int>(GameBoard::WIDTH, 0));
    // 复制固定方块
    for (int r = 0; r < GameBoard::HEIGHT; ++r)
        for (int c = 0; c < GameBoard::WIDTH; ++c)
            grid[r][c] = m_board.cell(r, c);

    // 叠加当前活动方块
    if (m_currentPiece && (m_state == GameState::PLAYING ||
                           m_state == GameState::PAUSED)) {
        const auto& shape = m_currentPiece->shape();
        int size = m_currentPiece->matrixSize();
        int colorId = static_cast<int>(m_currentPiece->type()) + 1;

        for (int r = 0; r < size; ++r) {
            for (int c = 0; c < size; ++c) {
                if (shape[r][c] == 0) continue;
                int br = m_pieceY + r, bc = m_pieceX + c;
                if (br >= 0 && br < GameBoard::HEIGHT &&
                    bc >= 0 && bc < GameBoard::WIDTH)
                    grid[br][bc] = colorId;
            }
        }
    }
    return grid;
}

int GameController::getGhostY() const
{
    if (!m_currentPiece) return m_pieceY;
    int ghostY = m_pieceY;
    while (m_board.canPlace(*m_currentPiece, m_pieceX, ghostY + 1))
        ++ghostY;
    return ghostY;
}
