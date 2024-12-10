#ifndef QCOSTSWIDGET_H
#define QCOSTSWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QDateTimeEdit>
#include <QComboBox>

class QCostsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QCostsWidget(QWidget *parent = nullptr);
    QTableWidget * m_pCostsTableWidget;

    QComboBox * m_pStatiaComboBox;
    QComboBox * m_pItemTypeComboBox;
    QComboBox * m_pEmplComboBox;
    QDateTimeEdit * m_pFromDateTimeEdit;
    QDateTimeEdit * m_pToDateTimeEdit;

    void UpdateCostsList();

    QString m_filtersStr;
signals:
public slots:
    void OnCostsDblClk(QTableWidgetItem*);
    void OnFilterApplyPressed();
    void ItemTypeComboChanged(int);
    void StatiaComboChanged(int);
};

#endif // QCOSTSWIDGET_H
