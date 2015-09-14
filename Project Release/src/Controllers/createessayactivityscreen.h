#ifndef CREATEESSAYACTIVITYSCREEN_H
#define CREATEESSAYACTIVITYSCREEN_H

#include <QFileDialog>
#include <QWidget>
#include <string>
#include <QString>
#include <QMap>
#include "QStandardItemModel.h"

#include "../Models/rubric_model.h"
#include "../Models/rubricitem_model.h"


namespace Ui {
class createessayactivityscreen;
}

class createessayactivityscreen : public QWidget
{
    Q_OBJECT

public:
    explicit createessayactivityscreen(QWidget *parent = 0);
    ~createessayactivityscreen();

private slots:
    void on_backButton_clicked();

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_createButton_clicked();

    void on_toolButton_3_clicked();

    void on_AddBtn_clicked();

    void on_RemoveBtn_clicked();

    void on_pushButton_clicked();

private:
    Ui::createessayactivityscreen *ui;
    QMap<QString, QString> rubricList;
    QStandardItemModel *itemModel;
    Rubric rubricModel;
    QList<QMap<QString, QString> > rubricItemsList;
};

#endif // CREATEESSAYACTIVITYSCREEN_H
