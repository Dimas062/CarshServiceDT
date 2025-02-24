#include "qcarshempltaskswidget.h"
//#include "common.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QUuid>
#include <QSqlQuery>
#include <QDateTime>
#include "qcarshpenaltyparktaskcarddlg.h"
#include "qcarshplatetaskcarddlg.h"
#include "qcarshparkingtaskcarddlg.h"
#include "qrettozonecarddlg.h"
#include "qsmenataskdlg.h"
#include "qdocstaskdlg.h"
#include <QSplashScreen>
#include "BDPatterns.h"
#include <QPushButton>


extern QUuid uuidCurrentUser;

QCarshEmplTasksWidget::QCarshEmplTasksWidget()
{
    m_filtersStr = QString("");

    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    QHBoxLayout * pFilterHLoyuot = new QHBoxLayout();

    pFilterHLoyuot->addStretch();

    QLabel * pTaskTypeLabel = new QLabel("Тип задачи");
    pFilterHLoyuot->addWidget(pTaskTypeLabel);
    m_pTaskTypeComboBox = new QComboBox();
    pFilterHLoyuot->addWidget(m_pTaskTypeComboBox);

    /*Загрузка типов задач*/
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

    QLabel * pDateTimeFromLabel = new QLabel("c: ");
    pFilterHLoyuot->addWidget(pDateTimeFromLabel);
    m_pFromDateTimeEdit = new QDateTimeEdit(QDateTime(QDate::currentDate().addDays(-(QDate::currentDate().day()) + 1) , QTime(0,0,0)));
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
    m_pTasksTableWidget->setColumnCount(5);
    QStringList headers;
    headers << "Дата/время" << "Задача" << "ГРЗ"<<"Сотрудник"<<"Затраты";;
    m_pTasksTableWidget->setHorizontalHeaderLabels(headers);
    connect(m_pTasksTableWidget , SIGNAL(itemDoubleClicked(QTableWidgetItem*)) , this , SLOT(OnTasksDblClk(QTableWidgetItem*)));
    pVMainLayout->addWidget(m_pTasksTableWidget);
    m_pTasksTableWidget->resizeColumnsToContents();

    /*Лаяуты с фиьтрами типов задач*/
    QHBoxLayout * pAddFiltresLayout = new QHBoxLayout;
    m_pPenalParkTaskFilterWidget = new QPenalParkTaskFilterWidget;
    m_pPenalParkTaskFilterWidget->hide();
    pAddFiltresLayout->addWidget(m_pPenalParkTaskFilterWidget);

    pVMainLayout->addLayout(pAddFiltresLayout);

    OnFilterApplyPressed();
}


void QCarshEmplTasksWidget::TaskTypeComboChanged(int )
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

void QCarshEmplTasksWidget::OnFilterApplyPressed()
{
    QString typeFilterString;

    if(m_pTaskTypeComboBox->currentData()!=QVariant(QUuid()))
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

    //TODO: В union добавлять все задачи, в которых есть госномер
    QString numberFilterString = "";
    if(m_pNumberEdit->text().length()>0)
    {
        numberFilterString=QString(" and Задачи.\"Расширение\" in ((select id from \"Расширение задачи Возврат в зону\" where Госномер ilike '%%1%')  union  (select id from \"Расширение задачи Номера\" where Госномер ilike '%%1%') union  (select id from \"Расширение задачи Парковка\" where Госномер ilike '%%1%') union  (select id from \"Расширение задачи ШС\" where Госномер ilike '%%1%'))").arg(m_pNumberEdit->text());
    }



    m_filtersStr = QString("and Задачи.\"Дата Время\">'%1' and Задачи.\"Дата Время\"<'%2' %3 %4").arg(m_pFromDateTimeEdit->dateTime().toSecsSinceEpoch()).arg(m_pToDateTimeEdit->dateTime().toSecsSinceEpoch()).arg(typeFilterString).arg(numberFilterString);
    UpdateTasksList();
}


void QCarshEmplTasksWidget::UpdateTasksList()
{
    m_pTasksTableWidget->clear();
    m_pTasksTableWidget->clearSpans();
    QStringList headers;
    headers << "Дата/время" << "Задача" << "ГРЗ"<<"Сотрудник"<<"Затраты";
    m_pTasksTableWidget->setHorizontalHeaderLabels(headers);
    QString strQuery = QString("select Задачи.id,Задачи.\"Дата Время\",\"Типы задач\".Тип , Задачи.Тип , %3 , Пользователи.Фамилия, Пользователи.Имя , Пользователи. Отчество ,  %4 from Задачи,\"Заказчик-Пользователи\",\"Типы задач\", Пользователи where Задачи.Заказчик=\"Заказчик-Пользователи\".Заказчик and \"Типы задач\".id=Задачи.Тип and Задачи.Исполнитель=Пользователи.id and Задачи.Удалено<>true and \"Заказчик-Пользователи\".Пользователь='%1' %2 order by Задачи.\"Дата Время\"").arg(uuidCurrentUser.toString()).arg(m_filtersStr).arg(NUMBER_BY_TASK).arg(PAY_BY_TASK);
    QSqlQuery query;
    query.exec(strQuery);
    m_pTasksTableWidget->setRowCount(query.size() + 1);
    int iRowCounter = 0;
    double dblPay = 0;
    while(query.next())
    {
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
        pItem = new QTableWidgetItem(query.value(4).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 2,  pItem);

        /*Сотрудник*/
        pItem = new QTableWidgetItem(QString("%1 %2 %3").arg(query.value(5).toString()).arg(query.value(6).toString()).arg(query.value(7).toString()));
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 3,  pItem);

        /*Затраты*/
        pItem = new QTableWidgetItem(query.value(8).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 4,  pItem);
        dblPay = dblPay + query.value(8).toDouble();

        iRowCounter++;
        // QListWidgetItem * pItem = new QListWidgetItem(QString("%1 - %2").arg(QDateTime::fromSecsSinceEpoch(query.value(1).toInt()).toString("dd.MM.yyyy hh:mm")).arg(query.value(2).toString()));
        // pItem->setData(Qt::UserRole , query.value(0));
        // pItem->setData(Qt::UserRole +1 , query.value(3));
        // m_pTasksListWidget->addItem(pItem);
    }

    /*Добавим Итого*/
    m_pTasksTableWidget->setSpan(iRowCounter , 0 , 1 , 5);
    QTableWidgetItem * pItem = new QTableWidgetItem(QString("Итого: %1 руб.").arg(dblPay));
    m_pTasksTableWidget->setItem(iRowCounter , 0,  pItem);


    QFont font;
    font.setBold(true);
    m_pTasksTableWidget->item(iRowCounter, 0)->setFont(font);
    m_pTasksTableWidget->resizeColumnsToContents();
}

void QCarshEmplTasksWidget::OnTasksDblClk(QTableWidgetItem* item)
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
    if(item->data(Qt::UserRole+1).toUuid() == QUuid::fromString("78850df8-814b-41c8-8977-945c085f3021"))
    {
        QPixmap pixmap(":/icons/CarshServiceIcon256.png");
        QSplashScreen splash(pixmap);
        splash.show();
        splash.showMessage("Загрузка...");
        QSmenaTaskDlg dlg(item->data(Qt::UserRole).toString());
        splash.finish(&dlg);
        dlg.exec();
    }

    if(item->data(Qt::UserRole+1).toUuid() == QUuid::fromString("25695573-f5fe-43fd-93dc-76ee09e461fa"))
    {
        QPixmap pixmap(":/icons/CarshServiceIcon256.png");
        QSplashScreen splash(pixmap);
        splash.show();
        splash.showMessage("Загрузка...");
        QDocsTaskDlg dlg(item->data(Qt::UserRole).toString());
        splash.finish(&dlg);
        dlg.exec();
    }
}
