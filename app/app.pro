QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

FORMS += \
    src/mainwindow.ui

HEADERS += \
    src/Chart/Core/DataBinding.h \
    src/Chart/Core/NumberCore.h \
    src/Chart/Core/StockCore.h \
    src/Chart/Core/Utils.h \
    src/Chart/Graphics/Graphics.h \
    src/Chart/Graphics/Painter.h \
    src/Chart/Graphics/PainterQt.h \
    src/Chart/Graphics/Widget.h \
    src/Chart/Model/ChartModel.h \
    src/Chart/Model/ChartPlugin.h \
    src/Chart/Model/Indicator/Core/ExpCore.h \
    src/Chart/Model/Indicator/Core/IndexCore.h \
    src/Chart/Model/Indicator/IndicatorParser.h \
    src/Chart/Model/Indicator/Parser/ColorParser.h \
    src/Chart/Model/Indicator/Parser/DrawingParser.h \
    src/Chart/Model/Indicator/Parser/FunctionParser.h \
    src/Chart/Model/Indicator/Parser/KeywordParser.h \
    src/Chart/Model/Indicator/Parser/SubParser.h \
    src/Chart/Model/Indicator/PluginIndicator.h \
    src/Chart/View/BG/LayerBG.h \
    src/Chart/View/ChartContext.h \
    src/Chart/View/ChartCoordinate.h \
    src/Chart/View/ChartLayer.h \
    src/Chart/View/ChartProps.h \
    src/Chart/View/ChartView.h \
    src/Chart/View/CrossLine/LayerCrossLine.h \
    src/Chart/View/Indicator/LayerIndicator.h \
    src/Chart/View/Stock/LayerStock.h \
    src/Chart/View/Title/LayerTitle.h \
    src/ChartImpl/ChartWidget.h \
    src/mainwindow.h

SOURCES += \
    src/ChartImpl/ChartWidget.cpp \
    src/main.cpp \
    src/mainwindow.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
