#include "progtestoutput_model.h"

void ProgTestOutput::insertOutputFiles(QList<QMap<QString, QString> > outputs)
{
    if(outputs.length() > 0)
        insertDBItems(outputs);
}

QList<QMap<QString, QString> > ProgTestOutput::getOutputFiles(QString testID)
{
    QStringList ql;
    ql.append(testID);
    QList<QMap<QString, QString> > items = getDBItems(QString("programmingTestID"), ql);
    return items;
}

bool ProgTestOutput::deleteProgTestOutputs(QString testID)
{
    QStringList ql;
    ql.append(testID);
    QList<QMap<QString, QString> > rows = getDBItems(QString("programmingTestID"), ql);
    return deleteDBItems(rows);
}
