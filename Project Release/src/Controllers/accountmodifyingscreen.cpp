#include "accountmodifyingscreen.h"
#include "ui_accountmodifyingscreen.h"
#include "mainwindow.h"

#include "../Models/user_model.h"
#include "../Models/session.h"
#include "../Views/Components/inputframe.h"
#include "../Views/Components/inputsearchframe.h"
#include "../Helpers/controller_helper.h"

#include "loginscreen.h"
#include "sysadminhomescreen.h"

extern MainWindow *w;
extern Session *session;

AccountModifyingScreen::AccountModifyingScreen(QWidget *parent)
        : QWidget(parent), ui(new Ui::AccountModifyingScreen) {

    ui->setupUi(this);

    allUsers = userModel.getAllUsers();
    currentUser = new QMap<QString, QString>();
    int count = allUsers.count();
    for( int i = 0; i < count; ++i ){
        *currentUser = allUsers.at(i);
        allUserIDs.append(User::getUserID(currentUser));
    }

    ui->liUsers->addItems(allUserIDs);
    if(allUserIDs.size() > 0) {
        ui->liUsers->setCurrentRow(0);
    }
}

void AccountModifyingScreen::on_buSave_clicked()
{
    QMap<QString, QString> tmpUser;
    QMessageBox msgBox;
    QString msgBoxString = "Looks like there is a problem with ...\n";
    QStringList invalidInputs;
    QString currentUserID, newUserID;

    // Verify and fill in new user to be inserted
    currentUserID = ui->liUsers->currentItem()->text();
    newUserID = ui->leUserID->text();

    try {
        tmpUser = userModel.getUser(currentUserID);
    } catch(DBGetException& e) {
        DEBUG(DEBUG_WARN, e.what());
        msgBox.setText("Coult not update at this time. (" + User::getUserID(&tmpUser) + ")");
        msgBox.exec();
        refresh();
    }

    if(!User::setUserID(newUserID, &tmpUser, true)) { invalidInputs << "user ID"; }
    if(!User::setPassword(ui->lePassword->text(), &tmpUser, true)) { invalidInputs << "temporary password"; }
    if(!User::setEmployeeID(ui->leEmployeeID->text(), &tmpUser, true)) { invalidInputs << "employee ID"; }
    if(!User::setEmail(ui->leEmail->text(), &tmpUser, true))  { invalidInputs << "email"; }

    if(!User::setFirstName(ui->leFName->text(), &tmpUser, true))   {invalidInputs << "first name"; }
    if(!User::setMiddleName(ui->leMName->text(), &tmpUser, true)) {invalidInputs << "middle name"; }
    if(!User::setLastName(ui->leLName->text(), &tmpUser, true))     {invalidInputs << "last name"; }

    User::setAdministrativeAssistant(ui->chckAdminAssistant->checkState() == Qt::Checked, &tmpUser);
    User::setAdministrator(ui->chckAdministrator->checkState() == Qt::Checked, &tmpUser);
    User::setInstructor(ui->chkInstructor->checkState() == Qt::Checked, &tmpUser);
    User::setSystemAdministrator(ui->chckSystemAdmin->checkState() == Qt::Checked, &tmpUser);
    User::setTeachingAssistant(ui->chckTeachingAssistant->checkState() == Qt::Checked, &tmpUser);
    if(!User::verifyRoles(&tmpUser)) { invalidInputs << "roles"; }
    User::setBlocked(ui->raBlocked->isChecked(), &tmpUser);

    // Check what went wrong
    if(!invalidInputs.isEmpty()) {
        foreach(QString input, invalidInputs) {
            msgBoxString += input + "\n";
        }
        msgBox.setText(msgBoxString);
        msgBox.exec();
        return;

    }

    // Still no errors, update the user
    if(newUserID != currentUserID
            && !userModel.updateUserID(currentUserID, User::getUserID(&tmpUser))) {
        DEBUG(DEBUG_WARN, "UserID update failed");
        msgBox.setText("Coult not update at this time. (" + User::getUserID(&tmpUser) + ")");
        msgBox.exec();
        refresh();
    }

    if(!userModel.updateUser(tmpUser)) {
        DEBUG(DEBUG_WARN, "User update failed");
        msgBox.setText("Coult not update at this time. (" + User::getUserID(&tmpUser) + ")");
        msgBox.exec();
        refresh();
    }

    msgBox.setText("User (" + User::getUserID(&tmpUser) + ") has been updated");
    msgBox.exec();
    w->setCentralWidget(new AccountModifyingScreen);
}

void AccountModifyingScreen::on_buCancel_clicked() {
    w->setCentralWidget(new SysAdminHomeScreen);
}

void AccountModifyingScreen::on_buDelete_clicked() {
    QString currentUserID = ui->liUsers->currentItem()->text();

    // If deleting own account
    if(session->getUserID() == currentUserID.toStdString()) {
        msgBox("Deleting your account!",
                    "You may not delete your own account!");
        return;
    }

    // Scrub account
    if(!msgBoxYesNo("Deleting..",
                    "The user will be removed as an Instructor and Teaching Assistant from any courses he has been assigned. All records of this user will be lost.\n\nAre you sure you want to delete "
                    + currentUserID + "?")) {
        return;
    }

    if(!userModel.deleteUser(currentUserID)) {
        msgBox("Hmmm...", "Couldn't delete at this time. Please try again later.");
    }
    msgBox("Deleted", currentUserID + " has been deleted.");
    w->setCentralWidget(new SysAdminHomeScreen);
}

void AccountModifyingScreen::refresh() {
    w->setCentralWidget(new AccountModifyingScreen);
}

void AccountModifyingScreen::on_liUsers_currentRowChanged(int currentRow) {
    *currentUser = allUsers.at(currentRow);
    setKnownFields(currentUser);
}

void AccountModifyingScreen::setKnownFields(QMap<QString, QString> *user) {

    ui->leUserID->setText(User::getUserID(user));
    ui->leEmployeeID->setText(User::getEmployeeID(user));
    ui->lePassword->setText(User::getPassword(user));
    ui->leEmail->setText(User::getEmail(user));

    ui->leFName->setText(User::getFirstName(user));
    ui->leMName->setText(User::getMiddleName(user));
    ui->leLName->setText(User::getLastName(user));

    ui->chckAdminAssistant->setChecked(User::isAdministrativeAssistant(user));
    ui->chckAdministrator->setChecked(User::isAdministrator(user));
    ui->chckSystemAdmin->setChecked(User::isSystemAdministrator(user));
    ui->chckTeachingAssistant->setChecked(User::isTeachingAssistant(user));
    ui->chkInstructor->setChecked(User::isInstructor(user));

    if(User::isBlocked(user)) {
        ui->raBlocked->setChecked(true);
    } else {
        ui->raUnblocked->setChecked(true);
    }
}

AccountModifyingScreen::~AccountModifyingScreen() {
    delete currentUser;
    delete ui;
}

bool AccountModifyingScreen::msgBoxYesNo(QString text, QString informativeText) {
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.setInformativeText(informativeText);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    if(msgBox.exec() == QMessageBox::No) {
        return false;
    }
    return true;
}

void AccountModifyingScreen::on_leFName_textChanged(const QString &arg1) {
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

void AccountModifyingScreen::on_leLName_textChanged(const QString &arg1) {
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

void AccountModifyingScreen::on_buRevert_clicked() {
    if(!msgBoxYesNo("Reverting..",
                    "Any unsaved changes will be lost. Are you sure you want to revert?")) {
        return;
    }
    setKnownFields(currentUser);
}
