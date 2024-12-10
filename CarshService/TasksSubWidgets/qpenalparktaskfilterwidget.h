#ifndef QPENALPARKTASKFILTERWIDGET_H
#define QPENALPARKTASKFILTERWIDGET_H

#include <QWidget>
#include <QComboBox>

class QPenalParkTaskFilterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QPenalParkTaskFilterWidget(QWidget *parent = nullptr);

    QComboBox * m_pGibddCombo;
    QComboBox * m_pParkingCombo;
    QComboBox * m_pReasonCombo;

signals:
};

#endif // QPENALPARKTASKFILTERWIDGET_H
