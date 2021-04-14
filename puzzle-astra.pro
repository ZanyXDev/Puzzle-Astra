!versionAtLeast(QT_VERSION, 5.10.0):error("Requires Qt version 5.10.0 or greater.")

TEMPLATE +=app
QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 resources_big

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp

#HEADERS +=

RESOURCES += \
    resources.qrc

TRANSLATIONS += \
    i18n/puzzle-astra_ru_RU.ts
include(gitversion.pri)
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/mainwindow.h
