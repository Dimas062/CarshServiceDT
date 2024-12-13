#include "qpartnerswidget.h"
#include <QTabWidget>
#include <QVBoxLayout>

#include "Partners/qplatepartnerswidget.h"

QPartnersWidget::QPartnersWidget(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout;


    QTabWidget * pTabs = new QTabWidget();


    QPlatePartnersWidget * pPlatePartnersWidget = new QPlatePartnersWidget;
    pTabs->addTab(pPlatePartnersWidget , "Номера");


    pVMainLayout->addWidget(pTabs);
    this->setLayout(pVMainLayout);
}
