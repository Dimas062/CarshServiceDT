#include "qempltaskswidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QUuid>
#include <QSqlQuery>
#include <QLineEdit>
#include <QDateTime>
#include "../../Carshs/qcarshpenaltyparktaskcarddlg.h"
#include "../../Carshs/qcarshplatetaskcarddlg.h"
#include "../../Carshs/qcarshparkingtaskcarddlg.h"
#include "../../Carshs/qrettozonecarddlg.h"
#include "../../Carshs/qsmenataskdlg.h"
//#include "../../Carshs/qdocstaskdlg.h"
#include "../CarshService/tasks/qdocstaskdlg.h"
#include <QSplashScreen>
#include <QPushButton>
#include <QTableWidgetItem>
#include "BDPatterns.h"
#include <QFont>

QEmplTasksWidget::QEmplTasksWidget(QWidget *parent)
    : QWidget{parent}
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
    //m_pTasksTableWidget->setColumnHidden(2,true);  //Скрыли зазказчика
    QStringList headers;
    headers << "Дата/время" << "Задача" << "Заказчик"<<"Сумма"<<"ГРЗ";
    m_pTasksTableWidget->setHorizontalHeaderLabels(headers);
    connect(m_pTasksTableWidget , SIGNAL(itemDoubleClicked(QTableWidgetItem*)) , this , SLOT(OnTasksDblClk(QTableWidgetItem*)));
    pVMainLayout->addWidget(m_pTasksTableWidget);
    m_pTasksTableWidget->resizeColumnsToContents();
    OnFilterApplyPressed();
}

void QEmplTasksWidget::OnTasksDblClk(QTableWidgetItem* item)
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
        QDocsTaskDlg dlg;//(item->data(Qt::UserRole).toString());
        dlg.LoadDataFromBD(QUuid::fromString(item->data(Qt::UserRole).toString()));
        splash.finish(&dlg);
        dlg.exec();
    }
}

void QEmplTasksWidget::UpdateTasksList()
{
    m_pTasksTableWidget->clear();
    m_pTasksTableWidget->clearSpans();
    m_pTasksTableWidget->setRowCount(0);

    QStringList headers;
    headers << "Дата/время" << "Задача" << "Заказчик"<<"Сумма"<<"ГРЗ";
    m_pTasksTableWidget->setHorizontalHeaderLabels(headers);
    QString strQuery =  QString("SELECT Задачи.id, Задачи.\"Дата Время\", \"Типы задач\".\"Тип\" , \"Типы задач\".id , Задачи.\"Время выполнения\" , Заказчики.Название , Задачи.Цена , %3 FROM \"Типы задач\", Задачи, Заказчики where Заказчики.id=Задачи.Заказчик and Задачи.Тип = \"Типы задач\".id and Задачи.Удалено<> 'true' and Задачи.Исполнитель='%1' %2 order by Задачи.\"Дата Время\" desc").arg(m_strUuidCurrentUser).arg(m_filtersStr).arg(NUMBER_BY_TASK);

    QSqlQuery query;
    query.exec(strQuery);

    if(query.size() < 1) return;

    m_pTasksTableWidget->setRowCount(query.size() + 1);//+1 для итого

    int iRowCounter = 0;
    double dblSumm = 0;
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
        if(query.value(6).toInt() == 0) pItem->setBackground(QBrush(Qt::red));
        m_pTasksTableWidget->setItem(iRowCounter , 3,  pItem);
        dblSumm = dblSumm + query.value(6).toDouble();

        /*ГРЗ*/
        pItem = new QTableWidgetItem(query.value(7).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 4,  pItem);

        iRowCounter++;
    }

    /*Добавим Итого*/
    m_pTasksTableWidget->setSpan(iRowCounter , 0 , 1 , 5);
    QTableWidgetItem * pItem = new QTableWidgetItem(QString("Итого: %1 руб.").arg(dblSumm));
    m_pTasksTableWidget->setItem(iRowCounter , 0,  pItem);

    QFont font;
    font.setBold(true);
    m_pTasksTableWidget->item(iRowCounter, 0)->setFont(font);

    m_pTasksTableWidget->resizeColumnsToContents();
}

void QEmplTasksWidget::OnFilterApplyPressed()
{
    //TODO: В union добавлять все задачи, в которых ест госномер
    QString numberFilterString = "";
    if(m_pNumberEdit->text().length()>0)
    {
        numberFilterString=QString(" and Задачи.\"Расширение\" in ((select id from \"Расширение задачи Возврат в зону\" where Госномер ilike '%%1%')  union  (select id from \"Расширение задачи Номера\" where Госномер ilike '%%1%') union  (select id from \"Расширение задачи Парковка\" where Госномер ilike '%%1%') union  (select id from \"Расширение задачи ШС\" where Госномер ilike '%%1%'))").arg(m_pNumberEdit->text());
    }

    QString typeFilterString;

    if(m_pTaskTypeComboBox->currentData()!=QVariant(QUuid()))
    {
        typeFilterString=QString(" and Задачи.Тип='%1' ").arg(m_pTaskTypeComboBox->currentData().toUuid().toString());
    }

    m_filtersStr = QString("and Задачи.\"Дата Время\">'%1' and Задачи.\"Дата Время\"<'%2' %3 %4").arg(m_pFromDateTimeEdit->dateTime().toSecsSinceEpoch()).arg(m_pToDateTimeEdit->dateTime().toSecsSinceEpoch()).arg(typeFilterString).arg(numberFilterString);
    UpdateTasksList();
}

void QEmplTasksWidget::SetActivEmpl(QString strUuid)
{
    m_strUuidCurrentUser = strUuid;
    UpdateTasksList();
}
