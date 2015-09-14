#ifndef COURSESELECTIONSCREEN_H
#define COURSESELECTIONSCREEN_H

#include <QWidget>
#include <QString>
#include <QMap>

namespace Ui {
class CourseSelectionScreen;
}

class CourseSelectionScreen : public QWidget
{
    Q_OBJECT

public:
    explicit CourseSelectionScreen(QWidget *parent = 0);
    ~CourseSelectionScreen();
    void AddCourse(QString courseNumber, QString courseName);

private slots:
    void on_selectCourseButton_clicked();

    //void on_backButton_clicked();

private:
    Ui::CourseSelectionScreen *ui;
    QList<QMap<QString, QString> > allCourses;
    //QList<QMap<QString, QString> > allActivities;
};

#endif // COURSESELECTIONSCREEN_H
