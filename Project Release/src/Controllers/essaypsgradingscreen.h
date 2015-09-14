#ifndef ESSAYPSGRADINGSCREEN_H
#define ESSAYPSGRADINGSCREEN_H

#include <QWidget>
#include <QListWidget>
#include "Helpers/pdfprocessor.h"

#include "Models/activity_model.h"

namespace Ui {
class EssayPSGradingScreen;
}

class EssayPSGradingScreen : public QWidget
{
    Q_OBJECT

public:
    explicit EssayPSGradingScreen(QWidget *parent = 0);
    explicit EssayPSGradingScreen(QMap<QString, QString> activity, QString studentID, QWidget *parent = 0);
    ~EssayPSGradingScreen();

#ifdef USINGPOPPLER
    PDFProcessor submission;
    PDFProcessor solution;
#endif

    //void mouseReleaseEvent(QMouseEvent * event);

    void updateSubmissionScene(QGraphicsScene * scene);
    void updateSolutionScene(QGraphicsScene * scene);
    void updateAnnotationList();
    int getAnnotationIndex(QString annotation);

    void setCommentPosition(int x, int y);

    QMap<QString, QString> activity;
    QMap<QString, QString> activityFile;
    QList<QMap<QString, QString> > rubricItemGrades;
    QString studentID;

private slots:
    void on_submitMarksButton_clicked();

    void on_solutionZoomSpinBox_valueChanged(int zoom);

    void on_submissionZoomSpinBox_valueChanged(int zoom);

    void on_submitCommentButton_clicked();

    void on_xSpinBox_valueChanged(int _x);

    void on_ySpinBox_valueChanged(int _y);

    void on_widthSpinBox_valueChanged(int _width);

    void on_heightSpinBox_valueChanged(int _height);

    void on_removeCommentButton_clicked();

    void on_newCommentButton_clicked();

    void on_cancelButton_clicked();

    void on_commentTextEdit_textChanged();

    void on_commentComboBox_currentIndexChanged(int index);

private:
    Ui::EssayPSGradingScreen *ui;
};

#endif // ESSAYPSGRADINGSCREEN_H
