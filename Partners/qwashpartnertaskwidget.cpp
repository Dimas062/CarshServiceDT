#include "qwashpartnertaskdlg.h"
#include "qwashpartnertaskwidget.h"
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
#include <QCheckBox>
#include "service/xlspatterns.h"

extern int iUserType;
extern QString strCurrentZakazId;

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

    pVMainLayout->addLayout(pFilterHLoyuot);

    QHBoxLayout * pSchetButtonsHLoyuot = new QHBoxLayout();

    QPushButton * pSchetButton = new QPushButton("Сформировать счет/акт партнера");
    connect(pSchetButton,SIGNAL(pressed()),this,SLOT(OnSchetPressed()));
    if(iUserType == CarshService) pSchetButtonsHLoyuot->addWidget(pSchetButton);

    QPushButton * pSchetZakazButton = new QPushButton("Сформировать счет/акт заказчика");
    connect(pSchetZakazButton,SIGNAL(pressed()),this,SLOT(OnSchetZakazPressed()));
    if(iUserType == CarshService) pSchetButtonsHLoyuot->addWidget(pSchetZakazButton);

    pVMainLayout->addLayout(pSchetButtonsHLoyuot);

    pVMainLayout->addSpacing(5);

    m_pTasksTableWidget = new QTableWidget;
    m_pTasksTableWidget->setColumnCount(7);
    //m_pTasksTableWidget->setColumnHidden(2,true);  //Скрыли заказчика
    QStringList headers;
    headers << "Дата/время" << "Точка" <<"Комментарий"<<"Поставщик"<<"Заказчик"<<"Стоимость"<<" ";
    m_pTasksTableWidget->setHorizontalHeaderLabels(headers);

    if(iUserType == Carsh)//У других партнеров для каршерингов делали невывод столбцов Заказчик и чекбокс, тут просто скроем их (так быстрее)
    {
        m_pTasksTableWidget->setColumnHidden(4,true);
        m_pTasksTableWidget->setColumnHidden(6,true);
    }

    connect(m_pTasksTableWidget , SIGNAL(itemDoubleClicked(QTableWidgetItem*)) , this , SLOT(OnTasksDblClk(QTableWidgetItem*)));
    pVMainLayout->addWidget(m_pTasksTableWidget);
    m_pTasksTableWidget->resizeColumnsToContents();
    OnFilterApplyPressed();
}

void QWashPartnerTaskWidget::UpdateTasksList()
{
    m_pTasksTableWidget->clear();
    m_pTasksTableWidget->clearSpans();
    m_pTasksTableWidget->setRowCount(0);

    QStringList headers;
    headers  << "Дата/время" << "Точка" <<"Комментарий"<<"Поставщик"<<"Заказчик"<<"Стоимость"<<" ";
    m_pTasksTableWidget->setHorizontalHeaderLabels(headers);



    QString strQuery =  QString("select \"Задачи партнера Мойка\".id , \"Задачи партнера Мойка\".ДатаВремя, \"Точки Партнеров\".Название , \"Задачи партнера Мойка\".Комментарий, Поставщики.Название, Партнеры.Поставщик, Заказчики.Название, Заказчики.ЮЛ , Заказчики.id from \"Задачи партнера Мойка\", \"Точки Партнеров\" , Поставщики, Партнеры, Заказчики where Поставщики.id = Партнеры.Поставщик  and \"Задачи партнера Мойка\".Партнер = Партнеры.id  and \"Точки Партнеров\".id = \"Задачи партнера Мойка\".Точка and \"Задачи партнера Мойка\".Заказчик=Заказчики.id and \"Задачи партнера Мойка\".Партнер='%1' %2").arg(m_strUuidCurrentPartner).arg(m_filtersStr);
    if(iUserType == Carsh)
    {
        QString strCarshByPartnerFilter = QString(" and \"Задачи партнера Мойка\".Заказчик='%1'").arg(strCurrentZakazId);
        strQuery.append(strCarshByPartnerFilter);
    }
    QSqlQuery query;
    query.exec(strQuery);

    if(query.size() < 1) return;

    m_pTasksTableWidget->setRowCount(query.size());

    int iRowCounter = 0;
    while(query.next())
    {
        SSchetItem schetItem;
        SSchetItem schetItemZakaz;

        /*Дата/время*/
        QTableWidgetItem * pItem = new QTableWidgetItem(QDateTime::fromSecsSinceEpoch(query.value(1).toInt()).toString("dd.MM.yyyy hh:mm"));
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole + 1, query.value(7));
        pItem->setData(Qt::UserRole + 2, query.value(8));
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

        /*Поставщик*/
        pItem = new QTableWidgetItem(query.value(4).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 3,  pItem);

        /*Заказчик*/
        pItem = new QTableWidgetItem(query.value(6).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 4,  pItem);

        /*Детализация построчно (потипно) для счета, рассчет стоимости суммарно по каждой задача и штрафа суммарно для всех типов*/
        QString strTypesExec=QString("select \"Типы задач Мойка\".Цена, SUM(\"Задача Мойка - Типы\".Количество), \"Типы задач Мойка\".Тип, \"Задача Мойка - Типы\".Ночь , sum((select \"Отмена Мойки\".Количество from \"Отмена Мойки\" where \"Отмена Мойки\".Задача = \"Задача Мойка - Типы\".Задача and \"Отмена Мойки\".Тип = \"Задача Мойка - Типы\".Тип and \"Отмена Мойки\".Ночь = \"Задача Мойка - Типы\".Ночь and \"Отмена Мойки\".Удалено<>'true')) from \"Задача Мойка - Типы\" , \"Типы задач Мойка\" where \"Типы задач Мойка\".id = \"Задача Мойка - Типы\".Тип and  \"Задача Мойка - Типы\".Задача = '%1' group by \"Типы задач Мойка\".Тип , \"Задача Мойка - Типы\".Ночь , \"Типы задач Мойка\".Цена").arg(query.value(0).toString());
        if(iUserType == Carsh)
        {
            strTypesExec=QString("select ЦеныЗаказчиков.Цена, SUM(\"Задача Мойка - Типы\".Количество), \"Типы задач Мойка\".Тип, \"Задача Мойка - Типы\".Ночь , 0  from \"Задача Мойка - Типы\" , \"Типы задач Мойка\" ,ЦеныЗаказчиков where ЦеныЗаказчиков.Заказчик='%2' and ЦеныЗаказчиков.ТипЗадачи = \"Типы задач Мойка\".id and \"Типы задач Мойка\".id = \"Задача Мойка - Типы\".Тип and  \"Задача Мойка - Типы\".Задача = '%1' group by \"Типы задач Мойка\".Тип , \"Задача Мойка - Типы\".Ночь , ЦеныЗаказчиков.Цена").arg(query.value(0).toString()).arg(strCurrentZakazId);
        }


        QSqlQuery TypesQuery;
        TypesQuery.exec(strTypesExec);

        double dblPrice = 0;//Стоимость по всей задаче суммарно
        double dblPen = 0; //Штраф по всей задаче суммарно
        while(TypesQuery.next())
        {
            int iItemCount = TypesQuery.value(1).toDouble();
            double dblCost = TypesQuery.value(0).toDouble();

            dblPrice = dblPrice + dblCost * iItemCount;

            int iSubPenCount = TypesQuery.value(4).toInt();

            dblPen = dblPen + iSubPenCount*dblCost;
        }

        /*Стоимость*/
        QString strPrice = QString(" %1 руб.").arg(dblPrice - dblPen);
        pItem = new QTableWidgetItem(strPrice);
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 5,  pItem);

        /*Чек-бокс*/
        QCheckBox * pCheckBox = new QCheckBox();
        pCheckBox->setChecked(true);
        m_pTasksTableWidget->setCellWidget(iRowCounter , 6 , pCheckBox);

        iRowCounter++;

        m_strIdPostavshik = query.value(5).toString();
    }

    m_pTasksTableWidget->resizeColumnsToContents();

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

/*Счет заказчику*/
void QWashPartnerTaskWidget::OnSchetZakazPressed()
{
    QSqlQuery query;

    QUuid uuidULZakazIdUL = QUuid();

    //Заполнение массива элементов для счета по работам и затратам(компенсация)
    QVector<SSchetItem> vCurrentZakazSchetItems;
    QVector<SSchetItem> vCurrentCompensSchetItems;

    SSchetItem schetItemZakaz;


    int iRowCount = m_pTasksTableWidget->rowCount();

    for(int iRowCounter = 0 ; iRowCounter<iRowCount ; iRowCounter++)
    {

        if(((QCheckBox *)m_pTasksTableWidget->cellWidget(iRowCounter , 6))->isChecked() == false) continue;

        QUuid uuidCurrentLineZakazUL = m_pTasksTableWidget->item(iRowCounter , 0)->data(Qt::UserRole + 1).toUuid();
        QUuid uuidCurrentLineZakaz   = m_pTasksTableWidget->item(iRowCounter , 0)->data(Qt::UserRole + 2).toUuid();

        if(uuidULZakazIdUL != QUuid() && uuidULZakazIdUL!=uuidCurrentLineZakazUL)
        {
            QMessageBox::information(this , "Счет, акт для заказчика" , "В таблице отмечены позиции для разных заказчиков. Для формирования счета и акта для заказчика оставьте отмеченными только позиции одного заказчика");
            return;
        }
        uuidULZakazIdUL = uuidCurrentLineZakazUL;

        QString strTaskId = m_pTasksTableWidget->item(iRowCounter , 1)->data(Qt::UserRole).toString();

        /*Детализация построчно (потипно) для счета, рассчет стоимости суммарно по каждой задача и штрафа суммарно для всех типов*/
        QString strTypesExec=QString("select ЦеныЗаказчиков.Цена, SUM(\"Задача Мойка - Типы\".Количество), \"Типы задач Мойка\".Тип, \"Задача Мойка - Типы\".Ночь , sum((select \"Отмена Мойки\".Количество from \"Отмена Мойки\" where \"Отмена Мойки\".Задача = \"Задача Мойка - Типы\".Задача and \"Отмена Мойки\".Тип = \"Задача Мойка - Типы\".Тип and \"Отмена Мойки\".Ночь = \"Задача Мойка - Типы\".Ночь and \"Отмена Мойки\".Удалено<>'true')) from \"Задача Мойка - Типы\" , \"Типы задач Мойка\", ЦеныЗаказчиков where \"Типы задач Мойка\".id = \"Задача Мойка - Типы\".Тип and ЦеныЗаказчиков.Заказчик='%2' and ЦеныЗаказчиков.ТипЗадачи = \"Типы задач Мойка\".id and \"Задача Мойка - Типы\".Задача = '%1' group by \"Типы задач Мойка\".Тип , \"Задача Мойка - Типы\".Ночь , ЦеныЗаказчиков.Цена").arg(strTaskId).arg(uuidCurrentLineZakaz.toString());
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

            schetItemZakaz.dblItemPrice = dblCost;

            //В счете для заказчика подсчет тот же, только количество штрафов всегда = 0 (да неоптимально их в SQL запрашивать и не использовать, но время!)
            //int iSubPenCount = TypesQuery.value(4).toInt();
            int iSubPenCount = 0;

            dblPen = dblPen + iSubPenCount*dblCost;

            schetItemZakaz.dblCount = iItemCount - iSubPenCount;
            schetItemZakaz.strName = strWorks;
            schetItemZakaz.strUnitMeasure =" шт.";


            bool bIsCaurrentAdded = false;
            for(SSchetItem &item : vCurrentZakazSchetItems)
            {
                if(item.strName == schetItemZakaz.strName)
                {
                    item.dblCount = item.dblCount + schetItemZakaz.dblCount;
                    bIsCaurrentAdded = true;
                    break;
                }
            }
            if(!bIsCaurrentAdded) vCurrentZakazSchetItems.push_back(schetItemZakaz);
        }
    }


    QString strULPostavRequest = QString("select ЮЛ from Поставщики where id='%1'").arg(m_strIdPostavshik);

    QUuid uuidULPostavId;
    query.exec(strULPostavRequest);
    while(query.next())
        uuidULPostavId = query.value(0).toUuid();


    QString strFileName = QFileDialog::getSaveFileName(this , "Счет партнера Мойка" , QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) , tr("Excel (*.xls *.xlsx)"));
    strFileName.append(".xlsx");
    if(strFileName.length()>5)
    {
        QString strTmpFile = GetTempFNameSchet();

        QString strNumber = GenNextShcetActNumber();

        WriteULsSchetInfo(strTmpFile , uuidULZakazIdUL , uuidULPostavId , vCurrentZakazSchetItems , strNumber);

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

        WriteULsActInfo(strTmpFile , uuidULZakazIdUL , uuidULPostavId , vCurrentZakazSchetItems , strNumber);

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

/*Счет партнеру-поставщику*/
void QWashPartnerTaskWidget::OnSchetPressed()
{
    //Заполнение массива элементов для счета
    QVector<SSchetItem> vCurrentSchetItems;

    SSchetItem schetItem;

    int iRowCount = m_pTasksTableWidget->rowCount();


    for(int iRowCounter = 0 ; iRowCounter<iRowCount ; iRowCounter++)
    {
        if(((QCheckBox *)m_pTasksTableWidget->cellWidget(iRowCounter , 6))->isChecked() == false) continue;

        QString strTaskId = m_pTasksTableWidget->item(iRowCounter , 1)->data(Qt::UserRole).toString();


        /*Детализация построчно (потипно) для счета, рассчет стоимости суммарно по каждой задача и штрафа суммарно для всех типов*/
        QString strTypesExec=QString("select \"Типы задач Мойка\".Цена, SUM(\"Задача Мойка - Типы\".Количество), \"Типы задач Мойка\".Тип, \"Задача Мойка - Типы\".Ночь , sum((select \"Отмена Мойки\".Количество from \"Отмена Мойки\" where \"Отмена Мойки\".Задача = \"Задача Мойка - Типы\".Задача and \"Отмена Мойки\".Тип = \"Задача Мойка - Типы\".Тип and \"Отмена Мойки\".Ночь = \"Задача Мойка - Типы\".Ночь and \"Отмена Мойки\".Удалено<>'true')) from \"Задача Мойка - Типы\" , \"Типы задач Мойка\" where \"Типы задач Мойка\".id = \"Задача Мойка - Типы\".Тип and  \"Задача Мойка - Типы\".Задача = '%1' group by \"Типы задач Мойка\".Тип , \"Задача Мойка - Типы\".Ночь , \"Типы задач Мойка\".Цена").arg(strTaskId);

        QSqlQuery TypesQuery;
        QString strWorks;
        TypesQuery.exec(strTypesExec);

        double dblPrice = 0;//Стоимость по всей задаче суммарно
        double dblPen = 0; //Штраф по всей задаче суммарно
        while(TypesQuery.next())
        {
            int iItemCount = TypesQuery.value(1).toDouble();
            double dblCost = TypesQuery.value(0).toDouble();

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
            for(SSchetItem &item : vCurrentSchetItems)
            {
                if(item.strName == schetItem.strName)
                {
                    item.dblCount = item.dblCount + schetItem.dblCount;
                    bIsCaurrentAdded = true;
                    break;
                }
            }
            if(!bIsCaurrentAdded) vCurrentSchetItems.push_back(schetItem);
        }
    }


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
    strFileName.append(".xlsx");
    if(strFileName.length()>5)
    {
        QString strTmpFile = GetTempFNameSchet();

        QString strNumber = GenNextShcetActNumber();

        WriteULsSchetInfo(strTmpFile , uuidULPostavId , uuidULPartnerId, vCurrentSchetItems , strNumber);

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

        WriteULsActInfo(strTmpFile , uuidULPostavId , uuidULPartnerId, vCurrentSchetItems , strNumber);

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
