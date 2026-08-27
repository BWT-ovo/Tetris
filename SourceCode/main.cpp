#include <QApplication>
#include "GameController.h"
#include "GameWidget.h"

// 俄罗斯方块游戏 —— 主入口函数
// 程序启动流程：
// 1. 创建 QApplication 对象（Qt GUI 应用的基础）
// 2. 创建 GameController（模型层 + 控制层）
// 3. 创建 GameWidget 窗口（视图层），传入控制器指针
// 4. 显示窗口
// 5. 进入 Qt 事件循环（app.exec()）
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("Tetris"));
    app.setApplicationVersion(QStringLiteral("1.0"));

    // 创建模型/控制器
    GameController controller;

    // 创建视图窗口
    GameWidget window(&controller);
    window.show();

    // 进入事件循环
    return app.exec();
}
