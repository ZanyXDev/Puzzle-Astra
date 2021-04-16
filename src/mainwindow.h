#pragma once

#include <QMainWindow>
#include <QCoreApplication>
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include <QSize>
#include <QFileDialog>
#include <QPainter>
#include <QColor>
#include <QBitmap>
#include <QRandomGenerator>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QTimer>
#include <QMouseEvent>
#include <QEvent>
#include <QScrollArea>
#include <QVector>
#include <QtConcurrent>
#include <QProgressDialog>


#ifdef QT_DEBUG
#include <QDebug>
#include <QElapsedTimer>
#endif

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:

public slots:
    void newPuzzle();
    void savePuzzleToFile();
    void loadPuzzle();
    void alignmentPuzzle();
    void previewPuzzle();
    void aboutApp();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    //TODO init list for class member
    int puzzleOrigWidth = 200;
    int puzzleOrigHeight = 165;

    int puzzleBorderWidth = 30;
    int puzzleBorderHeight = 28;

    int puzzleWidth = puzzleOrigWidth-57;       // 143
    int puzzleHeight = puzzleOrigHeight-57;     // 108
    int distanceForAutoBonding = 15;

    //TODO if use threads this options not need - DELETE
    bool showPuzzleBeforStart = false;

    QString lastPath;
    QString puzzleFilename;

    QWidget *widgetTable;
    QWidget *centralWidget;

    QPushButton *btnNewPuzzle;
    QPushButton *btnSavePuzzle;
    QPushButton *btnLoadPuzzle;
    QPushButton *btnAlignment;
    QPushButton *btnPreview;
    QPushButton *btnAbout;
    QPushButton *btnExit;

    QHBoxLayout *centralLayout;
    QVBoxLayout *btnLayout;

    QPixmap puzzlePixmap;

    QParallelAnimationGroup puzzleAnimationGroup;
    QVector<QLabel*> listItems;

    void setupButtons();
    /**
     * @brief createPuzzle
     * @return currentPuzzleCount
     */
    int createPuzzle();

    bool isEven(int number);

    /**
     * @brief setupAnimation for puzzleItem
     * @param item puzzleItem
     * @param mode true ordered, false random
     */
    void setupAnimation(QLabel *item, int pos_x=0,int pos_y=0,bool mode=false);

    void changeWindowSizeAnimated();

};

