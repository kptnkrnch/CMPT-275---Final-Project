#ifndef CSV_PROCESSOR
#define CSV_PROCESSOR

#include <fstream>
#include <sstream>
#include <string>

#include "multiplechoicetest_model.h"

#include <QMap>
#include <QString>
#include <QList>
#include <QListIterator>

/**
 * ClassName: CSVProcessor
 *
 * Description: Handles opening, closing, reading, and writing of csv files.
 *
 * Methods:
 *      CSVProcessor(std::string csvFile);
 *          Constructor that opens the specified csv file.
 *      bool openCSV(std::string _file);
 *          Opens the specified csv file.
 *      bool closeCSV();
 *          Closes the csv file.
 *      QList<QMap<QString, QString> > getStudentList();
 *          Obtains student information from a student list csv file and returns that information as a list of map objects
 *          (keys include: firstName, lastName, middleName, studentID)
 *      QMap<QString, QList<QString> > getMultipleChoiceTest();
 *          Reads a multiple choice test submission file and stores the studentID as the key in the map and the list of student
 *          answers as the value for that key.
 *      bool generateCSV(QString _file, QList<QString> data);
 *          Creates a new csv file and writes the contents of the data list to that file (each item in data represents a line).
 *
 * Members:
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
class CSVProcessor {

public:
    CSVProcessor();
    ~CSVProcessor();
    CSVProcessor(std::string csvFile);
    bool openCSV(std::string _file);
    bool closeCSV();

    QList<QMap<QString, QString> > getStudentList();
    QMap<QString, QList<QString> > getMultipleChoiceTest();
    bool generateCSV(QString _file, QList<QString> data);

private:
    std::fstream file;
};

#endif // CSV_PROCESSOR

