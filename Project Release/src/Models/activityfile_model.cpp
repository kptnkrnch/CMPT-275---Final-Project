#include "model.h"
#include "activityfile_model.h"

 bool ActivityFile::InsertActivityFile() {
     params.remove("fileNumber");
     try {
         QMap<QString, QString> temp = insertDBItem(params, true);
         return true;
     } catch (ParameterException e) {
         DEBUG(DEBUG_ALRM, e.what());
         return false;
     }
}

QString ActivityFile::getID()
{
    return params["fileNumber"];
}

void ActivityFile::SetParameter(QString name, QString value) {
    if (name != NULL) {
        params[name] = value;
    }
}

QList<QMap<QString, QString> > ActivityFile::getByActivityID(QString activityID) {
    QStringList ql;
    ql.append(activityID);
    return parseResult(DBA._fromWhere(getTableName(), "activityID", ql));
}

bool ActivityFile::updateActivityFile(QString fileNumber, QMap<QString, QString> valuesToChange) {

    // Lookup the course to change just by the key since it can't change
    QMap<QString, QString> tmp;
    tmp.insert(getKey(), fileNumber);

    // Auto incremented, so why would this need to change
    valuesToChange.remove(getKey());

    return updateDBItem(tmp, valuesToChange);
}

bool ActivityFile::deleteActivityFiles(QString activityID) {
    return deleteDBItems("activityID", activityID);
}
