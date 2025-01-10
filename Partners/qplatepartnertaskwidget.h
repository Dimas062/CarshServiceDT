#ifndef QPLATEPARTNERTASKWIDGET_H
#define QPLATEPARTNERTASKWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QDateTimeEdit>
#include "service/xlspatterns.h"

class QPlatePartnerTaskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QPlatePartnerTaskWidget(QWidget *parent = nullptr);
    void SetActivPartner(QString strUuid);

    QTableWidget * m_pTasksTableWidget;

    void UpdateTasksList();
    QVector<SSchetItem> m_vCurrentSchetItems;

    QDateTimeEdit * m_pFromDateTimeEdit;
    QDateTimeEdit * m_pToDateTimeEdit;

    QString m_filtersStr;
    QString m_strUuidCurrentPartner;
    QString m_strIdPostavshik;

public slots:
    void OnTasksDblClk(QTableWidgetItem*);
    void OnFilterApplyPressed();
    void OnSchetPressed();
signals:
};

#endif // QPLATEPARTNERTASKWIDGET_H
