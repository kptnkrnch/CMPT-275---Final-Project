#ifndef CREATEPROGACTIVITYSCREEN_H
#define CREATEPROGACTIVITYSCREEN_H

#include <QWidget>
#include <QFileDialog>
#include <QInputDialog>
#include <QApplication>
#include <QMessageBox>
#include <string>
#include <QString>
#include <QMap>
#include "QStandardItemModel.h"

#include "../Models/rubric_model.h"
#include "../Models/rubricitem_model.h"
#include "programmingtest.h"

namespace Ui {
class createprogactivityscreen;
}

class createprogactivityscreen : public QWidget
{
    Q_OBJECT

public:
    explicit createprogactivityscreen(QWidget *parent = 0);
    ~createprogactivityscreen();

private slots:
    void on_backButton_clicked();

    void on_addFileButton_clicked();

    void on_createProgActivityButton_clicked();

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_removeFileButton_clicked();

    void on_addTestButton_clicked();

    void on_editTestButton_clicked();

    void on_removeTestButton_clicked();

    void on_fileList_currentRowChanged(int currentRow);

    void on_typeList_currentRowChanged(int currentRow);

    void on_toolButton_3_clicked();

    void on_AddBtn_clicked();

    void on_RemoveBtn_clicked();

    void on_pushButton_clicked();

private:
    Ui::createprogactivityscreen *ui;
    QList<ProgrammingTest> testList;
    QMap<QString, QString> rubricList;
    QStandardItemModel *itemModel;
    Rubric rubricModel;
    QList<QMap<QString, QString> > rubricItemsList;
};

#endif // CREATEPROGACTIVITYSCREEN_H
