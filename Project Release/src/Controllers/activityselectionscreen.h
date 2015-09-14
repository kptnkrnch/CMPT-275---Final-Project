#ifndef ACTIVITYSELECTIONSCREEN_H
#define ACTIVITYSELECTIONSCREEN_H

#include <QString>
#include <QMap>
#include <QMessageBox>
#include <QListWidget>

namespace Ui {
class ActivitySelectionScreen;
}

class ActivitySelectionScreen : public QWidget
{
    Q_OBJECT

public:
    explicit ActivitySelectionScreen(QWidget *parent = 0);
    ~ActivitySelectionScreen();

private slots:
    void on_selectActivityButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::ActivitySelectionScreen *ui;
    QList<QMap<QString, QString> > allActivities;
};

#endif // ACTIVITYSELECTIONSCREEN_H
