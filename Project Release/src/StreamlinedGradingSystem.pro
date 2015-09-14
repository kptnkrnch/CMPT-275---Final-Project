#-------------------------------------------------
#
# Project created by QtCreator 2015-07-06T11:58:15
#
#-------------------------------------------------

QT += core gui
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StreamlinedGradingSystem
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    "Models/activity_model.cpp" \
    "Models/activityfile_model.cpp" \
    "Models/course_model.cpp" \
    "Models/marker_model.cpp" \
    "Models/model.cpp" \
    "Models/user_model.cpp" \
    "Models/rubric_model.cpp" \
    "Models/rubricitem_model.cpp" \
    "Models/session.cpp" \
    "Models/dbaccess.cpp" \
    "Controllers/roleselectionscreen.cpp" \
    "Controllers/sysadminhomescreen.cpp" \
    "Controllers/loginscreen.cpp" \
    "Controllers/courseselectionscreen.cpp" \
    "Controllers/adminassistanthomescreen.cpp" \
    "Controllers/coursecreationscreen.cpp" \
    "Controllers/rubriccreationscreen.cpp" \
    "Controllers/accountmodifyingscreen.cpp" \
    "Controllers/instructorhomescreen.cpp" \
    "Controllers/activitymanagementscreen.cpp" \
    "Controllers/activitytypeselectionscreen.cpp" \
    "Controllers/createprogactivityscreen.cpp" \
    "Controllers/createmcactivityscreen.cpp" \
    "Controllers/createessayactivityscreen.cpp" \
    "Controllers/createpsactivityscreen.cpp" \
    "Controllers/activity_view_screen.cpp" \
    "Controllers/activityselectionscreen.cpp" \
    "Controllers/coursemodifyscreen.cpp" \
    Controllers/resetpasswordscreen.cpp \
    Views/Components/inputsearchframe.cpp \
    "Controllers/changepasswordscreen.cpp" \
    Views/Components/inputframe.cpp \
    Controllers/essaypsgradingscreen.cpp \
    Controllers/rubricitemgradewidget.cpp \
    Controllers/gradeactivityselectionscreen.cpp \
    Controllers/accountcreationscreen.cpp \
    Helpers/controller_helper.cpp \
    Models/student_model.cpp \
    Models/csv_processor.cpp \
    programmingtestdialog.cpp \
    programmingtest.cpp \
    Models/studentmark_model.cpp \
    gradestudentselectionscreen.cpp \
    Models/studentmark_model.cpp \
    Helpers/pdfprocessor.cpp \
    Models/progtest_model.cpp \
    Controllers/activitymodifyscreen.cpp \
    norubricactivityselectionscreen.cpp\
    Models/progtestinput_model.cpp \
    Models/progtestoutput_model.cpp \
    Models/rubricitemgrade_model.cpp \
    programminggradingscreen.cpp \
    Models/multiplechoicetest_model.cpp \
    Controllers/multiplechoicegradingscreen.cpp \
    Controllers/gradefilenamedialog.cpp

HEADERS  += mainwindow.h \
    "Models/activity_model.h" \
    "Models/activityfile_model.h" \
    "Models/course_model.h" \
    "Models/marker_model.h" \
    "Models/model.h" \
    "Models/user_model.h" \
    "Models/rubric_model.h" \
    "Models/rubricitem_model.h" \
    "Models/dbaccess.h" \
    "Models/session.h" \
    "Exceptions/loginexception.h" \
    "Exceptions/ParameterException.h" \
    "Exceptions/dbgetexception.h" \
    Helpers/debug.h \
    "Controllers/roleselectionscreen.h" \
    "Controllers/sysadminhomescreen.h" \
    "Controllers/loginscreen.h" \
    "Controllers/courseselectionscreen.h" \
    "Controllers/adminassistanthomescreen.h" \
    "Controllers/coursecreationscreen.h" \
    "Controllers/rubriccreationscreen.h" \
    "Controllers/accountmodifyingscreen.h" \
    "Controllers/instructorhomescreen.h" \
    "Controllers/activitymanagementscreen.h" \
    "Controllers/activitytypeselectionscreen.h" \
    "Controllers/createprogactivityscreen.h" \
    "Controllers/createmcactivityscreen.h" \
    "Controllers/createessayactivityscreen.h" \
    "Controllers/createpsactivityscreen.h" \
    "Controllers/activity_view_screen.h" \
    "Controllers/activityselectionscreen.h" \
    "Controllers/coursemodifyscreen.h" \
    Exceptions/userexception.h \
    Controllers/resetpasswordscreen.h \
    "Views/Components/inputframe.h" \
    "Views/Components/inputsearchframe.h" \
    "Controllers/changepasswordscreen.h" \
    Exceptions/nullpointerexception.h \
    Controllers/essaypsgradingscreen.h \
    Controllers/rubricitemgradewidget.h \
    Controllers/gradeactivityselectionscreen.h \
    Exceptions/nullpointerexception.h \
    Controllers/accountcreationscreen.h \
    Models/csv_processor.h \
    Models/multiplechoicetest_model.h \
    Helpers/controller_helper.h \
    Exceptions/notexistsexception.h \
    Models/student_model.h \
    programmingtestdialog.h \
    programmingtest.h \
    Exceptions/queryexception.h \
    Models/studentmark_model.h \
    gradestudentselectionscreen.h \
    Helpers/pdfprocessor.h \
    Models/progtest_model.h \
    Controllers/activitymodifyscreen.h \
    norubricactivityselectionscreen.h\
    Models/progtestinput_model.h \
    Models/progtestoutput_model.h \
    rubricitemgrade_model.h \
    Exceptions/openfileexception.h \
    programminggradingscreen.h \
    Models/rubricitemgrade_model.h \
    Controllers/pdfgraphicsview.h \
    Controllers/multiplechoicegradingscreen.h \
    Controllers/gradefilenamedialog.h

FORMS    += "Views/mainwindow.ui" \
    "Views/roleselectionscreen.ui" \
    "Views/sysadminhomescreen.ui" \
    "Views/loginscreen.ui" \
    "Views/courseselectionscreen.ui" \
    "Views/adminassistanthomescreen.ui" \
    "Views/coursecreationscreen.ui" \
    "Views/rubriccreationscreen.ui" \
    "Views/accountmodifyingscreen.ui" \
    "Views/instructorhomescreen.ui" \
    "Views/activitymanagementscreen.ui" \
    "Views/activitytypeselectionscreen.ui" \
    "Views/createprogactivityscreen.ui" \
    "Views/createmcactivityscreen.ui" \
    "Views/createessayactivityscreen.ui" \
    "Views/createpsactivityscreen.ui" \
    "Views/activity_view_screen.ui" \
    "Views/activityselectionscreen.ui" \
    "Views/coursemodifyscreen.ui" \
    "Views/resetpasswordscreen.ui" \
    "Views/changepasswordscreen.ui" \
    "Views/changepasswordscreen.ui" \
    Views/essaypsgradingscreen.ui \
    Views/rubricitemgradewidget.ui \
    Views/gradeactivityselectionscreen.ui\
    Views/accountcreationscreen.ui \
    programmingtestdialog.ui \
    gradestudentselectionscreen.ui \
    Controllers/activitymodifyscreen.ui \
    norubricactivityselectionscreen.ui \
    programminggradingscreen.ui \
    Views/multiplechoicegradingscreen.ui \
    Controllers/gradefilenamedialog.ui

DISTFILES += \
    coursecreationscreen.ui.autosave

INCLUDEPATH += $$PWD/../lib/poppler_src

win32: LIBS += -L$$PWD/../lib/ -lpoppler

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../lib/poppler.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../lib/libpoppler.a

win32: LIBS += -L$$PWD/../lib/ -lpoppler-qt5

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../lib/poppler-qt5.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../lib/libpoppler-qt5.a
