#ifndef SESSION
#define SESSION

#include "model.h"
#include <string>
#include <sstream>
#include <cstdlib>

#include <QDateTime>

extern DBAccess DBA;

/**
 * ClassName: Session
 *
 * Description: Handles maintaining user info, page info, and functionality such as logging in, logging out and setting user roles
 *
 * Methods:
 *      Session(std::string _userID, int _userRole) : Model("UserSession", "sessionID")
 *          constructor that creates a user session in the database upon creation
 *
 *      static bool logout(Session ** _session)
 *          deletes and cleans up the user's session
 *
 *      bool SetRole(int _role)
 *          sets the users role and updates the database session data
 *
 * Members:
 *
 * Author: Joshua Campbell
 */

class Session : public Model {
public:
    Session(std::string _userID, int _userRole) : Model("UserSession", "sessionID") {
        userID = _userID;
        userRole = _userRole;

        sessionCode = (QDateTime::currentDateTime().time().second() + 1) * (QDateTime::currentDateTime().time().minute() + 1) * ((rand() % 100000) + 1) * 123;

        QMap<QString, QString> map;
        map = getEmptyRecord();

        std::ostringstream oss;
        oss << sessionCode;

        map.remove("sessionID");
        map["userID"] = userID.c_str();
        map["sessionCode"] = oss.str().c_str();
        map["lastActive"] = QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss");
        oss.clear();
        oss.str("");
        oss << userRole;
        map["role"] = oss.str().c_str();

        try {
            QStringList value;
            value.push_back(_userID.c_str());
            QList<QMap<QString, QString> > db_sessions = getDBItems("userID", value);
            if (db_sessions.count() == 0) {
                QMap<QString, QString> item = insertDBItem(map, true);
                sessionID = item["sessionID"];
            } else {
                QMap<QString, QString> db_session = db_sessions.front();
                sessionID = db_session["sessionID"];
                QMap<QString, QString> pKeys;
                pKeys["sessionID"] = sessionID;
                updateDBItem(pKeys, map);
            }

            DEBUG(DEBUG_INFO, "Session ID: " << sessionID);
        } catch(ParameterException e) {
            DEBUG(DEBUG_WARN, e.what());
        }
    }
	
    static bool logout(Session ** _session) {
        try {
            bool success = true;
            if (_session != 0 && (*_session) != 0) {
                try {
                    QString sessionID = (*_session)->getSessionID();
                    success = (*_session)->deleteDBItem(sessionID);
                } catch (ParameterException& e) {
                    DEBUG(DEBUG_ALRM, e.what());
                }

                if ((*_session) != 0) {
                    delete (*_session);
                    (*_session) = 0;
                }
            }
            return success;
        } catch (ParameterException e) {
            DEBUG(DEBUG_ALRM, e.what());
            return false;
        }
    }

    bool SetRole(int _role) {
        QStringList value;
        value.push_back(userID.c_str());
        QMap<QString, QString> db_session = getDBItem(sessionID);

        if (db_session["sessionID"] == sessionID) {
            std::ostringstream oss;
            oss << _role;
            userRole = _role;

            QMap<QString, QString> map;
            QMap<QString, QString> pKeys;
            pKeys["sessionID"] = sessionID;
            map["role"] = oss.str().c_str();
            map["lastActive"] = QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss");
            updateDBItem(pKeys, map);

            return true;
        }
        return false;
    }

    std::string getUserID() {
        return userID;
    }

    QString getSessionID() {
        return sessionID;
    }

    int getSessionCode() {
        return sessionCode;
    }

    int getUserRole() {
        return userRole;
    }

    void setCourseID(QString cid){
        courseID = cid;
    }

    QString getCourseID() {
        return courseID;
    }

    QString getActivityID(){
        return activityID;
    }

    void setActivityID(QString aid){
        activityID = aid;
    }
    void setCourseTitle(QString title){
        courseTitle = title;
    }

    QString getCourseTitle(){
        return courseTitle;
    }

    void setStudentID(QString sid){
        studentID = sid;
    }

    QString getStudentID(){
        return studentID;
    }

    void setActivityType(QString type){
        activityType = type;
    }

    QString getActivityType(){
        return activityType;
    }

private:
    std::string userID;
    QString sessionID;
    int sessionCode;
    int userRole;
    QString courseID;
    QString activityID;
    QString courseTitle;
    QString studentID;
    QString activityType;
};

#endif // SESSION

