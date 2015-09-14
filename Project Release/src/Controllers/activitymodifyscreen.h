#ifndef ACTIVITYMODIFYSCREEN_H
#define ACTIVITYMODIFYSCREEN_H
#include <QListWidgetItem>
#include <string>
#include <QString>
#include <QWidget>
#include <QMap>
#include "QStandardItemModel.h"
#include "../Models/rubric_model.h"
#include "../Models/rubricitem_model.h"
#include "../Models/activity_model.h"
#include "../Models/activityfile_model.h"
#include "programmingtest.h"

namespace Ui {
class activitymodifyscreen;
}

class activitymodifyscreen : public QWidget
{
    Q_OBJECT

public:
    explicit activitymodifyscreen(QWidget *parent = 0);
    ~activitymodifyscreen();

private slots:
    void on_activitiesListWidget_currentRowChanged(int currentRow);

    void on_backButton_clicked();

    void on_updateBtn_clicked();

    void on_toolButtonSUB_clicked();

    void on_toolButtonSOL_clicked();

    void on_toolButtonGrade_clicked();

    void on_addFileButton_clicked();

    void on_removeFileButton_clicked();

    void on_subFilesList_itemEntered(QListWidgetItem *item);

    void on_subFilesList_currentRowChanged(int currentRow);

    void on_addFileList_currentRowChanged(int currentRow);

    void on_toolButtonCSV_clicked();

    void on_addTestButton_clicked();

    void on_editTestButton_clicked();

    void on_removeTestButton_clicked();

    void on_activityBtn_clicked();

    void on_rubricBtn_clicked();

    void on_deleteBtn_clicked();

    void on_cancelBtn_clicked();

    void on_updateBtn_2_clicked();

    void on_AddBtn_clicked();

    void on_RemoveBtn_clicked();

    void toggleEditable(bool enable);

    void on_deleteBtn_2_clicked();

private:
    Ui::activitymodifyscreen *ui;
    QList<QMap<QString, QString> > allActivities;
    QList<QMap<QString, QString> > allActivityFiles;
    Activity activityModel;

    QList<QMap<QString, QString> > currentProgammingFiles;
    QList<ProgrammingTest> testList;

    QMap<QString, QString> rubricList;
    QStandardItemModel *itemModel;
    Rubric rubricModel;
    QList<QMap<QString, QString> > rubricItemsList;

};

#endif // ACTIVITYMODIFYSCREEN_H
