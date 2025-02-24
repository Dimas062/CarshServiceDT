#include "qplatepartnerswidget.h"
#include <QHBoxLayout>
#include <QSqlQuery>
#include "common.h"

extern int iUserType;

QPlatePartnersWidget::QPlatePartnersWidget(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout;
    this->setLayout(pVMainLayout);

    QHBoxLayout * pHPlatePartnerLayout = new QHBoxLayout;

    QVBoxLayout * pPartnersLayout = new QVBoxLayout;

    QLabel * pPartnersLabel = new QLabel("Партнеры");
    pPartnersLabel->setAlignment(Qt::AlignHCenter);

    pPartnersLayout->addWidget(pPartnersLabel);

    m_pPlatePartnerListWidget = new QListWidget();

    pPartnersLayout->addWidget(m_pPlatePartnerListWidget);

    pHPlatePartnerLayout->addLayout(pPartnersLayout , 1);

    m_pPlatePartnerTabWidget = new QTabWidget;

    m_pPlatePartnerTasksWidget = new QPlatePartnerTaskWidget;

    m_pPlatePartnerTabWidget->addTab(m_pPlatePartnerTasksWidget , "Задачи");

    m_pPlatePartnerCardWidget = new QPlatePartnerCardWidget;

    m_pPlatePartnerTabWidget->addTab(m_pPlatePartnerCardWidget , "Карточка");

    pHPlatePartnerLayout->addWidget(m_pPlatePartnerTabWidget , 3);

    pVMainLayout->addLayout(pHPlatePartnerLayout);


    /*Заполним пользователей*/
    QString strPlatePartnerQuery("select Партнеры.id , ЮЛ.Название from Партнеры, ЮЛ where Партнеры.ЮЛ = ЮЛ.id and Партнеры.Удалено<>true and Партнеры.Тип='9c671ee9-2749-4717-a343-b18825855c29'");

    if(iUserType == Carsh) strPlatePartnerQuery.append(" and Партнеры.Подтвержден = true");


    QSqlQuery PlatePartnerQuery;
    PlatePartnerQuery.exec(strPlatePartnerQuery);
    while(PlatePartnerQuery.next())
    {
        QListWidgetItem * pItem = new QListWidgetItem;
        pItem->setText(PlatePartnerQuery.value(1).toString());
        pItem->setData(Qt::UserRole , PlatePartnerQuery.value(0));
        m_pPlatePartnerListWidget->addItem(pItem);
    }

    connect(m_pPlatePartnerListWidget , SIGNAL(itemClicked(QListWidgetItem*)) , this , SLOT(PlatePartnerClicked(QListWidgetItem*)));
}


void QPlatePartnersWidget::PlatePartnerClicked(QListWidgetItem * item)
{
    QString strPartnerUuid = item->data(Qt::UserRole).toString();
    m_pPlatePartnerCardWidget->SetActivPartner(strPartnerUuid);
    m_pPlatePartnerTasksWidget->SetActivPartner(strPartnerUuid);
}
