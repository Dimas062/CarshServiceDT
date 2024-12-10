#ifndef QEMPLGRAPHWIDGET_H
#define QEMPLGRAPHWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QChartView>
#include <QDateTimeEdit>
#include <QComboBox>


class QEmplGraphWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QEmplGraphWidget(QWidget *parent = nullptr);

    QDateEdit * m_pFromDateEdit;
    QDateEdit * m_pToDateEdit;
    QComboBox * m_pEmplCombo;

protected:
    void UpdateGraph();
    QBarSeries * m_pSeries;
    QChart * m_pChart;
    QBarCategoryAxis * m_pAxisX;
    QValueAxis * m_pAxisY;
    QChartView *m_pChartView;
    QBarSet *m_pBarSet;

public slots:
    void OnFilterApplyPressed();
};

#endif // QEMPLGRAPHWIDGET_H
