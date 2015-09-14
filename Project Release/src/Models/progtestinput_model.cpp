#include "progtestinput_model.h"

void ProgTestInput::insertInputFiles(QList<QMap<QString, QString> > inputs)
{
    if(inputs.length() > 0)
        insertDBItems(inputs);
}

QList<QMap<QString, QString> > ProgTestInput::getInputFiles(QString testID)
{
    QStringList ql;
    ql.append(testID);
    QList<QMap<QString, QString> > items = getDBItems(QString("programmingTestID"), ql);
    return items;
}

bool ProgTestInput::deleteProgTestInputs(QString testID)
{
    QStringList ql;
    ql.append(testID);
    QList<QMap<QString, QString> > rows = getDBItems(QString("programmingTestID"), ql);
    return deleteDBItems(rows);
}
