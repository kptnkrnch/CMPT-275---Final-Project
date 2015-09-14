#ifndef PROGRAMMINGGRADINGSCREEN_H
#define PROGRAMMINGGRADINGSCREEN_H

#include "programmingtest.h"

#include <QWidget>
#include <QListWidget>

namespace Ui {
class ProgrammingGradingScreen;
}

class ProgrammingGradingScreen : public QWidget
{
    Q_OBJECT

public:
    explicit ProgrammingGradingScreen(QWidget *parent = 0);
    explicit ProgrammingGradingScreen(QMap<QString, QString> activity, QString studentID, QWidget *parent = 0);
    ~ProgrammingGradingScreen();
    QMap<QString, QString> activity;
    QMap<QString, QString> activityFile;
    QString studentID;

private slots:
    void on_compileButton_clicked();

    void on_cancelButton_clicked();

    void on_runButton_clicked();

    void on_codeList_currentRowChanged(int currentRow);

    void on_outputFileList_currentRowChanged(int currentRow);

    void on_programmingTestList_currentRowChanged(int currentRow);

    void on_codeSubmissionText_selectionChanged();

    void on_commentButton_clicked();

    void on_commentEdit_textChanged();

    void on_submitButton_clicked();

private:
    bool compileStudentCode();
    void setTextEditFonts();
    Ui::ProgrammingGradingScreen *ui;
    QList<ProgrammingTest> tests;
    QList<QMap<QString, QString> > sources;
    QString path;
    QString exeName;
    QString fullTestDir;
    QString commentPath;
    QStringList fullOutputPaths;
    QList<QMap<QString, QString> > rubricItemGrades;
};

#endif // PROGRAMMINGGRADINGSCREEN_H
