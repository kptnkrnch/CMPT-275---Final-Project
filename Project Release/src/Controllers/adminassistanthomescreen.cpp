#include "adminassistanthomescreen.h"
#include "ui_adminassistanthomescreen.h"
#include "mainwindow.h"

#include "../Models/session.h"
#include "../Controllers/coursecreationscreen.h"
#include "../Controllers/coursemodifyscreen.h"
#include "../Controllers/loginscreen.h"

extern MainWindow *w;
extern Session *session;

AdminAssistantHomeScreen::AdminAssistantHomeScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminAssistantHomeScreen)
{
    ui->setupUi(this);
}

AdminAssistantHomeScreen::~AdminAssistantHomeScreen()
{
    delete ui;
}

void AdminAssistantHomeScreen::on_createCourseButton_clicked()
{
    /*
    CourseCreationScreen * courseCreateScreen = new CourseCreationScreen;


    User userModel;
    User userModel2;

    QList<QMap<QString, QString> > tas = userModel.getAllTAs();
    QList<QMap<QString, QString> > instructors = userModel2.getAllInstructors();

    for (int i = 0; i < tas.length(); i++)
    {
        courseCreateScreen->addTA(tas[i]["userID"] + " : " + tas[i]["firstName"] + " " +tas[i]["lastName" );
    }
    for (int i = 0; i < instructors.length(); i++)
    {
        courseCreateScreen->addInstructor(instructors[i]["userID"] + " : " + instructors[i]["firstName"] + " " +instructors[i]["lastName" );
    }
*/
    //w->setCentralWidget(courseCreateScreen);
    w->setCentralWidget(new CourseCreationScreen);
}

void AdminAssistantHomeScreen::on_modifyCourseButton_clicked()
{
    w->setCentralWidget(new CourseModifyScreen);
}
