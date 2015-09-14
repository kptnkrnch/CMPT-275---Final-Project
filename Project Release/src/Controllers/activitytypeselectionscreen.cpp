#include "activitytypeselectionscreen.h"
#include "ui_activitytypeselectionscreen.h"
#include "mainwindow.h"

#include "../Models/session.h"
#include "../Controllers/loginscreen.h"
#include "../Controllers/activitymanagementscreen.h"

#include "../Controllers/createessayactivityscreen.h"
#include "../Controllers/createmcactivityscreen.h"
#include "../Controllers/createprogactivityscreen.h"
#include "../Controllers/createpsactivityscreen.h"

extern MainWindow *w;
extern Session *session;

activitytypeselectionscreen::activitytypeselectionscreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::activitytypeselectionscreen)
{
    ui->setupUi(this);
}

activitytypeselectionscreen::~activitytypeselectionscreen()
{
    delete ui;
}

void activitytypeselectionscreen::on_backButton_clicked()
{
    w->setCentralWidget(new ActivityManagementScreen);
}

void activitytypeselectionscreen::on_SelectButton_clicked()
{
    QString type = ui->comboBox->currentText();
    if (type == "Essay")
    {
        w->setCentralWidget(new createessayactivityscreen);
    }
    else if (type == "Multiple Choice")
    {
        w->setCentralWidget(new createmcactivityscreen);
    }
    else if (type == "Programming")
    {
        w->setCentralWidget(new createprogactivityscreen);
    }
    else if (type == "Problem Set")
    {
        w->setCentralWidget(new createpsactivityscreen);
    }
}
