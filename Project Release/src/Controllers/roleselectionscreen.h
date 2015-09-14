#ifndef ROLESELECTIONSCREEN_H
#define ROLESELECTIONSCREEN_H

#include "ui_roleselectionscreen.h"
#include "mainwindow.h"

#include "../Controllers/sysadminhomescreen.h"
#include "../Controllers/adminassistanthomescreen.h"
#include "../Controllers/instructorhomescreen.h"
#include "../Controllers/courseselectionscreen.h"
#include "../Controllers/rubriccreationscreen.h"
#include "../Controllers/changepasswordscreen.h"
#include "../Models/course_model.h"
#include "../Models/user_model.h"
#include "../Models/session.h"
#include "../Helpers/debug.h"

#include <QWidget>
#include <QString>
#include <QList>
#include <QMap>
#include <QListIterator>

namespace Ui {
class RoleSelectionScreen;
}

class RoleSelectionScreen : public QWidget
{
    Q_OBJECT
public:
    RoleSelectionScreen(QWidget *parent = 0);
    ~RoleSelectionScreen();

private slots:
    void on_pushButton_clicked();

    void on_changePassword_clicked();

private:
    Ui::RoleSelectionScreen *ui;
};



#endif // ROLESELECTIONSCREEN_H
