#ifndef QWashPARTNERSWIDGET_H
#define QWashPARTNERSWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QListWidget>
#include "partners/qWashpartnertaskwidget.h"
#include "partners/qWashpartnercardwidget.h"

class QWashPartnersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QWashPartnersWidget(QWidget *parent = nullptr);
    QListWidget * m_pWashPartnerListWidget;
    QTabWidget * m_pWashPartnerTabWidget;

    QWashPartnerTaskWidget * m_pWashPartnerTasksWidget;

    QWashPartnerCardWidget * m_pWashPartnerCardWidget;

public slots:
    void WashPartnerClicked(QListWidgetItem*);
signals:
};

#endif // QWashPARTNERSWIDGET_H
