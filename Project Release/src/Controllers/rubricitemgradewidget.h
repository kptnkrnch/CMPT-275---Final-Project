#ifndef RUBRICITEMGRADEWIDGET_H
#define RUBRICITEMGRADEWIDGET_H

#include <QWidget>

namespace Ui {
class RubricItemGradeWidget;
}

class RubricItemGradeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RubricItemGradeWidget(QWidget *parent = 0);
    void setItemDescription(QString desc);
    void setMaximumGrade(double grade);
    void setGradeValue(QString grade);
    QString getGrade();
    ~RubricItemGradeWidget();

private:
    Ui::RubricItemGradeWidget *ui;
};

#endif // RUBRICITEMGRADEWIDGET_H
