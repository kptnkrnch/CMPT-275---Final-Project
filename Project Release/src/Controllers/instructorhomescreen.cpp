#include "instructorhomescreen.h"
#include "ui_instructorhomescreen.h"
#include "mainwindow.h"

#include "../Models/session.h"
#include "../Controllers/loginscreen.h"
#include "../Controllers/courseselectionscreen.h"
#include "../Controllers/activitymanagementscreen.h"

#include "../Models/student_model.h"

#include "gradeactivityselectionscreen.h"

extern MainWindow *w;
extern Session *session;

InstructorHomeScreen::InstructorHomeScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InstructorHomeScreen)
{
    ui->setupUi(this);

    QString courseID = session->getCourseID();
    Student m;
    QStringList studentIDs = m.getEnrolledStudentIDs(courseID);
    if (studentIDs.length() == 0)
    {
        ui->gradeActivitiesButton->setDisabled(true);
    }
}

InstructorHomeScreen::~InstructorHomeScreen()
{
    delete ui;
}

void InstructorHomeScreen::on_manageActivitiesButton_clicked()
{
    w->setCentralWidget(new ActivityManagementScreen);
}

void InstructorHomeScreen::on_backButton_clicked()
{
    w->setCentralWidget(new CourseSelectionScreen);
}

void InstructorHomeScreen::on_gradeActivitiesButton_clicked()
{
//    GradeActivitySelectionScreen * screen = new GradeActivitySelectionScreen(this);
//    screen->setAutoFillBackground(true);
//    screen->show();
    w->setCentralWidget(new GradeActivitySelectionScreen);
}
