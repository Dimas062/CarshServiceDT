#include "qpenparkgraphwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSqlQuery>
#include <QPushButton>
#include <QUuid>


QPenParkGraphWidget::QPenParkGraphWidget(QWidget *parent)
    : QWidget{parent}
{
    qDebug()<<"1";
    QVBoxLayout * pVMainLayout = new QVBoxLayout;

    QHBoxLayout * pHFilterLayout = new QHBoxLayout;

    /*Период*/

    QLabel * pDateTimeFromLabel = new QLabel("c: ");
    pHFilterLayout->addWidget(pDateTimeFromLabel);
    m_pFromDateEdit = new QDateEdit(QDateTime::currentDateTime().addMonths(-1).date());
    pHFilterLayout->addWidget(m_pFromDateEdit);


    qDebug()<<"1.1";
    QLabel * pDateTimeToLabel = new QLabel("по: ");
    pHFilterLayout->addWidget(pDateTimeToLabel);
    m_pToDateEdit = new QDateEdit(QDateTime::currentDateTime().date());
    pHFilterLayout->addWidget(m_pToDateEdit);

    pHFilterLayout->addStretch();

    m_pParkingCombo = new QComboBox;
    pHFilterLayout->addWidget(m_pParkingCombo);
    qDebug()<<"1.2";
    QString strParkingQuery = "select id, Название from Штрафстоянки";
    QSqlQuery query;
    query.exec(strParkingQuery);
    qDebug()<<"1.2.1";
    while(query.next())
    {
        qDebug()<<"1.2.2";
        m_pParkingCombo->addItem(query.value(1).toString() , query.value(0));
        qDebug()<<"1.2.3";
    }
    // qDebug()<<"1.3";
    m_pParkingCombo->addItem("Bce" , QUuid());
    m_pParkingCombo->setCurrentIndex(m_pParkingCombo->findData(QUuid()));
    // qDebug()<<"1.4";

    QPushButton * pFilterApplyButton = new QPushButton("Применть фильтры");
    connect(pFilterApplyButton,SIGNAL(pressed()),this,SLOT(OnFilterApplyPressed()));
    pHFilterLayout->addWidget(pFilterApplyButton);
    qDebug()<<"1.5";
    pVMainLayout->addLayout(pHFilterLayout);

    this->setLayout(pVMainLayout);

    m_pSeries = new QBarSeries();

    m_pChart = new QChart();
    m_pChart->addSeries(m_pSeries);
    m_pChart->setTitle("Title");
    m_pChart->setAnimationOptions(QChart::SeriesAnimations);

    m_pAxisX = new QBarCategoryAxis();
    m_pChart->addAxis(m_pAxisX, Qt::AlignBottom);
    m_pSeries->attachAxis(m_pAxisX);

    m_pAxisY = new QValueAxis();

    m_pChart->addAxis(m_pAxisY, Qt::AlignLeft);
    m_pSeries->attachAxis(m_pAxisY);

    m_pChart->legend()->setVisible(true);
    m_pChart->legend()->setAlignment(Qt::AlignBottom);

    m_pChartView = new QChartView(m_pChart, this);
    m_pChartView->setRenderHint(QPainter::Antialiasing);

    pVMainLayout->addWidget(m_pChartView);

    m_pBarSet  = new QBarSet("Test");

   UpdateGraph();
}

void QPenParkGraphWidget::OnFilterApplyPressed()
{
    UpdateGraph();
}


void QPenParkGraphWidget::UpdateGraph()
{

    m_pBarSet->remove(0 , m_pBarSet->count());

    m_pAxisX->clear();

    QStringList categories;
    int iMaxCash = 0;

    if(m_pParkingCombo->currentData()!=QUuid()) //Выбрана одна из стоянок
    {
        //Получаем номера месяцев
        QDate from = m_pFromDateEdit->date();
        QDate to = m_pToDateEdit->date();

        QSqlQuery query;

        for(QDate date = from; date <= to; date = date.addMonths(1))
        {
            int time_from = QDateTime(QDate(date.year() , date.month() , 1) , QTime(0,0,0)).toSecsSinceEpoch();
            int time_to   = QDateTime(QDate(date.year() , date.month() , date.daysInMonth()) , QTime(23,59,59)).toSecsSinceEpoch();

            QString strMonthName = date.toString("MM.yyyy");

            QString strQuery = QString("select SUM(\"Платежи сотрудников\".Сумма) from \"Платежи сотрудников\" where \"Платежи сотрудников\".id in (select \"Расширение задачи ШС\".\"Оплата парковки\" from \"Расширение задачи ШС\",Задачи where \"Расширение задачи ШС\".id = Задачи.Расширение and Задачи.\"Дата Время\">%1 and  Задачи.\"Дата Время\"<=%2 and \"Расширение задачи ШС\".Штрафстоянка='%3')").arg(time_from).arg(time_to).arg(m_pParkingCombo->currentData().toString());
            qDebug()<<strQuery;
            query.exec(strQuery);
            while(query.next())
            {
                *m_pBarSet<< query.value(0).toInt();
                if(iMaxCash<query.value(0).toInt()) iMaxCash = query.value(0).toInt();
                categories<< strMonthName;
            }
        }

        m_pAxisY->setRange(0,iMaxCash);
        m_pSeries->append(m_pBarSet);
        m_pAxisX->append(categories);
        m_pChart->setTitle(m_pParkingCombo->currentText());
        m_pBarSet->setLabel("Месяцы");

    }
    else
    {
        QDate from = m_pFromDateEdit->date();
        QDate to = m_pToDateEdit->date();

        QSqlQuery query;


        int time_from = QDateTime(QDate(from.year() , from.month() , 1) , QTime(0,0,0)).toSecsSinceEpoch();
        int time_to   = QDateTime(QDate(to.year() , to.month() , to.daysInMonth()) , QTime(23,59,59)).toSecsSinceEpoch();
        qDebug()<<"2";
        QString strQuery = QString("select SUM(\"Платежи сотрудников\".Сумма),Штрафстоянки.Название from \"Платежи сотрудников\",Штрафстоянки where \"Платежи сотрудников\".id in (select \"Расширение задачи ШС\".\"Оплата парковки\" from \"Расширение задачи ШС\",Задачи where \"Расширение задачи ШС\".id = Задачи.Расширение and Задачи.\"Дата Время\">%1 and  Задачи.\"Дата Время\"<=%2 and \"Расширение задачи ШС\".Штрафстоянка=Штрафстоянки.id) group by Штрафстоянки.Название").arg(time_from).arg(time_to);
        qDebug()<<strQuery;
        query.exec(strQuery);
        while(query.next())
        {
            *m_pBarSet<< query.value(0).toInt();
            if(iMaxCash<query.value(0).toInt()) iMaxCash = query.value(0).toInt();
            categories<< query.value(1).toString();
            qDebug()<<"2 "<<query.value(1).toString();
        }

        m_pAxisY->setRange(0,iMaxCash);
        m_pSeries->append(m_pBarSet);
        m_pAxisX->append(categories);
        m_pChart->setTitle(" ");
        m_pBarSet->setLabel("Штрафстоянки");

    }
}
