#include "QCSGraphicsWidget.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include "GraphicSubWidgets/qpenparkgraphwidget.h"
#include "GraphicSubWidgets/qemplgraphwidget.h"

QCSGraphicsWidget::QCSGraphicsWidget(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout;
    this->setLayout(pVMainLayout);

    QTabWidget * pTabs = new QTabWidget();

    QPenParkGraphWidget * pPenParkGraphWidget = new QPenParkGraphWidget;
    pTabs->addTab(pPenParkGraphWidget , "Штрафстоянки");

    QEmplGraphWidget * pQEmplGraphWidget = new QEmplGraphWidget;
    pTabs->addTab(pQEmplGraphWidget , "Сотрудники");

    pVMainLayout->addWidget(pTabs);
}
