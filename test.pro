include(test/gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

HEADERS += \
    lib-chart/Core/DataBinding.h \
    lib-chart/Core/NumberCore.h \
    lib-chart/Core/Utils.h \
    lib-chart/Graphics/Graphics.h \
    lib-chart/Graphics/Painter.h

SOURCES += \
    test/main.cpp \
    test/Core/DataBindingTest.cpp \
    test/Core/NumberCoreTest.cpp \
    test/Core/UtilsTest.cpp \
    test/Graphics/GraphicsTest.cpp \
    test/Graphics/PainterTest.cpp
