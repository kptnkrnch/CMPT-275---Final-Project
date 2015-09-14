#ifndef ACCOUNTMODIFYINGSCREEN_H
#define ACCOUNTMODIFYINGSCREEN_H

#include <QWidget>
#include <QListWidgetItem>
#include "../Models/user_model.h"

namespace Ui {
class AccountModifyingScreen;
}

class AccountModifyingScreen : public QWidget
{
    Q_OBJECT

public:
    explicit AccountModifyingScreen(QWidget *parent = 0);
    ~AccountModifyingScreen();

private slots:
    void on_buSave_clicked();

    void on_buDelete_clicked();

    void on_liUsers_currentRowChanged(int currentRow);

    void on_buCancel_clicked();

    void on_leFName_textChanged(const QString &arg1);

    void on_leLName_textChanged(const QString &arg1);

    void on_buRevert_clicked();

private:

    void refresh();
    void setKnownFields(QMap<QString, QString> *user);
    bool msgBoxYesNo(QString text, QString informativeText);

    Ui::AccountModifyingScreen *ui;

    User userModel;
    QStringList allUserIDs;
    QList<QMap<QString, QString> > allUsers;
    QMap<QString, QString> *currentUser;
};

#endif // ACCOUNTMODIFYINGSCREEN_H
