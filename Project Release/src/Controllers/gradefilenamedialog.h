#ifndef GRADEFILENAMEDIALOG_H
#define GRADEFILENAMEDIALOG_H

#include <QDialog>
#include "gradestudentselectionscreen.h"

namespace Ui {
class GradeFileNameDialog;
}

class GradeFileNameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GradeFileNameDialog(QWidget *parent = 0);
    ~GradeFileNameDialog();

    void setGradeStudentSelectionScreen(GradeStudentSelectionScreen * _gsss);

    GradeStudentSelectionScreen * gsss;
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::GradeFileNameDialog *ui;
};

#endif // GRADEFILENAMEDIALOG_H
