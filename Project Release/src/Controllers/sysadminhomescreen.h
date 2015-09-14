#ifndef SYSADMINHOMESCREEN_H
#define SYSADMINHOMESCREEN_H

#include <QWidget>

namespace Ui {
class SysAdminHomeScreen;
}

class SysAdminHomeScreen : public QWidget
{
    Q_OBJECT

public:
    explicit SysAdminHomeScreen(QWidget *parent = 0);
    ~SysAdminHomeScreen();

private slots:
    void on_createButton_clicked();

    void on_modifyButton_clicked();

private:
    Ui::SysAdminHomeScreen *ui;
};

#endif // SYSADMINHOMESCREEN_H
