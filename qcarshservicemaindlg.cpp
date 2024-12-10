#include "qcarshservicemaindlg.h"
#include <QTabWidget>
#include "CarshService/qcarshswidget.h"
#include "CarshService/qpartnerswidget.h"
#include "CarshService/qemploeeswidget.h"
#include "CarshService/qtaskswidget.h"
#include "CarshService/qproviderswidget.h"
#include "CarshService/QCSGraphicsWidget.h"
#include "CarshService/qcostswidget.h"

#include <QVBoxLayout>

QCarshServiceMainDlg::QCarshServiceMainDlg()
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout;
    this->setLayout(pVMainLayout);

    QTabWidget * pTabs = new QTabWidget();

    QCarshsWidget * pCarshWidget = new QCarshsWidget;
    pTabs->addTab(pCarshWidget , "Службы каршеринга");

    QProvidersWidget * pProvidersWidget = new QProvidersWidget;
    pTabs->addTab(pProvidersWidget , "Поставщики услуг");

    QPartnersWidget * pPartnersWidget = new QPartnersWidget;
    pTabs->addTab(pPartnersWidget , "Партнеры");

    QEmploeesWidget * pEmploeesWidget = new QEmploeesWidget;
    pTabs->addTab(pEmploeesWidget , "Сотрудники");

    QTasksWidget * pTasksWidget = new QTasksWidget;
    pTabs->addTab(pTasksWidget , "Задачи");

    QCostsWidget * pCostsWidget = new QCostsWidget;
    pTabs->addTab(pCostsWidget , "Затраты");

    QCSGraphicsWidget * pGraphicsWidget = new QCSGraphicsWidget;
    pTabs->addTab(pGraphicsWidget , QString("Графики"));

    pVMainLayout->addWidget(pTabs);

    //this->setWindowFlags(Qt::Window);
    //this->showFullScreen();
}
