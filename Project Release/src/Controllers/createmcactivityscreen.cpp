#include "createmcactivityscreen.h"
#include "ui_createmcactivityscreen.h"
#include "mainwindow.h"


#include "../Models/session.h"
#include "../Models/activity_model.h"
#include "../Models/multiplechoicetest_model.h"
#include "../Controllers/loginscreen.h"
#include "../Controllers/activitytypeselectionscreen.h"
#include "../Controllers/activitymanagementscreen.h"
#include "../Controllers/rubriccreationscreen.h"

extern MainWindow *w;
extern Session *session;

createmcactivityscreen::createmcactivityscreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::createmcactivityscreen)
{
    ui->setupUi(this);
    itemModel = new QStandardItemModel();
    ui->answerTableView->setModel( itemModel );
    itemModel->setHorizontalHeaderLabels( (QStringList() << "Answer" << "Weight"));
}

createmcactivityscreen::~createmcactivityscreen()
{
    delete ui;
}

void createmcactivityscreen::on_backButton_clicked()
{
    w->setCentralWidget(new activitytypeselectionscreen);
}


void createmcactivityscreen::on_toolButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "");
    ui->pathCSVEdit->setText(fileName);
}


void createmcactivityscreen::on_createButton_clicked()
{
    QMessageBox box;
    bool success=false;
    if(ui->activityNameEdit->text()!= ""&&
            ui->selectLanguageBox->currentText()!="" &&
            ui->pathCSVEdit->text()!="Select Language...")
         //   ui->MCDueDateEdit->currrentText()>currentDateTime())
    {
        Activity new_activity;
        new_activity.SetParameter("courseID", session->getCourseID());
        new_activity.SetParameter("activityName", ui->activityNameEdit->text());
        new_activity.SetParameter("pathToWorkToMark", ui->pathCSVEdit->text());//pathToWorkToMark or..?
        new_activity.SetParameter("pathToGradeFile", ui->pathToGradeFileEdit->text());
        new_activity.SetParameter("language", ui->selectLanguageBox->currentText());
        new_activity.SetParameter("dueDate", ui->MCDueDateEdit->date().toString("yyyy-MM-dd"));

        new_activity.SetParameter("isEssayActivity", "0");
        new_activity.SetParameter("isProblemSetActivity", "0");
        new_activity.SetParameter("isMultipleChoiceActivity", "1");
        new_activity.SetParameter("isProgrammingActivity", "0");

        //success=new_activity.InsertActivity();
        //activityID=new_activity.getID();
        QString aID ;
        QMap<QString, QString> new_activity_withID = new_activity.InsertActivity(true);
        if (new_activity_withID.count()>0){
            success = true;
            aID = new_activity_withID["activityID"];
            session->setActivityID(aID);
        }
        if(success){
            if (itemModel->rowCount() > 0){//have rubric
                MultipleChoiceTest testModel;
                QMap<QString, QString> test_data;
                test_data.insert("activityID", session->getActivityID());
                QMap<QString, QString> mc_test = testModel.insertMultipleChoiceTest(test_data);
                QString mcID = mc_test["multipleChoiceAnswerKeyID"];

                for (int i=0 ; i<itemModel->rowCount();++i){
                    QMap<QString, QString> testAnswer;
                    testAnswer.insert("multipleChoiceAnswerKeyID",mcID);
                    testAnswer.insert("value", itemModel->item(i,0)->text());
                    testAnswer.insert("weight", itemModel->item(i,1)->text());
                    testModel.insertMultipleChoiceTestAnswer(testAnswer);
                }
                box.setText("Test and Answer(s) were created successfully!");
                box.exec();
            } else {
                box.setText("create activity successfully.");
                box.exec();
            }
            w->setCentralWidget(new createmcactivityscreen);
        }else{
            box.setText("Failed to create activity.");
            box.exec();
        }
    }
    else{
        box.setText("Some field are required");
        box.exec();
    }
}

void createmcactivityscreen::on_addAnswerButton_clicked()
{
    QString text;
    QString weight = QString::number(ui->weightSpinBox->value());
    if(!ui->answerEdit->text().isEmpty()){
        text = ui->answerEdit->text();

        QList<QStandardItem *>  row = QList<QStandardItem *>() << new QStandardItem("c1") << new QStandardItem("c2");
        row[0] = new QStandardItem(text);
        row[1] = new QStandardItem(weight);
        row[1]->setTextAlignment(Qt::AlignCenter);
        itemModel->appendRow(row);

        ui->answerTableView->resizeRowsToContents();
        ui->answerTableView->resizeColumnsToContents();
        ui->answerTableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);

        ui->answerEdit->clear();
        ui->weightSpinBox->setValue(0);
    }
}

void createmcactivityscreen::on_removeAnswerButton_clicked()
{
    int index = ui->answerTableView->currentIndex().row();
    if (index > -1)
        itemModel->removeRow(index);
}

void createmcactivityscreen::on_toolButton_2_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), "", QFileDialog::ShowDirsOnly);
    ui->pathToGradeFileEdit->setText(dirName);
}
