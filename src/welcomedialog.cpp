#include "welcomedialog.h"

WelcomeDialog::WelcomeDialog(QWidget *parent)
    : QDialog(parent)
{
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
    const QString infoText = tr("<ul><li>Right click to access the main menu</li><li>Ctrl+ leeft click rotate item</li><li>Scroll to zoom in and out</li><li>Use ESC keys to switch fullscreen</li></ul>");

    setWindowTitle(tr("Puzzle AstraLinux version %1").arg(VERSION));

    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    setStyleSheet(" background-color: #212121; ");

    auto *vLayout = new QVBoxLayout(this);
    auto *hLayout = new QHBoxLayout();
    auto *logoLabel =new QLabel(tr("Welcome"));
    logoLabel->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    logoLabel->setFont(font1);
    logoLabel->setStyleSheet("QLabel { color : white; }");
    logoLabel->setAlignment(Qt::AlignCenter | Qt::AlignTop);

    hLayout->addStretch();
    hLayout->addWidget(logoLabel,Qt::AlignCenter);
    hLayout->addStretch();

    auto *subtitleLabel = new QLabel( subtitleText );
    subtitleLabel->setFont(font2);
    subtitleLabel->setStyleSheet("QLabel { color : white; }");

    auto *infoLabel = new QLabel( infoText );
    infoLabel->setStyleSheet("QLabel { color : white; }");
    infoLabel->setFont(font3);

    QSettings settings;

    auto *updateCheckBox = new QCheckBox(tr("Disable notifications on next startup?"));
    updateCheckBox->setChecked(settings.value("options/hideNotification", false).toBool());

    updateCheckBox->setStyleSheet(
    " QCheckBox {color: white; font-family: Kelly Slab; spacing: 5px;}"
    " QCheckBox::indicator { width: 12px; height: 12px; border: 2px solid #FFFFFF;border-radius: 4px; background: none; }"
    " QCheckBox::indicator:unchecked:hover { background: rgba(255, 255, 255, 90); } "
    " QCheckBox::indicator:unchecked:pressed { background: rgba(255, 255, 255, 127); } "
    " QCheckBox::indicator:checked { image: url(:/res/images/checkmark.png); } "
    " QCheckBox::indicator:checked:hover { background: rgba(255, 255, 255, 90); } "
    " QCheckBox::indicator:checked:pressed { background: rgba(255, 255, 255, 127); }" );

    QFrame* hLineFrame = new QFrame();
    hLineFrame->setFrameShape(QFrame::HLine);

    vLayout->addLayout(hLayout);
    vLayout->addStretch();
    vLayout->addWidget(subtitleLabel,Qt::AlignCenter);
    vLayout->addStretch(2);
    vLayout->addWidget(infoLabel,Qt::AlignCenter);
    vLayout->addStretch();
    vLayout->addWidget(hLineFrame);
    vLayout->addWidget(updateCheckBox);
    setLayout(vLayout);
    setFixedSize(sizeHint());

    connect(updateCheckBox, &QCheckBox::stateChanged, qApp, [](int state){
        QSettings settings;
        settings.setValue("options/hideNotification", state > 0);
        settings.sync();

    });
    setCenterOnScreen();
}

void WelcomeDialog::setCenterOnScreen()
{
    // Get current screen size
    QRect rec = QGuiApplication::screenAt(this->pos())->geometry();

    // Using minimum size of window
    QSize size = this->minimumSize();

    // Set top left point
    QPoint topLeft = QPoint((rec.width() / 2) - (size.width() / 2), (rec.height() / 2) - (size.height() / 2));

    // set window position
    setGeometry(QRect(topLeft, size));
}
