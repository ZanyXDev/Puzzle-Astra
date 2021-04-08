#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    //TODO load setting from QSetting
    this->showPuzzleBeforStart = true;
    this->setWindowTitle(tr("Astra Puzzle v0.1-%1").arg(GIT_HASH));
    this->setWindowIcon(QIcon(":/res/images/puzzle.png"));

    centralWidget = new QWidget(this);
    centralLayout = new QHBoxLayout();

    centralWidget->setLayout(centralLayout);

    //    widgetTable = new QWidget(this);
    //    widgetTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //    widgetTable->setMouseTracking(true);
    //    widgetTable->installEventFilter(this);


    widgetTable = new QWidget(this);
    //TODO need auto crop load images to max_size 2048
    widgetTable->setFixedSize(2048, 2048);

    QScrollArea* m_pQScrollArea = new QScrollArea;
    m_pQScrollArea->setWidget(widgetTable);
    m_pQScrollArea->setWidgetResizable(true);
    m_pQScrollArea->viewport()->installEventFilter(this);

    //TODO set style for scrollbar
    m_pQScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_pQScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    btnLayout =new QVBoxLayout();

    setupButtons();

    centralLayout->addWidget(m_pQScrollArea,9);
    centralLayout->addLayout(btnLayout,1);
    setCentralWidget(centralWidget);
}

// ------------------- Public slots -----------------------------------------

void MainWindow::newPuzzle()
{
    //TODO load last open picture folder from config [QSettings]
    lastPath= QString("");
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open pictures"),
                lastPath,
                tr("Pictures (*.png *.xpm *.jpg *.jpeg *.tiff *.webp *.bmp)"));
    if (filename.isEmpty())
    {
        return;
    }

    //TODO save last open picture folder to config [QSettings]
    puzzleFilename = QFileInfo(filename).fileName();
    puzzlePixmap = QPixmap(filename);

    int cWidth = std::max((puzzlePixmap.width()+(2*puzzleWidth)),
                          widgetTable->width());
    int cHeight = std::max((puzzlePixmap.height()+(2*puzzleHeight)),
                           widgetTable->height());

    widgetTable->setFixedSize(cWidth, cHeight);

    puzzleAnimationGroup.clear();

#ifdef QT_DEBUG
    qDebug() << QString("Before widgetTable size w%1,h%2")
                .arg(widgetTable->width())
                .arg(widgetTable->height());
    QElapsedTimer timer;
    timer.start();
#endif

    int currentPuzzleCount = createPuzzle();

#ifdef QT_DEBUG
    qDebug() << "The slow operation took" << timer.elapsed() << "milliseconds";
    qDebug() << "The slow operation took" << timer.nsecsElapsed() << "nanoseconds";
#endif

    changeWindowSizeAnimated();
#ifdef QT_DEBUG
    qDebug() << QString("after widgetTable size w%1,h%2")
                .arg(widgetTable->width())
                .arg(widgetTable->height());
#endif
    if (showPuzzleBeforStart) {
        puzzleAnimationGroup.addPause(500);
#ifdef QT_DEBUG
        qDebug() << "Function Name: " << Q_FUNC_INFO;
        qDebug() << "puzzleAnimationGroup.animationCount:" << puzzleAnimationGroup.animationCount();
#endif
        puzzleAnimationGroup.start();
    }

    // alignmentPuzzle();

    this->setWindowTitle(QString(tr("Puzzle [%1] with %2 pieces."))
                         .arg(puzzleFilename)
                         .arg(currentPuzzleCount)
                         );
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

/**
 * @brief MainWindow::createPuzzle
 * @note  Creating a puzzle from an puzzlePixmap
 */
int MainWindow::createPuzzle()
{

    int countX = puzzlePixmap.width()/puzzleWidth;
    int countY = puzzlePixmap.height()/puzzleHeight;

    QPixmap pixPreview = puzzlePixmap.copy(0,0,countX*puzzleWidth,countY*puzzleHeight);
    //TODO setup Widget preview  picture as  preview->setPicture(pixPreview);

    /**
     * @brief qDeleteAll delete all list items and clear list
     */
    qDeleteAll(listItems.begin(), listItems.end());
    listItems.clear();

    QString typePuzzle;
    int zOrder=0;
    for (int x=0;x<countX;x++){
        for (int y=0;y<countY;y++){
            typePuzzle="1";     // default
            if ( ( !isEven( x ) &&  isEven( y ) )  ||
                 (  isEven( x ) && !isEven( y ) ) ) {
                typePuzzle="2";
            }

            if (x==0){
                if (y==0){
                    typePuzzle="1-l-t";
                }else{
                    typePuzzle=( isEven( y ) ? "1-l" : "2-l" );
                }
            }

            if ( (y==0) && (x!=0) ){
                typePuzzle=( isEven( x ) ? "1-t" : "2-t" );
            }

            if (x==countX-1){
                if (y==0){
                    typePuzzle=( isEven( countX ) ? "2-r-t" : "1-r-t" );
                }else{
                    if ( (isEven( y ) && isEven( countX ) ) ||
                         (!isEven( y ) && !isEven( countX )) ){
                        typePuzzle="2-r";
                    }else{
                        typePuzzle="1-r";
                    }
                }
            }

            if (y==countY-1){
                if (x==0){
                    typePuzzle=( isEven( y ) ? "2-l-b" : "1-l-b" );
                }else{
                    if ( (isEven( countX ) && isEven( countY )) ||
                         (!isEven( countX ) && !isEven( countY )) ){
                        typePuzzle="2-b";
                    }else{
                        typePuzzle="1-b";
                    }
                }
            }

            if (x==countX-1 && y==countY-1){
                if ( (isEven( countX ) && isEven( countY )) ||
                     (!isEven( countX ) && !isEven( countY )) ){
                    typePuzzle="1-r-b";
                }else{
                    typePuzzle="2-r-b";
                }
            }

            // create item puzzle
            QLabel *puzzle = new QLabel(widgetTable);
            puzzle->setScaledContents(true);
            puzzle->setProperty("cell_x",x);
            puzzle->setProperty("cell_y",y);
            puzzle->setProperty("type_puzzle",typePuzzle);
            puzzle->setProperty("zOrder",zOrder);
            puzzle->setAttribute(Qt::WA_TranslucentBackground);

            setPicturePuzzle(puzzle,"effect1");
            setupAnimation(puzzle,
                           x*(puzzleWidth+5),y*(puzzleHeight+5),
                           showPuzzleBeforStart);
            puzzle->show();
            listItems.push_back(std::move(puzzle));
            zOrder++;

        }
    }
    return countX * countY;
}

bool MainWindow::isEven(int number)
{
    return !(number & 1);
}

// Setting the shape of the puzzle on the image
//TODO rewrite to QPixmap getPixmap(Qt::ReturnByValueConstant) const
void MainWindow::setPicturePuzzle(QLabel *item, const QString &effect)
{
#ifdef QT_DEBUG
    qDebug() << Q_FUNC_INFO;
    QElapsedTimer timer;
    timer.start();
#endif

    QImage pix_temp = puzzlePixmap.toImage();
    QPixmap pix(pix_temp.width()+puzzleOrigWidth*2,
                pix_temp.height()+puzzleOrigHeight*2);

    QPainter painterPix(&pix);
    painterPix.drawImage(puzzleBorderWidth,puzzleBorderHeight,pix_temp,0,0);
    painterPix.end();

    QString typePuzzle = item->property("type_puzzle").toString();
    QPixmap puzzle_mask(QLatin1String(":res/images/pieces/piece%1.png").arg(typePuzzle));
    QImage puzzle_top(QLatin1String(":res/images/pieces/piece%1_%2.png").arg(typePuzzle).arg(effect));

    int cell_x = item->property("cell_x").toInt();
    int cell_y = item->property("cell_y").toInt();

    QPixmap temp = pix.copy(cell_x*puzzleWidth,cell_y*puzzleHeight,puzzleOrigWidth,puzzleOrigHeight);

    temp.setMask(puzzle_mask.createMaskFromColor(Qt::black,Qt::MaskOutColor));

    QPainter p(&temp);
    p.drawImage(0,0,puzzle_top.copy(0,0,puzzleOrigWidth,puzzleOrigHeight),0,0);
    p.end();
    item->setPixmap(temp);

#ifdef QT_DEBUG
    qDebug() << "The slow operation took" << timer.elapsed() << "milliseconds";
    qDebug() << "The slow operation took" << timer.nsecsElapsed() << "nanoseconds";
#endif
#ifdef QT_DEBUG_OFF
    QString fn = QString("/tmp/%1_%2piece%3.jpg")
            .arg(cell_x)
            .arg(cell_y)
            .arg(typePuzzle);
    temp.save(fn,"JPG");
#endif
}

void MainWindow::setupAnimation(QLabel *item, int pos_x, int pos_y, bool mode)
{
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
    QPropertyAnimation *anim = new QPropertyAnimation(this,"geometry");
    int cWidth = std::max((puzzlePixmap.width()+(2*puzzleWidth)),
                          this->width());
    int cHeight = std::max((puzzlePixmap.height()+(2*puzzleHeight)),
                           this->height());

    anim->setDuration(300);
    anim->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    anim->setEndValue(QRect(this->x(),this->y(),cWidth,cHeight));
    anim->start();
    qDebug() << QString("puzzlePixmap size w%1,h%2")
                .arg(puzzlePixmap.width())
                .arg(puzzlePixmap.height());

    qDebug() << QString("New wnd size w%1,h%2").arg(cWidth).arg(cHeight);
}
