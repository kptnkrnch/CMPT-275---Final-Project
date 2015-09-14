#include "progtest_model.h"
#include "dbaccess.h"
#include "model.h"

#include "progtestinput_model.h"
#include "progtestoutput_model.h"

extern DBAccess DBA;


QString ProgTest::insertProgTest(ProgrammingTest * test, QString activityID)
{
    QString testID;

    QMap<QString, QString> temp;
    temp["pathToConsoleInput"] = test->consoleInput();
    temp["pathToConsoleOutput"] = test->consoleOutput();
    temp["pathToConsoleError"] = test->consoleError();
    temp["activityID"] = activityID;
    temp["name"] = test->name();
    temp["timeout"] = QString::number(test->timeout());
    try {
        temp = insertDBItem(temp, true);
        testID = temp["programmingTestID"];
    } catch (ParameterException e) {
        DEBUG(DEBUG_ALRM, e.what());
        return testID;
    }

    QList<QMap<QString, QString> > inputs;
    QList<QMap<QString, QString> > outputs;

    for (int i = 0; i < test->inputPaths().length(); i++)
    {
        DEBUG(DEBUG_INFO, QString::number(test->inputPaths().length()));
        QMap<QString, QString> inputFile;
        inputFile["programmingTestID"] = testID;
        inputFile["filePath"] = test->inputPaths()[i];
        inputs.append(inputFile);
    }

    for (int i = 0; i < test->outputPaths().length(); i++)
    {
        DEBUG(DEBUG_INFO, QString::number(test->inputPaths().length()));
        QMap<QString, QString> outputFile;
        outputFile["programmingTestID"] = testID;
        outputFile["filePath"] = test->outputPaths()[i];
        outputs.append(outputFile);
    }

    ProgTestInput ti;
    ti.deleteProgTestInputs(testID);
    ti.insertInputFiles(inputs);
    ProgTestOutput to;
    to.deleteProgTestOutputs(testID);
    to.insertOutputFiles(outputs);

    return testID;
}

QList<ProgrammingTest> ProgTest::getProgTests(QString activityID)
{
    QStringList ql;
    ql.append(activityID);

    QList<QMap<QString, QString> > items = getDBItems(QString("activityID"), ql);
    QList<ProgrammingTest> programmingTestList;

    ProgTestInput in;
    ProgTestOutput out;

    for (int i = 0; i < items.length(); i++)
    {
        QString testID = items[i]["programmingTestID"];
        QString name = items[i]["name"];
        QString consoleInput = items[i]["pathToConsoleInput"];
        QString consoleOutput = items[i]["pathToConsoleOutput"];
        QString consoleError = items[i]["pathToConsoleError"];
        int timeout = items[i]["timeout"].toInt();

        QList<QMap<QString, QString> > inputs = in.getInputFiles(items[i]["programmingTestID"]);
        QStringList inputFiles;
        for (int j = 0; j < inputs.length(); j++)
        {
            inputFiles.append(inputs[j]["filePath"]);
        }

        QList<QMap<QString, QString> > outputs = out.getOutputFiles(items[i]["programmingTestID"]);
        QStringList outputFiles;
        for (int j = 0; j < outputs.length(); j++)
        {
            outputFiles.append(outputs[j]["filePath"]);
        }

        ProgrammingTest test(name, consoleInput, consoleOutput,
                             consoleError, inputFiles, outputFiles,
                             timeout);

        programmingTestList.append(test);
    }

    return programmingTestList;
}

bool ProgTest::deleteProgTests(QString activityID)
{
    QStringList ql;
    ql.append(activityID);
    QList<QMap<QString, QString> > testRows = getDBItems("activityID", ql);

    ProgTestInput in;
    ProgTestOutput out;


    for (int i = 0; i < testRows.length(); i++)
    {
        QString testID = testRows[i]["programmingTestID"];

        if (!in.deleteProgTestInputs(testID))
            return false;
        if(!out.deleteProgTestOutputs(testID))
            return false;
    }
    return deleteDBItems(testRows);

}
