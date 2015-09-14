#include "programmingtestdialog.h"
#include "ui_programmingtestdialog.h"

ProgrammingTestDialog::ProgrammingTestDialog(bool * ok, QString title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgrammingTestDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(title);
    ui->timeoutEdit->setValidator(new QIntValidator(0, INT_MAX, this));
    this->ok = ok;
}

ProgrammingTestDialog::ProgrammingTestDialog(bool * ok, QString title, ProgrammingTest * test, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgrammingTestDialog)
{
    ui->setupUi(this);
    this->test = test;
    this->setWindowTitle(title);
    ui->timeoutEdit->setValidator(new QIntValidator(0, INT_MAX, this));

    ui->nameEdit->setText(test->name());
    ui->consoleInputEdit->setText(test->consoleInput());
    ui->consoleOutputEdit->setText(test->consoleOutput());
    ui->consoleErrorEdit->setText(test->consoleError());
    ui->timeoutEdit->setText(QString::number(test->timeout()));

    for(int i = 0; i < test->inputPaths().length(); i++)
    {
        ui->inputList->addItem(test->inputPaths()[i]);
    }

    for (int i = 0; i < test->outputPaths().length(); i++)
    {
        ui->outputList->addItem(test->outputPaths()[i]);
    }

    this->ok = ok;
}

ProgrammingTestDialog::~ProgrammingTestDialog()
{
    delete ui;
}

bool listWidgetContains(QListWidget * lw, QString s)
{
    bool found = false;
    for (int i = 0; i < lw->count(); i++)
    {
        if (lw->item(i)->text() == s)
        {
            found = true;
            break;
        }
    }

    return found;
}

ProgrammingTest * ProgrammingTestDialog::getProgrammingTest()
{
    return this->test;
}

void ProgrammingTestDialog::on_consoleInputBrowseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "");
    ui->consoleInputEdit->setText(fileName);
}

void ProgrammingTestDialog::on_consoleOutputBrowseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "");
    ui->consoleOutputEdit->setText(fileName);
}

void ProgrammingTestDialog::on_consoleErrorBrowseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "");
    ui->consoleErrorEdit->setText(fileName);
}

void ProgrammingTestDialog::on_inputBrowseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "");
    if (!listWidgetContains(ui->inputList, fileName))
    {
        ui->inputList->addItem(fileName);
    }
}

void ProgrammingTestDialog::on_outputBrowseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "");
    if (!listWidgetContains(ui->outputList, fileName))
    {
        ui->outputList->addItem(fileName);
    }
}

void ProgrammingTestDialog::on_inputRemoveButton_clicked()
{
    ui->inputList->takeItem(ui->inputList->currentRow());
}

void ProgrammingTestDialog::on_outputRemoveButton_clicked()
{
    ui->outputList->takeItem(ui->outputList->currentRow());
}

void ProgrammingTestDialog::on_cancelButton_clicked()
{
    *ok = false;
    this->close();
}

void ProgrammingTestDialog::on_doneButton_clicked()
{
    *ok = true;

    QString name = ui->nameEdit->text();
    QString consoleIn = ui->consoleInputEdit->text();
    QString consoleOut = ui->consoleOutputEdit->text();
    QString consoleError = ui->consoleErrorEdit->text();
    QString timeout = ui->timeoutEdit->text();
    if (name == "" ||
        consoleIn  == "" ||
        consoleOut == "" ||
        timeout == "")
    {
        QMessageBox msg;
        msg.setText("A required field is empty");
        msg.exec();
    }
    else
    {
        QStringList inputs, outputs;
        for (int i = 0; i < ui->inputList->count(); i++)
        {
            inputs.append(ui->inputList->item(i)->text());
        }

        for (int i = 0; i < ui->outputList->count(); i++)
        {
            outputs.append(ui->outputList->item(i)->text());
        }

        int timeout = ui->timeoutEdit->text().toInt();

        this->test = new ProgrammingTest(name, consoleIn, consoleOut, consoleError, inputs, outputs, timeout);

        this->close();
    }
}
