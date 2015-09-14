#ifndef NORUBRICACTIVITYSELECTIONSCREEN_H
#define NORUBRICACTIVITYSELECTIONSCREEN_H

#include <QString>
#include <QMap>
#include <QMessageBox>
#include <QListWidget>

namespace Ui {
class NoRubricActivitySelectionScreen;
}

class NoRubricActivitySelectionScreen : public QWidget
{
    Q_OBJECT

public:
    explicit NoRubricActivitySelectionScreen(QWidget *parent = 0);
    ~NoRubricActivitySelectionScreen();

private slots:
    void on_selectButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::NoRubricActivitySelectionScreen *ui;
    QList<QMap<QString, QString> > rubriclessActivities;
};

#endif // NORUBRICACTIVITYSELECTIONSCREEN_H
