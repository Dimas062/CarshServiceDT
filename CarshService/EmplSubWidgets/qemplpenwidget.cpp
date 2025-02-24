#include "qemplpenwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QUuid>
#include <QSqlQuery>
#include <QLineEdit>
#include <QDateTime>
#include <QPushButton>
#include "qemplpendlg.h"


QEmplPenWidget::QEmplPenWidget(QWidget *parent)
    : QWidget{parent}
{
   // m_filtersStr = QString("");
    m_strUuidCurrentUser = QString(" ");

    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    QHBoxLayout * pFilterHLoyuot = new QHBoxLayout();

    pFilterHLoyuot->addStretch();

    QLabel * pDateTimeFromLabel = new QLabel("c: ");
    pFilterHLoyuot->addWidget(pDateTimeFromLabel);
    m_pFromDateTimeEdit = new QDateTimeEdit(QDateTime(QDate::currentDate().addDays(-(QDate::currentDate().day()) + 1) , QTime(0,0,0)));
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

    m_pPenTableWidget = new QTableWidget;
    m_pPenTableWidget->setColumnCount(3);

    QStringList headers;
    headers << "Дата/время" << "Баллы" << "Комментарий";
    m_pPenTableWidget->setHorizontalHeaderLabels(headers);
    connect(m_pPenTableWidget , SIGNAL(itemDoubleClicked(QTableWidgetItem*)) , this , SLOT(OnPenDblClk(QTableWidgetItem*)));
    pVMainLayout->addWidget(m_pPenTableWidget);
    m_pPenTableWidget->resizeColumnsToContents();

    QHBoxLayout * pButtonsLayout = new QHBoxLayout;

    QPushButton * pAddButton = new QPushButton("Добавить");
    connect(pAddButton,SIGNAL(pressed()),this,SLOT(OnAddPen()));
    pButtonsLayout->addWidget(pAddButton);

    QPushButton * pRemoveButton = new QPushButton("Удалить");
    connect(pRemoveButton,SIGNAL(pressed()),this,SLOT(OnRemovePen()));
    pButtonsLayout->addWidget(pRemoveButton);

    pVMainLayout->addLayout(pButtonsLayout);
    OnFilterApplyPressed();


}

void QEmplPenWidget::OnPenDblClk(QTableWidgetItem* item)
{
    QString strPenUuid = item->data(Qt::UserRole).toString();
    QEmplPenDlg dlg(QUuid::fromString(strPenUuid) , m_strUuidCurrentUser);
    dlg.exec();
    UpdatePenList();
}

void QEmplPenWidget::OnFilterApplyPressed()
{

}

void QEmplPenWidget::OnAddPen()
{
    QEmplPenDlg dlg(QUuid() , m_strUuidCurrentUser);
    dlg.exec();
    UpdatePenList();
}

void QEmplPenWidget::OnRemovePen()
{
    QTableWidgetItem* item = m_pPenTableWidget->currentItem();
    if(item)
    {
        QString strPenUuid = item->data(Qt::UserRole).toString();
        QString strQuery = QString("delete from ПоощренияНаказания where id='%1'").arg(strPenUuid);

        QSqlQuery query;
        query.exec(strQuery);

        UpdatePenList();
    }
}

void QEmplPenWidget::SetActivEmpl(QString strUuid)
{
    m_strUuidCurrentUser = strUuid;
    UpdatePenList();
}

void QEmplPenWidget::UpdatePenList()
{
    m_pPenTableWidget->clear();
    m_pPenTableWidget->setRowCount(0);

    QStringList headers;
    headers << "Дата/время" << "Баллы" << "Комментарий";
    m_pPenTableWidget->setHorizontalHeaderLabels(headers);
    QString strQuery = QString("select id, ДатаВремя, Значение, Комментарий from ПоощренияНаказания where Сотрудник='%1' order by ДатаВремя").arg(m_strUuidCurrentUser);

    QSqlQuery query;
    query.exec(strQuery);


    m_pPenTableWidget->setRowCount(query.size());

    int iRowCounter = 0;

    while(query.next())
    {

        QTableWidgetItem * pItem = new QTableWidgetItem(QDateTime::fromSecsSinceEpoch(query.value(1).toInt()).toString("dd.MM.yyyy hh:mm"));
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pPenTableWidget->setItem(iRowCounter , 0,  pItem);

        pItem = new QTableWidgetItem(query.value(2).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pPenTableWidget->setItem(iRowCounter , 1,  pItem);


        pItem = new QTableWidgetItem(query.value(3).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pPenTableWidget->setItem(iRowCounter , 2,  pItem);

        iRowCounter++;
    }

    m_pPenTableWidget->resizeColumnsToContents();
}
