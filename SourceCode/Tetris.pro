# ============================================================
#  Tetris — Qt project file (qmake)
#  在 Qt Creator 中直接打开此文件即可构建运行
# ============================================================

QT       += core gui widgets
CONFIG   += c++17
TEMPLATE  = app
TARGET    = tetris

# --- 源文件 ---
SOURCES += \
    main.cpp \
    Tetromino.cpp \
    TetrominoI.cpp \
    TetrominoO.cpp \
    TetrominoT.cpp \
    TetrominoL.cpp \
    TetrominoJ.cpp \
    TetrominoS.cpp \
    TetrominoZ.cpp \
    GameBoard.cpp \
    ScoreManager.cpp \
    GameController.cpp \
    GameController_mechanics.cpp \
    GameCanvas.cpp \
    NextPiecePreview.cpp \
    GameWidget.cpp

# --- 头文件 ---
HEADERS += \
    Tetromino.h \
    TetrominoI.h \
    TetrominoO.h \
    TetrominoT.h \
    TetrominoL.h \
    TetrominoJ.h \
    TetrominoS.h \
    TetrominoZ.h \
    GameBoard.h \
    ScoreManager.h \
    GameController.h \
    BlockColors.h \
    GameCanvas.h \
    NextPiecePreview.h \
    GameWidget.h

# ============================================================
#  单元测试（独立可执行文件，纯控制台，无 GUI）
#  构建方式：在 Qt Creator 中切换构建设置为 Release 后单独编译，
#  或在命令行中执行：
#      qmake Tetris.pro CONFIG+=test
#      make
# ============================================================
test {
    TARGET   = tetris_test
    QT        = core gui      # QColor 在 QtGui 中
    CONFIG   -= app_bundle
    SOURCES  -= main.cpp
    SOURCES  += test.cpp
    HEADERS  -= GameWidget.h GameCanvas.h NextPiecePreview.h BlockColors.h
    SOURCES  -= GameWidget.cpp GameCanvas.cpp NextPiecePreview.cpp \
               GameController.cpp GameController_mechanics.cpp
}
