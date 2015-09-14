#ifndef CHANGEPASSWORDSCREEN_H
#define CHANGEPASSWORDSCREEN_H

#include <QWidget>

namespace Ui {
class ChangePasswordScreen;
}

class ChangePasswordScreen : public QWidget
{
    Q_OBJECT

public:
    explicit ChangePasswordScreen(QWidget *parent = 0);
    ~ChangePasswordScreen();

private slots:
    void on_continueButton_clicked();

    void on_passwordBox_returnPressed();

    void on_confirmBox_returnPressed();

    void on_pushButton_clicked();

private:
    Ui::ChangePasswordScreen *ui;
    void checkAndContinue();
};

#endif // CHANGEPASSWORDSCREEN_H
