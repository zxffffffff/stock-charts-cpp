QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

FORMS += \
    app/mainwindow.ui

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
    lib-chart/Model/Indicator/IndicatorParser.h \
    lib-chart/Model/Indicator/Parser/ColorParser.h \
    lib-chart/Model/Indicator/Parser/DrawingParser.h \
    lib-chart/Model/Indicator/Parser/FunctionParser.h \
    lib-chart/Model/Indicator/Parser/KeywordParser.h \
    lib-chart/Model/Indicator/Parser/SubParser.h \
    lib-chart/Model/Indicator/PluginIndicator.h \
    lib-chart/View/BG/LayerBG.h \
    lib-chart/View/ChartContext.h \
    lib-chart/View/ChartCoordinate.h \
    lib-chart/View/ChartLayer.h \
    lib-chart/View/ChartProps.h \
    lib-chart/View/ChartView.h \
    lib-chart/View/CrossLine/LayerCrossLine.h \
    lib-chart/View/Indicator/LayerIndicator.h \
    lib-chart/View/Stock/LayerStock.h \
    lib-chart/View/Title/LayerTitle.h \
    app/chartwidget.h \
    app/mainwindow.h \
    app/version.h

SOURCES += \
    app/chartwidget.cpp \
    app/main.cpp \
    app/mainwindow.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
