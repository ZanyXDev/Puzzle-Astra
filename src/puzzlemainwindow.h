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

protected:
   // void contextMenuEvent(QContextMenuEvent *event) override;

private:
    PuzzleGraphicsView *graphicsView;
    QMenu *contextMenu;
    bool justLaunchedWithImage;
    Qt::WindowStates storedWindowState;
    QShortcut *escShortcut;

};

