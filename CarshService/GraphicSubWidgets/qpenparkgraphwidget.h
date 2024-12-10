#ifndef QPENPARKGRAPHWIDGET_H
#define QPENPARKGRAPHWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QChartView>
#include <QDateTimeEdit>
#include <QComboBox>


class QPenParkGraphWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QPenParkGraphWidget(QWidget *parent = nullptr);

    void UpdateGraph();

    QDateEdit * m_pFromDateEdit;
    QDateEdit * m_pToDateEdit;
    QComboBox * m_pParkingCombo;

protected:
    QBarSeries * m_pSeries;
    QChart * m_pChart;
    QBarCategoryAxis * m_pAxisX;
    QValueAxis * m_pAxisY;
    QChartView *m_pChartView;
    QBarSet *m_pBarSet;

public slots:
    void OnFilterApplyPressed();

signals:
};

#endif // QPENPARKGRAPHWIDGET_H
