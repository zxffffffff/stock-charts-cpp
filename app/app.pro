QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

FORMS += \
    src/Chart/View/kchartview.ui \
    src/Chart/View/tchartview.ui \
    src/mainwindow.ui

HEADERS += \
    src/Chart/Model/KChartModel.h \
    src/Chart/Model/TChartModel.h \
    src/Chart/VM/KChartBodyVM.h \
    src/Chart/VM/TChartVM.h \
    src/Chart/View/kchartview.h \
    src/Chart/View/tchartview.h \
    src/Core/NumberCore.h \
    src/Core/StockCore.h \
    src/Core/Utils.h \
    src/Indicator/Core/ExpCore.h \
    src/Indicator/Core/IndexCore.h \
    src/Indicator/IndicatorParser.h \
    src/Indicator/Parser/ColorParser.h \
    src/Indicator/Parser/DrawingParser.h \
    src/Indicator/Parser/FunctionParser.h \
    src/Indicator/Parser/KeywordParser.h \
    src/Indicator/Parser/SubParser.h \
    src/mainwindow.h

SOURCES += \
    src/Chart/Model/KChartModel.cpp \
    src/Chart/Model/TChartModel.cpp \
    src/Chart/VM/KChartBodyVM.cpp \
    src/Chart/VM/TChartVM.cpp \
    src/Chart/View/kchartview.cpp \
    src/Chart/View/tchartview.cpp \
    src/Core/NumberCore.cpp \
    src/Core/StockCore.cpp \
    src/Core/Utils.cpp \
    src/Indicator/IndicatorParser.cpp \
    src/Indicator/Parser/ColorParser.cpp \
    src/Indicator/Parser/DrawingParser.cpp \
    src/Indicator/Parser/FunctionParser.cpp \
    src/Indicator/Parser/KeywordParser.cpp \
    src/Indicator/Parser/SubParser.cpp \
    src/main.cpp \
    src/mainwindow.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
