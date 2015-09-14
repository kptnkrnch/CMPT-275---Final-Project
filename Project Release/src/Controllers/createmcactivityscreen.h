#ifndef CREATEMCACTIVITYSCREEN_H
#define CREATEMCACTIVITYSCREEN_H

#include <QWidget>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QCalendarWidget>
#include <QMap>
#include <QStandardItemModel>


namespace Ui {
class createmcactivityscreen;
}

class createmcactivityscreen : public QWidget
{
    Q_OBJECT

public:
    explicit createmcactivityscreen(QWidget *parent = 0);
    ~createmcactivityscreen();

private slots:
    void on_backButton_clicked();

    void on_toolButton_clicked();

    void on_createButton_clicked();

    void on_addAnswerButton_clicked();

    void on_removeAnswerButton_clicked();

    void on_toolButton_2_clicked();

private:
    Ui::createmcactivityscreen *ui;
    QStandardItemModel *itemModel;
};

#endif // CREATEMCACTIVITYSCREEN_H
