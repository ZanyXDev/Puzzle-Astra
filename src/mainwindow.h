#pragma once

#include <QMainWindow>
#include <QCoreApplication>
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include <QSize>
#include <QDebug>

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
    QLabel *backgroundButtons;
    QPushButton *btnNewPuzzle;
    QPushButton *btnSavePuzzle;
    QPushButton *btnLoadPuzzle;
    QPushButton *btnAlignment;
    QPushButton *btnPreview;
    QPushButton *btnAbout;
    QPushButton *btnExit;

    void setupButtons();

};

