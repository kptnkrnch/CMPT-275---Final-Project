#include "controller_helper.h"

bool msgBoxYesNo(QString title, QString question, QWidget *parent) {
    QMessageBox msgBox;
    msgBox.setParent(parent);
    msgBox.setText(title);
    msgBox.setInformativeText(question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    if(msgBox.exec() == QMessageBox::No) {
        return false;
    }
    return true;
}

bool msgBoxCancel(QWidget *parent) {
    return msgBoxYesNo("Cancelling...",
        "You may have some unsaved changes; are you sure you want to discard them?");
}

void msgBox(QString title, QString question, QWidget *parent) {
    QMessageBox msgBox;
    msgBox.setParent(parent);
    msgBox.setText(title);
    msgBox.setInformativeText(question);
    msgBox.exec();
}
