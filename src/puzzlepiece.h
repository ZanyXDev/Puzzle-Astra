#pragma once

class PuzzlePiece : public QGraphicsPixmapItem
{
public:
    explicit PuzzlePiece(QGraphicsItem* parent = nullptr);
    QRectF boundingRect() const override;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    int type() const override;
private:
    bool m_dragged;
    QPointF m_center;
    QPointF m_shiftMouseCoords;
};

