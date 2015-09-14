#include "multiplechoicetest_model.h"

#include <sstream>
#include <QMapIterator>

#include "dbaccess.h"
#include "../Helpers/debug.h"
#include "csv_processor.h"
#include "activity_model.h"

extern DBAccess DBA;

/**
 * Method: insertMultipleChoiceTest(QMap<QString, QString> _test)
 *
 * Description:
 *      Inserts a new multiple choice test into the database and returns the newly created entry (test map contains
 *      information about the column values for the multiple choice test).
 *
 * Attributes:
 *      QMap<QString, QString> _test
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
QMap<QString, QString> MultipleChoiceTest::insertMultipleChoiceTest(QMap<QString, QString> _test) {
    QMap<QString, QString> test;
    try {
        test = insertDBItem(_test, true);
    } catch (ParameterException e) {
        DEBUG(DEBUG_WARN, e.what());
    }
    return test;
}

/**
 * Method: insertMultipleChoiceTestAnswer(QMap<QString, QString> testAnswer)
 *
 * Description:
 *      Inserts a new multiple choice answer into the database for a particular multiple choice test (testAnswer map
 *      contains information about the column values for the multiple choice test answer (includes the test id it is
 *      associated with).
 *
 * Attributes:
 *      QMap<QString, QString> testAnswer - the map containing the columns and their values for insertion into the database
 *          (for multiple choice test answers)
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
QMap<QString, QString> MultipleChoiceTest::insertMultipleChoiceTestAnswer(QMap<QString, QString> testAnswer) {
    QMap<QString, QString> test;
    try {
        Model model("MultipleChoiceAnswer", "answerID");
        test = model.insertDBItem(testAnswer, true);
    } catch (ParameterException e) {
        DEBUG(DEBUG_WARN, e.what());
    }
    return test;
}

/**
 * Method: getTestAnswers(QString _activityID)
 *
 * Description:
 *      Obtains the answers for a particular activity's multiple choice test and returns them as a list of Multiple Choice
 *      Test Answers (stored in a map).
 *
 * Attributes:
 *      QString _activityID - the activity to retrieve test answers for
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
QList<QMap<QString, QString> > MultipleChoiceTest::getTestAnswers(QString _activityID) {
    std::ostringstream oss;
    oss << "SELECT MultipleChoiceAnswer.answerID, MultipleChoiceAnswerKey.activityID, MultipleChoiceAnswer.multipleChoiceAnswerKeyID, ";
    oss << "MultipleChoiceAnswer.value, MultipleChoiceAnswer.weight FROM MultipleChoiceAnswer ";
    oss << "JOIN MultipleChoiceAnswerKey ON MultipleChoiceAnswerKey.multipleChoiceAnswerKeyID = MultipleChoiceAnswer.multipleChoiceAnswerKeyID ";
    oss << "WHERE MultipleChoiceAnswerKey.activityID = '" << _activityID.toStdString().c_str() << "';";
    QSqlQuery * query = DBA.__query(oss.str().c_str());
    Model model("MultipleChoiceAnswer", "answerID");
    QList<QMap<QString, QString> > result = model.parseResult(query);
    return result;
}

/**
 * Method: generateGradeFile(QString _activityID)
 *
 * Description:
 *      Marks the student work for a particular activity (using the activity's pathToWorkToMark attribute) and returns
 *      the contents of a formatted multiple choice test grade file in the form of a list of strings for writing to a file.
 *
 * Attributes:
 *      QString _activityID - the activity to get students and their work to mark for
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
QList<QString> MultipleChoiceTest::generateGradeFile(QString _activityID) {
    QList<QString> csv_output;
    Activity activityModel;
    QMap<QString, QString> activityMap = activityModel.GetActivityByID(_activityID);
    activityModel.SetParameters(activityMap);
    QList<QMap<QString, QString> > answerList = getTestAnswers(_activityID);

    CSVProcessor csv_processor;

    csv_processor.openCSV(activityMap["pathToWorkToMark"].toStdString().c_str());
    QMap<QString, QList<QString> > student_work = csv_processor.getMultipleChoiceTest();
    QMapIterator<QString, QList<QString> > workIter(student_work);

    while(workIter.hasNext()) {
        workIter.next();
        std::ostringstream oss;
        oss << workIter.key().toStdString();
        size_t index = 0;
        QList<QString> submission = workIter.value();
        float grade = 0;

        QListIterator<QMap<QString, QString> > answerListIterator(answerList);
        while(answerListIterator.hasNext()) {
            QMap<QString, QString> tempMap = answerListIterator.next();
            QString student_answer = submission[index];


            if (tempMap["value"].toStdString() == student_answer.toStdString()) {
                grade += atof(tempMap["weight"].toStdString().c_str());
            }
            index++;
        }
        oss << "," << grade;
        csv_output.push_back(oss.str().c_str());
    }

    return csv_output;
}

/**
 * Method: deleteMCAnswers(QString _activityID)
 *
 * Description:
 *      Deletes the multiple choice test and its answers for a particular activity
 *
 * Attributes:
 *      QString _activityID - the activity to delete multiple choice answers for
 *
 * Author: Ching Lam
 *
 * Version: 1
 */
void MultipleChoiceTest::deleteMCAnswers(QString _activityID){
    QList<QMap<QString, QString> > answerList = getTestAnswers(_activityID);
    if (answerList.count()>0){
        deleteDBItems("activityID",_activityID);//del from MCAnswersKey table
        QString sqlStr;
        QString valStr;
        //del from MCAnswers table
        QListIterator<QMap<QString, QString> > iter(answerList); //del from MCAnswers table
        while (iter.hasNext()) {
            QMapIterator<QString, QString> jter(iter.next());
            while(jter.hasNext()) {
                jter.next();
                if (jter.key() == "multipleChoiceAnswerKeyID"){
                    valStr += jter.key() + " = ";
                    valStr += "'" + jter.value().trimmed() + "'";
                    break;
                 }
            }
            if(iter.hasNext()) {
                valStr += " OR ";
            }
        }
        sqlStr = "DELETE FROM MultipleChoiceAnswer WHERE " + valStr + ";";
        DBA.__query(sqlStr);
    }
}
