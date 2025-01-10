#include "qWashpartnertaskdlg.h"
#include "qWashpartnertaskwidget.h"
#include "service/xlspatterns.h"
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QString>
#include <QUuid>
#include <QVBoxLayout>
#include "common.h"

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

    QPushButton * pSchetButton = new QPushButton("Сформировать счет партнера");
    connect(pSchetButton,SIGNAL(pressed()),this,SLOT(OnSchetPressed()));
    pFilterHLoyuot->addWidget(pSchetButton);

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

    m_vCurrentSchetItems.clear();

    QStringList headers;
    headers  << "Дата/время" << "Точка" <<"Комментарий"<<"Заказчик"<<"Стоимость";
    m_pTasksTableWidget->setHorizontalHeaderLabels(headers);
    QString strQuery =  QString("select \"Задачи партнера Мойка\".id , \"Задачи партнера Мойка\".ДатаВремя, \"Точки Партнеров\".Название , \"Задачи партнера Мойка\".Комментарий, Поставщики.Название, Партнеры.Поставщик from \"Задачи партнера Мойка\", \"Точки Партнеров\" , Поставщики, Партнеры where Поставщики.id = Партнеры.Поставщик  and \"Задачи партнера Мойка\".Партнер = Партнеры.id  and \"Точки Партнеров\".id = \"Задачи партнера Мойка\".Точка and \"Задачи партнера Мойка\".Партнер='%1' %2").arg(m_strUuidCurrentPartner).arg(m_filtersStr);

    QSqlQuery query;
    query.exec(strQuery);

    if(query.size() < 1) return;

    m_pTasksTableWidget->setRowCount(query.size());

    int iRowCounter = 0;
    while(query.next())
    {
        SSchetItem schetItem;

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


        /*Комментарий*/
        pItem = new QTableWidgetItem(query.value(3).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 2,  pItem);

        /*Заказчик*/
        pItem = new QTableWidgetItem(query.value(4).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 3,  pItem);

        /*Детализация построчно (потипно) для счета, рассчет стоимости суммарно по каждой задача и штрафа суммарно для всех типов*/
        QString strTypesExec=QString("select \"Типы задач Мойка\".Цена, SUM(\"Задача Мойка - Типы\".Количество), \"Типы задач Мойка\".Тип, \"Задача Мойка - Типы\".Ночь , sum((select \"Отмена Мойки\".Количество from \"Отмена Мойки\" where \"Отмена Мойки\".Задача = \"Задача Мойка - Типы\".Задача and \"Отмена Мойки\".Тип = \"Задача Мойка - Типы\".Тип and \"Отмена Мойки\".Ночь = \"Задача Мойка - Типы\".Ночь and \"Отмена Мойки\".Удалено<>'true')) from \"Задача Мойка - Типы\" , \"Типы задач Мойка\" where \"Типы задач Мойка\".id = \"Задача Мойка - Типы\".Тип and  \"Задача Мойка - Типы\".Задача = '%1' group by \"Типы задач Мойка\".Тип , \"Задача Мойка - Типы\".Ночь , \"Типы задач Мойка\".Цена").arg(query.value(0).toString());
        QSqlQuery TypesQuery;
        QString strWorks;
        TypesQuery.exec(strTypesExec);
        int iItemCount = 0;
        double dblCost = 0;
        double dblPrice = 0;//Стоимость по всей задаче суммарно
        double dblPen = 0; //Штраф по всей задаче суммарно
        while(TypesQuery.next())
        {
            iItemCount = TypesQuery.value(1).toDouble();
            dblCost = TypesQuery.value(0).toDouble();

            dblPrice = dblPrice + dblCost * iItemCount;
            strWorks = TypesQuery.value(2).toString();
            if(TypesQuery.value(3).toBool())
                strWorks.append(" (ночь)");
            else strWorks.append(" (день)");

            schetItem.dblItemPrice = dblCost;

            int iSubPenCount = TypesQuery.value(4).toInt();

            dblPen = dblPen + iSubPenCount*dblCost;

            schetItem.dblCount = iItemCount - iSubPenCount;
            schetItem.strName = strWorks;
            schetItem.strUnitMeasure =" шт.";

            /*Ищем такие же типы задач (по названию) в уже имеющимся списке, при нахождении не добавляем новый элемент, а к существующему плюсуем дополнительное засчитываемое количество*/
            bool bIsCaurrentAdded = false;
            for(SSchetItem &item : m_vCurrentSchetItems)
            {
                if(item.strName == schetItem.strName)
                {
                    item.dblCount = item.dblCount + schetItem.dblCount;//nicht arbiten
                    bIsCaurrentAdded = true;
                    break;
                }
            }
            if(!bIsCaurrentAdded) m_vCurrentSchetItems.push_back(schetItem);

        }

        /*Стоимость*/
        QString strPrice = QString(" %1 руб.").arg(dblPrice - dblPen);
        pItem = new QTableWidgetItem(strPrice);
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 4,  pItem);

        iRowCounter++;

        m_strIdPostavshik = query.value(5).toString();
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
    UpdateTasksList();
}

void QWashPartnerTaskWidget::OnFilterApplyPressed()
{
    m_filtersStr = QString("and \"Задачи партнера Мойка\".ДатаВремя>'%1' and \"Задачи партнера Мойка\".ДатаВремя<'%2'").arg(m_pFromDateTimeEdit->dateTime().toSecsSinceEpoch()).arg(m_pToDateTimeEdit->dateTime().toSecsSinceEpoch());
    UpdateTasksList();
}

void QWashPartnerTaskWidget::OnSchetPressed()
{
    QSqlQuery query;
    QString strULPostavRequest = QString("select ЮЛ from Поставщики where id='%1'").arg(m_strIdPostavshik);

    QUuid uuidULPostavId;
    query.exec(strULPostavRequest);
    while(query.next())
        uuidULPostavId = query.value(0).toUuid();


    QString strULPartnerRequest = QString("select ЮЛ from Партнеры where id='%1'").arg(m_strUuidCurrentPartner);
    QUuid uuidULPartnerId;
    query.exec(strULPartnerRequest);
    while(query.next())
        uuidULPartnerId = query.value(0).toUuid();

    QString strFileName = QFileDialog::getSaveFileName(this , "Счет партнера Мойка" , QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) , tr("Excel (*.xls *.xlsx)"));

    if(strFileName.length()>5)
    {
        QString strTmpFile = GetTempFNameSchet();

        QString strNumber = GenNextShcetActNumber();

        WriteULsSchetInfo(strTmpFile , uuidULPostavId , uuidULPartnerId, m_vCurrentSchetItems , strNumber);

        QFile::remove(strFileName);

        if(QFile::copy(strTmpFile , strFileName))
        {
            QMessageBox::information(this , "КаршерингСервис" , "Счет сохранен " + strFileName);
        }
        else
        {
            QMessageBox::information(this , "КаршерингСервис" , "Не удалось сохранить счет " + strFileName);
        }

        strTmpFile = GetTempFNameAct();

        WriteULsActInfo(strTmpFile , uuidULPostavId , uuidULPartnerId, m_vCurrentSchetItems , strNumber);

        strFileName.insert(strFileName.length() - 4 , "_Акт");

        QFile::remove(strFileName);


        if(QFile::copy(strTmpFile , strFileName))
        {
            QMessageBox::information(this , "КаршерингСервис" , "Акт сохранен " + strFileName);
        }
        else
        {
            QMessageBox::information(this , "КаршерингСервис" , "Не удалось сохранить акт " + strFileName);
        }

        DeleteTempFiles();
    }
}
