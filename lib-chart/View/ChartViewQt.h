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
            m_vm->onMouseMove(Point(event->pos().x(), event->pos().y()));
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
            int yStep = event->angleDelta().y() / 120;
            int xStep = event->angleDelta().x() / 120;
            if (yStep != 0)
            {
                auto &ctx = getContext();
                if (ctx.crossLineVisible)
                    m_vm->onWheelY(yStep);
                else
                    m_vm->onScrollX(-yStep);
            }
            if (xStep != 0)
            {
                m_vm->onScrollX(xStep);
            }
        }

        virtual void leaveEvent(QEvent *event) override
        {
            m_vm->onMouseLeave();
        }

        virtual void mouseDoubleClickEvent(QMouseEvent *event) override
        {
            m_vm->onDBClick(Point(event->pos().x(), event->pos().y()));
        }

    private:
        std::shared_ptr<ChartViewModel> m_vm;
    };
}
