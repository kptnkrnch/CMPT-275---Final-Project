#include "inputsearchframe.h"

/**
 * @brief InputSearchFrame::InputSearchFrame
 * You don't have to use this constructor if you used the UI drag/drop tool.
 * Create a QFrame, right click, promote to InputSearchFrame, QT will automatically
 * make the call to this constructor in some secret build file.
 *
 * This sets up the editbox and the listbox. You still have to call setAllowList to
 * populate the listbox with items.
 *
 * @param parent parent QWidget
 */
InputSearchFrame::InputSearchFrame(QWidget *parent)
        : QFrame(parent) {

    textField = new QLineEdit(this);
    listBox = new QListWidget(this);

    layout = new QVBoxLayout;
    layout->addWidget(textField);
    layout->addWidget(listBox);
    this->setLayout(layout);

    connect(textField, SIGNAL(textChanged(QString)), this, SLOT(gc_filterListBox(QString)));
    connect(listBox, SIGNAL(currentRowChanged(int)), this, SLOT(gc_changeSelectedItem(int)));
}

/**
 * @brief InputSearchFrame::changeSelectedItem; this callback gets triggered when the user clicks on an item
 * from the listbox.
 * @param index; the index of the clicked element
 */
void InputSearchFrame::gc_changeSelectedItem(int index) {
    this->setWindowTitle(allowedList.at(index));
}

int InputSearchFrame::gc_getSelectedIndex() const {
    return this->listBox->currentRow();
}

QString InputSearchFrame::gc_getSelectedRow() const {
    return allowedList.at(gc_getSelectedIndex());
    //return this->windowTitle();
}


/**
 * @brief InputSearchFrame::setCompleter; creates an autocomplete for the lineedit, it attempts to find a match from list
 * @param list; the list to match from, if not provided will use the allowedList
 */
void InputSearchFrame::gc_setCompleter(QStringList list) {
    if(list.isEmpty()) {
        list = allowedList;
    }
    QCompleter *completer = new QCompleter(list);
    completer->setCompletionMode(QCompleter::InlineCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    textField->setCompleter(completer);
}

/**
 * @brief InputSearchFrame::setAllowedList; sets the list of all possible values in the listbox and may optionally
 * refresh the listbox with the new items
 * @param domain; the list of all possible values for the listbox
 * @param refresh; default is true - if true the listbox will refresh after setting the allowed values
 */
void InputSearchFrame::gc_setAllowedList(QStringList domain, bool refresh) {
    allowedList = domain;
    if(refresh) {
        gc_setBoxOptions(allowedList);
    }
}

/**
 * @brief InputSearchFrame::setBoxOptions; typically called when refreshing the list of items to be showin in the listbox
 * while the user types on the lineedit to filter results. Can be used to display and list in the listbox.
 * @param subDomain; the items to display
 */
void InputSearchFrame::gc_setBoxOptions(QStringList subDomain) {
    listBox->clear();
    foreach (QString tmp, subDomain) {
        listBox->addItem(tmp);
    }
}

void InputSearchFrame::gc_removeItemAt(int i) {
    listBox->takeItem(i);
}

/**
 * @brief InputSearchFrame::gcc_addItem; adds string to the listbox
 * @param displayName; label to add
 * @param indx; if provided, will insert at that spot, else will append
 */
void InputSearchFrame::gc_addItem(QString displayName, int indx) {
    if(indx == -1) {
        listBox->addItem(displayName);
        return;
    }
    listBox->insertItem(indx, displayName);
}


/**
 * @brief InputSearchFrame::filterListBox; displays filtered items that start with prefix into the listbox
 * @param prefix; the prefix to match for
 */
void InputSearchFrame::gc_filterListBox(QString prefix) {
    // show all results when there is no specified prefix
    if(prefix.isEmpty()) {
        gc_setBoxOptions(allowedList);
        return;
    }
    QRegularExpression regExp(prefix+"*");
    QStringList newList = allowedList.filter(regExp);
    gc_setBoxOptions(newList);
}

/**
 * @brief InputSearchFrame::getText; accessor for what is typed in the lineedit
 */
void InputSearchFrame::gc_getText() const {
    textField->text();
}

/**
 * @brief InputSearchFrame::setText; mutator to modify what is typed in the lineedit
 * @param text; text to change to
 */
void InputSearchFrame::gc_setText(const QString& text) {
    textField->setText(text);
}
