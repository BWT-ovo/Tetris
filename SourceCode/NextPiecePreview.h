#ifndef NEXTPIECEPREVIEW_H
#define NEXTPIECEPREVIEW_H

#include <QWidget>
#include <memory>
#include "Tetromino.h"

// 下一个方块预览控件
// 在固定 130×110 区域内居中显示即将出现的方块形状和颜色。
// 通过 setPieceType() 更新预览内容。
class NextPiecePreview : public QWidget {
    Q_OBJECT
public:
    explicit NextPiecePreview(QWidget* parent = nullptr);

    // 设置要预览的方块类型（工厂创建具体方块对象）
    void setPieceType(PieceType type);

    QSize sizeHint() const override;

protected:
    // 绘制事件 —— 深色背景 + 居中圆角方块预览
    void paintEvent(QPaintEvent* event) override;

private:
    std::unique_ptr<Tetromino> m_previewPiece;
};

#endif // NEXTPIECEPREVIEW_H
