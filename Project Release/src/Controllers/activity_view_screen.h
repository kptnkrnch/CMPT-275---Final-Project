#ifndef ACTIVITY_VIEW_SCREEN_H
#define ACTIVITY_VIEW_SCREEN_H

#include <QWidget>
#include <QString>
#include <QMessageBox>
#include <QMap>

namespace Ui {
class Activity_View_Screen;
}

class Activity_View_Screen : public QWidget
{
    Q_OBJECT

public:
    explicit Activity_View_Screen(QWidget *parent = 0);
    ~Activity_View_Screen();

private slots:
    void on_activityListWidget_currentRowChanged(int currentRow);

    void on_backButton_clicked();

private:
    Ui::Activity_View_Screen *ui;
    QList<QMap<QString, QString> > allActivities;
    QList<QMap<QString, QString> > allActivityFiles;
};

#endif // ACTIVITY_VIEW_SCREEN_H
