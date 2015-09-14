#ifndef MULTIPLECHOICEGRADINGSCREEN_H
#define MULTIPLECHOICEGRADINGSCREEN_H

#include <QWidget>

namespace Ui {
class MultipleChoiceGradingScreen;
}

class MultipleChoiceGradingScreen : public QWidget
{
    Q_OBJECT

public:
    explicit MultipleChoiceGradingScreen(QWidget *parent = 0);
    ~MultipleChoiceGradingScreen();

private slots:
    void on_backButton_clicked();

    void on_generateGradesButton_clicked();

private:
    Ui::MultipleChoiceGradingScreen *ui;
};

#endif // MULTIPLECHOICEGRADINGSCREEN_H
