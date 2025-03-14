#ifndef QCarshEmplTasksWidget_H
#define QCarshEmplTasksWidget_H

#include <QDialog>
#include <QTableWidget>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QLineEdit>
#include "../CarshService/TasksSubWidgets/qpenalparktaskfilterwidget.h"

//Класс для отображения задач сотрудников службам каршеринга (яндексу, ситидрайву и тд)
class QCarshEmplTasksWidget : public QWidget
{
    Q_OBJECT
public:
    QCarshEmplTasksWidget();

    QTableWidget * m_pTasksTableWidget;


    void UpdateTasksList();

    QComboBox * m_pTaskTypeComboBox;
    QDateTimeEdit * m_pFromDateTimeEdit;
    QDateTimeEdit * m_pToDateTimeEdit;
    QLineEdit * m_pNumberEdit;

    QString m_filtersStr;
    QPenalParkTaskFilterWidget * m_pPenalParkTaskFilterWidget;


public slots:
    void OnTasksDblClk(QTableWidgetItem*);
    void OnFilterApplyPressed();
    void TaskTypeComboChanged(int);
};

#endif // QCarshEmplTasksWidget_H
