#include <QApplication>
#include <QTranslator>
#include <QTime>
#include <QDir>
#include <QStandardPaths>
#include <QLibraryInfo>
#include <QDebug>
#include <QPushButton>
#include <QDirIterator>
//#include "mainwindow.h"


int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    // create folder AppConfigLocation
    QDir dirConfig(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    if (dirConfig.exists()==false) {
        dirConfig.mkpath(dirConfig.path());
    }

    // debug, print qrc resources
    QDirIterator it(":/", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        qDebug() << it.next();
    }

    QTranslator myappTranslator;
    QString locale = QLocale::system().name();

    bool ok=myappTranslator.load( QLatin1String("puzzle-astra_")+ locale, QLatin1String(":/i18n/"));
    app.installTranslator(&myappTranslator);

    qDebug() << "load trans:" << ok;

    qDebug() << QString("branch: %1, version: %2, built_at: %3").arg(GIT_BRANCH).arg(GIT_HASH).arg(BUILD_TIMESTAMP);

    QWidget widget;
    widget.setWindowTitle(QObject::tr("Hello World!"));
    widget.show();

    return app.exec();
}

