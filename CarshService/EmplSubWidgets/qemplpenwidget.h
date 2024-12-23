#ifndef QEMPLPENWIDGET_H
#define QEMPLPENWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QDateTimeEdit>

class QEmplPenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QEmplPenWidget(QWidget *parent = nullptr);


    QDateTimeEdit * m_pFromDateTimeEdit;
    QDateTimeEdit * m_pToDateTimeEdit;
    QLineEdit * m_pNumberEdit;

    void SetActivEmpl(QString strUuid);

    QTableWidget * m_pPenTableWidget;
    QString m_strUuidCurrentUser;

    void UpdatePenList();

public slots:
    void OnPenDblClk(QTableWidgetItem*);
    void OnFilterApplyPressed();
    void OnAddPen();
    void OnRemovePen();

signals:
};

#endif // QEMPLPENWIDGET_H
