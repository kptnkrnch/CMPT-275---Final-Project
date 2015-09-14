#include "rubricitem_model.h"
#include "model.h"
#include "dbaccess.h"
#include <QListIterator>

extern DBAccess DBA;

bool RubricItem::InsertRubricItem(){
    params.remove("rubricItemID");
    try {
        QMap<QString, QString> temp = insertDBItem(params, true);
        return true;
    } catch (ParameterException e) {
        DEBUG(DEBUG_ALRM, e.what());
        return false;
    }
}

QString RubricItem::getID()
{
    return params["rubricItemID"];
}

void RubricItem::SetParameter(QString name, QString value) {
    if (name != NULL) {
        params[name] = value;
    }
}

bool RubricItem::generateGradeSummaryCSV(QString rubricID) {
    QString sqlStr;
    int numberOfItems;
    QList<QMap<QString, QString> > queryResult;
    QSqlQuery *dbResult;

    sqlStr = "SELECT * FROM RubricItem WHERE rubricID ='" + rubricID + "' ORDER BY rubricItemID;";
    dbResult = DBA.__query(sqlStr);

    QMap<QString, QString> tmp;
    while(dbResult->next()) {
        tmp.insert("rubricItemID", dbResult->value("rubricItemID").toString().trimmed());
        tmp.insert("mark", dbResult->value("mark").toString().trimmed());
        queryResult.append(tmp);
    }
    numberOfItems = queryResult.count();
    QStringList rubItmIDs;
    for(int i = 0; i < numberOfItems; i++) {
        rubItmIDs.append(queryResult[i]["rubricItemID"]);
    }
    sqlStr = "SELECT * FROM RubricItemGrade g ";
    sqlStr+= "JOIN RubricItem i ";
    sqlStr+=   "ON g.rubricItemID = i.rubricItemID ";
    sqlStr+= "JOIN Rubric r ";
    sqlStr+=   "ON i.rubricID = r.rubricID ";
    sqlStr+= "WHERE r.rubricID='" + rubricID + "' ";
    sqlStr+= "ORDER BY g.studentID;";
    dbResult = DBA.__query(sqlStr);
    QMap<QString, QString> tmp1;
    QList<QMap<QString, QString> > studentMark;
    while(dbResult->next()) {
        tmp1.insert("studentID", dbResult->value("studentID").toString().trimmed());
        tmp1.insert("rubricItemID", dbResult->value("rubricItemID").toString().trimmed());
        tmp1.insert("mark", dbResult->value("mark").toString().trimmed());
        studentMark.append(tmp1);
    }
    int size = studentMark.size();
    QMap<QString, QStringList> everyStudent;
    for(int i = 0; i < size; i++) {
        QString studentID = studentMark[i]["studentID"];
        everyStudent.insert(studentID, QStringList());
    }

    QMapIterator<QString, QStringList> iter(everyStudent);
    int last_size=0; //number of student
    while(iter.hasNext()) {
        int itSize;
        iter.next();
        sqlStr = "SELECT * FROM RubricItemGrade g ";
        sqlStr+= "JOIN RubricItem i ";
        sqlStr+=   "ON g.rubricItemID = i.rubricItemID ";
        sqlStr+= "JOIN Rubric r ";
        sqlStr+=   "ON i.rubricID = r.rubricID ";
        sqlStr+= "WHERE r.rubricID='" + rubricID + "' ";
        sqlStr+= "AND g.studentID='" + iter.key() +"'";
        sqlStr+= "ORDER BY i.rubricItemID;";
        dbResult = DBA.__query(sqlStr);
        itSize = 0;
        while(dbResult->next()) {
            itSize++;
        }
        QStringList il;
        for(int i = 0; i < numberOfItems; i++) {
            il.append("0");
        }

        for(int i = 0; i < itSize; i++) {
            il[rubItmIDs.indexOf(studentMark[i+last_size]["rubricItemID"])] = studentMark[i+last_size]["mark"];
        }
        last_size +=itSize;
        everyStudent.insert(iter.key(), il);
        DEBUG(DEBUG_INFO, everyStudent);
    }
    return true;
}

bool RubricItem::delRubricItems(QString _rubricID){
    return deleteDBItems("rubricID",_rubricID);



}
