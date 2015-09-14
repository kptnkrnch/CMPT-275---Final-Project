#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <QMessageBox>
#include <QSize>
#include <QRect>
#include <QVariant>
#include <QMainWindow>
#include <QResizeEvent>
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setScreen(MainWindow*, QWidget*);
    void executeLogin();

    //void resizeEvent(QResizeEvent * event);
    void closeEvent(/* QCloseEvent *event */);

public slots:
    void on_actionLogout_triggered();

private slots:
    void on_actionChange_Password_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
