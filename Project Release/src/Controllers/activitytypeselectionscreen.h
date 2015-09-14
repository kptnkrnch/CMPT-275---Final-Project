#ifndef ACTIVITYTYPESELECTIONSCREEN_H
#define ACTIVITYTYPESELECTIONSCREEN_H

#include <QWidget>

namespace Ui {
class activitytypeselectionscreen;
}

class activitytypeselectionscreen : public QWidget
{
    Q_OBJECT

public:
    explicit activitytypeselectionscreen(QWidget *parent = 0);
    ~activitytypeselectionscreen();

private slots:

    void on_backButton_clicked();

    void on_SelectButton_clicked();

private:
    Ui::activitytypeselectionscreen *ui;
};

#endif // ACTIVITYTYPESELECTIONSCREEN_H
