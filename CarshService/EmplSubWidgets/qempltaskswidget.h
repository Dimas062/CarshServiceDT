#ifndef QEMPLTASKSWIDGET_H
#define QEMPLTASKSWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QDateTimeEdit>

class QEmplTasksWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QEmplTasksWidget(QWidget *parent = nullptr);
    void SetActivEmpl(QString strUuid);

    QTableWidget * m_pTasksTableWidget;

    void UpdateTasksList();

    QComboBox * m_pTaskTypeComboBox;
    QDateTimeEdit * m_pFromDateTimeEdit;
    QDateTimeEdit * m_pToDateTimeEdit;
    QLineEdit * m_pNumberEdit;

    QString m_filtersStr;
    QString m_strUuidCurrentUser;

signals:

public slots:
    void OnTasksDblClk(QTableWidgetItem*);
    void OnFilterApplyPressed();
};

#endif // QEMPLTASKSWIDGET_H
