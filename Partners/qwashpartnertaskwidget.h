#ifndef QWashPARTNERTASKWIDGET_H
#define QWashPARTNERTASKWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QDateTimeEdit>

class QWashPartnerTaskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QWashPartnerTaskWidget(QWidget *parent = nullptr);
    void SetActivPartner(QString strUuid);

    QTableWidget * m_pTasksTableWidget;

    void UpdateTasksList();

    QDateTimeEdit * m_pFromDateTimeEdit;
    QDateTimeEdit * m_pToDateTimeEdit;

    QString m_filtersStr;
    QString m_strUuidCurrentPartner;

public slots:
    void OnTasksDblClk(QTableWidgetItem*);
    void OnFilterApplyPressed();
    void OnSchetPressed();

signals:
};

#endif // QWashPARTNERTASKWIDGET_H
