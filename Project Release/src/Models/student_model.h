#ifndef STUDENT_MODEL_H
#define STUDENT_MODEL_H

#include "model.h"
#include "user_model.h"
#include "course_model.h"
#include "csv_processor.h"

#include "../Exceptions/openfileexception.h"

#include <QList>
#include <QMap>
#include <QString>

/**
* ClassName: Student
* Inherits: Model
*
* Description: Student model to abstract database query calls. A map, specifically QMap, is used to represent an
*              object of the database. The key of this map represents the column in the datbase with its assigned value.
*			   This class handles students' additions and withdrawals from courses.
*
* Methods:
 *	 Student() : Model ("Student", "studentID") { }
 *			Initializes the model to represent the Student table with the studentID primary key.
 *	 
 *	 QList<QMap<QString, QString> > processStudentListFile(QString fileName) throw (OpenFileException);
 *			Opens a CSV file formatted to contain student information and returns all students as a list of maps, where each map
 *			represents one student and the keys of the map are a column of the student table.
 *	 bool setCourseStudentList(QString courseID, QList<QMap<QString, QString> > newStudentList);
 *			Compares current class list of students to newStudentList; removes any students that have dropped the course and adds
 *			new students enrolled to the course.
 *	 bool enrollStudentCourses(QString userID, QStringList courseIDs);
 *			Enrolls the student with userID to the courses specified by courseIDs.
 *	 bool removeAllFromCourse(QString courseID);
 *			Withdraws ever student from the specified course.
 *	 QMap<QString, QString> createStudent();
 *			Returna  a map representing a student object where each key of the map is a column of the student table.
 *	 bool deleteStudent(QString studentID);
 *			Removes the student from all courses.
 *	
 *	 
 *	 
 *	 QList<QMap<QString, QString> > getEnrolledStudents(QString courseID);
 *			Returns full information of all students enrolled into specified course.
 *	 QList<QString> getEnrolledStudentIDs(QString courseID);
 *			Returns a class list of studentIDs for the specified course.
 *	 bool addToCourse(QString studentID, QString courseID);
 *			Enrolls specified student to specified course.
 *	 bool removeFromCourse(QString studentID, QString courseID);
 *			Withdraws specified student from specified course.
 *	 
 *	 static bool setStudentID(QString newValue, QMap<QString, QString> *student, bool verify = false);
 *	 static bool verifyStudentID(QString newValue);
 *	 static QString getStudentID(QMap<QString, QString> const * const student);
*
* Author: German Villarreal
*
* Version: 1
*/
class Student : public Model {
public:

    Student() : Model ("Student", "studentID") { }
    ~Student() { }

    QList<QMap<QString, QString> > processStudentListFile(QString fileName) throw (OpenFileException);
    bool setCourseStudentList(QString courseID, QList<QMap<QString, QString> > newStudentList);
    bool enrollStudentCourses(QString userID, QStringList courseIDs);
    bool removeAllFromCourse(QString courseID);

    QMap<QString, QString> createStudent();
    bool deleteStudent(QString studentID);


    QList<QMap<QString, QString> > getEnrolledStudents(QString courseID);
    QList<QString> getEnrolledStudentIDs(QString courseID);
    bool addToCourse(QString studentID, QString courseID);
    bool removeFromCourse(QString studentID, QString courseID);

    static bool setStudentID(QString newValue, QMap<QString, QString> *student, bool verify = false);
    static bool verifyStudentID(QString newValue);
    static QString getStudentID(QMap<QString, QString> const * const student);


private:

};

#endif // STUDENT_MODEL_H
