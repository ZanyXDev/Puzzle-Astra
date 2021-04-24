#include "puzzleboardview.h"

PuzzleBoardView::PuzzleBoardView(QWidget *parent)
    : QGraphicsView(parent)
    , isPuzzleLoad(false)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //this->setAlignment(Qt::AlignCenter);
    //this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_scene.setSceneRect(0,0,this->width(),this->height());
    setScene(&m_scene);
    //resizeEvent( nullptr );
    //TODO change  fitInView to Scale factor
    // fitInView(m_scene.sceneRect(), Qt::KeepAspectRatio);
}

void PuzzleBoardView::setPixmap(const QPixmap &pixmap)
{

    if (!pixmap.isNull()){
        puzzlePixmap = pixmap;
        isPuzzleLoad = true;
        m_scene.clear();
        m_scene.setSceneRect(QRectF(0,0,puzzlePixmap.width(),puzzlePixmap.height()));

        qDebug() << Q_FUNC_INFO;
        qDebug() << "PuzzleBoardView->sceneRect()"<< this->sceneRect();
        qDebug() << "m_scene.sceneRect() " <<m_scene.sceneRect();

        QRectF rect(0,0,80,40);
        QBrush myBrush(Qt::darkGray, Qt::Dense5Pattern);

        QGraphicsRectItem *rItem1 = new QGraphicsRectItem(rect);

        rItem1->setFlag(QGraphicsItem::ItemIsMovable);
        rItem1->setPos(160,40);
        rItem1->setBrush(myBrush);
        m_scene.addItem(rItem1);
        //createPuzzle();
        // m_scene.update();
        // fitInView(m_scene.sceneRect(), Qt::KeepAspectRatio);
    }

}

void PuzzleBoardView::createPuzzle()
{
    if (isPuzzleLoad){
        countPuzzleWidth = puzzlePixmap.width()/puzzleWidth;
        countPuzzleHeight = puzzlePixmap.height()/puzzleHeight;

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
        for (int x=0;x<countPuzzleWidth;x++){
            for (int y=0;y<countPuzzleHeight;y++){
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

                if (x==countPuzzleWidth-1){
                    if (y==0){
                        typePuzzle=( isEven( countPuzzleWidth ) ? "2-r-t" : "1-r-t" );
                    }else{
                        if ( (isEven( y ) && isEven( countPuzzleWidth ) ) ||
                             (!isEven( y ) && !isEven( countPuzzleWidth )) ){
                            typePuzzle="2-r";
                        }else{
                            typePuzzle="1-r";
                        }
                    }
                }

                if (y==countPuzzleHeight-1){
                    if (x==0){
                        typePuzzle=( isEven( countPuzzleHeight ) ? "2-l-b" : "1-l-b" );
                    }else{
                        if ( (isEven( x ) && isEven( countPuzzleHeight )) ||
                             (!isEven( x ) && !isEven( countPuzzleHeight )) ){
                            typePuzzle="2-b";
                        }else{
                            typePuzzle="1-b";
                        }
                    }
                }

                if (x==countPuzzleWidth-1 && y==countPuzzleHeight-1){
                    if ( (isEven( countPuzzleWidth ) && isEven( countPuzzleHeight )) ||
                         (!isEven( countPuzzleWidth ) && !isEven( countPuzzleHeight )) ){
                        typePuzzle="1-r-b";
                    }else{
                        typePuzzle="2-r-b";
                    }
                }

                // create item puzzle
                QGraphicsPixmapItem *puzzle = new QGraphicsPixmapItem();
                puzzle->setFlag(QGraphicsItem::ItemIsMovable);
                puzzle->setShapeMode(QGraphicsPixmapItem::HeuristicMaskShape);
                //puzzle->setFlag(QGraphicsItem::ItemIsSelectable);
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

        emit puzzleCounts(countPuzzleWidth * countPuzzleHeight);
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

void PuzzleBoardView::resizeEvent(QResizeEvent *event)
{
    //fitInView(m_scene.sceneRect(), Qt::KeepAspectRatio);
    static const int GRAPHICS_VIEW_MARGIN = 20;
    double scale = std::min(
                ( this->width() - GRAPHICS_VIEW_MARGIN ) / m_scene.width(),
                ( this->height() - GRAPHICS_VIEW_MARGIN ) / m_scene.height()
                );
    //    qDebug() << Q_FUNC_INFO;
    //    qDebug() << QString("PuzzleBoardView size[%1,%2]").arg(this->width()).arg(this->height());
    //    qDebug() << "PuzzleBoardView->sceneRect()"<< this->sceneRect();
    //    qDebug() << "m_scene.sceneRect()"<< m_scene.sceneRect();
    //    qDebug() << "scale" << scale;

    //this->resetTransform();
    //this->scale( scale, scale );
    Q_UNUSED(event);
}

bool PuzzleBoardView::isEven(int number)
{
    return !(number & 1);
}
