#ifndef COURSEMODIFYSCREEN_H
#define COURSEMODIFYSCREEN_H

#include "ui_coursemodifyscreen.h"
#include "mainwindow.h"

#include "../Models/user_model.h"
#include "../Models/course_model.h"
#include "../Models/session.h"
#include "../Models/marker_model.h"
#include "../Models/student_model.h"
#include "../Controllers/adminassistanthomescreen.h"
#include "../Controllers/loginscreen.h"
#include "../Helpers/controller_helper.h"

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QCompleter>

namespace Ui {
class coursemodifyscreen;
}

class CourseModifyScreen : public QDialog
{
    Q_OBJECT

public:
    explicit CourseModifyScreen(QWidget *parent = 0);
    ~CourseModifyScreen();

private slots:
    void on_buSave_clicked();
    void on_liCourses_currentRowChanged(int currentRow);
    void on_buDelete_clicked();
    void on_buRevert_clicked();
    void on_buCancel_clicked();
    void on_buAdd_clicked();
    void on_buRemove_clicked();
    void on_liCourseTAs_currentRowChanged(/* int currentRow */);
    void on_liAllTAs_currentRowChanged(/* int currentRow */);
    void on_buBrowse_clicked();


    void on_leStudentList_textChanged(const QString &arg1);

    void on_pushButton_clicked();

private:
    void setKnownFields(QMap<QString, QString> const * const course, QStringList const * const TAs);
    void refresh();
    Ui::coursemodifyscreen *ui;

    Course courseModel;
    QStringList allCourseNames;
    QMap<QString, QString> *currentCourse;
    QList<QMap<QString, QString> > allCourses;

    User userModel;
    QStringList allTAIDs;
    QStringList allInstructorIDs;
    QCompleter *instrCompleter;

    Marker markerModel;
    QStringList *currentCourseTAIDs;

    Student studentModel;
    QList<QMap<QString, QString> > currentCourseStudents;

};

#endif // COURSEMODIFYSCREEN_H
