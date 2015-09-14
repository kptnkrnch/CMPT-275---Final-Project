#include "coursecreationscreen.h"

extern MainWindow *w;
extern Session *session;

CourseCreationScreen::CourseCreationScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CourseCreationScreen)
{
    ui->setupUi(this);
    QList<QMap<QString, QString> > tmp;
    QMap<QString, QString> currUser;

    currentCourseTAIDs = new QStringList();

    tmp = userModel.getAllInstructorsAndTAs();
    int count = tmp.size();
    for(int i = 0; i < count; ++i) {
        currUser = tmp.at(i);
        if(User::isInstructor(&currUser)) {
            allInstructorIDs << User::getUserID(&currUser);
        }

        if(User::isTeachingAssistant(&currUser)) {
            QString tmpID = User::getUserID(&currUser);
            allTAIDs.append(tmpID);
            ui->liAllTAs->addItem(tmpID);
        }
    }

    // Autocomplete all instructor IDs
    instrCompleter = new QCompleter(allInstructorIDs, this);
    instrCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    ui->leInstructor->setCompleter(instrCompleter);
    //ui->leStudentList->setReadOnly(true);
    ui->leName->setFocus();
}

CourseCreationScreen::~CourseCreationScreen() {
    delete ui;    
    delete currentCourseTAIDs;
    delete instrCompleter;
}

void CourseCreationScreen::on_buSave_clicked() {
    QMap<QString, QString> tmpCourse;
    QMessageBox msgBox;
    QString msgBoxString = "Looks like there is a problem with...\n";
    QStringList invalidInputs;

    // Verify and fill in new course to be inserted
    tmpCourse = courseModel.createCourse();
    if(!Course::setCourseName(ui->leName->text(), &tmpCourse, true)) { invalidInputs << "course name"; }
    if(!Course::setCourseNumber(ui->leNumber->text(), &tmpCourse, true)) { invalidInputs << "course number"; }
    if(!Course::setInstructorID(ui->leInstructor->text(), &tmpCourse, true)) { invalidInputs << "instructor ID"; }

    Course::setStartDate(ui->deStart->text(), &tmpCourse);
    Course::setEndDate(ui->deEnd->text(), &tmpCourse);
    if(!Course::verifyDates(&tmpCourse)) { invalidInputs << "dates"; }

    // Check what went wrong
    if(!invalidInputs.isEmpty()) {
        foreach(QString input, invalidInputs) {
            msgBoxString += input + "\n";
        }

        // Alert the errors and exit
        msgBox.setText(msgBoxString);
        msgBox.exec();
        return;
    }

    // Create the course
    try {
        courseModel.insertCourse(&tmpCourse,
                !Course::getInstructorID(&tmpCourse).isEmpty());
    } catch (DBGetException& e) {
        // userID does not exist
        msgBox.setText("Hmmm...");
        msgBox.setInformativeText("No such instructor exists.");
        msgBox.exec();

        ui->leInstructor->selectAll();
        return;
    } catch (NotExistsException& e) {
        // userID not an instructor
        msgBox.setText("Hmmm...");
        msgBox.setInformativeText(Course::getInstructorID(&tmpCourse)
            + " is not an instructor.");
        msgBox.exec();
        ui->leInstructor->selectAll();
        return;
    }

    // Add the TAs
    if(!markerModel.setCourseTAs(*currentCourseTAIDs,
            Course::getCourseID(&tmpCourse))) {
        msgBox.setText("Hmmm...");
        msgBox.setInformativeText("The course TAs could not be set at this time.\nPlease try again later.");
        msgBox.exec();
        return;
    }

    // Add the students
    if(!currentCourseStudents.isEmpty()
            && !studentModel.setCourseStudentList(Course::getCourseID(&tmpCourse),
                                          currentCourseStudents) ) {
        msgBox.setText("Hmmm..");
        msgBox.setInformativeText("The course students could not be updated at this time.\nPlease try again later.");
        msgBox.exec();
        return;
    }

    msgBox.setText("Course '" + Course::getCourseNumber(&tmpCourse)
                   + ": " + Course::getCourseName(&tmpCourse)
                   + "' has been created!");
    msgBox.exec();
    w->setCentralWidget(new AdminAssistantHomeScreen);
}

void CourseCreationScreen::on_buCancel_clicked() {
    if(!msgBoxCancel()) {
        return;
    }
    w->setCentralWidget(new AdminAssistantHomeScreen);
}

void CourseCreationScreen::on_liAllTAs_currentRowChanged(/* int currentRow */) {
    ui->buAdd->setEnabled(true);
}

void CourseCreationScreen::on_liCourseTAs_currentRowChanged(/* int currentRow */) {
    ui->buRemove->setEnabled(true);
}

void CourseCreationScreen::on_buAdd_clicked() {
    QString selectedTA = allTAIDs.at(ui->liAllTAs->currentRow());

    // TA already a TA of this course?
    if(currentCourseTAIDs->contains(selectedTA)) {
        return;
    }

    // Add the TA to our cache and box
    currentCourseTAIDs->append(selectedTA);
    ui->liCourseTAs->addItem(selectedTA);
}

void CourseCreationScreen::on_buRemove_clicked() {
    // Remove the TA from our cache and box
    currentCourseTAIDs->removeAt(ui->liCourseTAs->currentRow());
    delete ui->liCourseTAs->item(ui->liCourseTAs->currentRow());

    // Won't allow clicks since its empty..
    if(ui->liCourseTAs->count() == 0) {
        ui->buRemove->setEnabled(false);
        ui->buRemove->clearFocus();
    }
}

void CourseCreationScreen::on_leStudentList_textChanged(const QString &fileName) {   
    if(fileName.isEmpty()) {
        currentCourseStudents = QList<QMap<QString, QString> >();
        return;
    }

    // Set the new student list; will be saved to the course on save button pressed
    try {
        currentCourseStudents = studentModel.processStudentListFile(fileName);
    } catch(OpenFileException& e) {
        DEBUG(DEBUG_INFO, e.what());
        QMessageBox msgBox;
        msgBox.setText(fileName);
        msgBox.setInformativeText("There was a problem opening the requested file.");
        msgBox.exec();
        ui->leStudentList->setText("");
    }


}

void CourseCreationScreen::on_buBrowse_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select Student List..."), "", "CSV(*.csv)");
    ui->leStudentList->setText(fileName);
}
