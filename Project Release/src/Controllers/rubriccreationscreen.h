#ifndef RUBRICCREATIONSCREEN_H
#define RUBRICCREATIONSCREEN_H

#include <string>
#include <QString>
#include <QWidget>
#include <QMap>
#include "QStandardItemModel.h"

#include "../Models/rubric_model.h"
#include "../Models/rubricitem_model.h"
namespace Ui {
class RubricCreationScreen;
}

class RubricCreationScreen : public QWidget
{
    Q_OBJECT

public:
    explicit RubricCreationScreen(QWidget *parent = 0);
    explicit RubricCreationScreen(bool loadRubrics,QWidget *parent = 0);
    ~RubricCreationScreen();

private slots:
    void on_ComfirmBtn_clicked();

    void on_backButton_clicked();

    void on_AddBtn_clicked();

    void on_RemoveBtn_clicked();

private:
    Ui::RubricCreationScreen *ui;
    QMap<QString, QString> rubricList;
    QStandardItemModel *itemModel;
    Rubric rubricModel;
    QList<QMap<QString, QString> > rubricItemsList;
};

#endif // CREATERUBRICSCREEN_H
