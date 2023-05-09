include(test/gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

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
    lib-chart/View/Title/LayerTitle.h

SOURCES += \
    test/main.cpp \
    test/Core/DataBindingTest.cpp \
    test/Core/NumberCoreTest.cpp
