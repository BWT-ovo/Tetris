俄罗斯方块游戏
版本：1.0
实现代码：C++（Qt 6 Widgets）
开发日期：2026.06.15 — 2026.06.26
编写者：张艺、赵乙丞


一、简介
------本俄罗斯方块游戏基于 Qt 6 Widgets 框架和 C++ 面向对象思想开发，采用 MVC 三层架构设计，将模型层（方块、棋盘、分数）、控制层（游戏逻辑、计时器）和视图层（界面绘制、用户交互）完全分离，通过 Qt 信号/槽机制实现层间通信。游戏包含 7 种标准方块、幽灵块预览、逐帧消行动画、等级加速系统和最高分持久化功能。


二、功能特点
1. 完整游戏体验
------支持 7 种标准俄罗斯方块（I/O/T/L/J/S/Z），每种独立颜色。包含左右移动、软降、顺时针旋转、硬降等全部标准操作。游戏状态涵盖未开始、游戏中、暂停、游戏结束四种模式。

2. 幽灵块预览
------当前方块下方以半透明虚线显示硬降落点位置，帮助玩家预判落点，提高操作精度。

3. 逐帧消行动画
------满行消除时播放 400ms 逐帧动画（白光闪烁 + 收缩消失），动画逻辑与数据操作分离，视觉体验流畅。

4. 等级加速系统
------每消除 10 行等级提升 1 级。下落速度从 800ms 逐渐加快至 100ms，难度逐步递增。计分采用经典 NES 规则（100/300/500/800 × 等级）。

5. 最高分持久化
------历史最高分自动保存至本地文件，程序重启后自动加载，跨游戏局保留。

6. MVC 架构分离
------Model 层纯数据类不依赖 GUI，Controller 层通过信号与 View 通信，View 层只负责绘制和输入转发。三层单向依赖，职责清晰。

7. 圆角渲染
------所有方块采用圆角矩形绘制，搭配渐变高光和深色描边，呈现立体视觉效果。


三、安装指南
------开发环境：Qt 6.x + CMake 3.16+ 或 qmake
------运行环境：Windows 11 
------编译器要求：支持 C++17（如 MinGW 8.1+、MSVC 2019+、GCC 8+）

构建步骤：
1. 打开 Qt Creator，选择"文件 → 打开文件或项目"
2. 选择项目根目录下的 Tetris.pro 或 CMakeLists.txt
3. 选择 Qt 6.x Kit，点击 Configure Project
4. 按 Ctrl+R 编译运行

或使用命令行：
------cmake -B build
------cmake --build build
------./build/tetris


四、使用方法
1. 启动软件
------运行编译生成的可执行文件，进入游戏主界面。

2. 开始游戏
------点击 Start 按钮，第一个方块从棋盘顶部出生，右侧面板显示下一个方块预览。

3. 操作方块
------键盘 ← → 键左右移动，↓ 键软降，↑ 键旋转，Space 键硬降直接落底。

4. 暂停与重置
------点击 Pause 按钮暂停游戏，再次点击 Start 继续。点击 Reset 按钮清空棋盘回到初始状态。

5. 游戏结束
------当方块堆叠到棋盘顶部无法出生时游戏结束，最高分自动保存，右侧面板显示 GAME OVER 和最终分数。

6. 运行单元测试
------cmake --build build --target tetris_test
------./build/tetris_test
------控制台输出 6 个测试套件（25+ 断言），验证方块创建、旋转、碰撞检测、消行逻辑和多态特性。


五、常见问题及解决方法
1. 程序无法编译
------检查 Qt 版本是否为 6.x 或 5.15+，确保 CMakeLists.txt 中 find_package 能找到对应版本的 Qt Widgets 模块。
------确认编译器支持 C++17 标准（CMakeLists.txt 中已设置 CMAKE_CXX_STANDARD 17）。
------若使用 qmake，确认 Tetris.pro 文件中 QT += widgets 配置正确。

2. 程序无法启动
------检查编译生成的可执行文件是否在正确目录下，确保 Qt 动态库路径已配置到系统环境变量。
------若提示缺少 DLL（Windows），将 Qt 安装目录下的 bin 文件夹添加到 PATH 环境变量，或使用 windeployqt 工具自动部署。

3. 方块旋转行为异常
------贴墙旋转时可能无法旋转（当前版本使用碰撞回退策略，未实现完整墙踢系统）。若需改善手感，可在 GameController::tryRotate() 中添加墙踢偏移表进行多位置试探。

4. 幽灵块位置不准确
------检查 GameController::getGhostY() 中的 while 循环逻辑，确认 canPlace 逐行下探的终止条件正确。若幽灵块与活动方块重叠且不显示，确认 View 层对 ghostY != currentY 的判断正确。

5. 消行动画卡顿或不显示
------检查 m_animTimer 是否正确创建并绑定 onClearAnimationTimeout 槽函数。确认 GameCanvas::paintEvent 中 isClearAnimActive() 分支能正确读取 clearAnimProgress() 返回值。
------若动画播放期间点击 Reset 导致异常，在 reset() 中添加 m_animTimer->stop() 和 m_clearAnimActive = false 的状态重置。

6. 最高分未保存或无法读取
------检查系统是否有 AppData 目录的写入权限。文件路径由 QStandardPaths::AppDataLocation 自动确定，若路径异常可手动打印 scoreFilePath() 返回值检查。
------确认 saveHighScore() 仅在 GameController::spawnPiece() 中 GAME_OVER 分支被调用，loadHighScore() 仅在构造函数中被调用。


六、反馈与支持
如有更多问题请联系项目组成员！
电子邮件:zhaoyc@nwafu.edu.cn