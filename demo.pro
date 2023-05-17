QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

FORMS += \
    demo/mainwindow.ui

HEADERS += \
    lib-chart/Core/DataBinding.h \
    lib-chart/Core/NumberCore.h \
    lib-chart/Core/StockCore.h \
    lib-chart/Core/Utils.h \
    lib-chart/Graphics/Graphics.h \
    lib-chart/Graphics/Painter.h \
    lib-chart/Graphics/PainterQt.h \
    lib-chart/Graphics/Widget.h \
    lib-chart/Model/ChartModel.h \
    lib-chart/Model/ChartPlugin.h \
    lib-chart/Model/Indicator/Core/ExpCore.h \
    lib-chart/Model/Indicator/Core/IndexCore.h \
    lib-chart/Model/Indicator/Parser/ColorParser.h \
    lib-chart/Model/Indicator/Parser/DrawingParser.h \
    lib-chart/Model/Indicator/Parser/FunctionParser.h \
    lib-chart/Model/Indicator/Parser/KeywordParser.h \
    lib-chart/Model/Indicator/Parser/SubParser.h \
    lib-chart/Model/Indicator/IndicatorParser.h \
    lib-chart/Model/Indicator/PluginIndicator.h \
    lib-chart/View/ChartViewQt.h \
    lib-chart/ViewModel/BG/LayerBG.h \
    lib-chart/ViewModel/CrossLine/LayerCrossLine.h \
    lib-chart/ViewModel/Indicator/LayerIndicator.h \
    lib-chart/ViewModel/Stock/LayerStock.h \
    lib-chart/ViewModel/Title/LayerTitle.h \
    lib-chart/ViewModel/ChartContext.h \
    lib-chart/ViewModel/ChartCoordinate.h \
    lib-chart/ViewModel/ChartLayer.h \
    lib-chart/ViewModel/ChartProps.h \
    lib-chart/ViewModel/ChartViewModel.h \
    lib-chart/Version.h \
    demo/mainwindow.h

SOURCES += \
    demo/main.cpp \
    demo/mainwindow.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
