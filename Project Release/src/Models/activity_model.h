#ifndef ACTIVITY_MODEL_H
#define ACTIVITY_MODEL_H

#include <QString>
#include <QMap>
#include <QList>
#include "../Models/model.h"
#include "activityfile_model.h"
#include "multiplechoicetest_model.h"
#include "progtest_model.h"

#include "../Helpers/debug.h"
#include "../Models/dbaccess.h"
#include "session.h"

/**
 * ClassName: Activity
 *
 * Description: Handles interations between the application and the database when dealing with activities.
 *              Handles some interactions between activities and rubrics.
 *
 * Methods:
 *      Activity():Model("Activity", "activityID")
 *          Constructor that sets the Activity to point to the Activity table in the database with the
 *          primary key "gradeID".
 *      QList<QMap<QString, QString> > getRubric()
 *          Returns a list of rubric items stored in a map (column name, value) for this activity
 *      static QList<QMap<QString, QString> > getRubric(QString activityID)
 *          Returns a list of rubric items stored in a map (column name, value) for the specified activityID
 *      QMap<QString, QString> GetActivityByID(QString _activityID)
 *          Returns a the activity from the database that is associated with the _activityID parameter and stores
 *          it in a map (column name, value)
 *      QMap<QString, QString> InsertActivity(bool returnInsertedItem = true)
 *          Inserts an activity and returns the newly created activity if returnInsertedItem is true
 *      bool InsertActivity()
 *          Inserts a new activity using Activity's param member
 *      QString getID()
 *          
 *      void SetParameter(QString name, QString value)
 *          Sets a parameter in the param map to the name (key) and value (value)
 *      void SetParameters(QMap<QString, QString> _params)
 *          Sets the param member to _params
 *      bool updateActivity(QString activityID, QMap<QString, QString> valuesToChange)
 *          Updates the specified activity in the database who has the same activity and stores the contents of valuesToChange
 *          in that row.
 *      QList<QMap<QString, QString> >  getByCourseID(QStringList courseIDs)
 *          
 *      QList<QMap<QString, QString> >  getByCourseID(QString courseID)
 *          
 *      QList<QMap<QString, QString> >  getByCourseIDIfCanGrade(QString courseID, int role)
 *          
 *      QList<QMap<QString, QString> > getRubriclessByCourseID(QString currentCourseID)
 *          
 *      bool delActivity (QString _activityID)
 *          
 *
 * Members:
 *      QMap<QString, QString> params - stores table columns and their values in a map structure for an activity
 *
 * Author: Connor MacLeod
 *
 * Version: 1
 */

extern Session *session;
extern DBAccess DBA;
class Activity: public Model {
public:
    Activity():Model("Activity", "activityID") {
        params = getEmptyRecord();
    }

    QList<QMap<QString, QString> > getRubric();
    static QList<QMap<QString, QString> > getRubric(QString activityID);
    QMap<QString, QString> GetActivityByID(QString _activityID);
    QMap<QString, QString> InsertActivity(bool returnInsertedItem = true);
    bool InsertActivity();
    QString getID();
    void SetParameter(QString name, QString value);
    void SetParameters(QMap<QString, QString> _params);
    bool updateActivity(QString activityID, QMap<QString, QString> valuesToChange);
    QList<QMap<QString, QString> >  getByCourseID(QStringList courseIDs);
    QList<QMap<QString, QString> >  getByCourseID(QString courseID);
    QList<QMap<QString, QString> >  getByCourseIDIfCanGrade(QString courseID, int role);
    QList<QMap<QString, QString> > getRubriclessByCourseID(QString currentCourseID);
    bool delActivity (QString _activityID);
private:
    QMap<QString, QString> params;
};


#endif // ACTIVITY_MODEL_H
