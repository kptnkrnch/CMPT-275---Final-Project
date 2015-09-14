#include "gradefilenamedialog.h"
#include "ui_gradefilenamedialog.h"

#include <string>

GradeFileNameDialog::GradeFileNameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GradeFileNameDialog)
{
    ui->setupUi(this);
}

GradeFileNameDialog::~GradeFileNameDialog()
{
    delete ui;
}

void GradeFileNameDialog::on_buttonBox_accepted()
{
    std::string temp = ui->lineEdit->text().toStdString() + ".csv";
    gsss->setFileName(temp.c_str());
    gsss->generateGrades();
    this->close();
}

void GradeFileNameDialog::on_buttonBox_rejected()
{
    this->close();
}

void GradeFileNameDialog::setGradeStudentSelectionScreen(GradeStudentSelectionScreen *_gsss) {
    gsss = _gsss;
}
