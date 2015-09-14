#ifndef ADMINASSISTANTHOMESCREEN_H
#define ADMINASSISTANTHOMESCREEN_H

#include <QWidget>

namespace Ui {
class AdminAssistantHomeScreen;
}

class AdminAssistantHomeScreen : public QWidget
{
    Q_OBJECT

public:
    explicit AdminAssistantHomeScreen(QWidget *parent = 0);
    ~AdminAssistantHomeScreen();

private slots:

    void on_createCourseButton_clicked();

    void on_modifyCourseButton_clicked();

private:
    Ui::AdminAssistantHomeScreen *ui;
};

#endif // ADMINASSISTANTHOMESCREEN_H
