#ifndef PROGTESTINPUT_MODEL_H
#define PROGTESTINPUT_MODEL_H

#include <QString>
#include <QMap>
#include <QList>
#include "model.h"
#include "dbaccess.h"
#include "../programmingtest.h"

/**
* ClassName: ProgTestInput
* Inherits: Model
*
* Description: Handles all data manipulation, and database queries to the ProgrammingTestInputs table. Operations include
*              adding, deleting, accessing programming test input files by the testID of the ProgrammingTest to which they belong.
*              A map, specifically QMap, is used to represent an object of this table.
*			   The key of this map represents the column in the table with its assigned value.
*
* Methods:
*   void insertInputFiles(QList<QMap<QString, QString> > inputs);
*       Inserts a list of Programming Test input files into the database.
*   QList<QMap<QString, QString> > getInputFiles(QString testID);
*       Returns a list of all programming test input files belonging to the specified ProgrammingTest that exist in the database
*   bool deleteProgTestInputs(QString testID);
*       Deletes all programming test input files associated with the ProgrammingTest identified by testID from the database
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
class ProgTestInput : public Model {
public:
    ProgTestInput() : Model("ProgrammingTestInputs", "testFileID") {
        params = getEmptyRecord();
    }

    void insertInputFiles(QList<QMap<QString, QString> > inputs);
    QList<QMap<QString, QString> > getInputFiles(QString testID);
    bool deleteProgTestInputs(QString testID);

private:
    QMap<QString, QString> params;
};

#endif // PROGTESTINPUT_MODEL_H

