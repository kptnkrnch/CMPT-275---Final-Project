#include "norubricactivityselectionscreen.h"
#include "ui_norubricactivityselectionscreen.h"
#include "mainwindow.h"

#include "Models/dbaccess.h"
#include "Models/user_model.h"
#include "Models/course_model.h"
#include "Models/activity_model.h"
#include "Models/session.h"
#include "Models/model.h"

#include "Controllers/loginscreen.h"
#include "Controllers/activitymanagementscreen.h"
//#include "Controllers/courseselectionscreen.h"
#include "Controllers/rubriccreationscreen.h"

#include "Helpers/debug.h"

extern MainWindow *w;
extern Session *session;
//extern DBAccess *dba;


NoRubricActivitySelectionScreen::NoRubricActivitySelectionScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NoRubricActivitySelectionScreen)
{
    ui->setupUi(this);

    //QString currentCourseID = session->getCourseID();

    //DBAccess dba;
    Activity activityModel;

    rubriclessActivities = activityModel.getRubriclessByCourseID(session->getCourseID());


    //my ugly sql query to get activities without rubrics
    //QString sqlString = "SELECT DISTINCT activityID FROM Activity A, Rubric R WHERE R.courseID ='";
    //sqlString.append(currentCourseID);
    //sqlString.append("' AND A.activityID NOT IN(SELECT DISTINCT activityID FROM Rubric WHERE courseID = '");
    //sqlString.append(currentCourseID);
    //sqlString.append("')");

    //DEBUG(DEBUG_INFO, sqlString);

    //dba.__query(sqlString);
    //QSqlQuery rubriclessResults(sqlString);

    //DEBUG(DEBUG_INFO, rubriclessResults.value(0).toString());

    //add to the widget list the results
    int count = rubriclessActivities.count();
    for(int k=0; k<count; k++){
        ui->RubriclessActivityListWidget->addItem(rubriclessActivities[k]["activityName"]);
    }

}

NoRubricActivitySelectionScreen::~NoRubricActivitySelectionScreen()
{
    delete ui;
}

void NoRubricActivitySelectionScreen::on_selectButton_clicked()
{
    if(ui->RubriclessActivityListWidget->currentItem() != NULL){
        int index = ui->RubriclessActivityListWidget->currentRow();
        session->setActivityID(rubriclessActivities[index]["activityID"]);
        DEBUG(DEBUG_INFO, session->getCourseID());
        DEBUG(DEBUG_INFO, session->getActivityID());
        w->setCentralWidget(new RubricCreationScreen);

    }
}

void NoRubricActivitySelectionScreen::on_cancelButton_clicked()
{
    w->setCentralWidget(new ActivityManagementScreen);
}
