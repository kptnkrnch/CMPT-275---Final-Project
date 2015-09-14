#ifndef ACCOUNTCREATIONSCREEN_H
#define ACCOUNTCREATIONSCREEN_H

#include <QDialog>

#include "../Models/session.h"
#include "../Models/user_model.h"
#include "../Controllers/sysadminhomescreen.h"
#include "../Controllers/roleselectionscreen.h"


namespace Ui {
class AccountCreationScreen;
}

class AccountCreationScreen : public QDialog
{
    Q_OBJECT

public:
    explicit AccountCreationScreen(QWidget *parent = 0);
    ~AccountCreationScreen();

private slots:
    void on_buSave_clicked();
    void on_buCancel_clicked();
    void on_leFName_textChanged(const QString &arg1);
    void on_leLName_textChanged(const QString &arg1);

private:
    User userModel;
    Ui::AccountCreationScreen *ui;
    bool isUserAllowed();
    QList<int> allowedRoles;

};

#endif // ACCOUNTCREATIONSCREEN_H
