#include "programminggradingscreen.h"
#include "ui_programminggradingscreen.h"

#include "Models/activity_model.h"
#include "Models/activityfile_model.h"
#include "Controllers/rubricitemgradewidget.h"
#include "programmingtest.h"
#include "Models/progtest_model.h"
#include "gradestudentselectionscreen.h"
#include "Models/rubricitemgrade_model.h"
#include "Models/rubricitem_model.h"
#include "Models/rubric_model.h"

#include <QFile>

#include "mainwindow.h"
#include "Models/session.h"

extern MainWindow * w;
extern Session * session;

ProgrammingGradingScreen::ProgrammingGradingScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProgrammingGradingScreen)
{
    ui->setupUi(this);

    RubricItemGrade rubricItemGradeModel;
    Activity activityModel;
    activity = activityModel.GetActivityByID(session->getActivityID());
    studentID = session->getStudentID();
    QList<QMap<QString, QString> > rubric = Activity::getRubric(session->getActivityID());

    for (int i = 0; i < rubric.length(); i++)
    {
        QListWidgetItem * item = new QListWidgetItem();
        item->setSizeHint(QSize(item->sizeHint().width(), 48));
        ui->rubricListWidget->addItem(item);
        RubricItemGradeWidget * rubricItem = new RubricItemGradeWidget();


        double grade = rubric[i]["rubricItemValue"].toDouble();
        rubricItem->setMaximumGrade(grade);

        QString desc = rubric[i]["rubricItemText"];
        rubricItem->setItemDescription(desc);

        ui->rubricListWidget->setItemWidget(ui->rubricListWidget->item(i), rubricItem);


        QList<QMap<QString, QString> > rubricItemGrades = rubricItemGradeModel.GetGradesByItemID(rubric[i]["rubricItemID"]);
        QListIterator<QMap<QString, QString> > iterator(rubricItemGrades);
        while (iterator.hasNext()) {
            QMap<QString, QString> temp = iterator.next();
            QMap<QString, QString> data = temp;
            if (temp["studentID"] == session->getStudentID()) {
                rubricItem->setGradeValue(temp["mark"]);
                this->rubricItemGrades.push_back(data);
                break;
            }
        }


    }

    int height = 48 * rubric.length() + 4;
    if (height > 436)
    {
        height = 436;
    }
    ui->rubricListWidget->setFixedHeight(height);



    ProgTest p;
    tests = p.getProgTests(session->getActivityID());
    for (int i = 0; i < tests.length(); i++)
    {
        ui->programmingTestList->addItem(tests[i].name());
    }


    path = activity["pathToWorkToMark"] + "/" + studentID;
    commentPath = path + "/" + studentID + "_comments";


    exeName = "activity" + activity["activityID"] + "out.exe";

    ActivityFile af;
    sources = af.getByActivityID(activity["activityID"]);
    for (int i = 0; i < sources.length(); i++)
    {
        QString fileName = sources[i]["fileName"] + "." + sources[i]["fileType"];
        ui->codeList->addItem(fileName);
        QFile f(path + "/" + fileName);
        if (!f.exists())
        {
            ui->codeList->item(i)->setForeground(Qt::gray);
        }
    }
    setTextEditFonts();
    ui->splitter->setStyleSheet("QSplitter::handle{background-color: white;}");
    ui->splitter_2->setStyleSheet("QSplitter::handle{background-color: white;}");
}

ProgrammingGradingScreen::~ProgrammingGradingScreen()
{
    delete ui;
}

bool ProgrammingGradingScreen::compileStudentCode()
{
    if (activity["language"] == "C++")
    {
        DEBUG(DEBUG_INFO, "***PATH: " + path);

        QProcess * proc = new QProcess(this);
        proc->setWorkingDirectory(path);

        QString compiler = "C:/qt/tools/mingw491_32/bin/g++.exe";





        QString errorFilePath = path + "/error.txt";

        QStringList arguments;
        for (int i = 0; i < sources.length(); i++)
        {
            if (QString::compare(sources[i]["fileType"], "cpp", Qt::CaseInsensitive) == 0)
            {
                arguments << sources[i]["fileName"] + ".cpp";
            }
        }
        arguments << "-o" << exeName;

        proc->setStandardErrorFile(errorFilePath);

        proc->start(compiler, arguments);

        proc->waitForFinished();



        QFile errFile(errorFilePath);
        if (proc->exitStatus() == QProcess::NormalExit && errFile.size() == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

void ProgrammingGradingScreen::on_compileButton_clicked()
{
    if (this->compileStudentCode())
    {
        ui->compileMessage->setText("Build successful");
        ui->compileMessage->setStyleSheet("QLabel { color : green; }");
        ui->runButton->setEnabled(true);
    }
    else
    {
        ui->compileMessage->setText("Build failed");
        ui->compileMessage->setStyleSheet("QLabel { color : red; }");
        ui->runButton->setDisabled(true);
    }
}



void ProgrammingGradingScreen::on_cancelButton_clicked()
{
    w->setCentralWidget(new GradeStudentSelectionScreen);
}

QString pathToFileName(QString path)
{
    int index = path.lastIndexOf("/");
    QString name = path.right(path.length() - index - 1);
    return name;
}

void ProgrammingGradingScreen::on_runButton_clicked()
{
    int index = ui->programmingTestList->currentRow();

    if (index >= 0)
    {
        ProgrammingTest test = tests[index];
        QString testDir = "Test" + test.ID() + "_" + test.name();
        fullTestDir = path + "/" + testDir + "/";

        DEBUG(DEBUG_INFO, "*****TEST PATH: " + fullTestDir);

        QDir d;
        d.setCurrent(path);
        d.mkdir(testDir);

        bool filesFound = true;

        QString cIn = test.consoleInput();
        QString cInFilePath = fullTestDir + pathToFileName(cIn);
        QFile newCIn(cInFilePath);
        if (!newCIn.exists())
        {
            filesFound = filesFound && QFile::copy(cIn, cInFilePath);
        }

        QStringList ins = test.inputPaths();

        for (int i = 0; i < ins.length(); i++)
        {
            QString in = ins[i];
            QString inFilePath = fullTestDir + pathToFileName(in);
            QFile newIn(inFilePath);
            if (!newIn.exists())
            {
                filesFound = filesFound && QFile::copy(in, inFilePath);
            }
        }
        QString exePath(fullTestDir + exeName);
        QFile newExe(exePath);
        if (!newExe.exists())
        {
            filesFound = filesFound && QFile::copy(path + "/" + exeName, exePath);
        }

        if (filesFound)
        {
            ui->runMessage->setText("Found all files");
            ui->runMessage->setStyleSheet("QLabel { color : green; }");

            QProcess * process = new QProcess(this);
            QString executable = fullTestDir + exeName;

            QString cOutFileName = fullTestDir + pathToFileName(test.consoleOutput());
            process->setStandardOutputFile(cOutFileName);

            if (test.consoleError() != "")
            {
                QString cErrFileName = fullTestDir + pathToFileName(test.consoleError());
                process->setStandardErrorFile(cErrFileName);
            }

            process->setStandardInputFile(cInFilePath);
            process->setWorkingDirectory(fullTestDir);
            process->start(exeName);

            bool done = process->waitForFinished(1000 * test.timeout());

            if (done)
            {
                ui->runMessage->setText("Execution finished");
                ui->runMessage->setStyleSheet("QLabel { color : green; }");
            }
            else
            {
                if (process->state() == QProcess::Running)
                {
                    process->kill();
                    ui->runMessage->setText("Execution timed out (" + QString::number(test.timeout()) + " seconds)");
                    ui->runMessage->setStyleSheet("QLabel { color : red; }");
                }
                else
                {
                    ui->runMessage->setText("Execution failed");
                    ui->runMessage->setStyleSheet("QLabel { color : red; }");
                }
            }

            ui->outputSolutionText->clear();
            ui->outputSubmissionText->clear();
            ui->outputFileList->clear();
            ui->outputFileList->clearSelection();
            fullOutputPaths.clear();

            QString cOutputPath = test.consoleOutput();
            QString cErrorPath = test.consoleError();
            QStringList outputPaths = test.outputPaths();

            ui->outputFileList->addItem(pathToFileName(cOutputPath));
            fullOutputPaths.append(cOutputPath);

            if (cErrorPath != "")
            {
                fullOutputPaths.append(cErrorPath);
                ui->outputFileList->addItem(pathToFileName(cErrorPath));
            }

            for (int i = 0; i < outputPaths.length(); i++)
            {
                fullOutputPaths.append(outputPaths[i]);
                ui->outputFileList->addItem(pathToFileName(outputPaths[i]));
            }

        }
        else
        {
            ui->runMessage->setText("Could not find all input files");
            ui->runMessage->setStyleSheet("QLabel { color : red; }");
        }



    }
}

void ProgrammingGradingScreen::on_codeList_currentRowChanged(int currentRow)
{
    if (currentRow >= 0)
    {
        QString fileName = ui->codeList->item(currentRow)->text();
        QFile studentFile(path + "/" + fileName);
        QFile solutionFile(activity["pathToSolutionFile"] + "/" + fileName);

        if(studentFile.open(QIODevice::ReadOnly))
        {
            QTextStream s(&studentFile);
            ui->codeSubmissionText->setText(s.readAll());
        }



        if (solutionFile.open(QIODevice::ReadOnly))
        {
            QTextStream s(&solutionFile);
            ui->codeSolutionText->setText(s.readAll());
        }
    }
    ui->lineNumbersLabel->clear();
}

void ProgrammingGradingScreen::on_outputFileList_currentRowChanged(int currentRow)
{
    if (currentRow >= 0)
    {
        QString fileName = ui->outputFileList->item(currentRow)->text();
        QFile studentFile(fullTestDir + fileName);
        QFile solutionFile(fullOutputPaths[currentRow]);

        if(studentFile.size() < 10*1024*1024 && studentFile.open(QIODevice::ReadOnly))
        {
            QTextStream s(&studentFile);
            QString text;
            text = s.readAll();
            ui->outputSubmissionText->setText(text);
        }
        else
        {
            ui->outputSubmissionText->setHtml("<font color=\"red\">Could not open file</font>");
        }
        if (solutionFile.size() < 10*1024*1024 && solutionFile.open(QIODevice::ReadOnly))
        {
            QTextStream s(&solutionFile);
            ui->outputSolutionText->setText(s.readAll());
        }
        else
        {
            ui->outputSolutionText->setHtml("<font color=\"red\">Could not open file</font>");
        }
    }
}

void ProgrammingGradingScreen::setTextEditFonts()
{
    QFont codeFont;
    codeFont.setFamily("Courier");
    codeFont.setStyleHint(QFont::Monospace);
    codeFont.setFixedPitch(true);
    QFontMetrics m(codeFont);

    ui->codeSolutionText->setFont(codeFont);
    ui->codeSolutionText->setTabStopWidth(4 * m.width(" "));
    ui->codeSubmissionText->setFont(codeFont);
    ui->codeSubmissionText->setTabStopWidth(4 * m.width(" "));
    ui->outputSolutionText->setFont(codeFont);
    ui->outputSolutionText->setTabStopWidth(4 * m.width(" "));
    ui->outputSubmissionText->setFont(codeFont);
    ui->outputSubmissionText->setTabStopWidth(4 * m.width(" "));
}

void ProgrammingGradingScreen::on_programmingTestList_currentRowChanged(int currentRow)
{
    ui->runMessage->clear();
}

void ProgrammingGradingScreen::on_codeSubmissionText_selectionChanged()
{
    QTextCursor c = ui->codeSubmissionText->textCursor();

    int start = c.selectionStart();
    int end = c.selectionEnd();

    if (c.hasSelection())
    {
        c.setPosition(start);
        int startLine = c.blockNumber() + 1;
        c.setPosition(end);
        int endLine = c.blockNumber() + 1;

        QString lineText;
        if (startLine == endLine)
        {
            lineText = "Line " + QString::number(startLine);
        }
        else
        {
            lineText = "Lines " + QString::number(startLine) + " - " + QString::number(endLine);
        }

        ui->lineNumbersLabel->setText(lineText);
    }


}

void ProgrammingGradingScreen::on_commentButton_clicked()
{
    if (ui->commentEdit->toPlainText() == "")
    {
        ui->commentMessage->clear();
        return;
    }
    if (!ui->codeSubmissionText->textCursor().hasSelection())
    {
        ui->commentMessage->setText("Please select code to comment");
        ui->commentMessage->setStyleSheet("QLabel { color : red; }");
        return;
    }

    QFile commentFile(commentPath);
    if (commentFile.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QString lines = ui->lineNumbersLabel->text();
        QString file = ui->codeList->currentItem()->text();
        QString comment = ui->commentEdit->toPlainText();

        commentFile.write(("[" + file.toStdString() + ", " + lines.toStdString() + "]\n").c_str());
        commentFile.write((comment.toStdString() + "\n\n").c_str());

        ui->commentMessage->setText("Comment added");
        ui->commentMessage->setStyleSheet("QLabel { color : green; }");

        ui->commentEdit->clear();
    }
    else
    {
        ui->commentMessage->setText("Could not add comment");
        ui->commentMessage->setStyleSheet("QLabel { color : red; }");
    }
}

void ProgrammingGradingScreen::on_commentEdit_textChanged()
{
    if (ui->commentEdit->toPlainText() != "")
    {
        ui->commentMessage->clear();
    }
}

void ProgrammingGradingScreen::on_submitButton_clicked()
{
    RubricItemGrade rig;
    Rubric rubricModel;
    QList<QMap<QString, QString> > rubricItems = rubricModel.getRubric(rubricModel.getRubricID(activity["activityID"]));
    for (size_t i = 0; i < ui->rubricListWidget->count(); i++) {
        QListWidgetItem * item = ui->rubricListWidget->item(i);
        RubricItemGradeWidget * rubricItem = (RubricItemGradeWidget *)ui->rubricListWidget->itemWidget(item);

        QMap<QString, QString> gradeItem;
        gradeItem.insert("studentID", session->getStudentID());
        gradeItem.insert("rubricItemID", (rubricItems[i])["rubricItemID"]);
        gradeItem.insert("mark", rubricItem->getGrade());
        std::ostringstream oss;
        oss << session->getUserRole();
        gradeItem.insert("markerPrivilege", oss.str().c_str());
        gradeItem.insert("markerID", session->getUserID().c_str());
        oss.clear();
        oss.str("");
        oss << "Rubric Items " << rubricItemGrades.size();

        if (rubricItemGrades.size() > 0) {
            gradeItem.insert("gradeID", rubricItemGrades[i]["gradeID"]);
            rig.UpdateRubricItemGrade(gradeItem);
        } else {
            rig.InsertRubricItemGrade(gradeItem);
        }
    }
    QMessageBox msgBox;
    msgBox.setText("Marks were submitted.");
    msgBox.exec();
}
