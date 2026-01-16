QT += core gui widgets
QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG += c++17

SOURCES += \
    src/GoalManager/goalbuilder.cpp \
    src/GoalManager/goalmanager.cpp \
    src/Summaries/summary.cpp \
    src/Summaries/summarypage.cpp \
    src/Summaries/summaryview.cpp \
    src/mainwindow.cpp \
    main.cpp \
    src/modules/category.cpp \
    src/modules/datamanager.cpp \
    src/CategoryManager/categorymanager.cpp \
    src/modules/goal.cpp \
    src/modules/transaction.cpp \
    src/TransactionManager/transactionmanager.cpp \
    src/SaveLoad/saveLoad.cpp

HEADERS += \
    src/GoalManager/goalbuilder.h \
    src/GoalManager/goalmanager.h \
    src/Interface/Record.h \
    src/Summaries/summary.h \
    src/Summaries/summarypage.h \
    src/Summaries/summaryview.h \
    src/mainwindow.h \
    src/modules/category.h \
    src/modules/datamanager.h \
    src/CategoryManager/categorymanager.h \
    src/modules/goal.h \
    src/modules/transaction.h \
    src/TransactionManager/transactionmanager.h \
    src/SaveLoad/saveLoad.h \

FORMS += \
    src/GoalManager/goalmanager.ui \
    src/SaveLoad/saveLoad.ui \
    src/Summaries/summarypage.ui \
    src/Summaries/summaryview.ui \
    src/TransactionManager/transactionmanager.ui \
    src/CategoryManager/categorymanager.ui \
    src/mainwindow.ui \
