#include "activityselectionscreen.h"
#include "ui_activityselectionscreen.h"
#include "mainwindow.h"

#include "../Models/dbaccess.h"
#include "../Models/user_model.h"
#include "../Models/course_model.h"
#include "../Models/activity_model.h"
#include "../Models/session.h"

#include "../Controllers/loginscreen.h"
#include "../Controllers/activitymanagementscreen.h"
#include "../Controllers/courseselectionscreen.h"
#include "../Controllers/rubriccreationscreen.h"

#include "../Helpers/debug.h"

extern MainWindow *w;
extern Session *session;

ActivitySelectionScreen::ActivitySelectionScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ActivitySelectionScreen)
{
    ui->setupUi(this);
    QString text = session->getCourseTitle();
    ui->label->setText(text);
    Activity activityModel;
    allActivities = activityModel.getByCourseID(session->getCourseID());
    //QStringList al;
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
        //al.append(allActivities[i]["activityName"]);
        //ui->activityListWidget->addItem( allActivities[i]["activityName"] + " : " + allActivities[i]["activityName"] );
    }

    //ui->activityListWidget->addItems(al);
}

ActivitySelectionScreen::~ActivitySelectionScreen()
{
    delete ui;
}

void ActivitySelectionScreen::on_selectActivityButton_clicked()
{
    int index = ui->activityListWidget->currentRow();
    if ( index!= -1){
        int index = ui->activityListWidget->currentRow();
        session->setActivityID(allActivities[index]["activityID"]);
        if (session->getUserRole() == 3) // role.h : ROLE_TEACHINGASSISTANT = 3
            w->setCentralWidget(new ActivitySelectionScreen);//should go to grading screen
        else
            w->setCentralWidget(new RubricCreationScreen);
    }
}

void ActivitySelectionScreen::on_cancelButton_clicked()
{
    if (session->getUserRole() == 3) // role.h : ROLE_TEACHINGASSISTANT = 3
        w->setCentralWidget(new CourseSelectionScreen);
    else
        w->setCentralWidget(new ActivityManagementScreen);
}
