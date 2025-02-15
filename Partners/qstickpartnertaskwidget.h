#ifndef QStickPARTNERTASKWIDGET_H
#define QStickPARTNERTASKWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QDateTimeEdit>
#include "service/xlspatterns.h"

class QStickPartnerTaskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QStickPartnerTaskWidget(QWidget *parent = nullptr);
    void SetActivPartner(QString strUuid);

    QTableWidget * m_pTasksTableWidget;

    void UpdateTasksList();


    QDateTimeEdit * m_pFromDateTimeEdit;
    QDateTimeEdit * m_pToDateTimeEdit;

    QString m_filtersStr;
    QString m_strUuidCurrentPartner;
    QString m_strIdPostavshik;

public slots:
    void OnTasksDblClk(QTableWidgetItem*);
    void OnFilterApplyPressed();
    void OnSchetPressed();
    void OnSchetZakazPressed();
signals:
};

#endif // QStickPARTNERTASKWIDGET_H
