#ifndef QStickPARTNERSWIDGET_H
#define QStickPARTNERSWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QListWidget>
#include "Partners/qStickpartnertaskwidget.h"
#include "Partners/qStickpartnercardwidget.h"

class QStickPartnersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QStickPartnersWidget(QWidget *parent = nullptr);
    QListWidget * m_pStickPartnerListWidget;
    QTabWidget * m_pStickPartnerTabWidget;

    QStickPartnerTaskWidget * m_pStickPartnerTasksWidget;

    QStickPartnerCardWidget * m_pStickPartnerCardWidget;

public slots:
    void StickPartnerClicked(QListWidgetItem*);
signals:
};

#endif // QStickPARTNERSWIDGET_H
