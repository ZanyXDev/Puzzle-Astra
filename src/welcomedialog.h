#pragma once

#include <QDialog>
#include <QCheckBox>

class WelcomeDialog : public QDialog
{
       Q_OBJECT
public:
    explicit WelcomeDialog(QWidget *parent);
private:
    void setCenterOnScreen();
};

