#include "puzzlemainwindow.h"
#include "welcomedialog.h"

PuzzleMainWindow::PuzzleMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
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

    // Context menu
    contextMenu = new QMenu(this);

    // Load window geometry
    QSettings settings;
    // Check first launch
    auto firstLaunch = settings.value("firstlaunch", false).toBool();
    if ( firstLaunch ){
        settings.setValue("firstlaunch", true);
        settings.setValue("configversion", QString("%1-%2").arg(VERSION).arg(GIT_HASH));
        // Show welcome dialog on first launch
        openWelcomeDialog(this);
    }else{
        restoreGeometry(settings.value("geometry").toByteArray());
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

void PuzzleMainWindow::openFile(const QString &fileName)
{

}
