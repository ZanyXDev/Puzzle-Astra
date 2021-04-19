#include "puzzleboardview.h"

PuzzleBoardView::PuzzleBoardView(QWidget *parent)
    : QGraphicsView(parent)
    , isPuzzleLoad(false)
    ,  m_scene(0, 0, this->width(), this->height(), this)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setAlignment(Qt::AlignCenter);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setScene(&m_scene);
    fitInView(m_scene.sceneRect(), Qt::KeepAspectRatio);
}

void PuzzleBoardView::setPixmap(const QPixmap &pixmap)
{
    if (!pixmap.isNull()){
        puzzlePixmap = pixmap;
        int cWidth = std::max((puzzlePixmap.width()+(2*puzzleWidth)),
                              this->width());
        int cHeight = std::max((puzzlePixmap.height()+(2*puzzleHeight)),
                               this->height());
        this->setPuzzleRect(QRectF(0,0,cWidth,cHeight));
        isPuzzleLoad = true;
        createPuzzle();
    }

}

void PuzzleBoardView::createPuzzle()
{
    if (isPuzzleLoad){
        int countX = puzzlePixmap.width()/puzzleWidth;
        int countY = puzzlePixmap.height()/puzzleHeight;

        /** TODO setup Widget preview
  * picture as  preview->setPicture(pixPreview);
  * QPixmap pixPreview = puzzlePixmap.copy(0,0,countX*puzzleWidth,countY*puzzleHeight);
  */
        QImage pix_temp = puzzlePixmap.toImage();
        QPixmap pix(pix_temp.width()+puzzleOrigWidth*2,
                    pix_temp.height()+puzzleOrigHeight*2);

        QPainter painterPix(&pix);
        painterPix.drawImage(puzzleBorderWidth,puzzleBorderHeight,pix_temp,0,0);
        painterPix.end();

        QString typePuzzle;
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
                        typePuzzle=( isEven( countY ) ? "2-l-b" : "1-l-b" );
                    }else{
                        if ( (isEven( x ) && isEven( countY )) ||
                             (!isEven( x ) && !isEven( countY )) ){
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
                QGraphicsPixmapItem *puzzle = new QGraphicsPixmapItem();
                puzzle->setFlag(QGraphicsItem::ItemIsMovable);
                puzzle->setPos(x * puzzleWidth,y * puzzleHeight);
                puzzle->setData(0,typePuzzle);
                puzzle->setData(1,"effect1");

                QPixmap temp = pix.copy(x*puzzleWidth,y*puzzleHeight,
                                        puzzleOrigWidth,puzzleOrigHeight);
                puzzle->setPixmap(temp);

                QFuture<QPixmap> future = QtConcurrent::run(this, &PuzzleBoardView::makePicturePuzzle,
                                                            temp, typePuzzle,QString("effect1"));

                puzzle->setPixmap( future.result() );

                m_scene.addItem(puzzle);
            }
        }
        emit puzzleCounts(countX * countY);
    }
}

QPixmap PuzzleBoardView::makePicturePuzzle(const QPixmap &pixmap, QString puzzleType, QString effectType) const
{
    QPixmap puzzle_mask(QLatin1String(":res/images/pieces/piece%1.png").arg(puzzleType));
    QImage puzzle_top(QLatin1String(":res/images/pieces/piece%1_%2.png").arg(puzzleType).arg(effectType));

    QPixmap pm = pixmap;
    pm.setMask(puzzle_mask.createMaskFromColor(Qt::black,Qt::MaskOutColor));

    QPainter p(&pm);
    p.drawImage(0,0,puzzle_top.copy(0,0,200,165),0,0);
    p.end();
    return pm;
}

void PuzzleBoardView::setPuzzleRect(QRectF m_rect)
{
    m_scene.setSceneRect(m_rect);
    m_scene.clear();
}

void PuzzleBoardView::resizeEvent(QResizeEvent *event)
{
    fitInView(m_scene.sceneRect(), Qt::KeepAspectRatio);
    Q_UNUSED(event);
}

bool PuzzleBoardView::isEven(int number)
{
    return !(number & 1);
}
