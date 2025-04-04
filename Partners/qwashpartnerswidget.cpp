#include "qwashpartnerswidget.h"
#include <QHBoxLayout>
#include <QSqlQuery>
#include "common.h"

extern int iUserType;
extern QString strCurrentZakazId;

QWashPartnersWidget::QWashPartnersWidget(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout;
    this->setLayout(pVMainLayout);

    QHBoxLayout * pHWashPartnerLayout = new QHBoxLayout;

    QVBoxLayout * pPartnersLayout = new QVBoxLayout;

    QLabel * pPartnersLabel = new QLabel("Партнеры");
    pPartnersLabel->setAlignment(Qt::AlignHCenter);

    pPartnersLayout->addWidget(pPartnersLabel);

    m_pWashPartnerListWidget = new QListWidget();

    pPartnersLayout->addWidget(m_pWashPartnerListWidget);

    pHWashPartnerLayout->addLayout(pPartnersLayout , 1);

    m_pWashPartnerTabWidget = new QTabWidget;

    m_pWashPartnerTasksWidget = new QWashPartnerTaskWidget;

    m_pWashPartnerTabWidget->addTab(m_pWashPartnerTasksWidget , "Задачи");

    m_pWashPartnerCardWidget = new QWashPartnerCardWidget;

    if(iUserType == CarshService)
    {
        m_pWashPartnerTabWidget->addTab(m_pWashPartnerCardWidget , "Карточка");
    }

    pHWashPartnerLayout->addWidget(m_pWashPartnerTabWidget , 3);

    pVMainLayout->addLayout(pHWashPartnerLayout);


    /*Заполним пользователей*/
    QString strWashPartnerQuery("select Партнеры.id , ЮЛ.Название from Партнеры, ЮЛ where Партнеры.ЮЛ = ЮЛ.id and Партнеры.Удалено<>true and Партнеры.Тип='932a4dc1-238b-478d-8911-3de46dd8da65'");

    if(iUserType == Carsh)
    {
        QString strPartnerByCarshFilter = QString(" and Партнеры.Подтвержден = true and Партнеры.id in (select ИсполнительПартнер from ИсполнителиЗаказчики where Заказчик = '%1')").arg(strCurrentZakazId);
        strWashPartnerQuery.append(strPartnerByCarshFilter);
    }
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
}


void QWashPartnersWidget::WashPartnerClicked(QListWidgetItem * item)
{
    QString strPartnerUuid = item->data(Qt::UserRole).toString();
    if(iUserType == CarshService) m_pWashPartnerCardWidget->SetActivPartner(strPartnerUuid);
    m_pWashPartnerTasksWidget->SetActivPartner(strPartnerUuid);
}
