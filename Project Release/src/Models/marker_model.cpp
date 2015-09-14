#include "model.h"
#include "marker_model.h"

bool Marker::insertMarker(QMap<QString, QString> *marker) {
    // Check if the courseid and userid exist in their pk table
    try {
        insertDBItem(*marker);
    } catch (ParameterException& e) {
        return false;
    }
    return true;
}

bool Marker::updateTA(QString currID, QString newID) {
    return updateDBItems("userID", currID, newID);
}

bool Marker::removeMarker(QString userID) {
    QList<QMap<QString, QString> > removeList;
    QMap<QString, QString> removeParams;

    User::setUserID(userID, &removeParams);
    removeList.append(removeParams);

    return deleteDBItems(removeList);
}

bool Marker::removeMarker(QString userID, QString courseID) {
    QMap<QString, QString> delMarker;
    Course::setCourseID(courseID, &delMarker);
    User::setUserID(userID, &delMarker);
    return deleteDBItem(delMarker);
}

bool Marker::InsertMarker() {
    try {
        insertDBItem(params, false);
        return true;
    } catch (ParameterException e) {
        DEBUG(DEBUG_ALRM, e.what());
        return false;
    }
}

void Marker::SetParameter(QString name, QString value) {
    if (name != NULL) {
        params[name] = value;
    }
}

// What is the privilege level?
bool Marker::verifyPrivilege(QString newValue) {
    if(newValue.toInt() < 0) {
        return false;
    }
    return true;
}

bool Marker::setPrivilege(QString newValue, QMap<QString, QString> *marker, bool verify) {
    if(verify && !verifyPrivilege(newValue)) {
        return false;
    }
    return setParameter("privilegeLevel", newValue, marker);
}

QMap<QString, QString> Marker::createMarker() {
    QMap<QString, QString> out = getEmptyRecord();
    setPrivilege("1", &out);
    return out;
}

bool Marker::setCourseTA(QString userID, QString courseID) {
    QMap<QString, QString> marker = createMarker();

    User::setUserID(userID, &marker);
    Course::setCourseID(courseID, &marker);

    return insertDBItem(marker);
}

bool Marker::setCourseTAs(QStringList userIDs, QString courseID) {
    bool out = true;
    QStringList newAssigned;
    QStringList alreadyAssigned;
    QStringList removeAssigned;
    QList<QMap<QString, QString> > courseMarkers;
    QList<QMap<QString, QString> > removedMarkers;

    alreadyAssigned = getMarkerIDsByCourse(courseID);

    // Find any new TAs
    foreach(QString tmp, userIDs) {
        if(alreadyAssigned.contains(tmp)) {
            continue;
        }
        newAssigned << tmp;
    }

    // Check for removed TAs
    foreach(QString tmp, alreadyAssigned) {
        if(!userIDs.contains(tmp)) {
            removeAssigned << tmp;
        }
    }

    // Prepare to insert
    foreach(QString userID, newAssigned) {
        QMap<QString, QString> tmp = createMarker();
        User::setUserID(userID, &tmp);
        Course::setCourseID(courseID, &tmp);
        courseMarkers.append(tmp);
    }

    // Prepare to remove
    foreach(QString userID, removeAssigned) {
        QMap<QString, QString> tmp;
        // only want to look and delete by the primary key,
        // so we will now filter on privilege level
        User::setUserID(userID, &tmp);
        Course::setCourseID(courseID, &tmp);
        removedMarkers.append(tmp);
    }

    if(removedMarkers.size() > 0) {
        out = deleteDBItems(removedMarkers);
    }

    if (courseMarkers.size() > 0) {
        out = out && insertDBItems(courseMarkers);
    }
    return out;
}

bool Marker::assignTACourses(QString userID, QList<QMap<QString, QString> > courses) {
    QStringList courseIDs;
    int size = courses.size();

    for(int i = 0; i < size; i++) {
        QMap<QString, QString> iCourse = courses.at(i);
        courseIDs.append(Course::getCourseID(&iCourse));
    }

    return assignTACourses(userID, courseIDs);
}

bool Marker::assignTACourses(QString userID, QStringList courseIDs) {
    QList<QMap<QString, QString> > newMarkers;

    foreach(QString courseID, courseIDs) {
        QMap<QString, QString> iMarker = createMarker();
        User::setUserID(userID, &iMarker);
        Course::setCourseID(courseID, &iMarker);
        newMarkers.append(iMarker);
    }

    return insertDBItems(newMarkers);
}

QList<QMap<QString, QString> > Marker::getMarkersByCourse(QString courseID) {
    return getDBItems("courseID", QStringList(courseID));
}

QStringList Marker::getMarkerIDsByCourse(QString courseID) {
    QStringList out;
    QList<QMap<QString, QString> > markers = getMarkersByCourse(courseID);
    int count = markers.size();
    for(int i = 0; i < count; i++) {
        QMap<QString, QString> tmp = markers.at(i);
        out << User::getUserID(&tmp);
    }
    return out;
}

QList<QMap<QString, QString> >  Marker::getCoursesByMarker(QString userID) {
    return getDBItems("userID", userID);
}

bool Marker::removeCourse(QString courseID) {
    return deleteDBItems("courseID", courseID);
}
