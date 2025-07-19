#include "qcarshinputpenparktaskswidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QUuid>
#include <QSqlQuery>
#include <QLineEdit>
#include <QDateTime>
#include <QPushButton>
#include "common.h"
#include "qcarshinputpenparkeditdlg.h"

QCarshInputPenParkTasksWidget::QCarshInputPenParkTasksWidget(QWidget *parent)
    : QWidget{parent}
{
    m_filtersStr = QString("");

    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    QHBoxLayout * pFilterHLoyuot = new QHBoxLayout();

    /*Период*/
    QLabel * pDateTimeFromLabel = new QLabel("c: ");
    pFilterHLoyuot->addWidget(pDateTimeFromLabel);
    m_pFromDateTimeEdit = new QDateTimeEdit(QDateTime(QDate::currentDate().addDays(-(QDate::currentDate().day()) + 1) , QTime(0,0,0)));
    pFilterHLoyuot->addWidget(m_pFromDateTimeEdit);

    QLabel * pDateTimeToLabel = new QLabel("по: ");
    pFilterHLoyuot->addWidget(pDateTimeToLabel);
    m_pToDateTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime());
    pFilterHLoyuot->addWidget(m_pToDateTimeEdit);

    QHBoxLayout * pCarshsHLayout = new QHBoxLayout;
    QLabel * pCarshsComboLabel = new QLabel("Служба каршеринга*");
    pCarshsHLayout->addWidget(pCarshsComboLabel);
    m_pCarshsCombo = new QComboBox();
    m_pCarshsCombo->setIconSize(QSize(100,30));
    pCarshsHLayout->addWidget(m_pCarshsCombo);
    pFilterHLoyuot->addLayout(pCarshsHLayout);

    /*Заполнение комбобокс со службами */
    QString strCarshsExec = "select id, Название, Логотип from Заказчики where Удалено=false";
    QSqlQuery query;
    query.exec(strCarshsExec);
    while(query.next())
    {
        QString strLogo(query.value(2).toString());

        QImage imgLogo = Base64ToImage(strLogo);

        QPixmap pxmpLogo = QPixmap::fromImage(imgLogo);

        m_pCarshsCombo->addItem(QIcon(pxmpLogo) , query.value(1).toString() , QVariant(query.value(0).toString()));
    }
    m_pCarshsCombo->addItem("Все" , QVariant(QUuid()));

    pFilterHLoyuot->addStretch();

    QPushButton * pFilterApplyButton = new QPushButton("Применить фильтры");
    connect(pFilterApplyButton,SIGNAL(pressed()),this,SLOT(OnFilterApplyPressed()));
    pFilterHLoyuot->addWidget(pFilterApplyButton);

    pVMainLayout->addLayout(pFilterHLoyuot);

    m_pTasksTableWidget = new QTableWidget;
    m_pTasksTableWidget->setColumnCount(3);
    pVMainLayout->addWidget(m_pTasksTableWidget);
    connect(m_pTasksTableWidget , SIGNAL(itemDoubleClicked(QTableWidgetItem*)) , this , SLOT(OnTasksDblClk(QTableWidgetItem*)));

    OnFilterApplyPressed();
}

void QCarshInputPenParkTasksWidget::OnTasksDblClk(QTableWidgetItem* item)
{
    QCarshInputPenParkEditDlg dlg(item->data(Qt::UserRole).toUuid() , item->data(Qt::UserRole + 1).toString() , item->data(Qt::UserRole + 2).toString() , item->data(Qt::UserRole + 3).toString());
    dlg.exec();
    UpdateTasksList();
}

void QCarshInputPenParkTasksWidget::UpdateTasksList()
{
    m_pTasksTableWidget->clear();

    QStringList headers;
    headers << "Дата/время помещения" << "Штрафстоянка" << "ГРЗ";
    m_pTasksTableWidget->setHorizontalHeaderLabels(headers);

    QString strExec= QString("SELECT ЗадачиЗаказчикаШС.Госномер, ЗадачиЗаказчикаШС.ДатаВремяПомещения , Штрафстоянки.Название , ЗадачиЗаказчикаШС.id from ЗадачиЗаказчикаШС, Штрафстоянки  where ЗадачиЗаказчикаШС.Штрафстоянка = Штрафстоянки.id and ЗадачиЗаказчикаШС.ПереведенаВЗадачу IS NULL %1 order by ЗадачиЗаказчикаШС.ДатаВремяПомещения").arg(m_filtersStr);
    QSqlQuery Query;
    Query.exec(strExec);

    if(Query.size() < 1) return;

    m_pTasksTableWidget->setRowCount(Query.size());

    int iRowCounter = 0;

    while(Query.next())
    {
        /*Дата время*/
        QString strTaskDateTime = QDateTime::fromSecsSinceEpoch(Query.value(1).toInt()).toString("dd.MM.yyyy hh:mm");
        QTableWidgetItem * pItem = new QTableWidgetItem(strTaskDateTime);
        pItem->setData(Qt::UserRole , Query.value(3)); //id задачи ШС от заказчика
        pItem->setData(Qt::UserRole + 1, Query.value(0).toString());//ГРЗ
        pItem->setData(Qt::UserRole + 2, strTaskDateTime);//Время
        pItem->setData(Qt::UserRole + 3, Query.value(2).toString());//Название ШС
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 0,  pItem);

        /*Штрафстоянка*/
        pItem = new QTableWidgetItem(Query.value(2).toString());
        pItem->setData(Qt::UserRole , Query.value(3)); //id задачи ШС от заказчика
        pItem->setData(Qt::UserRole + 1, Query.value(0).toString());//ГРЗ
        pItem->setData(Qt::UserRole + 2, strTaskDateTime);//Время
        pItem->setData(Qt::UserRole + 3, Query.value(2).toString());//Название ШС
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 1,  pItem);

        /*ГРЗ*/
        pItem = new QTableWidgetItem(Query.value(0).toString());
        pItem->setData(Qt::UserRole , Query.value(3)); //id задачи ШС от заказчика
        pItem->setData(Qt::UserRole + 1, Query.value(0).toString());//ГРЗ
        pItem->setData(Qt::UserRole + 2, strTaskDateTime);//Время
        pItem->setData(Qt::UserRole + 3, Query.value(2).toString());//Название ШС
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 2,  pItem);

        iRowCounter++;
    }

    m_pTasksTableWidget->resizeColumnsToContents();
}

void QCarshInputPenParkTasksWidget::OnFilterApplyPressed()
{
    QString carshFilterString = " ";
    if(m_pCarshsCombo->currentData()!=QVariant(QUuid()))
    {
        carshFilterString=QString(" and ЗадачиЗаказчикаШС.Заказчик='%1' ").arg(m_pCarshsCombo->currentData().toUuid().toString());
    }

    m_filtersStr = QString("and ЗадачиЗаказчикаШС.ДатаВремяПомещения>'%1' and ЗадачиЗаказчикаШС.ДатаВремяПомещения<'%2' %3").arg(m_pFromDateTimeEdit->dateTime().toSecsSinceEpoch()).arg(m_pToDateTimeEdit->dateTime().toSecsSinceEpoch()).arg(carshFilterString);

    UpdateTasksList();
}
