#pragma once

#include <QMainWindow>
#include <QShortcut>
#include <QMenu>

#include "puzzlegraphicsview.h"

class PuzzleMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit PuzzleMainWindow(QWidget *parent = nullptr);
    void setJustLaunchedWithImage(bool value);
    void toggleFullScreen();
    void openWelcomeDialog(QWidget *parent);
signals:

public slots:
    void openFile(const QString &fileName);
private slots:
    void ShowContextMenu(const QPoint &pos);

protected:
   // void contextMenuEvent(QContextMenuEvent *event) override;

private:
    bool justLaunchedWithImage;
    Qt::WindowStates storedWindowState;

    QAction *actNewPuzzle;
    QAction *actSavePuzzle;
    QAction *actLoadPuzzle;
    QAction *actAlignment;
    QAction *actPreview;
    QAction *actAbout;
    QAction *actToggleFullScreen;
    QAction *actExit;

    PuzzleGraphicsView *graphicsView;
    QMenu *contextMenu;
    QShortcut *escShortcut;

    void setupContextMenu();
    void setupActions();
};

