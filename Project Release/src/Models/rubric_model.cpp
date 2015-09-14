#include "rubric_model.h"
#include "rubricitem_model.h"
#include "rubricitemgrade_model.h"
#include "dbaccess.h"
#include <QListIterator>
#include "model.h"

extern DBAccess DBA;

QMap<QString, QString> Rubric::InsertRubric(bool returnInsertedItem){
    params.remove("rubricID");
    QMap<QString, QString> temp;
    try {
        temp = insertDBItem(params, returnInsertedItem);
        return temp;
    } catch (ParameterException e) {
        DEBUG(DEBUG_ALRM, e.what());
        return temp;
    }
}
QString Rubric::getID()
{
    return params["rubricID"];
}

void Rubric::SetParameter(QString name, QString value) {
    if (name != NULL) {
        params[name] = value;
    }
}

/*
    Description:
        This function will return a list of rubric sections associated with the rubric you want to get.
    Parameters:
        _rubricID - the ID of the rubric that you would like to obtain
*/

QList<QMap<QString, QString> > Rubric::getRubric(QString _rubricID) throw(ParameterException){
    QList<QMap<QString, QString> > rubricItems;
    Model m("RubricItem", "rubricItemID");
    QString sqlStr = "SELECT * FROM RubricItem WHERE rubricID='" + _rubricID + "';";
    if (DBA.isConnected()) {
        QSqlQuery * q = DBA.__query(sqlStr);
        QStringList params = m.getParameters();
        while(q != 0 && q->next()) {
            QMap<QString, QString> section;
            foreach(QString parameter, params) {
                section[parameter] = q->value(parameter).toString();
            }
            rubricItems.push_back(section);
        }
        if (q != 0) {
            delete q;
        } else {
            throw ParameterException("This rubric does not exist.");
        }
    } else {
        throw ParameterException("Application is not connected to the database.");
    }
    return rubricItems;
}

/*
    Description:
        This function will create new rubric section and add it to the rubric objects section list.
        Note: This function will not insert rubric sections into the database.
    Parameters:
        _itemNumber - the position in the rubric's section list
        _expectedOutcome - description of the expected outcome
        _weight - value of the rubric section for grading
*/
void Rubric::createRubricSection(QString _itemNumber, QString _expectedOutcome, QString _weight) {
    QMap<QString, QString> section;
    section["sectionID"] = _itemNumber;
    section["expectedOutcome"] = _expectedOutcome;
    section["weight"] = _weight;
    rubric_sections.push_back(section);
}

/*
    Description:
        This function will first insert a new rubric for the desired activity. It will then insert this rubric object's
        rubric sections into the database using the returned rubricID.
    Parameters:
        _activityID - the activity that this rubric will belong to.
*/
void Rubric::InsertRubric(QString _activityID) {
    QMap<QString, QString> rubric_item;
    rubric_item["activityID"] = _activityID;
    try {
        QMap<QString, QString> temp = insertDBItem(rubric_item, true);
        QString rubricID = temp["rubricID"];

        QListIterator<QMap<QString, QString> > iter(rubric_sections);
        while (iter.hasNext()) {
            QMap<QString, QString> temp = iter.next();
            InsertRubricSection(rubricID, temp);
        }

    } catch(ParameterException e) {
        DEBUG(DEBUG_ALRM, e.what());
    }
}

/*
    Description:
        This function will insert a new rubric section into the database for the specified rubric.
    Parameters:
        _rubricID - the rubric this section will belong to
        _section - the QMap containing column values for the RubricSection
*/
void Rubric::InsertRubricSection(QString _rubricID, QMap<QString, QString> _section) throw(ParameterException){
    Model m("RubricSection", "sectionID");
    _section["rubricID"] = _rubricID;
    try {
        m.insertDBItem(_section);
    } catch (ParameterException e) {
        throw e;
    }
}

QString Rubric::getRubricID(QString _activityID){

    QList<QMap<QString, QString> > list = getDBItems("activityID", QStringList(_activityID));

    if (list.length() == 0)
    {
        return "";
    }

    else return list[0]["rubricID"];
}

bool Rubric::delRubric(QString _activityID){
    RubricItem rubricItemsModel;
    RubricItemGrade rubricItemGradeModel;
    bool success = false;
    QString rubricID = getRubricID(_activityID);

    QList<QMap<QString, QString> > rubricItems = getRubric(rubricID);

    if (rubricItems.count()>0){
        success = deleteDBItem(rubricID); //del rubric

        rubricItemsModel.delRubricItems(rubricID);//del rubricItem


        QStringList ids = QStringList();
        for (int i=0; i<rubricItems.count();i++){
            ids.append(rubricItems[i]["rubricItemID"]);
        }

        rubricItemGradeModel.delRubricItemGrade(ids);//del rubricItemGrade
    }
//    QString sqlStr;
//    QString valStr;
//    QListIterator<QMap<QString, QString> > iter(rubricItems);
//    while (iter.hasNext()) {
//        QMapIterator<QString, QString> jter(iter.next());
//        while(jter.hasNext()) {
//            jter.next();
//            if (jter.key() == "Column"){
//                valStr += jter.key() + " = ";
//                valStr += "'" + jter.value().trimmed() + "'";
//                break;
//             }
//        }
//        if(iter.hasNext()) {
//            valStr += " OR ";
//        }
//    }
//    sqlStr = "DELETE FROM xxxtablexxxx WHERE " + valStr + ";";
//    DBA.__query(sqlStr);

    return success;
}
