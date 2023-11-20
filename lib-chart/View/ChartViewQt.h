/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "../Core/DataBinding.h"
#include "../ViewModel/ChartViewModel.h"
#include "../Graphics/PainterQt.h"
#include <QWidget>
#include <QResizeEvent>

namespace StockCharts
{
    class ChartViewQt : public QWidget, public DataBinding
    {
        Q_OBJECT

    public:
        explicit ChartViewQt(QWidget *parent = nullptr)
            : QWidget(parent)
        {
            QWidget *p = this;
            while (p)
            {
                p->setMouseTracking(true); // for mousemove
                p = p->parentWidget();
            }
            setFocusPolicy(Qt::StrongFocus); // for keypress
        }

        virtual ~ChartViewQt() = default;

        void init(std::shared_ptr<ChartViewModel> vm)
        {
            m_vm = vm;
            listen(m_vm.get());
        }

        virtual void on(DataBinding *sender, const std::string &id) override
        {
            if (m_vm.get() == sender)
            {
                fire(id);
                update();
            }
        }

    protected:
        const ChartProps &getProps() const
        {
            return m_vm->getProps();
        }

        const ChartContext &getContext() const
        {
            return m_vm->getContext();
        }

    public slots:
        void slotDrawingType(int i)
        {
            auto props = m_vm->getProps();
            if (props.lineType == (EnStockLineType)i)
                return;
            props.lineType = (EnStockLineType)i;
            m_vm->setProps(props);
        }

        void slotCorrdinate(int i)
        {
            auto props = m_vm->getProps();
            if (props.yCoordType == (EnYCoordinateType)i)
                return;
            props.yCoordType = (EnYCoordinateType)i;
            m_vm->setProps(props);
        }

        void slotYLWidth(int i)
        {
            auto props = m_vm->getProps();
            if (props.ylAxisWidth == i)
                return;
            props.ylAxisWidth = i;
            m_vm->setProps(props);
        }

        void slotYRWidth(int i)
        {
            auto props = m_vm->getProps();
            if (props.yrAxisWidth == i)
                return;
            props.yrAxisWidth = i;
            m_vm->setProps(props);
        }

        void slotXHeight(int i)
        {
            auto props = m_vm->getProps();
            if (props.xAxisHeight == i)
                return;
            props.xAxisHeight = i;
            m_vm->setProps(props);
        }

        void slotPaddingLeft(int i)
        {
            auto props = m_vm->getProps();
            if (props.paddingLeft == i)
                return;
            props.paddingLeft = i;
            m_vm->setProps(props);
        }

        void slotPaddingTop(int i)
        {
            auto props = m_vm->getProps();
            if (props.paddingTop == i)
                return;
            props.paddingTop = i;
            m_vm->setProps(props);
        }

        void slotPaddingRight(int i)
        {
            auto props = m_vm->getProps();
            if (props.paddingRight == i)
                return;
            props.paddingRight = i;
            m_vm->setProps(props);
        }

        void slotPaddingBottom(int i)
        {
            auto props = m_vm->getProps();
            if (props.paddingBottom == i)
                return;
            props.paddingBottom = i;
            m_vm->setProps(props);
        }

    protected:
        virtual void paintEvent(QPaintEvent *event) override
        {
            PainterQt painter(this);
            m_vm->onPaint(painter);
        }

        virtual void resizeEvent(QResizeEvent *event) override
        {
            m_vm->onResize(Rect(0, 0, event->size().width(), event->size().height()));
        }

        virtual void mouseMoveEvent(QMouseEvent *event) override
        {
            int x = event->pos().x();
            int y = event->pos().y();
            m_vm->onMouseMove(Point(x, y));
            if (c_drag_flag == 'x')
            {
                double x_offset = x - x_drag_last;
                double x_remainder = m_vm->onScrollX_pix(x_offset, true);
                x_drag_last = x - x_remainder;
            }
            else if (c_drag_flag == 'y')
            {
                double y_offset = y - y_drag_last;
                double y_remainder = m_vm->onWheelY_pix(y_offset, true);
                y_drag_last = y - y_remainder;
            }
        }

        virtual void mousePressEvent(QMouseEvent *event) override
        {
            Point point(event->pos().x(), event->pos().y());
            if (m_vm->getContext().rectChart.contains(point) || m_vm->getContext().rectXAxis.contains(point))
            {
                c_drag_flag = 'x';
                x_drag_last = point.x;
                setCursor(Qt::SizeHorCursor);
            }
            else
            {
                c_drag_flag = 'y';
                y_drag_last = point.y;
                setCursor(Qt::SizeVerCursor);
            }
        }

        virtual void mouseReleaseEvent(QMouseEvent *event) override
        {
            if (c_drag_flag != '\0')
            {
                setCursor(Qt::ArrowCursor);
            }
            c_drag_flag = '\0';
        }

        virtual void leaveEvent(QEvent *event) override
        {
            m_vm->onMouseLeave();
        }

        virtual void mouseDoubleClickEvent(QMouseEvent *event) override
        {
            m_vm->onDBClick(Point(event->pos().x(), event->pos().y()));
        }

        virtual void keyPressEvent(QKeyEvent *event) override
        {
            switch (event->key())
            {
            case Qt::Key_Left:
                m_vm->onScrollX(-1);
                break;
            case Qt::Key_Right:
                m_vm->onScrollX(1);
                break;
            case Qt::Key_Up:
                m_vm->onWheelY(1);
                break;
            case Qt::Key_Down:
                m_vm->onWheelY(-1);
                break;
            default:
                return;
            }
        }

        virtual void wheelEvent(QWheelEvent *event) override
        {
            QPoint numPixels = event->pixelDelta();
            QPoint numDegrees = event->angleDelta() / 8;
            int xStep = 0;
            int yStep = 0;
            if (!numPixels.isNull())
            {
                xStep = numPixels.x();
                yStep = numPixels.y();
            }
            else if (!numDegrees.isNull())
            {
                QPoint numSteps = numDegrees / 15;
                xStep = numSteps.x();
                yStep = numSteps.y();
            }

            if (qAbs(xStep) > qAbs(yStep) && xStep != 0)
            {
                m_vm->onScrollX_pix(xStep, event->inverted());
            }
            else if (yStep != 0)
            {
                auto &ctx = getContext();
                if (ctx.crossLineVisible)
                    m_vm->onWheelY_pix(yStep, event->inverted());
                else
                    m_vm->onScrollX_pix(-yStep, event->inverted());
            }
        }

    private:
        std::shared_ptr<ChartViewModel> m_vm;

        char c_drag_flag = '\0';
        double x_drag_last = 0;
        double y_drag_last = 0;
    };
}
