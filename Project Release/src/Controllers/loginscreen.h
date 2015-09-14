#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <QWidget>
#include <QMessageBox>
#include <QMenuBar>
#include <QMap>
#include <QString>

namespace Ui {
class LoginScreen;
}

class LoginScreen : public QWidget
{
    Q_OBJECT

public:
    explicit LoginScreen(QWidget *parent = 0);
    void executeLogin();
    ~LoginScreen();

private slots:
    void on_usernameEdit_returnPressed();

    void on_passwordEdit_returnPressed();

    void on_loginButton_clicked();

private:
    Ui::LoginScreen *ui;
};

#endif // LOGINSCREEN_H
