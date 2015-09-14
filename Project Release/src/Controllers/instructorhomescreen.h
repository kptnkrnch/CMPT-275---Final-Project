#ifndef INSTRUCTORHOMESCREEN_H
#define INSTRUCTORHOMESCREEN_H

#include <QWidget>

namespace Ui {
class InstructorHomeScreen;
}

class InstructorHomeScreen : public QWidget
{
    Q_OBJECT

public:
    explicit InstructorHomeScreen(QWidget *parent = 0);
    ~InstructorHomeScreen();

private slots:

    void on_manageActivitiesButton_clicked();

    void on_backButton_clicked();

    void on_gradeActivitiesButton_clicked();

private:
    Ui::InstructorHomeScreen *ui;
};

#endif // INSTRUCTORHOMESCREEN_H
