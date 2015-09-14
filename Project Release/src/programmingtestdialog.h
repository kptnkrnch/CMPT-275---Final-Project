#ifndef PROGRAMMINGTESTDIALOG_H
#define PROGRAMMINGTESTDIALOG_H

#include <QMessageBox>
#include <QDialog>
#include <QFileDialog>
#include "programmingtest.h"

namespace Ui {
class ProgrammingTestDialog;
}

class ProgrammingTestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgrammingTestDialog(bool * ok, QString title, QWidget *parent = 0);
    explicit ProgrammingTestDialog(bool * ok, QString title, ProgrammingTest * test, QWidget *parent = 0);
    ProgrammingTest * getProgrammingTest();
    ~ProgrammingTestDialog();

private slots:
    void on_consoleInputBrowseButton_clicked();

    void on_consoleOutputBrowseButton_clicked();

    void on_inputBrowseButton_clicked();

    void on_outputBrowseButton_clicked();

    void on_inputRemoveButton_clicked();

    void on_outputRemoveButton_clicked();

    void on_cancelButton_clicked();

    void on_doneButton_clicked();

    void on_consoleErrorBrowseButton_clicked();

private:
    Ui::ProgrammingTestDialog *ui;
    ProgrammingTest *test;
    bool * ok;
};

#endif // PROGRAMMINGTESTDIALOG_H
