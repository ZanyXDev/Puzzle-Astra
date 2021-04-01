#pragma once

#include <QMainWindow>
#include <QCoreApplication>
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include <QSize>
#include <QDebug>
#include <QFileDialog>
#include <QPainter>
#include <QColor>
#include <QBitmap>
#include <QRandomGenerator>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSizePolicy>


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

private:
    QVector<QLabel*> listItems;

    QLabel *backgroundButtons;
    QLabel *widgetTable;

    QPushButton *btnNewPuzzle;
    QPushButton *btnSavePuzzle;
    QPushButton *btnLoadPuzzle;
    QPushButton *btnAlignment;
    QPushButton *btnPreview;
    QPushButton *btnAbout;
    QPushButton *btnExit;

    QWidget *centralWidget;
    QHBoxLayout *centralLayout;
    QVBoxLayout *btnLayout;

    QString lastPath;
    QString puzzleFilename;
    QPixmap puzzlePixmap;

    int puzzleOrigWidth = 200;
    int puzzleOrigHeight = 165;

    int puzzleBorderWidth = 30;
    int puzzleBorderHeight = 28;

    int puzzleWidth = puzzleOrigWidth-57;       // 143
    int puzzleHeight = puzzleOrigHeight-57;     // 108
    int distanceForAutoBonding = 15;

    void setupButtons();
    void createPuzzle();

    bool isEven(int number);
    void setPicturePuzzle(QLabel *item, const QString &effect);
};

