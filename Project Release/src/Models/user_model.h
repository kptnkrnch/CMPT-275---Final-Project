#ifndef USER_H
#define USER_H

#include "model.h"
#include "session.h"
#include "../Models/course_model.h"
#include "../Models/marker_model.h"
#include "../Exceptions/loginexception.h"
#include "../Exceptions/dbgetexception.h"
#include "../Exceptions/userexception.h"
#include "../Exceptions/nullpointerexception.h"
#include "../Exceptions/notexistsexception.h"
#include <QString>
#include <QStringList>
#include <QMap>

/**
 * ClassName: User
 *
 * Description: Handles creating user maps, executing methods pertaining to users, and interfacing with the
 *              user table in the database. 
 *
 * Methods:
 *      User() : Model("Users", "userID");
 *          Constructor that sets the table for the model to the Users table with primary key userID.
 *      static const QRegularExpression idRegExp();
 *			Formatting constraint of allowable userIDs
 *      static const QRegularExpression emailRegExp();
 *			Validates for an email string
 *      static const QRegularExpression nameRegExp();
 *			
 *      QList<QMap<QString, QString> > getAllUsers();
 *          Returns a list of all users stored in a map (column name, value)
 *      QList<QMap<QString, QString> > getAllTAs();
 *          Returns a list of all users who are TAs as a map (column name, value)
 *      QList<QMap<QString, QString> > getAllInstructors();
 *          Returns a list of all users who are Instructors as a map (column name, value)
 *      QList<QMap<QString, QString> > getAllSystemAdmins();
 *          Returns a list of all users who are System Administrators as a map (column name, value)
 *      QList<QMap<QString, QString> > getAllInstructorsAndTAs();
 *          Returns a list of all users who are Instructors or TAs as a map (column name, value)
 *      bool isLastSystemAdministrator(QMap<QString, QString> const * const user);
 *          Checks if the user is the last System Administrator in the database.
 *      QMap<QString, QString> createUser();
 *          Returns a map (column name, value) of the user object to be inserted into the database.
 *			New users must reset their password, other defaults also set.
 *      QMap<QString, QString> createUser(QString userID);
 *          Creates a new user with a specified userID (returns that newly created user as a map)
 *      bool updateUserID(QString oldID, QString newID);
 *          Updates the databased entry with oldID and replaces it with newID (returns true upon success)
 *      bool insertUser(QMap<QString, QString> user) throw(ParameterException);
 *          Processes and inserts the contents of the user map into the Users table 
 *			Throws exception if missing information, returns false if database query failed.
 *      bool deleteUser(QMap<QString, QString> user) throw(ParameterException);
 *          Processes and deletes the user associated with the user map parameter (from the database)
 *			Removes from any associated instructor, or marker roles, and does not allow removing the last system administrator
 *      bool deleteUser(QString userID) throw (ParameterException);
 *          Overridden deleteUser method, deletes the user with the userID parameter in the database
 *      bool verify (QMap<QString, QString> const * const   user);
 *			Checks all fields of the user map (column name, value) are allowable types by the database and other constraints set.
 *
 *      bool updateUser(QMap<QString, QString> user);
 *          Updates the user in the database who has the same userID parameter as that of the user map. The database user's
 *          remaining data is replaced by the contents of the user map.
 *      static QMap<QString, QString> login(QString username, QString password) throw(LoginException, UserException);
 *          Returns a user who has the same username and password as that of the username and password parameters
 *      QMap<QString, QString> getUser(const QString username) throw (DBGetException);
 *          Returns a user who has the same userID as the username parameter.
 *      static bool verifyUserID(const QString newID);
 *      static bool verifyEmployeeID(const QString newID);
 *		...
 *			Checks the specified parameter in the map with the required constraints
 *		
 *		static QString getUserID         (QMap<QString, QString> const * const user) throw (NullPointerException);
 *		static QString getEmployeeID     (QMap<QString, QString> const * const user) throw (NullPointerException);
 *		...
 *			static Getters return the information of the specified column name found in the map, maps can be any object that contains the 
 *			same field
 *
 *		static bool setUserID            (QString newID, QMap<QString, QString> *user, bool verify = false) throw (NullPointerException);
 *		static bool setEmployeeID        (QString newID, QMap<QString, QString> *user, bool verify = false) throw (NullPointerException);
 *		...
 *			static Setters create a new key value pair in the map of the specified column of the user table, allows adding the foreign key parameters
 *			to maps representing other objects with the same field name with the same restrictions if the verify parameter is set as true
 *
 * Author: German Villarreal, Joshua Campbell & Connor MacLeod
 * 
 * Version: 1
 */

class User : public Model {
public:

    static const int LEN_USERID = 10;
    static const int MAX_LOGIN_ATTEMPTS  = 6;

    enum { ROLE_ADMINISTRATOR = 1 };
    enum { ROLE_INSTRUCTOR = 2 };
    enum { ROLE_TEACHINGASSISTANT = 3 };
    enum { ROLE_SYSTEMADMINISTRATOR = 4 };
    enum { ROLE_ADMINISTRATIVEASSISTANT = 5 };
    
    static const QRegularExpression idRegExp();
    static const QRegularExpression emailRegExp();
    static const QRegularExpression nameRegExp();

    User() : Model("Users", "userID") { }

    QList<QMap<QString, QString> > getAllUsers();
    QList<QMap<QString, QString> > getAllTAs();
    QList<QMap<QString, QString> > getAllInstructors();
    QList<QMap<QString, QString> > getAllSystemAdmins();
    QList<QMap<QString, QString> > getAllInstructorsAndTAs();

    bool isLastSystemAdministrator(QMap<QString, QString> const * const user);

    QMap<QString, QString> createUser();
    QMap<QString, QString> createUser(QString userID);

    bool updateUserID(QString oldID, QString newID);
    bool insertUser(QMap<QString, QString> user) throw(ParameterException);
    bool deleteUser(QMap<QString, QString> user) throw(ParameterException);
    bool deleteUser(QString userID) throw (ParameterException);
    bool verify (QMap<QString, QString> const * const   user);
    bool updateUser(QMap<QString, QString> user);

    static QStringList getUserAccounts(int start, int end);

    /* Description: Validates the login credentials and creates a new session object,
     *              stores session data in the database for comparisons
     * Exceptions: LoginException
    */
    static QMap<QString, QString> login(QString username, QString password) throw(LoginException, UserException);
    QMap<QString, QString> getUser(const QString username) throw (DBGetException);

    static QString getUserID         (QMap<QString, QString> const * const user) throw (NullPointerException);
    static QString getEmployeeID     (QMap<QString, QString> const * const user) throw (NullPointerException);
    static QString getPassword       (QMap<QString, QString> const * const user) throw (NullPointerException);
    static QString getEmail          (QMap<QString, QString> const * const user) throw (NullPointerException);
    static QString getFirstName      (QMap<QString, QString> const * const user) throw (NullPointerException);
    static QString getMiddleName     (QMap<QString, QString> const * const user) throw (NullPointerException);
    static QString getLastName       (QMap<QString, QString> const * const user) throw (NullPointerException);
    static QList<int> getRoles       (QMap<QString, QString> const * const user) throw (NullPointerException);
    static bool isInstructor         (QMap<QString, QString> const * const user) throw (NullPointerException);
    static bool isSystemAdministrator(QMap<QString, QString> const * const user) throw (NullPointerException);
    static bool isAdministrativeAssistant(QMap<QString, QString> const * const user) throw (NullPointerException);
    static bool isTeachingAssistant  (QMap<QString, QString> const * const user) throw (NullPointerException);
    static bool isAdministrator      (QMap<QString, QString> const * const user) throw (NullPointerException);
    static bool isRole               (QString userID, int role);
    static bool isRole               (QMap<QString, QString> const * const user, int role) throw (NullPointerException);
    static bool isBlocked            (QMap<QString, QString> const * const user) throw (NullPointerException);
    static bool isPwdResetNeeded     (QMap<QString, QString> const * const user) throw (NullPointerException);
    static int getLoginAttempts      (QMap<QString, QString> const * const user) throw (NullPointerException);

    static bool setUserID            (QString newID, QMap<QString, QString> *user, bool verify = false) throw (NullPointerException);
    static bool setEmployeeID        (QString newID, QMap<QString, QString> *user, bool verify = false) throw (NullPointerException);
    static bool setPassword          (QString password, QMap<QString, QString> *user, bool verify = false) throw (NullPointerException);
    static bool setEmail             (QString email, QMap<QString, QString> *user, bool verify = false) throw (NullPointerException);
    static bool setFirstName         (QString fName, QMap<QString, QString> *user, bool verify = false) throw (NullPointerException);
    static bool setMiddleName        (QString mName, QMap<QString, QString> *user, bool verify = false) throw (NullPointerException);
    static bool setLastName          (QString lName, QMap<QString, QString> *user, bool verify = false) throw (NullPointerException);
    static bool setInstructor(bool instructor, QMap<QString, QString> *user, bool verify = false) throw (NullPointerException);
    static bool setSystemAdministrator(bool sysAdmin, QMap<QString, QString> *user, bool verify = false) throw (NullPointerException);
    static bool setAdministrativeAssistant(bool adminAssist, QMap<QString, QString> *user, bool verify = false) throw (NullPointerException);
    static bool setTeachingAssistant(bool ta, QMap<QString, QString> *user, bool verify = false) throw (NullPointerException);
    static bool setAdministrator     (bool admin, QMap<QString, QString> *user, bool verify = false) throw (NullPointerException);
    static void setBlocked           (bool blocked, QMap<QString, QString> *user) throw (NullPointerException);
    static void setPwdResetNeeded    (bool pwdReset, QMap<QString, QString> *user) throw (NullPointerException);
    static void setLoginAttempts     (int totalAttempts, QMap<QString, QString> *user) throw (NullPointerException);

    static bool verifyUserID            (const QString newID);
    static bool verifyEmployeeID        (const QString newID);
    static bool verifyPassword          (const QString password);
    static bool verifyEmail             (const QString email);
    static bool verifyFirstName         (const QString fName);
    static bool verifyMiddleName        (const QString mName);
    static bool verifyLastName          (const QString lName);
    static bool verifyRoles             (QMap<QString, QString> const * const user) throw (NullPointerException);
    static bool verifyLoginAttempts     (const int totalAttempts);

private:

};

#endif // USER_H
