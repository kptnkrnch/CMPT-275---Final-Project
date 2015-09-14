#include "coursemodifyscreen.h"

extern MainWindow *w;
extern Session *session;

CourseModifyScreen::CourseModifyScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::coursemodifyscreen) {

    ui->setupUi(this);
    QList<QMap<QString, QString> > tmp;
    QMap<QString, QString> currUser;

    // Set up course list
    allCourses = courseModel.getAllCourses();
    currentCourse = new QMap<QString, QString>();
    currentCourseTAIDs = new QStringList();
    int count = allCourses.count();

    // Populate list of all courses
    for( int i = 0; i < count; ++i ) {
        *currentCourse = allCourses.at(i);
        QString tmpNumber = Course::getCourseNumber(currentCourse);
        allCourseNames.append(tmpNumber);
        ui->liCourses->addItem(tmpNumber);
    }

    // Fixes bug:
    // ModifyCourse:
    //      if you set student list before selecting a course, it will not update that student list.
    if(count > 0) {
        ui->liCourses->setCurrentRow(0);
    }

    // Populate list of all TAs and Instructors
    tmp = userModel.getAllInstructorsAndTAs();
    count = tmp.size();
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



    ui->leName->setFocus();
}

CourseModifyScreen::~CourseModifyScreen() {
    delete ui;
    delete currentCourse;
    delete currentCourseTAIDs;
    delete instrCompleter;
}

void CourseModifyScreen::on_buRevert_clicked() {
    if(!msgBoxYesNo("Reverting...",
                    "Any unsaved changes will be lost. Are you sure you want to revert?")) {
        return;
    }
    on_liCourses_currentRowChanged(ui->liCourses->currentRow());
}

void CourseModifyScreen::on_buSave_clicked() {
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

    // Update the course
    Course::setCourseID(Course::getCourseID(currentCourse), &tmpCourse);
    if(!courseModel.updateCourse(Course::getCourseID(currentCourse), tmpCourse)) {
        msgBox.setText("Hmmm...");
        msgBox.setInformativeText("The course could not be updated at this time.\nPlease try again later.");
        msgBox.exec();
        return;
    }

    // Add the TAs
    if(!markerModel.setCourseTAs(*currentCourseTAIDs,
            Course::getCourseID(currentCourse))) {
        msgBox.setText("Hmmm...");
        msgBox.setInformativeText("The course TAs could not be updated at this time.\nPlease try again later.");
        msgBox.exec();
        return;
    }

    // Add the students
    if(!currentCourseStudents.isEmpty()
            && !studentModel.setCourseStudentList(Course::getCourseID(currentCourse),
                                              currentCourseStudents)) {
        msgBox.setText("Hmmm...");
        msgBox.setInformativeText("The course students could not be updated at this time.\nPlease try again later.");
        msgBox.exec();
        return;
    }

    msgBox.setText("Course (" + Course::getCourseNumber(currentCourse)
                   + ": " + Course::getCourseName(&tmpCourse)
                   + ") has been updated!");
    msgBox.exec();
    refresh();
}

void CourseModifyScreen::on_buCancel_clicked() {
    if(!msgBoxCancel()) {
        return;
    }

    w->setCentralWidget(new AdminAssistantHomeScreen);
}

void CourseModifyScreen::on_buDelete_clicked() {
    QString courseID = Course::getCourseID(currentCourse);
    QString courseName = Course::getCourseName(currentCourse);

    if(!msgBoxYesNo("Deleting..",
               "Are you sure you want to delete "
               + courseID + ": " + courseName + "?")) {
        return;
    }

    if(!courseModel.deleteCourse(courseID)) {
        QMessageBox msgBox;
        msgBox.setText("Could not delete "
                       + courseID + ": " + courseName
                       + " at this time.");
        msgBox.setInformativeText("Check that the course does not have any activities associated with it and try again later.");
        msgBox.exec();
        return;
    }
    refresh();
}

void CourseModifyScreen::on_liCourses_currentRowChanged(int currentRow) {
    QList<QMap<QString, QString> > currentCourseTAs;
    int count;

    // Flush old data
    currentCourseTAIDs = new QStringList();
    currentCourseStudents = QList<QMap<QString,QString> >();

    // Grab course information from cache
    *currentCourse = allCourses.at(currentRow);

    // Find all TAs for the course
    currentCourseTAs = markerModel.getMarkersByCourse(Course::getCourseID(currentCourse));
    count = currentCourseTAs.size();

    // Store only the TA's userID
    for(int i = 0; i < count; ++i) {
        QMap<QString, QString> TA = currentCourseTAs.at(i);
        currentCourseTAIDs->append(User::getUserID(&TA));
    }

    // Allow adding TAs
    ui->buAdd->setEnabled(true);

    // Update UI
    setKnownFields(currentCourse, currentCourseTAIDs);

    // These buttons will enable once you click something in the box.. or SEGFAULT
    ui->buAdd->setEnabled(false);
    ui->buRemove->setEnabled(false);

    ui->buSave->setEnabled(true);
    ui->buRevert->setEnabled(true);
}

void CourseModifyScreen::on_liCourseTAs_currentRowChanged( /* int currentRow */ ) {
    ui->buRemove->setEnabled(true);
}

void CourseModifyScreen::on_liAllTAs_currentRowChanged( /*int currentRow */ ) {
    if(ui->liCourses->currentRow() >= 0) {
        ui->buAdd->setEnabled(true);
    }
}

void CourseModifyScreen::on_buAdd_clicked() {
    QString selectedTA = allTAIDs.at(ui->liAllTAs->currentRow());

    // TA already a TA of this course?
    if(currentCourseTAIDs->contains(selectedTA)) {
        return;
    }

    // Add the TA to our cache and box
    currentCourseTAIDs->append(selectedTA);
    ui->liCourseTAs->addItem(selectedTA);
}

void CourseModifyScreen::on_buRemove_clicked() {
    // Remove the TA from our cache and box
    currentCourseTAIDs->removeAt(ui->liCourseTAs->currentRow());
    delete ui->liCourseTAs->item(ui->liCourseTAs->currentRow());

    // Won't allow clicks since its empty..
    if(ui->liCourseTAs->count() == 0) {
        ui->buRemove->setEnabled(false);
        ui->buRemove->clearFocus();
    }
}

void CourseModifyScreen::setKnownFields(QMap<QString, QString> const * const course, QStringList const * const TAs) {

    if(course == NULL) {
        throw NotExistsException("Course was not defined");
    }

    ui->leName->setText(Course::getCourseName(course));
    ui->leNumber->setText(Course::getCourseNumber(course));
    ui->leInstructor->setText(Course::getInstructorID(course));
    ui->deStart->setDate(Course::getStartDate(course));
    ui->deEnd->setDate(Course::getEndDate(course));

    // Added this object of QT looses its mind.
    // doesn't like QStringList const
    QStringList taList = *TAs;
    ui->liCourseTAs->clear();
    ui->liCourseTAs->addItems(taList);
    ui->leStudentList->setText("");
}

void CourseModifyScreen::on_buBrowse_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select Student List..."), "", "CSV(*.csv)");
    ui->leStudentList->setText(fileName);
}

void CourseModifyScreen::on_leStudentList_textChanged(const QString &arg1) {

    if(arg1.isEmpty()) {
        return;
    }

    // Set the new student list; will be saved to the course on save button pressed
    try {
        currentCourseStudents = studentModel.processStudentListFile(arg1);
    } catch(OpenFileException& e) {
        DEBUG(DEBUG_INFO, e.what());
        QMessageBox msgBox;
        msgBox.setInformativeText(arg1);
        msgBox.setText("There was a problem opening the requested file.");
        msgBox.exec();
        ui->leStudentList->setText("");
    }
}

void CourseModifyScreen::refresh() {
    w->setCentralWidget(new CourseModifyScreen);
}

void CourseModifyScreen::on_pushButton_clicked() {
    if(!msgBoxYesNo("Removing all students...", "Are you sure you want to remove all currents from this course?")) {
        return;
    }
    QMessageBox msgBox;
    msgBox.setText("Coming soon..");
    msgBox.exec();
}
