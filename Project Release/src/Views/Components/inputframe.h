#ifndef INPUTFRAME_H
#define INPUTFRAME_H

#include <QFrame>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCompleter>
#include <QMouseEvent>

#include <QDebug>

class InputFrame : public QFrame {

public:

    static const int DEFAULT_HEIGHT = 30;
    static const int DEFAULT_WIDTH = 300;

    InputFrame(QWidget *parent = 0);
    QString gc_getLabelString() const;
    bool gc_isEmpty() const;
    QString gc_getLabelText() const;

    void gc_setMaxInputSize(int width = DEFAULT_WIDTH);
    void gc_setLabelText(QString name);
    void gc_setPlaceholderText(QString placeholder);
    void gc_setText(QString test);
    void gc_displayError();
    void gc_displaySuccess();

    ~InputFrame() { }

    void gc_toggleOnOff();
    void gc_setCompleter(QStringList list);

private:
    bool disabled;
    QLabel *label;
    QLineEdit *lineEdit;
    QGridLayout *layout;
    bool event(QEvent *e);
};

#endif // INPUTFRAME_H
