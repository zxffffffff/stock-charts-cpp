QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    ../lib-chart/Core/DataBinding.h \
    ../lib-chart/Core/NumberCore.h \
    ../lib-chart/Core/StockCore.h \
    ../lib-chart/Core/Utils.h \
    ../lib-chart/Graphics/Graphics.h \
    ../lib-chart/Graphics/Painter.h \
    ../lib-chart/Graphics/PainterQt.h \
    ../lib-chart/Graphics/Widget.h \
    ../lib-chart/Model/ChartModel.h \
    ../lib-chart/Model/ChartPlugin.h \
    ../lib-chart/Model/Plugin/Indicator/Core/ExpCore.h \
    ../lib-chart/Model/Plugin/Indicator/Core/IndexCore.h \
    ../lib-chart/Model/Plugin/Indicator/Parser/ColorParser.h \
    ../lib-chart/Model/Plugin/Indicator/Parser/DrawingParser.h \
    ../lib-chart/Model/Plugin/Indicator/Parser/FunctionParser.h \
    ../lib-chart/Model/Plugin/Indicator/Parser/KeywordParser.h \
    ../lib-chart/Model/Plugin/Indicator/Parser/SubParser.h \
    ../lib-chart/Model/Plugin/Indicator/IndicatorParser.h \
    ../lib-chart/Model/Plugin/PluginIndicator.h \
    ../lib-chart/Model/Plugin/PluginIndicator.h \
    ../lib-chart/Model/Plugin/PluginPainting.h \
    ../lib-chart/Model/Plugin/PluginSuperimposition.h \
    ../lib-chart/View/ChartViewQt.h \
    ../lib-chart/ViewModel/Layer/LayerBG.h \
    ../lib-chart/ViewModel/Layer/LayerCrossLine.h \
    ../lib-chart/ViewModel/Layer/LayerIndicator.h \
    ../lib-chart/ViewModel/Layer/LayerPainting.h \
    ../lib-chart/ViewModel/Layer/LayerStock.h \
    ../lib-chart/ViewModel/Layer/LayerSuperimposition.h \
    ../lib-chart/ViewModel/Layer/LayerTitle.h \
    ../lib-chart/ViewModel/ChartContext.h \
    ../lib-chart/ViewModel/ChartCoordinate.h \
    ../lib-chart/ViewModel/ChartLayer.h \
    ../lib-chart/ViewModel/ChartProps.h \
    ../lib-chart/ViewModel/ChartViewModel.h \
    ../lib-chart/Version.h \
    mainwindow.h

SOURCES += \
    main.cpp \
    mainwindow.cpp

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
