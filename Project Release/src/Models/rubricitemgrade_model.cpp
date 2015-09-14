#include "rubricitemgrade_model.h"
#include "../Helpers/debug.h"
#include "activity_model.h"
#include "csv_processor.h"
#include "student_model.h"
#include <QMapIterator>
#include <QListIterator>
#include <QStringList>
#include <sstream>
#include <fstream>
#include "dbaccess.h"

extern DBAccess DBA;

/**
 * Method: InsertRubricItemGrade(QMap<QString, QString> _grade)
 *
 * Description:
 *      Handles inserting a new RubricItemGrade entry into the database (column names and values are stored in the _grade
 *      map parameter).
 *
 * Attributes:
 *      QMap<QString, QString> _grade - Map containing columns and vectors corresponding to the RubricItemGrade table in the database
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
void RubricItemGrade::InsertRubricItemGrade(QMap<QString, QString> _grade) {
    try {
        insertDBItem(_grade, false);
    } catch (ParameterException e) {
        DEBUG(DEBUG_ALRM, e.what());
    }
}

/**
 * Method: UpdateRubricItemGrade(QMap<QString, QString> _grade)
 *
 * Description:
 *      Handles updating an existing RubricItemGrade entry in the database (column names and values are stored in the
 *      _grade map parameter (with the RubricItemGrade's gradeID set)).
 *
 * Attributes:
 *      QMap<QString, QString> _grade - Map containing columns and vectors corresponding to the RubricItemGrade table in the database
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
void RubricItemGrade::UpdateRubricItemGrade(QMap<QString, QString> _grade) {
    QMap<QString, QString> pkeys;
    pkeys.insert("gradeID", _grade["gradeID"]);
    _grade.remove("gradeID");
    updateDBItem(pkeys, _grade);
}

/**
 * Method: GenerateGradeFile(QString _activityID)
 *
 * Description:
 *      Obtains the grades for all students in a course that is tied to the _activityID, obtains the grades for those
 *      students for that activity, and then stores each student grade file line in the returned QList<QString>
 *      (formatted for writing to a file using the CSVProcessor).
 *
 * Attributes:
 *      QString _activityID - the id of the activity to get grades for
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
QList<QString> RubricItemGrade::GenerateGradeFile(QString _activityID) {
    Activity activityModel;
    QMap<QString, QString> activityMap = activityModel.GetActivityByID(_activityID);
    activityModel.SetParameters(activityMap);
    QList<QMap<QString, QString> > rubricMapList = activityModel.getRubric();
    QListIterator<QMap<QString, QString> > rubricMapListIterator(rubricMapList);
    Student s;
    QList<QString> studentList = s.getEnrolledStudentIDs(activityMap["courseID"]);
    QList<QString> gradeOutput;
    for (size_t i = 0; i < studentList.count(); i++) {
        std::ostringstream oss;
        oss << studentList[i].toStdString();
        while(rubricMapListIterator.hasNext()) {
            QMap<QString, QString> tempMap = rubricMapListIterator.next();
            QList<QMap<QString, QString> > itemGrades = GetGradesByItemID(tempMap["rubricItemID"]);
            QListIterator<QMap<QString, QString> > itemGradesIterator(itemGrades);
            while (itemGradesIterator.hasNext()) {
                QMap<QString, QString> tempItemGrade = itemGradesIterator.next();
                if (tempItemGrade["studentID"] == studentList[i]) {
                    oss << "," << tempItemGrade["mark"].toStdString();
                }
            }
        }
        oss << "\n";
        gradeOutput.push_back(oss.str().c_str());
    }
    return gradeOutput;
}

/**
 * Method: GetGradesByItemID(QString _rubricItemID)
 *
 * Description:
 *      Retrieves all student grades for a particular Rubric Item.
 *
 * Attributes:
 *      QString _rubricItemID - the rubric item's id to get grades by
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
QList<QMap<QString, QString> > RubricItemGrade::GetGradesByItemID(QString _rubricItemID) {
    if (DBA.isConnected()) {
        std::ostringstream oss;
        oss << "SELECT * FROM RubricItemGrade WHERE rubricItemID='" << _rubricItemID.toStdString() << "' ";
        oss << "ORDER BY studentID ASC;";
        QSqlQuery * query = DBA.__query(oss.str().c_str());
        return parseResult(query);
    }
}

/**
 * Method: CheckIfCanBeRemarked(QString _activityID, QString _studentID, int _markerPrivilegeLevel)
 *
 * Description:
 *      Checks if the current user has the appropriate privilege level to remark an activity for a student.
 *
 * Attributes:
 *      QString _activityID - the activity id of the grades
 *      QString _studentID - the student who those grades belong to
 *      int _markerPrivilegeLevel - the privilege of the current marker to compare to the last marker
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
bool RubricItemGrade::CheckIfCanBeRemarked(QString _activityID, QString _studentID, int _markerPrivilegeLevel) {
    QList<QMap<QString, QString> > gradeItems = GetActivityGradesByStudentID(_activityID, _studentID);
    QListIterator<QMap<QString, QString> > iter(gradeItems);
    bool can_mark = true;
    while(iter.hasNext()) {
        QMap<QString, QString> temp = iter.next();
        if (atoi(temp["markerPrivilege"].toStdString().c_str()) < _markerPrivilegeLevel && atoi(temp["markerPrivilege"].toStdString().c_str()) != 0) {
            can_mark = false;
            break;
        }
    }
    return can_mark;
}

/**
 * Method: CheckIfHasBeenMarked(QString _activityID, QString _studentID)
 *
 * Description:
 *      Checks if any marks exist for a particular activity and a particular student (primarily used to see if an
 *      administrator can remark an activity).
 *
 * Attributes:
 *      QString _activityID - the activity the grades belong to
 *      QString _studentID - the student the grades belong to
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
bool RubricItemGrade::CheckIfHasBeenMarked(QString _activityID, QString _studentID) {
    QList<QMap<QString, QString> > gradeItems = GetActivityGradesByStudentID(_activityID, _studentID);
    if (gradeItems.size() > 0) {
        return true;
    } else {
        return false;
    }
}

/**
 * Method: GetActivityGradesByStudentID(QString _activityID, QString _studentID)
 *
 * Description:
 *      Returns a list of RubricItemGrades in the form of a map (table columns and values) for a particular activity and
 *      a particular student.
 *
 * Attributes:
 *      QString _activityID - the activity the grades belong to
 *      QString _studentID - the student the grades belong to
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
QList<QMap<QString, QString> > RubricItemGrade::GetActivityGradesByStudentID(QString _activityID, QString _studentID) {
    QList<QMap<QString, QString> > gradeItems;
    Activity activityModel;
    QMap<QString, QString> activityMap = activityModel.GetActivityByID(_activityID);
    activityModel.SetParameters(activityMap);
    QList<QMap<QString, QString> > rubricMapList = activityModel.getRubric();
    QListIterator<QMap<QString, QString> > rubricMapListIterator(rubricMapList);
    if (rubricMapListIterator.hasNext()) {
        QMap<QString, QString> rubric = rubricMapListIterator.next();
        std::ostringstream oss;
        oss << "SELECT RubricItemGrade.gradeID, RubricItemGrade.mark, RubricItemGrade.groupID, RubricItemGrade.studentID, ";
        oss << "RubricItemGrade.markerPrivilege, RubricItemGrade.markerID, RubricItemGrade.rubricItemID ";
        oss << "FROM RubricItemGrade JOIN RubricItem ON RubricItemGrade.rubricItemID = RubricItem.rubricItemID ";
        oss << "WHERE RubricItem.rubricID='" << rubric["rubricID"].toStdString().c_str() << "' AND ";
        oss << "RubricItemGrade.studentID='" << _studentID.toStdString().c_str() << "';";
        QSqlQuery * query = DBA.__query(oss.str().c_str());
        gradeItems = parseResult(query);
        if (gradeItems.size() > 0) {
            DEBUG(DEBUG_ALRM, gradeItems[0]["markerPrivilege"]);
        }
    }
    return gradeItems;
}

/**
 * Method: delRubricItemGrade(QStringList _rubricItems)
 *
 * Description:
 *      Deletes rubric item grades from the database
 *
 * Attributes:
 *      QStringList _rubricItems - contains the rubricItemIDs to delete on
 *
 * Author: Ching Lam
 *
 * Version: 1
 */
bool RubricItemGrade::delRubricItemGrade(QStringList _rubricItems){
    return deleteDBItems("rubricItemID",_rubricItems);
}
