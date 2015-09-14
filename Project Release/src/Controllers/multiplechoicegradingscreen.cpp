#include "multiplechoicegradingscreen.h"
#include "ui_multiplechoicegradingscreen.h"

#include "../Models/multiplechoicetest_model.h"
#include "../Models/session.h"
#include "mainwindow.h"
#include "../Models/csv_processor.h"
#include "../Models/activity_model.h"
#include "gradeactivityselectionscreen.h"

#include <QList>
#include <QString>
#include <QMap>
#include <QMessageBox>
#include <sstream>
#include <string>

extern Session * session;
extern MainWindow * w;

MultipleChoiceGradingScreen::MultipleChoiceGradingScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MultipleChoiceGradingScreen)
{
    ui->setupUi(this);
}

MultipleChoiceGradingScreen::~MultipleChoiceGradingScreen()
{
    delete ui;
}

void MultipleChoiceGradingScreen::on_backButton_clicked()
{
    w->setCentralWidget(new GradeActivitySelectionScreen());
}

void MultipleChoiceGradingScreen::on_generateGradesButton_clicked()
{
    MultipleChoiceTest test;
    QList<QString> gradeData = test.generateGradeFile(session->getActivityID());
    CSVProcessor csv_processor;
    std::ostringstream oss;
    Activity activityModel;
    QMap<QString, QString> activity = activityModel.GetActivityByID(session->getActivityID());
    oss << activity["pathToGradeFile"].toStdString() << "/" << ui->gradeFileEdit->text().toStdString().c_str() << ".csv";
    if (csv_processor.generateCSV(oss.str().c_str(), gradeData)) {
        QMessageBox msgBox;
        std::string message = "Created student grade file at:\n" + oss.str();
        msgBox.setText(message.c_str());
        msgBox.exec();
    } else {
        QMessageBox msgBox;
        std::string message = "Failed to create student grade file at:\n" + oss.str();
        msgBox.setText(message.c_str());
        msgBox.exec();
    }
}
