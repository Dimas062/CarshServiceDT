#ifndef QStickPARTNERTASKWIDGET_H
#define QStickPARTNERTASKWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QDateTimeEdit>

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

public slots:
    void OnTasksDblClk(QTableWidgetItem*);
    void OnFilterApplyPressed();

signals:
};

#endif // QStickPARTNERTASKWIDGET_H
