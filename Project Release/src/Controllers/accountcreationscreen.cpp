#include "accountcreationscreen.h"
#include "ui_accountcreationscreen.h"
#include "mainwindow.h"

extern MainWindow *w;
extern Session *session;

AccountCreationScreen::AccountCreationScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AccountCreationScreen)
{
    ui->setupUi(this);
}

AccountCreationScreen::~AccountCreationScreen()
{
    delete ui;
}

void AccountCreationScreen::on_buSave_clicked()
{
    QMap<QString, QString> tmpUser;
    QMessageBox msgBox;
    QString msgBoxString = "Looks like there is a problem with ...\n";
    QStringList invalidInputs;
    bool err;

    // Verify and fill in new user to be inserted
    tmpUser = userModel.createUser();
    if(!User::setUserID(ui->leUserID->text(), &tmpUser, true)) { invalidInputs << "user ID"; }
    if(!User::setPassword(ui->lePassword->text(), &tmpUser, true)) { invalidInputs << "temporary password"; }
    if(!User::setEmployeeID(ui->leEmployeeID->text(), &tmpUser, true)) { invalidInputs << "employee ID"; }
    if(!User::setEmail(ui->leEmail->text(), &tmpUser, true)) { invalidInputs << "email"; }

    if(!User::setFirstName(ui->leFName->text(), &tmpUser, true))   {invalidInputs << "first name"; }
    if(!User::setMiddleName(ui->leMName->text(), &tmpUser, true)) {invalidInputs << "middle name"; }
    if(!User::setLastName(ui->leLName->text(), &tmpUser, true))     {invalidInputs << "last name"; }

    User::setAdministrativeAssistant(ui->chckAdminAssistant->checkState() == Qt::Checked, &tmpUser);
    User::setAdministrator(ui->chckAdministrator->checkState() == Qt::Checked, &tmpUser);
    User::setInstructor(ui->chkInstructor->checkState() == Qt::Checked, &tmpUser);
    User::setSystemAdministrator(ui->chckSystemAdmin->checkState() == Qt::Checked, &tmpUser);
    User::setTeachingAssistant(ui->chckTeachingAssistant->checkState() == Qt::Checked, &tmpUser);

    // Check what went wrong
    if((err = !invalidInputs.isEmpty())) {
        foreach(QString input, invalidInputs) {
            msgBoxString += input + "\n";
        }
    }

    if(!User::verifyRoles(&tmpUser)) {
        err = true;
        msgBoxString += "roles\n";
    }

    // Alert the errors and exit
    if(err) {
        msgBox.setText(msgBoxString);
        msgBox.exec();
        return;
    }

    // No errors, so see if the userid already exists
    if(userModel.pKeyExists(ui->leUserID->text())) {
        msgBox.setText("Sorry, user " + ui->leUserID->text() + " already exists.");
        msgBox.exec();
        return;
    }

    // Still no errors, add the new user
    if(!userModel.insertUser(tmpUser)) {
        msgBox.setText("New user could not be created at this time.");
        msgBox.exec();
        return;
    }

    // Notify successful insert
    msgBox.setText(User::getUserID(&tmpUser) + " has been created!");
    msgBox.exec();
    w->setCentralWidget(new SysAdminHomeScreen);
}

void AccountCreationScreen::on_buCancel_clicked() {
    w->setCentralWidget(new SysAdminHomeScreen);
}

void AccountCreationScreen::on_leFName_textChanged(const QString &arg1) {
    QSize size = ui->leFName->size();
    QFont font(ui->leFName->font());

    if(arg1.isEmpty()) {
        font.setBold(true);
        ui->leFName->setFont(font);
    }
    else {
        font.setBold(false);
        ui->leFName->setFont(font);
    }
    ui->leFName->setFixedSize(size);
}

void AccountCreationScreen::on_leLName_textChanged(const QString &arg1) {
    QSize size = ui->leLName->size();
    QFont font(ui->leLName->font());
    if(arg1.isEmpty()) {
        font.setBold(true);
        ui->leLName->setFont(font);
    }
    else {
        font.setBold(false);
        ui->leLName->setFont(font);
    }
    ui->leLName->setFixedSize(size);
}
