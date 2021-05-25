#pragma once

#include <QGraphicsView>
#include <QObject>

class PuzzleGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    enum class ScaleMode
    {
        resetScale,
        zoom
    };
    Q_ENUM(ScaleMode)

    explicit PuzzleGraphicsView(QWidget *parent = nullptr);

    void loadFile(const QString &fileName);
    void resetScale();
    void scaleExpensively(ScaleMode mode);
    void originalSize(bool setVariables = true);

    //returned const reference is read-only
    const QPixmap& getLoadedPixmap() const
    {
        return loadedPixmap;
    }

signals:
    void updatedLoadedPixmapItem();

protected:

    void resizeEvent(QResizeEvent *event) override;

    void enterEvent(QEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    //    void fitInViewMarginless(bool setVariables = true);

    //    void centerOn(const QPointF &pos);

    //    void centerOn(qreal x, qreal y);

    //    void centerOn(const QGraphicsItem *item);

private slots:

    //    void updateLoadedPixmapItem();

    //    void error(int errorNum, const QString &errorString, const QString &fileName);

private:
    QGraphicsPixmapItem *loadedPixmapItem;
    QRectF adjustedBoundingRect;
    QSize adjustedImageSize;

    QTransform fittedTransform;
    QTransform scaledTransform;
    bool isScalingEnabled;
    bool isScrollZoomsEnabled;
    bool isCursorZoomEnabled;
    qreal scaleFactor;

    qreal currentScale;
    QSize scaledSize;
    bool isOriginalSize;
    bool movieCenterNeedsUpdating;
    QPixmap loadedPixmap;
};

