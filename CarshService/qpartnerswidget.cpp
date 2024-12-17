#include "qpartnerswidget.h"
#include <QTabWidget>
#include <QVBoxLayout>

#include "Partners/qplatepartnerswidget.h"
#include "Partners/qstickpartnerswidget.h"
#include "Partners/qwashpartnerswidget.h"

QPartnersWidget::QPartnersWidget(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout;


    QTabWidget * pTabs = new QTabWidget();


    QPlatePartnersWidget * pPlatePartnersWidget = new QPlatePartnersWidget;
    pTabs->addTab(pPlatePartnersWidget , "Номера");

    QStickPartnersWidget * pStickPartnersWidget = new QStickPartnersWidget;
    pTabs->addTab(pStickPartnersWidget , "Оклейка");

    QWashPartnersWidget * pWashPartnersWidget = new QWashPartnersWidget;
    pTabs->addTab(pWashPartnersWidget , "Мойка");

    pVMainLayout->addWidget(pTabs);
    this->setLayout(pVMainLayout);
}
