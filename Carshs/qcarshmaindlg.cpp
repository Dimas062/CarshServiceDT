#include "qcarshmaindlg.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include "Carshs/qcarshempltaskswidget.h"
#include "Partners/qplatepartnerswidget.h"
#include "Partners/qstickpartnerswidget.h"
#include "Partners/qwashpartnerswidget.h"

QCarshMainDlg::QCarshMainDlg()
{
    showMaximized();

    QVBoxLayout * pVMainLayout = new QVBoxLayout;
    this->setLayout(pVMainLayout);

    QTabWidget * pTabs = new QTabWidget();


    QCarshEmplTasksWidget * pCarshEmplTasksWidget = new QCarshEmplTasksWidget;
    pTabs->addTab(pCarshEmplTasksWidget , "Задачи");

    QPlatePartnersWidget * pPlatePartnersWidget = new QPlatePartnersWidget;
    pTabs->addTab(pPlatePartnersWidget , "Номера");

    QStickPartnersWidget * pStickPartnersWidget = new QStickPartnersWidget;
    pTabs->addTab(pStickPartnersWidget , "Оклейка");

    QWashPartnersWidget * pWashPartnersWidget = new QWashPartnersWidget;
    pTabs->addTab(pWashPartnersWidget , "Мойка");

    pVMainLayout->addWidget(pTabs);
}
