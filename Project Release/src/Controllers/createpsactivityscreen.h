#ifndef CREATEPSACTIVITYSCREEN_H
#define CREATEPSACTIVITYSCREEN_H

#include <QWidget>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <string>
#include <QString>
#include <QMap>
#include "QStandardItemModel.h"

#include "../Models/rubric_model.h"
#include "../Models/rubricitem_model.h"

namespace Ui {
class createpsactivityscreen;
}

class createpsactivityscreen : public QWidget
{
    Q_OBJECT

public:
    explicit createpsactivityscreen(QWidget *parent = 0);
    ~createpsactivityscreen();

private slots:
    void on_backButton_clicked();

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_createActivityButton_clicked();

    void on_toolButton_3_clicked();

    void on_AddBtn_clicked();

    void on_RemoveBtn_clicked();

    void on_pushButton_clicked();

private:
    Ui::createpsactivityscreen *ui;
    QMap<QString, QString> rubricList;
    QStandardItemModel *itemModel;
    Rubric rubricModel;
    QList<QMap<QString, QString> > rubricItemsList;
};

#endif // CREATEPSACTIVITYSCREEN_H
