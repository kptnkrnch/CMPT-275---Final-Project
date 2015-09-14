#include "resetpasswordscreen.h"
#include "ui_resetpasswordscreen.h"

#include "mainwindow.h"
#include <QMessageBox>

#include "../Models/user_model.h"


extern MainWindow * w;

ResetPasswordScreen::ResetPasswordScreen(QMap<QString, QString> user, RoleSelectionScreen *screen, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResetPasswordScreen)
{
    this->user = user;
    roleScreen = screen;
    ui->setupUi(this);
}

ResetPasswordScreen::~ResetPasswordScreen()
{
    delete ui;
}

void ResetPasswordScreen::checkAndContinue()
{
    QString pw = ui->passwordBox->text();
    QString pw2 = ui->confirmBox->text();

    QMessageBox msgbox;
    QString message;


    if (pw.length() < 4)
    {
        message = "Password is too short";
        msgbox.setText(message);
        msgbox.exec();
    }
    else if (pw.length() > 40)
    {
        message = "Password is too long";
        msgbox.setText(message);
        msgbox.exec();
    }
    else if (pw == pw2)
    {
        User::setPassword(pw, &user);
        User::setPwdResetNeeded(false, &user);
        User m;
        if(!m.updateUser(user)) {
            message = "Password could not be changed at this time.";
            msgbox.setText(message);
            msgbox.exec();
        }

        message = "Your password has been changed successfully";
        msgbox.setText(message);
        msgbox.exec();

        w->setCentralWidget(roleScreen);
    }
    else
    {
        message = "Passwords do not match";
        msgbox.setText(message);
        msgbox.exec();
    }
}

void ResetPasswordScreen::on_passwordBox_returnPressed()
{
    checkAndContinue();
}

void ResetPasswordScreen::on_ContinueButton_clicked()
{
    checkAndContinue();
}

void ResetPasswordScreen::on_confirmBox_returnPressed()
{
    checkAndContinue();
}
