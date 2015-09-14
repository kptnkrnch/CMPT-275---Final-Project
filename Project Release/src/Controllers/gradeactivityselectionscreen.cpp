#include "gradeactivityselectionscreen.h"
#include "ui_gradeactivityselectionscreen.h"

#include "Models/session.h"
#include "mainwindow.h"

#include "gradestudentselectionscreen.h"
#include "essaypsgradingscreen.h"
#include "Models/activity_model.h"
#include "Models/user_model.h"

#include "Controllers/instructorhomescreen.h"
#include "Controllers/courseselectionscreen.h"
#include "Controllers/multiplechoicegradingscreen.h"


extern MainWindow * w;
extern Session * session;

GradeActivitySelectionScreen::GradeActivitySelectionScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GradeActivitySelectionScreen)
{
    ui->setupUi(this);

    QString text = session->getCourseTitle();
    ui->label->setText(text);
    Activity activityModel;
    allActivities = activityModel.getByCourseIDIfCanGrade(session->getCourseID(), session->getUserRole());
    int count=allActivities.count();
    for( int i=0; i<count; ++i ){
        if (allActivities[i]["isProgrammingActivity"] == "1" )
            ui->activityListWidget->addItem( "Programming : " + allActivities[i]["activityName"] );
        else if (allActivities[i]["isEssayActivity"] == "1" )
            ui->activityListWidget->addItem( "Essay : " + allActivities[i]["activityName"] );
        else if (allActivities[i]["isProblemSetActivity"] == "1" )
            ui->activityListWidget->addItem( "Problem Set : " + allActivities[i]["activityName"] );
        else if (allActivities[i]["isMultipleChoiceActivity"] == "1" )
            ui->activityListWidget->addItem( "Multiple Choice :" + allActivities[i]["activityName"] );
    }
}

GradeActivitySelectionScreen::~GradeActivitySelectionScreen()
{
    delete ui;
}

void GradeActivitySelectionScreen::on_selectActivityButton_clicked()
{
    int index = ui->activityListWidget->currentRow();
    if ( index!= -1){
        int index = ui->activityListWidget->currentRow();


        session->setActivityID(allActivities[index]["activityID"]);
        if (allActivities[index]["isProgrammingActivity"] == "1")
        {
            session->setActivityType("Programming");
        }
        else if (allActivities[index]["isEssayActivity"] == "1")
        {
            session->setActivityType("Essay");
        }
        else if (allActivities[index]["isMultipleChoiceActivity"] == "1")
        {
            session->setActivityType("Multiple Choice");
        }
        else
        {
            session->setActivityType("Problem Set");
        }
        QString mc = "Multiple Choice";
        if (session->getActivityType() == mc) {
            w->setCentralWidget(new MultipleChoiceGradingScreen());
        } else {
            w->setCentralWidget(new GradeStudentSelectionScreen());
        }
    }
}

void GradeActivitySelectionScreen::on_cancelButton_clicked()
{
    if(session->getUserRole() == User::ROLE_TEACHINGASSISTANT){
        w->setCentralWidget(new CourseSelectionScreen);
    }
    else{
    w->setCentralWidget(new InstructorHomeScreen);
    }
}
