#include "qemplgraphwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSqlQuery>
#include <QPushButton>
#include <QUuid>



QEmplGraphWidget::QEmplGraphWidget(QWidget *parent)
    : QWidget{parent}
{

    QVBoxLayout * pVMainLayout = new QVBoxLayout;

    QHBoxLayout * pHFilterLayout = new QHBoxLayout;

    /*Период*/

    QLabel * pDateTimeFromLabel = new QLabel("c: ");
    pHFilterLayout->addWidget(pDateTimeFromLabel);
    m_pFromDateEdit = new QDateEdit(QDateTime::currentDateTime().addMonths(-1).date());
    pHFilterLayout->addWidget(m_pFromDateEdit);

    QLabel * pDateTimeToLabel = new QLabel("по: ");
    pHFilterLayout->addWidget(pDateTimeToLabel);
    m_pToDateEdit = new QDateEdit(QDateTime::currentDateTime().date());
    pHFilterLayout->addWidget(m_pToDateEdit);

    pHFilterLayout->addStretch();



    m_pEmplCombo = new QComboBox;
    pHFilterLayout->addWidget(m_pEmplCombo);

    QString strEmplQuery = "select id, Фамилия, Имя, Отчество from Пользователи where Роль='80066f83-c025-410b-b439-f3e9b2299461'";
    QSqlQuery query;
    query.exec(strEmplQuery);

    while(query.next())
    {
        m_pEmplCombo->addItem(query.value(1).toString() +" " + query.value(2).toString() +" " + query.value(3).toString(), query.value(0));
    }

    m_pEmplCombo->addItem("Bce" , QUuid());
    m_pEmplCombo->setCurrentIndex(m_pEmplCombo->findData(QUuid()));

    QPushButton * pFilterApplyButton = new QPushButton("Применть фильтры");
    connect(pFilterApplyButton,SIGNAL(pressed()),this,SLOT(OnFilterApplyPressed()));
    pHFilterLayout->addWidget(pFilterApplyButton);
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

void QEmplGraphWidget::OnFilterApplyPressed()
{
    UpdateGraph();
}


void QEmplGraphWidget::UpdateGraph()
{

    m_pBarSet->remove(0 , m_pBarSet->count());

    m_pAxisX->clear();

    QStringList categories;
    int iMaxCash = 0;

    if(m_pEmplCombo->currentData()!=QUuid()) //Выбран один из сотрудников
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

            QString strQuery = QString("select SUM(\"Платежи сотрудников\".Сумма) from \"Платежи сотрудников\" where \"Платежи сотрудников\".id in (select \"Расширение задачи ШС\".\"Оплата парковки\" from \"Расширение задачи ШС\",Задачи where \"Расширение задачи ШС\".id = Задачи.Расширение and Задачи.\"Дата Время\">%1 and  Задачи.\"Дата Время\"<=%2 and Задачи.Исполнитель='%3')").arg(time_from).arg(time_to).arg(m_pEmplCombo->currentData().toString());

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
        m_pChart->setTitle(m_pEmplCombo->currentText());
        m_pBarSet->setLabel("Месяцы");

    }
    // else
    // {
    //     QDate from = m_pFromDateEdit->date();
    //     QDate to = m_pToDateEdit->date();

    //     QSqlQuery query;


    //     int time_from = QDateTime(QDate(from.year() , from.month() , 1) , QTime(0,0,0)).toSecsSinceEpoch();
    //     int time_to   = QDateTime(QDate(to.year() , to.month() , to.daysInMonth()) , QTime(23,59,59)).toSecsSinceEpoch();

    //     QString strQuery = QString("select SUM(\"Платежи сотрудников\".Сумма),Штрафстоянки.Название from \"Платежи сотрудников\",Штрафстоянки where \"Платежи сотрудников\".id in (select \"Расширение задачи ШС\".\"Оплата парковки\" from \"Расширение задачи ШС\",Задачи where \"Расширение задачи ШС\".id = Задачи.Расширение and Задачи.\"Дата Время\">%1 and  Задачи.\"Дата Время\"<=%2 and \"Расширение задачи ШС\".Штрафстоянка=Штрафстоянки.id) group by Штрафстоянки.Название").arg(time_from).arg(time_to);

    //     query.exec(strQuery);
    //     while(query.next())
    //     {
    //         *m_pBarSet<< query.value(0).toInt();
    //         if(iMaxCash<query.value(0).toInt()) iMaxCash = query.value(0).toInt();
    //         categories<< query.value(1).toString();

    //     }

    //     m_pAxisY->setRange(0,iMaxCash);
    //     m_pSeries->append(m_pBarSet);
    //     m_pAxisX->append(categories);
    //     m_pChart->setTitle(" ");
    //     m_pBarSet->setLabel("Штрафстоянки");

    // }
}
