#ifndef MULTIPLECHOICETEST
#define MULTIPLECHOICETEST

#include <QString>
#include <QList>
#include <QMap>
#include "model.h"

/**
 * ClassName: MultipleChoiceTest
 *
 * Description: Handles opening, closing, reading, and writing of csv files.
 *
 * Methods:
 *      MultipleChoiceTest() : Model("MultipleChoiceAnswerKey", "multipleChoiceAnswerKeyID")
 *          Constructor creates a new MultipleChoiceTest and points it to the MultipleChoiceAnswerKey table in the database
 *          with the primary key multipleChoiceAnswerKeyID.
 *      QMap<QString, QString> insertMultipleChoiceTest(QMap<QString, QString> test)
 *          Inserts a new multiple choice test into the database and returns the newly created entry (test map contains
 *          information about the column values for the multiple choice test).
 *      QMap<QString, QString> insertMultipleChoiceTestAnswer(QMap<QString, QString> testAnswer)
 *          Inserts a new multiple choice answer into the database for a particular multiple choice test (testAnswer map
 *          contains information about the column values for the multiple choice test answer (includes the test id it is
 *          associated with).
 *      QList<QString> generateGradeFile(QString _activityID)
 *          Marks the student work for a particular activity (using the activity's pathToWorkToMark attribute) and returns
 *          the contents of a formatted multiple choice test grade file in the form of a list of strings for writing to a file.
 *      QList<QMap<QString, QString> > getTestAnswers(QString _activityID)
 *          Obtains the answers for a particular activity's multiple choice test and returns them as a list of Multiple Choice
 *          Test Answers (stored in a map).
 *      void deleteMCAnswers(QString _activityID)
 *
 * Members:
 *
 * Author: Joshua Campbell and Ching Lam
 *
 * Version: 1
 */
class MultipleChoiceTest : public Model {
public:
    MultipleChoiceTest() : Model("MultipleChoiceAnswerKey", "multipleChoiceAnswerKeyID") {
        params = getEmptyRecord();
    }
    QMap<QString, QString> insertMultipleChoiceTest(QMap<QString, QString> test);
    QMap<QString, QString> insertMultipleChoiceTestAnswer(QMap<QString, QString> testAnswer);

    QList<QString> generateGradeFile(QString _activityID);

    QList<QMap<QString, QString> > getTestAnswers(QString _activityID);
    void deleteMCAnswers(QString _activityID);

    QString studentID;
    QList<QString> answers;

    QMap<QString, QString> params;
};

#endif // MULTIPLECHOICETEST

