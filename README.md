# Tetris‑Qt6‑Cpp

俄罗斯方块游戏｜C++ / Qt 6 Widgets｜MVC 架构实现

> 
> 开发日期：**2026‑06‑15 — 2026‑06‑26**

<img width="50" height="20" alt="image" src="https://github.com/user-attachments/assets/b3e3ed1b-b855-4061-bc20-43279fe36857" />
<img width="58" height="20" alt="image" src="https://github.com/user-attachments/assets/28422df4-4c95-4554-86a3-cf538064d4be" />
<img width="84" height="20" alt="image" src="https://github.com/user-attachments/assets/fbbe2124-b6e6-4f7e-ab8d-180cb6981eb1" />




## 📖 简介

本俄罗斯方块游戏基于 **Qt 6 Widgets** 框架与 C++ 面向对象思想开发，采用 **MVC 三层架构**设计，将**模型层（方块、棋盘、分数）、控制层（游戏逻辑、计时器）和视图层（界面绘制、用户交互）**完全分离，通过 Qt **信号 / 槽**机制实现层间通信。

游戏包含 7 种标准方块、幽灵块预览、逐帧消行动画、等级加速系统和最高分持久化功能。

---

## ✨ 功能特点

### 🎮 完整游戏体验

- 支持 7 种标准俄罗斯方块（`I / O / T / L / J / S / Z`），每种方块拥有独立颜色
- 完整操作：左右移动、软降、顺时针旋转、硬降
- 四种游戏状态：`未开始` / `游戏中` / `暂停` / `游戏结束`

### 👻 幽灵块预览

当前方块下方以半透明虚线显示硬降落点位置，帮助玩家预判落点，提高操作精度。

### 🎬 逐帧消行动画

满行消除时播放 **400ms** 逐帧动画（白光闪烁 + 收缩消失），动画逻辑与数据操作分离，视觉体验流畅。

### ⚡ 等级加速系统

- 每消除 10 行等级提升 1 级
- 下落速度：从 `800 ms` 逐步加快至 `100 ms`，难度递增
- 计分规则：经典 NES 计分 `(100 / 300 / 500 / 800) × 等级`

### 💾 最高分持久化

历史最高分自动保存至本地文件，程序重启后自动加载，跨游戏局永久保留。

### 🏗️ MVC 架构分离

- **Model 层**：纯数据类，无 GUI 依赖，存放棋盘、方块、分数等游戏数据
- **Controller 层**：负责游戏逻辑、碰撞检测、计时器调度，通过信号与视图通信
- **View 层**：仅负责界面绘制、键盘鼠标输入转发
- 三层单向依赖，职责清晰，易于扩展维护

### 🎨 圆角渲染效果

所有方块采用圆角矩形绘制，搭配渐变高光与深色描边，呈现立体视觉效果。

---

## 🛠️ 安装指南

### 开发环境

- Qt 6.x
- CMake 3.16+ 或 qmake
- Windows 11（推荐运行环境）
- 编译器：支持 C++17（MinGW 8.1+ / MSVC 2019+ / GCC 8+）

### 构建步骤 —— Qt Creator

1. 打开 Qt Creator，选择 `文件 → 打开文件或项目`
2. 选中项目根目录下的 `Tetris.pro` 或 `CMakeLists.txt`
3. 选择 Qt 6.x Kit，点击 `Configure Project`
4. 按下 `Ctrl + R` 编译运行

### 构建步骤 —— 命令行（CMake）

```
cmake -B build
cmake --build build
./build/tetris
```

---

## 🎯 使用方法

### 启动游戏

运行编译生成的可执行文件，进入游戏主界面。

### 开始游戏

点击 `Start` 按钮，第一个方块从棋盘顶部出生；右侧面板实时显示**下一个方块预览**。

### 键盘操作

表格

| 按键 | 功能 |
| --- | --- |
| ← / → | 方块左右移动 |
| ↓ | 软降（缓慢下落一格） |
| ↑ | 顺时针旋转方块 |
| Space | 硬降，方块直接落到底部 |

### 暂停与重置

- `Pause`：暂停游戏；再次点击 `Start` 继续游戏
- `Reset`：清空棋盘，回到初始状态

### 游戏结束

方块堆叠至棋盘顶部、新方块无法生成时判定游戏结束。最高分自动保存，右侧面板显示 `GAME OVER` 与最终得分。

### 运行单元测试

```
cmake --build build --target tetris_test
./build/tetris_test
```

控制台输出 6 个测试套件（**25+ 断言**），验证方块创建、旋转、碰撞检测、消行逻辑和多态特性。

---

## ❓ 常见问题 & 解决方案

### 程序无法编译

- 检查 Qt 版本是否为 6.x 或 5.15+，`CMakeLists.txt` 能够正常找到 Qt Widgets
- 确认编译器开启 C++17（`CMAKE_CXX_STANDARD 17`）
- qmake 用户：`Tetris.pro` 中配置 `QT += widgets`

### 程序无法启动（Windows）

- 确认可执行文件路径正确
- 将 Qt 的 `bin` 目录加入系统 `PATH` 环境变量
- 使用 `windeployqt` 工具一键部署依赖库，解决缺失 DLL 问题

### 方块旋转行为异常

> 
> 当前版本采用**碰撞回退策略**，尚未实现完整墙踢系统，贴墙时可能无法旋转。
> 如需优化手感，可在 `GameController::tryRotate()` 添加墙踢偏移表进行多位置试探。

### 幽灵块位置不准确

检查 `GameController::getGhostY()` 内逐行下探 while‑循环终止条件；
视图层确保幽灵块仅在 `ghostY != currentY` 时绘制。

### 消行动画卡顿 / 不显示

- 动画计时器 `m_animTimer` 是否正确创建并绑定 `onClearAnimationTimeout` 槽函数
- `GameCanvas::paintEvent` 中读取 `isClearAnimActive()`、`clearAnimProgress()`
- Reset 时停止动画：调用 `m_animTimer->stop()`，重置 `m_clearAnimActive = false`

### 最高分无法保存 / 读取失败

- 检查程序是否拥有本地 AppData 目录写入权限
- 文件路径由 `QStandardPaths::AppDataLocation` 自动生成，可打印 `scoreFilePath()` 排查路径
- 保存逻辑：`saveHighScore()` 在游戏结束分支调用；加载逻辑：`loadHighScore()` 在控制器构造函数调用

---

## 📮 反馈与支持

如有问题、Bug 反馈或功能建议，欢迎联系项目组成员：

> 
> 📧 wszychhh@gmail.com
