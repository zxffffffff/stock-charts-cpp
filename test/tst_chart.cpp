#include <QtTest>

// add necessary includes here

class Chart : public QObject
{
    Q_OBJECT

public:
    Chart();
    ~Chart();

private slots:
    void test_case1();

};

Chart::Chart()
{

}

Chart::~Chart()
{

}

void Chart::test_case1()
{

}

QTEST_APPLESS_MAIN(Chart)

#include "tst_chart.moc"
