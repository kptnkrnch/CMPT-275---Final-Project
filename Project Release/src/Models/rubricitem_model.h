#ifndef RUBRICITEM_H
#define RUBRICITEM_H

#include <QString>
#include <QMap>
#include <QList>
#include "model.h"
#include "../Helpers/debug.h"
#include "dbaccess.h"

class RubricItem: public Model {
public:
    RubricItem(): Model("RubricItem", "rubricItemID") {
        params = getEmptyRecord();
    }
    bool InsertRubricItem();
    QString getID();
    void SetParameter(QString name, QString value);
    bool generateGradeSummaryCSV(QString rubricID);
    bool delRubricItems(QString _rubricID);

private:
    QMap<QString, QString> params;


};

#endif // RUBRICITEM_H
