#include "programmingtest.h"

ProgrammingTest::ProgrammingTest(QString name,
                                 QString consoleInputPath,
                                 QString consoleOutputPath,
                                 QString consoleErrorPath,
                                 QStringList inputPaths,
                                 QStringList outputPaths,
                                 int timeout)
{
    _name = name;
    _testID = "0";
    _consoleInputPath = consoleInputPath;
    _consoleOutputPath = consoleOutputPath;
    _consoleErrorPath = consoleErrorPath;
    _inputPaths = inputPaths;
    _outputPaths = outputPaths;
    _timeout = timeout;
}

QString ProgrammingTest::name()
{
    return _name;
}

QString ProgrammingTest::ID()
{
    return _testID;
}

QString ProgrammingTest::consoleInput()
{
    return _consoleInputPath;
}

QString ProgrammingTest::consoleOutput()
{
    return _consoleOutputPath;
}

QString ProgrammingTest::consoleError()
{
    return _consoleErrorPath;
}

QStringList ProgrammingTest::inputPaths()
{
    return _inputPaths;
}

QStringList ProgrammingTest::outputPaths()
{
    return _outputPaths;
}

int ProgrammingTest::timeout()
{
    return _timeout;
}

void ProgrammingTest::setName(QString name)
{
    _name = name;
}

void ProgrammingTest::setID(QString ID)
{
    _testID = ID;
}

void ProgrammingTest::setConsoleInput(QString path)
{
    _consoleInputPath = path;
}

void ProgrammingTest::setConsoleOutput(QString path)
{
    _consoleOutputPath = path;
}

void ProgrammingTest::setConsoleError(QString path)
{
    _consoleErrorPath = path;
}

void ProgrammingTest::setInputPaths(QStringList paths)
{
    _inputPaths = paths;
}

void ProgrammingTest::setOutputPaths(QStringList paths)
{
    _outputPaths = paths;
}

void ProgrammingTest::setTimeout(int time)
{
    _timeout = time;
}
