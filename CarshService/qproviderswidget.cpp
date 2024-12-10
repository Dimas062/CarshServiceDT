#include "qproviderswidget.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include "ProviderSubWidgets/qproviderempllistwidget.h"
#include "ProviderSubWidgets/qproviderslistwidget.h"

QProvidersWidget::QProvidersWidget(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout;
    this->setLayout(pVMainLayout);

    QTabWidget * pTabs = new QTabWidget();

    QProvidersListWidget * pProvidersWidget = new QProvidersListWidget;
    pTabs->addTab(pProvidersWidget , "Поставщики");

    QProviderEmplListWidget * pProviderEmplWidget = new QProviderEmplListWidget;
    pTabs->addTab(pProviderEmplWidget , "Сотрудники поставщиков");

    pVMainLayout->addWidget(pTabs);
}
