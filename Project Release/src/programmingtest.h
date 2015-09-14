#ifndef PROGRAMMINGTEST_H
#define PROGRAMMINGTEST_H

#include <QString>
#include <QMap>
#include <QStringList>

class ProgrammingTest
{
    public:
        ProgrammingTest(QString name,
                        QString consoleInputPath,
                        QString consoleOutputPath,
                        QString consoleErrorPath,
                        QStringList inputPaths,
                        QStringList outputPaths,
                        int timeout);

        QString name();
        QString ID();
        QString consoleInput();
        QString consoleOutput();
        QString consoleError();
        QStringList inputPaths();
        QStringList outputPaths();
        int timeout();

        void setName(QString);
        void setID(QString);
        void setConsoleInput(QString);
        void setConsoleOutput(QString);
        void setConsoleError(QString);
        void setInputPaths(QStringList);
        void setOutputPaths(QStringList);
        void setTimeout(int);

    private:
        QString _name;
        QString _testID;
        QString _consoleInputPath;
        QString _consoleOutputPath;
        QString _consoleErrorPath;
        QStringList _inputPaths;
        QStringList _outputPaths;
        int _timeout;
};

#endif // PROGRAMMINGTEST_H
