#include "qcarshservicemaindlg.h"
#include <QTabWidget>
#include "CarshService/qcarshswidget.h"
#include "CarshService/qpartnerswidget.h"
#include "CarshService/qemploeeswidget.h"
#include "CarshService/qtaskswidget.h"
#include "CarshService/qproviderswidget.h"
#include "CarshService/QCSGraphicsWidget.h"
#include "CarshService/qcostswidget.h"
#include "Dictionary/qdicwidget.h"

#include <QVBoxLayout>

QCarshServiceMainDlg::QCarshServiceMainDlg()
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout;
    this->setLayout(pVMainLayout);

    QTabWidget * pTabs = new QTabWidget();

    QPartnersWidget * pPartnersWidget = new QPartnersWidget;
    pTabs->addTab(pPartnersWidget , "Партнеры");

    QTasksWidget * pTasksWidget = new QTasksWidget;
    pTabs->addTab(pTasksWidget , "Задачи");

    QCostsWidget * pCostsWidget = new QCostsWidget;
    pTabs->addTab(pCostsWidget , "Затраты");

    QCSGraphicsWidget * pGraphicsWidget = new QCSGraphicsWidget;
    pTabs->addTab(pGraphicsWidget , QString("Графики"));

    QCarshsWidget * pCarshWidget = new QCarshsWidget;
    pTabs->addTab(pCarshWidget , "Службы каршеринга");

    QProvidersWidget * pProvidersWidget = new QProvidersWidget;
    pTabs->addTab(pProvidersWidget , "Поставщики услуг");

    QEmploeesWidget * pEmploeesWidget = new QEmploeesWidget;
    pTabs->addTab(pEmploeesWidget , "Сотрудники");

    QDicWidget * pDicWidget = new QDicWidget;
    pTabs->addTab(pDicWidget , "Словари");

    pVMainLayout->addWidget(pTabs);

    //this->setWindowFlags(Qt::Window);
    //this->showFullScreen();
}
