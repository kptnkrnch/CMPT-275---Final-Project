#include "createprogactivityscreen.h"
#include "ui_createprogactivityscreen.h"
#include "mainwindow.h"

#include "../Models/session.h"
#include "../Models/activity_model.h"
#include "../Models/activityfile_model.h"
#include "../Models/progtest_model.h"
#include "../Controllers/loginscreen.h"
#include "../Controllers/adminassistanthomescreen.h"
#include "../Controllers/rubriccreationscreen.h"
#include "../Controllers/activitytypeselectionscreen.h"
#include "../Controllers/instructorhomescreen.h"


#include "programmingtest.h"
#include "programmingtestdialog.h"

extern MainWindow *w;
extern Session *session;

createprogactivityscreen::createprogactivityscreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::createprogactivityscreen)
{
    ui->setupUi(this);
    itemModel = new QStandardItemModel();
    ui->tableView->setModel( itemModel );
    itemModel->setHorizontalHeaderLabels( (QStringList() << "Description" << "Weight"));
}

createprogactivityscreen::~createprogactivityscreen()
{
    delete ui;
}

void createprogactivityscreen::on_backButton_clicked()
{
    w->setCentralWidget(new activitytypeselectionscreen);
}


void createprogactivityscreen::on_addFileButton_clicked()
{
    bool ok;
    QString input = QInputDialog::getText(this, "Add File", "Name:", QLineEdit::Normal, "", &ok);
    if (input.count()>0)
    {
        QString input2 = QInputDialog::getText(this, "Add type", "Name:", QLineEdit::Normal, "", &ok);
        if (input2.count()>0)
        {
             ui->fileList->addItem(input);
             ui->typeList->addItem(input2);
        }

    }

}

void createprogactivityscreen::on_createProgActivityButton_clicked()
{
    QMessageBox box;

    if (ui->activityNameEdit->text() != "" && ui->pathToSolEdit->text() != "" &&
        ui->languageBox->currentText() != "" && (ui->fileList->count()==ui->typeList->count()))
    {
        Activity new_activity;
        new_activity.SetParameter("courseID", session->getCourseID());
        new_activity.SetParameter("activityName", ui->activityNameEdit->text());
        new_activity.SetParameter("pathToSolutionFile", ui->pathToSolEdit->text());
        new_activity.SetParameter("pathToWorkToMark", ui->pathToSubmissionsEdit->text());
        new_activity.SetParameter("pathToGradeFile", ui->gradeDirEdit->text());
        new_activity.SetParameter("language", ui->languageBox->currentText());
        new_activity.SetParameter("isProgrammingActivity", "1");
        new_activity.SetParameter("isEssayActivity", "0");
        new_activity.SetParameter("isProblemSetActivity", "0");
        new_activity.SetParameter("isMultipleChoiceActivity", "0");
        new_activity.SetParameter("dueDate", ui->dateEdit->date().toString("yyyy-MM-dd"));

        bool success = false;
        QString aID;
        QMap<QString, QString> new_activity_withID = new_activity.InsertActivity(true);
        if (new_activity_withID.count()>0){
            success = true;
            aID = new_activity_withID["activityID"];
            session->setActivityID(aID);
        }

        for (int i = 0; i < ui->fileList->count(); i++)
        {
            ActivityFile new_activityFile;
            new_activityFile.SetParameter("activityID", aID);
            new_activityFile.SetParameter("fileName", ui->fileList->item(i)->text());
            new_activityFile.SetParameter("fileType", ui->typeList->item(i)->text());
            new_activityFile.InsertActivityFile();
        }

        for (int i = 0; i < testList.length(); i++)
        {
            ProgTest p;
            p.insertProgTest(&testList[i], aID);
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
                box.setText("Programming Activity and Section(s) were created successfully!");
                box.exec();
            } else { //no rubric
                box.setText("Programming Activity was created successfully!");
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
        box.setText("Some required fields are empty / wrong number of file and type ");
        box.exec();
    }
}

void createprogactivityscreen::on_toolButton_clicked()
{
        QString dirName = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), "", QFileDialog::ShowDirsOnly);
        ui->pathToSolEdit->setText(dirName);
}

void createprogactivityscreen::on_toolButton_2_clicked()
{
        QString dirName = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), "", QFileDialog::ShowDirsOnly);
        ui->pathToSubmissionsEdit->setText(dirName);
}

void createprogactivityscreen::on_removeFileButton_clicked()
{
    int index = ui->fileList->currentRow();
    ui->fileList->takeItem(index);
    ui->typeList->takeItem(index);
}

void createprogactivityscreen::on_addTestButton_clicked()
{
    bool ok = false;
    ProgrammingTestDialog * dialog = new ProgrammingTestDialog(&ok, QString("Create Programming Test"));
    dialog->exec();

    if (ok)
    {
        ProgrammingTest * test = dialog->getProgrammingTest();
        ui->testList->addItem(test->name());
        testList.append(*test);
    }
    delete dialog;
}

void createprogactivityscreen::on_editTestButton_clicked()
{
    bool ok = false;
    int index = ui->testList->currentRow();
    if (index >= 0)
    {
        ProgrammingTest test = testList[index];

        ProgrammingTestDialog * dialog = new ProgrammingTestDialog(&ok, QString("Edit Programming Test"), &test);
        dialog->exec();

        if (ok)
        {
            ProgrammingTest newTest = *(dialog->getProgrammingTest());
            testList[index] = newTest;

            QListWidgetItem * item = ui->testList->currentItem();
            item->setText(newTest.name());
        }
        delete dialog;
    }
}

void createprogactivityscreen::on_removeTestButton_clicked()
{
    int index = ui->testList->currentRow();
    ui->testList->takeItem(index);
    testList.removeAt(index);
}

void createprogactivityscreen::on_fileList_currentRowChanged(int currentRow)
{
    if (currentRow > -1){
        ui->fileList->setCurrentRow(currentRow);
        ui->typeList->setCurrentRow(currentRow);
    }
}

void createprogactivityscreen::on_typeList_currentRowChanged(int currentRow)
{
    if (currentRow > -1){
        ui->fileList->setCurrentRow(currentRow);
        ui->typeList->setCurrentRow(currentRow);
    }
}

void createprogactivityscreen::on_toolButton_3_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), "", QFileDialog::ShowDirsOnly);
    ui->gradeDirEdit->setText(dirName);
}

void createprogactivityscreen::on_AddBtn_clicked()
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

void createprogactivityscreen::on_RemoveBtn_clicked()
{
    int index = ui->tableView->currentIndex().row();
    if (index > -1)
        itemModel->removeRow(index);
}

void createprogactivityscreen::on_pushButton_clicked()
{
    //reset screen
     w->setCentralWidget(new createprogactivityscreen);
}
