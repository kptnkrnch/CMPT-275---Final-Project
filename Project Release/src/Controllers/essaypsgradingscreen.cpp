#include "essaypsgradingscreen.h"
#include "ui_essaypsgradingscreen.h"
#include "../Helpers/debug.h"
#include "rubricitemgradewidget.h"
#include "Models/activity_model.h"
#include "Models/rubric_model.h"
#include "Models/activityfile_model.h"
#include "../Helpers/pdfprocessor.h"
#include "gradestudentselectionscreen.h"
#include "Models/rubricitemgrade_model.h"
#include <QMap>
#include <sstream>
#include <string>
#include <fstream>

#include "mainwindow.h"
#include "../Models/session.h"

#define DEFAULT_COMMENT_WIDTH 2
#define DEFAULT_COMMENT_HEIGHT 2

extern MainWindow * w;
extern Session * session;
/*
EssayPSGradingScreen::EssayPSGradingScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EssayPSGradingScreen)
{
    ui->setupUi(this);
}
*/
EssayPSGradingScreen::EssayPSGradingScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EssayPSGradingScreen)
{
    ui->setupUi(this);
    ui->tabWidget->setStyleSheet("QTabBar::tab { height: 20px; width: 271px; }");

    ui->submissionGraphicsView->setContainer(this);
    Activity activityModel;
    activity = activityModel.GetActivityByID(session->getActivityID());
    studentID = session->getStudentID();
    QList<QMap<QString, QString> > rubric = Activity::getRubric(session->getActivityID());

#ifdef USINGPOPPLER
    ActivityFile activityFileModel;
    QList<QMap<QString, QString> > activityFiles = activityFileModel.getByActivityID(session->getActivityID());

    submission.setWindow(w);
    solution.setWindow(w);
    if (activityFiles.count() > 0) {
        activityFile = activityFiles[0];
        std::ostringstream oss;
        oss << activity["pathToWorkToMark"].toStdString() << "/" << studentID.toStdString() << "/";
        oss << activityFile["fileName"].toStdString() << "." << activityFile["fileType"].toStdString();
        submission.LoadDocument(oss.str().c_str());
    }
    solution.LoadDocument(activity["pathToSolutionFile"]);
    updateAnnotationList();
    updateSubmissionScene(submission.getPDFImage());
    updateSolutionScene(solution.getPDFImage());
#endif
    RubricItemGrade rubricItemGradeModel;

    for (int i = 0; i < rubric.length(); i++)
    {
        QListWidgetItem * item = new QListWidgetItem();
        item->setSizeHint(QSize(item->sizeHint().width(), 48));
        ui->listWidget->addItem(item);
        RubricItemGradeWidget * rubricItem = new RubricItemGradeWidget();


        double grade = rubric[i]["rubricItemValue"].toDouble();
        rubricItem->setMaximumGrade(grade);

        QString desc = rubric[i]["rubricItemText"];
        rubricItem->setItemDescription(desc);

        ui->listWidget->setItemWidget(ui->listWidget->item(i), rubricItem);

        QList<QMap<QString, QString> > rubricItemGrades = rubricItemGradeModel.GetGradesByItemID(rubric[i]["rubricItemID"]);
        QListIterator<QMap<QString, QString> > iterator(rubricItemGrades);
        while (iterator.hasNext()) {
            QMap<QString, QString> temp = iterator.next();
            QMap<QString, QString> data = temp;
            if (temp["studentID"] == session->getStudentID()) {
                rubricItem->setGradeValue(temp["mark"]);
                DEBUG(DEBUG_ALRM, "HERE!!!!");
                this->rubricItemGrades.push_back(data);
                break;
            }
        }
        std::ostringstream temp;
        temp << "Rubric Items: " << this->rubricItemGrades.size();
        DEBUG(DEBUG_ALRM, temp.str().c_str());
    }
    std::ostringstream temp;
    temp << "Rubric Items Final: " << this->rubricItemGrades.size();
    DEBUG(DEBUG_ALRM, temp.str().c_str());

    int height = 48 * rubric.length() + 4;
    if (height > 484)
    {
        height = 484;
    }
    ui->listWidget->setFixedHeight(height);
}

EssayPSGradingScreen::~EssayPSGradingScreen()
{
#ifdef USINGPOPPLER
    if (activity.count() > 0 && activityFile.count() > 0) {
        std::ostringstream oss;
        oss << activity["pathToWorkToMark"].toStdString() << "/" << studentID.toStdString() << "/";
        oss << "~" << activityFile["fileName"].toStdString() << "." << activityFile["fileType"].toStdString();

        if (submission.SaveDocument(oss.str().c_str())) {
            DEBUG(DEBUG_ALRM, "SAVED DOCUMENT");
        } else {
            DEBUG(DEBUG_ALRM, "FAILED TO SAVE DOCUMENT");
        }
    }
    submission.CloseDocument();
    solution.CloseDocument();

    std::ostringstream ossTemp, ossOriginal;
    ossTemp << activity["pathToWorkToMark"].toStdString() << "/" << studentID.toStdString() << "/";
    ossTemp << "~" << activityFile["fileName"].toStdString() << "." << activityFile["fileType"].toStdString();
    ossOriginal << activity["pathToWorkToMark"].toStdString() << "/" << studentID.toStdString() << "/";
    ossOriginal << activityFile["fileName"].toStdString() << "." << activityFile["fileType"].toStdString();

    std::ifstream tempfile;
    std::ofstream originalFile;
    tempfile.open(ossTemp.str().c_str(), std::ios::binary);
    originalFile.open(ossOriginal.str().c_str(), std::ios::binary);

    if (tempfile.is_open() && originalFile.is_open()) {
        originalFile << tempfile.rdbuf();
        tempfile.close();
        originalFile.close();
        remove(ossTemp.str().c_str());
    }
#endif
    QGraphicsScene * temp = ui->submissionGraphicsView->scene();
    if (temp != 0) {
        delete temp;
    }
    temp = ui->solutionGraphicsView->scene();
    if (temp != 0) {
        delete temp;
    }
    delete ui;
}

void EssayPSGradingScreen::on_submitMarksButton_clicked() {
    RubricItemGrade rig;
    Rubric rubricModel;
    QList<QMap<QString, QString> > rubricItems = rubricModel.getRubric(rubricModel.getRubricID(activity["activityID"]));
    for (size_t i = 0; i < ui->listWidget->count(); i++) {
        QListWidgetItem * item = ui->listWidget->item(i);
        RubricItemGradeWidget * rubricItem = (RubricItemGradeWidget *)ui->listWidget->itemWidget(item);
        DEBUG(DEBUG_ALRM, rubricItem->getGrade());
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
        DEBUG(DEBUG_ALRM, oss.str().c_str());
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

void EssayPSGradingScreen::on_solutionZoomSpinBox_valueChanged(int zoom) {
#ifdef USINGPOPPLER
    double percent = (double)zoom / 100;
    solution.setZoomPercent(percent);
    updateSolutionScene(solution.getPDFImage());
#endif
}

void EssayPSGradingScreen::updateSubmissionScene(QGraphicsScene * scene) {
#ifdef USINGPOPPLER
    QGraphicsScene * temp = ui->submissionGraphicsView->scene();
    delete temp;
    ui->submissionGraphicsView->setScene(scene);

    /*if (activity.count() > 0 && activityFile.count() > 0) {
        std::ostringstream oss;
        oss << activity["pathToWorkToMark"].toStdString() << "/" << session->getStudentID().toStdString() << "/";
        oss << "~" << activityFile["fileName"].toStdString() << "." << activityFile["fileType"].toStdString();

        if (submission.SaveDocument(oss.str().c_str())) {
            DEBUG(DEBUG_ALRM, "SAVED DOCUMENT");
        } else {
            DEBUG(DEBUG_ALRM, "FAILED TO SAVE DOCUMENT");
        }
    }*/
#endif
}

void EssayPSGradingScreen::updateSolutionScene(QGraphicsScene * scene) {
#ifdef USINGPOPPLER
    QGraphicsScene * temp = ui->solutionGraphicsView->scene();
    delete temp;
    ui->solutionGraphicsView->setScene(scene);
#endif
}

void EssayPSGradingScreen::on_submissionZoomSpinBox_valueChanged(int zoom) {
#ifdef USINGPOPPLER
    double percent = (double)zoom / 100;
    submission.setZoomPercent(percent);
    updateSubmissionScene(submission.getPDFImage());
#endif
}

void EssayPSGradingScreen::updateAnnotationList() {
#ifdef USINGPOPPLER
    for (size_t i = ui->commentComboBox->count() - 1; i > 0; i--) {
        ui->commentComboBox->removeItem(i);
    }
    QList<Poppler::Annotation * > list = submission.getAnnotations();
    QListIterator<Poppler::Annotation * > iter(list);
    int index = 0;
    while(iter.hasNext()) {
        Poppler::Annotation * temp = iter.next();
        std::ostringstream oss;
        oss << index << " - " << temp->contents().toStdString().substr(0, 20);
        ui->commentComboBox->addItem(oss.str().c_str());
        index++;
    }

    if (activity.count() > 0 && activityFile.count() > 0) {
        std::ostringstream oss;
        oss << activity["pathToWorkToMark"].toStdString() << "/" << session->getStudentID().toStdString() << "/";
        oss << "~" << activityFile["fileName"].toStdString() << "." << activityFile["fileType"].toStdString();

        if (submission.SaveDocument(oss.str().c_str())) {
            DEBUG(DEBUG_ALRM, "SAVED DOCUMENT");
        } else {
            DEBUG(DEBUG_ALRM, "FAILED TO SAVE DOCUMENT");
        }
    }
#endif
}

void EssayPSGradingScreen::on_submitCommentButton_clicked() {
#ifdef USINGPOPPLER
    double max = 100;
    double x = (double)ui->xSpinBox->value() / max;
    double y = (double)ui->ySpinBox->value() / max;
    double width = (double)ui->widthSpinBox->value() / max;
    double height = (double)ui->heightSpinBox->value() / max;
    QString text = ui->commentTextEdit->toPlainText();
    Poppler::Annotation * annotation = submission.CreateAnnotation(text, x, y, width, height);
    submission.AddAnnotation(annotation);
    updateSubmissionScene(submission.getPDFImage());
    DEBUG(DEBUG_ALRM, submission.getAnnotations().count());
    updateAnnotationList();
    ui->commentComboBox->setCurrentIndex(ui->commentComboBox->count() - 1);
#endif
}

int EssayPSGradingScreen::getAnnotationIndex(QString annotation) {
    int index = atoi(annotation.toStdString().substr(0,1).c_str());
    return index;
}

void EssayPSGradingScreen::on_xSpinBox_valueChanged(int _x) {
#ifdef USINGPOPPLER
    if (ui->commentComboBox->currentIndex() != 0) {
        int index = getAnnotationIndex(ui->commentComboBox->currentText());
        double max = 100;
        double x = (double)_x / max;

        QList<Poppler::Annotation * > list = submission.getAnnotations();
        submission.ModifyAnnotation(list[index], list[index]->contents(), x, list[index]->boundary().y(), list[index]->boundary().width(), list[index]->boundary().height());

        int hor = ui->submissionGraphicsView->getHorizontalScrollBarPosition();
        int vert = ui->submissionGraphicsView->getVerticalScrollBarPosition();

        updateSubmissionScene(submission.getPDFImage());

        ui->submissionGraphicsView->setScrollPosition(hor, vert);
    }
#endif
}

void EssayPSGradingScreen::on_ySpinBox_valueChanged(int _y) {
#ifdef USINGPOPPLER
    if (ui->commentComboBox->currentIndex() != 0) {
        int index = getAnnotationIndex(ui->commentComboBox->currentText());
        double max = 100;
        double y = (double)_y / max;

        QList<Poppler::Annotation * > list = submission.getAnnotations();
        submission.ModifyAnnotation(list[index], list[index]->contents(), list[index]->boundary().x(), y, list[index]->boundary().width(), list[index]->boundary().height());

        int hor = ui->submissionGraphicsView->getHorizontalScrollBarPosition();
        int vert = ui->submissionGraphicsView->getVerticalScrollBarPosition();

        updateSubmissionScene(submission.getPDFImage());

        ui->submissionGraphicsView->setScrollPosition(hor, vert);
    }
#endif
}

void EssayPSGradingScreen::on_widthSpinBox_valueChanged(int _width) {
#ifdef USINGPOPPLER
    if (ui->commentComboBox->currentIndex() != 0) {
        int index = getAnnotationIndex(ui->commentComboBox->currentText());
        double max = 100;
        double width = (double)_width / max;

        QList<Poppler::Annotation * > list = submission.getAnnotations();
        submission.ModifyAnnotation(list[index], list[index]->contents(), list[index]->boundary().x(), list[index]->boundary().y(), width, list[index]->boundary().height());

        int hor = ui->submissionGraphicsView->getHorizontalScrollBarPosition();
        int vert = ui->submissionGraphicsView->getVerticalScrollBarPosition();

        updateSubmissionScene(submission.getPDFImage());

        ui->submissionGraphicsView->setScrollPosition(hor, vert);
    }
#endif
}

void EssayPSGradingScreen::on_heightSpinBox_valueChanged(int _height) {
#ifdef USINGPOPPLER
    if (ui->commentComboBox->currentIndex() != 0) {
        int index = getAnnotationIndex(ui->commentComboBox->currentText());
        double max = 100;
        double height = (double)_height / max;

        QList<Poppler::Annotation * > list = submission.getAnnotations();
        submission.ModifyAnnotation(list[index], list[index]->contents(), list[index]->boundary().x(), list[index]->boundary().y(), list[index]->boundary().width(), height);

        int hor = ui->submissionGraphicsView->getHorizontalScrollBarPosition();
        int vert = ui->submissionGraphicsView->getVerticalScrollBarPosition();

        updateSubmissionScene(submission.getPDFImage());

        ui->submissionGraphicsView->setScrollPosition(hor, vert);
    }
#endif
}

void EssayPSGradingScreen::on_removeCommentButton_clicked() {
#ifdef USINGPOPPLER
    if (ui->commentComboBox->currentIndex() != 0) {
        int index = getAnnotationIndex(ui->commentComboBox->currentText());
        QList<Poppler::Annotation * > list = submission.getAnnotations();
        submission.RemoveAnnotation(list[index]);
        updateSubmissionScene(submission.getPDFImage());
        updateAnnotationList();
        ui->commentComboBox->setCurrentIndex(0);
    }
#endif
}

void EssayPSGradingScreen::on_newCommentButton_clicked() {
#ifdef USINGPOPPLER
    ui->commentComboBox->setCurrentIndex(0);
    ui->commentTextEdit->setText("");
    ui->xSpinBox->setValue(0);
    ui->ySpinBox->setValue(0);
    ui->widthSpinBox->setValue(DEFAULT_COMMENT_WIDTH);
    ui->heightSpinBox->setValue(DEFAULT_COMMENT_HEIGHT);
#endif
}

void EssayPSGradingScreen::on_cancelButton_clicked()
{
    w->setCentralWidget(new GradeStudentSelectionScreen);
}

void EssayPSGradingScreen::on_commentTextEdit_textChanged() {
#ifdef USINGPOPPLER
    if (ui->commentComboBox->currentIndex() != 0) {
        int index = getAnnotationIndex(ui->commentComboBox->currentText());
        QList<Poppler::Annotation * > list = submission.getAnnotations();
        std::string temp = ui->commentTextEdit->toPlainText().toStdString();
        int num_chars = 0;
        int num_newlines = 0;
        int max_chars = 0;
        for (size_t i = 0; i < temp.length(); i++) {
            if (temp[i] == '\n') {
                num_newlines++;
                num_chars = 0;
            } else {
                num_chars++;
                if (num_chars > max_chars) {
                    max_chars = num_chars;
                }
            }
        }
        ui->widthSpinBox->setValue(DEFAULT_COMMENT_WIDTH + (int)((float)max_chars/1.6));
        ui->heightSpinBox->setValue(DEFAULT_COMMENT_HEIGHT + num_newlines);
        submission.ModifyAnnotation(list[index], ui->commentTextEdit->toPlainText(), list[index]->boundary().x(), list[index]->boundary().y(), list[index]->boundary().width(), list[index]->boundary().height());

        int hor = ui->submissionGraphicsView->getHorizontalScrollBarPosition();
        int vert = ui->submissionGraphicsView->getVerticalScrollBarPosition();

        updateSubmissionScene(submission.getPDFImage());

        ui->submissionGraphicsView->setScrollPosition(hor, vert);
    }
#endif
}

void EssayPSGradingScreen::on_commentComboBox_currentIndexChanged(int index) {
#ifdef USINGPOPPLER
    if (ui->commentComboBox->currentIndex() != 0) {
        int index = getAnnotationIndex(ui->commentComboBox->currentText());
        QList<Poppler::Annotation * > list = submission.getAnnotations();
        ui->commentTextEdit->setText(list[index]->contents());
        ui->xSpinBox->setValue((int)((list[index]->boundary().x() * 100)));
        ui->ySpinBox->setValue((int)((list[index]->boundary().y() * 100)));
        ui->widthSpinBox->setValue((int)((list[index]->boundary().width() * 100)));
        ui->heightSpinBox->setValue((int)((list[index]->boundary().height() * 100)));
    }
#endif
}

void EssayPSGradingScreen::setCommentPosition(int x, int y) {
    ui->xSpinBox->setValue(x);
    ui->ySpinBox->setValue(y);
}
