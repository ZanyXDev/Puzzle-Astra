#include "puzzleboardview.h"
#include "puzzlepiece.h"

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
#ifdef QT_DEBUG
        qDebug() << Q_FUNC_INFO;
        qDebug() << "PuzzleBoardView->sceneRect()"<< this->sceneRect() << " m_scene.sceneRect() " <<m_scene.sceneRect();

#endif
        createPuzzle();
        //m_scene.update();
        //fitInView(m_scene.sceneRect(), Qt::KeepAspectRatio);
        resizeEvent( NULL );
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

#ifdef QT_DEBUG
        // show scene rect

        QBrush myBrush(Qt::darkGray, Qt::Dense5Pattern);
        QGraphicsRectItem *rItem1 = new QGraphicsRectItem(m_scene.sceneRect());
        rItem1->setBrush(myBrush);
        m_scene.addItem(rItem1);



        QPixmap temp_1 = pix.copy(puzzleWidth,puzzleHeight,
                                  puzzleOrigWidth,puzzleOrigHeight);
        // QPixmap temp_2(QLatin1String(":res/images/pieces/piece2-b.png"));

        temp_1.save("/tmp/temp_1.jpg");

        QFuture<QPixmap> future = QtConcurrent::run(this, &PuzzleBoardView::makePicturePuzzle,
                                                    temp_1, QString("2-r-t"),QString("effect1"));

        QPixmap temp_2 = future.result();
        temp_2.save("/tmp/temp_2.jpg");

        PuzzlePiece *item2 = new PuzzlePiece();

        item2->setFlags( QGraphicsItem::ItemSendsGeometryChanges);
        item2->setPixmap( temp_2 );

        //item2->setShapeMode(QGraphicsPixmapItem::MaskShape);
        m_scene.addItem(item2);

#else
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

                QPixmap temp = pix.copy(x*puzzleWidth,y*puzzleHeight,
                                        puzzleOrigWidth,puzzleOrigHeight);

                // create item puzzle
#ifdef QT_DEBUG
                //                QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
                //                effect->setColor(QColor(40,40,40,245));
                //                effect->setOffset(0,10);
                //                effect->setBlurRadius(50);


                temp.save(QString("/tmp/%1x%2.jpg").arg(x).arg(y));
                PuzzlePiece *puzzle = new PuzzlePiece();
                puzzle->setFlags(QGraphicsItem::ItemSendsGeometryChanges);
                puzzle->setShapeMode(QGraphicsPixmapItem::MaskShape);
                // puzzle->setGraphicsEffect(effect);
#else

                QGraphicsPixmapItem *puzzle = new QGraphicsPixmapItem();
                puzzle->setFlag(QGraphicsItem::ItemIsMovable);
                puzzle->setShapeMode(QGraphicsPixmapItem::HeuristicMaskShape);
                //puzzle->setFlag(QGraphicsItem::ItemIsSelectable);

#endif

                puzzle->setPos(x * (puzzleWidth+5),y * (puzzleHeight+5));
                puzzle->setData(0,typePuzzle);
                puzzle->setData(1,"effect1");
                //   puzzle->setPixmap(temp);

                QFuture<QPixmap> future = QtConcurrent::run(this, &PuzzleBoardView::makePicturePuzzle,
                                                            temp, typePuzzle,QString("effect1"));


                puzzle->setPixmap( future.result() );

                m_scene.addItem(puzzle);
            }
        }
        emit puzzleCounts(countPuzzleWidth * countPuzzleHeight);
#endif
    }
}

QPixmap PuzzleBoardView::makePicturePuzzle(const QPixmap &pixmap, QString puzzleType, QString effectType) const
{
    QPixmap puzzle_mask(QLatin1String(":res/images/pieces/piece%1.png").arg(puzzleType));
    QImage puzzle_top(QLatin1String(":res/images/pieces/piece%1_%2.png").arg(puzzleType).arg(effectType));

    QPixmap pm = pixmap;
    pm.setMask(puzzle_mask.createMaskFromColor(Qt::black,Qt::MaskOutColor));

    QPainter p(&pm);
    p.drawImage(0,0,puzzle_top.copy(0,0,puzzleOrigWidth,puzzleOrigHeight),0,0);
    p.end();
    return pm;
}

void PuzzleBoardView::resizeEvent(QResizeEvent *event)
{
    static const int GRAPHICS_VIEW_MARGIN = 20;
    double m_scale = std::min(
                ( this->width() - GRAPHICS_VIEW_MARGIN ) / m_scene.width(),
                ( this->height() - GRAPHICS_VIEW_MARGIN ) / m_scene.height()
                );

    this->resetTransform();
    this->scale( m_scale, m_scale );
    Q_UNUSED(event);
}

bool PuzzleBoardView::isEven(int number)
{
    return !(number & 1);
}
