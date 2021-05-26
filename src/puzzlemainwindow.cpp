#include "puzzlemainwindow.h"
#include "welcomedialog.h"

PuzzleMainWindow::PuzzleMainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    // Initialize variables
    justLaunchedWithImage = false;
    storedWindowState = Qt::WindowNoState;

    setWindowTitle(tr("Astra Puzzle v0.2-%1").arg(GIT_HASH));
    setWindowIcon(QIcon(":/res/images/puzzle.png"));

    // Initialize graphicsview
    graphicsView = new PuzzleGraphicsView(this);

    auto *centralWidget = new QWidget(this);
    auto  centralLayout = new QVBoxLayout();
    centralLayout->addWidget(graphicsView);
    centralWidget->setLayout(centralLayout);

    // Connect graphicsview signals
    // connect(graphicsView, &PuzzleGraphicsView::updatedLoadedPixmapItem, this, &PuzzleMainWindow::setWindowSize);

    // Initialize escape shortcut
    escShortcut = new QShortcut(Qt::Key_Escape, this);
    connect(escShortcut, &QShortcut::activated, this, [this](){
        if (windowState() == Qt::WindowFullScreen)
            toggleFullScreen();
    });

    setupActions();
    setContextMenuPolicy(Qt::CustomContextMenu);
    // Context menu
    //setupContextMenu();
    connect(this, &PuzzleMainWindow::customContextMenuRequested,
            this, &PuzzleMainWindow::ShowContextMenu);
    // Load window geometry
    QSettings settings;
    // Check first launch
    if ( ! settings.value("firstlaunch", false).toBool() ){
        settings.setValue("firstlaunch", true);
        settings.setValue("configversion", QString("%1").arg(VERSION));

        settings.beginGroup("options");
        settings.setValue("hideNotification", false);
        settings.endGroup();
        settings.sync();
    }else{
        //restore settings
        //TODO check geometry to empty
        //restoreGeometry(settings.value("geometry").toByteArray());
        lastPath = settings.value("options/lastPath",QString("")).toString();
    }

    if (  !settings.value("options/hideNotification").toBool() ){
        // Show welcome dialog
        openWelcomeDialog(this);
    }
}


void PuzzleMainWindow::setJustLaunchedWithImage(bool value)
{
    justLaunchedWithImage = value;
}

void PuzzleMainWindow::toggleFullScreen()
{
    qDebug() << Q_FUNC_INFO;

    if (windowState() == Qt::WindowFullScreen){
        setWindowState(storedWindowState);
    } else {
        storedWindowState = windowState();
        showFullScreen();
    }
}

void PuzzleMainWindow::openWelcomeDialog(QWidget *parent)
{
#ifdef Q_OS_MACOS
    // On macOS, the dialog should not be dependent on any window
    parent = nullptr;
#endif
    auto *welcomeDialog = new WelcomeDialog(parent);
    welcomeDialog->show();
}

void PuzzleMainWindow::openFile()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open pictures"),
                lastPath,
                tr("Pictures (*.png *.xpm *.jpg *.jpeg *.tiff *.webp *.bmp)"));

    if (filename.isEmpty()){
        return;
    }

    puzzleFilename = QFileInfo(filename).fileName();
    lastPath = QFileInfo(filename).dir().canonicalPath();
}

void PuzzleMainWindow::ShowContextMenu(const QPoint &pos)
{
    QMenu contextMenu(tr("Context menu"), this);
    contextMenu.addAction(actNewPuzzle);
    contextMenu.addAction(actSavePuzzle);
    contextMenu.addSeparator();
    contextMenu.addAction(actToggleFullScreen);
    contextMenu.exec(mapToGlobal(pos));
}

void PuzzleMainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.beginGroup("options");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("lastPath",lastPath);
    settings.endGroup();
    settings.sync();
    QMainWindow::closeEvent(event);
}

void PuzzleMainWindow::setupContextMenu()
{
    contextMenu = new QMenu(this);
}

void PuzzleMainWindow::setupActions()
{
    actNewPuzzle = new QAction(tr("&Open image ..."), this);
    actNewPuzzle->setShortcut(QKeySequence::Open);
    actNewPuzzle->setStatusTip(tr("Open image and create puzzle"));
    connect(actNewPuzzle, &QAction::triggered, this, &PuzzleMainWindow::openFile);

    actSavePuzzle = new QAction(tr("&Save puzzle ..."), this);
    actSavePuzzle->setShortcut(QKeySequence::Save);
    actSavePuzzle->setStatusTip(tr("Save puzzle to file"));
    // connect(actNewPuzzle, &QAction::triggered, this, &PuzzleMainWindow::newFile);

    //    actAlignment;
    //    actPreview;
    //    actAbout;
    actToggleFullScreen = new QAction(tr("&Toggel fullscreen"), this);
    actToggleFullScreen->setShortcut(Qt::Key_F11);
    actToggleFullScreen->setStatusTip(tr("Toggle fullscreen mode"));
    connect(actToggleFullScreen, &QAction::triggered, this, &PuzzleMainWindow::toggleFullScreen);
    //    actExit;
}
