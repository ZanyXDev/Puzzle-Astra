
#include "mainwindow.h"
#include "puzzlepiece.h"
#include "puzzlemainwindow.h"

#define QA_TEST 1

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("ZanyXDev Inc.");
    QCoreApplication::setApplicationName("puzzle-astra");
    QCoreApplication::setApplicationVersion(QString("%1").arg(VERSION));

    QApplication app(argc, argv);

    // create folder AppConfigLocation
    QDir dirConfig(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
     qDebug() << "dirConfig.path()" <<dirConfig.path();
    if (dirConfig.exists()==false) {

        dirConfig.mkpath(dirConfig.path());
    }

    QTranslator myappTranslator;
    QString locale = QLocale::system().name();

    myappTranslator.load( QLatin1String("puzzle-astra_")+ locale, QLatin1String(":/i18n/"));
    app.installTranslator(&myappTranslator);

#ifdef QT_DEBUG_1
    // debug, print qrc resources
    QDirIterator it(":/", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        qDebug() << it.next();
    }
    qDebug() << QString("branch: %1, version: %2, built_at: %3").arg(GIT_BRANCH,GIT_HASH,BUILD_TIMESTAMP);
#endif

#ifdef QA_TEST
    PuzzleMainWindow qWinMain;
    qWinMain.setJustLaunchedWithImage(true);
#else
    MainWindow qWinMain;
    qWinMain.setStyleSheet(" background-image: url(:/res/images/table.png); ");

#endif
    const QRect availableGeometry = QApplication::desktop()->availableGeometry(&qWinMain);

    qWinMain.setMinimumSize(availableGeometry.width() / 2, (availableGeometry.height() * 2) / 3);
    qWinMain.move((availableGeometry.width() - qWinMain.width()) / 2,
                  (availableGeometry.height() - qWinMain.height()) / 2);
    qWinMain.show();
    qDebug() << "after show";
    return app.exec();
}
