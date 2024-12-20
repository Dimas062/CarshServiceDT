#ifndef QPLATEPARTNERTASKWIDGET_H
#define QPLATEPARTNERTASKWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QDateTimeEdit>

class QPlatePartnerTaskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QPlatePartnerTaskWidget(QWidget *parent = nullptr);
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

#endif // QPLATEPARTNERTASKWIDGET_H
