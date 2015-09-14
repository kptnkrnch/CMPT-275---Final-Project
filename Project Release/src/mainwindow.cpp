#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Models/dbaccess.h"
#include "Models/user_model.h"
#include "Models/session.h"
#include "Controllers/roleselectionscreen.h"
#include "Controllers/resetpasswordscreen.h"
#include "Controllers/loginscreen.h"
#include "Controllers/changepasswordscreen.h"
#include "Helpers/pdfprocessor.h"

#include "Helpers/debug.h"

extern DBAccess DBA;
extern DBAccess DBA;
extern Session *session;
MainWindow *w;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    w = this;
    setCentralWidget(new LoginScreen);
#ifdef USINGPOPPLER
    DEBUG(DEBUG_ALRM, "USINGPOPPLER defined in Helpers/pdfprocessor.h");
#endif
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::closeEvent (/* QCloseEvent *event */) {
    Session::logout(&session);
    DBA.disconnect();
}

void MainWindow::on_actionLogout_triggered() {
    Session::logout(&session);
    if (session == 0) {
//        w->menuBar()->actions();
        w->setCentralWidget(new LoginScreen);
    }
}

void MainWindow::on_actionChange_Password_triggered() {
    ChangePasswordScreen *screen = new ChangePasswordScreen;
    screen->setParent(this);
    screen->setAutoFillBackground(true);
    screen->show();
}
