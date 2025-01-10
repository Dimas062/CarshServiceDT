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


    QPushButton * pSchetButton = new QPushButton("Сформировать счет партнера");
    connect(pSchetButton,SIGNAL(pressed()),this,SLOT(OnSchetPressed()));
    pFilterHLoyuot->addWidget(pSchetButton);

    pVMainLayout->addLayout(pFilterHLoyuot);

    pVMainLayout->addSpacing(5);

    m_pTasksTableWidget = new QTableWidget;
    m_pTasksTableWidget->setColumnCount(5);
    //m_pTasksTableWidget->setColumnHidden(2,true);  //Скрыли зазказчика
    QStringList headers;
    headers << "Дата/время" << "Точка" << "Количество"<<"Комментарий"<<"Заказчик";
    m_pTasksTableWidget->setHorizontalHeaderLabels(headers);
    connect(m_pTasksTableWidget , SIGNAL(itemDoubleClicked(QTableWidgetItem*)) , this , SLOT(OnTasksDblClk(QTableWidgetItem*)));
    pVMainLayout->addWidget(m_pTasksTableWidget);
    OnFilterApplyPressed();
}

void QPlatePartnerTaskWidget::UpdateTasksList()
{
    m_pTasksTableWidget->clear();
    m_pTasksTableWidget->clearSpans();
    m_pTasksTableWidget->setRowCount(0);
    m_vCurrentSchetItems.clear();

    QSqlQuery query;

    double dplItemPrice = 0;
    query.exec("select Цена from \"Типы задач\" where id='99b4e860-5a7b-42a4-9136-f96252ef4192'");
    while(query.next()) dplItemPrice = query.value(0).toDouble();

    QStringList headers;
    headers  << "Дата/время" << "Точка" << "Количество"<<"Комментарий"<<"Заказчик";
    m_pTasksTableWidget->setHorizontalHeaderLabels(headers);
    QString strQuery =  QString("select \"Задачи партнера Номера\".id , \"Задачи партнера Номера\".ДатаВремя, \"Точки Партнеров\".Название , \"Задачи партнера Номера\".Количество, \"Задачи партнера Номера\".Комментарий, Поставщики.Название, Партнеры.Поставщик from \"Задачи партнера Номера\", \"Точки Партнеров\" , Поставщики, Партнеры where Поставщики.id = Партнеры.Поставщик  and \"Задачи партнера Номера\".Партнер = Партнеры.id  and \"Точки Партнеров\".id = \"Задачи партнера Номера\".Точка and \"Задачи партнера Номера\".Партнер='%1' %2").arg(m_strUuidCurrentPartner).arg(m_filtersStr);


    query.exec(strQuery);

    if(query.size() < 1) return;

    m_pTasksTableWidget->setRowCount(query.size());//+1 для итого

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

        /*Количество*/
        pItem = new QTableWidgetItem(query.value(3).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 2,  pItem);

        schetItem.dblCount = query.value(3).toInt();
        schetItem.strUnitMeasure = "шт.";
        schetItem.strName = "Изготовление номеров";
        schetItem.dblItemPrice = dplItemPrice;

        /*Комментарий*/
        pItem = new QTableWidgetItem(query.value(4).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 3,  pItem);

        /*Заказчик*/
        pItem = new QTableWidgetItem(query.value(5).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 4,  pItem);

        iRowCounter++;

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

        m_strIdPostavshik = query.value(6).toString();
    }
}

void QPlatePartnerTaskWidget::OnSchetPressed()
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
