#pragma once

class PuzzleBoardView  :public QGraphicsView
{
    Q_OBJECT
public:
    explicit PuzzleBoardView(QWidget *parent = nullptr);

signals:
    void puzzleCounts(int counts);

public slots:
    void setPixmap(const QPixmap &pixmap);

protected:
       void resizeEvent(QResizeEvent *event);
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

    int countPuzzleWidth;
    int countPuzzleHeight;
    QPixmap puzzlePixmap;
    QGraphicsScene m_scene;

    bool isEven(int number);
    void createPuzzle();

    QPixmap makePicturePuzzle(const QPixmap &pixmap, QString puzzleType, QString effectType) const;
};

