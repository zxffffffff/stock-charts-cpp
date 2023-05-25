include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

HEADERS += \
    ../lib-chart/Core/DataBinding.h \
    ../lib-chart/Core/NumberCore.h \
    ../lib-chart/Core/Utils.h \
    ../lib-chart/Graphics/Graphics.h \
    ../lib-chart/Graphics/Painter.h

SOURCES += \
    main.cpp \
    Core/DataBindingTest.cpp \
    Core/NumberCoreTest.cpp \
    Core/UtilsTest.cpp \
    Graphics/GraphicsTest.cpp \
    Graphics/PainterTest.cpp
