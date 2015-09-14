#include "model.h"
#include "activity_model.h"
#include "rubric_model.h"
#include "user_model.h"

QList<QMap<QString, QString> > Activity::getRubric() {
    QList<QMap<QString, QString> > rubric;

    Rubric r;

    QString activityID = params["activityID"];
    QString rubricID = r.getRubricID(activityID);
    rubric = r.getRubric(rubricID);
    return rubric;
}

QList<QMap<QString, QString> > Activity::getRubric(QString activityID) {
    QList<QMap<QString, QString> > rubric;

    Rubric r;

    QString rubricID = r.getRubricID(activityID);
    rubric = r.getRubric(rubricID);
    return rubric;
}

QMap<QString, QString> Activity::GetActivityByID(QString _activityID) {
    QMap<QString, QString> activity;
    try {
        activity = getDBItem(_activityID);
    } catch (ParameterException e) {
        DEBUG(DEBUG_ALRM, e.what());
    }
    return activity;
}

bool Activity::InsertActivity() {
    params.remove("activityID");
    try {
        insertDBItem(params, true);
        return true;
    } catch (ParameterException e) {
        DEBUG(DEBUG_ALRM, e.what());
        return false;
    }
}

QMap<QString, QString> Activity::InsertActivity(bool returnInsertedItem) {
    params.remove("activityID");
    QMap<QString, QString> temp;
    try {
        temp = insertDBItem(params, returnInsertedItem);
        return temp;
    } catch (ParameterException e) {
        DEBUG(DEBUG_ALRM, e.what());
        return temp;
    }
}

QString Activity::getID()
{
    return params["activityID"];
}

void Activity::SetParameter(QString name, QString value) {
    if (name != NULL) {
        params.insert(name, value);
    }
}

void Activity::SetParameters(QMap<QString, QString> _params) {
    params = _params;
}

bool Activity::updateActivity(QString activityID, QMap<QString, QString> valuesToChange) {
    // Lookup the course to change just by the key since it can't change
    QMap<QString, QString> tmp;
    tmp.insert(getKey(), activityID);

    // Auto incremented, so why would this need to change
    valuesToChange.remove(getKey());

    return updateDBItem(tmp, valuesToChange);
}

QList<QMap<QString, QString> > Activity::getByCourseID(QStringList courseIDs) {
    return parseResult(DBA._fromWhere(getTableName(), "courseID", courseIDs));
}

QList<QMap<QString, QString> > Activity::getByCourseID(QString courseID) {
    return getDBItems("courseID", courseID);

}


QList<QMap<QString, QString> > Activity::getByCourseIDIfCanGrade(QString courseID, int role) {
    QStringList ql;
    Rubric r;
    ql.append(courseID);

    QList<QMap<QString, QString> > items = getDBItems("courseID", ql);
    QList<QMap<QString, QString> > itemsCanGrade;

    for (int i = 0; i < items.length(); i++)
    {
        QString actID = items[i]["activityID"];
        QString rubricID = r.getRubricID(actID);
        if ((rubricID != "" && items[i]["pathToSolutionFile"] != "") || items[i]["isMultipleChoiceActivity"] == "1")
        {
            itemsCanGrade.append(items[i]);
        }

    }
    return itemsCanGrade;
}

QList<QMap<QString, QString> > Activity::getRubriclessByCourseID(QString currentCourseID){

    QString sqlString = "SELECT DISTINCT * FROM Activity WHERE courseID ='";
    sqlString.append(currentCourseID);
    sqlString.append("' AND activityID NOT IN(SELECT DISTINCT A.activityID FROM Rubric R, Activity A WHERE R.activityID = A.activityID AND A.courseID ='" );
    sqlString.append(currentCourseID);
    sqlString.append("')");

    DEBUG(DEBUG_INFO, sqlString);

    return parseResult(DBA.__query(sqlString));
}


bool Activity::delActivity(QString _activityID) {
    if (!deleteDBItem(_activityID))
        return false;

    ActivityFile activityFileModel;
    ProgTest ProgTestModel;
    MultipleChoiceTest MCTestModel;
    if(!activityFileModel.deleteActivityFiles(_activityID)) {
        return false;
    }
    if (session->getActivityType()=="Programming")
        ProgTestModel.deleteProgTests(_activityID);
    if (session->getActivityType()=="Multiple Choice")
        MCTestModel.deleteMCAnswers(_activityID);
    return true;
}

