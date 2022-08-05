/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#include "mainwindow.h"
#include "kchartview.h"
#include "Indicator/IndicatorParser.h"
#include <QPainter>
#include <QRandomGenerator>

using namespace StockCharts;

namespace
{
    StockCore GenerateStock()
    {
        StockCore stock;
        stock.open = { 790.0, 803.5, 811.5, 838.0, 880.0, 830.5, 869.0, 827.5, 808.0, 818.0, 808.0, 800.5, 806.0, 817.0, 783.5, 802.0, 831.5, 900.0, 917.5, 940.0, 947.0, 895.0, 870.0, 837.0, 824.0, 820.0, 811.0, 789.5, 739.0, 758.0 };
        stock.high = { 800.0, 803.5, 829.5, 879.0, 888.0, 867.5, 869.0, 830.0, 811.0, 821.5, 808.0, 817.5, 814.5, 817.0, 790.0, 812.0, 867.0, 916.0, 940.0, 976.5, 949.0, 895.0, 870.0, 843.0, 835.0, 826.0, 814.5, 790.0, 739.5, 785.0 };
        stock.low = { 753.0, 790.0, 811.0, 838.0, 871.5, 830.0, 842.0, 811.0, 803.0, 803.0, 799.5, 799.0, 800.0, 808.0, 778.0, 802.0, 831.5, 894.5, 912.0, 932.0, 917.0, 884.0, 856.0, 823.0, 822.0, 816.0, 803.5, 761.5, 716.0, 755.0 };
        stock.close = { 800.0, 800.0, 828.0, 870.0, 874.0, 855.0, 850.0, 812.0, 809.0, 816.5, 801.0, 809.0, 809.5, 814.0, 790.0, 810.0, 860.0, 905.0, 932.0, 943.0, 941.5, 891.0, 860.0, 838.0, 828.0, 818.0, 808.0, 787.0, 729.0, 778.0 };
        return stock;
    }

    IndexFormula GenerateMACD()
    {
        IndexFormula formular;
        formular.name = "MACD";
        formular.expression =
            "DIF:EMA(CLOSE,SHORT)-EMA(CLOSE,LONG),COLORFF8D1E;\n"
            "DEA:EMA(DIF,M),COLOR0CAEE6;\n"
            "MACD:(DIF-DEA)*2,COLORSTICK,COLORE970DC;\n";
        formular.params = {
            {"SHORT", 12},
            {"LONG", 26},
            {"M", 9}
        };
        return formular;
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setMouseTracking(true);

    auto stock = std::make_shared<StockCore>(GenerateStock());
    ui.kchart0->init(stock, true);
    ui.kchart1->init(stock, false);
    ui.kchart1->addIndicator(GenerateMACD());

    // general
    connect(ui.generalDrawingType, &QComboBox::currentIndexChanged, ui.kchart0, &KChartView::slotDrawingType);
    // connect(ui.generalDrawingType, &QComboBox::currentIndexChanged, ui.kchart1, &KChartView::slotDrawingType);
    connect(ui.generalCorrdinate, &QComboBox::currentIndexChanged, ui.kchart0, &KChartView::slotCorrdinate);
    // connect(ui.generalCorrdinate, &QComboBox::currentIndexChanged, ui.kchart1, &KChartView::slotCorrdinate);
    connect(ui.generalYLWidth, &QSpinBox::valueChanged, ui.kchart0, &KChartView::slotYLWidth);
    connect(ui.generalYLWidth, &QSpinBox::valueChanged, ui.kchart1, &KChartView::slotYLWidth);
    connect(ui.generalYRWidth, &QSpinBox::valueChanged, ui.kchart0, &KChartView::slotYRWidth);
    connect(ui.generalYRWidth, &QSpinBox::valueChanged, ui.kchart1, &KChartView::slotYRWidth);
    connect(ui.generalXHeight, &QSpinBox::valueChanged, ui.kchart0, &KChartView::slotXHeight);
    connect(ui.generalXHeight, &QSpinBox::valueChanged, ui.kchart1, &KChartView::slotXHeight);
    connect(ui.generalPaddingLeft, &QSpinBox::valueChanged, ui.kchart0, &KChartView::slotPaddingLeft);
    connect(ui.generalPaddingLeft, &QSpinBox::valueChanged, ui.kchart1, &KChartView::slotPaddingLeft);
    connect(ui.generalPaddingTop, &QSpinBox::valueChanged, ui.kchart0, &KChartView::slotPaddingTop);
    connect(ui.generalPaddingTop, &QSpinBox::valueChanged, ui.kchart1, &KChartView::slotPaddingTop);
    connect(ui.generalPaddingRight, &QSpinBox::valueChanged, ui.kchart0, &KChartView::slotPaddingRight);
    connect(ui.generalPaddingRight, &QSpinBox::valueChanged, ui.kchart1, &KChartView::slotPaddingRight);
    connect(ui.generalPaddingBottom, &QSpinBox::valueChanged, ui.kchart0, &KChartView::slotPaddingBottom);
    connect(ui.generalPaddingBottom, &QSpinBox::valueChanged, ui.kchart1, &KChartView::slotPaddingBottom);
    connect(ui.generalNodeWidth, &QSpinBox::valueChanged, this, &MainWindow::slotNodeWidth);
    connect(ui.generalStickWidth, &QSpinBox::valueChanged, this, &MainWindow::slotStickWidth);

    // indicator
    connect(ui.indicatorBtnAdd, &QPushButton::clicked, this, &MainWindow::slotIndicatorBtnAdd);
    connect(ui.indicatorBtnClear, &QPushButton::clicked, this, &MainWindow::slotIndicatorBtnClear);
}

MainWindow::~MainWindow()
{
}

void MainWindow::slotNodeWidth(int node)
{
    int odd = (node % 2 ? node : node + 1);
    if (odd != node) {
        ui.generalNodeWidth->setValue(odd);
        return;
    }

    int stick = ui.generalStickWidth->value();
    if (stick > node) {
        ui.generalStickWidth->setValue(node);
        return;
    }

    ui.kchart0->slotNodeStickWidth(node, stick);
    ui.kchart1->slotNodeStickWidth(node, stick);
}

void MainWindow::slotStickWidth(int stick)
{
    int odd = (stick % 2 ? stick : stick + 1);
    if (odd != stick) {
        ui.generalStickWidth->setValue(odd);
        return;
    }

    int node = ui.generalNodeWidth->value();
    if (stick > node) {
        ui.generalNodeWidth->setValue(stick);
        return;
    }

    ui.kchart0->slotNodeStickWidth(node, stick);
    ui.kchart1->slotNodeStickWidth(node, stick);
}

void MainWindow::slotIndicatorBtnAdd()
{
    QString input = ui.indicatorFormularEdit->toPlainText();
    std::string expression = input.toStdString();
    std::map<std::string, int> params;
    for (int row = 0; row < ui.indicatorParamTable->rowCount(); ++row) {
        auto key = ui.indicatorParamTable->item(row, 0);
        auto val = ui.indicatorParamTable->item(row, 1);
        if (!key || !val)
            continue;
        params[key->text().toStdString()] = val->text().toInt();
    }
    IndexFormula formular = { "Index", expression, params};

    bool bMain = ui.indicatorRadioMain->isChecked();
    auto indicator = bMain ? ui.kchart0->addIndicator(formular) : ui.kchart1->addIndicator(formular);

    QString tips;
    if (!indicator) {
        tips = QString::fromLocal8Bit("未知错误!\n\n");
    }
    else if (indicator->indexCore.err) {
        tips = QString::fromLocal8Bit("error!\n\n");
        if (!indicator->indexCore.errExpression.empty())
            tips += QString::fromLocal8Bit("errExpression: ") + QString::fromStdString(indicator->indexCore.errExpression) + "\n\n";
        if (!indicator->indexCore.errWord.empty())
            tips += QString::fromLocal8Bit("errWord: ") + QString::fromStdString(indicator->indexCore.errWord) + "\n\n";
    }
    else {
        tips = QString::fromLocal8Bit("success!\n\n");
    }
    ui.indicatorLabel->setText(tips);
}

void MainWindow::slotIndicatorBtnClear()
{
    ui.kchart0->clearIndicators();
    ui.kchart1->clearIndicators();

    ui.indicatorLabel->clear();
}

/*
void MainWindow::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    QColor colorBg(Qt::white);
    QColor colorText(Qt::black);
    QColor colorRise(241, 62, 58);
    QColor colorFall(0, 168, 67);

    QRect mainRect(ui.label->pos(), ui.label->size());
    QRect subRect(ui.label_2->pos(), ui.label_2->size());
    painter.fillRect(mainRect, colorBg);
    painter.fillRect(subRect, colorBg);

    // x, y
    const int size = g_stock.close.size();
    const int barW = mainRect.width() / size / 2 * 2 - 2;
    const int barW2 = barW / 2;
    double mainMax = -DBL_MAX;
    double mainMin = DBL_MAX;
    double subMax = -DBL_MAX;
    double subMin = DBL_MAX;
    for (int i = 0; i < size; ++i) {
        mainMax = std::max(mainMax, g_stock.high[i]);
        mainMin = std::min(mainMin, g_stock.low[i]);
    }
    for (auto exp : g_result.exps) {
        if (exp.info.renameAssign || exp.core.size() != size)
            continue;
        for (int i = 0; i < size; ++i) {
            double val = exp.core[i];
            if (val == NumberNull)
                continue;
            if (g_formular.sub) {
                subMax = std::max(subMax, val);
                subMin = std::min(subMin, val);
            }
            else {
                mainMax = std::max(mainMax, val);
                mainMin = std::min(mainMin, val);
            }
        }
    }
    auto get_x = [&](int i) -> qreal
    {
        return mainRect.left() + (mainRect.width() * i / size) + barW2;
    };
    auto get_y = [&](double val, bool main = true) -> qreal
    {
        if (main) {
            if (val == NumberNull)
                return mainRect.bottom();
            return mainRect.top() + (mainRect.bottom() - mainRect.top()) * (mainMax - val) / (mainMax - mainMin);
        }
        else {
            if (val == NumberNull)
                return subRect.bottom();
            return subRect.top() + (subRect.bottom() - subRect.top()) * (subMax - val) / (subMax - subMin);
        }
    };

    // kline
    for (int i = 0; i < size; ++i) {
        int x = get_x(i);
        qreal h = get_y(g_stock.high[i]);
        qreal l = get_y(g_stock.low[i]);
        qreal o = get_y(g_stock.open[i]);
        qreal c = get_y(g_stock.close[i]);
        bool rise = g_stock.close[i] >= g_stock.open[i];
        QColor color = rise ? colorRise : colorFall;
        painter.setPen(color);
        painter.setBrush(color);
        painter.drawLine(x, h, x, l);
        painter.fillRect(x - barW2, std::min(o, c), barW, std::abs(o - c), color);
    }

    // index
    for (auto exp : g_result.exps) {
        if (exp.info.renameAssign || exp.core.size() != size)
            continue;
        QVector<QPoint> vPoints;
        qreal zero = get_y(0, !g_formular.sub);
        for (int i = 0; i < size; ++i) {
            int x = get_x(i);
            qreal y = get_y(exp.core[i], !g_formular.sub);
            vPoints.emplace_back(x, y);
        }
        QColor color(QString::fromStdString('#' + exp.color.color));
        Qt::PenStyle penStyle = Qt::SolidLine;

        if (exp.drawingType.type == EnDrawingType::None) {
            int lineWidth = int(exp.color.thick);
            switch (exp.color.type)
            {
            case EnExpLineType::COLORSTICK:
                for (int i = 0; i < size; ++i) {
                    if (exp.core[i] == NumberNull)
                        continue;
                    bool rise = exp.core[i] >= 0;
                    painter.setPen(rise ? colorRise : colorFall);
                    painter.drawLine(vPoints[i].x(), zero, vPoints[i].x(), vPoints[i].y());
                }
                break;

            default:
                if (exp.color.type == EnExpLineType::DOTLINE)
                    penStyle = Qt::DashLine;
                painter.setPen(QPen(color, lineWidth, penStyle));
                for (int i = 1; i < size; ++i) {
                    if (exp.core[i - 1] == NumberNull || exp.core[i] == NumberNull)
                        continue;
                    painter.drawLine(vPoints[i - 1], vPoints[i]);
                }
                break;
            }
        }
        else if (exp.drawingType.type == EnDrawingType::Number) {
            painter.setPen(color);
            for (int i = 0; i < size; ++i) {
                std::string str = exp.core.getOther(i);
                if (exp.core[i] == NumberNull || str.empty())
                    continue;
                painter.drawText(vPoints[i], QString::fromStdString(str));
            }
        }
        else if (exp.drawingType.type == EnDrawingType::CandleStick) {
            if(exp.drawingType.stickEmpty == -1)
                penStyle = Qt::DashLine;
            int width = exp.drawingType.stickWidth * barW;
            painter.setPen(QPen(color, 1, penStyle));
            painter.setBrush(Qt::transparent);
            for (int i = 0; i < size; ++i) {
                if (exp.core[i] == NumberNull || exp.core.getOther(i) == NumberNullStr)
                    continue;
                int y = get_y(std::stod(exp.core.getOther(i)), !g_formular.sub);
                painter.drawRect(vPoints[i].x() - width / 2, std::min(vPoints[i].y(), y), width, std::abs(vPoints[i].y() - y));
            }
            break;
        }
    }

    //text
    painter.setPen(colorText);
    painter.drawText(mainRect.topLeft(), QString::number(mainMax));
    painter.drawText(mainRect.bottomLeft(), QString::number(mainMin));
    painter.drawText(subRect.topLeft(), QString::number(subMax));
    painter.drawText(subRect.bottomLeft(), QString::number(subMin));
}
//*/
