#ifndef ACTIVITYMANAGEMENTSCREEN_H
#define ACTIVITYMANAGEMENTSCREEN_H

#include <QWidget>

namespace Ui {
class activitymanagementscreen;
}

class ActivityManagementScreen : public QWidget
{
    Q_OBJECT

public:
    explicit ActivityManagementScreen(QWidget *parent = 0);
    ~ActivityManagementScreen();

private slots:

    void on_backButton_clicked();

    void on_createActivityButton_clicked();

    void on_createRubricButton_clicked();

    void on_modifyActivityButton_clicked();

    void on_modifyRubricButton_clicked();

private:
    Ui::activitymanagementscreen *ui;
};

#endif // ACTIVITYMANAGEMENTSCREEN_H
