#pragma once

class PuzzlePiece : public QGraphicsPixmapItem
{
public:
    explicit PuzzlePiece(QGraphicsItem* parent = nullptr);
     QRectF boundingRect() const;
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
private:
    bool m_dragged;
    QPointF m_center;
    QPointF m_shiftMouseCoords;
};

