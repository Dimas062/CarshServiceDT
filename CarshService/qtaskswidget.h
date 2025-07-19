#ifndef QTASKSWIDGET_H
#define QTASKSWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QCheckBox>
#include "TasksSubWidgets/qpenalparktaskfilterwidget.h"
#include <QMap>
#include <QColor>

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
    QCheckBox * m_pOnlyFinishedCheckBox;
    QCheckBox * m_pOnlyUnfinishedCheckBox;
    QCheckBox * m_pDateByEndOfTask;


    QPenalParkTaskFilterWidget * m_pPenalParkTaskFilterWidget;
    QString m_filtersStr;

protected:
    int m_iCheckBoxCol;//Чек-бокс всегда последний столбец! Важно! (По нему считается общее количество столбцов m_iCheckBoxCol+1)
    QMap<int , QColor> m_ColorMap;//Будут захардкоженные номера для каждого столбца, аля его номер, т.к. цвета проверять для каждой ячейки, сравнение строк - накладно
    //QMap<QString , int> m_ColNameNumMap;//Собственно соответствие названий столбцов их номерам
    QColor m_defaultColor = QColor("#FFFFFF"); // Белый цвет

    void LoadColorMap();
    void SaveColorMap();
signals:
public slots:
    void OnTasksDblClk(QTableWidgetItem*);
    void OnFilterApplyPressed();
    void TaskTypeComboChanged(int);
    void OnSchetPressed();
    void onFinishedStateChanged(int state);
    void onUnfinishedStateChanged(int state);
    void onHeaderDoubleClicked(int logicalIndex);
    void OnExcelPressed();
    void OnCreateTaskPressed();
};

#endif // QTASKSWIDGET_H
