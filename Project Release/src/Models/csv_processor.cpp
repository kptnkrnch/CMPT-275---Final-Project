#include "csv_processor.h"

#include <QListIterator>
#include "Helpers/debug.h"

CSVProcessor::CSVProcessor() {

}

CSVProcessor::~CSVProcessor() {
    if (file.is_open()) {
        file.close();
    }
}

CSVProcessor::CSVProcessor(std::string csvFile) {
//    file = std::fstream(csvFile.c_str());
    file.open(csvFile.c_str());
}


// ALWAYS returned false with my tests
bool CSVProcessor::openCSV(std::string _file) {
    file.open(_file.c_str());
    return file.is_open();
}

bool CSVProcessor::closeCSV() {
    if (file.is_open()) {
        file.close();
    }
    return !(file.is_open());
}

QList<QMap<QString, QString> > CSVProcessor::getStudentList() {
    QList<QString> parameters;
    parameters.push_back("firstName");
    parameters.push_back("middleName");
    parameters.push_back("lastName");
    parameters.push_back("studentID");
    QList<QMap<QString, QString> > csv;
    std::string line = "";

    if (file.is_open()) {
        while(std::getline(file, line)) {
            QMap<QString, QString> map;
            QListIterator<QString> iter(parameters);
            size_t i = 0;
            while(iter.hasNext()) {
                QString curParam = iter.next();
                std::string temp = "";
                for (;i < line.length(); i++) {
                    if (line.at(i) != ',') {
                        temp += line.at(i);
                    } else {
                        i++;
                        break;
                    }
                }
                map.insert(curParam, temp.c_str());
            }
            csv.push_back(map);
        }
    }

    return csv;
}

QMap<QString, QList<QString> > CSVProcessor::getMultipleChoiceTest() {
    QMap<QString, QList<QString> > data;

    std::string line = "";

    if (file.is_open()) {
        while(std::getline(file, line)) {
            QList<QString> answers;
            size_t i = 0;
            std::string studentID = "";
            for (;i < line.length(); i++) {
                if (line.at(i) != ',') {
                    studentID += line.at(i);
                } else {
                    i++;
                    break;
                }
            }

            while (i < line.length()) {
                std::string answer = "";
                for (;i < line.length(); i++) {
                    if (line.at(i) != ',') {
                        answer += line.at(i);
                    } else {
                        i++;
                        break;
                    }
                }
                answers.push_back(answer.c_str());
            }
            data.insert(studentID.c_str(), answers);
        }
    }
    return data;
}

bool CSVProcessor::generateCSV(QString _file, QList<QString> data) {
    std::ofstream file;
    file.open(_file.toStdString().c_str(), std::ios_base::trunc);
    if (file.is_open()) {
        QListIterator<QString> iter(data);
        while(iter.hasNext()) {
            QString line = iter.next();
            file << line.toStdString().c_str();
            if (iter.hasNext()) {
                char newline = '\n';
                file << newline;
            }
        }
        file.close();
        return true;
    } else {
        DEBUG(DEBUG_ALRM, "CSV file failed to open.");
        return false;
    }
}
