!versionAtLeast(QT_VERSION, 5.10.0):error("Requires Qt version 5.10.0 or greater.")

TEMPLATE +=app
QT       += core gui concurrent

VERSION = 0.3
# allows use of version variable elsewhere
DEFINES += "VERSION=$$VERSION"

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LANGUAGE  = C++
CONFIG += c++17 resources_big cmdline precompile_header

include(gitversion.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = src/stable.h


SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/puzzleboardview.cpp \
    src/puzzlepiece.cpp

HEADERS += \
    src/stable.h \
    src/mainwindow.h \
    src/puzzleboardview.h \
    src/puzzlepiece.h

RESOURCES += \
    resources.qrc

TRANSLATIONS += \
    i18n/puzzle-astra_ru_RU.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


