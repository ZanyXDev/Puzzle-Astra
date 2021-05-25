#include "welcomedialog.h"

WelcomeDialog::WelcomeDialog(QWidget *parent)
    : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(windowFlags() & (~Qt::WindowContextHelpButtonHint | Qt::CustomizeWindowHint));
    // Application modal on mac, window modal everywhere else
#ifdef Q_OS_MACOS
    setWindowModality(Qt::ApplicationModal);
#else
    setWindowModality(Qt::WindowModal);
#endif
    // add fonts
    QFontDatabase::addApplicationFont(":/res/fonts/KellySlab-Regular.ttf");

    int modifier = 0;
    //set main title font
#ifdef Q_OS_MACOS
    const QFont font1 = QFont("Kelly Slab", 72, QFont::Light);
    modifier = 4;
#else
    QFont font1 = QFont("Kelly Slab", 54, QFont::Light);
#endif
    //set subtitle font & text
    QFont font2 = QFont("Kelly Slab", 14 + modifier);
    //set info font & text
    QFont font3 = QFont("Kelly Slab", 12 + modifier);

     const QString subtitleText = tr("Thank you for downloading Puzzle AstraLinux version.<br>Here's a few tips to get you started:");
     const QString updateText = tr("<ul><li>Right click to access the main menu</li><li>Ctrl+ leeft click rotate item</li><li>Scroll to zoom in and out</li><li>Use ESC keys to switch fullscreen</li></ul>");

     setWindowTitle(tr("Puzzle AstraLinux version"));
     resize(300,300);
     setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
     setStyleSheet(" background-color: #212121; ");

     auto *vLayout = new QVBoxLayout(this);
     auto *logoLabel =new QLabel(tr("Welcome"));
     logoLabel->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
     logoLabel->setFont(font1);
     setLayout(vLayout);
}
