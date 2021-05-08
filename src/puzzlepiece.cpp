#include "puzzlepiece.h"

PuzzlePiece::PuzzlePiece(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
    , m_dragged(false)
{

}

QRectF PuzzlePiece::boundingRect() const
{
//    qDebug() << "opaqueArea().boundingRect()"
//             << this->opaqueArea().boundingRect()
//             << "QRegion(this->pixmap().createMaskFromColor(Qt::black,Qt::MaskOutColor)).boundingRect()"
//             << QRegion(this->pixmap().createMaskFromColor(Qt::black,Qt::MaskOutColor)).boundingRect()
//             << "QRegion(this->pixmap().createMaskFromColor(Qt::transparent)).boundingRect()"
//             << QRegion(this->pixmap().createMaskFromColor(Qt::transparent)).boundingRect();
    return QRegion(this->pixmap().createMaskFromColor(Qt::transparent)).boundingRect();
}

void PuzzlePiece::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    qDebug() << "Custom item move event.";
    m_dragged = true;
    this->setPos(mapToScene(event->pos() + m_shiftMouseCoords));
    QGraphicsPixmapItem::mouseMoveEvent(event);
}

void PuzzlePiece::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Custom item mousePressEvent";
    m_shiftMouseCoords = this->pos() - mapToScene(event->pos());
    this->setCursor(QCursor(Qt::ClosedHandCursor));

    if(event->button() == Qt::LeftButton) {
        if(event->modifiers() == Qt::ShiftModifier) {
            qDebug() << "Custom item left clicked with shift key.";
            // add the item to the selection
            setSelected(true);
        } else if(event->modifiers() == Qt::AltModifier){
            qDebug() << "Custom item left clicked with alt key.";
            // resize the item
            double radius = boundingRect().width() / 2.0;
            m_center = QPointF(boundingRect().topLeft().x() + pos().x() + radius, boundingRect().topLeft().y() + pos().y() + radius);
            QPointF pos = event->scenePos();
            qDebug() << boundingRect() << radius << this->pos() << pos << event->pos();

        } else {
            qDebug() << "Custom item left clicked.";
            QGraphicsItem::mousePressEvent(event);
            event->accept();
        }
    } else if(event->button() == Qt::RightButton) {
        qDebug() << "Custom item right clicked.";
        event->ignore();
    }
}

void PuzzlePiece::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    qDebug() << "Custom item mouseReleaseEvent";
    this->setCursor(QCursor(Qt::ArrowCursor));
    if(m_dragged){
        QList<QGraphicsItem*> colItems = collidingItems();
        qDebug() << "colItems.count()"<<colItems.count();
        if(colItems.isEmpty()){

        }
        else {
            //            QGraphicsItemGroup * myGroup = new QGraphicsItemGroup();
            //            QGraphicsItem* closestItem = colItems.at(0);
            //            myGroup->addToGroup(this);
            //            myGroup->addToGroup(closestItem);

            //            qreal shortestDist = 100000;
            //            foreach(QGraphicsItem* item, colItems){
            //                QLineF line(item->sceneBoundingRect().center(),
            //                            this->sceneBoundingRect().center());
            //                if(line.length() < shortestDist){
            //                    shortestDist = line.length();
            //                    closestItem = item;
            //                }
            //            }

            //            this->setPos(closestItem->scenePos());
        }
        m_dragged = false;
    }
    QGraphicsPixmapItem::mouseReleaseEvent(event);
}

QVariant PuzzlePiece::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene()) {
        // value is the new position.
        QPointF newPos = value.toPointF();
        QRectF m_sceneRect = scene()->sceneRect();

        if (!m_sceneRect.contains(newPos)) {

            qDebug() <<"sceneRect:" <<m_sceneRect << " item->boundingRect():"<< this->boundingRect();
            qDebug() << "current pos:" << this->pos() << " newPos" << newPos;

            // Keep the item inside the scene rect.
            newPos.setX(qMin(m_sceneRect.right(), qMax(newPos.x(), m_sceneRect.left())));
            newPos.setY(qMin(m_sceneRect.bottom(), qMax(newPos.y(), m_sceneRect.top())));
            qDebug() << "change" << change <<" newPos" << newPos;
            return newPos;
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

int PuzzlePiece::type() const
{
    // Enable the use of qgraphicsitem_cast with this item.
    return UserType+1;
}

