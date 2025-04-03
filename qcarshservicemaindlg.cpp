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
#include <QSqlQuery>
#include <common.h>

#include <QVBoxLayout>

QCarshServiceMainDlg::QCarshServiceMainDlg()
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout;
    this->setLayout(pVMainLayout);

    QTabWidget * pTabs = new QTabWidget();

    QPartnersWidget * pPartnersWidget = new QPartnersWidget;
    pTabs->addTab(pPartnersWidget ,QIcon(":/icons/partners_icon.png"), "Партнеры");

    QTasksWidget * pTasksWidget = new QTasksWidget;
    pTabs->addTab(pTasksWidget ,QIcon(":/icons/tasks_icon.png"), "Задачи");

    QCostsWidget * pCostsWidget = new QCostsWidget;
    pTabs->addTab(pCostsWidget ,QIcon(":/icons/pay_icon.png"), "Затраты");

    QCSGraphicsWidget * pGraphicsWidget = new QCSGraphicsWidget;
    pTabs->addTab(pGraphicsWidget ,QIcon(":/icons/charts2_icon.png"), QString("Графики"));

    QCarshsWidget * pCarshWidget = new QCarshsWidget;
    pTabs->addTab(pCarshWidget ,QIcon(":/icons/carshering_icon.png"), "Службы каршеринга");

    QProvidersWidget * pProvidersWidget = new QProvidersWidget;
    pTabs->addTab(pProvidersWidget ,QIcon(":/icons/man_icon.png"), "Поставщики услуг");

    QEmploeesWidget * pEmploeesWidget = new QEmploeesWidget;
    pTabs->addTab(pEmploeesWidget ,QIcon(":/icons/driver_icon.png"), "Сотрудники");

    QDicWidget * pDicWidget = new QDicWidget;
    pTabs->addTab(pDicWidget ,QIcon(":/icons/book_icon.png"), "Словари");

    pVMainLayout->addWidget(pTabs);

    setWindowFlags(windowFlags() |
                   Qt::WindowMinimizeButtonHint |
                   Qt::WindowMaximizeButtonHint);

    mHeartbeatTimer = std::make_shared<QTimer>(this);
    connect(mHeartbeatTimer.get() , &QTimer::timeout , this , &QCarshServiceMainDlg::OnProcessingHeartbeatTimer);
    mHeartbeatTimer->start(m_iHeartbeatTime);
}


void QCarshServiceMainDlg::OnProcessingHeartbeatTimer()
{
    mHeartbeatTimer->stop();

    QSqlQuery query;

    if(!executeQueryWithReconnect(query , "select id from Поставщики"))
        qDebug()<<"Hardbeat no next() from bd";

    mHeartbeatTimer->start(m_iHeartbeatTime);
}
