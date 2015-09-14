#include "roleselectionscreen.h"

extern MainWindow *w;
extern Session *session;

RoleSelectionScreen::RoleSelectionScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoleSelectionScreen)
{
    ui->setupUi(this);

    QStringList roles;
    QMap<QString, QString> user;
    try {
        User userModel;
        user =  userModel.getUser(QString::fromStdString(session->getUserID()));
    } catch (ParameterException& e) {
        DEBUG(DEBUG_WARN, e.what());
    } catch (DBGetException& e) {
        DEBUG(DEBUG_WARN, e.what());
    }

    // Prepare new screen roles
    if (User::isSystemAdministrator(&user)) {
        roles << "System Administrator";
    }
    if (User::isAdministrativeAssistant(&user)) {
        roles << "Administrative Assistant";
    }
    if (User::isAdministrator(&user)) {
        roles << "Administrator";
    }
    if (User::isInstructor(&user)) {
        roles << "Instructor";
    }
    if (User::isTeachingAssistant(&user)) {
        roles << "Teaching Assistant";
    }

    foreach(QString role, roles) {
        ui->comboBox->addItem(role);
    }

    ui->laWelcome->setText("Welcome, " + User::getFirstName(&user) + "!");
}

RoleSelectionScreen::~RoleSelectionScreen() {
    delete ui;
}

void RoleSelectionScreen::on_pushButton_clicked() {
    QString role = ui->comboBox->currentText();
    if (role == "System Administrator") {
        session->SetRole(User::ROLE_SYSTEMADMINISTRATOR);
        w->setCentralWidget(new SysAdminHomeScreen);
    }
    else if (role == "Administrative Assistant") {
        session->SetRole(User::ROLE_ADMINISTRATIVEASSISTANT);
        w->setCentralWidget(new AdminAssistantHomeScreen);
    }
    else if (role == "Administrator") {
        session->SetRole(User::ROLE_ADMINISTRATOR);
        w->setCentralWidget(new CourseSelectionScreen);
    }
    else if (role == "Instructor") {
        session->SetRole(User::ROLE_INSTRUCTOR);
        w->setCentralWidget(new CourseSelectionScreen);
    }
    else if (role == "Teaching Assistant") {
        session->SetRole(User::ROLE_TEACHINGASSISTANT);
        w->setCentralWidget(new CourseSelectionScreen);
    }
}

void RoleSelectionScreen::on_changePassword_clicked() {

    ChangePasswordScreen * screen = new ChangePasswordScreen(this);

    screen->setAutoFillBackground(true);

    screen->show();
}
