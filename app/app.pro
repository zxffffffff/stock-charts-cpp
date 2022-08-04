QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

FORMS += \
    src/kchartview.ui \
    src/mainwindow.ui

HEADERS += \
    src/Chart/ChartModel.h \
    src/Chart/ChartVM.h \
    src/Chart/Context/ChartContext.h \
    src/Chart/Context/ChartCoordinate.h \
    src/Chart/Plugins/ChartPlugin.h \
    src/Chart/Plugins/PluginBG.h \
    src/Chart/Plugins/PluginCrossLine.h \
    src/Chart/Plugins/PluginIndicator.h \
    src/Chart/Plugins/PluginKLine.h \
    src/Core/Graphics.h \
    src/Core/NumberCore.h \
    src/Core/Painter.h \
    src/Core/PainterQt.h \
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
    src/kchartview.h \
    src/mainwindow.h

SOURCES += \
    src/Chart/ChartModel.cpp \
    src/Chart/ChartVM.cpp \
    src/Chart/Context/ChartCoordinate.cpp \
    src/Chart/Plugins/ChartPlugin.cpp \
    src/Chart/Plugins/PluginBG.cpp \
    src/Chart/Plugins/PluginCrossLine.cpp \
    src/Chart/Plugins/PluginIndicator.cpp \
    src/Chart/Plugins/PluginKLine.cpp \
    src/Core/NumberCore.cpp \
    src/Core/Utils.cpp \
    src/Indicator/IndicatorParser.cpp \
    src/Indicator/Parser/ColorParser.cpp \
    src/Indicator/Parser/DrawingParser.cpp \
    src/Indicator/Parser/FunctionParser.cpp \
    src/Indicator/Parser/KeywordParser.cpp \
    src/Indicator/Parser/SubParser.cpp \
    src/kchartview.cpp \
    src/main.cpp \
    src/mainwindow.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
