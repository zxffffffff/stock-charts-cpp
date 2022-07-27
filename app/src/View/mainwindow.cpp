#include "mainwindow.h"
#include "../Model/Indicator/IndicatorParser.h"
#include <QPainter>
#include <QRandomGenerator>

using namespace StockCharts;

IndexFormula g_formular;
StockCore g_stock;
ResultIndex g_result;

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
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.btnRun, &QPushButton::clicked, this, &MainWindow::slotBtnRun);

    g_stock = GenerateStock();
    slotBtnRun();
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
    g_formular = { "Index", bSub, expression, params};

    IndicatorParser parser;
    parser.setFormula(g_formular);
    parser.setStockCore(g_stock);
    bool ok = parser.run();
    g_result = parser.getResult();

    QString tips;
    if (g_result.err) {
        tips = QString::fromLocal8Bit("error!\n\n");
        if (!g_result.errExpression.empty())
            tips += QString::fromLocal8Bit("errExpression: ") + QString::fromStdString(g_result.errExpression) + "\n\n";
        if (!g_result.errWord.empty())
            tips += QString::fromLocal8Bit("errWord: ") + QString::fromStdString(g_result.errWord) + "\n\n";
    }
    else {
        tips = QString::fromLocal8Bit("success!\n\n");
    }
    ui.label_err->setText(tips);

    update();
}

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
            if (val == NumberCore::EmptyValue)
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
            if (val == NumberCore::EmptyValue)
                return mainRect.bottom();
            return mainRect.top() + (mainRect.bottom() - mainRect.top()) * (mainMax - val) / (mainMax - mainMin);
        }
        else {
            if (val == NumberCore::EmptyValue)
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

        if (exp.drawing.type == EnDrawingType::None) {
            int lineWidth = int(exp.color.thick);
            switch (exp.color.type)
            {
            case EnExpColorType::COLORSTICK:
                for (int i = 0; i < size; ++i) {
                    if (exp.core[i] == NumberCore::EmptyValue)
                        continue;
                    bool rise = exp.core[i] >= 0;
                    painter.setPen(rise ? colorRise : colorFall);
                    painter.drawLine(vPoints[i].x(), zero, vPoints[i].x(), vPoints[i].y());
                }
                break;

            default:
                if (exp.color.type == EnExpColorType::DOTLINE)
                    penStyle = Qt::DashLine;
                painter.setPen(QPen(color, lineWidth, penStyle));
                for (int i = 1; i < size; ++i) {
                    if (exp.core[i - 1] == NumberCore::EmptyValue || exp.core[i] == NumberCore::EmptyValue)
                        continue;
                    painter.drawLine(vPoints[i - 1], vPoints[i]);
                }
                break;
            }
        }
        else if (exp.drawing.type == EnDrawingType::Number) {
            painter.setPen(color);
            for (int i = 0; i < size; ++i) {
                std::string str = exp.core.getOther(i);
                if (exp.core[i] == NumberCore::EmptyValue || str.empty())
                    continue;
                painter.drawText(vPoints[i], QString::fromStdString(str));
            }
        }
        else if (exp.drawing.type == EnDrawingType::StickLine) {
            if(exp.drawing.stickEmpty == -1)
                penStyle = Qt::DashLine;
            int width = exp.drawing.stickWidth * barW;
            painter.setPen(QPen(color, 1, penStyle));
            painter.setBrush(Qt::transparent);
            for (int i = 0; i < size; ++i) {
                if (exp.core[i] == NumberCore::EmptyValue || exp.core.getOther(i) == NumberCore::EmptyValueStr)
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

