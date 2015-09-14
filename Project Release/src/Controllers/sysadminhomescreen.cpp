#include "sysadminhomescreen.h"
#include "ui_sysadminhomescreen.h"
#include "mainwindow.h"

#include "../Models/session.h"
#include "../Controllers/accountcreationscreen.h"
#include "../Controllers/accountmodifyingscreen.h"
#include "../Controllers/loginscreen.h"

extern MainWindow *w;
extern Session *session;

SysAdminHomeScreen::SysAdminHomeScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysAdminHomeScreen)
{
    ui->setupUi(this);
}

SysAdminHomeScreen::~SysAdminHomeScreen()
{
    delete ui;
}

void SysAdminHomeScreen::on_createButton_clicked()
{
    w->setCentralWidget(new AccountCreationScreen);
}

void SysAdminHomeScreen::on_modifyButton_clicked()
{
    w->setCentralWidget(new AccountModifyingScreen);
}
