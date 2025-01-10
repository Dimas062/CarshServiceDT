#ifndef QWashPARTNERTASKWIDGET_H
#define QWashPARTNERTASKWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QDateTimeEdit>
#include "service/xlspatterns.h"

class QWashPartnerTaskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QWashPartnerTaskWidget(QWidget *parent = nullptr);
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

#endif // QWashPARTNERTASKWIDGET_H
