#ifndef QCARSHMAINDLG_H
#define QCARSHMAINDLG_H

#include <QDialog>
#include <QTableWidget>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QLineEdit>
#include "../CarshService/TasksSubWidgets/qpenalparktaskfilterwidget.h"

class QCarshMainDlg : public QDialog
{
    Q_OBJECT
public:
    QCarshMainDlg();

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

#endif // QCARSHMAINDLG_H
