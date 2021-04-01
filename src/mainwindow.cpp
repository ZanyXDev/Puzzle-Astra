#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setWindowTitle(tr("Astra Puzzle v0.1-%1").arg(GIT_HASH));
    this->setWindowIcon(QIcon(":/res/images/puzzle.png"));

    centralWidget = new QWidget();
    centralLayout = new QHBoxLayout();

    centralWidget->setLayout(centralLayout);

    widgetTable = new QLabel();
    widgetTable->setFrameShape(QFrame::Box);

    // background form buttons
    //    backgroundButtons = new QLabel(this);
    //    backgroundButtons->setPixmap(QPixmap(":/res/images/background.png"));
    //    backgroundButtons->setScaledContents(true);
    //    backgroundButtons->installEventFilter(this);
    btnLayout =new QVBoxLayout();

    setupButtons();

    centralLayout->addWidget(widgetTable,9);
    centralLayout->addLayout(btnLayout,1);
    setCentralWidget(centralWidget);
}

// ------------------- Public slots -----------------------------------------

void MainWindow::newPuzzle()
{
    qDebug() << "new puzzle";
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

    this->setWindowTitle(QString(tr("Puzzle [%1]")).arg(puzzleFilename));

    createPuzzle();
    alignmentPuzzle();
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
    quint32 x;
    quint32 y;
    QRandomGenerator *gen = QRandomGenerator::system();
    for (auto *item:listItems){
        x = gen->bounded(0,centralWidget->width()-puzzleWidth-100);
        y = gen->bounded(0,centralWidget->height()-puzzleHeight-100);
        item->move(0-widgetTable->x()+x,0-widgetTable->y()+y);

        setPicturePuzzle(item,"effect1");
        item->show();
    }
}

void MainWindow::previewPuzzle()
{
    qDebug() << "previewPuzzle ";
}

void MainWindow::aboutApp()
{
    qDebug() << "about App";
}


// ------------------- Private functions ------------------------------------

/**
 * @brief MainWindow::createPuzzle
 * @note  Creating a puzzle from an puzzlePixmap
 */
void MainWindow::createPuzzle()
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
                typePuzzle=( isEven( x ) ? "1-t" : "2-2" );
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
            if (x==countX-1 && y==countY-1){
                if ( (isEven( countX ) && isEven( countY )) ||
                     (!isEven( countX ) && !isEven( countY )) ){
                    typePuzzle="1-r-b";
                }else{
                    typePuzzle="2-r-b";
                }
            }

            // create item puzzle
            QLabel *puzzle = new QLabel(this);//(widgetTable);
            puzzle->setGeometry(x*(puzzleWidth+20),10+y*(puzzleHeight+20),puzzleOrigWidth,puzzleOrigHeight);
            puzzle->setScaledContents(true);
            puzzle->setProperty("cell_x",x);
            puzzle->setProperty("cell_y",y);
            puzzle->setProperty("type_puzzle",typePuzzle);
            puzzle->setProperty("zOrder",zOrder);
            //setPicturePuzzle(puzzle,"effect1");
            //puzzle->show();
            listItems.push_back(std::move(puzzle));
            zOrder++;

        }
    }

}

bool MainWindow::isEven(int number)
{
    return !(number & 1);
}

// Setting the shape of the puzzle on the image
//TODO rewrite to QPixmap getPixmap(Qt::ReturnByValueConstant) const
void MainWindow::setPicturePuzzle(QLabel *item, const QString &effect)
{
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

}

void MainWindow::setupButtons()
{
    QPixmap pixmapNewBtn(":res/images/buttons/new.png");

    pixmapNewBtn = pixmapNewBtn.scaled(96, 96, Qt::KeepAspectRatioByExpanding);
    QSize btnSize = pixmapNewBtn.size();

    btnNewPuzzle = new QPushButton(this);
    btnNewPuzzle->setShortcut(QKeySequence::New);
    btnNewPuzzle->setFlat(true);
    btnNewPuzzle->setIcon(QIcon(pixmapNewBtn));
    btnNewPuzzle->setIconSize(btnSize);
    btnNewPuzzle->setStyleSheet("QPushButton{border-radius:5px;border: 1px solid transparent; background: transparent}");
    btnNewPuzzle->setToolTip(tr("New picture from file")+"  (Ctrl+N)");

    QPixmap pixmapSaveBtn(":res/images/buttons/save.png");
    pixmapSaveBtn = pixmapSaveBtn.scaled(96, 96, Qt::KeepAspectRatioByExpanding);

    btnSavePuzzle = new QPushButton(this);
    btnSavePuzzle->setShortcut(QKeySequence::Save);
    btnSavePuzzle->setFlat(true);
    btnSavePuzzle->setIcon(QIcon(pixmapSaveBtn));
    btnSavePuzzle->setIconSize(btnSize);
    btnSavePuzzle->setStyleSheet("QPushButton{border-radius:5px;border: 1px solid transparent; background: transparent}");
    btnSavePuzzle->setToolTip(tr("Save puzzle to file")+"  (Ctrl+S)");

    QPixmap pixmapLoadBtn(":res/images/buttons/open.png");
    pixmapLoadBtn = pixmapLoadBtn.scaled(96, 96, Qt::KeepAspectRatioByExpanding);

    btnLoadPuzzle = new QPushButton(this);
    btnLoadPuzzle->setShortcut(Qt::CTRL+Qt::Key_L);
    btnLoadPuzzle->setFlat(true);
    btnLoadPuzzle->setIcon(QIcon(pixmapLoadBtn));
    btnLoadPuzzle->setIconSize(btnSize);
    btnLoadPuzzle->setStyleSheet("QPushButton{border-radius:5px;border: 1px solid transparent; background: transparent}");
    btnLoadPuzzle->setToolTip(tr("Load puzzle from file")+"  (Ctrl+L)");

    QPixmap pixmapAligBtn(":res/images/buttons/alignment.png");
    pixmapAligBtn = pixmapAligBtn.scaled(96, 96, Qt::KeepAspectRatioByExpanding);

    btnAlignment = new QPushButton(this);
    btnAlignment->setShortcut(Qt::CTRL+Qt::Key_A);
    btnAlignment->setFlat(true);
    btnAlignment->setIcon(QIcon(pixmapAligBtn));
    btnAlignment->setIconSize(btnSize);
    btnAlignment->setStyleSheet("QPushButton{border-radius:5px;border: 1px solid transparent; background: transparent}");
    btnAlignment->setToolTip(tr("Alignment")+"  (Ctrl+A)");

    QPixmap pixmapPreviewBtn(":res/images/buttons/preview.png");
    pixmapPreviewBtn = pixmapPreviewBtn.scaled(96, 96, Qt::KeepAspectRatioByExpanding);

    btnPreview = new QPushButton(this);
    btnPreview->setShortcut(QKeySequence::Print);
    btnPreview->setFlat(true);
    btnPreview->setIcon(QIcon(pixmapPreviewBtn));
    btnPreview->setIconSize(btnSize);
    btnPreview->setStyleSheet("QPushButton{border-radius:5px;border: 1px solid transparent; background: transparent}");
    btnPreview->setToolTip(tr("Show/hide preview puzzle")+"  (Ctrl+P)");

    QPixmap pixmapAboutBtn(":res/images/buttons/about.png");
    pixmapAboutBtn = pixmapAboutBtn.scaled(96, 96, Qt::KeepAspectRatioByExpanding);

    btnAbout = new QPushButton(this);
    btnAbout->setShortcut(Qt::CTRL+Qt::Key_I);
    btnAbout->setFlat(true);
    btnAbout->setIcon(QIcon(pixmapAboutBtn));
    btnAbout->setIconSize(btnSize);
    btnAbout->setStyleSheet("QPushButton{border-radius:5px;border: 1px solid transparent; background: transparent}");
    btnAbout->setToolTip(tr("About app")+"  (Ctrl+I)");

    QPixmap pixmapExitBtn(":res/images/buttons/exit.png");
    pixmapExitBtn = pixmapExitBtn.scaled(96, 96, Qt::KeepAspectRatioByExpanding);

    btnExit = new QPushButton(this);
    btnExit->setShortcut(QKeySequence::Quit);
    btnExit->setFlat(true);
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
