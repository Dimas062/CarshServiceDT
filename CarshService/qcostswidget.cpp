#include "qcostswidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QUuid>
#include <QSqlQuery>
#include <QLineEdit>
#include <QDateTime>
#include <QPushButton>
#include <QMessageBox>


QCostsWidget::QCostsWidget(QWidget *parent)
    : QWidget{parent}
{
    m_filtersStr = QString("");

    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    QHBoxLayout * pFilterHLoyuot = new QHBoxLayout();

    /*Период*/

    QLabel * pDateTimeFromLabel = new QLabel("c: ");
    pFilterHLoyuot->addWidget(pDateTimeFromLabel);
    m_pFromDateTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime().addDays(-1));
    pFilterHLoyuot->addWidget(m_pFromDateTimeEdit);



    QLabel * pDateTimeToLabel = new QLabel("по: ");
    pFilterHLoyuot->addWidget(pDateTimeToLabel);
    m_pToDateTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime());
    pFilterHLoyuot->addWidget(m_pToDateTimeEdit);

    pFilterHLoyuot->addStretch();

    /*Пользователи*/
    QLabel * pEmplLabel = new QLabel("Сотрудник:");
    pFilterHLoyuot->addWidget(pEmplLabel);

    m_pEmplComboBox = new QComboBox();
    pFilterHLoyuot->addWidget(m_pEmplComboBox);

    m_pEmplComboBox->addItem("Все" , QUuid());

    QString strEmplExec = QString("select id, Фамилия, Имя, Отчество from Пользователи where Роль='80066f83-c025-410b-b439-f3e9b2299461' and Удалено<>true");
    QSqlQuery EmplQuery;
    EmplQuery.exec(strEmplExec);
    while(EmplQuery.next())
    {
        m_pEmplComboBox->addItem(QString("%1 %2 %3").arg(EmplQuery.value(1).toString()).arg(EmplQuery.value(2).toString()).arg(EmplQuery.value(3).toString()) , EmplQuery.value(0));
    }

    pFilterHLoyuot->addStretch();


    /*Статья расходов*/
    QLabel * pStatiaLabel = new QLabel("Статья расходов:");
    pFilterHLoyuot->addWidget(pStatiaLabel);

    m_pStatiaComboBox = new QComboBox();
    pFilterHLoyuot->addWidget(m_pStatiaComboBox);

    m_pStatiaComboBox->addItem("Все" , QUuid());

    QString strStatiaExec = QString("select id, \"Статья расходов\" from \"Статьи расходов\" where Удалено<>true");
    QSqlQuery StatiaQuery;
    StatiaQuery.exec(strStatiaExec);
    while(StatiaQuery.next())
    {
        m_pStatiaComboBox->addItem(QString("%1").arg(StatiaQuery.value(1).toString()) , StatiaQuery.value(0));
    }
    m_pStatiaComboBox->addItem("Все" , QUuid());

    pFilterHLoyuot->addStretch();


    /*Товар*/
    QLabel * pItemTypeLabel = new QLabel("Товар:");
    pFilterHLoyuot->addWidget(pItemTypeLabel);

    m_pItemTypeComboBox = new QComboBox();
    pFilterHLoyuot->addWidget(m_pItemTypeComboBox);

    m_pItemTypeComboBox->addItem("Все" , QUuid());

    QString strItemTypeExec = QString("select id, Товар from Товары");
    QSqlQuery ItemTypeQuery;
    ItemTypeQuery.exec(strItemTypeExec);
    while(ItemTypeQuery.next())
    {
        m_pItemTypeComboBox->addItem(ItemTypeQuery.value(1).toString() , ItemTypeQuery.value(0));
    }
    m_pItemTypeComboBox->addItem("Все" , QUuid());

    pFilterHLoyuot->addStretch();

    QPushButton * pFilterApplyButton = new QPushButton("Применть фильтры");
    connect(pFilterApplyButton,SIGNAL(pressed()),this,SLOT(OnFilterApplyPressed()));
    pFilterHLoyuot->addWidget(pFilterApplyButton);

    pVMainLayout->addLayout(pFilterHLoyuot);

    m_pCostsTableWidget = new QTableWidget;
    m_pCostsTableWidget->setColumnCount(5);
    m_pCostsTableWidget->setColumnWidth(0 ,250);
    m_pCostsTableWidget->setColumnWidth(1 ,250);
    m_pCostsTableWidget->setColumnWidth(2 ,250);
    m_pCostsTableWidget->setColumnWidth(3 ,250);
    m_pCostsTableWidget->setColumnWidth(4 ,250);

    QStringList headers;
    headers << "Дата/время" << "Статья расходов" << "Товар"<<"Стоимость"<<"Сотрудник";
    m_pCostsTableWidget->setHorizontalHeaderLabels(headers);
    connect(m_pCostsTableWidget , SIGNAL(itemDoubleClicked(QTableWidgetItem*)) , this , SLOT(OnTasksDblClk(QTableWidgetItem*)));
    pVMainLayout->addWidget(m_pCostsTableWidget);

    OnFilterApplyPressed();
}


void QCostsWidget::UpdateCostsList()
{
    m_pCostsTableWidget->clear();
   // QString strExec= QString("SELECT Расходы.id, Расходы.\"Дата Время\" , \"Платежи сотрудников\".Сумма , Товары.Товар  FROM Расходы, \"Платежи сотрудников\", Товары where Расходы.Удалено<>'true' and Расходы.Товар=Товары.id and Расходы.Оплата=\"Платежи сотрудников\".id and Расходы.Сотрудник='%1' order by Расходы.\"Дата Время\" desc").arg(m_userUuid.toString());
    QString strExec= QString("SELECT Расходы.id, Расходы.\"Дата Время\" , \"Платежи сотрудников\".Сумма , Товары.Товар , \"Статьи расходов\".\"Статья расходов\"  , Пользователи.Фамилия, Пользователи.Имя, Пользователи.Отчество FROM Расходы, \"Платежи сотрудников\", Товары , Пользователи , \"Статьи расходов\"  where Расходы.Удалено<>'true' and Расходы.Товар=Товары.id and Расходы.Оплата=\"Платежи сотрудников\".id and \"Статьи расходов\".id=Расходы.Статья  and Пользователи.id=Расходы.Сотрудник and Расходы.Удалено <> true %1 order by Расходы.\"Дата Время\" desc").arg(m_filtersStr);
    QSqlQuery Query;
    Query.exec(strExec);

    //QMessageBox::information(NULL , "ddasda" , strExec);

    if(Query.size() < 1) return;

    m_pCostsTableWidget->setRowCount(Query.size() + 1);//+1 для итого

    int iRowCounter = 0;
    double dblSumm = 0;

    while(Query.next())
    {
        /*Дата время*/
        QTableWidgetItem * pItem = new QTableWidgetItem(QDateTime::fromSecsSinceEpoch(Query.value(1).toInt()).toString("dd.MM.yyyy hh:mm"));
        pItem->setData(Qt::UserRole , Query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pCostsTableWidget->setItem(iRowCounter , 0,  pItem);

        /*Статья*/
        pItem = new QTableWidgetItem(Query.value(4).toString());
        pItem->setData(Qt::UserRole , Query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pCostsTableWidget->setItem(iRowCounter , 1,  pItem);

        /*Товар*/
        pItem = new QTableWidgetItem(Query.value(3).toString());
        pItem->setData(Qt::UserRole , Query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pCostsTableWidget->setItem(iRowCounter , 2,  pItem);

        /*Стоимость*/
        pItem = new QTableWidgetItem(Query.value(2).toString());
        pItem->setData(Qt::UserRole , Query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pCostsTableWidget->setItem(iRowCounter , 3,  pItem);
        dblSumm = dblSumm + Query.value(2).toDouble();

        /*Сотрудник*/
        pItem = new QTableWidgetItem(QString("%1 %2 %3").arg(Query.value(5).toString()).arg(Query.value(6).toString()).arg(Query.value(7).toString()));
        pItem->setData(Qt::UserRole , Query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pCostsTableWidget->setItem(iRowCounter , 4,  pItem);

        iRowCounter++;
    }


    /*Добавим Итого*/
    m_pCostsTableWidget->setSpan(iRowCounter , 0 , 1 , 5);
    QTableWidgetItem * pItem = new QTableWidgetItem(QString("Итого: %1 руб.").arg(dblSumm));
    m_pCostsTableWidget->setItem(iRowCounter , 0,  pItem);
}

void QCostsWidget::OnCostsDblClk(QTableWidgetItem* item)
{

}

void QCostsWidget::OnFilterApplyPressed()
{
    QString emplFilterString = " ";
    if(m_pEmplComboBox->currentData()!=QVariant(QUuid()))
    {
        emplFilterString=QString(" and Расходы.Сотрудник='%1' ").arg(m_pEmplComboBox->currentData().toUuid().toString());
    }

    QString ItemTypeFilter = " ";
    if(m_pItemTypeComboBox->currentData()!=QVariant(QUuid()))
    {
        ItemTypeFilter=QString(" and Расходы.Товар='%1' ").arg(m_pItemTypeComboBox->currentData().toUuid().toString());
    }

    QString StatiaFilter = " ";
    if(m_pStatiaComboBox->currentData()!=QVariant(QUuid()))
    {
        ItemTypeFilter=QString(" and Расходы.Статья='%1' ").arg(m_pStatiaComboBox->currentData().toUuid().toString());
    }

    m_filtersStr = QString("and Расходы.\"Дата Время\">'%1' and Расходы.\"Дата Время\"<'%2' %3 %4 %5").arg(m_pFromDateTimeEdit->dateTime().toSecsSinceEpoch()).arg(m_pToDateTimeEdit->dateTime().toSecsSinceEpoch()).arg(emplFilterString).arg(ItemTypeFilter).arg(StatiaFilter);


    UpdateCostsList();
}

void QCostsWidget::ItemTypeComboChanged(int idx)
{

}

void QCostsWidget::StatiaComboChanged(int idx)
{

}
