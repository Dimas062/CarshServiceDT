#include "qtaskswidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QUuid>
#include <QSqlQuery>
#include <QLineEdit>
#include <QDateTime>
#include "../Carshs/qcarshpenaltyparktaskcarddlg.h"
#include "../Carshs/qcarshplatetaskcarddlg.h"
#include "../Carshs/qcarshparkingtaskcarddlg.h"
#include "../Carshs/qrettozonecarddlg.h"
#include <QSplashScreen>
#include <QPushButton>
#include <QTableWidgetItem>
#include "BDPatterns.h"
#include <QFont>

QTasksWidget::QTasksWidget(QWidget *parent)
    : QWidget{parent}
{
    m_filtersStr = QString("");

    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    QHBoxLayout * pFilterHLoyuot = new QHBoxLayout();

    pFilterHLoyuot->addStretch();

    /*Типы задач*/

    QLabel * pTaskTypeLabel = new QLabel("Тип задачи");
    pFilterHLoyuot->addWidget(pTaskTypeLabel);
    m_pTaskTypeComboBox = new QComboBox();
    pFilterHLoyuot->addWidget(m_pTaskTypeComboBox);


    m_pTaskTypeComboBox->addItem("Все" , QUuid());

    QString strTaskTypesExec = QString("select id,Тип from \"Типы задач\"");
    QSqlQuery TaskTypesQuery;
    TaskTypesQuery.exec(strTaskTypesExec);
    while(TaskTypesQuery.next())
    {
        m_pTaskTypeComboBox->addItem(TaskTypesQuery.value(1).toString() , TaskTypesQuery.value(0));
    }

    connect(m_pTaskTypeComboBox , SIGNAL(currentIndexChanged(int)), this , SLOT(TaskTypeComboChanged(int)));

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


    /*Заказчики*/
    QLabel * pCarshsLabel = new QLabel("Зказчик:");
    pFilterHLoyuot->addWidget(pCarshsLabel);

    m_pCarshsComboBox = new QComboBox();
    pFilterHLoyuot->addWidget(m_pCarshsComboBox);

    m_pCarshsComboBox->addItem("Все" , QUuid());

    QString strCarshsExec = QString("select id, Название from Заказчики where Удалено<>true");
    QSqlQuery CarshsQuery;
    CarshsQuery.exec(strCarshsExec);
    while(CarshsQuery.next())
    {
        m_pCarshsComboBox->addItem(CarshsQuery.value(1).toString() , CarshsQuery.value(0));
    }

    pFilterHLoyuot->addStretch();


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

    QLabel * pNumberLabel = new QLabel("ГРЗ: ");
    pFilterHLoyuot->addWidget(pNumberLabel);
    m_pNumberEdit = new QLineEdit();
    pFilterHLoyuot->addWidget(m_pNumberEdit);

    pFilterHLoyuot->addStretch();

    QPushButton * pFilterApplyButton = new QPushButton("Применть фильтры");
    connect(pFilterApplyButton,SIGNAL(pressed()),this,SLOT(OnFilterApplyPressed()));
    pFilterHLoyuot->addWidget(pFilterApplyButton);

    pVMainLayout->addLayout(pFilterHLoyuot);

    pVMainLayout->addSpacing(5);

    m_pTasksTableWidget = new QTableWidget;
    m_pTasksTableWidget->setColumnCount(7);
    //m_pTasksTableWidget->setColumnHidden(2,true);  //Скрыли зазказчика
    QStringList headers;
    headers << "Дата/время" << "Задача" << "Заказчик"<<"Сумма сотруднику"<<"ГРЗ"<<"Сотрудник"<<"Затраты";
    m_pTasksTableWidget->setHorizontalHeaderLabels(headers);
    connect(m_pTasksTableWidget , SIGNAL(itemDoubleClicked(QTableWidgetItem*)) , this , SLOT(OnTasksDblClk(QTableWidgetItem*)));
    pVMainLayout->addWidget(m_pTasksTableWidget);

    /*Лаяуты с фиьтрами типов задач*/
    QHBoxLayout * pAddFiltresLayout = new QHBoxLayout;
    m_pPenalParkTaskFilterWidget = new QPenalParkTaskFilterWidget;
    m_pPenalParkTaskFilterWidget->hide();
    pAddFiltresLayout->addWidget(m_pPenalParkTaskFilterWidget);

    pVMainLayout->addLayout(pAddFiltresLayout);
    OnFilterApplyPressed();
}

void QTasksWidget::TaskTypeComboChanged(int )
{

    /*Выбран тип Штрафстоянка*/
    if(m_pTaskTypeComboBox->currentData().toUuid() == QUuid::fromString("8078b7ce-e423-49ae-9ce6-17758b852b33"))
    {
        m_pPenalParkTaskFilterWidget->show();
        return;
    }

    /*Выбраны все типы или один из типов без доп. панели фильтров*/
    m_pPenalParkTaskFilterWidget->hide();
}

void QTasksWidget::OnTasksDblClk(QTableWidgetItem* item)
{
    if(item->data(Qt::UserRole+1).toUuid() == QUuid::fromString("8078b7ce-e423-49ae-9ce6-17758b852b33"))
    {
        QPixmap pixmap(":/icons/CarshServiceIcon256.png");
        QSplashScreen splash(pixmap);
        splash.show();
        splash.showMessage("Загрузка...");
        QCarshPenaltyParkTaskCardDlg dlg(item->data(Qt::UserRole).toString());
        splash.finish(&dlg);
        dlg.exec();
    }
    if(item->data(Qt::UserRole+1).toUuid() == QUuid::fromString("99b4e860-5a7b-42a4-9136-f96252ef4192"))
    {
        QPixmap pixmap(":/icons/CarshServiceIcon256.png");
        QSplashScreen splash(pixmap);
        splash.show();
        splash.showMessage("Загрузка...");
        QCarshPlateTaskCardDlg dlg(item->data(Qt::UserRole).toString());
        splash.finish(&dlg);
        dlg.exec();
    }
    if(item->data(Qt::UserRole+1).toUuid() == QUuid::fromString("057b3b6f-2848-479b-a546-3f16cb531ffe"))
    {
        QPixmap pixmap(":/icons/CarshServiceIcon256.png");
        QSplashScreen splash(pixmap);
        splash.show();
        splash.showMessage("Загрузка...");
        QCarshParkingTaskCardDlg dlg(item->data(Qt::UserRole).toString());
        splash.finish(&dlg);
        dlg.exec();
    }

    if(item->data(Qt::UserRole+1).toUuid() == QUuid::fromString("fe81daf9-a838-4bac-84aa-595e038d3a12"))
    {
        QPixmap pixmap(":/icons/CarshServiceIcon256.png");
        QSplashScreen splash(pixmap);
        splash.show();
        splash.showMessage("Загрузка...");
        QRetToZoneCardDlg dlg(item->data(Qt::UserRole).toString());
        splash.finish(&dlg);
        dlg.exec();
    }
}

void QTasksWidget::UpdateTasksList()
{
    m_pTasksTableWidget->clear();
    m_pTasksTableWidget->clearSpans();
    m_pTasksTableWidget->setRowCount(0);
    m_pTasksTableWidget->setColumnCount(7);

    /*Задачи пользователей*/

    QStringList headers;
    headers << "Дата/время" << "Задача" << "Заказчик"<<"Сумма сотруднику"<<"ГРЗ"<<"Сотрудник"<<"Затраты";

    /*Выбрана штрафстоянка - доп. столбцы штрафстоянки*/
    if(m_pTaskTypeComboBox->currentData().toUuid()==QUuid::fromString("8078b7ce-e423-49ae-9ce6-17758b852b33"))
    {
        headers<<"Причина задержания";
        m_pTasksTableWidget->setColumnCount(8);
    }

    m_pTasksTableWidget->setHorizontalHeaderLabels(headers);
    QString strQuery =  QString("SELECT Задачи.id, Задачи.\"Дата Время\", \"Типы задач\".\"Тип\" , \"Типы задач\".id , Задачи.\"Время выполнения\" , Заказчики.Название , Задачи.Цена , %3 , Пользователи.Имя, Пользователи.Фамилия , Пользователи. Отчество ,%4 FROM \"Типы задач\", Задачи, Заказчики, Пользователи where Заказчики.id=Задачи.Заказчик and Задачи.Исполнитель=Пользователи.id and Задачи.Тип = \"Типы задач\".id and Задачи.Удалено<> 'true'  %2 order by Задачи.\"Дата Время\" desc").arg(m_filtersStr).arg(NUMBER_BY_TASK).arg(PAY_BY_TASK);

    QSqlQuery query;
    query.exec(strQuery);

    if(query.size() < 1) return;

    m_pTasksTableWidget->setRowCount(query.size() + 1);//+1 для итого

    int iRowCounter = 0;
    double dblSumm = 0;
    double dblPay = 0;
    while(query.next())
    {

        // QTableWidgetItem * pItem = new QTableWidgetItem(QString("%1 - %2 (%3). %4 руб.").arg(QDateTime::fromSecsSinceEpoch(query.value(1).toInt()).toString("dd.MM.yyyy hh:mm")).arg(query.value(2).toString()).arg(query.value(5).toString()).arg(query.value(6).toString()));
        QTableWidgetItem * pItem = new QTableWidgetItem(QDateTime::fromSecsSinceEpoch(query.value(1).toInt()).toString("dd.MM.yyyy hh:mm"));
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 0,  pItem);

        pItem = new QTableWidgetItem(query.value(2).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 1,  pItem);

        /*Заказчик*/
        pItem = new QTableWidgetItem(query.value(5).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 2,  pItem);

        /*Сумма*/
        pItem = new QTableWidgetItem(query.value(6).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 3,  pItem);
        dblSumm = dblSumm + query.value(6).toDouble();

        /*ГРЗ*/
        pItem = new QTableWidgetItem(query.value(7).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 4,  pItem);

        /*Сотрудник*/
        pItem = new QTableWidgetItem(QString("%1 %2 %3").arg(query.value(8).toString()).arg(query.value(9).toString()).arg(query.value(10).toString()));
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 5,  pItem);

        /*Затраты*/
        pItem = new QTableWidgetItem(query.value(11).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 6,  pItem);
        dblPay = dblPay + query.value(11).toDouble();

        /*Выбрана штрафстоянка - доп. столбцы штрафстоянки*/
        if(m_pTaskTypeComboBox->currentData().toUuid()==QUuid::fromString("8078b7ce-e423-49ae-9ce6-17758b852b33"))
        {
            QString strPenParkQuery =  QString("SELECT \"Причины задержания\".Название from \"Причины задержания\" , Задачи , \"Расширение задачи ШС\" where Задачи.id='%1' and Задачи.Расширение = \"Расширение задачи ШС\".id and \"Расширение задачи ШС\".\"Причина задержания\"=\"Причины задержания\".id").arg(query.value(0).toString());

            QSqlQuery PenParkquery;
            PenParkquery.exec(strPenParkQuery);
            while(PenParkquery.next())
            {
                pItem = new QTableWidgetItem(PenParkquery.value(0).toString());
                pItem->setData(Qt::UserRole , query.value(0));
                pItem->setData(Qt::UserRole +1 , query.value(3));
                pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                m_pTasksTableWidget->setItem(iRowCounter , 7,  pItem);
            }
        }

        iRowCounter++;
    }

    /*Задачи партнёров*/

    /*Добавим Итого*/
    m_pTasksTableWidget->setSpan(iRowCounter , 0 , 1 , 5);
    QTableWidgetItem * pItem = new QTableWidgetItem(QString("Итого: %1 руб.").arg(dblSumm));
    m_pTasksTableWidget->setItem(iRowCounter , 0,  pItem);

    pItem = new QTableWidgetItem(QString(" %1 руб.").arg(dblPay));
    m_pTasksTableWidget->setItem(iRowCounter , 6,  pItem);

    QFont font;
    font.setBold(true);
    m_pTasksTableWidget->item(iRowCounter, 0)->setFont(font);
}

void QTasksWidget::OnFilterApplyPressed()
{
    //TODO: В union добавлять все задачи, в которых ест госномер
    QString numberFilterString = "";
    if(m_pNumberEdit->text().length()>0)
    {
        numberFilterString=QString(" and Задачи.\"Расширение\" in ((select id from \"Расширение задачи Возврат в зону\" where Госномер ilike '%%1%')  union  (select id from \"Расширение задачи Номера\" where Госномер ilike '%%1%') union  (select id from \"Расширение задачи Парковка\" where Госномер ilike '%%1%') union  (select id from \"Расширение задачи ШС\" where Госномер ilike '%%1%'))").arg(m_pNumberEdit->text());
    }

    QString typeFilterString;

    if(m_pTaskTypeComboBox->currentData().toUuid()!=QUuid())
    {
        typeFilterString=QString(" and Задачи.Тип='%1' ").arg(m_pTaskTypeComboBox->currentData().toUuid().toString());

        /*Выбрана штрафстоянка - доп. фильтры штрафстоянки*/
        if(m_pTaskTypeComboBox->currentData().toUuid()==QUuid::fromString("8078b7ce-e423-49ae-9ce6-17758b852b33"))
        {
            QString strPenParkFilter = "";

            if(m_pPenalParkTaskFilterWidget->m_pGibddCombo->currentData()!=QVariant(QUuid()))
            {
                strPenParkFilter = QString("  and Задачи.Расширение in (select  \"Расширение задачи ШС\".id from \"Расширение задачи ШС\" where Задачи.Расширение=\"Расширение задачи ШС\".id and \"Расширение задачи ШС\".\"Отдел ГАИ\"='%1')  ").arg(m_pPenalParkTaskFilterWidget->m_pGibddCombo->currentData().toString());
                typeFilterString = typeFilterString.append(strPenParkFilter);
            }
            if(m_pPenalParkTaskFilterWidget->m_pParkingCombo->currentData()!=QVariant(QUuid()))
            {
                strPenParkFilter = QString("  and Задачи.Расширение in (select  \"Расширение задачи ШС\".id from \"Расширение задачи ШС\" where Задачи.Расширение=\"Расширение задачи ШС\".id and \"Расширение задачи ШС\".\"Штрафстоянка\"='%1')  ").arg(m_pPenalParkTaskFilterWidget->m_pParkingCombo->currentData().toString());
                typeFilterString = typeFilterString.append(strPenParkFilter);
            }
            if(m_pPenalParkTaskFilterWidget->m_pReasonCombo->currentData()!=QVariant(QUuid()))
            {
                strPenParkFilter = QString("  and Задачи.Расширение in (select  \"Расширение задачи ШС\".id from \"Расширение задачи ШС\" where Задачи.Расширение=\"Расширение задачи ШС\".id and \"Расширение задачи ШС\".\"Причина задержания\"='%1')  ").arg(m_pPenalParkTaskFilterWidget->m_pReasonCombo->currentData().toString());
                typeFilterString = typeFilterString.append(strPenParkFilter);
            }

        }
    }

    QString emplFilterString;
    if(m_pEmplComboBox->currentData()!=QVariant(QUuid()))
    {
        emplFilterString=QString(" and Задачи.Исполнитель='%1' ").arg(m_pEmplComboBox->currentData().toUuid().toString());
    }

    QString CarshsFilterString;
    if(m_pCarshsComboBox->currentData()!=QVariant(QUuid()))
    {
        CarshsFilterString=QString(" and Заказчики.id='%1' ").arg(m_pCarshsComboBox->currentData().toUuid().toString());
    }

    m_filtersStr = QString("and Задачи.\"Дата Время\">'%1' and Задачи.\"Дата Время\"<'%2' %3 %4 %5 %6").arg(m_pFromDateTimeEdit->dateTime().toSecsSinceEpoch()).arg(m_pToDateTimeEdit->dateTime().toSecsSinceEpoch()).arg(typeFilterString).arg(numberFilterString).arg(emplFilterString).arg(CarshsFilterString);
    UpdateTasksList();
}
