#include "activitymanagementscreen.h"
#include "ui_activitymanagementscreen.h"
#include "activity_view_screen.h"

#include "../Models/session.h"
#include "../Controllers/loginscreen.h"
#include "../Controllers/instructorhomescreen.h"
#include "../Controllers/activitytypeselectionscreen.h"
#include "activitymodifyscreen.h"

#include "norubricactivityselectionscreen.h"
#include "activityselectionscreen.h"
#include "mainwindow.h"

extern MainWindow *w;
extern Session *session;

ActivityManagementScreen::ActivityManagementScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::activitymanagementscreen)
{
    ui->setupUi(this);
    ui->modifyRubricButton->hide();
}

ActivityManagementScreen::~ActivityManagementScreen()
{
    delete ui;
}

void ActivityManagementScreen::on_backButton_clicked()
{
    w->setCentralWidget(new InstructorHomeScreen);
}

void ActivityManagementScreen::on_createActivityButton_clicked()
{
    w->setCentralWidget(new activitytypeselectionscreen);
}

void ActivityManagementScreen::on_createRubricButton_clicked()
{
    w->setCentralWidget(new NoRubricActivitySelectionScreen);
}

void ActivityManagementScreen::on_modifyActivityButton_clicked()
{
    w->setCentralWidget(new activitymodifyscreen);
}

void ActivityManagementScreen::on_modifyRubricButton_clicked()
{

}
