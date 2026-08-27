#include "ScoreManager.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>

// 构造函数 —— 初始化分数为 0，等级为 1，最高分为 0
// 最高分在构造函数中暂时为 0，
// 需要在程序启动时调用 loadHighScore() 从文件加载。
ScoreManager::ScoreManager()
    : m_score(0)
    , m_level(1)
    , m_linesCleared(0)
    , m_highScore(0)
{
}

// 根据消行数和当前等级增加分数
void ScoreManager::addLinesCleared(int count, int level)
{
    // 经典 NES 俄罗斯方块计分表
    static const int pointsTable[] = { 0, 100, 300, 500, 800 };
    if (count < 1 || count > 4) return;

    m_score += pointsTable[count] * level;
    m_linesCleared += count;

    // 每消除 10 行，等级提升 1 级
    m_level = 1 + m_linesCleared / 10;

    // 内存中更新最高分（不立即写入文件，游戏结束时统一保存）
    if (m_score > m_highScore) {
        m_highScore = m_score;
    }
}

// 重置游戏数据 —— 分数、等级、消行数归零/一
// m_highScore 故意保留不重置，确保历史最高分在新游戏中仍然可见。
void ScoreManager::reset()
{
    m_score       = 0;
    m_level       = 1;
    m_linesCleared = 0;
    // m_highScore 保留不重置
}

// 将最高分保存到本地文件
// 文件路径通过 scoreFilePath() 确定。
// 如果父目录不存在则自动创建。
// 文件格式为纯文本，内容为最高分的十进制数字。
void ScoreManager::saveHighScore() const
{
    QString path = scoreFilePath();
    QDir().mkpath(QFileInfo(path).absolutePath());   // 确保目录存在

    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << m_highScore;
    }
}

// 从本地文件加载历史最高分
// 若文件不存在或读取失败，最高分保持为 0，
// 这在首次运行时是正常情况。
void ScoreManager::loadHighScore()
{
    QFile file(scoreFilePath());
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in >> m_highScore;
    }
    // 文件不存在时 m_highScore 保持为 0 —— 正常情况
}

// 获取最高分文件的完整存储路径
// 使用 QStandardPaths::AppDataLocation 获取用户应用程序数据目录，
QString ScoreManager::scoreFilePath()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
           + QStringLiteral("/tetris_highscore.txt");
}
