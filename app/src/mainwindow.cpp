/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#include "mainwindow.h"
#include "kchartview.h"
#include "Indicator/Parser/IndicatorParser.h"
#include <QPainter>
#include <QRandomGenerator>

using namespace StockCharts;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.btnRun, &QPushButton::clicked, this, &MainWindow::slotBtnRun);
    connect(ui.btnClear, &QPushButton::clicked, this, &MainWindow::slotBtnClear);

    m_chartWidget = new KChartView();
    ui.verticalLayout_4->addWidget(m_chartWidget);
}

MainWindow::~MainWindow()
{

}

void MainWindow::slotBtnRun()
{
    QString input = ui.textEdit->toPlainText();
    std::string expression = input.toStdString();
    std::map<std::string, int> params;
    for (int row = 0; row < ui.tableWidget->rowCount(); ++row) {
        auto key = ui.tableWidget->item(row, 0);
        auto val = ui.tableWidget->item(row, 1);
        if (!key || !val)
            continue;
        params[key->text().toStdString()] = val->text().toInt();
    }
    bool bSub = ui.radioSub->isChecked();
    IndexFormula formular = { "Index", bSub, expression, params};

    auto indicator = m_chartWidget->addIndicator(bSub ? 1 : 0, formular);

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
    ui.label_err->setText(tips);
    update();
}

void MainWindow::slotBtnClear()
{
    m_chartWidget->clearIndicators();

    ui.label_err->clear();
    update();
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
            if (val == NumberCore::EmptyNumber)
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
            if (val == NumberCore::EmptyNumber)
                return mainRect.bottom();
            return mainRect.top() + (mainRect.bottom() - mainRect.top()) * (mainMax - val) / (mainMax - mainMin);
        }
        else {
            if (val == NumberCore::EmptyNumber)
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
                    if (exp.core[i] == NumberCore::EmptyNumber)
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
                    if (exp.core[i - 1] == NumberCore::EmptyNumber || exp.core[i] == NumberCore::EmptyNumber)
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
                if (exp.core[i] == NumberCore::EmptyNumber || str.empty())
                    continue;
                painter.drawText(vPoints[i], QString::fromStdString(str));
            }
        }
        else if (exp.drawingType.type == EnDrawingType::StickLine) {
            if(exp.drawingType.stickEmpty == -1)
                penStyle = Qt::DashLine;
            int width = exp.drawingType.stickWidth * barW;
            painter.setPen(QPen(color, 1, penStyle));
            painter.setBrush(Qt::transparent);
            for (int i = 0; i < size; ++i) {
                if (exp.core[i] == NumberCore::EmptyNumber || exp.core.getOther(i) == NumberCore::EmptyNumberStr)
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
