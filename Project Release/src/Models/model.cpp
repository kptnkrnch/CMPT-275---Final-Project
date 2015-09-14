#include "model.h"

extern DBAccess DBA;

Model::Model(QString tableName, QString pKey) throw(ParameterException) {

    this->tableName = tableName;
    this->pKey = QString();   // initialize to null
    pKeyIndex = -1;    // if -1 as index then it didnt find the key

    if(!DBA.isConnected()) {
        DBA.connect();
    }

    // check if the "key" is an actual parameter in this table
    parameters = requestParameters();
    int max = parameters.count();
    for(int i = 0; i < max; i++) {
        if(pKey == parameters.at(i)) {
            this->pKey = pKey;
            pKeyIndex = i;
        }
    }

    if(pKeyIndex == -1) {
        throw ParameterException(pKey.toStdString() + " not found in " + tableName.toStdString());
    }
}


/**
 * Method: getEmptyRecord()
 *
 * Description:
 *		Creates a map where each key represents a column of the table for this model
 *
 * Returns:
 * 		QMap<QString, QString> - map with keys representing columns and empty values
 */
QMap<QString, QString> Model::getEmptyRecord() {
    QMap<QString, QString> out;

    foreach(QString p, parameters) {
        out.insert(p, "");
    }
    return out;
}

/**
 * Method: parseResult(QSqlQuery *query)
 *
 * Description:
 *		Extracts the values returned by a SQL query statement execution
 *
 * Attributes:
 *		QSqlQuery *query  - the result a sql query execution
 *
 * Returns:
 *		QList<QMap<QString, QString> > - a list of maps, where each map represents one row and each key in the map represents a column for the table of this model
 *
*/
QList<QMap<QString, QString> > Model::parseResult(QSqlQuery *query) {
    QList<QMap<QString, QString> > out;
    while(query->next()) {
        QMap<QString, QString> temp = getEmptyRecord();
        foreach(QString p, parameters) {
            temp[p] = query->value(p).toString().trimmed();
        }
        out.append(temp);
    }
    return out;
}

/**
 * Returns maps, where each key of the map represents a column of the table that the model represents.
 * The maps will all contain a common key, value pair where the key is the passed in as parameter,
 * and value passed in as value.
*/
QList<QMap<QString, QString> > Model::getDBItems(QString parameter, QString value) {
    return getDBItems(parameter, QStringList(value));
}

/**
 * Returns maps, where each key of the map represents a column of the table that the model represents.
 * The maps will all contain the key value pair of the passed in parameter, with any of the passed in values.
*/
QList<QMap<QString, QString> > Model::getDBItems(QString parameter, QStringList values) {
    QList<QMap<QString, QString> > out;
    QSqlQuery *query;
    try {
        query = DBA._fromWhere(tableName, parameter, values);
    } catch(QueryException&) {
        return out;
    }
    return parseResult(query);
}

/**
 * Method: getAll()
 *
 * Description:
 *		Gets all rows of the representative table
 *
 * Returns:
 *	QList<QMap<QString, QString> > - a list of maps representing each row, each key of the map represents a column
 * 
 * Author: German Villarreal
 *
 * Version: 1
*/
QList<QMap<QString, QString> > Model::getAll() {
    QList<QMap<QString, QString> > out;
    QSqlQuery *query;
    try {
        query = DBA._getAll(tableName, pKey);
    } catch(QueryException&) {
        return out;
    }

    return parseResult(query);
}

/**
 * Method: getDBItems(QMap<QString, QString> keyValues, QString andor)
 *
 * Description:
 *		Gets all items after filtering for the specified values
 *
 * Attributes:
 *		QMap<QString, QString> keyValues -  each key represents a column, each value represents the filter
 *		QString andor -  the filter operator to use within the key value pairs, AND or OR
 *
 * Returns:
 *	QList<QMap<QString, QString> > - a list of maps representing each row, each key of the map represents a column
 *
 * Author: German Villarreal
 *
 * Version: 1
*/
QList<QMap<QString, QString> > Model::getDBItems(QMap<QString, QString> keyValues, QString andor) {
    QSqlQuery *query = NULL;
    QList<QMap<QString, QString> > out;
    try {
        query = DBA._fromWhere(tableName, keyValues, andor);
    } catch(QueryException&) {
        return out;
    }

    return parseResult(query);
}

/**
 * Method: getDBItem(QString getKey)
 * 
 * Description:
 * 		Gets a specific item from this table
 * 
 * Attributes:
 * 		QString getKey - the value of a primary key to filter
 * 
 * Returns:
 * 		QMap<QString, QString> - a map of the item, each key of the map refers to a column of the table
 * 
 * Throws:
 * 	ParameterException - the query failed to execute, or the value was not found in the table
 * 
 * Author: German Villarreal
 * 
 * Version: 1
*/
QMap<QString, QString> Model::getDBItem(QString getKey) throw(ParameterException){
    QSqlQuery *query;
    QMap<QString, QString> out;

    out = getEmptyRecord();

    try {
        query = DBA._get(tableName, pKey, getKey);
    } catch(QueryException&) {
        // Should throw a QueryException, but that would mean
        // every submodel would need to try to catch to exceptions
        // anyone's welcome to make the change!
        throw ParameterException("Could not execute query");
    }

    if(query->numRowsAffected() == 0) {
        std::string error = "Could not find a " + pKey.toStdString() + " with " + getKey.toStdString();
        throw ParameterException(error);
    }

    out = parseResult(query).first();

    // if it didn't insert anything into THIS mandatory field, then it didnt find it
    if(out[pKey].isEmpty()) {
        std::string error = "Could not find " + getKey.toStdString() + " as " + pKey.toStdString();
        throw ParameterException(error);
    }

    return out;
}

/**
* Method: deleteDBItem(QMap<QString, QString> row)
*
* Description:
* 		Delete a specific item from this table
*
* Attributes:
* 		QMap<QString, QString> row - a map of the item to delete, each key of the map refers to a column of the table
*
* Returns:
* 		bool - true if successfully deleted
*
*
* Author: German Villarreal
*
* Version: 1
*/
bool Model:: deleteDBItem(QMap<QString, QString> row) {
    QList<QMap<QString, QString> > delList;
    delList.append(row);
    return deleteDBItems(delList);
}

/**
* Method: deleteDBItems(QList<QMap<QString, QString> > rows)
*
* Description:
* 		Delete specific items from this table
*
* Attributes:
* 		QList<QMap<QString, QString> > rows - a list of maps of the items to delete, each key of the map refers to a column of the table
*
* Returns:
* 		bool - true if successfully deleted
*
*
* Author: German Villarreal
*
* Version: 1
*/
bool Model::deleteDBItems(QList<QMap<QString, QString> > rows) {
    if(rows.isEmpty()) {
        return true;
    }

    try {
        DBA._deleteBatch(tableName, rows);
    } catch(QueryException&) {
        return false;
    }
    return true;
}

bool Model::deleteDBItems(QString parameter, QString value) {

    return DBA._delete(tableName, parameter, value);
}

/**
 * Method: deleteDBItem(QString delKey) 
 *
 * Description:
 * 		Gets a specific item from this table
 *
 * Attributes:
 * 		QString delKey - the value of a primary key to filter
 *
 * Returns:
 * 		bool - true if successfully deleted
 *
 * Throws:
 * 	ParameterException - the query failed to execute, or the value was not found in the table
 *
 * Author: German Villarreal
 *
 * Version: 1
*/
bool Model::deleteDBItem(QString delKey) throw(ParameterException){

    // might not be needed anymore..
    if(!pKeyExists(delKey)) {
        // parameter not found exception
        throw ParameterException(delKey.toStdString() + " does not exist in " + tableName.toStdString());
    }

    return DBA._delete(tableName, pKey, delKey);
}


/**
 * Method: deleteDBItem(QString delKey)
 *
 * Description:
 * 		Gets a specific item from this table
 *
 * Attributes:
 * 		QString delKey - the value of a primary key to filter
 *
 * Returns:
 * 		bool - true if successfully deleted
 *
 * Throws:
 * 	ParameterException - the query failed to execute, or the value was not found in the table
 *
 * Author: German Villarreal
 *
 * Version: 1
*/
bool Model::deleteDBItems(QString parameter, QStringList values) {

    QList<QMap<QString, QString> > delRows;
    foreach(QString value, values){
        QMap<QString, QString> delRow;
        delRow[parameter] = value;
        delRows.append(delRow);

    }
    return DBA._deleteBatch(tableName,delRows);
}

/**
 * Method: pKeyExists(QString pKey)
 *
 * Description:
 * 		Verifies the value exists in the table, should only be used when the full object is not needed.
 *		See also getDBItem
 *
 * Attributes:
 * 		QString pKey - the value of a primary key to verify
 *
 * Returns:
 * 		bool - true if the value was found
 *
 * Author: German Villarreal
 *
 * Version: 1
*/
bool Model::pKeyExists(QString pKey) {
    if(pKey.isEmpty()) {
        // probably misleadig
        // caller may not have known it was empty
        return false;
    }
    try {
        getDBItem(pKey);
    } catch (ParameterException& e) {
        return false;
    }
    return true;
}

/**
 * Method: insertDBItem(QMap<QString, QString> item)
 *
 * Description:
 *		Inserts a specified item into the table
 *
 * Attributes:
 * 		QMap<QString, QString> item - each key of the map represents a column of the table with its specified value
 *
 * Returns:
 * 		bool - true if the row was inserted
 *
 * Throws: 
 *		ParameterException - if the keys of the map do not match the columns of the table
 *
 * Author: German Villarreal
 *
 * Version: 1
*/
bool Model::insertDBItem(QMap<QString, QString> item) throw (ParameterException){

    if(pKeyExists(item[pKey])) {
        // parameter exists exception
        return false;
    }

    try {
        isRowComplete(item);
    } catch (ParameterException& e) {
        throw e;
    }

    return DBA._insert(tableName, item);
}

/**
* Method: insertDBItems(QList<QMap<QString, QString> > items)
*
* Description:
*		Inserts a specified item into the table
*
* Attributes:
* 		QList<QMap<QString, QString> > items - a list of maps, each key of the map represents a column of the table with its specified value
*
* Returns:
* 		bool - true if the rows were inserted
*
* Author: German Villarreal
*
* Version: 1
*/
bool Model::insertDBItems(QList<QMap<QString, QString> > items) {
    try {
        DBA._insertBatch(tableName, items);
    } catch (QueryException&) {
        return false;
    }
    return true;
}

/**
 * Method: insertDBItems(QList<QMap<QString, QString> > items)
 *
 * Description:
 *		Inserts a specified item into the table. It is possible to retrieve the newly inserted item. Useful for inserting into autoincrement key tables
 *
 * Attributes:
 * 		QMap<QString, QString> item - a map where each key of the map represents a column of the table with its specified value
 *
 * Returns:
 * 		QMap<QString, QString> - the newly inserted item into the table, where each key of the map represents a column of the table
 *
 * Author: Joshua Campbell
 *
 * Version: 1
*/
QMap<QString, QString> Model::insertDBItem(QMap<QString, QString> item, bool returnInsertedObject) throw (ParameterException){
    QSqlQuery *query;
    QMap<QString, QString> out;
    try {
        //isRowComplete(item);
    } catch (ParameterException& e) {
        DEBUG(DEBUG_INFO, e.what());
        throw e;
    }

    out = getEmptyRecord();
    try {
        query = DBA._insert(tableName, item, returnInsertedObject);
    } catch(QueryException&) {
        return out;
    }

    if(query->numRowsAffected() == 0) {
        std::string error = "";//"Could not find a " + pKey.toStdString() + " with " + getKey.toStdString();
        throw ParameterException(error);
    }

    query->next();
    foreach(QString p, parameters) {
        out[p] = query->value(p).toString().trimmed();
    }

    // if it didn't insert anything into THIS mandatory field, then it didnt find it
    if(out[pKey] == "") {
        std::string error = "";//"Could not find " + getKey.toStdString() + " as " + pKey.toStdString();
        throw ParameterException(error);
    }
    return out;
}

/**
* Method: isRowFilledIn(QSqlField field, QString value)
*
* Description:
*		Checks if the required fields of this table are provided in the 
*
* Attributes:
* 		QMap<QString, QString> item - a map where each key of the map represents a column of the table with its specified value
*
* Returns:
* 		QMap<QString, QString> - the newly inserted item into the table, where each key of the map represents a column of the table
*
* Author: Joshua Campbell
*
* Version: 1
*/
bool Model::isRowFilledIn(QSqlField field, QString value) {

    // if its required
    if(field.requiredStatus() == QSqlField::Required) {
        // and not filled in
        if(value.size() == 0) {
            // parameter length exception
            return false;
        }

        // and entered length is longer
        if(field.length() < value.size()) {
            // parameter length exception
            return false;
        }
    }
    return true;
}

bool Model::isRowComplete(QMap<QString, QString> row) throw (ParameterException){

    // Right AMOUNT of parameters?
    if(row.size() != Model::getNumberOfParameters()) {
        throw ParameterException("New entry does not have the right amount of parameters.");
    }

    // Does it have the Primary Key parameter?
    if(!row.contains(pKey)) {
        std::string error = "Must include the primary key parameter: " + pKey.toStdString();
        throw ParameterException(error);
    }

    // Does it have all of the right parameters?
    foreach(QString parameter, parameters) {
        if(!row.contains(parameter)) {
            std::string error = "Cannot find parameter:" + parameter.toStdString() + " in the new entry";
            throw ParameterException(error);
        }
    }

    // Are the required ones filled in?
    QSqlRecord rec = DBA.getRecord(tableName);
    foreach(QString parameter, parameters) {
        if(!isRowFilledIn(DBA.getRecord(tableName).field(parameter), row[parameter])) {
            std::string error = parameter.toStdString() + " not the right format";
            throw ParameterException(error);
        }
    }

    return true;
}

bool Model::updateItemKey(QString oldVal, QString newVal) throw (ParameterException) {
    QMap<QString, QString> oldItem;
    QMap<QString, QString> newItem;

    try {
        oldItem = getDBItem(oldVal);
    } catch (ParameterException& e) {
            throw e;
    }

    newItem = oldItem;
    newItem[getKey()] = newVal;

    try {
        DBA._update(tableName, oldItem, newItem);
    } catch (QueryException&) {
        return false;
    }
    return true;
}

bool Model::updateDBItem(QMap<QString, QString> currentRow, QMap<QString, QString> newRow) {
    try {
        DBA._update(tableName, currentRow, newRow);
    } catch(QueryException&) {
        return false;
    }
    return true;
}

bool Model::updateDBItems(QList<QMap<QString, QString> > currValues, QMap<QString, QString> newValues) {
    if(currValues.isEmpty()) {
        return true;
    }
    return DBA._updateBatch(tableName, currValues, newValues);
}

/**
 * Method: updateDBItems(QString parameter, QString currValue, QString newValue)
 *
 * Description:
 * 		Updates the value of a certain column from one value to another
 *
 * Returns:
 *	 QString parameter - the column name of the value to change	 
 *	 QString currValue - the value to change from
 *	 QString newValue - the value to change to
 *
 * Author: German Villarreal
 *
 * Version: 1
*/
bool Model::updateDBItems(QString parameter, QString currValue, QString newValue) {
    return DBA._updateBatch(tableName, parameter, currValue, newValue);
}

/**
 * Method: requestParameters()
 * Description:
 * 		Asks the database for the column names of this table
 *
 * Returns:
 *	 QStringList - the column names of this table
 *
 * Author: German Villarreal
 *
 * Version: 1
*/
QStringList Model::requestParameters() {
    QStringList out;
    QSqlRecord rec = DBA.getRecord(tableName);

    //DEBUG(DEBUG_INFO, "Table: " << tableName);
    int count = rec.count();
    for(int i =0; i< count; i++) {
        //DEBUG(DEBUG_INFO, "Field(" << i << "): " << rec.fieldName(i));
        out.append(rec.fieldName(i));
    }

    return (parameters = out);
}

/**
* Method: getParameter(QString parameter, QMap<QString, QString> const * const object)
* Description:
* 		Returns the value found in the map's paramter key
*
* Attributes:
* 		QString parameter - the key of the value to retrieve
* 		QMap<QString, QString> *object - the object containing a key of parameter
*
* Returns:
*		QString - true if the parameter was changed,
*
* Author: German Villarreal
*
* Version: 1
*/
QString Model::getParameter(QString parameter, QMap<QString, QString> const * const object) {
    if(object == NULL) {
        return QString();
    }
    return object->value(parameter);
}

/**
 * Method: setParameter(QString parameter, QString newValue, QMap<QString, QString> *object)
 * Description:
 * 		Inserts a new key/value pair into the map
 * 
 * Attributes: 
 * 		QString parameter - the parameter to alter
 * 		QString newValue - the new value to change to
 * 		QMap<QString, QString> *object - the object to set the new parameter of
 * 
 * Returns:
 *		bool - true if the parameter was changed,
 * 			 - false if the pointer was NULL
 *
 * Author: German Villarreal
 * 
 * Version: 1
*/
bool Model::setParameter(QString parameter, QString newValue, QMap<QString, QString> *object) {
    if(object == NULL) {
        return false;
    }
    object->insert(parameter, newValue);
    return true;
}
