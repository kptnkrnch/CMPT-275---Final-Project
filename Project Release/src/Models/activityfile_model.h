#ifndef ACTIVITYFILE_MODEL_H
#define ACTIVITYFILE_MODEL_H

#include <QString>
#include <QMap>
#include <QList>
#include "model.h"
#include "../Helpers/debug.h"
#include "dbaccess.h"

extern DBAccess DBA;

class ActivityFile : public Model {
public:
    ActivityFile() : Model("ActivityFile", "fileNumber") {
        params = getEmptyRecord();
    }

    bool InsertActivityFile();
    QString getID();
    void SetParameter(QString name, QString value);
    QList<QMap<QString, QString> >  getByActivityID(QString activityID);
    bool updateActivityFile(QString fileNumber, QMap<QString, QString> valuesToChange);
    bool deleteActivityFiles(QString activityID);

private:
    QMap<QString, QString> params;
};

#endif // ACTIVITYFILE_MODEL_H
