QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/CandlestickChart/Model/KChartModel.cpp \
    src/CandlestickChart/VM/KChartBodyVM.cpp \
    src/CandlestickChart/View/kchartview.cpp \
    src/Core/NumberCore.cpp \
    src/Core/StockCore.cpp \
    src/Core/Utils.cpp \
    src/TechnicalIndicators/IndicatorParser.cpp \
    src/TechnicalIndicators/Parser/ColorParser.cpp \
    src/TechnicalIndicators/Parser/DrawingParser.cpp \
    src/TechnicalIndicators/Parser/FunctionParser.cpp \
    src/TechnicalIndicators/Parser/KeywordParser.cpp \
    src/TechnicalIndicators/Parser/SubParser.cpp \
    src/TimeFrameChart/Model/TChartModel.cpp \
    src/TimeFrameChart/VM/TChartVM.cpp \
    src/TimeFrameChart/View/tchartview.cpp \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS += \
    src/CandlestickChart/Model/KChartModel.h \
    src/CandlestickChart/VM/KChartBodyVM.h \
    src/CandlestickChart/View/kchartview.h \
    src/Core/NumberCore.h \
    src/Core/StockCore.h \
    src/Core/Utils.h \
    src/TechnicalIndicators/Core/ExpCore.h \
    src/TechnicalIndicators/Core/IndexCore.h \
    src/TechnicalIndicators/IndicatorParser.h \
    src/TechnicalIndicators/Parser/ColorParser.h \
    src/TechnicalIndicators/Parser/DrawingParser.h \
    src/TechnicalIndicators/Parser/FunctionParser.h \
    src/TechnicalIndicators/Parser/KeywordParser.h \
    src/TechnicalIndicators/Parser/SubParser.h \
    src/TimeFrameChart/Model/TChartModel.h \
    src/TimeFrameChart/VM/TChartVM.h \
    src/TimeFrameChart/View/tchartview.h \
    src/mainwindow.h

FORMS += \
    src/CandlestickChart/View/kchartview.ui \
    src/TimeFrameChart/View/tchartview.ui \
    src/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
