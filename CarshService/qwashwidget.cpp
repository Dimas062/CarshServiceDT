#include "qwashwidget.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include "WashWidgets/qwashemplwidget.h"
#include "WashWidgets/qwashtaskwidget.h"

QWashWidget::QWashWidget(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout;


    QTabWidget * pTabs = new QTabWidget();

    QWashTaskWidget * pWashTasksWidget = new QWashTaskWidget;
    pTabs->addTab(pWashTasksWidget , "Задачи мойки");

    QWashEmplWidget * pWashEmplWidget = new QWashEmplWidget;
    pTabs->addTab(pWashEmplWidget , "Сотрудники мойки");

    pVMainLayout->addWidget(pTabs);
    this->setLayout(pVMainLayout);
}
