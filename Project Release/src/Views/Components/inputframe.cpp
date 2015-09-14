#include "inputframe.h"

/**
 * @brief InputFrame::InputFrame
 * You do not have to call this constructor if you used the drag/drop tool.
 * Create a QFrame in the UI editor and promote it to an InputFrame; QT will take care of the rest.
 *
 * Sets up the label and lineedit
 *
 * @param parent; parent QWidget
 */
InputFrame::InputFrame(QWidget *parent)
        : QFrame(parent)
{
    setMouseTracking(true);

    label = new QLabel(this);
    lineEdit = new QLineEdit(this);
    lineEdit->setDisabled((disabled = true));
    label->setBuddy(lineEdit);

    layout = new QGridLayout;
    layout->addWidget(label, 0, 0);
    layout->addWidget(lineEdit, 0, 1);
    this->setLayout(layout);

    InputFrame::gc_setMaxInputSize();
}

/**
 * @brief InputFrame::gc_setMaxInputSize; set the width of the input box
 * @param width; the width..
 */
void InputFrame::gc_setMaxInputSize(int width) {
    this->lineEdit->setMaximumSize(QSize(width, DEFAULT_HEIGHT));
}

/**
 * @brief InputFrame::gc_getLabelString; accessor for the label
 * @return the label string..
 */
QString InputFrame::gc_getLabelString() const {
    return label->text();
}

/**
 * @brief InputFrame::gc_isEmpty; checks for string in the lineedit
 * @return true if the lineedit is empty
 */
bool InputFrame::gc_isEmpty() const {
    return lineEdit->text().length();
}


QString InputFrame::gc_getLabelText() const {
    return lineEdit->text();
}

void InputFrame::gc_setLabelText(QString name) {
    this->label->setText(name);
}

void InputFrame::gc_setPlaceholderText(QString placeholder) {
    this->lineEdit->setPlaceholderText(placeholder);
}

void InputFrame::gc_setText(QString test) {
    this->lineEdit->setText(test);
}

void InputFrame::gc_displayError() {
    // fill me in
    // level: ghetto: msg box
    // level: boss: change style for a second
}

void InputFrame::gc_displaySuccess() {
    // fill me in
    // level: ghetto: msg box
    // level: boss: change style for a second
}

void InputFrame::gc_toggleOnOff() {
    lineEdit->setDisabled((disabled = !disabled));
    if(lineEdit->isEnabled()) {
        lineEdit->selectAll();
        lineEdit->setFocus();
    } else {
        lineEdit->clearFocus();
        QFrame::parentWidget()->activateWindow();
    }

    this->setWindowTitle(gc_getLabelText());
}

void InputFrame::gc_setCompleter(QStringList list) {
    QCompleter *completer = new QCompleter(list);
    completer->setCompletionMode(QCompleter::InlineCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    lineEdit->setCompleter(completer);
}

/**
 * @brief InputFrame::event; overridden event handler callback; using this general one because
 * most tihngs are undetected when the field is disabled
 * @param e event
 * @return
 */
bool InputFrame::event(QEvent *e) {
    if(e->type() == QMouseEvent::MouseButtonDblClick
            //|| e->type() == QMouseEvent::FocusOut
            //|| e->type() == QMouseEvent::NonClientAreaMouseButtonPress
            || (!disabled && e->type() == QMouseEvent::Leave)) {
        gc_toggleOnOff();
    }
    return QFrame::event(e);
}
