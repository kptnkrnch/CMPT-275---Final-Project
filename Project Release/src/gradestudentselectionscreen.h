#ifndef GRADESTUDENTSELECTIONSCREEN_H
#define GRADESTUDENTSELECTIONSCREEN_H

#include <QWidget>

namespace Ui {
class GradeStudentSelectionScreen;
}

class GradeStudentSelectionScreen : public QWidget
{
    Q_OBJECT

public:
    explicit GradeStudentSelectionScreen(QWidget *parent = 0);
    ~GradeStudentSelectionScreen();
    void setFileName(QString _filename);
    void generateGrades();
private slots:
    void on_backButton_clicked();

    void on_continueButton_clicked();

    void on_generateGradesButton_clicked();

private:
    Ui::GradeStudentSelectionScreen *ui;
    QList<QMap<QString, QString> > students;
    QString filename;
};

#endif // GRADESTUDENTSELECTIONSCREEN_H
