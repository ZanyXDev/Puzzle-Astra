#include "puzzlegraphicsview.h"
#include <QGestureEvent>
#include <QWheelEvent>

PuzzleGraphicsView::PuzzleGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    // GraphicsView setup
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setFrameShape(QFrame::NoFrame);
    setTransformationAnchor(QGraphicsView::NoAnchor);
    // part of a pathetic attempt at gesture support
    grabGesture(Qt::PinchGesture);

    // Scene setup
    auto *scene = new QGraphicsScene(0.0, 0.0, 100000.0, 100000.0, this);
    setScene(scene);

    // Initialize configurable variables

    isScalingEnabled = true;
    isScrollZoomsEnabled = true;
    isCursorZoomEnabled = true;
    scaleFactor = 1.25;

    // Initialize other variables
    adjustedBoundingRect = QRectF();
    adjustedImageSize = QSize();
    currentScale = 1.0;
    scaledSize = QSize();
    isOriginalSize = false;
}

// Events

void PuzzleGraphicsView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
//    if (!isOriginalSize){
//        resetScale();
//    }
//    else{
//        centerOn(loadedPixmapItem);
//    }
}

void PuzzleGraphicsView::enterEvent(QEvent *event)
{
    QGraphicsView::enterEvent(event);
    viewport()->setCursor(Qt::ArrowCursor);
}

void PuzzleGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    viewport()->setCursor(Qt::ArrowCursor);
}



