#include "student_model.h"

QList<QMap<QString, QString> > Student::getEnrolledStudents(QString courseID) {
    QList<QMap<QString, QString> > students;
    students = getDBItems("courseID", QStringList(courseID));
    return students;
}

QList<QMap<QString, QString> > Student::processStudentListFile(QString fileName) throw (OpenFileException) {
    QList<QMap<QString, QString> > studentIDs;

    if(fileName.isEmpty()) {
        throw OpenFileException("Filename was not specified");
    }

    // Always fails
    CSVProcessor csvp;
    if(!csvp.openCSV(fileName.toStdString())) {
        throw OpenFileException("Could not open file at the specified path");
    }

    // How can check if open successfully, always returns empty list
//    CSVProcessor csvp(fileName.toStdString());
    studentIDs = csvp.getStudentList();
    DEBUG(DEBUG_WARN, "New student list size: " << QString::number(studentIDs.size()));
    return studentIDs;
}

bool Student::setCourseStudentList(QString courseID, QList<QMap<QString, QString> > newStudentList) {
    // deleteDBItems(getEnrolledStudents(courseID));
    // removeAllFromCourse(courseID); <-- preferred

    bool out = true;
    int newClassSize = newStudentList.size();
    QStringList currentStudentIDs;

    //QMap<QString, QString> newStudents; // students who appear in newStudentList but not currently enrolled
    QStringList droppedStudents;

    QList<QMap<QString, QString> > newStudents;
    QList<QMap<QString, QString> > removedStudents;

    currentStudentIDs = getEnrolledStudentIDs(courseID);

    // Find any new students
    for(int i = 0; i < newClassSize; ++i) {
        QMap<QString, QString> iStudent = newStudentList.at(i);
        // TODO low priority...
        // This checks if the userID already in the table;
        // So, if the student was previously put int he course only by the studentID
        // and now, the new course list has his full name info, the user will not be updated..
        if(currentStudentIDs.contains(Student::getStudentID(&iStudent))) {
            continue;
        }
        Course::setCourseID(courseID, &iStudent);
        newStudents.append(iStudent);
    }

    // Find any dropped students
    foreach(QString i, currentStudentIDs) {
        for(int i = 0; i < newClassSize; ++i) {
            QMap<QString, QString> iStudent = newStudentList.at(i);
            QString iStudentID = Student::getStudentID(&iStudent);
            if(droppedStudents.contains(iStudentID)) {
                continue;
            }
            Course::setCourseID(courseID, &iStudent);
            droppedStudents.append(iStudentID);
        }
    }

    if(newStudents.size() > 0) {
        out = insertDBItems(newStudents);
    }

    if(removedStudents.size() > 0) {
        out = out && deleteDBItems(removedStudents);
    }

    return out;
}

QList<QString> Student::getEnrolledStudentIDs(QString courseID) {
    QList<QMap<QString, QString> > enrolledStudents;
    QList<QString> enrolledIDs;
    int classSize;

    enrolledStudents = getDBItems("courseID", QStringList(courseID));
    classSize = enrolledStudents.size();
    if(classSize == 0) {
        return enrolledIDs;
    }

    for(int i = 0; i < classSize; ++i) {
        QMap<QString, QString> j = enrolledStudents.at(i);
        enrolledIDs.push_back(getStudentID(&j));
    }

    return enrolledIDs;
}

bool Student::enrollStudentCourses(QString userID, QStringList courseIDs) {
    DEBUG(DEBUG_ALRM, "enrollStudentCourses(" << userID << ", " << courseIDs
          << ") NOT YET IMPLEMENTED!!");
    return false;
}

QMap<QString, QString> Student::createStudent() {
    QMap<QString, QString> newStudent = getEmptyRecord();
    // Initialize parameters..

    return newStudent;
}

bool Student::deleteStudent(QString userID) {
    DEBUG(DEBUG_ALRM, "deleteStudent("
          << userID << ") NOT YET IMPLEMENTED!!");
    return false;
}

bool Student::removeFromCourse(QString studentID, QString courseID) {
    DEBUG(DEBUG_ALRM, "removeFromCourse(" << studentID << ", "
          << courseID << ") NOT YET IMPLEMENTED!!");
    return false;
}

bool Student::removeAllFromCourse(QString courseID) {
    return deleteDBItems(getEnrolledStudents(courseID));
}

bool Student::addToCourse(QString studentID, QString courseID) {
    DEBUG(DEBUG_ALRM, "addToCourse(" << studentID<< ", " << courseID
          << ") NOT YET IMPLEMENTED!!");
    return false;
}

bool Student::setStudentID(QString newValue, QMap<QString, QString> *student, bool verify) {
    if(verify && !verifyStudentID(newValue)) {
        return false;
    }
    return setParameter("studentID", newValue, student);
}
bool Student::verifyStudentID(QString newValue) {
    // Use same checks as a user ID
    return User::verifyUserID(newValue);
}
QString Student::getStudentID(QMap<QString, QString> const * const student) {
    return getParameter("studentID", student);
}
