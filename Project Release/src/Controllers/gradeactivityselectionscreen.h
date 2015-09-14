#ifndef GRADEACTIVITYSELECTIONSCREEN_H
#define GRADEACTIVITYSELECTIONSCREEN_H

#include <QWidget>

namespace Ui {
class GradeActivitySelectionScreen;
}

class GradeActivitySelectionScreen : public QWidget
{
    Q_OBJECT

public:
    explicit GradeActivitySelectionScreen(QWidget *parent = 0);
    ~GradeActivitySelectionScreen();

private slots:
    void on_selectActivityButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::GradeActivitySelectionScreen *ui;
    QList<QMap<QString, QString> > allActivities;
};

#endif // GRADEACTIVITYSELECTIONSCREEN_H
