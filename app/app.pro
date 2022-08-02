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
    src/Chart/Model/Area/ChartAreaContext.h \
    src/Chart/Model/Area/ChartCoordinate.h \
    src/Chart/Model/ChartModel.h \
    src/Chart/Model/ChartProps.h \
    src/Chart/Plugin/Background/PluginBG.h \
    src/Chart/Plugin/CandleStick/PluginKLine.h \
    src/Chart/Plugin/PluginLayer.h \
    src/Chart/View/Area/ChartAreaVM.h \
    src/Chart/View/Area/ChartAreaView.h \
    src/Chart/View/Timebar/ChartTimebarView.h \
    src/Chart/View/Title/ChartTitleView.h \
    src/Core/Graphics.h \
    src/Core/NumberCore.h \
    src/Core/Painter.h \
    src/Core/PainterQt.h \
    src/Core/StockCore.h \
    src/Core/Utils.h \
    src/Indicator/Core/ExpCore.h \
    src/Indicator/Core/IndexCore.h \
    src/Indicator/Parser/ColorParser.h \
    src/Indicator/Parser/DrawingParser.h \
    src/Indicator/Parser/FunctionParser.h \
    src/Indicator/Parser/IndicatorParser.h \
    src/Indicator/Parser/KeywordParser.h \
    src/Indicator/Parser/SubParser.h \
    src/Plugin/Indicator/PluginIndicator.h \
    src/kchartview.h \
    src/mainwindow.h

SOURCES += \
    src/Chart/Model/Area/ChartCoordinate.cpp \
    src/Chart/Model/ChartModel.cpp \
    src/Chart/Model/ChartProps.cpp \
    src/Chart/Plugin/Background/PluginBG.cpp \
    src/Chart/Plugin/CandleStick/PluginKLine.cpp \
    src/Chart/Plugin/PluginLayer.cpp \
    src/Chart/View/Area/ChartAreaVM.cpp \
    src/Chart/View/Area/ChartAreaView.cpp \
    src/Chart/View/Timebar/ChartTimebarView.cpp \
    src/Chart/View/Title/ChartTitleView.cpp \
    src/Core/NumberCore.cpp \
    src/Core/Utils.cpp \
    src/Indicator/Parser/ColorParser.cpp \
    src/Indicator/Parser/DrawingParser.cpp \
    src/Indicator/Parser/FunctionParser.cpp \
    src/Indicator/Parser/IndicatorParser.cpp \
    src/Indicator/Parser/KeywordParser.cpp \
    src/Indicator/Parser/SubParser.cpp \
    src/Plugin/Indicator/PluginIndicator.cpp \
    src/kchartview.cpp \
    src/main.cpp \
    src/mainwindow.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
