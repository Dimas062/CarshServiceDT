#ifndef QPLATEPARTNERSWIDGET_H
#define QPLATEPARTNERSWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QListWidget>
#include "Partners/qplatepartnertaskwidget.h"
#include "Partners/qplatepartnercardwidget.h"

class QPlatePartnersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QPlatePartnersWidget(QWidget *parent = nullptr);
    QListWidget * m_pPlatePartnerListWidget;
    QTabWidget * m_pPlatePartnerTabWidget;

    QPlatePartnerTaskWidget * m_pPlatePartnerTasksWidget;

    QPlatePartnerCardWidget * m_pPlatePartnerCardWidget;

public slots:
    void PlatePartnerClicked(QListWidgetItem*);
signals:
};

#endif // QPLATEPARTNERSWIDGET_H
