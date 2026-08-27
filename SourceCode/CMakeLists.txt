cmake_minimum_required(VERSION 3.16)
project(Tetris VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Try Qt6 first, fall back to Qt5
find_package(Qt6 QUIET COMPONENTS Widgets)
if(NOT Qt6_FOUND)
    find_package(Qt5 5.15 REQUIRED COMPONENTS Widgets)
    set(QT_VERSION_MAJOR 5)
else()
    set(QT_VERSION_MAJOR 6)
endif()

# === Model Layer: Tetromino pieces ===
set(TETROMINO_SOURCES
    Tetromino.cpp
    TetrominoI.cpp
    TetrominoO.cpp
    TetrominoT.cpp
    TetrominoL.cpp
    TetrominoJ.cpp
    TetrominoS.cpp
    TetrominoZ.cpp
)

set(TETROMINO_HEADERS
    Tetromino.h
    TetrominoI.h
    TetrominoO.h
    TetrominoT.h
    TetrominoL.h
    TetrominoJ.h
    TetrominoS.h
    TetrominoZ.h
)

# === Model Layer: GameBoard & ScoreManager ===
set(MODEL_SOURCES
    GameBoard.cpp
    ScoreManager.cpp
)

set(MODEL_HEADERS
    GameBoard.h
    ScoreManager.h
)

# === Controller Layer ===
set(CONTROLLER_SOURCES
    GameController.cpp
    GameController_mechanics.cpp
)

set(CONTROLLER_HEADERS
    GameController.h
)

# === View Layer ===
set(VIEW_SOURCES
    GameCanvas.cpp
    NextPiecePreview.cpp
    GameWidget.cpp
)

set(VIEW_HEADERS
    BlockColors.h
    GameCanvas.h
    NextPiecePreview.h
    GameWidget.h
)

# === Main executable ===
add_executable(tetris
    main.cpp
    ${TETROMINO_SOURCES}
    ${MODEL_SOURCES}
    ${CONTROLLER_SOURCES}
    ${VIEW_SOURCES}
)

target_include_directories(tetris PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(tetris PRIVATE Qt6::Widgets)
else()
    target_link_libraries(tetris PRIVATE Qt5::Widgets)
endif()

# === Test executable (console, no GUI needed in test code) ===
add_executable(tetris_test
    test.cpp
    ${TETROMINO_SOURCES}
    ${MODEL_SOURCES}
)

target_include_directories(tetris_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(tetris_test PRIVATE Qt6::Core Qt6::Gui)
else()
    target_link_libraries(tetris_test PRIVATE Qt5::Core Qt5::Gui)
endif()
