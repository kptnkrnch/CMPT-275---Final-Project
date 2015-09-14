#ifndef PROGTEST_MODEL_H
#define PROGTEST_MODEL_H

#include <QString>
#include <QMap>
#include <QList>
#include "model.h"
#include "dbaccess.h"
#include "../programmingtest.h"

/**
* ClassName: ProgTest
* Inherits: Model
*
* Description: Handles all data manipulation, and database queries to the ProgrammingTest table. Operations include
*              adding, deleting, accessing programming tests by the activityID of the activity to which they belong.
*              a ProgrammingTest object represents the data in the table.
*
* Methods:
*   QString insertProgTest(ProgrammingTest * test, QString activityID);
*       Inserts a ProgrammingTest object's data into the ProgrammingTest table; the test will belong to the activity
*       indentified by activityID in the database. Returns the ID of the newly inserted item
*   QList<ProgrammingTest> getProgTests(QString activityID);
*       Returns a list of all programming tests belonging to the specified activity that exist in the database
*   bool deleteProgTests(QString activityID);
*       Deletes all programming tests associated with the activity identified by activityID from the database;
*       also recursively removes all ProgrammingTestInputs data and ProgrammingTestOutputs data associated
*       with the deleted tests
*
*
* Members:
*   params
*       A QMap with each key representing a column in the ProgrammingTest table.
*
* Author: Connor MacLeod
*
* Version: 1
*/

extern DBAccess DBA;
class ProgTest : public Model {
public:
    ProgTest() : Model("ProgrammingTest", "programmingTestID") {
        params = getEmptyRecord();
    }

    QString insertProgTest(ProgrammingTest * test, QString activityID);
    QList<ProgrammingTest> getProgTests(QString activityID);
    bool deleteProgTests(QString activityID);


private:
    QMap<QString, QString> params;
};

#endif // PROGTEST_MODEL_H

