#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include <QString>

// 分数管理器 —— 追踪分数、等级、消行数和最高分
class ScoreManager {
public:
    // 构造函数 —— 初始化分数为 0，等级为 1
    ScoreManager();

    // 根据消行数增加分数，使用经典 NES 计分表计算得分
    void addLinesCleared(int count, int level);

    // 重置分数、等级、消行数为初始值（最高分保留不重置）
    void reset();

    // ========== 属性访问器 ==========
    // 获取当前分数
    int score()       const { return m_score; }
    // 获取当前等级
    int level()       const { return m_level; }
    // 获取累计消行数
    int linesCleared() const { return m_linesCleared; }
    // 获取历史最高分
    int highScore()   const { return m_highScore; }

    // ========== 文件持久化 ==========
    // 将当前最高分保存到本地文件
    void saveHighScore() const;

    // 从本地文件加载历史最高分，若文件不存在则最高分保持为 0
    void loadHighScore();

private:
    int m_score;          // 当前分数
    int m_level;          // 当前等级（1 + 累计消行数 / 10）
    int m_linesCleared;   // 累计消除行数
    int m_highScore;      // 历史最高分

    // 获取最高分文件的完整路径
    // 使用 QStandardPaths 获取用户 AppData 目录
    static QString scoreFilePath();
};

#endif // SCOREMANAGER_H
