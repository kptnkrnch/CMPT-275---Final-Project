#include "createessayactivityscreen.h"
#include "ui_createessayactivityscreen.h"
#include "mainwindow.h"

#include "../Models/session.h"
#include "../Models/activity_model.h"
#include "../Models/activityfile_model.h"
#include "../Controllers/loginscreen.h"
//#include "../Controllers/rubriccreationscreen.h"
#include "../Controllers/activitytypeselectionscreen.h"
#include "../Controllers/activitymanagementscreen.h"
#include "../Controllers/instructorhomescreen.h"


extern MainWindow *w;
extern Session *session;

createessayactivityscreen::createessayactivityscreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::createessayactivityscreen)
{
    ui->setupUi(this);
    itemModel = new QStandardItemModel();
    ui->tableView->setModel( itemModel );
    itemModel->setHorizontalHeaderLabels( (QStringList() << "Description" << "Weight"));
}

createessayactivityscreen::~createessayactivityscreen()
{
    delete ui;
}

void createessayactivityscreen::on_backButton_clicked()
{
    w->setCentralWidget(new activitytypeselectionscreen);
}


void createessayactivityscreen::on_createButton_clicked()
{
    QMessageBox box;

    if (ui->activityNameEdit->text() != "" &&
        ui->solutionPathEdit->text() != "" &&
        ui->selectLanguageBox->currentText() != "" &&
        ui->selectLanguageBox->currentText() != "Select Language...")
    {
        Activity new_activity;
        new_activity.SetParameter("courseID", session->getCourseID());
        new_activity.SetParameter("activityName", ui->activityNameEdit->text());
        new_activity.SetParameter("pathToWorkToMark", ui->submissionDirEdit->text());
        new_activity.SetParameter("pathToSolutionFile", ui->solutionPathEdit->text());
        new_activity.SetParameter("pathToGradeFile", ui->gradeDirEdit->text());
        new_activity.SetParameter("language", ui->selectLanguageBox->currentText());
        new_activity.SetParameter("dueDate", ui->EssayDueDateEdit->date().toString("yyyy-MM-dd"));

        new_activity.SetParameter("isEssayActivity", "1");
        new_activity.SetParameter("isProblemSetActivity", "0");
        new_activity.SetParameter("isMultipleChoiceActivity", "0");
        new_activity.SetParameter("isProgrammingActivity", "0");

        bool success = false;
        QString aID;
        QMap<QString, QString> new_activity_withID = new_activity.InsertActivity(true);
        if (new_activity_withID.count()>0){
            success = true;
            aID = new_activity_withID["activityID"];
            session->setActivityID(aID);
        }
        QString name = ui->fileEdit->text();
        if (name !=""){
            ActivityFile new_activityFile;
            new_activityFile.SetParameter("activityID", aID);
            new_activityFile.SetParameter("fileName", name);
            new_activityFile.SetParameter("fileType", "PDF");
            new_activityFile.InsertActivityFile();
        }
        if (success) {//activity created
            if (itemModel->rowCount() > 0){//have rubric
                Rubric rubric;
                rubric.SetParameter("activityID",session->getActivityID());
                QMap<QString, QString> one_rubric = rubric.InsertRubric(true);
                QString rid = one_rubric["rubricID"];

                RubricItem rubricItem;

                for (int i=0 ; i<itemModel->rowCount();++i){
                    rubricItem.SetParameter("rubricID",rid);
                    rubricItem.SetParameter("rubricItemText", itemModel->item(i,0)->text());
                    rubricItem.SetParameter("rubricItemValue", itemModel->item(i,1)->text());
                    rubricItem.InsertRubricItem();
                }
                box.setText("Esssay and Section(s) were created successfully!");
                box.exec();
            } else { //no rubric
                box.setText("Esssay was created successfully!");
                box.exec();
            }
            w->setCentralWidget(new InstructorHomeScreen);
        } else {
            box.setText("Failed to create activity.");
            box.exec();
        }
    }
    else
    {
        box.setText("Some required fields are empty");
        box.exec();
    }
}

void createessayactivityscreen::on_toolButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "");
    ui->solutionPathEdit->setText(fileName);
}

void createessayactivityscreen::on_toolButton_2_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), "", QFileDialog::ShowDirsOnly);
    ui->submissionDirEdit->setText(dirName);
}

void createessayactivityscreen::on_toolButton_3_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), "", QFileDialog::ShowDirsOnly);
    ui->gradeDirEdit->setText(dirName);
}

void createessayactivityscreen::on_AddBtn_clicked()
{
    QString text;
    QString weight = QString::number(ui->doubleSpinBox->value());
    if(!ui->textEdit->toPlainText().isEmpty()){
        text = ui->textEdit->toPlainText();

        QList<QStandardItem *>  row = QList<QStandardItem *>() << new QStandardItem("c1") << new QStandardItem("c2");
        row[0] = new QStandardItem(text);
        row[1] = new QStandardItem(weight);
        row[1]->setTextAlignment(Qt::AlignCenter);
        itemModel->appendRow(row);

        ui->tableView->resizeRowsToContents();
        ui->tableView->resizeColumnsToContents();
        ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);

        ui->textEdit->clear();
        ui->doubleSpinBox->setValue(0);
    }
}

void createessayactivityscreen::on_RemoveBtn_clicked()
{
    int index = ui->tableView->currentIndex().row();
    if (index > -1)
        itemModel->removeRow(index);
}

void createessayactivityscreen::on_pushButton_clicked()
{
    //reset screen
     w->setCentralWidget(new createessayactivityscreen);
}
