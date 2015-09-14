#ifndef PROGTESTOUTPUT_MODEL_H
#define PROGTESTOUTPUT_MODEL_H

#include <QString>
#include <QMap>
#include <QList>
#include "model.h"
#include "dbaccess.h"
#include "../programmingtest.h"

/**
* ClassName: ProgTestOutput
* Inherits: Model
*
* Description: Handles all data manipulation, and database queries to the ProgrammingTestOutputs table. Operations include
*              adding, deleting, accessing programming test output files by the testID of the ProgrammingTest to which they belong.
*              A map, specifically QMap, is used to represent an object of this table.
*			   The key of this map represents the column in the table with its assigned value.
*
* Methods:
*   void insertOutputFiles(QList<QMap<QString, QString> > outputs);
*       Inserts a list of Programming Test output files into the database.
*   QList<QMap<QString, QString> > getOutputFiles(QString testID);
*       Returns a list of all programming test output files belonging to the specified ProgrammingTest that exist in the database
*   bool deleteProgTestOutputs(QString testID);
*       Deletes all programming test output files associated with the ProgrammingTest identified by testID from the database
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
class ProgTestOutput : public Model {
public:
    ProgTestOutput() : Model("ProgrammingTestOutputs", "testFileID") {
        params = getEmptyRecord();
    }

    void insertOutputFiles(QList<QMap<QString, QString> > outputs);
    QList<QMap<QString, QString> > getOutputFiles(QString testID);
    bool deleteProgTestOutputs(QString testID);

private:
    QMap<QString, QString> params;
};

#endif // PROGTESTOUTPUT_MODEL_H

