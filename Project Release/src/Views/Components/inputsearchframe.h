#ifndef INPUTSEARCHFRAME_H
#define INPUTSEARCHFRAME_H

#include <QFrame>
#include <QLineEdit>
#include <QListWidget>
#include <QVBoxLayout>
#include <QCompleter>
#include "../../Helpers/debug.h"

class InputSearchFrame : public QFrame {

    Q_OBJECT

public:
    InputSearchFrame(QWidget *parent = 0);
    void gc_setAllowedList(QStringList domain, bool refresh = false);
    ~InputSearchFrame() { }

    void gc_setText(const QString& text);
    void gc_setCompleter(QStringList list = QStringList());
    void gc_getText() const;
    int gc_getSelectedIndex() const;
    QString gc_getSelectedRow() const;
    void gc_addItem(QString displayName, int indx = -1);
    void gc_removeItemAt(int i);

public slots:
    void gc_filterListBox(QString prefix);
    void gc_changeSelectedItem(int index);

private:
    void gc_setBoxOptions(QStringList subDomain);

    QLineEdit *textField;
    QListWidget *listBox;
    QVBoxLayout *layout;
    QStringList allowedList;

};
#endif // INPUTSEARCHFRAME_H

