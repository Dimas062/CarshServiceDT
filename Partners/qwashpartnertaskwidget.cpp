#include "qWashpartnertaskwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QUuid>
#include <QSqlQuery>
#include <QPushButton>
#include "qWashpartnertaskdlg.h"

QWashPartnerTaskWidget::QWashPartnerTaskWidget(QWidget *parent)
    : QWidget{parent}
{
    m_filtersStr = QString("");

    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    QHBoxLayout * pFilterHLoyuot = new QHBoxLayout();

    pFilterHLoyuot->addStretch();

    pFilterHLoyuot->addStretch();

    QLabel * pDateTimeFromLabel = new QLabel("c: ");
    pFilterHLoyuot->addWidget(pDateTimeFromLabel);
    m_pFromDateTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime().addDays(-2));
    pFilterHLoyuot->addWidget(m_pFromDateTimeEdit);



    QLabel * pDateTimeToLabel = new QLabel("по: ");
    pFilterHLoyuot->addWidget(pDateTimeToLabel);
    m_pToDateTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime());
    pFilterHLoyuot->addWidget(m_pToDateTimeEdit);

    pFilterHLoyuot->addStretch();

    QPushButton * pFilterApplyButton = new QPushButton("Применть фильтры");
    connect(pFilterApplyButton,SIGNAL(pressed()),this,SLOT(OnFilterApplyPressed()));
    pFilterHLoyuot->addWidget(pFilterApplyButton);

    pVMainLayout->addLayout(pFilterHLoyuot);

    pVMainLayout->addSpacing(5);

    m_pTasksTableWidget = new QTableWidget;
    m_pTasksTableWidget->setColumnCount(5);
    //m_pTasksTableWidget->setColumnHidden(2,true);  //Скрыли зазказчика
    QStringList headers;
    headers << "Дата/время" << "Точка" <<"Комментарий"<<"Заказчик"<<"Стоимость";
    m_pTasksTableWidget->setHorizontalHeaderLabels(headers);
    connect(m_pTasksTableWidget , SIGNAL(itemDoubleClicked(QTableWidgetItem*)) , this , SLOT(OnTasksDblClk(QTableWidgetItem*)));
    pVMainLayout->addWidget(m_pTasksTableWidget);
    OnFilterApplyPressed();
}

void QWashPartnerTaskWidget::UpdateTasksList()
{
    m_pTasksTableWidget->clear();
    m_pTasksTableWidget->clearSpans();
    m_pTasksTableWidget->setRowCount(0);

    QStringList headers;
    headers  << "Дата/время" << "Точка" << "Номер"<<"Комментарий"<<"Заказчик"<<"Стоимость";
    m_pTasksTableWidget->setHorizontalHeaderLabels(headers);
    QString strQuery =  QString("select \"Задачи партнера Оклейка\".id , \"Задачи партнера Оклейка\".ДатаВремя, \"Точки Партнеров\".Название , \"Задачи партнера Оклейка\".Номер, \"Задачи партнера Оклейка\".Комментарий, Поставщики.Название from \"Задачи партнера Оклейка\", \"Точки Партнеров\" , Поставщики, Партнеры where Поставщики.id = Партнеры.Поставщик  and \"Задачи партнера Оклейка\".Партнер = Партнеры.id  and \"Точки Партнеров\".id = \"Задачи партнера Оклейка\".Точка and \"Задачи партнера Оклейка\".Партнер='%1' %2").arg(m_strUuidCurrentPartner).arg(m_filtersStr);

    QSqlQuery query;
    query.exec(strQuery);

    if(query.size() < 1) return;

    m_pTasksTableWidget->setRowCount(query.size());//+1 для итого

    int iRowCounter = 0;
    while(query.next())
    {
        /*Дата/время*/
        QTableWidgetItem * pItem = new QTableWidgetItem(QDateTime::fromSecsSinceEpoch(query.value(1).toInt()).toString("dd.MM.yyyy hh:mm"));
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 0,  pItem);

        /*Точка*/
        pItem = new QTableWidgetItem(query.value(2).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 1,  pItem);

        /*Количество*/
        pItem = new QTableWidgetItem(query.value(3).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 2,  pItem);

        /*Комментарий*/
        pItem = new QTableWidgetItem(query.value(4).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 3,  pItem);

        /*Заказчик*/
        pItem = new QTableWidgetItem(query.value(5).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 4,  pItem);

        QString strTypesExec=QString("select  \"Типы задач Оклейка\".Цена from \"Задача Оклейка - Типы\" , \"Типы задач Оклейка\"  where \"Типы задач Оклейка\".id = \"Задача Оклейка - Типы\".Тип and  \"Задача Оклейка - Типы\".Задача = '%1'").arg(query.value(0).toString());
        QSqlQuery TypesQuery;
        QString strWorks;
        TypesQuery.exec(strTypesExec);
        double dblPrice = 0;
        while(TypesQuery.next())
        {
            dblPrice = dblPrice + TypesQuery.value(0).toDouble();
        }
        QString strPrice = QString(" %1 руб.").arg(dblPrice);
        /*Заказчик*/
        pItem = new QTableWidgetItem(strPrice);
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 5,  pItem);

        iRowCounter++;
    }
}

void QWashPartnerTaskWidget::SetActivPartner(QString strUuid)
{
    m_strUuidCurrentPartner = strUuid;
    UpdateTasksList();
}

void QWashPartnerTaskWidget::OnTasksDblClk(QTableWidgetItem* item)
{
    QWashPartnerTaskDlg dlg(item->data(Qt::UserRole).toString());
    dlg.exec();
}

void QWashPartnerTaskWidget::OnFilterApplyPressed()
{
    m_filtersStr = QString("and \"Задачи партнера Оклейка\".ДатаВремя>'%1' and \"Задачи партнера Оклейка\".ДатаВремя<'%2'").arg(m_pFromDateTimeEdit->dateTime().toSecsSinceEpoch()).arg(m_pToDateTimeEdit->dateTime().toSecsSinceEpoch());
    UpdateTasksList();
}
