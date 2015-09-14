#ifndef CONTROLLER_HELPER_H
#define CONTROLLER_HELPER_H

#include <QString>
#include <QMessageBox>

void msgBox(QString title, QString question, QWidget *parent = 0);
bool msgBoxYesNo(QString title, QString question, QWidget *parent = 0);
bool msgBoxCancel(QWidget *parent = 0);

#endif // CONTROLLER_HELPER_H

