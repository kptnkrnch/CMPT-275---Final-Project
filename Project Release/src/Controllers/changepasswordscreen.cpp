#include "changepasswordscreen.h"
#include "ui_changepasswordscreen.h"

#include "../Models/session.h"
#include "../Models/user_model.h"

#include <QMessageBox>

extern Session * session;

ChangePasswordScreen::ChangePasswordScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangePasswordScreen)
{
    ui->setupUi(this);
}

ChangePasswordScreen::~ChangePasswordScreen()
{
    delete ui;
}

void ChangePasswordScreen::checkAndContinue()
{
    QString pw = ui->passwordBox->text();
    QString pw2 = ui->confirmBox->text();

    QMessageBox msgbox;
    QString message;

    User m;

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
        QMap<QString, QString> user = m.getUser(QString::fromStdString(session->getUserID()));
        user["password"] = pw;
        user["resetPassword"] = "0";
        m.updateUser(user);

        message = "Your password has been changed successfully";
        msgbox.setText(message);
        msgbox.exec();

        this->close();
    }
    else
    {
        message = "Passwords do not match";
        msgbox.setText(message);
        msgbox.exec();
    }
}

void ChangePasswordScreen::on_continueButton_clicked()
{
    checkAndContinue();
}

void ChangePasswordScreen::on_passwordBox_returnPressed()
{
    checkAndContinue();
}

void ChangePasswordScreen::on_confirmBox_returnPressed()
{
    checkAndContinue();
}

void ChangePasswordScreen::on_pushButton_clicked()
{
    this->close();
}
