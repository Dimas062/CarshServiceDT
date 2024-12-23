#include "qWashpartnerswidget.h"
#include <QHBoxLayout>
#include <QSqlQuery>
#include "common.h"

extern int iUserType;

QWashPartnersWidget::QWashPartnersWidget(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout;
    this->setLayout(pVMainLayout);

    QHBoxLayout * pHMainLayout = new QHBoxLayout;

    QHBoxLayout * pHWashPartnerLayout = new QHBoxLayout;

    m_pWashPartnerListWidget = new QListWidget();

    pHWashPartnerLayout->addWidget(m_pWashPartnerListWidget);

    m_pWashPartnerTabWidget = new QTabWidget;

    m_pWashPartnerTasksWidget = new QWashPartnerTaskWidget;

    m_pWashPartnerTabWidget->addTab(m_pWashPartnerTasksWidget , "Задачи");

    m_pWashPartnerCardWidget = new QWashPartnerCardWidget;

    m_pWashPartnerTabWidget->addTab(m_pWashPartnerCardWidget , "Карточка");

    pHWashPartnerLayout->addWidget(m_pWashPartnerTabWidget);

    pVMainLayout->addLayout(pHWashPartnerLayout);


    /*Заполним пользователей*/
    QString strWashPartnerQuery("select Партнеры.id , ЮЛ.Название from Партнеры, ЮЛ where Партнеры.ЮЛ = ЮЛ.id and Партнеры.Удалено<>true and Партнеры.Тип='932a4dc1-238b-478d-8911-3de46dd8da65'");
    if(iUserType == Carsh) strWashPartnerQuery.append(" and Партнеры.Подтвержден = true");
    QSqlQuery WashPartnerQuery;
    WashPartnerQuery.exec(strWashPartnerQuery);
    while(WashPartnerQuery.next())
    {
        QListWidgetItem * pItem = new QListWidgetItem;
        pItem->setText(WashPartnerQuery.value(1).toString());
        pItem->setData(Qt::UserRole , WashPartnerQuery.value(0));
        m_pWashPartnerListWidget->addItem(pItem);
    }

    connect(m_pWashPartnerListWidget , SIGNAL(itemClicked(QListWidgetItem*)) , this , SLOT(WashPartnerClicked(QListWidgetItem*)));

    this->setLayout(pHMainLayout);
}


void QWashPartnersWidget::WashPartnerClicked(QListWidgetItem * item)
{
    QString strPartnerUuid = item->data(Qt::UserRole).toString();
    m_pWashPartnerCardWidget->SetActivPartner(strPartnerUuid);
    m_pWashPartnerTasksWidget->SetActivPartner(strPartnerUuid);
}
