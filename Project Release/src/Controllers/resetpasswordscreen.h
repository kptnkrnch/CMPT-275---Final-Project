#ifndef RESETPASSWORDSCREEN_H
#define RESETPASSWORDSCREEN_H

#include <QWidget>
#include "roleselectionscreen.h"

#include "../Models/user_model.h"

namespace Ui {
class ResetPasswordScreen;
}

class ResetPasswordScreen : public QWidget
{
    Q_OBJECT

public:
    explicit ResetPasswordScreen(QMap<QString, QString> user, RoleSelectionScreen *screen, QWidget *parent = 0);
    ~ResetPasswordScreen();

private slots:
    void on_ContinueButton_clicked();

    void on_passwordBox_returnPressed();

    void on_confirmBox_returnPressed();

private:
    Ui::ResetPasswordScreen *ui;
    QMap<QString, QString> user;
    RoleSelectionScreen *roleScreen;
    void checkAndContinue();
};

#endif // RESETPASSWORDSCREEN_H
