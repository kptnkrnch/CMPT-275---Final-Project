#include "activity_view_screen.h"
#include "ui_activity_view_screen.h"
#include "mainwindow.h"

#include "../Models/dbaccess.h"
#include "../Models/user_model.h"
#include "../Models/course_model.h"
#include "../Models/activity_model.h"
#include "../Models/activityfile_model.h"
#include "../Models/session.h"
#include "../Controllers/loginscreen.h"
#include "../Controllers/activitymanagementscreen.h"

#include "../Helpers/debug.h"

extern DBAccess DBA;
extern MainWindow *w;
extern Session *session;

Activity_View_Screen::Activity_View_Screen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Activity_View_Screen)
{
    ui->setupUi(this);
    Activity activityModel;
    allActivities = activityModel.getByCourseID(session->getCourseID());
    QStringList al;
    int count=allActivities.count();
    for( int i=0; i<count; ++i ){
        al.append(allActivities[i]["activityName"]);
    }

    ui->activityListWidget->addItems(al);
}

Activity_View_Screen::~Activity_View_Screen()
{
    delete ui;
}

void Activity_View_Screen::on_activityListWidget_currentRowChanged(int currentRow)
{
    QMap<QString, QString> currentActivity = allActivities[currentRow];


    ui->actNameEdit->setText(currentActivity["activityName"]);
    ui->pathSubEdit->setText(currentActivity["pathToWorkToMark"]);
    ui->pathSolEdit->setText(currentActivity["pathToSolutionFile"]);

    //activity type:

    if (currentActivity["isProgrammingActivity"] == "1" ) ui->typeBox->setText("Programming");
    else if (currentActivity["isEssayActivity"] == "1" ) ui->typeBox->setText("Essay");
    else if (currentActivity["isProblemSetActivity"] == "1" ) ui->typeBox->setText("Problem Set");
    else if (currentActivity["isMultipleChoiceActivity"] == "1" ) ui->typeBox->setText("Multiple Choice");

    //language:
    ui->languageBox->setText(currentActivity["language"]);

    //files:
    ActivityFile activityFileModel;
    allActivityFiles = activityFileModel.getByActivityID(currentActivity["activityID"]);
    QStringList al;
    int count=allActivityFiles.count();
    for( int i=0; i<count; ++i ){
        al.append(allActivityFiles[i]["fileName"]);
    }
    ui->fileList->clear();
    ui->fileList->addItems(al);
}

void Activity_View_Screen::on_backButton_clicked()
{
    w->setCentralWidget(new ActivityManagementScreen);
}
