#include "dbaccess.h"

DBAccess DBA;

/**
* Method: DBAccess()
*
* Description:
*		Basic constructor of a database connection handler. Prepares all neccessary parameters for a
*		connection to a MS SQL Server database with specified parameters.
*
* Author: Joshua Campbell
*
* Version: 1
*/
DBAccess::DBAccess() {

    //Your info!
    hostname = "cypress.csil.sfu.ca";
    dbname = "c275g09A";
    username = "c275g09";                      // SQL Server; inside your database, in the dbo.helpdesk table
    password = "gr64g7EeGfANdfL7";             // SQL Server; inside your database, in the dbo.helpdesk table
    // No more edits needed


#ifdef JOSH_SERVER
    DEBUG(DEBUG_ALRM, "JOSH_SERVER defined somewhere!");
    hostname = "96.55.60.146,4224";
#endif
    connected = false;
    db = QSqlDatabase::addDatabase("QODBC");

    //SQL Server connection string
    connection_string = QString("Driver={SQL SERVER};Server=%1;Database=%2;User id=%3;password=%4;").arg(hostname).arg(dbname).arg(username).arg(password);
}

/**
* Method: ~DBAccess()
*
* Description:
*		Basic destructor of a database connection handler. Closes the connection if still open.
*
* Author: Joshua Campbell
*
* Version: 1
*/
DBAccess::~DBAccess() {
    disconnect();
}

/**
 * Method: tableExists(QString table)
 *
 * Description:
 *		Checks if a table with specified name exists in the database
 *
 * Attributes:
 *		QString table - the name of the table to check
 *
 * Returns:
 *		bool - true if it exists, false otherwise
 *
 * Author: German Villarreal
 *
 * Version: 1
*/
bool DBAccess::tableExists(QString table) {
    // if not connected, then tables aren't initialized
    if(!connected) {
        return false;
    }

    foreach(QString t, tables) {
        if(t == table) {
            return true;
        }
    }
    return false;
}

/**
* Method: connect()
*
* Description:
*		Creates a connection to the database
*
* Returns:
*		bool - true if connection was established
*
* Author: Joshua Campbell
*
* Version: 1
*/
bool DBAccess::connect() {
    db.setDatabaseName(connection_string);
#ifdef JOSH_SERVER
    db.setUserName("c275g09");
    db.setPassword("gr64g7EeGfANdfL7");
#endif

    if(connected) {
        return true;
    }

    if (db.open()) {
        DEBUG(DEBUG_INFO, "Connection opened.");
        connected = true;
        tables = getTables();
        return true;
    } else {
        //DEBUG(DEBUG_ALRM, dbaccess::db.lastError().text());
        return false;
    }
}

/**
* Method: disconnect()
*
* Description:
*		Destroys a connection to the database
*
* Returns:
*		bool - true if connection was closed
*
* Author: Joshua Campbell
*
* Version: 1
*/
bool DBAccess::disconnect() {
    db.close();
    return !(db.isOpen());
}


/**
* Method: disconnect()
*
* Description:
*		Returns connection status
*
* Returns:
*		bool - true if connection is open
*
* Author: Joshua Campbell
*
* Version: 1
*/
bool DBAccess::isConnected() {
    return db.isOpen();
}

/**
* Method: __query(QString sql)
*
* Description:
*		Executes the specified query on the database
*
* Attributes:
*		QString sql - the query to be executed
*
* Returns:
*		QSqlQuery* - the result of the executed query
*
* Throws:
*	QueryException - if the query was not successfully executed on the database.
*
* Author: German Villarreal
*
* Version: 1
*/
QSqlQuery *DBAccess::__query(QString sql) throw (QueryException) {

    QSqlQuery *query = new QSqlQuery();
    query->exec(sql);
    if(query->lastError().isValid()
            || db.lastError().isValid()) {
        DEBUG(DEBUG_INFO, "Attempted: " << sql);
        DEBUG(DEBUG_WARN, "Query: " << query->lastError().text());
        DEBUG(DEBUG_WARN, "DB: " << db.lastError().text());
        throw QueryException("Query failed to execute on the database");
    }
    DEBUG(DEBUG_INFO, "Excecuted: " << query->executedQuery()
          << "\n\t" << query->numRowsAffected() << "row(s) affected.");

    return query;
}

/**
* Method: _insert(QString tableName, QMap<QString, QString> values)
*
* Description:
*		Inserts the specified values into the specified table
*
* Attributes:
*		QString table - the database table to insert values into
*		QMap<QString, QString> values - the values to insert
*			(where each key of the map is one column of the table)
*
* Returns:
*		bool - true if successfully inserted
*
* Author: German Villarreal
*
* Version: 1
*/
bool DBAccess::_insert(QString tableName, QMap<QString, QString> values) {
    QString sqlStr;
    QString parStr;
    QString valStr;
    QMapIterator<QString, QString> iter(values);

    parStr = " ( ";
    valStr = " ( ";

    while(iter.hasNext()) {
        iter.next();
        parStr += iter.key();
        valStr += "'" + iter.value() + "'";
        if(iter.hasNext()) {
            parStr += ", ";
            valStr += ", ";
        }
    }
    parStr += " ) ";
    valStr += " ) ";

    sqlStr = "INSERT INTO " + tableName + parStr + " VALUES " + valStr + ";";

    try {
        __query(sqlStr);
    } catch (QueryException& e) {
        return false;
    }
    return true;
}

/**
* Method: _insert(QString tableName, QMap<QString, QString> values)
*
* Description:
*		Inserts the specified values into the specified table, optionally 
*		can return the newly isnerted object. Often used for auto increment key tables.
*
* Attributes:
*		QString table - the database table to insert values into
*		QMap<QString, QString> values - the values to insert
*			(where each key of the map is one column of the table)
*		bool returnInsertedObject - if true
*
* Returns:
*		QSqlQuery* - the result of the database query
*
* Author: Joshua Campbell
*
* Version: 1
*/
QSqlQuery* DBAccess::_insert(QString tableName, QMap<QString, QString> values, bool returnInsertedObject)  throw (QueryException) {
    // TO-DO: Check table name exists

    QString sqlStr;
    QString parStr;
    QString valStr;
    QMapIterator<QString, QString> iter(values);

    parStr = " ( ";
    valStr = " ( ";

    while(iter.hasNext()) {
        iter.next();
        parStr += iter.key();
        valStr += "'" + iter.value() + "'";
        if(iter.hasNext()) {
            parStr += ", ";
            valStr += ", ";
        }
    }
    parStr += " ) ";
    valStr += " ) ";
    if (returnInsertedObject) {
        sqlStr = "INSERT INTO " + tableName + parStr + " OUTPUT inserted.* VALUES " + valStr + ";";
    } else {
        sqlStr = "INSERT INTO " + tableName + parStr + " VALUES " + valStr + ";";
    }
    DEBUG(DEBUG_INFO, sqlStr.toStdString().c_str());
    try {
        return __query(sqlStr);
    } catch(QueryException& e) {
        throw e;
    }
}

/**
* Method: _insertBatch(QString tableName, QList<QMap<QString, QString> > rows) throw (QueryException)
*
* Description:
*		Inserts the specified list of values into the specified table.
*
* Attributes:
*		QString tableName - the database table to insert values into
*		 QList<QMap<QString, QString> > rows - the values to insert
*			(where each key of the map is one column of the table)
*
* Returns:
*		QSqlQuery* - the result of the database query
*
* Throws:
*		QueryException - if the database query failed toexecute
*
* Author: German Villarreal
*
* Version: 1
*/
QSqlQuery* DBAccess::_insertBatch(QString tableName, QList<QMap<QString, QString> > rows) throw (QueryException) {

    QString sqlStr;
    QString valStr = QString();
    QString parStr = QString();
    QMap<QString, QString> tmp = rows.at(0);

    // For each parameter..
    parStr += " (";
    QMapIterator<QString, QString> kter(tmp);
    while(kter.hasNext()) {
        kter.next();
        parStr += kter.key();
        if(kter.hasNext()) {
            parStr += ", ";
        }
    }
    parStr += ") ";

    // For each row..
    QListIterator<QMap<QString, QString> > iter(rows);
    while (iter.hasNext()) {

        // For each value..
        valStr += " (";
        QMapIterator<QString, QString> jter(iter.next());
        while(jter.hasNext()) {
            jter.next();
            valStr += "'" + jter.value() + "'";
            if(jter.hasNext()) {
                valStr += ", ";
            }
        }
        valStr += ")";

        if(iter.hasNext()) {
            valStr += ", ";
        }
    }

    sqlStr = "INSERT INTO " + tableName + parStr + " VALUES " + valStr + ";";

    try {
        return __query(sqlStr);
    } catch(QueryException& e) {
        throw e;
    }

}

/**
 * Method: _get(QString tableName, QString pKey, QString which) throw (QueryException);
 * 
 * Description:
 *		Gets items from specified table that match a unique value for a parameter.
 * 
 * Attributes:
 *		QString tableName - reference table
 *		QString pKey - table column with unique attributes
 *		QString which - specific attribute filter
 * 
 * Returns:
 *		QSqlQuery* - the result of the database query
 * 
 * Throws:
 *		QueryException - if query failed to execute on the database
 * 
 * Author: German Villarreal
 * 
 * Version: 1
*/
QSqlQuery* DBAccess::_get(QString tableName, QString pKey, QString which) throw (QueryException) {
    QString sqlStr;

    sqlStr = "SELECT * FROM " + tableName + " WHERE " + pKey + " = '" + which + "';";
     try {
        return __query(sqlStr);
    } catch(QueryException& e) {
        throw e;
    }
}

/**
 * Method: _getAll(QString tableName, QString pKey)
 * 
 * Description:
 * 	Returns all rows of the specified table.
 * 
 * Attributes:
 * 		QString tableName - reference table
 *		QString pKey - a non null column
 *
 * Returns:
 *		QSqlQuery* - the result of the database query
 * 
 * Throws:
 *		QueryException - if query failed to execute on the database
 *
 * Author: German Villarreal
 * 
 * Version: 1
*/
QSqlQuery* DBAccess::_getAll(QString tableName, QString pKey) throw (QueryException) {
    QString sqlStr;


    sqlStr = "SELECT * FROM " + tableName + " WHERE " + pKey + " IS NOT NULL;";
    try {
        return __query(sqlStr);
    } catch(QueryException& e) {
        throw e;
    }
}

/**
 * Method: _fromWhere(QString tableName, QString parameter, QStringList values)
 * 
 * Description:
 *		Executes SELECT FROM WHERE X=Y OR X=Z on the database.
 * 
 * Attributes:
 * 		QString tableName - reference table
 * 		QString parameter - the column name to filter
 * 		QStringList values - the values to filter
 * 
 * Returns:
 *		QSqlQuery* - the result of the database query
 * 
 * Throws:
 *		QueryException - if query failed to execute on the database
 * 
 * Author: German Villarreal
 * 
 * Version: 1
*/
QSqlQuery* DBAccess::_fromWhere(QString tableName, QString parameter, QStringList values) throw (QueryException) {
    QString sqlStr;

    sqlStr = "SELECT * FROM " + tableName + " WHERE ";
    QStringListIterator iter(values);
    while(iter.hasNext()) {
        QString temp = iter.next();

        sqlStr += parameter + " = '" + temp + "' ";
        if(iter.hasNext()) {
            sqlStr += " OR ";
        }
    }
    sqlStr += ";";
    try {
        return __query(sqlStr);
    } catch(QueryException& e) {
        throw e;
    }
}

/**
 * Method: _fromWhere(QString tableName, QMap<QString, QString> keyValues, QString andor)
 * 
 * Description:
 *		Executes SELECT FROM WHERE X=Y andor Z=A
 * 
 * Attributes:
 * 		QString tableName - reference table
 * 		QMap<QString, QString> keyValues - the columns and values to filter
 * 		QString andor - the operator to use between values, "AND" or "OR" are allowed types
 * 
 * Returns:
 * 		QSqlQuery* - the result of the database query
 * 
 * Throws:
 *		QueryException - if the query failed to execute on the database
 * 
 * Author: German Villarreal
 * 
 * Version: 1
*/
QSqlQuery* DBAccess::_fromWhere(QString tableName, QMap<QString, QString> keyValues, QString andor) throw (QueryException) {
    QString sqlStr;

    sqlStr = "SELECT * FROM " + tableName + " WHERE ";
    QMapIterator<QString, QString> iter(keyValues);
    while(iter.hasNext()) { iter.next();
        sqlStr += iter.key() + " = '" + iter.value() + "'";
        if(iter.hasNext()) {
            sqlStr += " " + andor + " ";
        }
    }
    sqlStr += ";";

    try {
        return __query(sqlStr);
    } catch(QueryException& e) {
        throw e;
    }
}

/**
 * Method: _update(QString tableName, QMap<QString, QString> currentValues, QMap<QString, QString> newValues)
 * 
 * Description:
 * 	Updates values of a table to new values
 * 
 * Attributes:
 * 		QString tableName - reference table
 * 		QMap<QString, QString> currentValues - the columns(map keys) and values(map values) to filter
 * 		QMap<QString, QString> newValues - the columns(map keys) and values(map values) to change to
 * 
 * Returns:
 *		QSqlQuery* - the result of the database query
 *
 * Throws:
 * 		QueryException - if the query failed to execute on the database
 *
 *  Author: German Villarreal
 * 
 * Version: 1
*/
QSqlQuery* DBAccess::_update(QString tableName, QMap<QString, QString> currentValues, QMap<QString, QString> newValues) throw (QueryException) {
    QString sqlStr;

    sqlStr = "UPDATE " + tableName + " SET ";

    // Set every attribute to specified value
    QMapIterator<QString, QString> value_iterator(newValues);
    while(value_iterator.hasNext()) {
        value_iterator.next();
        sqlStr += value_iterator.key() + "="
                + ((value_iterator.value().isEmpty()) ? "\'\'" : "'" + value_iterator.value() + "'");
                //+ ((value_iterator.value().isEmpty()) ? "NULL" : "'" + value_iterator.value() + "'");
        if (value_iterator.hasNext()) {
            sqlStr += ", ";
        }
    }
    sqlStr += " WHERE ";

    // From this very specific row
    QMapIterator<QString, QString> pkey_iterator(currentValues);
    while(pkey_iterator.hasNext()) {
        pkey_iterator.next();
        sqlStr += pkey_iterator.key() + "="
                + "'" + pkey_iterator.value() + "'";
        if (pkey_iterator.hasNext()) {
            sqlStr += " AND ";
        }
    }
    sqlStr += ";";
    try {
        return __query(sqlStr);
    } catch(QueryException& e) {
        throw e;
    }
}

/**
 * Method: _updateBatch(QString tableName, QString parameter, QString currVal, QString newVal)
 * 
 * Description:
 *		Updates all values of a specific column and value to a new value
 *
 * Attributes:
 *		QString tableName - reference table
 *		QString parameter - the column name that the updated value will be found in
 *		QString currVal - the current value to filter
 *		QString newVal - the value to change to
 *
 * Returns:
 *		bool - true on successful update
 *
 * Author: German Villarreal
 *
 * Version: 1
*/
bool DBAccess::_updateBatch(QString tableName, QString parameter, QString currVal, QString newVal) {
    QString sqlStr;

    sqlStr = "UPDATE " + tableName + " SET " + parameter + "=";
//    if(newVal.isEmpty()) {
//        sqlStr += "NULL";
//    } else {
        sqlStr += "'" + newVal + "'";
//    }
    sqlStr += " WHERE " + parameter + " = '" + currVal + "'";

    try {
        __query(sqlStr);
    } catch(QueryException&) {
        return false;
    }
    return true;
}

/**
 * Method: _updateBatch(QString tableName, QList<QMap<QString, QString> > currentValues, QMap<QString, QString> newValues)
 * 	
 * Description:
 * 		Updates the a list of specified items to a set of values
 * 
 * Attributes:
 * 		QString tableName - reference table
*	 	QList<QMap<QString, QString> > currentValues - the values to filter(where each map key is a column name)
 * 		QMap<QString, QString> newValues - the values to change to(where each map key is a column name)
 * 
 * Returns:
 *		bool - true on successful update
 * 
 * Author: German Villarreal
 * 
 * Version: 1
*/
bool DBAccess::_updateBatch(QString tableName, QList<QMap<QString, QString> > currentValues, QMap<QString, QString> newValues) {
    QString sqlStr;
    int size;

    sqlStr = "UPDATE " + tableName + " SET ";

    // Set every attribute to specified value
    QMapIterator<QString, QString> value_iterator(newValues);
    while(value_iterator.hasNext()) {
        value_iterator.next();
        sqlStr += value_iterator.key() + "='" + value_iterator.value() + "'";
        if (value_iterator.hasNext()) {
            sqlStr += ", ";
        }
    }
    sqlStr += " WHERE ";

    // From these rows
    size = currentValues.size();
    for(int i = 0; i < size; i++) {
        QMap<QString, QString> iMap = currentValues.at(i);
        QMapIterator<QString, QString> par_iterator(iMap);
        sqlStr += " ( ";
        while(par_iterator.hasNext()) {
            par_iterator.next();
            sqlStr += par_iterator.key() + "="
                + "'" + par_iterator.value() + "'";
            if (par_iterator.hasNext()) {
                sqlStr += " AND ";
            }
        }
        sqlStr += " ) ";
        if(i != size-1) {
            sqlStr += " OR ";
        }
    }
    sqlStr += ";";

    try {
        __query(sqlStr);
    } catch(QueryException&) {
        return false;
    }
    return true;
}

/**
 * Method: _delete(QString tableName, QString pKey, QString which)
 * 
 * Description: 
 *		Deletes items with specified parameter and value
 * 
 * Attributes:
 *		QString tableName - reference table
 *		QString pKey - column name
 *		QString which - value to delete
 *
 * Returns: 
 *		bool - true on successful deletion
 * 
 * Author: German Villarreal
 * 
 * Version: 1
*/
bool DBAccess::_delete(QString tableName, QString pKey, QString which) {
    QString sqlStr;

    sqlStr = "DELETE FROM " + tableName + " WHERE " + pKey + " = '" + which + "';";

    try {
        __query(sqlStr);
    } catch(QueryException&) {
        return false;
    }
    return true;
}

/**
 * Method: _deleteBatch(QString tableName, QList<QMap<QString, QString> > rows)
 *
 * Description:
 *	 	Deletes specific rows from a table
 * 
 * Attributes:
 *		QString tableName - reference table
 *		QList<QMap<QString, QString> > rows - the columns(map keys) and values(map values) to delete
 *
 * Returns:
 *		QSqlQuery* - the result of the database query
 * 
 * Throws:
 * 		QueryException - if the query failed to execute on the database
 *
 * Author: German Villarreal
 * 
 * Version: 1
*/
QSqlQuery* DBAccess::_deleteBatch(QString tableName, QList<QMap<QString, QString> > rows) throw (QueryException) {
    QString sqlStr;
    QString valStr;

    // For each row..
    QListIterator<QMap<QString, QString> > iter(rows);
    while (iter.hasNext()) {

        // For each value..
        valStr += " (";
        QMapIterator<QString, QString> jter(iter.next());
        while(jter.hasNext()) {
            jter.next();
            valStr += jter.key() + " = ";
            valStr += "'" + jter.value() + "'";
            if(jter.hasNext()) {
                valStr += " AND ";
            }
        }
        valStr += ")";

        if(iter.hasNext()) {
            valStr += " OR ";
        }
    }
    sqlStr = "DELETE FROM " + tableName + " WHERE " + valStr + ";";
    try {
        return __query(sqlStr);
    } catch(QueryException& e) {
        throw e;
    }
}
