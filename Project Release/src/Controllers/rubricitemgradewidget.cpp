#include "rubricitemgradewidget.h"
#include "ui_rubricitemgradewidget.h"
#include "float.h"

RubricItemGradeWidget::RubricItemGradeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RubricItemGradeWidget)
{
    ui->setupUi(this);
    ui->rubricItemGrade->setValidator( new QDoubleValidator(0, DBL_MAX, 2, this) );
}

void RubricItemGradeWidget::setMaximumGrade(double grade)
{
    QString s;
    s.sprintf("%3.2f", grade);
    ui->rubricItemMax->setText(s);
}

void RubricItemGradeWidget::setItemDescription(QString desc)
{
    ui->rubricItemDescription->setPlainText(desc);
}

void RubricItemGradeWidget::setGradeValue(QString grade) {
    ui->rubricItemGrade->setText(grade);
}

QString RubricItemGradeWidget::getGrade() {
    return ui->rubricItemGrade->text();
}

RubricItemGradeWidget::~RubricItemGradeWidget()
{
    delete ui;
}
