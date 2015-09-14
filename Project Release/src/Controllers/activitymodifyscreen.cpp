#include "activitymodifyscreen.h"
#include "ui_activitymodifyscreen.h"
#include "mainwindow.h"
#include <QFileDialog>
#include <QInputDialog>
#include "../Models/dbaccess.h"
#include "../Models/activity_model.h"
#include "../Models/user_model.h"
#include "../Models/course_model.h"
#include "../Models/progtest_model.h"

#include "../Models/activityfile_model.h"
#include "../Models/session.h"
#include "../Controllers/loginscreen.h"
#include "../Controllers/activitymanagementscreen.h"
#include "../Controllers/rubriccreationscreen.h"
#include "../Helpers/debug.h"

#include "programmingtestdialog.h"

extern DBAccess DBA;
extern MainWindow *w;
extern Session *session;

activitymodifyscreen::activitymodifyscreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::activitymodifyscreen)
{
    ui->setupUi(this);
    //Get activities by course to populate list
    Activity activityModel;
    allActivities = activityModel.getByCourseID(session->getCourseID());
    QStringList al;
    int count = allActivities.count();
    for( int i=0; i<count; ++i ){
        al.append(allActivities[i]["activityName"]);
    }
    ui->deleteBtn_2->move(900,70);
    ui->updateBtn_2->move(900,510);
    ui->rubricGroupBox->move(280,60);
    ui->activitiesListWidget->addItems(al);
    ui->updateBtn->hide();
    ui->updateBtn_2->hide();
    ui->activityBtn->hide();
    ui->deleteBtn->hide();
    ui->rubricBtn->hide();
    ui->rubricGroupBox->hide();
    ui->deleteBtn->hide();
    ui->deleteBtn_2->hide();
    toggleEditable(false);
}

activitymodifyscreen::~activitymodifyscreen()
{
    delete ui;
}
//for choosing an activity show its related info
void activitymodifyscreen::on_activitiesListWidget_currentRowChanged(int currentRow)
{
    QMap<QString, QString> currentActivity = allActivities[currentRow];
    session->setActivityID(currentActivity["activityID"]);
    //session->setActivityType(currentActivity["activityType"]);
    if (currentActivity["isProgrammingActivity"] == "1")
    {
        session->setActivityType("Programming");
    }
    else if (currentActivity["isEssayActivity"] == "1")
    {
        session->setActivityType("Essay");
    }
    else if (currentActivity["isMultipleChoiceActivity"] == "1")
    {
        session->setActivityType("Multiple Choice");
    }
    else
    {
        session->setActivityType("Problem Set");
    }
    ui->label_15->setText("View Activity");
    ui->activityNameEdit->setText(currentActivity["activityName"]);
    ui->pathSubsEdit->setText(currentActivity["pathToWorkToMark"]);
    ui->pathCSVEdit->setText(currentActivity["pathToWorkToMark"]);
    ui->pathSolutionEdit->setText(currentActivity["pathToSolutionFile"]);
    ui->gradefileEdit->setText(currentActivity["pathToGradeFile"]);

    ui->dateEdit->setDate( QDate::fromString(currentActivity["dueDate"], "yyyy-MM-dd") );

    //"English","French","Chinese","German","Java","C++","Python"
    ui->languageBox->clear();
    ui->languageBox->addItems(QStringList ( QList<QString>() << "English"<<"French"<<"German"<<"Chinese"<<"C++"<<"Java"<<"Python"));
    ui->rubricGroupBox->hide();
    ui->gridGroupBox->show();
    ui->testGroupBox->show();

    ui->activityBtn->show();
    ui->deleteBtn->hide();
    ui->deleteBtn_2->hide();
    ui->updateBtn_2->hide();
    //Show type
    if (currentActivity["isProgrammingActivity"] == "1" )
    {
        ui->testList->clear();
        ui->rubricBtn->show();
        ui->gradefile->show();
        ui->gradefileEdit->show();
        ui->toolButtonGrade->show();
        ui->pathSubs->show();
        ui->pathSubsEdit->show();
        ui->toolButtonSUB->show();
        ui->pathSolution->show();
        ui->pathSolutionEdit->show();
        ui->toolButtonSOL->show();
        ui->pathCSVEdit->hide();
        ui->pathCSV->hide();
        ui->toolButtonCSV->hide();

        ui->typeEdit->setText("Programming");
        ui->subFile->hide();
        ui->subFileEdit->hide();
        ui->subFilesList->show();
        ui->addFileList->show();
        ui->removeFileButton->show();
        ui->files->show();
        ui->addFileButton->show();
        ui->languageBox->removeItem(0);
        ui->languageBox->removeItem(0);
        ui->languageBox->removeItem(0);
        ui->languageBox->removeItem(0);
        int langIndex = ui->languageBox->findText(currentActivity["language"]);
        if (langIndex != -1){
            ui->languageBox->setCurrentIndex(langIndex);
        }
        else
            ui->languageBox->setCurrentIndex(0);

        ui->testGroupBox->show();

        ProgTest p;

        testList = p.getProgTests(currentActivity["activityID"]);
        for (int i = 0; i < testList.length(); i++)
        {
            ui->testList->addItem(testList[i].name());
        }
    }
        else if (currentActivity["isEssayActivity"] == "1" )
    {
        ui->rubricBtn->show();
        ui->gradefile->show();
        ui->gradefileEdit->show();
        ui->toolButtonGrade->show();
        ui->pathSubs->show();
        ui->pathSubsEdit->show();
        ui->toolButtonSUB->show();
        ui->pathSolution->show();
        ui->pathSolutionEdit->show();
        ui->toolButtonSOL->show();
        ui->pathCSVEdit->hide();
        ui->pathCSV->hide();
        ui->toolButtonCSV->hide();

        ui->typeEdit->setText("Essay");
        ui->subFile->show();
        ui->subFileEdit->show();
        ui->subFilesList->hide();
        ui->addFileList->hide();
        ui->removeFileButton->hide();
        ui->files->hide();
        ui->addFileButton->hide();

        ui->languageBox->removeItem(4);
        ui->languageBox->removeItem(4);
        ui->languageBox->removeItem(4);
        int langIndex = ui->languageBox->findText(currentActivity["language"]);
        if (langIndex != -1){
            ui->languageBox->setCurrentIndex(langIndex);
        }
        else
            ui->languageBox->setCurrentIndex(0);

        ui->testGroupBox->hide();

    }
        else if (currentActivity["isProblemSetActivity"] == "1" )
    {
        ui->rubricBtn->show();
        ui->gradefile->show();
        ui->gradefileEdit->show();
        ui->toolButtonGrade->show();
        ui->pathSubs->show();
        ui->pathSubsEdit->show();
        ui->toolButtonSUB->show();
        ui->pathSolution->show();
        ui->pathSolutionEdit->show();
        ui->toolButtonSOL->show();
        ui->pathCSVEdit->hide();
        ui->pathCSV->hide();
        ui->toolButtonCSV->hide();

        ui->typeEdit->setText("Problem Set");
        ui->subFile->show();
        ui->subFileEdit->show();
        ui->subFilesList->hide();
        ui->addFileList->hide();
        ui->removeFileButton->hide();
        ui->files->hide();
        ui->addFileButton->hide();

        ui->languageBox->removeItem(4);
        ui->languageBox->removeItem(4);
        ui->languageBox->removeItem(4);
        int langIndex = ui->languageBox->findText(currentActivity["language"]);
        if (langIndex != -1){
            ui->languageBox->setCurrentIndex(langIndex);
        }
        else
            ui->languageBox->setCurrentIndex(0);

        ui->testGroupBox->hide();
    }
        else if (currentActivity["isMultipleChoiceActivity"] == "1" )
    {
        ui->rubricBtn->hide();
        ui->gradefile->hide();
        ui->gradefileEdit->hide();
        ui->toolButtonGrade->hide();
        ui->pathSubs->hide();
        ui->pathSubsEdit->hide();
        ui->toolButtonSUB->hide();
        ui->pathSolution->hide();
        ui->pathSolutionEdit->hide();
        ui->toolButtonSOL->hide();
        ui->pathCSVEdit->show();
        ui->pathCSV->show();
        ui->toolButtonCSV->show();

        ui->typeEdit->setText("Multiple Choice");
        ui->subFile->hide();
        ui->subFileEdit->hide();
        ui->subFilesList->hide();
        ui->addFileList->hide();
        ui->removeFileButton->hide();
        ui->files->hide();
        ui->addFileButton->hide();

        ui->languageBox->removeItem(4);
        ui->languageBox->removeItem(4);
        ui->languageBox->removeItem(4);
        int langIndex = ui->languageBox->findText(currentActivity["language"]);
        if (langIndex != -1){
            ui->languageBox->setCurrentIndex(langIndex);
        }
        else
            ui->languageBox->setCurrentIndex(0);

        ui->testGroupBox->hide();
    }

     //files: for programming
     ActivityFile activityFileModel;
     allActivityFiles = activityFileModel.getByActivityID(currentActivity["activityID"]);
     QStringList al,a2;
     int count=allActivityFiles.count();
     for( int i=0; i<count; ++i ){
         al.append(allActivityFiles[i]["fileName"]);
     }
     ui->subFilesList->clear();
     ui->subFilesList->addItems(al);

     for(int i=0; i<count; ++i ){
         a2.append(allActivityFiles[i]["fileType"]);
     }
     ui->addFileList->clear();
     ui->addFileList->addItems(a2);
}

void activitymodifyscreen::on_backButton_clicked()
{
     w->setCentralWidget(new ActivityManagementScreen);

}

void activitymodifyscreen::on_updateBtn_clicked()
{
    QMessageBox box;
    Activity activityModel;
    ActivityFile activityFileModel;
    QMap<QString, QString> tmp = allActivities[ui->activitiesListWidget->currentRow()];
    bool isUpdated = false;


    if (tmp["isMultipleChoiceActivity"] == "1")
        tmp["pathToWorkToMark"] = ui->pathCSVEdit->text();
    if (ui->activityNameEdit->text() != "" &&
        ui->pathSolutionEdit->text() != "" ||
            tmp["isMultipleChoiceActivity"] == "1" && tmp["pathToWorkToMark"] !="")
    {
        //Get currently selected activity QMap
        //QMap<QString, QString> tmp = allActivities[ui->activitiesListWidget->currentRow()];
        QString activityID = tmp["activityID"];
        //new QMap parameters for update
        tmp["activityName"] = ui->activityNameEdit->text();
        tmp["language"] = ui->languageBox->currentText();
        tmp["pathToGradeFile"] = ui->gradefileEdit->text();
        tmp["pathToWorkToMark"] = ui->pathSubsEdit->text();
        tmp["pathToSolutionFile"] = ui->pathSolutionEdit->text();
        tmp["dueDate"] = ui->dateEdit->text();

        if(tmp["isProgrammingActivity"] == "1")
        {
            int ndx = ui->activitiesListWidget->currentRow();
            currentProgammingFiles = activityFileModel.getByActivityID(allActivities.at(ndx)["activityID"]);

            if (currentProgammingFiles.count()>0)
                DBA._deleteBatch("ActivityFile",currentProgammingFiles);

            QList<QMap<QString, QString> > newProgrammingFiles;
            int numActivityFiles = ui->subFilesList->count();
            for(int i = 0; i < numActivityFiles; i++) {
                // create the map
                QMap<QString, QString> newMap;
                newMap["activityID"] = activityID;
                newMap["fileName"] = ui->subFilesList->item(i)->text();
                newMap["fileType"] = ui->addFileList->item(i)->text();

                // insert to new list
                newProgrammingFiles.append(newMap);
            }
            // insert new to db
            if (newProgrammingFiles.count()>0)
                DBA._insertBatch("ActivityFile", newProgrammingFiles);

            ProgTest p;
            p.deleteProgTests(activityID);

            for (int i = 0; i < testList.length(); i++)
            {
                p.insertProgTest(&testList[i], activityID);
            }

            isUpdated = activityModel.updateActivity(activityID, tmp);
        }
        else if(tmp["isEssayActivity"] == "1")
        {
            isUpdated = activityModel.updateActivity(activityID, tmp);
        }
        else if(tmp["isProblemSetActivity"] == "1")
        {
            isUpdated = activityModel.updateActivity(activityID, tmp);
        }
        else if(tmp["isMultipleChoiceActivity"] == "1")
        {
            tmp["pathToWorkToMark"] = ui->pathCSVEdit->text();
            isUpdated = activityModel.updateActivity(activityID, tmp);
        }

        if (isUpdated){
            box.setText("Updated!");
            box.exec();
            w->setCentralWidget(new activitymodifyscreen);
        }
        else
        {
            box.setText("Not updated.....");
            box.exec();
        }
    }
    else
    {
        box.setText("Some required fields are empty");
        box.exec();
    }
}

void activitymodifyscreen::on_toolButtonSUB_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), "", QFileDialog::ShowDirsOnly);
    ui->pathSubsEdit->setText(dirName);
}

void activitymodifyscreen::on_toolButtonSOL_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), "", QFileDialog::ShowDirsOnly);
    ui->pathSolutionEdit->setText(dirName);
}

void activitymodifyscreen::on_toolButtonGrade_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), "", QFileDialog::ShowDirsOnly);
    ui->gradefileEdit->setText(dirName);
}

void activitymodifyscreen::on_toolButtonCSV_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "");
    ui->pathCSVEdit->setText(fileName);
}

void activitymodifyscreen::on_addFileButton_clicked()
{
    bool ok;
    QString input = QInputDialog::getText(this, "Add File", "Name:", QLineEdit::Normal, "", &ok);
    if (input.count()>0)
    {
        QString input2 = QInputDialog::getText(this, "Add type", "Name:", QLineEdit::Normal, "", &ok);
        if (input2.count()>0)
        {
             ui->subFilesList->addItem(input);
             ui->addFileList->addItem(input2);
        }

    }
}

void activitymodifyscreen::on_removeFileButton_clicked()
{
    int index = ui->subFilesList->currentRow();
    ui->subFilesList->takeItem(index);
    ui->addFileList->takeItem(index);
}

void activitymodifyscreen::on_subFilesList_itemEntered(QListWidgetItem *item)
{
     DEBUG(DEBUG_WARN, "HELLO WE CHANGED SHIT");
}

void activitymodifyscreen::on_subFilesList_currentRowChanged(int currentRow)
{
    if (currentRow > -1){
        ui->subFilesList->setCurrentRow(currentRow);
        ui->addFileList->setCurrentRow(currentRow);
    }
}

void activitymodifyscreen::on_addFileList_currentRowChanged(int currentRow)
{
    if (currentRow > -1){
        ui->subFilesList->setCurrentRow(currentRow);
        ui->addFileList->setCurrentRow(currentRow);
    }
}

void activitymodifyscreen::on_addTestButton_clicked()
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

void activitymodifyscreen::on_editTestButton_clicked()
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

void activitymodifyscreen::on_removeTestButton_clicked()
{
    int index = ui->testList->currentRow();
    ui->testList->takeItem(index);
    testList.removeAt(index);
}

void activitymodifyscreen::on_activityBtn_clicked()
{
    //edit activity
    toggleEditable(true);

    ui->label_15->setText("Modify Activity:");

    ui->deleteBtn->show();
    ui->deleteBtn_2->hide();
    ui->activityBtn->hide();
    ui->rubricBtn->hide();
    ui->updateBtn->show();
    ui->updateBtn_2->hide();

    ui->activitiesListWidget->setEnabled(false);

}

void activitymodifyscreen::on_rubricBtn_clicked()
{
    //RubricCreationScreen  dialog = new ProgrammingTestDialog(&ok, QString("Edit Programming Test"), &test);
    //w->setCentralWidget(new RubricCreationScreen(true));
    ui->rubricGroupBox->show();
    ui->rubricBtn->hide();
    ui->activityBtn->hide();
    ui->deleteBtn->hide();
    ui->deleteBtn_2->show();
    ui->gridGroupBox->hide();
    ui->testGroupBox->hide();
    ui->label_15->setText("Modify Rubric");
    ui->updateBtn->hide();
    ui->updateBtn_2->show();
    ui->activitiesListWidget->setEnabled(false);
    //load rubricItems
    itemModel = new QStandardItemModel();
    ui->tableView->setModel( itemModel );
    itemModel->setHorizontalHeaderLabels( (QStringList() << "Description" << "Weight"));

    Rubric rubricModel;
    QString rubricID = rubricModel.getRubricID(session->getActivityID());
    rubricItemsList = rubricModel.getRubric(rubricID);

    if (rubricItemsList.count()>0){
        QList<QStandardItem *>  row = QList<QStandardItem *>() << new QStandardItem("c1") << new QStandardItem("c2");
        for (int i=0; i<rubricItemsList.count(); i++){
            row[0] = new QStandardItem(rubricItemsList[i]["rubricItemText"]);
            row[1] = new QStandardItem(rubricItemsList[i]["rubricItemValue"]);
            row[1]->setTextAlignment(Qt::AlignCenter);
            itemModel->appendRow(row);
        }
        ui->tableView->resizeRowsToContents();
        ui->tableView->resizeColumnsToContents();
        ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
        ui->deleteBtn_2->setEnabled(true);
    }
    else{ //no rubic has been created, disable delete rubricButton
        ui->deleteBtn_2->setEnabled(false);
    }
}

void activitymodifyscreen::on_cancelBtn_clicked()
{
    w->setCentralWidget(new activitymodifyscreen);
}

void activitymodifyscreen::on_updateBtn_2_clicked()
{
    //update only rubric
    //to DO: remove old items first
    QMessageBox msg;
    if (itemModel->rowCount() > 0){
        Rubric rubric;
        rubric.SetParameter("activityID",session->getActivityID());
        QMap<QString, QString> one_rubric = rubric.InsertRubric(true);
        QString rid = one_rubric["rubricID"];

        RubricItem rubricItem;
        rubricModel.delRubric(session->getActivityID());
        for (int i=0 ; i<itemModel->rowCount();++i){
            rubricItem.SetParameter("rubricID",rid);
            rubricItem.SetParameter("rubricItemText", itemModel->item(i,0)->text());
            rubricItem.SetParameter("rubricItemValue", itemModel->item(i,1)->text());
            rubricItem.InsertRubricItem();
        }
        msg.setText("Section(s) updated");
        msg.exec();
        w->setCentralWidget(new activitymodifyscreen);
    }
    else{
        msg.setText("Cannot update section, please verify the input");
        msg.exec();
    }
}

void activitymodifyscreen::on_AddBtn_clicked()
{
    //add a rubricItem
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

void activitymodifyscreen::on_RemoveBtn_clicked()
{
    //remove a rubricItem
    int index = ui->tableView->currentIndex().row();
    if (index > -1)
        itemModel->removeRow(index);
}

void activitymodifyscreen::toggleEditable(bool enable)
{
    //change a set of widgets Color and Enable/Disable
    QPalette *palette = new QPalette();
    palette->setColor(QPalette::Base,QColor(242, 242, 242));
    QPalette *palette2 = new QPalette();
    palette2->setColor(QPalette::Base,Qt::white);

    ui->activityNameEdit->setReadOnly(!enable);
    ui->pathSubsEdit->setReadOnly(!enable);
    ui->toolButtonSUB->setEnabled(enable);
    ui->pathSolutionEdit->setReadOnly(!enable);
    ui->toolButtonSOL->setEnabled(enable);
    ui->gradefileEdit->setReadOnly(!enable);
    ui->toolButtonGrade->setEnabled(enable);
    ui->pathCSVEdit->setReadOnly(!enable);
    ui->toolButtonCSV->setEnabled(enable);
    ui->dateEdit->setEnabled(enable);
    ui->languageBox->setEnabled(enable);
    ui->subFileEdit->setReadOnly(!enable);
    ui->addFileButton->setEnabled(enable);
    ui->removeFileButton->setEnabled(enable);
    ui->addTestButton->setEnabled(enable);
    ui->editTestButton->setEnabled(enable);
    ui->removeTestButton->setEnabled(enable);

    if (!enable){
        ui->activityNameEdit->setPalette(*palette);
        ui->pathSubsEdit->setPalette(*palette);
        ui->pathSolutionEdit->setPalette(*palette);
        ui->gradefileEdit->setPalette(*palette);
        ui->pathCSVEdit->setPalette(*palette);
        ui->dateEdit->setPalette(*palette);
        ui->subFileEdit->setPalette(*palette);
        ui->subFilesList->setPalette(*palette);
        ui->addFileList->setPalette(*palette);
        ui->testList->setPalette(*palette);

    }else{
        ui->activityNameEdit->setPalette(*palette2);
        ui->pathSubsEdit->setPalette(*palette2);
        ui->pathSolutionEdit->setPalette(*palette2);
        ui->gradefileEdit->setPalette(*palette2);
        ui->pathCSVEdit->setPalette(*palette2);
        ui->dateEdit->setPalette(*palette2);
        ui->subFileEdit->setPalette(*palette2);
        ui->subFilesList->setPalette(*palette2);
        ui->addFileList->setPalette(*palette2);
        ui->testList->setPalette(*palette2);
    }
}

void activitymodifyscreen::on_deleteBtn_clicked()
{
    //delete activity
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete", "All related files and rubric will also be deleted.\nAre you sure want to delete this activity?",QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        rubricModel.delRubric(session->getActivityID());
        activityModel.delActivity(session->getActivityID());
        w->setCentralWidget(new activitymodifyscreen);
    } else {
    }

}

void activitymodifyscreen::on_deleteBtn_2_clicked()
{
    //delete rubric
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete", "Are you sure want to delete this rubric?",QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        rubricModel.delRubric(session->getActivityID());
        w->setCentralWidget(new activitymodifyscreen);
    } else {
    }

}
