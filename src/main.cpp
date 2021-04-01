#include <QApplication>
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

#ifdef QT_DEBUG
    // debug, print qrc resources
    QDirIterator it(":/", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        qDebug() << it.next();
        qDebug() << QString("branch: %1, version: %2, built_at: %3").arg(GIT_BRANCH).arg(GIT_HASH).arg(BUILD_TIMESTAMP);
    }
#endif

    QTranslator myappTranslator;
    QString locale = QLocale::system().name();

    myappTranslator.load( QLatin1String("puzzle-astra_")+ locale, QLatin1String(":/i18n/"));
    app.installTranslator(&myappTranslator);

    MainWindow qWinMain;
    qWinMain.setStyleSheet(" background-image: url(:/res/images/table.png); ");
    const QRect availableGeometry = QApplication::desktop()->availableGeometry(&qWinMain);

    qWinMain.setMinimumSize(availableGeometry.width() / 2, (availableGeometry.height() * 2) / 3);
    qWinMain.move((availableGeometry.width() - qWinMain.width()) / 2,
                  (availableGeometry.height() - qWinMain.height()) / 2);

    qWinMain.show();

    return app.exec();
}

