QT += core testlib
QT -= gui

QMAKE_CXXFLAGS += -std=gnu++17
CONFIG += console c++17
TEMPLATE = app
TARGET = FinanceTrackerTests

INCLUDEPATH += \
    ../../googletest-old/googletest/include \
    ../../googletest-old/googlemock/include \
    ../.. \
    ../../googletest-old/googletest \
    ../../googletest-old/googlemock

SOURCES += \
    ../../googletest-old/googletest/src/gtest-all.cc \
    ../../googletest-old/googlemock/src/gmock-all.cc \
    ../../tests/FinanceTrackerTests/main.cpp \
    ../../tests/FinanceTrackerTests/test_datamanager.cpp \
    ../../src/modules/transaction.cpp \
    ../../src/modules/goal.cpp \
    ../../src/modules/category.cpp \
    ../../src/modules/datamanager.cpp \
    test_transaction.cpp

HEADERS += \
    ../../src/modules/datamanager.h \
    ../../src/modules/goal.h \
    ../../src/modules/category.h \
    ../../src/modules/transaction.h

DEFINES += GTEST_HAS_PTHREAD=0
DEFINES += GTEST_HAS_VISIBILITY=0
DEFINES += GTEST_INTERNAL_HAVE_CPP_ATTRIBUTE=0





