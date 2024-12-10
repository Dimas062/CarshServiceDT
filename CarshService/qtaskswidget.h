#ifndef QTASKSWIDGET_H
#define QTASKSWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QDateTimeEdit>
#include "TasksSubWidgets/qpenalparktaskfilterwidget.h"

class QTasksWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QTasksWidget(QWidget *parent = nullptr);
    QTableWidget * m_pTasksTableWidget;

    void UpdateTasksList();

    QComboBox * m_pTaskTypeComboBox;
    QComboBox * m_pEmplComboBox;
    QComboBox * m_pCarshsComboBox;
    QDateTimeEdit * m_pFromDateTimeEdit;
    QDateTimeEdit * m_pToDateTimeEdit;
    QLineEdit * m_pNumberEdit;


    QPenalParkTaskFilterWidget * m_pPenalParkTaskFilterWidget;
    QString m_filtersStr;
signals:
public slots:
    void OnTasksDblClk(QTableWidgetItem*);
    void OnFilterApplyPressed();
    void TaskTypeComboChanged(int);
};

#endif // QTASKSWIDGET_H
