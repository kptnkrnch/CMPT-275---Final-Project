#include "gradestudentselectionscreen.h"
#include "ui_gradestudentselectionscreen.h"

#include "Models/student_model.h"
#include "Controllers/gradeactivityselectionscreen.h"
#include "Controllers/essaypsgradingscreen.h"
#include "programminggradingscreen.h"

#include "Models/rubricitemgrade_model.h"

#include "Models/csv_processor.h"

#include "Controllers/gradefilenamedialog.h"

#include "Models/session.h"
#include "mainwindow.h"

#include <string>

extern MainWindow * w;
extern Session * session;

GradeStudentSelectionScreen::GradeStudentSelectionScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GradeStudentSelectionScreen)
{
    ui->setupUi(this);

    Student m;

    if (session->getUserRole() == User::ROLE_INSTRUCTOR) {
        ui->generateGradesButton->setEnabled(true);
    } else {
        ui->generateGradesButton->hide();
    }

    students = m.getEnrolledStudents(session->getCourseID());

    for (int i = 0; i < students.length(); i++)
    {
        QString studentID = students[i]["studentID"];
        QString firstName = students[i]["firstName"];
        QString middleName = students[i]["middleName"];
        QString lastName = students[i]["lastName"];

        ui->studentList->addItem(studentID + ": " + firstName + " " + middleName + " " + lastName);
    }
}

GradeStudentSelectionScreen::~GradeStudentSelectionScreen()
{
    delete ui;
}

void GradeStudentSelectionScreen::on_backButton_clicked()
{
    w->setCentralWidget(new GradeActivitySelectionScreen);
}

void GradeStudentSelectionScreen::on_continueButton_clicked()
{
    int index = ui->studentList->currentRow();

    if (index >= 0)
    {
        QString studentID = students[index]["studentID"];
        session->setStudentID(studentID);
        RubricItemGrade itemGrade;
        bool canremark = itemGrade.CheckIfCanBeRemarked(session->getActivityID(), session->getStudentID(), session->getUserRole());
        bool marked = itemGrade.CheckIfHasBeenMarked(session->getActivityID(), session->getStudentID());
        if ((canremark && session->getUserRole() != User::ROLE_ADMINISTRATOR) || (marked && canremark && session->getUserRole() == User::ROLE_ADMINISTRATOR)) {
            if (session->getActivityType() == "Essay" || session->getActivityType() == "Problem Set")
            {
                w->setCentralWidget(new EssayPSGradingScreen);
            }
            else if (session->getActivityType() == "Programming")
            {
                w->setCentralWidget(new ProgrammingGradingScreen);
            }
            else
            {
                //w->setCentralWidget(new MCGradingScreen);
            }
        } else {
            if ((!marked && canremark && session->getUserRole() == User::ROLE_ADMINISTRATOR)) {
                QMessageBox msgBox;
                msgBox.setText("Sorry, there are no marks submitted that can be regraded.");
                msgBox.exec();
            } else {
                QMessageBox msgBox;
                msgBox.setText("Sorry, these grades have been marked by a higher privilege user.\nYou can no longer regrade them.");
                msgBox.exec();
            }
        }
    }
}

void GradeStudentSelectionScreen::on_generateGradesButton_clicked()
{
    GradeFileNameDialog dialog;
    dialog.setGradeStudentSelectionScreen(this);
    dialog.exec();
    /*RubricItemGrade rig;
    QList<QString> csv = rig.GenerateGradeFile(session->getActivityID());
    CSVProcessor csv_processor;
    Activity activityModel;
    QMap<QString, QString> tempActivity = activityModel.GetActivityByID(session->getActivityID());
    std::string grade_file = tempActivity["pathToGradeFile"].toStdString() + "/grades.csv";
    if (csv_processor.generateCSV(grade_file.c_str(), csv)) {
        QMessageBox msgBox;
        std::string text = "Grade file created at:\n" + grade_file;
        msgBox.setText(text.c_str());
        msgBox.exec();
    } else {
        QMessageBox msgBox;
        msgBox.setText("Failed to create grade file.");
        msgBox.exec();
    }*/
}

void GradeStudentSelectionScreen::generateGrades() {
    RubricItemGrade rig;
    QList<QString> csv = rig.GenerateGradeFile(session->getActivityID());
    CSVProcessor csv_processor;
    Activity activityModel;
    QMap<QString, QString> tempActivity = activityModel.GetActivityByID(session->getActivityID());

    std::string grade_file = tempActivity["pathToGradeFile"].toStdString() + "/" + filename.toStdString();
    if (filename.length() > 0 && csv_processor.generateCSV(grade_file.c_str(), csv)) {
        QMessageBox msgBox;
        std::string text = "Grade file created at:\n" + grade_file;
        msgBox.setText(text.c_str());
        msgBox.exec();
    } else {
        QMessageBox msgBox;
        msgBox.setText("Failed to create grade file.");
        msgBox.exec();
    }
}

void GradeStudentSelectionScreen::setFileName(QString _filename) {
    filename = _filename;
}
