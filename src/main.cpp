
#include <QtGui>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QApplication>
#include <QGraphicsSceneMouseEvent>

class CustomItem : public QGraphicsEllipseItem
{
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        if(event->button() == Qt::LeftButton) {
            if(event->modifiers() == Qt::ShiftModifier) {
                qDebug() << "Custom item left clicked with shift key.";
                // add the item to the selection
                setSelected(true);
            } else if(event->modifiers() == Qt::AltModifier){
                qDebug() << "Custom item left clicked with alt key.";
                // resize the item
                double radius = boundingRect().width() / 2.0;
                _center = QPointF(boundingRect().topLeft().x() + pos().x() + radius, boundingRect().topLeft().y() + pos().y() + radius);
                QPointF pos = event->scenePos();
                qDebug() << boundingRect() << radius << this->pos() << pos << event->pos();
                double dist = sqrt(pow(_center.x()-pos.x(), 2) + pow(_center.y()-pos.y(), 2));
                if(dist / radius > 0.8) {
                    qDebug() << dist << radius << dist / radius;
                    _isResizing = true;
                } else {
                    _isResizing = false;
                }
            } else {
                qDebug() << "Custom item left clicked.";
                QGraphicsItem::mousePressEvent(event);
                event->accept();
            }
        } else if(event->button() == Qt::RightButton) {
            qDebug() << "Custom item right clicked.";
            event->ignore();
        }
    }
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)
    {
        if(event->modifiers() == Qt::AltModifier && _isResizing){
            QPointF pos = event->scenePos();
            double dist = sqrt(pow(_center.x()-pos.x(), 2) + pow(_center.y()-pos.y(), 2));
            setRect(_center.x()-this->pos().x()-dist, _center.y()-this->pos().y()-dist, dist*2, dist*2);
        } else if(event->modifiers() != Qt::AltModifier) {
            qDebug() << "Custom item moved.";
            QGraphicsItem::mouseMoveEvent(event);
            qDebug()<<"moved"<<pos();
        }
    }
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {
        if(event->modifiers() == Qt::AltModifier && _isResizing) {
            _isResizing = false;
        } else if(event->modifiers() != Qt::ShiftModifier) {
            QGraphicsItem::mouseReleaseEvent(event);
        }
    }
    QVariant itemChange(GraphicsItemChange change, const QVariant &value)
    {
        if (change == ItemPositionChange && scene()) {
            qDebug() << "item->boundingRect()"<< this->boundingRect();
            // value is the new position.
            QPointF newPos = value.toPointF();
            qDebug() << "current pos:" << this->pos() << "newPos" << newPos;
            QRectF m_sceneRect = scene()->sceneRect();
            qDebug() << "change" << change <<" newPos" << newPos <<"sceneRect" <<m_sceneRect;
            if (!m_sceneRect.contains(newPos)) {
                // Keep the item inside the scene rect.
                newPos.setX(qMin(m_sceneRect.right(), qMax(newPos.x(), m_sceneRect.left())));
                newPos.setY(qMin(m_sceneRect.bottom(), qMax(newPos.y(), m_sceneRect.top())));
                return newPos;
            }
        }
        return QGraphicsItem::itemChange(change, value);
    }
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return UserType+1;
    }
private:
    QPointF _center;
    bool _isResizing;
};

class CustomScene : public QGraphicsScene
{
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        qDebug() << "Custom scene clicked.";
        QGraphicsScene::mousePressEvent(event);
        if(!event->isAccepted()) {
            if(event->button() == Qt::LeftButton) {
                // add a custom item to the scene
                QPointF pt = event->scenePos();
                CustomItem * item = new CustomItem();
                item->setRect(pt.x()-25, pt.y()-25, 50, 50);
                item->setFlags(QGraphicsItem::ItemIsSelectable|
                               QGraphicsItem::ItemIsMovable|
                               QGraphicsItem::ItemSendsGeometryChanges);

                addItem(item);
            } else if(event->button() == Qt::RightButton) {
                // check whether there is an item under the cursor
                QGraphicsItem * itemToRemove = NULL;
                foreach(auto item, items(event->scenePos())) {
                    if(item->type() == QGraphicsItem::UserType+1) {
                        itemToRemove = item;
                        break;
                    }
                }
                if(itemToRemove) {
                    // remove the item from the graphicsScene
                    removeItem(itemToRemove);
                }
            }
        }
    }
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)
    {
        qDebug() << "Custom scene moved.";
        QGraphicsScene::mouseMoveEvent(event);
    }
    void keyPressEvent(QKeyEvent * event) {
        if(event->key() == Qt::Key_Backspace) {
            // remove all selected items
            qDebug() << "selected items" << selectedItems().size();
            while(!selectedItems().isEmpty()) {
                removeItem(selectedItems().front());
            }
        } else {
            QGraphicsScene::keyPressEvent(event);
        }
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    qreal width=640;
    qreal height=480;

    QGraphicsRectItem *paper_rect =
            new QGraphicsRectItem (QRectF(0,0, width,height));
    paper_rect->setPen(QPen(Qt::black));
    paper_rect->setBrush(QBrush(Qt::white));
    paper_rect->setZValue(-1000.0);
    paper_rect->setData(0, "Paper");

    CustomItem item;
    item.setRect(20, 20, 60, 60);
    item.setFlags(QGraphicsItem::ItemIsSelectable|
                  QGraphicsItem::ItemIsMovable |
                  QGraphicsItem::ItemSendsGeometryChanges);
    item.setParentItem(paper_rect);

    CustomScene scene;
    scene.setSceneRect(0, 0, width, height);
    scene.setBackgroundBrush(Qt::transparent);
    scene.addItem(paper_rect);

    //scene.addItem(&item);


    QGraphicsView view;
    view.resize(width,height);
    // view.setSceneRect(-250, -250, 250, 250);
    view.setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    view.setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    view.setScene(&scene);
    view.show();
    return a.exec();
}

/**
#include <QApplication>
//#include <QGuiApplication>
//#include <QScreen>
#include <QTranslator>
#include <QStandardPaths>
#include <QLibraryInfo>
#include <QDesktopWidget>
#include <QDebug>
#include <QDirIterator>
#include "mainwindow.h"


int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    // create folder AppConfigLocation
    QDir dirConfig(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    if (dirConfig.exists()==false) {
        dirConfig.mkpath(dirConfig.path());
    }

#ifdef QT_DEBUG_1
    // debug, print qrc resources
    QDirIterator it(":/", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        qDebug() << it.next();
    }
    qDebug() << QString("branch: %1, version: %2, built_at: %3").arg(GIT_BRANCH,GIT_HASH,BUILD_TIMESTAMP);
#endif

    QTranslator myappTranslator;
    QString locale = QLocale::system().name();

    myappTranslator.load( QLatin1String("puzzle-astra_")+ locale, QLatin1String(":/i18n/"));
    app.installTranslator(&myappTranslator);

    MainWindow qWinMain;
#ifdef QT_DEBUG

#else
     qWinMain.setStyleSheet(" background-image: url(:/res/images/table.png); ");
#endif
    const QRect availableGeometry = QApplication::desktop()->availableGeometry(&qWinMain);

    qWinMain.setMinimumSize(availableGeometry.width() / 2, (availableGeometry.height() * 2) / 3);
    qWinMain.move((availableGeometry.width() - qWinMain.width()) / 2,
                  (availableGeometry.height() - qWinMain.height()) / 2);

    qWinMain.show();

    return app.exec();
}
*/
