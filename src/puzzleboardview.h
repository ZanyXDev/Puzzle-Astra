#pragma once
#include <QGraphicsView>
#include <QGraphicsEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QtConcurrent>
#include <QBitmap>
#include <QDebug>

class PuzzleBoardView  :public QGraphicsView
{
    Q_OBJECT
public:
    explicit PuzzleBoardView(QWidget *parent = nullptr);
    void setPuzzleRect(QRectF m_rect);

signals:
    void puzzleCounts(int counts);

public slots:
    void setPixmap(const QPixmap &pixmap);

private:
    //TODO init list for class member
    bool isPuzzleLoad;
    int puzzleOrigWidth = 200;
    int puzzleOrigHeight = 165;
    int puzzleBorderWidth = 30;
    int puzzleBorderHeight = 28;

    int puzzleWidth = puzzleOrigWidth-57;       // 143
    int puzzleHeight = puzzleOrigHeight-57;     // 108
    int distanceForAutoBonding = 15;

    QPixmap puzzlePixmap;
    QGraphicsScene m_scene;


    void resizeEvent(QResizeEvent *event);
    bool isEven(int number);
    void createPuzzle();

    QPixmap makePicturePuzzle(const QPixmap &pixmap, QString puzzleType, QString effectType) const;
};

