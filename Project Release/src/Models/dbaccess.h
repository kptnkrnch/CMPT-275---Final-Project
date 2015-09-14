#ifndef DBACCESS
#define DBACCESS

//#define JOSH_SERVER

#include <QCoreApplication>
#include <QtSql>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>

#include <QString>
#include <QMap>

#include "../Helpers/debug.h"
#include "../Exceptions/queryexception.h"

/**
 * ClassName: DBAccess
 *
 * Description: Handles formatting and executing SQL queries to the database
 *  Using a map (specifically QMap) to represent the columns and values of the table
 *
 *
 * Methods:
 *    bool _insert(QString tableName, QMap<QString, QString> values);
 *    QSqlQuery* _insert(QString tableName, QMap<QString, QString> values, bool returnInsertedObject) throw (QueryException);
 *      Translates into an INSERT SQL query. The values map contains a key for all required columns of tableName.
 *      returnInsertedObject is mainly for tables witha auto incrementing keys, the item is returned to know the key of
 *      our newly inserted object for further manipulation.
 *    QSqlQuery* _insertBatch(QString tableName, QList<QMap<QString, QString> > rows) throw (QueryException);
 *      Translates into an INSERT INTO tableName (rows0Key1, rows0Key2 ...)
 *              VALUES ((rows0Value1, rows0Values2 ...), (rows1Value1, rows1Values2 ...), ...)
 *
 *    QSqlQuery* _getAll(QString tableName, QString pKey) throw (QueryException);
 *      Returns all rows of tableName.
 *    QSqlQuery* _get(QString tableName, QString pKey, QString which) throw (QueryException);
 *      Returns a specific row, pKey represents a column in tableName with unique values
 *      Translates into SELECT * FROM tableName WHERE pKey = which
 *
 *    QSqlQuery* _fromWhere(QString tableName, QMap<QString, QString> keyValues, QString andor = "AND") throw (QueryException);
 *      Returns the selected items from tableName. keyValues map represents a row for tableName with keys as the columns.
 *      andor is the operator to filter with
 *      Translates into SELECT * FROM tableName WHERE pKeyKey1 = pKeyValue1, andor
 *    QSqlQuery* _fromWhere(QString tableName, QString parameter, QStringList values) throw (QueryException);
 *      Returns specified items form tableName
 *      Translates into SELECT * FROM tableName WHERE parameter = values1 OR parameter = values2 ...
 *
 *    QSqlQuery* _update(QString tableName, QMap<QString, QString> pKeys, QMap<QString, QString> values) throw (QueryException);
 *      Updates the specified current values stored in pKeys (each key in the maps represent a column in tableName) to
 *      the new values tored in values.
 *      Translates into UPDATE tableName SET(valuesKey1 = valuesValue1, valuesKey2 = values, ...)
 *          WHERE pKeysKey1 = pKeysValue1 AND pKeysKey2 = pKeysValue2 ...
 *    bool _updateBatch(QString tableName, QList<QMap<QString, QString> > currentValues, QMap<QString, QString> newValues);
 *      Translates into UPDATE tableName SET(valuesKey1 = valuesValue1, valuesKey2 = values, ...)
 *          WHERE (currentValues1Key1 = currentValues1Value1 AND currentValues1Key2 = currentValues1Value2 ...)
 *              OR (currentValues2Key1 = currentValues2Value1 AND currentValues2Key2 = currentValues2Value2 ...)
 *              ...
 *    bool _updateBatch(QString tableName, QString parameter, QString currVal, QString newVal);
 *
 *    bool _delete(QString tableName, QString pKey, QString which);
 *    QSqlQuery* _deleteBatch(QString tableName, QList<QMap<QString, QString> > rows) throw (QueryException);
 *
 *    bool connect();
 *    bool disconnect();
 *    bool isConnected();
 *    bool tableExists(QString table);
 *      Checks if the table exists in the database.
 *    QStringList getTables() const { return db.tables(QSql::Tables); }
 *    QSqlRecord getRecord(QString tableName) { return db.driver()->record(tableName); }
 *      Gets information about the columns in tableName.
 *    QSqlQuery * __query(QString sql) throw (QueryException);
 *      Executes the prepared SQL string on the database. Throws QueryException when query failed to execute.
 * Members:
 *
 * Author: Joshua Campbell and German Villarreal
 *
 * Version: 1
 */
class DBAccess {
private:
    QString dbname;     // Database to access
    QString username;   // Username to log into database
    QString password;


    QString hostname;   // Database location
    QString connection_string;
    QSqlDatabase db;

    bool connected;
    QStringList tables;

    DBAccess(DBAccess const&);
    void operator=(DBAccess const&);

public:
    DBAccess();
    ~DBAccess();

    bool _insert(QString tableName, QMap<QString, QString> values);
    QSqlQuery* _insert(QString tableName, QMap<QString, QString> values, bool returnInsertedObject) throw (QueryException);
    QSqlQuery* _insertBatch(QString tableName, QList<QMap<QString, QString> > rows) throw (QueryException);

    QSqlQuery* _getAll(QString tableName, QString pKey) throw (QueryException);
    QSqlQuery* _get(QString tableName, QString pKey, QString which) throw (QueryException);

    QSqlQuery* _fromWhere(QString tableName, QMap<QString, QString> keyValues, QString andor = "AND") throw (QueryException);
    QSqlQuery* _fromWhere(QString tableName, QString parameter, QStringList values) throw (QueryException);

    QSqlQuery* _update(QString tableName, QMap<QString, QString> pKeys, QMap<QString, QString> values) throw (QueryException);
    bool _updateBatch(QString tableName, QList<QMap<QString, QString> > currentValues, QMap<QString, QString> newValues);
    bool _updateBatch(QString tableName, QString parameter, QString currVal, QString newVal);

    bool _delete(QString tableName, QString pKey, QString which);
    QSqlQuery* _deleteBatch(QString tableName, QList<QMap<QString, QString> > rows) throw (QueryException);

    bool connect();
    bool disconnect();
    bool isConnected();
    bool tableExists(QString table);
    QStringList getTables() const { return db.tables(QSql::Tables); }
    QSqlRecord getRecord(QString tableName) { return db.driver()->record(tableName); }
    QSqlQuery * __query(QString sql) throw (QueryException);
};

#endif // DBACCESS
