#include "qstickpartnerswidget.h"
#include <QHBoxLayout>
#include <QSqlQuery>
#include "common.h"

extern int iUserType;
extern QString strCurrentZakazId;

QStickPartnersWidget::QStickPartnersWidget(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout;
    this->setLayout(pVMainLayout);

    QHBoxLayout * pHStickPartnerLayout = new QHBoxLayout;

    QVBoxLayout * pPartnersLayout = new QVBoxLayout;

    QLabel * pPartnersLabel = new QLabel("Партнеры");
    pPartnersLabel->setAlignment(Qt::AlignHCenter);

    pPartnersLayout->addWidget(pPartnersLabel);

    m_pStickPartnerListWidget = new QListWidget();

    pPartnersLayout->addWidget(m_pStickPartnerListWidget);

    pHStickPartnerLayout->addLayout(pPartnersLayout , 1);

    m_pStickPartnerTabWidget = new QTabWidget;

    m_pStickPartnerTasksWidget = new QStickPartnerTaskWidget;

    m_pStickPartnerTabWidget->addTab(m_pStickPartnerTasksWidget , "Задачи");

    m_pStickPartnerCardWidget = new QStickPartnerCardWidget;

    if(iUserType == CarshService)
    {
        m_pStickPartnerTabWidget->addTab(m_pStickPartnerCardWidget , "Карточка");
    }

    pHStickPartnerLayout->addWidget(m_pStickPartnerTabWidget , 3);

    pVMainLayout->addLayout(pHStickPartnerLayout);


    /*Заполним пользователей*/
    QString strStickPartnerQuery("select Партнеры.id , ЮЛ.Название from Партнеры, ЮЛ where Партнеры.ЮЛ = ЮЛ.id and Партнеры.Удалено<>true and Партнеры.Тип='082cf73c-6f6f-4167-ae89-b87c347091b2'");
    if(iUserType == Carsh)
    {
        QString strPartnerByCarshFilter = QString(" and Партнеры.Подтвержден = true and Партнеры.id in (select ИсполнительПартнер from ИсполнителиЗаказчики where Заказчик = '%1')").arg(strCurrentZakazId);
        strStickPartnerQuery.append(strPartnerByCarshFilter);
    }
    QSqlQuery StickPartnerQuery;
    StickPartnerQuery.exec(strStickPartnerQuery);
    while(StickPartnerQuery.next())
    {
        QListWidgetItem * pItem = new QListWidgetItem;
        pItem->setText(StickPartnerQuery.value(1).toString());
        pItem->setData(Qt::UserRole , StickPartnerQuery.value(0));
        m_pStickPartnerListWidget->addItem(pItem);
    }

    connect(m_pStickPartnerListWidget , SIGNAL(itemClicked(QListWidgetItem*)) , this , SLOT(StickPartnerClicked(QListWidgetItem*)));
}


void QStickPartnersWidget::StickPartnerClicked(QListWidgetItem * item)
{
    QString strPartnerUuid = item->data(Qt::UserRole).toString();
    if(iUserType == CarshService) m_pStickPartnerCardWidget->SetActivPartner(strPartnerUuid);
    m_pStickPartnerTasksWidget->SetActivPartner(strPartnerUuid);
}
