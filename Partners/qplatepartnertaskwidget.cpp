#include "qplatepartnertaskwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QUuid>
#include <QSqlQuery>
#include <QPushButton>
#include "common.h"
#include <QStandardPaths>
#include <QFileDialog>
#include <QMessageBox>
#include <QCheckBox>
#include "service/qselfrombddlg.h"

extern int iUserType;

QPlatePartnerTaskWidget::QPlatePartnerTaskWidget(QWidget *parent)
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
    //m_pTasksTableWidget->setColumnHidden(2,true);  //Скрыли зазказчика
    QStringList headers;
    headers << "Дата/время" << "Точка" << "Количество"<<"Комментарий"<<"Поставщик"<<"Заказчик"<<" ";
    m_pTasksTableWidget->setHorizontalHeaderLabels(headers);
    connect(m_pTasksTableWidget , SIGNAL(itemDoubleClicked(QTableWidgetItem*)) , this , SLOT(OnTasksDblClk(QTableWidgetItem*)));
    pVMainLayout->addWidget(m_pTasksTableWidget);
    m_pTasksTableWidget->resizeColumnsToContents();
    OnFilterApplyPressed();
}

void QPlatePartnerTaskWidget::UpdateTasksList()
{
    m_pTasksTableWidget->clear();
    m_pTasksTableWidget->clearSpans();
    m_pTasksTableWidget->setRowCount(0);


    QStringList headers;
    headers  << "Дата/время" << "Точка" << "Количество"<<"Комментарий"<<"Поставщик"<<"Заказчик"<<" ";
    m_pTasksTableWidget->setHorizontalHeaderLabels(headers);

    QSqlQuery query;
    QString strQuery =  QString("select \"Задачи партнера Номера\".id , \"Задачи партнера Номера\".ДатаВремя, \"Точки Партнеров\".Название , \"Задачи партнера Номера\".Количество, \"Задачи партнера Номера\".Комментарий, Поставщики.Название, Партнеры.Поставщик, Заказчики.Название, Заказчики.ЮЛ , Заказчики.id from \"Задачи партнера Номера\", \"Точки Партнеров\" , Поставщики, Партнеры, Заказчики where Поставщики.id = Партнеры.Поставщик  and \"Задачи партнера Номера\".Партнер = Партнеры.id  and \"Точки Партнеров\".id = \"Задачи партнера Номера\".Точка and \"Задачи партнера Номера\".Заказчик=Заказчики.id and \"Задачи партнера Номера\".Партнер='%1' %2").arg(m_strUuidCurrentPartner).arg(m_filtersStr);
    query.exec(strQuery);

    if(query.size() < 1) return;

    m_pTasksTableWidget->setRowCount(query.size());

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

        /*Количество*/
        pItem = new QTableWidgetItem(query.value(3).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 2,  pItem);

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

        /*Заказчик*/
        pItem = new QTableWidgetItem(query.value(7).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 5,  pItem);

        /*Чек-бокс*/
        QCheckBox * pCheckBox = new QCheckBox();
        pCheckBox->setChecked(true);
        m_pTasksTableWidget->setCellWidget(iRowCounter , 6 , pCheckBox);

        iRowCounter++;

        m_strIdPostavshik = query.value(6).toString();
    }
    m_pTasksTableWidget->resizeColumnsToContents();
}

void QPlatePartnerTaskWidget::OnSchetZakazPressed()
{
    QSqlQuery query;

    QUuid uuidULZakazIdUL = QUuid();

    //Заполнение массива элементов для счета
    QVector<SSchetItem> vCurrentSchetItems;

    SSchetItem schetItem;

    int iRowCount = m_pTasksTableWidget->rowCount();

    double dplItemPrice = 0;

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

        //т.к. выше получили id заказчика и подразумевается что в счете он один (есть соответствующая проверка), то можно получить цен для данного заказчика
        if(dplItemPrice == 0)
        {
            query.exec(QString("select Цена from \"ЦеныЗаказчиков\" where Заказчик='%1' and ТипЗадачи='19608771-a77d-4d34-b667-bc515536894a'").arg(uuidCurrentLineZakaz.toString()));
            while(query.next()) dplItemPrice = query.value(0).toDouble();
        }

        QString strTaskId = m_pTasksTableWidget->item(iRowCounter , 1)->data(Qt::UserRole).toString();

        QString strQuery =  QString("select  \"Задачи партнера Номера\".Количество, \"Задачи партнера Номера\".Заказчик from \"Задачи партнера Номера\" where  \"Задачи партнера Номера\".id='%1'").arg(strTaskId);
        query.exec(strQuery);

        while(query.next())
        {
            schetItem.dblCount = query.value(0).toInt();
            schetItem.strUnitMeasure = "шт.";
            schetItem.strName = "Изготовление номеров";
            schetItem.dblItemPrice = dplItemPrice;

            /*Ищем такие же типы задач (по названию) в уже имеющимся списке, при нахождении не добавляем новый элемент, а к существующему плюсуем дополнительное засчитываемое количество*/
            bool bIsCaurrentAdded = false;
            for(SSchetItem &item : vCurrentSchetItems)
            {
                if(item.strName == schetItem.strName)
                {
                    item.dblCount = item.dblCount + schetItem.dblCount;//nicht arbiten
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


    QString strFileName = QFileDialog::getSaveFileName(this , "Счет партнера Мойка" , QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) , tr("Excel (*.xls *.xlsx)"));

    if(strFileName.length()>5)
    {
        QString strTmpFile = GetTempFNameSchet();

        QString strNumber = GenNextShcetActNumber();

        WriteULsSchetInfo(strTmpFile , uuidULZakazIdUL , uuidULPostavId , vCurrentSchetItems , strNumber);

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

        WriteULsActInfo(strTmpFile , uuidULZakazIdUL , uuidULPostavId, vCurrentSchetItems , strNumber);

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

void QPlatePartnerTaskWidget::OnSchetPressed()
{
    //Заполнение массива элементов для счета
    QVector<SSchetItem> vCurrentSchetItems;

    SSchetItem schetItem;

    int iRowCount = m_pTasksTableWidget->rowCount();

    QSqlQuery query;

    double dplItemPrice = 0;
    query.exec("select Цена from \"Типы задач Номера\" where id='19608771-a77d-4d34-b667-bc515536894a'");
    while(query.next()) dplItemPrice = query.value(0).toDouble();

    for(int iRowCounter = 0 ; iRowCounter<iRowCount ; iRowCounter++)
    {
        if(((QCheckBox *)m_pTasksTableWidget->cellWidget(iRowCounter , 6))->isChecked() == false) continue;

        QString strTaskId = m_pTasksTableWidget->item(iRowCounter , 1)->data(Qt::UserRole).toString();

        QString strQuery =  QString("select  \"Задачи партнера Номера\".Количество from \"Задачи партнера Номера\" where  \"Задачи партнера Номера\".id='%1'").arg(strTaskId);
        query.exec(strQuery);

        while(query.next())
        {
            schetItem.dblCount = query.value(0).toInt();
            schetItem.strUnitMeasure = "шт.";
            schetItem.strName = "Изготовление номеров";
            schetItem.dblItemPrice = dplItemPrice;

            /*Ищем такие же типы задач (по названию) в уже имеющимся списке, при нахождении не добавляем новый элемент, а к существующему плюсуем дополнительное засчитываемое количество*/
            bool bIsCaurrentAdded = false;
            for(SSchetItem &item : vCurrentSchetItems)
            {
                if(item.strName == schetItem.strName)
                {
                    item.dblCount = item.dblCount + schetItem.dblCount;//nicht arbiten
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

void QPlatePartnerTaskWidget::SetActivPartner(QString strUuid)
{
    m_strUuidCurrentPartner = strUuid;
    UpdateTasksList();
}

void QPlatePartnerTaskWidget::OnTasksDblClk(QTableWidgetItem*)
{

}

void QPlatePartnerTaskWidget::OnFilterApplyPressed()
{
    m_filtersStr = QString("and \"Задачи партнера Номера\".ДатаВремя>'%1' and \"Задачи партнера Номера\".ДатаВремя<'%2'").arg(m_pFromDateTimeEdit->dateTime().toSecsSinceEpoch()).arg(m_pToDateTimeEdit->dateTime().toSecsSinceEpoch());
    UpdateTasksList();
}
