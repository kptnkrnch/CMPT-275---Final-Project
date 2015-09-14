#ifndef MODEL_H
#define MODEL_H

#include "dbaccess.h"
#include "../Helpers/debug.h"
#include "../Exceptions/ParameterException.h"

#include <QStringList>
#include <QSqlRecord>
#include <QMap>
#include <QList>

extern DBAccess DBA;

/**
* ClassName: Model
*
* Description: Base model to abstract database query calls. Each derived model must implement their own methods
*			   to access the databse to provide additional checks and model specific logic while performing the
*			   database operations. A map, specifically QMap, is used to represent an object of the database.
*			   The key of this map represents the column in the datbase with its assigned value. 
*
* Methods:
*   Model(QString tableName, QString pKey) throw(ParameterException);
*		Constructor for the model looks for all other columns of this table, and checks if the table/key exist in the databse.
*		Parameter exception is thrown if the primary key cannot be found in the table.
*
*	QList<QMap<QString, QString> > getDBItems(QMap<QString, QString> keyValues, QString andor);
*		Most common use is to retrieve a single row with a compound primary key.  keyValues would contain a key for each table key
*		and use the AND operator to find the specific row from the database.
*	QList<QMap<QString, QString> > getDBItems(QString parameter, QStringList values);
*		Translates to: SELECT * FROM __
*           WHERE parameter = values1 OR parameter = values2 ..
*	QList<QMap<QString, QString> > getDBItems(QString parameter, QString value);
* *		Translates to: SELECT * FROM __ WHERE parameter = value
*	QMap<QString, QString> getDBItem(QString getKey) throw(ParameterException);
*		Retrieves the row with specified primary key
*
*   bool insertDBItems(QList<QMap<QString, QString> > items);
*   bool insertDBItem(QMap<QString, QString> item) throw(ParameterException);
*   QMap<QString, QString> insertDBItem(QMap<QString, QString> item, bool returnInsertedObject) throw (ParameterException);
*		Inserts the provided key value pair(s) into their table.
*		Translates into INERT INTO VALUES ( )
*
*   bool updateItemKey(QString oldVal, QString newVal) throw (ParameterException);
*   bool updateDBItem(QMap<QString, QString> pKeys, QMap<QString, QString> item);
*		Updates the exact values from pKeys (as key-value pairs representing each column for one specific row) to the new values
*		passed in as item. If multiple entries match those from pKeys, they will be updated as well.
*   bool updateDBItems(QList<QMap<QString, QString> > currValues, QMap<QString, QString> newValues);
*		Similar to updateDBItem but this will update a larger set of values passed in from currValues to a single newValues.
*		This is useful for updating several different rows with the same values into a single new value for each column.
*		Allows to match any number of columns(keys from currValues), but update a different set of columns(keys from newValues).
*   bool updateDBItems(QString parameter, QString currValue, QString newValue);
*		Updates all entries under the column 'parameter' with the specified 'currValue', to its 'newValue'.
*   
*   bool deleteDBItem(QString delKey) throw(ParameterException);
*   bool deleteDBItem(QMap<QString, QString> item);
*		Deletes a very specific row by matching all key value pairs to those found stored in the database.
*		Translates into DELETE __ FROM __ WHERE
*           (map1key1 = map1value1 AND map1key2 = map1value2 ... )
*   bool deleteDBItems(QList<QMap<QString, QString> > rows);
*		Deletes a very specific set of rows matching all key value pairs for the entire list of maps.
*		Translates into DELETE __ FROM __ WHERE
*           (map1key1 = map1value1 AND map1key2 = map1value2 ... ) OR (map2key1 = map2value1 AND map2key2 = map2value2 ... ) ...
*   bool deleteDBItems(QString parameter, QString value);
*		Deletes all rows that have specified 'value' under colum 'parameter'.
*   bool deleteDBItems(QString parameter, QStringList values);
*		Removes specific values from their table
*		Translates into DELETE FROM WHERE parameter = value1 OR parameter = value2 ...
*   
*   QStringList requestParameters();
*		Looks up the column names for the table
*
*   bool isRowComplete(QMap<QString, QString> row) throw(ParameterException);
*		Checks if the map contains a key for every column of this table
*
*   bool isRowFilledIn(QSqlField field, QString value);
*		Checks if the map contains the neccessary values for inserting a row into this table
*
* Members:
*	QString pKey;
*		Primary key for the model object
*	QString tableName;
*		Table name that this model will represent form the database
*	QStringList parameters;
*		All columns of this table
*
*
* Author: German Villarreal
*
* Version: 1
*/
class Model {
public:

    Model() {}
    Model(QString tableName, QString pKey) throw(ParameterException);
    ~Model() {}

    QList<QMap<QString, QString> > getAll();
    QMap<QString, QString> getEmptyRecord();

    bool insertDBItem(QMap<QString, QString> item) throw(ParameterException);
    QMap<QString, QString> insertDBItem(QMap<QString, QString> item, bool returnInsertedObject) throw (ParameterException);
    QList<QMap<QString, QString> > parseResult(QSqlQuery *query);

    // Accessors
    QString getTableName() const { return tableName; }
    QString getKey() const { return this->pKey; }
    QStringList getParameters() const { return parameters; }
    int getNumberOfParameters() const { return parameters.count(); }
    bool pKeyExists(QString pKey);

protected:

    // SELECT
    QList<QMap<QString, QString> > getDBItems(QMap<QString, QString> keyValues, QString andor);
    QList<QMap<QString, QString> > getDBItems(QString parameter, QStringList values);
    QList<QMap<QString, QString> > getDBItems(QString parameter, QString value);
    QMap<QString, QString> getDBItem(QString getKey) throw(ParameterException);

    // INSERT
    bool insertDBItems(QList<QMap<QString, QString> > items);

    // UPDATE
    bool updateItemKey(QString oldVal, QString newVal) throw (ParameterException);
    bool updateDBItem(QMap<QString, QString> pKeys, QMap<QString, QString> item);
    bool updateDBItems(QList<QMap<QString, QString> > currValues, QMap<QString, QString> newValues);
    bool updateDBItems(QString parameter, QString currValue, QString newValue);

    // DELETE
    bool deleteDBItem(QString delKey) throw(ParameterException);
    bool deleteDBItem(QMap<QString, QString> item);
    bool deleteDBItems(QList<QMap<QString, QString> > rows);
    bool deleteDBItems(QString parameter, QString value);
    bool deleteDBItems(QString parameter, QStringList values);

    static bool setParameter(QString parameter, QString newValue, QMap<QString, QString> *item);
    static QString getParameter(QString parameter, QMap<QString, QString> const * const object);

    QStringList requestParameters();   // Find from database
    bool isRowComplete(QMap<QString, QString> row) throw(ParameterException); // checks right parameters are present
    bool isRowFilledIn(QSqlField field, QString value);

    int pKeyIndex;
    QString pKey;
    QString tableName;
    QStringList parameters;

};

#endif // MODEL_H
