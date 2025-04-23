#include "qstickpartnertaskwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QUuid>
#include <QSqlQuery>
#include <QPushButton>
#include "qstickpartnertaskdlg.h"
#include "common.h"
#include <QStandardPaths>
#include <QFileDialog>
#include <QMessageBox>
#include <QCheckBox>
//#include "service/qselfrombddlg.h"

extern int iUserType;
extern QString strCurrentZakazId;

QStickPartnerTaskWidget::QStickPartnerTaskWidget(QWidget *parent)
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
    m_pTasksTableWidget->setColumnCount(6);
    //m_pTasksTableWidget->setColumnHidden(2,true);  //Скрыли зазказчика
    QStringList headers;
    headers << "Дата/время" << "Точка" << "Номер"<<"Комментарий"<<"Поставщик"<<"Стоимость";
    if(iUserType == CarshService)
    {
        m_pTasksTableWidget->setColumnCount(8);
        headers<<"Заказчик"<<" ";
    }
    m_pTasksTableWidget->setHorizontalHeaderLabels(headers);
    connect(m_pTasksTableWidget , SIGNAL(itemDoubleClicked(QTableWidgetItem*)) , this , SLOT(OnTasksDblClk(QTableWidgetItem*)));
    pVMainLayout->addWidget(m_pTasksTableWidget);
    m_pTasksTableWidget->resizeColumnsToContents();
    OnFilterApplyPressed();
}

void QStickPartnerTaskWidget::UpdateTasksList()
{
    m_pTasksTableWidget->clear();
    m_pTasksTableWidget->clearSpans();
    m_pTasksTableWidget->setRowCount(0);

    QStringList headers;
    headers  << "Дата/время" << "Точка " << "Номер"<<"Комментарий"<<"Поставщик"<<"Стоимость";
    if(iUserType == CarshService) headers<<"Заказчик"<<" ";

    m_pTasksTableWidget->setHorizontalHeaderLabels(headers);
    QString strQuery =  QString("select \"Задачи партнера Оклейка\".id , \"Задачи партнера Оклейка\".ДатаВремя, \"Точки Партнеров\".Название , \"Задачи партнера Оклейка\".Номер, \"Задачи партнера Оклейка\".Комментарий, Поставщики.Название, Партнеры.Поставщик, Заказчики.Название, Заказчики.ЮЛ , Заказчики.id from \"Задачи партнера Оклейка\", \"Точки Партнеров\" , Поставщики, Партнеры, Заказчики where Поставщики.id = Партнеры.Поставщик  and \"Задачи партнера Оклейка\".Партнер = Партнеры.id  and \"Точки Партнеров\".id = \"Задачи партнера Оклейка\".Точка and \"Задачи партнера Оклейка\".Заказчик=Заказчики.id and \"Задачи партнера Оклейка\".Партнер='%1' %2").arg(m_strUuidCurrentPartner).arg(m_filtersStr);

    if(iUserType == Carsh)
    {
        QString strCarshByPartnerFilter = QString(" and \"Задачи партнера Оклейка\".Заказчик='%1'").arg(strCurrentZakazId);
        strQuery.append(strCarshByPartnerFilter);
    }

    QSqlQuery query;
    query.exec(strQuery);

    if(query.size() < 1) return;

    m_pTasksTableWidget->setRowCount(query.size());//+1 для итого

    int iRowCounter = 0;
    while(query.next())
    {
        /*Дата/время*/
        QTableWidgetItem * pItem = new QTableWidgetItem(QDateTime::fromSecsSinceEpoch(query.value(1).toInt()).toString("dd.MM.yyyy hh:mm"));
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole + 1, query.value(8));
        pItem->setData(Qt::UserRole + 2, query.value(9));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 0,  pItem);

        /*Точка*/
        pItem = new QTableWidgetItem(query.value(2).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 1,  pItem);

        /*Госномер*/
        pItem = new QTableWidgetItem(query.value(3).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 2,  pItem);

        QString strGosNomer = query.value(3).toString();

        /*Комментарий*/
        pItem = new QTableWidgetItem(query.value(4).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 3,  pItem);

        /*Поставщик*/
        pItem = new QTableWidgetItem(query.value(5).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 4,  pItem);

        /*Стоимость*/
        QString strTypesExec=QString("select \"Типы задач Оклейка\".Цена , \"Типы задач Оклейка\".Тип from \"Задача Оклейка - Типы\" , \"Типы задач Оклейка\"  where \"Типы задач Оклейка\".id = \"Задача Оклейка - Типы\".Тип and  \"Задача Оклейка - Типы\".Задача = '%1'").arg(query.value(0).toString());
        QSqlQuery TypesQuery;

        TypesQuery.exec(strTypesExec);
        double dblPrice = 0;
        while(TypesQuery.next())
        {
            dblPrice = dblPrice + TypesQuery.value(0).toDouble();
        }
        QString strPrice = QString(" %1 руб.").arg(dblPrice);


        pItem = new QTableWidgetItem(strPrice);
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 5,  pItem);


        if(iUserType == CarshService)
        {
            /*Заказчик*/
            pItem = new QTableWidgetItem(query.value(7).toString());
            pItem->setData(Qt::UserRole , query.value(0));
            pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            m_pTasksTableWidget->setItem(iRowCounter , 6,  pItem);

            /*Чек-бокс*/
            QCheckBox * pCheckBox = new QCheckBox();
            pCheckBox->setChecked(true);
            m_pTasksTableWidget->setCellWidget(iRowCounter , 7 , pCheckBox);
        }

        iRowCounter++;

        m_strIdPostavshik = query.value(6).toString();
    }
    m_pTasksTableWidget->resizeColumnsToContents();
}

void QStickPartnerTaskWidget::OnSchetZakazPressed()
{
    QSqlQuery query;

    QUuid uuidULZakazIdUL = QUuid();

    //Заполнение массива элементов для счета
    QVector<SSchetItem> vCurrentSchetItems;

    SSchetItem schetItem;

    int iRowCount = m_pTasksTableWidget->rowCount();


    for(int iRowCounter = 0 ; iRowCounter<iRowCount ; iRowCounter++)
    {
        if(((QCheckBox *)m_pTasksTableWidget->cellWidget(iRowCounter , 7))->isChecked() == false) continue;

        QUuid uuidCurrentLineZakazUL = m_pTasksTableWidget->item(iRowCounter , 0)->data(Qt::UserRole + 1).toUuid();
        QUuid uuidCurrentLineZakaz   = m_pTasksTableWidget->item(iRowCounter , 0)->data(Qt::UserRole + 2).toUuid();

        if(uuidULZakazIdUL != QUuid() && uuidULZakazIdUL!=uuidCurrentLineZakazUL)
        {
            QMessageBox::information(this , "Счет, акт для заказчика" , "В таблице отмечены позиции для разных заказчиков. Для формирования счета и акта для заказчика оставьте отмеченными только позиции одного заказчика");
            return;
        }
        uuidULZakazIdUL = uuidCurrentLineZakazUL;

        QString strTaskId = m_pTasksTableWidget->item(iRowCounter , 1)->data(Qt::UserRole).toString();

        QString strTypesExec=QString("select ЦеныЗаказчиков.Цена , \"Типы задач Оклейка\".Тип from \"Задача Оклейка - Типы\" , \"Типы задач Оклейка\", ЦеныЗаказчиков  where ЦеныЗаказчиков.ТипЗадачи=\"Типы задач Оклейка\".id and ЦеныЗаказчиков.Заказчик = '%2' and \"Типы задач Оклейка\".id = \"Задача Оклейка - Типы\".Тип and  \"Задача Оклейка - Типы\".Задача = '%1'").arg(strTaskId).arg(uuidCurrentLineZakaz.toString());

        QSqlQuery TypesQuery;

        TypesQuery.exec(strTypesExec);
        double dblPrice = 0;
        while(TypesQuery.next())
        {
            dblPrice = dblPrice + TypesQuery.value(0).toDouble();

            schetItem.strName = TypesQuery.value(1).toString();
            schetItem.strUnitMeasure =" шт.";
            schetItem.dblCount = 1;
            schetItem.dblItemPrice = TypesQuery.value(0).toDouble();

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


    QString strULPostavRequest = QString("select ЮЛ from Поставщики where id='%1'").arg(m_strIdPostavshik);

    QUuid uuidULPostavId;

    query.exec(strULPostavRequest);
    while(query.next())
        uuidULPostavId = query.value(0).toUuid();


    QString strFileName = QFileDialog::getSaveFileName(this , "Счет партнера Оклейка" , QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) , tr("Excel (*.xls *.xlsx)"));
    strFileName.append(".xlsx");
    if(strFileName.length()>5)
    {
        QString strTmpFile = GetTempFNameSchet();

        QString strNumber = GenNextShcetActNumber();

        WriteULsSchetInfo(strTmpFile , uuidULZakazIdUL , uuidULPostavId, vCurrentSchetItems , strNumber);

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

        WriteULsActInfo(strTmpFile , uuidULZakazIdUL , uuidULPostavId , vCurrentSchetItems , strNumber);

        strFileName.insert(strFileName.length() - 5 , "_Акт");

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

void QStickPartnerTaskWidget::OnSchetPressed()
{   
    //Заполнение массива элементов для счета
    QVector<SSchetItem> vCurrentSchetItems;

    SSchetItem schetItem;

    int iRowCount = m_pTasksTableWidget->rowCount();

    for(int iRowCounter = 0 ; iRowCounter<iRowCount ; iRowCounter++)
    {

        if(((QCheckBox *)m_pTasksTableWidget->cellWidget(iRowCounter , 7))->isChecked() == false) continue;

        QString strTaskId = m_pTasksTableWidget->item(iRowCounter , 1)->data(Qt::UserRole).toString();

        QString strTypesExec=QString("select \"Типы задач Оклейка\".Цена , \"Типы задач Оклейка\".Тип from \"Задача Оклейка - Типы\" , \"Типы задач Оклейка\"  where \"Типы задач Оклейка\".id = \"Задача Оклейка - Типы\".Тип and  \"Задача Оклейка - Типы\".Задача = '%1'").arg(strTaskId);
        QSqlQuery TypesQuery;

        TypesQuery.exec(strTypesExec);
        double dblPrice = 0;
        while(TypesQuery.next())
        {
            dblPrice = dblPrice + TypesQuery.value(0).toDouble();

            schetItem.strName = TypesQuery.value(1).toString();
            schetItem.strUnitMeasure =" шт.";
            schetItem.dblCount = 1;
            schetItem.dblItemPrice = TypesQuery.value(0).toDouble();

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


    QString strULPostavRequest = QString("select ЮЛ from Поставщики where id='%1'").arg(m_strIdPostavshik);

    QUuid uuidULPostavId;
    QSqlQuery query;
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

        strFileName.insert(strFileName.length() - 5 , "_Акт");

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

void QStickPartnerTaskWidget::SetActivPartner(QString strUuid)
{
    m_strUuidCurrentPartner = strUuid;
    UpdateTasksList();
}

void QStickPartnerTaskWidget::OnTasksDblClk(QTableWidgetItem* item)
{
    QStickPartnerTaskDlg dlg(item->data(Qt::UserRole).toString());
    dlg.exec();
}

void QStickPartnerTaskWidget::OnFilterApplyPressed()
{
    m_filtersStr = QString("and \"Задачи партнера Оклейка\".ДатаВремя>'%1' and \"Задачи партнера Оклейка\".ДатаВремя<'%2'").arg(m_pFromDateTimeEdit->dateTime().toSecsSinceEpoch()).arg(m_pToDateTimeEdit->dateTime().toSecsSinceEpoch());
    UpdateTasksList();
}
