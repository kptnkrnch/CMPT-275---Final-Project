#include "ui_loginscreen.h"
#include "mainwindow.h"

#include "../Models/user_model.h"
#include "../Models/dbaccess.h"
#include "../Controllers/loginscreen.h"
#include "../Controllers/roleselectionscreen.h"
#include "../Exceptions/loginexception.h"
#include "../Exceptions/userexception.h"
#include "resetpasswordscreen.h"
#include "essaypsgradingscreen.h"

#include "../Helpers/debug.h"

extern DBAccess DBA;
extern MainWindow *w;

LoginScreen::LoginScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginScreen)
{
    ui->setupUi(this);
}

void LoginScreen::executeLogin() {
    QString username;
    QString password;
    QString message;
    QMessageBox msgBox;
    QMap<QString, QString> user;
    bool loginSuccess = false;

    username = ui->usernameEdit->text();
    password = ui->passwordEdit->text();

    // Logging in
    try {
        user = User::login(username, password);
        message = "Welcome " + username + ",\nyou have been successfully logged in.";
        loginSuccess = true;
    } catch (LoginException e) {
        DEBUG(DEBUG_ALRM, e.what());
        message = "Username or password is incorrect.";
    } catch (UserException e) {
        DEBUG(DEBUG_ALRM, e.what());
        message = "This account is currently blocked. Please contact your system administrator for assistance.";
    }

    // Alert user success or error
    msgBox.setText(message);
    if(!loginSuccess) {
        msgBox.exec();
    }

    // Exit if couldn't login
    if (!DBA.isConnected() || !loginSuccess) {
        return;
    }

    // Prepare role box
    RoleSelectionScreen *selectionScreen = new RoleSelectionScreen;

    // Force redirect to reset password if needed
    if (User::isPwdResetNeeded(&user)) {
        ResetPasswordScreen *resetScreen = new ResetPasswordScreen(user, selectionScreen);
        w->setCentralWidget(resetScreen);
        return;
    }


//    w->menuBar()->actions().at(1)->setEnabled(true);
    w->setCentralWidget(selectionScreen);
}

LoginScreen::~LoginScreen() {
    delete ui;
}

void LoginScreen::on_usernameEdit_returnPressed() {
    executeLogin();
}

void LoginScreen::on_passwordEdit_returnPressed()
{

    executeLogin();
}

void LoginScreen::on_loginButton_clicked()
{
    executeLogin();
}
