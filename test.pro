include(test/gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

SOURCES += \
    test/main.cpp \
    test/Core/DataBindingTest.cpp
