#ifndef QPENALPARKTASKFILTERWIDGET_H
#define QPENALPARKTASKFILTERWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QCheckBox>

class QPenalParkTaskFilterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QPenalParkTaskFilterWidget(QWidget *parent = nullptr);

    QComboBox * m_pGibddCombo;
    QComboBox * m_pParkingCombo;
    QComboBox * m_pReasonCombo;


    QCheckBox * m_pOnlyFromZakazchikCheckBox;
    QCheckBox * m_pOnlyManualCreatedCheckBox;

public slots:
    void onOnlyFromZakazchikStateChanged(int state);
    void onOnlyManualStateChanged(int state);

signals:
};

#endif // QPENALPARKTASKFILTERWIDGET_H
