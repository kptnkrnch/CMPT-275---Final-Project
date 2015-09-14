#ifndef MARKER_MODEL_H
#define MARKER_MODEL_H

#include <QString>
#include <QMap>
#include <QList>
#include "model.h"
#include "user_model.h"
#include "course_model.h"
#include "../Helpers/debug.h"
#include "dbaccess.h"

extern DBAccess DBA;

/**
* ClassName: Marker
* Inherits: Model
*
* Description: Handles all data manipulation, and database queries to the marker table. Operations include
*              adding, deleting, accessing, markers by various parameters.
*              A map, specifically QMap, is used to represent an object of this table.
*			   The key of this map represents the column in the table with its assigned value.
*
* Methods:
*   bool insertMarker(QMap<QString, QString> *marker);
*       The map represents a marker to be inserted, returns false if insertion failed due to
*       incorrect keys in  the map that represent this marker.
*   bool removeMarker(QString userID);
*       Removes the marker with specified userID from this table entirely. This marker is removed
*       from marking any course. Usually used when an account is removed the marker privilege.
*   bool removeMarker(QString userID, QString courseID);
*       Removes the marker from a specific course.
*   bool removeCourse(QString courseID);
*       Removes all markers for a specified course.
*
*   bool InsertMarker();
*       Inserts the marker specified in the params member of this class.
*   void SetParameter(QString name, QString value);
*       Adds name as a key to the params map with specified value. name represents a column for
*       the marker table.
*
*   QStringList getMarkerIDsByCourse(QString courseID);
*       Returns a list of userIDs that are assigned to courseID.
*   QList<QMap<QString, QString> > getMarkersByCourse(QString courseID);
*       Returns the markers (as a QMap with each key representing a column in the Marker table).
*   QList<QMap<QString, QString> >  getCoursesByMarker(QString userID);
*       Returns a list of complete Marker objects (as a QMap with each key representing a column in the Marker table)
*       of all markers that are assigned as a TA of the specified course.
*
*   QMap<QString, QString> createMarker();
*       Returns a marker with default parameters set (as a QMap with each key representing a column in the Marker table).

*   bool setCourseTA(QString userID, QString courseID);
*       Assigns userID to be a marker of courseID.
*
*   bool setCourseTAs(QStringList userIDs, QString courseID);
*       Assigns a list of usersIDs to be markers of courseID.
*
*   bool assignTACourses(QString userID, QStringList courseIDs);
*       Assigns userID to be a marker of specified courseIDs.
*   bool assignTACourses(QString userID, QList<QMap<QString, QString> > courses);
*       Assigns a userID to be a marker of specified courses. Courses is a list of maps where each map must contain a
*       courseID key.
*
*   bool updateTA(QString currID, QString newID);
*       Update the userID for a specific TA in all courses they are markers of.
*
* Members:
*   params
*       A QMap with each key representing a column in the Marker table.
*
* Author: German Villarreal & Connor MacLeod &
*
* Version: 1
*/
class Marker : public Model {
public:
    Marker() : Model("Marker", "courseID") {
        params = getEmptyRecord();
    }

    bool insertMarker(QMap<QString, QString> *marker);
    bool removeMarker(QString userID);
    bool removeMarker(QString userID, QString courseID);
    bool removeCourse(QString courseID);

    bool InsertMarker();
    void SetParameter(QString name, QString value);

    QStringList getMarkerIDsByCourse(QString courseID);
    QList<QMap<QString, QString> > getMarkersByCourse(QString courseID);
    QList<QMap<QString, QString> >  getCoursesByMarker(QString userID);

    QMap<QString, QString> createMarker();
    bool setCourseTA(QString userID, QString courseID);
    bool setCourseTAs(QStringList userIDs, QString courseID);

    bool assignTACourses(QString userID, QList<QMap<QString, QString> > courses);
    bool assignTACourses(QString userID, QStringList courseIDs);

    bool updateTA(QString currID, QString newID);

    static bool setPrivilege(QString newValue, QMap<QString, QString> *marker, bool verify = false);
    static bool verifyPrivilege(QString newValue);
    static int  getPrivilege(QMap<QString, QString> const * const marker);
private:
    QMap<QString, QString> params;
};

#endif // MARKER_MODEL_H

