#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setWindowTitle(tr("Astra Puzzle v0.1-%1").arg(GIT_HASH));
    this->setWindowIcon(QIcon(":/res/images/puzzle.png"));

    // background form buttons
    //    backgroundButtons = new QLabel(this);
    //    backgroundButtons->setPixmap(QPixmap(":/res/images/background.png"));
    //    backgroundButtons->setScaledContents(true);
    //    backgroundButtons->installEventFilter(this);

    //    btnNewPuzzle = new QLabel(this);
    //    btnNewPuzzle->setPixmap(QPixmap(":res/images/buttons/new.png"));
    //    btnNewPuzzle->setToolTip(tr("Open file")+"  (Ctrl+O)");
    //    btnNewPuzzle->setGeometry(0,32,80,80);
    //    btnNewPuzzle->setScaledContents(true);
    //    btnNewPuzzle->installEventFilter(this);
    this->setupButtons();
}

void MainWindow::setupButtons()
{
    QPixmap pixmapNewBtn(":res/images/buttons/new.png");
    pixmapNewBtn = pixmapNewBtn.scaled(96, 96, Qt::KeepAspectRatioByExpanding);

    //QIcon ButtonIcon(pixmapNewBtn);
    btnNewPuzzle = new QPushButton(this);
    btnNewPuzzle->setShortcut(QKeySequence::New);
    btnNewPuzzle->setFlat(true);
    btnNewPuzzle->setIcon(QIcon(pixmapNewBtn));
    btnNewPuzzle->setIconSize(pixmapNewBtn.size());
    btnNewPuzzle->setGeometry(QRect(QPoint(4, 4), QSize(96, 96)));
    btnNewPuzzle->setToolTip(tr("New picture from file")+"  (Ctrl+N)");

    QPixmap pixmapSaveBtn(":res/images/buttons/save.png");
    pixmapSaveBtn = pixmapSaveBtn.scaled(96, 96, Qt::KeepAspectRatioByExpanding);

    btnSavePuzzle = new QPushButton(this);
    btnSavePuzzle->setShortcut(QKeySequence::Save);
    btnSavePuzzle->setFlat(true);
    btnSavePuzzle->setIcon(QIcon(pixmapSaveBtn));
    btnSavePuzzle->setIconSize(pixmapSaveBtn.size());
    btnSavePuzzle->setGeometry(QRect(QPoint(4, 104), QSize(96, 96)));
    btnSavePuzzle->setToolTip(tr("Save puzzle to file")+"  (Ctrl+S)");

    QPixmap pixmapLoadBtn(":res/images/buttons/open.png");
    pixmapLoadBtn = pixmapLoadBtn.scaled(96, 96, Qt::KeepAspectRatioByExpanding);

    btnLoadPuzzle = new QPushButton(this);
    btnLoadPuzzle->setShortcut(Qt::CTRL+Qt::Key_L);
    btnLoadPuzzle->setFlat(true);
    btnLoadPuzzle->setIcon(QIcon(pixmapLoadBtn));
    btnLoadPuzzle->setIconSize(pixmapLoadBtn.size());
    btnLoadPuzzle->setGeometry(QRect(QPoint(4, 204), QSize(96, 96)));
    btnLoadPuzzle->setToolTip(tr("Load puzzle from file")+"  (Ctrl+L)");

    QPixmap pixmapAligBtn(":res/images/buttons/alignment.png");
    pixmapAligBtn = pixmapAligBtn.scaled(96, 96, Qt::KeepAspectRatioByExpanding);

    btnAlignment = new QPushButton(this);
    btnAlignment->setShortcut(Qt::CTRL+Qt::Key_A);
    btnAlignment->setFlat(true);
    btnAlignment->setIcon(QIcon(pixmapAligBtn));
    btnAlignment->setIconSize(pixmapAligBtn.size());
    btnAlignment->setGeometry(QRect(QPoint(4, 304), QSize(96, 96)));
    btnAlignment->setToolTip(tr("Alignment")+"  (Ctrl+A)");

    QPixmap pixmapPreviewBtn(":res/images/buttons/preview.png");
    pixmapPreviewBtn = pixmapPreviewBtn.scaled(96, 96, Qt::KeepAspectRatioByExpanding);

    btnPreview = new QPushButton(this);
    btnPreview->setShortcut(QKeySequence::Print);
    btnPreview->setFlat(true);
    btnPreview->setIcon(QIcon(pixmapPreviewBtn));
    btnPreview->setIconSize(pixmapPreviewBtn.size());
    btnPreview->setGeometry(QRect(QPoint(4, 404), QSize(96, 96)));
    btnPreview->setToolTip(tr("Show/hide preview puzzle")+"  (Ctrl+P)");

    QPixmap pixmapAboutBtn(":res/images/buttons/about.png");
    pixmapAboutBtn = pixmapAboutBtn.scaled(96, 96, Qt::KeepAspectRatioByExpanding);

    btnAbout = new QPushButton(this);
    btnAbout->setShortcut(Qt::CTRL+Qt::Key_I);
    btnAbout->setFlat(true);
    btnAbout->setIcon(QIcon(pixmapAboutBtn));
    btnAbout->setIconSize(pixmapAboutBtn.size());
    btnAbout->setGeometry(QRect(QPoint(4, 504), QSize(96, 96)));
    btnAbout->setToolTip(tr("About app")+"  (Ctrl+I)");

    QPixmap pixmapExitBtn(":res/images/buttons/exit.png");
    pixmapExitBtn = pixmapExitBtn.scaled(96, 96, Qt::KeepAspectRatioByExpanding);

    btnExit = new QPushButton(this);
    btnExit->setShortcut(QKeySequence::Quit);
    btnExit->setFlat(true);
    btnExit->setIcon(QIcon(pixmapExitBtn));
    btnExit->setIconSize(pixmapExitBtn.size());
    btnExit->setGeometry(QRect(QPoint(4, 604), QSize(96, 96)));
    btnExit->setToolTip(tr("Exit app")+"  (Ctrl+Q)");

    // Connect button signal to appropriate slot
    connect(btnNewPuzzle,  &QPushButton::released,
            this, &MainWindow::newPuzzle);
    connect(btnSavePuzzle, &QPushButton::released,
            this, &MainWindow::savePuzzleToFile);
    connect(btnLoadPuzzle, &QPushButton::released,
            this, &MainWindow::loadPuzzle);
    connect(btnAlignment,  &QPushButton::released,
            this, &MainWindow::alignmentPuzzle);
    connect(btnPreview,    &QPushButton::released,
            this, &MainWindow::previewPuzzle);
    connect(btnAbout,      &QPushButton::released,
            this, &MainWindow::aboutApp);
    connect(btnExit,       &QPushButton::released,
            QCoreApplication::instance(), &QCoreApplication::quit);
}

void MainWindow::newPuzzle()
{
    qDebug() << "new puzzle";
}

void MainWindow::savePuzzleToFile()
{
    qDebug() << "save puzzle";
}

void MainWindow::loadPuzzle()
{
    qDebug() << "load puzzle";
}

void MainWindow::alignmentPuzzle()
{
    qDebug() << "alignment puzzle";
}

void MainWindow::previewPuzzle()
{
    qDebug() << "previewPuzzle ";
}

void MainWindow::aboutApp()
{
    qDebug() << "about App";
}

void MainWindow::exitApp()
{
    qDebug() << "exit App";
}
