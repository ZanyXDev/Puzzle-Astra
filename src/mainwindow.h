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

#include <QProgressDialog>
#include "puzzleboardview.h"

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
    void changePuzzleCountValue(int m_count);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    int currentPuzzleCount;
    //TODO if use threads this options not need - DELETE
    bool showPuzzleBeforStart = false;

    QString lastPath;
    QString puzzleFilename;

    PuzzleBoardView *widgetTable;
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

    //QPixmap puzzlePixmap;

    QParallelAnimationGroup puzzleAnimationGroup;
    QVector<QLabel*> listItems;
 QFutureWatcher<void> futureWatcher;
    void setupButtons();

    /**
     * @brief setupAnimation for puzzleItem
     * @param item puzzleItem
     * @param mode true ordered, false random
     */
    void setupAnimation(QLabel *item, int pos_x=0,int pos_y=0,bool mode=false);

    void changeWindowSizeAnimated();

};

