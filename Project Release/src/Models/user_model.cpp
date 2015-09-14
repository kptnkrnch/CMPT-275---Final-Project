#include "user_model.h"
#include "dbaccess.h"
#include "session.h"

#include <sstream>
#include <cstdlib>

extern DBAccess DBA;
extern Session *session;

/**
 * @brief User::createUser; provides a map with the necessary fields for a user
 * @return; a default new user map
 */
QMap<QString, QString> User::createUser() {
    QMap<QString, QString> out = getEmptyRecord();

    // can initialize some values here
    setBlocked(false, &out);
    setPwdResetNeeded(true, &out);
    setLoginAttempts(0, &out);

    return out;
}

/**
 * @brief User::createUser; provides a map with the necessary fields for a user
 *          Warning, this does not verify the user id
 * @param userID; the user id of this new user
 * @return; a default user map with specified user id
 */
QMap<QString, QString> User::createUser(QString userID) {
    QMap<QString, QString> out = createUser();

    setUserID(userID, &out);
    return out;
}

bool User::insertUser(QMap<QString, QString> user) throw(ParameterException) {

    QMap<QString, QString> needAddressSigh = user;
    if(verify(&needAddressSigh)) {
        try {
            return insertDBItem(user);
        } catch (ParameterException& e) {
            throw e;
        }
    }
    return false;
}

bool User::isLastSystemAdministrator(QMap<QString, QString> const * const user) {
    return isSystemAdministrator(user) && getAllSystemAdmins().count() == 1;
}

/**
 * @brief User::deleteUser
 * @param user
 * @return
 */
bool User::deleteUser(QMap<QString, QString> user) throw(ParameterException) {

    if(isLastSystemAdministrator(&user)) {
        return false;
    }

    QString userID;
    QList<QMap<QString, QString> > coursesAsAdministrator;
    QList<QMap<QString, QString> > coursesAsInstructor;
    QList<QMap<QString, QString> > coursesAsTA;

    userID = User::getUserID(&user);

    try {
        Course courseModel;
        Marker markerModel;
        coursesAsAdministrator = courseModel.getCoursesByUser(userID, ROLE_ADMINISTRATOR);
        coursesAsInstructor = courseModel.getCoursesByUser(userID, ROLE_INSTRUCTOR);
        coursesAsTA = courseModel.getCoursesByUser(userID, ROLE_TEACHINGASSISTANT);

        // Hooray for no foreign key cascade! _/ _/

        // Clean up from instructor table
        if(User::isInstructor(&user)
                && !courseModel.removeInstructorFromAllCourses(userID)) {
            // Couldn't remove so don't emove from other tables
            return false;
        }

        // Clean up from TA table
        if(User::isTeachingAssistant(&user)
                && !markerModel.removeMarker(userID) // Couldn't remove from TA table
                && !courseModel.assignInstructorToCourses(userID, coursesAsInstructor)) { // Couldn't add back to Instrucor table
            // serious error has occurred
            // was able to remove him as instructor of courses
            // failed to remove him as TA of courses
            // failed to abort and add him back as an instructor
            DEBUG(DEBUG_ALRM, "User was removed as instructor, failed to be removed as TA, and failed the restore of adding back as an instructor. YAY DB DESIGN LACK OF FOREIGN KEYS!");
            return false;
        }

        // Clean up from user table
        if(!deleteDBItem(getUserID(&user))
                && !courseModel.assignInstructorToCourses(userID, coursesAsInstructor) //  Couldn't add back to instructor table
                && !markerModel.assignTACourses(userID, coursesAsTA)) { // Couldn't add back to TA table
            // serious error has occurred
            // was able to remove him as instructor of courses
            // was able to remove him as TA of courses
            // failed to abort and add him back as TA or as instructor
            DEBUG(DEBUG_ALRM, "User was removed as instructor, removed as TA, but failed to be removed as a user, and failed the restore of adding back as instructor and TA. YAY DB DESIGN LACK OF FOREIGN KEYS!");
            return false;
        }

        return true;
    } catch (ParameterException& e) {
        throw e;
    }
    return false;
}

bool User::deleteUser(QString userID) throw (ParameterException){
    try {
        return deleteUser(getUser(userID));
    } catch (ParameterException& e) {
        DEBUG(DEBUG_INFO, e.what());
    }
    return false;
}

/**
 * @brief User::verify; Verifies all user fields
 * @param user; the user to verify
 * @return  true if user fields are of the right format
 */
bool User::verify (QMap<QString, QString> const * const user) {
    if(user == NULL) {
        //NullPointerException("");
        DEBUG(DEBUG_WARN, "user is null");
        return false;
    }

    return verifyEmail(getEmail(user))
            && verifyEmployeeID(getEmployeeID(user))
            && verifyFirstName(getFirstName(user))
            && verifyLastName(getLastName(user))
            && verifyMiddleName(getMiddleName(user))
            && verifyPassword(getPassword(user))
            && verifyRoles(user)
            && verifyUserID(getUserID(user));
}

QList<QMap<QString, QString> > User::getAllUsers() {
    return  getAll();
}

QMap<QString, QString> User::getUser(QString username) throw (DBGetException){
    QMap<QString, QString> user;

    try {
        user = getDBItem(username);
    } catch (ParameterException e) {
        DEBUG(DEBUG_WARN, e.what());
        throw DBGetException("userID was not found in the database.");
    }
    return user;
}

QList<QMap<QString, QString> > User::getAllInstructorsAndTAs() {
    QMap<QString, QString> searchParams;
    setInstructor(true, &searchParams);
    setTeachingAssistant(true, &searchParams);
    return getDBItems(searchParams, "OR");
}

QList<QMap<QString, QString> > User::getAllTAs() {
    return getDBItems("isTeachingAssisstant", "1");
}

QList<QMap<QString, QString> > User::getAllInstructors() {
    return getDBItems("isInstructor", "1");
}

QList<QMap<QString, QString> > User::getAllSystemAdmins() {
    return getDBItems("isSystemAdministrator", "1");
}

QMap<QString, QString> User::login(QString username, QString password) throw(LoginException, UserException) {
    QMap<QString, QString> user;

    User m;
    try {
        user =  m.getDBItem(username);
    } catch (ParameterException e) {
        DEBUG(DEBUG_WARN, e.what());
        throw LoginException("Username was not found in the database.");
    }

    if(getUserID(&user) == username
            && getPassword(&user) == password
            && !isBlocked(&user)) {
        if (session == 0) {
            session = new Session(username.toStdString(), -1);
        } else {
            delete session;
            session = new Session(username.toStdString(), -1);
        }

        setLoginAttempts(0, &user);
        //user["loginAttempts"] = "0";
        m.updateUser(user);

    } else if (isBlocked(&user)) {
        throw UserException("This account is currently blocked.");
    } else {
        int attempts = getLoginAttempts(&user);
        attempts++;
        if (attempts >= MAX_LOGIN_ATTEMPTS) {
            setBlocked(true, &user);
        }

        setLoginAttempts(attempts, &user);
        m.updateUser(user);

        throw LoginException("Username and password did not match an entry in the database.");
    }
    return user;
}

bool User::updateUserID(QString oldID, QString newID) {
    Course courseModel;
    Marker markerModel;
    QMap<QString, QString> currentUser;
    try {
        currentUser = getUser(oldID);
    } catch(DBGetException& e) {
        DEBUG(DEBUG_INFO, e.what());
        return false;
    }

    if(isInstructor(&currentUser)
            && !courseModel.updateInstructor(oldID, newID)) {
        return false;
    }

    if(isTeachingAssistant(&currentUser)
            && !markerModel.updateTA(oldID, newID)) {
        return false;
    }

    try {
        return updateItemKey(oldID, newID);
    } catch(ParameterException& e) {
        DEBUG(DEBUG_WARN, e.what());
        return false;
    }
    return true;
}

/**
 * @brief User::updateUser; Updates all values of the user except for the userID.
 *  Performs required checks and removes him from any courses the user teaches or marks for
 * See Also: updateUserID
 * @param updatedUser; the values to change to
 * @return true if updated successfully
 */
bool User::updateUser(QMap<QString, QString> updatedUser) {
    QMap<QString, QString> currentUser;
    QMap<QString, QString> matchKey;
    QString userID;
    Course courseModel;
    Marker markerModel;

    try {
        userID = getUserID(&updatedUser);
        currentUser = getUser(userID);
    } catch(DBGetException& e) {
        // Should always find it..but gonna catch just for fun
        DEBUG(DEBUG_WARN, e.what());
        return false;
    }

    // If user is being removed the TA role, remove him as a marker
    if(isTeachingAssistant(&currentUser)
            && !isTeachingAssistant(&updatedUser)
            && !markerModel.removeMarker(userID)) {
        DEBUG(DEBUG_WARN, "Could not remove the user from marker table");
        return false;
    }

    // If user is being removed the instructor role remove him as an instructor
    if(isInstructor(&currentUser)
            && !isInstructor(&updatedUser)
            && !courseModel.removeInstructorFromAllCourses(userID)) {
        DEBUG(DEBUG_WARN, "Could not remove user as instructor");
        return false;
    }

    setUserID(userID, &matchKey);
    return updateDBItem(matchKey, updatedUser);
}

bool User::verifyUserID (QString newID) {
    //return QRegularExpression("^\\d{" + QString::number(LEN_USERID) + "}$").match(newID).hasMatch();
    return QRegularExpression("^[a-zA-Z0-9]{3," + QString::number(LEN_USERID) + "}$").match(newID).hasMatch();
}

bool User::verifyEmployeeID(const QString newID) {
    return !newID.isEmpty();
}
bool User::verifyPassword(QString password) {
    return !password.isEmpty();
}
bool User::verifyEmail(QString email) {
    return QRegularExpression("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b").match(email).hasMatch()
            || email.isEmpty();
}
bool User::verifyFirstName(QString fName) {
    //return QRegularExpression("^[a-zA-Z ]{2,10}$").match(fName.toLower()).hasMatch();
    return QRegularExpression("^[a-zA-Z0-9 ]{1,10}$").match(fName.toLower()).hasMatch();
}
bool User::verifyMiddleName(QString mName) {
    return QRegularExpression("^[a-zA-Z ]{1,10}$").match(mName.toLower()).hasMatch()
            || mName.isEmpty();
    //return QRegularExpression("^[a-zA-Z0-9 ]{0,10}$").match(mName.toLower()).hasMatch();
}
bool User::verifyLastName(QString lName) {
    //return QRegularExpression("^[a-zA-Z ]{2,10}$").match(lName.toLower()).hasMatch();
    return QRegularExpression("^[a-zA-Z0-9 ]{1,10}$").match(lName.toLower()).hasMatch();

}
bool User::verifyRoles(QMap<QString, QString> const * const user) throw (NullPointerException) {
    return getRoles(user).size() > 0;
}

QString User::getUserID(QMap<QString, QString> const * const user) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    return user->value("userID");
}
QString User::getEmployeeID(QMap<QString, QString> const * const user) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    return user->value("employeeID");
}
QString User::getPassword(QMap<QString, QString> const * const user) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    return user->value("password");
}
QString User::getEmail(QMap<QString, QString> const * const user) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    return user->value("email");
}
QString User::getFirstName(QMap<QString, QString> const * const user) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    return user->value("firstName");
}
QString User::getMiddleName(QMap<QString, QString> const * const user) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    return user->value("middleName");
}
QString User::getLastName(QMap<QString, QString> const * const user) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    return user->value("lastName");
}
bool User::isInstructor(QMap<QString, QString> const * const user) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    return user->value("isInstructor") == "1";
}
bool User::isSystemAdministrator(QMap<QString, QString> const * const user) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    return user->value("isSystemAdministrator") == "1";
}
bool User::isAdministrativeAssistant(QMap<QString, QString> const * const user) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    return user->value("isAdministrativeAssistant") == "1";
}
bool User::isTeachingAssistant(QMap<QString, QString> const * const user) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    return user->value("isTeachingAssistant") == "1";
}
bool User::isAdministrator(QMap<QString, QString> const * const user) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    return user->value("isAdministrator") == "1";
}

bool User::isRole(QMap<QString, QString> const * const user, int role) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }

    switch(role) {
    case ROLE_ADMINISTRATIVEASSISTANT:
        return isAdministrativeAssistant(user);
    case ROLE_ADMINISTRATOR:
        return isAdministrator(user);
    case ROLE_INSTRUCTOR:
        return isInstructor(user);
    case ROLE_SYSTEMADMINISTRATOR:
        return isSystemAdministrator(user);
    case ROLE_TEACHINGASSISTANT:
        return isTeachingAssistant(user);
    }
    return false;
}

bool User::isRole(QString userID, int role) {
    User m;
    QMap<QString, QString> temp;

    try {
        temp = m.getUser(userID);
    } catch (ParameterException e) {
        DEBUG(DEBUG_ALRM, e.what());
        return false;
    }

    return isRole(getUserID(&temp), role);
}

QList<int> User::getRoles(QMap<QString, QString> const * const user) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    QList<int> out;
    if(isAdministrativeAssistant(user)) {
        out.append(User::ROLE_ADMINISTRATIVEASSISTANT);
    }
    if(isAdministrator(user)) {
        out.append(User::ROLE_ADMINISTRATOR);
    }
    if(isInstructor(user)) {
        out.append(User::ROLE_INSTRUCTOR);
    }
    if(isSystemAdministrator(user)) {
        out.append(User::ROLE_SYSTEMADMINISTRATOR);
    }
    if(isTeachingAssistant(user)) {
        out.append(User::ROLE_TEACHINGASSISTANT);
    }
    return out;
}
bool User::isBlocked(QMap<QString, QString> const * const user) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    return user->value("isBlocked") == "1";
}
bool User::isPwdResetNeeded(QMap<QString, QString> const * const user) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    return user->value("resetPassword") == "1";
}
int User::getLoginAttempts(QMap<QString, QString> const * const user) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    return user->value("loginAttempts").toInt();
}
bool User::setUserID(QString newID, QMap<QString, QString> *user, bool verify) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    if(verify && !verifyUserID(newID)) {
        return false;
    }
    user->insert("userID", newID);
    return true;
}
bool User::setEmployeeID(QString newID, QMap<QString, QString> *user, bool verify) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    if(verify && !verifyEmployeeID(newID)) {
        return false;
    }
    user->insert("employeeID", newID);
    return true;
}
bool User::setPassword(QString password, QMap<QString, QString> *user, bool verify) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    if(verify && !verifyPassword(password)) {
        return false;
    }
    user->insert("password", password);
    return true;
}
bool User::setEmail(QString email, QMap<QString, QString> *user, bool verify) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    if(verify && !verifyEmail(email)) {
        return false;
    }
    user->insert("email", email);
    return true;
}
bool User::setFirstName(QString fName, QMap<QString, QString> *user, bool verify) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    if(verify && !verifyFirstName(fName)) {
        return false;
    }
    user->insert("firstName", fName);
    return true;
}
bool User::setMiddleName(QString mName, QMap<QString, QString> *user, bool verify) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    if(verify && !verifyMiddleName(mName)) {
        return false;
    }
    user->insert("middleName", mName);
    return true;
}
bool User::setLastName(QString lName, QMap<QString, QString> *user, bool verify) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    if(verify && !verifyLastName(lName)) {
        return false;
    }
    user->insert("lastName", lName);
    return true;
}
bool User::setInstructor(bool instructor, QMap<QString, QString> *user, bool verify) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    if(verify && !verifyRoles(user)) {
        return false;
    }
    user->insert("isInstructor", (instructor) ? "1" : "0");
    return true;
}
bool User::setSystemAdministrator(bool sysAdmin, QMap<QString, QString> *user, bool verify) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    if(verify && !verifyRoles(user)) {
        return false;
    }
    user->insert("isSystemAdministrator", (sysAdmin) ? "1" : "0");
    return true;
}
bool User::setAdministrativeAssistant(bool adminAssist, QMap<QString, QString> *user, bool verify) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    if(verify && !verifyRoles(user)) {
        return false;
    }
    user->insert("isAdministrativeAssistant", (adminAssist) ? "1" : "0");
    return true;
}
bool User::setTeachingAssistant(bool ta, QMap<QString, QString> *user, bool verify) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    if(verify && !verifyRoles(user)) {
        return false;
    }
    user->insert("isTeachingAssistant", (ta) ? "1" : "0");
    return true;
}
bool User::setAdministrator(bool admin, QMap<QString, QString> *user, bool verify) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }if(verify && !verifyRoles(user)) {
        return false;
    }
    user->insert("isAdministrator", (admin) ? "1" : "0");
    return true;
}
void User::setBlocked(bool blocked, QMap<QString, QString> *user) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    user->insert("isBlocked", (blocked) ? "1" : "0");
}
void User::setPwdResetNeeded(bool pwdReset, QMap<QString, QString> *user) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    user->insert("resetPassword", (pwdReset) ? "1" : "0");
}
void User::setLoginAttempts(int totalAttempts, QMap<QString, QString> *user) throw (NullPointerException) {
    if(user == NULL) {
        throw NullPointerException("User not defined");
    }
    user->insert("loginAttempts", QString:: number(totalAttempts));
}
