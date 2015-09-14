#ifndef COURSE_MODEL
#define COURSE_MODEL

#include <QWidget>
#include <QString>
#include <QMap>
#include <QList>
#include <QTime>

#include "model.h"
#include "user_model.h"
#include "activity_model.h"
#include "student_model.h"
#include "dbaccess.h"
#include "../Exceptions/nullpointerexception.h"
#include "../Exceptions/notexistsexception.h"
#include "../Exceptions/dbgetexception.h"
#include "../Helpers/debug.h"

extern DBAccess DBA;

/**
* ClassName: Course
* Inherits: Model
*
* Description:		Course model to abstract database query calls. A map, specifically QMap, is used to represent an
*					object of the database. The key of this map represents the column in the datbase with its assigned value.
*					This class handles data manipulation, insertion, deletion and verification of the course table.
*
* Methods:
*	Course();
*		Creates a course model, that allows access to the course table for manipulation.
*	
*	QList<QMap<QString, QString> > getAllCourses();
*		Returns a list of courses, as maps, where each key to the map is a column of the Course table.
*	QList<QMap<QString, QString> > getCoursesByUser(QString userID, int role);
*		Returns a list of courses, as maps, where each key of the map is a column in the Course table.
*	QMap<QString, QString> createCourse();
*		Returns a blank course with predefined defaults, as a map where each key of the map is a column in the Course table.
*	
*	bool insertCourse(QMap<QString, QString> *course, bool chckInstructor = false) throw (NotExistsException, DBGetException);
*		Inserts the course, as a map, where each key of the map is a column in the Course table, into the Course table. If the chckInstructor flag is set,
*		it will check if the instructorID belongs to a valid instructor from the Users table. Throws NotExistsException when the instructorID does not belong to
*		an Instructor in User table. Throws DBGetException when the instructorID does not belong to any user in the User table.
*	
*	bool updateCourse(QString courseID, QMap<QString, QString> valuesToChange);
*		Changes the specified parameters found in valuesToChange, where each key is a column found in the Course table, for the specified courseID.
*	bool updateInstructor(QString currID, QString newID);
*		Replaces an instructorID in all courses with a new instructorID.
*	bool deleteCourse(QString courseID);
*		Removes the course from the database is the course does not contain any activities. It withdraws all students, and delists
*		any markers associated with the course.
*	
*	bool verifyInstructorExists(QMap<QString, QString> course) throw (NotExistsException, DBGetException);
*		Checks if the value found under the instructorID key belongs to an instructor, user exists, and has valid instructor priviliges. 
*		Throws NotExistsException when the instructorID does not belong to an Instructor in User table. Throws DBGetException when the
*		instructorID does not belong to any user in the User table.
*	bool verifyInstructorExists(QString instructorID) throw (NotExistsException, DBGetException);
*		Overriden method, checks if instructorID is a valid instructor.
*	bool removeInstructor(QString instructorID, QString courseID);
*		Removes the instructor from the specified course.
*	bool removeInstructorFromAllCourses(QString instructorID);
*		Removes the instructor from all courses.
*	
*	bool assignInstructorToCourses(QString instructorID, QStringList courseIDs);
*		Adds the instructor to all of the specified courses.
*	bool assignInstructorToCourses(QString instructorID, QList<QMap<QString, QString> > courses);
*		Adds the instructor ro all courses specified by the maps representing courses in the course table,
*		where each key to the map is a column to the course table.
*	
*	static QString getCourseID      (QMap<QString, QString> const * const course);
*	static QString getCourseNumber  (QMap<QString, QString> const * const course);
*	...
*	
*	static bool setCourseID      (QString newValue, QMap<QString, QString> *course, bool verify = false);
*	static bool setCourseNumber  (QString newValue, QMap<QString, QString> *course, bool verify = false);
*	...
*	static bool verifyCourseID      (QString newValue);
*	static bool verifyCourseNumber  (QString newValue);
*	...
*
* Author: German Villarreal
*
* Version: 1
*/
class Course : public Model {
public:

    static const int LEN_CNUMBER = 10;
    static const int LEN_CNAME = 60;

    Course() : Model("Course", "courseID") {
        params = getEmptyRecord();
    }

    QString getID();
    bool InsertCourse();

    QList<QMap<QString, QString> > getAllCourses();
    QList<QMap<QString, QString> > getCoursesByUser(QString userID, int role);

    QMap<QString, QString> createCourse();

    bool insertCourse(QMap<QString, QString> *course, bool chckInstructor = false) throw (NotExistsException, DBGetException);

    bool updateCourse(QString courseID, QMap<QString, QString> valuesToChange);
    bool updateInstructor(QString currID, QString newID);
    bool deleteCourse(QString courseID);

    bool verifyInstructorExists(QMap<QString, QString> course) throw (NotExistsException, DBGetException);
    bool verifyInstructorExists(QString instructorID) throw (NotExistsException, DBGetException);
    bool removeInstructor(QString instructorID, QString courseID);
    bool removeInstructorFromAllCourses(QString instructorID);

    bool assignInstructorToCourses(QString instructorID, QStringList courseIDs);
    bool assignInstructorToCourses(QString instructorID, QList<QMap<QString, QString> > courses);

    static QString getCourseID      (QMap<QString, QString> const * const course);
    static QString getCourseNumber  (QMap<QString, QString> const * const course);
    static QString getCourseName    (QMap<QString, QString> const * const course);
    static QDate   getStartDate     (QMap<QString, QString> const * const course);
    static QDate   getEndDate       (QMap<QString, QString> const * const course);
    static QString getInstructorID  (QMap<QString, QString> const * const course);
    static QString getDateFormat();

    static bool setCourseID      (QString newValue, QMap<QString, QString> *course, bool verify = false);
    static bool setCourseNumber  (QString newValue, QMap<QString, QString> *course, bool verify = false);
    static bool setCourseName    (QString newValue, QMap<QString, QString> *course, bool verify = false);
    static bool setStartDate     (QString newValue,   QMap<QString, QString> *course, bool verify = false);
    static bool setEndDate       (QString newValue,   QMap<QString, QString> *course, bool verify = false);
    static bool setInstructorID  (QString newValue, QMap<QString, QString> *course, bool verify = false);

    static bool verifyCourseID      (QString newValue);
    static bool verifyCourseNumber  (QString newValue);
    static bool verifyCourseName    (QString newValue);
    static bool verifyDates         (QMap<QString, QString> const * const course);
    static bool verifyInstructorID  (QString newValue);

private:
    QMap<QString, QString> params;
    static QString getParameter(QString parameter, QMap<QString, QString> const * const course) throw (NullPointerException);
    static bool setParameter(QString parameter, QString newVal, QMap<QString, QString> *course);

};

#endif // COURSE_MODEL

