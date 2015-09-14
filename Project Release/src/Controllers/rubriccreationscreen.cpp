#include "rubriccreationscreen.h"
#include "ui_rubriccreationscreen.h"
#include "mainwindow.h"

#include "../Models/session.h"
#include "../Models/rubric_model.h"
#include "../Models/activity_model.h"
#include "../Controllers/instructorhomescreen.h"
#include "../Controllers/loginscreen.h"

extern MainWindow *w;
extern Session *session;

RubricCreationScreen::RubricCreationScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RubricCreationScreen)
{
    ui->setupUi(this);
    itemModel = new QStandardItemModel();
    ui->tableView->setModel( itemModel );
    itemModel->setHorizontalHeaderLabels( (QStringList() << "Description" << "Weight"));

}

RubricCreationScreen::RubricCreationScreen(bool loadRubrics, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RubricCreationScreen)
{
    ui->setupUi(this);
    itemModel = new QStandardItemModel();
    ui->tableView->setModel( itemModel );
    itemModel->setHorizontalHeaderLabels( (QStringList() << "Description" << "Weight"));

    Rubric rubricModel;
    QString rubricID = rubricModel.getRubricID(session->getActivityID());
    rubricItemsList = rubricModel.getRubric(rubricID);

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

}

RubricCreationScreen::~RubricCreationScreen()
{
    delete ui;
}

void RubricCreationScreen::on_backButton_clicked()
{
    w->setCentralWidget(new InstructorHomeScreen);
}

void RubricCreationScreen::on_ComfirmBtn_clicked()
{
    QMessageBox msg;
    if (itemModel->rowCount() > 0){
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
        msg.setText("Section(s) created");
        msg.exec();
        w->setCentralWidget(new InstructorHomeScreen);
    }
    else{
        msg.setText("Cannot create section, please verify the input");
        msg.exec();
    }
}

void RubricCreationScreen::on_AddBtn_clicked()
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

void RubricCreationScreen::on_RemoveBtn_clicked()
{
    int index = ui->tableView->currentIndex().row();
    if (index > -1)
        itemModel->removeRow(index);
}
