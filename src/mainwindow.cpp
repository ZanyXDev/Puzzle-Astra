#include "mainwindow.h"
#include "puzzlepiece.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    //TODO load setting from QSetting
    this->showPuzzleBeforStart = true; //TODO
    this->setWindowTitle(tr("Astra Puzzle v0.2-%1").arg(GIT_HASH));
    this->setWindowIcon(QIcon(":/res/images/puzzle.png"));

#ifdef QT_DEBUG
    QGraphicsDropShadowEffect *e = new QGraphicsDropShadowEffect;
    e->setColor(QColor(40,40,40,245));
    e->setOffset(0,10);
    e->setBlurRadius(50);

    QPixmap puzzle_pic1(QLatin1String(":res/images/pieces/piece1-b.png"));

    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->setSceneRect(0,0,640,480);
    PuzzlePiece *item1 = new PuzzlePiece();

    item1->setFlags(

                QGraphicsItem::ItemSendsGeometryChanges);
    item1->setPixmap(puzzle_pic1);
    //item->setShapeMode(QGraphicsPixmapItem::HeuristicMaskShape);
    item1->setShapeMode(QGraphicsPixmapItem::MaskShape);
    scene->addItem(item1);

    QPixmap puzzle_pic2(QLatin1String(":res/images/pieces/piece2-b.png"));


    PuzzlePiece *item2 = new PuzzlePiece();

    item2->setFlags(
                QGraphicsItem::ItemSendsGeometryChanges);
    item2->setPixmap(puzzle_pic2);
    item2->setShapeMode(QGraphicsPixmapItem::MaskShape);
    scene->addItem(item2);

    QPixmap puzzle_pic3(QLatin1String(":res/images/pieces/piece1-l-b.png"));

    PuzzlePiece *item3 = new PuzzlePiece();
    item3->setGraphicsEffect(e);

    item3->setFlags(
                QGraphicsItem::ItemSendsGeometryChanges);
    item3->setPixmap(puzzle_pic3);
    //item->setShapeMode(QGraphicsPixmapItem::HeuristicMaskShape);
    item3->setShapeMode(QGraphicsPixmapItem::MaskShape);
    scene->addItem(item3);

    QGraphicsView *view = new QGraphicsView(scene);
    setCentralWidget(view);

#else
    centralWidget = new QWidget(this);
    centralLayout = new QHBoxLayout();
    btnLayout     = new QVBoxLayout();

    centralWidget->setLayout(centralLayout);

    widgetTable = new PuzzleBoardView(this);
    connect(widgetTable,&PuzzleBoardView::puzzleCounts,
            this,&MainWindow::changePuzzleCountValue);
    setupButtons();

    centralLayout->addWidget(widgetTable,9);
    centralLayout->addLayout(btnLayout,1);
    setCentralWidget(centralWidget);
#endif
}

// ------------------- Public slots -----------------------------------------

void MainWindow::newPuzzle()
{
    //TODO move to Engine class
    //TODO load last open picture folder from config [QSettings]
    lastPath= QString("");
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open pictures"),
                lastPath,
                tr("Pictures (*.png *.xpm *.jpg *.jpeg *.tiff *.webp *.bmp)"));

    if (filename.isEmpty()){
        return;
    }

    //TODO save last open picture folder to config [QSettings]
    puzzleFilename = QFileInfo(filename).fileName();
    widgetTable->setPixmap( QPixmap(filename) );


#ifdef QT_DEBUG
    QElapsedTimer timer;
    timer.start();
    //   QRect geometry = btnLayout->geometry();
#endif


    //changeWindowSizeAnimated();



    if (showPuzzleBeforStart) {
        //puzzleAnimationGroup.addPause(500);
        //TODO warning cannot pause stoped animation
        puzzleAnimationGroup.pause();
        puzzleAnimationGroup.start();
    }

    // alignmentPuzzle();


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
    //TODO move to Engine class
    puzzleAnimationGroup.clear();

    for (auto *item:qAsConst(listItems)){ // ok, no detach attempt
        setupAnimation(item,0,0,false);
    }
#ifdef QT_DEBUG
    qDebug() << "Function Name: " << Q_FUNC_INFO;
    qDebug() << "puzzleAnimationGroup.animationCount:" << puzzleAnimationGroup.animationCount();
#endif
    puzzleAnimationGroup.start();
}

void MainWindow::previewPuzzle()
{
    qDebug() << "previewPuzzle ";
}

void MainWindow::aboutApp()
{
    qDebug() << "about App";
}

void MainWindow::changePuzzleCountValue(int m_count)
{
    currentPuzzleCount = m_count;
    //TODO add bottom window info line
    setWindowTitle(QString(tr("Puzzle [%1] with %2 pieces."))
                   .arg(puzzleFilename)
                   .arg(currentPuzzleCount)
                   );
}


// ------------------- Protected functions
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseMove){
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        qDebug() << QString("Mouse move (%1,%2)").arg(mouseEvent->pos().x()).arg(mouseEvent->pos().y());
    }
    return false;
}
// ------------------- Private functions ------------------------------------


void MainWindow::setupAnimation(QLabel *item, int pos_x, int pos_y, bool mode)
{
    /*
    QPropertyAnimation *anim = new QPropertyAnimation(item,"geometry");

    anim->setStartValue(QRect(0,0,puzzleOrigWidth,puzzleOrigHeight));
    if (mode){
        anim->setDuration(50);
        anim->setEndValue(QRect(pos_x,pos_y,puzzleOrigWidth,puzzleOrigHeight));
        anim->setEasingCurve(QEasingCurve::InCurve);
    }else{
        anim->setDuration(25);
        anim->setEndValue(QRect(QRandomGenerator::global()->bounded(0,centralWidget->width()-puzzleWidth-100),
                                QRandomGenerator::global()->bounded(0,centralWidget->height()-puzzleHeight-100),
                                puzzleOrigWidth,puzzleOrigHeight));
        anim->setEasingCurve(QEasingCurve::InCubic);
    }

    puzzleAnimationGroup.addAnimation(anim);
    */
}

void MainWindow::setupButtons()
{
    QPixmap pixmapNewBtn(":res/images/buttons/new.png");

    pixmapNewBtn = pixmapNewBtn.scaled(96, 96, Qt::KeepAspectRatioByExpanding);
    QSize btnSize = pixmapNewBtn.size();

    btnNewPuzzle = new QPushButton(this);
    btnNewPuzzle->setShortcut(QKeySequence::New);
    btnNewPuzzle->setIcon(QIcon(pixmapNewBtn));
    btnNewPuzzle->setIconSize(btnSize);
    btnNewPuzzle->setStyleSheet("QPushButton{border-radius:5px;border: 1px solid transparent; background: transparent}");
    btnNewPuzzle->setToolTip(tr("New picture from file")+"  (Ctrl+N)");

    QPixmap pixmapSaveBtn(":res/images/buttons/save.png");
    pixmapSaveBtn = pixmapSaveBtn.scaled(96, 96, Qt::KeepAspectRatioByExpanding);

    btnSavePuzzle = new QPushButton(this);
    btnSavePuzzle->setShortcut(QKeySequence::Save);
    btnSavePuzzle->setIcon(QIcon(pixmapSaveBtn));
    btnSavePuzzle->setIconSize(btnSize);
    btnSavePuzzle->setStyleSheet("QPushButton{border-radius:5px;border: 1px solid transparent; background: transparent}");
    btnSavePuzzle->setToolTip(tr("Save puzzle to file")+"  (Ctrl+S)");

    QPixmap pixmapLoadBtn(":res/images/buttons/open.png");
    pixmapLoadBtn = pixmapLoadBtn.scaled(96, 96, Qt::KeepAspectRatioByExpanding);

    btnLoadPuzzle = new QPushButton(this);
    btnLoadPuzzle->setShortcut(Qt::CTRL+Qt::Key_L);
    btnLoadPuzzle->setIcon(QIcon(pixmapLoadBtn));
    btnLoadPuzzle->setIconSize(btnSize);
    btnLoadPuzzle->setStyleSheet("QPushButton{border-radius:5px;border: 1px solid transparent; background: transparent}");
    btnLoadPuzzle->setToolTip(tr("Load puzzle from file")+"  (Ctrl+L)");

    QPixmap pixmapAligBtn(":res/images/buttons/alignment.png");
    pixmapAligBtn = pixmapAligBtn.scaled(96, 96, Qt::KeepAspectRatioByExpanding);

    btnAlignment = new QPushButton(this);
    btnAlignment->setShortcut(Qt::CTRL+Qt::Key_A);
    btnAlignment->setIcon(QIcon(pixmapAligBtn));
    btnAlignment->setIconSize(btnSize);
    btnAlignment->setStyleSheet("QPushButton{border-radius:5px;border: 1px solid transparent; background: transparent}");
    btnAlignment->setToolTip(tr("Alignment")+"  (Ctrl+A)");

    QPixmap pixmapPreviewBtn(":res/images/buttons/preview.png");
    pixmapPreviewBtn = pixmapPreviewBtn.scaled(96, 96, Qt::KeepAspectRatioByExpanding);

    btnPreview = new QPushButton(this);
    btnPreview->setShortcut(QKeySequence::Print);
    btnPreview->setIcon(QIcon(pixmapPreviewBtn));
    btnPreview->setIconSize(btnSize);
    btnPreview->setStyleSheet("QPushButton{border-radius:5px;border: 1px solid transparent; background: transparent}");
    btnPreview->setToolTip(tr("Show/hide preview puzzle")+"  (Ctrl+P)");

    QPixmap pixmapAboutBtn(":res/images/buttons/about.png");
    pixmapAboutBtn = pixmapAboutBtn.scaled(96, 96, Qt::KeepAspectRatioByExpanding);

    btnAbout = new QPushButton(this);
    btnAbout->setShortcut(Qt::CTRL+Qt::Key_I);
    btnAbout->setIcon(QIcon(pixmapAboutBtn));
    btnAbout->setIconSize(btnSize);
    btnAbout->setStyleSheet("QPushButton{border-radius:5px;border: 1px solid transparent; background: transparent}");
    btnAbout->setToolTip(tr("About app")+"  (Ctrl+I)");

    QPixmap pixmapExitBtn(":res/images/buttons/exit.png");
    pixmapExitBtn = pixmapExitBtn.scaled(96, 96, Qt::KeepAspectRatioByExpanding);

    btnExit = new QPushButton(this);
    btnExit->setShortcut(QKeySequence::Quit);
    btnExit->setIcon(QIcon(pixmapExitBtn));
    btnExit->setIconSize(btnSize);
    btnExit->setStyleSheet("QPushButton{border-radius:5px;border: 1px solid transparent; background: transparent}");
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
    btnLayout->addWidget(btnNewPuzzle);
    btnLayout->addWidget(btnSavePuzzle);
    btnLayout->addWidget(btnLoadPuzzle);
    btnLayout->addWidget(btnAlignment);
    btnLayout->addWidget(btnPreview);
    btnLayout->addWidget(btnAbout);
    btnLayout->addWidget(btnExit);

}

void MainWindow::changeWindowSizeAnimated()
{
    /*
    QPropertyAnimation *anim = new QPropertyAnimation(this,"geometry");
    int cWidth = std::max((puzzlePixmap.width()+(2*puzzleWidth)),
                          this->width());
    int cHeight = std::max((puzzlePixmap.height()+(2*puzzleHeight)),
                           this->height());

    anim->setDuration(300);
    anim->setStartValue(QRect(this->x(),this->y(),640,480));
    anim->setEndValue(QRect(this->x(),this->y(),cWidth,cHeight));
    anim->start();

    qDebug() << QString("puzzlePixmap size w%1,h%2")
                .arg(puzzlePixmap.width())
                .arg(puzzlePixmap.height());

    qDebug() << QString("New wnd size w%1,h%2").arg(cWidth).arg(cHeight);
    */
}
