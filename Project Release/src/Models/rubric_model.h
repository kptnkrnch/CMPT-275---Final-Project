#ifndef RUBRIC_MODEL
#define RUBRIC_MODEL

#include <QString>
#include <QMap>
#include <QList>
#include "model.h"
#include "dbaccess.h"

extern DBAccess DBA;

class Rubric : public Model {
public:
    Rubric() : Model("Rubric", "rubricID") {
        params = getEmptyRecord();
    }
                                    //getRubricItems
    QList<QMap<QString, QString> > getRubric(QString _rubricID) throw (ParameterException);
    void InsertRubric(QString _activityID);
    void createRubricSection(QString _itemNumber, QString _expectedOutcome, QString _weight);
    void InsertRubricSection(QString _rubricID, QMap<QString, QString> _section) throw(ParameterException);
    QString getRubricID(QString _activityID);

//delete somethings
    bool delRubric(QString _activityID);//delete rubric, rubricItem, rubricItemGrade
    QMap<QString, QString> InsertRubric(bool returnInsertedItem = true);
    QString getID();
    void SetParameter(QString name, QString value);

private:
    QList<QMap<QString,QString> > rubric_sections;
    QString activityID;
    QMap<QString, QString> params;
};

#endif // RUBRIC_MODEL

