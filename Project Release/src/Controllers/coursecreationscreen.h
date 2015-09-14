#ifndef COURSECREATIONSCREEN_H
#define COURSECREATIONSCREEN_H

#include "ui_coursecreationscreen.h"
#include "mainwindow.h"

#include "../Models/student_model.h"
#include "../Models/user_model.h"
#include "../Models/course_model.h"
#include "../Models/session.h"
#include "../Models/marker_model.h"
#include "../Controllers/adminassistanthomescreen.h"
#include "../Controllers/loginscreen.h"
#include "../Helpers/controller_helper.h"
#include "../Models/csv_processor.h"

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QCompleter>

namespace Ui {
class CourseCreationScreen;
}

class CourseCreationScreen : public QWidget
{
    Q_OBJECT

public:
    explicit CourseCreationScreen(QWidget *parent = 0);
    ~CourseCreationScreen();

private slots:

    void on_buSave_clicked();

    void on_buCancel_clicked();

    void on_liAllTAs_currentRowChanged(/* int currentRow*/);

    void on_liCourseTAs_currentRowChanged(/* int currentRow */);

    void on_buAdd_clicked();

    void on_buRemove_clicked();

    void on_leStudentList_textChanged(const QString &fileName);

    void on_buBrowse_clicked();

private:
    Ui::CourseCreationScreen *ui;

    Course courseModel;

    User userModel;
    QStringList allTAIDs;
    QStringList allInstructorIDs;
    QCompleter *instrCompleter;

    Marker markerModel;
    QStringList *currentCourseTAIDs;

    Student studentModel;
    QList<QMap<QString, QString> > currentCourseStudents;

};

#endif // COURSECREATIONSCREEN_H
