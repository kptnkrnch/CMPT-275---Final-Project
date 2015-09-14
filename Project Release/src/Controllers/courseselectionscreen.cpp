#include "ui_courseselectionscreen.h"
#include "courseselectionscreen.h"
#include "mainwindow.h"

#include "../Models/dbaccess.h"
#include "../Models/user_model.h"
#include "../Models/course_model.h"
#include "../Models/activity_model.h"
#include "../Models/session.h"

#include "../Controllers/roleselectionscreen.h"
#include "../Controllers/instructorhomescreen.h"
#include "../Controllers/loginscreen.h"
#include "../Controllers/activityselectionscreen.h"
#include "../Controllers/gradeactivityselectionscreen.h"

#include "../Helpers/debug.h"


extern MainWindow *w;
extern Session *session;

CourseSelectionScreen::CourseSelectionScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CourseSelectionScreen)
{
    ui->setupUi(this);
    Course courseModel;
    //Activity activityModel;
    QString qstr = QString::fromStdString(session->getUserID());
    allCourses = courseModel.getCoursesByUser(qstr ,session->getUserRole());
    //QStringList courseIDs;


    QStringList courseList;

    int count=allCourses.count();
    for( int i=0; i<count; ++i ){
        courseList.append(allCourses[i]["courseNumber"] + " : " + allCourses[i]["courseName"]);
        //courseIDs.append(allCourses[i]["courseID"]);
    }

    ui->courseListWidget->addItems(courseList);
}

CourseSelectionScreen::~CourseSelectionScreen()
{
    delete ui;
}

void CourseSelectionScreen::on_selectCourseButton_clicked()
{
    if((ui->courseListWidget->count() == 0) || (ui->courseListWidget->currentItem() == NULL)){
        return;
    }

    int index = ui->courseListWidget->currentRow();
    session->setCourseID(allCourses[index]["courseID"]);
    session->setCourseTitle(allCourses[index]["courseNumber"] + " : " + allCourses[index]["courseName"]);
    if (session->getUserRole() == User::ROLE_TEACHINGASSISTANT){
        w->setCentralWidget(new GradeActivitySelectionScreen);//should be grading screen
        DEBUG(DEBUG_INFO, session->getCourseID());
    }
    else
        w->setCentralWidget(new InstructorHomeScreen);
    //w->setCentralWidget(new InstructorHomeScreen);
}

void CourseSelectionScreen::AddCourse(QString courseNumber, QString courseName) {
    ui->courseListWidget->addItem(courseNumber);
}

//void CourseSelectionScreen::on_backButton_clicked()
//{
//     w->setCentralWidget(new RoleSelectionScreen);
//}
