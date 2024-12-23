#include "qcarshswidget.h"

#include <QTabWidget>
#include <QVBoxLayout>
#include "CarshSubWidgets/qcarshlistwidget.h"
#include "CarshSubWidgets/qcarshemplwidget.h"


QCarshsWidget::QCarshsWidget(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout;
    this->setLayout(pVMainLayout);

    QTabWidget * pTabs = new QTabWidget();

    QCarshListWidget * pCarshsWidget = new QCarshListWidget;
    pTabs->addTab(pCarshsWidget , "Службы");

    QCarshEmplWidget * pCarshEmplWidget = new QCarshEmplWidget;
    pTabs->addTab(pCarshEmplWidget , "Сотрудники каршеринга");

    pVMainLayout->addWidget(pTabs);
}



