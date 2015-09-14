#include "course_model.h"

bool Course::InsertCourse() {
    params.remove("courseID");
    try {
        QMap<QString, QString> temp = insertDBItem(params, true);
        //params["courseID"] = temp["courseID"];
        params.insert("CourseID", temp["courseID"]);
        return true;
    } catch (ParameterException e) {
        DEBUG(DEBUG_ALRM, e.what());
        return false;
    }
}
QString Course::getID()
{
    return params["CourseID"];
}


bool Course::insertCourse(QMap<QString, QString> *course, bool chckInstructor)
        throw (NotExistsException, DBGetException) {

    try {
        if(chckInstructor && !verifyInstructorExists(*course)) {
            // userID not an instructor
            return false;
        }
    } catch (DBGetException& e) {
        // userID does not exist
        throw e;
    } catch (NotExistsException& e) {
        // userID not an instructor
        throw e;
    }
    // Auto increment key, don't try to insert it!
    course->remove(getKey());

    // Return the course with the courseID
    try {
        *course  = insertDBItem(*course, true);
    } catch (ParameterException& e) {

    }

    return true;
}


bool Course::verifyInstructorExists(QString instructorID) throw (NotExistsException, DBGetException) {
    try {
        User userModel;
        QMap<QString, QString> instructor = userModel.getUser(instructorID);
        if(!User::isInstructor(&instructor)) {
            // IMPOSTER!
            throw NotExistsException(instructorID.toStdString()
                + " is not an instructor");
        }
    } catch (DBGetException& e) {
        // Doesn't even exist
        throw e;
    }
    return true;
}

bool Course::verifyInstructorExists(QMap<QString, QString> course) throw (NotExistsException, DBGetException) {
    QString instructorID = Course::getInstructorID(&course);
    return verifyInstructorExists(instructorID);
}

bool Course::assignInstructorToCourses(QString instructorID, QList<QMap<QString, QString> > courses) {
    QStringList courseIDs;
    int size = courses.size();

    for(int i = 0; i < size; i++) {
        QMap<QString, QString> iCourse = courses.at(i);
        courseIDs.append(Course::getCourseID(&iCourse));
    }

    return assignInstructorToCourses(instructorID, courseIDs);
}

bool Course::assignInstructorToCourses(QString instructorID, QStringList courseIDs) {
    try {
        verifyInstructorExists(instructorID);
    } catch(NotExistsException& e) {
        DEBUG(DEBUG_INFO, e.what());
        return false;
    } catch (DBGetException& e) {
        DEBUG(DEBUG_INFO, e.what());
        return false;
//    } catch (std::exception& e) {
//        DEBUG(DEBUG_INFO, e.what());
//        return false;
    }

    QList<QMap<QString, QString> > courseList;

    QMap<QString, QString> values;
    Course::setInstructorID(instructorID, &values);

    courseList = getDBItems("courseID", courseIDs);
    return updateDBItems(courseList, values);
}

bool Course::removeInstructorFromAllCourses(QString instructorID) {
    QList<QMap<QString, QString> > coursesTaught;
    QMap<QString, QString> newVal;

    coursesTaught = getCoursesByUser(instructorID, User::ROLE_INSTRUCTOR);

    Course::setInstructorID("", &newVal);
    return updateDBItems(coursesTaught, newVal);
}

bool Course::deleteCourse(QString courseID) {
    Activity activityModel;
    Marker markerModel;
    Student studentModel;

    // Only delete a course if it has no activities
    if(activityModel.getByCourseID(courseID).size() > 0) {
        DEBUG(DEBUG_INFO, "Course has some activities");
        return false;
    }

    // Remove TAs assigned to this course
    if(!markerModel.removeCourse(courseID)) {
        DEBUG(DEBUG_WARN, "Could not remove TAs from course");
        return false;
    }

    // Remove students enrolled in this course
    if(!studentModel.removeAllFromCourse(courseID)) {
        DEBUG(DEBUG_WARN, "Could not remove students from course");
        return false;
    }

    return deleteDBItem(courseID);
}

bool Course::updateCourse(QString courseID, QMap<QString, QString> valuesToChange) {
    // Lookup the course to change just by the key since it can't change
    QMap<QString, QString> tmp;
    tmp.insert(getKey(), courseID);

    // Auto incremented, so why would this need to change
    valuesToChange.remove(getKey());

    return updateDBItem(tmp, valuesToChange);
}

QMap<QString, QString> Course::createCourse() {
    QMap<QString, QString> out = getEmptyRecord();

    // can initialize some values here

    return out;
}

QList<QMap<QString, QString> > Course::getAllCourses() {
    return getAll();
}
QList<QMap<QString, QString> > Course::getCoursesByUser(QString userID, int role) {
    QString sqlStr;
    QSqlQuery * query;
    if (role == User::ROLE_INSTRUCTOR) {
        sqlStr = "SELECT * FROM Course WHERE instructorID='" + userID + "';";
        query = DBA.__query(sqlStr);
    } else if (role == User::ROLE_ADMINISTRATOR) {
        sqlStr = "SELECT * FROM Course;";
        query = DBA.__query(sqlStr);
    } else if (role == User::ROLE_TEACHINGASSISTANT) {
        sqlStr = "SELECT Course.courseID, Course.courseNumber, Course.courseName, Course.startDate, ";
        sqlStr += "Course.endDate, Course.instructorID FROM Marker LEFT JOIN Course ON ";
        sqlStr += "Marker.courseID=Course.courseID WHERE Marker.userID='" + userID + "';";
        query = DBA.__query(sqlStr);
    }
    return parseResult(query);
}

QString Course::getCourseID(QMap<QString, QString> const * const course) {
    QString out = QString();
    try {
        out = getParameter("courseID", course);
    } catch (NullPointerException& e) {
        DEBUG(DEBUG_WARN, e.what());
    }
    return out;
}
QString Course::getCourseNumber(QMap<QString, QString> const * const course) {
    QString out = QString();
    try {
        out = getParameter("courseNumber", course);
    } catch (NullPointerException& e) {
        DEBUG(DEBUG_WARN, e.what());
    }
    return out;
}
QString Course::getCourseName(QMap<QString, QString> const * const course) {
    QString out = QString();
    try {
        out = getParameter("courseName", course);
    } catch (NullPointerException& e) {
        DEBUG(DEBUG_WARN, e.what());
    }
    return out;
}
QString Course::getDateFormat() {
    return QString("yyyy-MM-dd");
}
QDate Course::getStartDate(QMap<QString, QString> const * const course) {
    QString out = QString();
    try {
        out = getParameter("startDate", course);
    } catch (NullPointerException& e) {
        DEBUG(DEBUG_WARN, e.what());
    }
    return QDate::fromString(out, getDateFormat());
}
QDate Course::getEndDate(QMap<QString, QString> const * const course) {
    QString out = QString();
    try {
        out = getParameter("endDate", course);
    } catch (NullPointerException& e) {
        DEBUG(DEBUG_WARN, e.what());
    }
    return QDate::fromString(out, getDateFormat());
}
QString Course::getInstructorID(QMap<QString, QString> const * const course) {
    QString out = QString();
    try {
        out = getParameter("instructorID", course);
    } catch (NullPointerException& e) {
        DEBUG(DEBUG_WARN, e.what());
    }
    return out;
}
QString Course::getParameter(QString parameter, QMap<QString, QString> const * const course) throw (NullPointerException){
    if(course == NULL) {
        throw NullPointerException("Course not defined");
    }
    return course->value(parameter);
}

bool Course::setParameter(QString parameter, QString newValue, QMap<QString, QString> *course) {
    if(course == NULL) {
        return false;
    }
    course->insert(parameter, newValue);
    return true;
}
bool Course::setCourseID        (QString newValue, QMap<QString, QString> *course, bool verify) {
    if(verify && !verifyCourseID(newValue)) {
        return false;
    }
    return setParameter("courseID", newValue, course);
}
bool Course::setCourseNumber    (QString newValue, QMap<QString, QString> *course, bool verify) {
    if(verify && !verifyCourseNumber(newValue)) {
        return false;
    }
    return setParameter("courseNumber", newValue, course);
}
bool Course::setCourseName      (QString newValue, QMap<QString, QString> *course, bool verify) {
    if(verify && !verifyCourseName(newValue)) {
        return false;
    }
    return setParameter("courseName", newValue, course);
}
bool Course::setInstructorID    (QString newValue, QMap<QString, QString> *course, bool verify) {
    if(verify && !verifyInstructorID(newValue)) {
        return false;
    }
    return setParameter("instructorID", newValue, course);
}
bool Course::setStartDate    (QString newValue, QMap<QString, QString> *course, bool verify) {
    if(verify && !verifyDates(course)) {
        return false;
    }
    return setParameter("startDate", newValue, course);
}
bool Course::setEndDate    (QString newValue, QMap<QString, QString> *course, bool verify) {
    if(verify && !verifyDates(course)) {
        return false;
    }
    return setParameter("endDate", newValue, course);
}

// TEMPORARY! MUST BE FILLED IN LATER!!
bool Course::verifyCourseID      (QString newValue) {
    if(0 >= newValue.length()) {
        return false;
    }
    return true;
}
bool Course::verifyCourseNumber  (QString newValue) {
    if(0 >= newValue.length()
            || newValue.length() >= LEN_CNUMBER) {
        return false;
    }
    return true;
}
bool Course::verifyCourseName    (QString newValue) {
    if(0 >= newValue.length()
            || newValue.length() >= LEN_CNAME) {
        return false;
    }
    return true;
}
bool Course::verifyDates     (QMap<QString, QString> const * const course) {
    if(course == NULL) {
        return false;
    }
    return getStartDate(course) <= getEndDate(course);
}
bool Course::verifyInstructorID  (QString newValue) {
    // Is it a user ID foreign key?
    // Course may have NO instructor?
    return newValue.isEmpty() || User::verifyUserID(newValue);
//    return newValue.isEmpty() || User::verifyEmployeeID(newValue);
}

bool Course::updateInstructor(QString curInstructorID, QString newInstructorID) {
     return updateDBItems("instructorID", curInstructorID, newInstructorID);
}
