#ifndef RUBRICITEMGRADE_MODEL
#define RUBRICITEMGRADE_MODEL

#include "model.h"
#include <QMap>
#include <QString>
#include <QList>

/**
 * ClassName: RubricItemGrade
 *
 * Description: Handles interactions between the application and the database in regards to rubric grades.
 *
 * Methods:
 *      RubricItemGrade():Model("RubricItemGrade", "gradeID")
 *          Constructor that sets the RubricItemGrade to point to the RubricItemGrade table in the database with the
 *          primary key "gradeID".
 *      void InsertRubricItemGrade(QMap<QString, QString> _grade)
 *          Handles inserting a new RubricItemGrade entry into the database (column names and values are stored in the _grade
 *          map parameter).
 *      void UpdateRubricItemGrade(QMap<QString, QString> _grade)
 *          Handles updating an existing RubricItemGrade entry in the database (column names and values are stored in the
 *          _grade map parameter (with the RubricItemGrade's gradeID set)).
 *      QList<QString> GenerateGradeFile(QString _activityID)
 *          Obtains the grades for all students in a course that is tied to the _activityID, obtains the grades for those
 *          students for that activity, and then stores each student grade file line in the returned QList<QString>
 *          (formatted for writing to a file using the CSVProcessor).
 *      QList<QMap<QString, QString> > GetGradesByItemID(QString _rubricItemID)
 *          Retrieves all student grades for a particular Rubric Item.
 *      bool CheckIfCanBeRemarked(QString _activityID, QString _studentID, int _markerPrivilegeLevel)
 *          Checks if the current user has the appropriate privilege level to remark an activity for a student.
 *      bool CheckIfHasBeenMarked(QString _activityID, QString _studentID)
 *          Checks if any marks exist for a particular activity and a particular student (primarily used to see if an
 *          administrator can remark an activity).
 *      QList<QMap<QString, QString> > GetActivityGradesByStudentID(QString _activityID, QString _studentID)
 *          Returns a list of RubricItemGrades in the form of a map (table columns and values) for a particular activity and
 *          a particular student.
 *      bool delRubricItemGrade(QStringList _rubricItems)
 *
 * Members:
 *
 * Author: Joshua Campbell and Ching Lam
 *
 * Version: 1
 */
class RubricItemGrade: public Model {
public:
    RubricItemGrade():Model("RubricItemGrade", "gradeID") {
        params = getEmptyRecord();
    }
    void InsertRubricItemGrade(QMap<QString, QString> _grade);
    void UpdateRubricItemGrade(QMap<QString, QString> _grade);
    QList<QString> GenerateGradeFile(QString _activityID);
    QList<QMap<QString, QString> > GetGradesByItemID(QString _rubricItemID);
    bool CheckIfCanBeRemarked(QString _activityID, QString _studentID, int _markerPrivilegeLevel);
    bool CheckIfHasBeenMarked(QString _activityID, QString _studentID);
    QList<QMap<QString, QString> > GetActivityGradesByStudentID(QString _activityID, QString _studentID);
    bool delRubricItemGrade(QStringList _rubricItems);

private:
    QMap<QString, QString> params;
};

#endif // RUBRICITEMGRADE_MODEL

