#ifndef QCARSHINPUTPENPARKTASKSWIDGET_H
#define QCARSHINPUTPENPARKTASKSWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QDateTimeEdit>
#include <QComboBox>

class QCarshInputPenParkTasksWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QCarshInputPenParkTasksWidget(QWidget *parent = nullptr);
    QTableWidget * m_pTasksTableWidget;
    QDateTimeEdit * m_pFromDateTimeEdit;
    QDateTimeEdit * m_pToDateTimeEdit;
    QComboBox * m_pCarshsCombo;

    void UpdateTasksList();

    QString m_filtersStr;

public slots:
    void OnFilterApplyPressed();
};

#endif // QCARSHINPUTPENPARKTASKSWIDGET_H
