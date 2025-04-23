#include "qtaskswidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QUuid>
#include <QSqlQuery>
#include <QLineEdit>
#include <QDateTime>
//#include "../Carshs/qcarshpenaltyparktaskcarddlg.h"
// #include "../Carshs/qcarshplatetaskcarddlg.h"
// #include "../Carshs/qcarshparkingtaskcarddlg.h"
// #include "../Carshs/qrettozonecarddlg.h"
// #include "../Carshs/qsmenataskdlg.h"
#include "../CarshService/tasks/qdocstaskdlg.h"
#include "../CarshService/tasks/qpenaltyparkingdialog.h"
#include "../CarshService/tasks/qparkingtaskdialog.h"
#include "../CarshService/tasks/qrettozonedialog.h"
#include "../CarshService/tasks/qsmenadlg.h"
#include "../CarshService/tasks/qplatetaskdialog.h"
#include <QSplashScreen>
#include <QPushButton>
#include <QTableWidgetItem>
#include "BDPatterns.h"
#include <QFont>
#include <QCheckBox>
#include "service/qselfrombddlg.h"
#include "service/xlspatterns.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QSettings>
#include <QCoreApplication>
#include <QHeaderView>
#include <QColorDialog>
#include "commondt.h"

extern QSettings settings;

QTasksWidget::QTasksWidget(QWidget *parent)
    : QWidget{parent}
{
    m_ColNameNumMap["QTasksWidget_DateTime"]         = 1;
    m_ColNameNumMap["QTasksWidget_Task"]             = 2;
    m_ColNameNumMap["QTasksWidget_Zakazchik"]        = 3;
    m_ColNameNumMap["QTasksWidget_Summa_sotr"]       = 4;
    m_ColNameNumMap["QTasksWidget_Summa_zak"]        = 5;
    m_ColNameNumMap["QTasksWidget_GRZ"]              = 6;
    m_ColNameNumMap["QTasksWidget_Sotr"]             = 7;
    m_ColNameNumMap["QTasksWidget_Zatrati"]          = 8;
    m_ColNameNumMap["QTasksWidget_Doc_Num"]          = 9;
    m_ColNameNumMap["QTasksWidget_Doc_Type"]         = 10;
    m_ColNameNumMap["QTasksWidget_Prichina_Zaderzg"] = 11;
    m_ColNameNumMap["QTasksWidget_Ot_Zak_PenPark"]   = 12;
    m_ColNameNumMap["QTasksWidget_Check"]            = 13;

    LoadColorMap();

    m_filtersStr = QString("");
    m_iCheckBoxCol = 8;



    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    QHBoxLayout * pFilterHLoyuot = new QHBoxLayout();
    QHBoxLayout * pFilterHLoyuot2 = new QHBoxLayout();

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

    m_pOnlyFinishedCheckBox = new QCheckBox("Только завершенные");
    pFilterHLoyuot->addWidget(m_pOnlyFinishedCheckBox);

    m_pOnlyUnfinishedCheckBox= new QCheckBox("Только не завершенные");
    pFilterHLoyuot->addWidget(m_pOnlyUnfinishedCheckBox);

    connect(m_pOnlyFinishedCheckBox, &QCheckBox::stateChanged,
            this, &QTasksWidget::onFinishedStateChanged);

    connect(m_pOnlyUnfinishedCheckBox, &QCheckBox::stateChanged,
            this, &QTasksWidget::onUnfinishedStateChanged);


    /*Период*/
    QLabel * pDateTimeFromLabel = new QLabel("c: ");
    pFilterHLoyuot2->addWidget(pDateTimeFromLabel);
    m_pFromDateTimeEdit = new QDateTimeEdit(QDateTime(QDate::currentDate().addDays(-(QDate::currentDate().day()) + 1) , QTime(0,0,0)));
    pFilterHLoyuot2->addWidget(m_pFromDateTimeEdit);



    QLabel * pDateTimeToLabel = new QLabel("по: ");
    pFilterHLoyuot2->addWidget(pDateTimeToLabel);
    m_pToDateTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime());
    pFilterHLoyuot2->addWidget(m_pToDateTimeEdit);

    pFilterHLoyuot2->addStretch();

    QLabel * pNumberLabel = new QLabel("ГРЗ: ");
    pFilterHLoyuot2->addWidget(pNumberLabel);
    m_pNumberEdit = new QLineEdit();
    m_pNumberEdit->setFixedWidth(120);
    pFilterHLoyuot2->addWidget(m_pNumberEdit);

    pVMainLayout->addLayout(pFilterHLoyuot);
    pVMainLayout->addLayout(pFilterHLoyuot2);

    QHBoxLayout * pButtonsHLoyuot = new QHBoxLayout();

    QPushButton * pFilterApplyButton = new QPushButton("Применить фильтры");
    connect(pFilterApplyButton,SIGNAL(pressed()),this,SLOT(OnFilterApplyPressed()));
    pButtonsHLoyuot->addWidget(pFilterApplyButton);

    QPushButton * pSchetApplyButton = new QPushButton("Счет/Акт");
    connect(pSchetApplyButton,SIGNAL(pressed()),this,SLOT(OnSchetPressed()));
    pButtonsHLoyuot->addWidget(pSchetApplyButton);

    QPushButton * pExcelButton = new QPushButton("Экспорт в Excel");
    connect(pExcelButton,SIGNAL(pressed()),this,SLOT(OnExcelPressed()));
    pButtonsHLoyuot->addWidget(pExcelButton);

    pVMainLayout->addLayout(pButtonsHLoyuot);

    pVMainLayout->addSpacing(5);

    m_pTasksTableWidget = new QTableWidget;
    m_pTasksTableWidget->setColumnCount(9);
    //m_pTasksTableWidget->setColumnHidden(2,true);  //Скрыли зазказчика
    QStringList headers;
    headers << "Дата/время" << "Задача" << "Заказчик"<<"Сумма сотруднику"<<"Сумма заказчику"<<"ГРЗ"<<"Сотрудник"<<"Затраты"<<" ";
    m_pTasksTableWidget->setHorizontalHeaderLabels(headers);
    connect(m_pTasksTableWidget , SIGNAL(itemDoubleClicked(QTableWidgetItem*)) , this , SLOT(OnTasksDblClk(QTableWidgetItem*)));
    pVMainLayout->addWidget(m_pTasksTableWidget);
    m_pTasksTableWidget->resizeColumnsToContents();
    QHeaderView *header = m_pTasksTableWidget->horizontalHeader();

    // Подключаем сигнал двойного клика по секции заголовка
    connect(header, &QHeaderView::sectionDoubleClicked,
            this, &QTasksWidget::onHeaderDoubleClicked);

    /*Лаяуты с фиьтрами типов задач*/
    QHBoxLayout * pAddFiltresLayout = new QHBoxLayout;
    m_pPenalParkTaskFilterWidget = new QPenalParkTaskFilterWidget;
    m_pPenalParkTaskFilterWidget->hide();
    pAddFiltresLayout->addWidget(m_pPenalParkTaskFilterWidget);

    pVMainLayout->addLayout(pAddFiltresLayout);
    OnFilterApplyPressed();
}

void QTasksWidget::LoadColorMap()
{
    m_ColorMap[1] = settings.value("QTasksWidget_DateTime" , m_defaultColor).value<QColor>();
    m_ColorMap[2] = settings.value("QTasksWidget_Task" , m_defaultColor).value<QColor>();
    m_ColorMap[3] = settings.value("QTasksWidget_Zakazchik" , m_defaultColor).value<QColor>();
    m_ColorMap[4] = settings.value("QTasksWidget_Summa_sotr" , m_defaultColor).value<QColor>();
    m_ColorMap[5] = settings.value("QTasksWidget_Summa_zak" , m_defaultColor).value<QColor>();
    m_ColorMap[6] = settings.value("QTasksWidget_GRZ" , m_defaultColor).value<QColor>();
    m_ColorMap[7] = settings.value("QTasksWidget_Sotr" , m_defaultColor).value<QColor>();
    m_ColorMap[8] = settings.value("QTasksWidget_Zatrati" , m_defaultColor).value<QColor>();
    m_ColorMap[9] = settings.value("QTasksWidget_Doc_Num" , m_defaultColor).value<QColor>();
    m_ColorMap[10] = settings.value("QTasksWidget_Doc_Type" , m_defaultColor).value<QColor>();
    m_ColorMap[11] = settings.value("QTasksWidget_Prichina_Zaderzg" , m_defaultColor).value<QColor>();
    m_ColorMap[12] = settings.value("QTasksWidget_Ot_Zak_PenPark" , m_defaultColor).value<QColor>();
    m_ColorMap[13] = settings.value("QTasksWidget_Check" , m_defaultColor).value<QColor>();
}

void QTasksWidget::SaveColorMap()
{
    settings.setValue("QTasksWidget_DateTime", m_ColorMap[1]);
    settings.setValue("QTasksWidget_Task", m_ColorMap[2]);
    settings.setValue("QTasksWidget_Zakazchik", m_ColorMap[3]);
    settings.setValue("QTasksWidget_Summa_sotr", m_ColorMap[4]);
    settings.setValue("QTasksWidget_Summa_zak", m_ColorMap[5]);
    settings.setValue("QTasksWidget_GRZ", m_ColorMap[6]);
    settings.setValue("QTasksWidget_Sotr", m_ColorMap[7]);
    settings.setValue("QTasksWidget_Zatrati", m_ColorMap[8]);
    settings.setValue("QTasksWidget_Doc_Num", m_ColorMap[9]);
    settings.setValue("QTasksWidget_Doc_Type", m_ColorMap[10]);
    settings.setValue("QTasksWidget_Prichina_Zaderzg", m_ColorMap[11]);
    settings.setValue("QTasksWidget_Ot_Zak_PenPark", m_ColorMap[12]);
    settings.setValue("QTasksWidget_Check", m_ColorMap[13]);

}

void QTasksWidget::TaskTypeComboChanged(int )
{

    /*Выбран тип Штрафстоянка*/
    if(m_pTaskTypeComboBox->currentData().toUuid() == QUuid(QString("8078b7ce-e423-49ae-9ce6-17758b852b33")))
    {
        m_pPenalParkTaskFilterWidget->show();
        m_iCheckBoxCol = 10;
        return;
    }

    /*Выбран тип Документы*/
    if(m_pTaskTypeComboBox->currentData().toUuid() == QUuid(QString("25695573-f5fe-43fd-93dc-76ee09e461fa")))
    {
        m_iCheckBoxCol = 10;
        return;
    }

    m_iCheckBoxCol=8;

    /*Выбраны все типы или один из типов без доп. панели фильтров*/
    m_pPenalParkTaskFilterWidget->hide();
}

void QTasksWidget::onHeaderDoubleClicked(int logicalIndex)
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Выберите цвет");
    if (color.isValid()) {

        /*Выбрана штрафстоянка - доп. столбцы штрафстоянки*/
        if(m_pTaskTypeComboBox->currentData().toUuid()==QUuid(QString("8078b7ce-e423-49ae-9ce6-17758b852b33"))&&(logicalIndex==8 || logicalIndex==9 || logicalIndex==10))
        {
            if(logicalIndex == 8) m_ColorMap[11] = color;
            if(logicalIndex == 9) m_ColorMap[12] = color;
            if(logicalIndex == 10) m_ColorMap[13] = color;
            SaveColorMap();
            UpdateTasksList();
            return;
        }
        /*Выбран тип Документы*/
        if(m_pTaskTypeComboBox->currentData().toUuid() == QUuid(QString("25695573-f5fe-43fd-93dc-76ee09e461fa"))&&(logicalIndex==8 || logicalIndex==9 || logicalIndex==10))
        {
            if(logicalIndex == 8) m_ColorMap[9] = color;
            if(logicalIndex == 9) m_ColorMap[10] = color;
            if(logicalIndex == 10) m_ColorMap[13] = color;
            SaveColorMap();
            UpdateTasksList();
            return;
        }
        if(logicalIndex == 8) m_ColorMap[13] = color;
        else m_ColorMap[logicalIndex+1] = color;

        SaveColorMap();
        UpdateTasksList();
    }
}

void QTasksWidget::OnTasksDblClk(QTableWidgetItem* item)
{
    if(item->data(Qt::UserRole+1).toUuid() == QUuid(QString("8078b7ce-e423-49ae-9ce6-17758b852b33")))
    {
        QPixmap pixmap(":/icons/CarshServiceIcon256.png");
        QSplashScreen splash(pixmap);
        splash.show();
        splash.showMessage("Загрузка...");
        //QCarshPenaltyParkTaskCardDlg dlg(item->data(Qt::UserRole).toString());
        QPenaltyParkingDialog dlg;
        dlg.LoadDataFromBD(QUuid::fromString(item->data(Qt::UserRole).toString()));
        splash.finish(&dlg);
        dlg.exec();
    }
    if(item->data(Qt::UserRole+1).toUuid() == QUuid(QString("99b4e860-5a7b-42a4-9136-f96252ef4192")))
    {
        QPixmap pixmap(":/icons/CarshServiceIcon256.png");
        QSplashScreen splash(pixmap);
        splash.show();
        splash.showMessage("Загрузка...");
        //QCarshPlateTaskCardDlg dlg(item->data(Qt::UserRole).toString());
        QPlateTaskDialog dlg;
        dlg.LoadDataFromBD(QUuid::fromString(item->data(Qt::UserRole).toString()));
        splash.finish(&dlg);
        dlg.exec();
    }
    if(item->data(Qt::UserRole+1).toUuid() == QUuid(QString("057b3b6f-2848-479b-a546-3f16cb531ffe")))
    {
        QPixmap pixmap(":/icons/CarshServiceIcon256.png");
        QSplashScreen splash(pixmap);
        splash.show();
        splash.showMessage("Загрузка...");
        //QCarshParkingTaskCardDlg dlg(item->data(Qt::UserRole).toString());
        QParkingTaskDialog dlg;
        dlg.LoadDataFromBD(QUuid::fromString(item->data(Qt::UserRole).toString()));
        splash.finish(&dlg);
        dlg.exec();
    }

    if(item->data(Qt::UserRole+1).toUuid() == QUuid(QString("fe81daf9-a838-4bac-84aa-595e038d3a12")))
    {
        QPixmap pixmap(":/icons/CarshServiceIcon256.png");
        QSplashScreen splash(pixmap);
        splash.show();
        splash.showMessage("Загрузка...");
        //QRetToZoneCardDlg dlg(item->data(Qt::UserRole).toString());
        QRetToZoneDialog dlg;
        dlg.LoadDataFromBD(QUuid::fromString(item->data(Qt::UserRole).toString()));
        splash.finish(&dlg);
        dlg.exec();
    }

    if(item->data(Qt::UserRole+1).toUuid() == QUuid(QString("78850df8-814b-41c8-8977-945c085f3021")))
    {
        QPixmap pixmap(":/icons/CarshServiceIcon256.png");
        QSplashScreen splash(pixmap);
        splash.show();
        splash.showMessage("Загрузка...");
        //QSmenaTaskDlg dlg(item->data(Qt::UserRole).toString());
        QSmenaDlg dlg;
        dlg.LoadDataFromBD(QUuid::fromString(item->data(Qt::UserRole).toString()));
        splash.finish(&dlg);
        dlg.exec();
    }

    if(item->data(Qt::UserRole+1).toUuid() == QUuid(QString("25695573-f5fe-43fd-93dc-76ee09e461fa")))
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

void QTasksWidget::OnExcelPressed()
{
    QString strFileName = QFileDialog::getSaveFileName(this , "Отчет" , QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) , tr("Excel (*.xls *.xlsx)"));
    strFileName.append(".xlsx");

    saveTableToExcel(m_pTasksTableWidget , strFileName , "Отчет по задачам");

}

//У задач сотрудников счет формируется только для заказчика, но в двух вариантах - услуги и затраты
void QTasksWidget::OnSchetPressed()
{
    QSqlQuery query;

    QUuid uuidULZakazIdUL = QUuid();

    /*Выбор поставщика тоже делаем диалогом*/
    QSelFromBdDlg selPostDlg("Поставщики" , "Название");
    if(selPostDlg.exec() == QDialog::Rejected) return;

    QUuid uuidULPostavId;

    QString strPostavULQueru = QString("select ЮЛ from Поставщики where id='%1'").arg(selPostDlg.m_strRetId);
    query.exec(strPostavULQueru);
    while(query.next())
        uuidULPostavId = QUuid::fromString(query.value(0).toString());

    //Заполнение массива элементов для счета
    QVector<SSchetItem> vCurrentSchetItems;
    QVector<SSchetItem> vCurrentSchetZatratiItems;

    SSchetItem schetItem;

    int iRowCount = m_pTasksTableWidget->rowCount();

    for(int iRowCounter = 0 ; iRowCounter<iRowCount - 1; iRowCounter++) //-1 - итого
    {
        if(((QCheckBox *)m_pTasksTableWidget->cellWidget(iRowCounter , m_iCheckBoxCol))->isChecked() == false) continue;

        QUuid uuidCurrentLineZakazUL = m_pTasksTableWidget->item(iRowCounter , 1)->data(Qt::UserRole + 5).toUuid();
        //QUuid uuidCurrentLineZakaz   = m_pTasksTableWidget->item(iRowCounter , 1)->data(Qt::UserRole + 6).toUuid();

        if(uuidULZakazIdUL != QUuid() && uuidULZakazIdUL!=uuidCurrentLineZakazUL)
        {
            QMessageBox::information(this , "Счет, акт для заказчика" , "В таблице отмечены позиции для разных заказчиков. Для формирования счета и акта для заказчика оставьте отмеченными только позиции одного заказчика");
            return;
        }
        uuidULZakazIdUL = uuidCurrentLineZakazUL;

        schetItem.dblCount = 1;
        schetItem.strName = m_pTasksTableWidget->item(iRowCounter , 1)->text() + "(" + m_pTasksTableWidget->item(iRowCounter , 4)->text() + ")";
        schetItem.strUnitMeasure =" шт.";
        schetItem.dblItemPrice = m_pTasksTableWidget->item(iRowCounter , 1)->data(Qt::UserRole +4).toDouble();//Стоимость задачи для заказчика

        vCurrentSchetItems.push_back(schetItem);

        schetItem.dblItemPrice = m_pTasksTableWidget->item(iRowCounter , 1)->data(Qt::UserRole +2).toDouble();//Затраты
        vCurrentSchetZatratiItems.push_back(schetItem);
    }

    QString strFileName = QFileDialog::getSaveFileName(this , "Счет" , QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) , tr("Excel (*.xls *.xlsx)"));
    strFileName.append(".xlsx");
    QString strFileNameZatrati = strFileName;
    strFileNameZatrati.insert(strFileNameZatrati.length()-5 , "_Затраты");

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

        //затраты

        strTmpFile = GetTempFNameSchet();


        strNumber = GenNextShcetActNumber();

        WriteULsSchetInfo(strTmpFile , uuidULZakazIdUL , uuidULPostavId , vCurrentSchetZatratiItems , strNumber);

        QFile::remove(strFileNameZatrati);

        if(QFile::copy(strTmpFile , strFileNameZatrati))
        {
            QMessageBox::information(this , "КаршерингСервис" , "Счет на затраты сохранен " + strFileNameZatrati);
        }
        else
        {
            QMessageBox::information(this , "КаршерингСервис" , "Не удалось сохранить счет на затраты " + strFileNameZatrati);
        }

        strTmpFile = GetTempFNameAct();

        WriteULsActInfo(strTmpFile , uuidULZakazIdUL , uuidULPostavId , vCurrentSchetZatratiItems , strNumber);

        strFileNameZatrati.insert(strFileNameZatrati.length() - 5 , "_Акт");

        QFile::remove(strFileNameZatrati);


        if(QFile::copy(strTmpFile , strFileNameZatrati))
        {
            QMessageBox::information(this , "КаршерингСервис" , "Акт на затраты сохранен " + strFileNameZatrati);
        }
        else
        {
            QMessageBox::information(this , "КаршерингСервис" , "Не удалось сохранить акт на затраты" + strFileNameZatrati);
        }


        DeleteTempFiles();
    }
}

void QTasksWidget::UpdateTasksList()
{
    m_pTasksTableWidget->clear();
    m_pTasksTableWidget->clearSpans();
    m_pTasksTableWidget->setRowCount(0);

    /*Задачи пользователей*/
    QStringList headers;

    QLocale locale(QLocale::Russian);

    QString strQuery =  QString("SELECT Задачи.id, Задачи.\"Дата Время\", \"Типы задач\".\"Тип\" , \"Типы задач\".id , Задачи.\"Время выполнения\" , Заказчики.Название , Задачи.Цена , %3 , Пользователи.Имя, Пользователи.Фамилия , Пользователи. Отчество ,  Заказчики.id , Заказчики.ЮЛ, ЦеныЗаказчиков.Цена,  %4 FROM \"Типы задач\", Задачи, Заказчики, Пользователи, ЦеныЗаказчиков where ЦеныЗаказчиков.Заказчик=Заказчики.id and ЦеныЗаказчиков.ТипЗадачи=Задачи.Тип and Заказчики.id=Задачи.Заказчик and Задачи.Исполнитель=Пользователи.id and Задачи.Тип = \"Типы задач\".id and Задачи.Удалено<> 'true'  %2 order by Задачи.\"Дата Время\" desc").arg(m_filtersStr).arg(NUMBER_BY_TASK).arg(PAY_BY_TASK);
    headers << "Дата/время" << "Задача" << "Заказчик"<<"Сумма сотруднику"<<"Сумма заказчику"<<"ГРЗ"<<"Сотрудник"<<"Затраты"<<" ";
    m_pTasksTableWidget->setColumnCount(m_iCheckBoxCol + 1);

    /*Выбрана штрафстоянка - доп. столбцы штрафстоянки*/
    if(m_pTaskTypeComboBox->currentData().toUuid()==QUuid(QString("8078b7ce-e423-49ae-9ce6-17758b852b33")))
    {
        headers.clear();
        headers << "Дата/время" << "Задача" << "Заказчик"<<"Сумма сотруднику"<<"Сумма заказчику"<<"ГРЗ"<<"Сотрудник"<<"Затраты"<<"Причина задержания"<<"От заказчика"<<" ";
        strQuery =  QString("SELECT Задачи.id, Задачи.\"Дата Время\", \"Типы задач\".\"Тип\" , \"Типы задач\".id , Задачи.\"Время выполнения\" , Заказчики.Название , Задачи.Цена , %2 , Пользователи.Имя, Пользователи.Фамилия , Пользователи. Отчество ,  Заказчики.id , Заказчики.ЮЛ, ЦеныЗаказчиков.Цена,  %3 , \"Причины задержания\".Название , EXISTS(select 1 from ЗадачиЗаказчикаШС where ПереведенаВЗадачу = Задачи.id) FROM \"Типы задач\", Задачи, Заказчики, Пользователи, ЦеныЗаказчиков, \"Расширение задачи ШС\", \"Причины задержания\" where ЦеныЗаказчиков.Заказчик=Заказчики.id and ЦеныЗаказчиков.ТипЗадачи=Задачи.Тип and Заказчики.id=Задачи.Заказчик and Задачи.Исполнитель=Пользователи.id and Задачи.Тип = \"Типы задач\".id and Задачи.Удалено<> 'true' and Задачи.Расширение = \"Расширение задачи ШС\".id and \"Расширение задачи ШС\".\"Причина задержания\"=\"Причины задержания\".id %1 order by Задачи.\"Дата Время\" desc").arg(m_filtersStr).arg(NUMBER_BY_TASK).arg(PAY_BY_TASK);
    }
    /*Выбран тип Документы*/
    if(m_pTaskTypeComboBox->currentData().toUuid() == QUuid(QString("25695573-f5fe-43fd-93dc-76ee09e461fa")))
    {
        headers.clear();
        headers << "Дата/время" << "Задача" << "Заказчик"<<"Сумма сотруднику"<<"Сумма заказчику"<<"ГРЗ"<<"Сотрудник"<<"Затраты"<<"Количество"<<"Тип документа"<<" ";
        strQuery =  QString("SELECT Задачи.id, Задачи.\"Дата Время\", \"Типы задач\".\"Тип\" , \"Типы задач\".id , Задачи.\"Время выполнения\" , Заказчики.Название , Задачи.Цена , %2 , Пользователи.Имя, Пользователи.Фамилия , Пользователи. Отчество ,  Заказчики.id , Заказчики.ЮЛ, ЦеныЗаказчиков.Цена,  %3 , \"Расширение задачи Документы\".Количество, \"Документы задачи документы\".Документ   FROM \"Типы задач\", Задачи, Заказчики, Пользователи, ЦеныЗаказчиков, \"Расширение задачи Документы\", \"Документы задачи документы\" where ЦеныЗаказчиков.Заказчик=Заказчики.id and ЦеныЗаказчиков.ТипЗадачи=Задачи.Тип and Заказчики.id=Задачи.Заказчик and Задачи.Исполнитель=Пользователи.id and Задачи.Тип = \"Типы задач\".id and Задачи.Удалено<> 'true' and Задачи.Расширение = \"Расширение задачи Документы\".id and \"Расширение задачи Документы\".\"Документ\"=\"Документы задачи документы\".id %1 order by Задачи.\"Дата Время\" desc").arg(m_filtersStr).arg(NUMBER_BY_TASK).arg(PAY_BY_TASK);
    }

    m_pTasksTableWidget->setHorizontalHeaderLabels(headers);

    QSqlQuery query;
    query.exec(strQuery);

    if(query.size() < 1) return;

    m_pTasksTableWidget->setRowCount(query.size() + 1);//+1 для итого

    int iRowCounter = 0;
    double dblSumm        = 0;//Итого сотрудникам
    double dblPay         = 0;//Итого затраты
    double dblZakazSumm   = 0;//Итого заказчику
    while(query.next())
    {
        QTableWidgetItem * pItem = new QTableWidgetItem(QDateTime::fromSecsSinceEpoch(query.value(1).toInt()).toString("dd.MM.yyyy hh:mm"));
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        pItem->setBackground(QBrush(m_ColorMap[1]));
        m_pTasksTableWidget->setItem(iRowCounter , 0,  pItem);

        /*Тип задачи - по этому столбцу строится счет, поэтому в нем сохраним затраты и и сумму*/
        pItem = new QTableWidgetItem(query.value(2).toString());
        pItem->setData(Qt::UserRole , query.value(0));//id задачи
        pItem->setData(Qt::UserRole +1 , query.value(3));//id типа задачи
        pItem->setData(Qt::UserRole +2 , query.value(14));//Затраты
        pItem->setData(Qt::UserRole +3 , query.value(6));//Сумма сотруднику (Задачи.цена)
        pItem->setData(Qt::UserRole +4 , query.value(13));//Сумма заказчику (ЦеныЗаказчиков.Цена)
        pItem->setData(Qt::UserRole +5 , query.value(12));//Заказчики.ЮЛ
        pItem->setData(Qt::UserRole +6 , query.value(11));//Заказчики.id
        pItem->setBackground(QBrush(m_ColorMap[2]));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTasksTableWidget->setItem(iRowCounter , 1,  pItem);

        /*Заказчик*/
        pItem = new QTableWidgetItem(query.value(5).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        pItem->setBackground(QBrush(m_ColorMap[3]));
        m_pTasksTableWidget->setItem(iRowCounter , 2,  pItem);

        /*Сумма сотруднику*/
        pItem = new QTableWidgetItem(locale.toString(query.value(6).toInt()));
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        pItem->setBackground(QBrush(m_ColorMap[4]));
        if(query.value(6).toInt() == 0) pItem->setBackground(QBrush(Qt::red));
        m_pTasksTableWidget->setItem(iRowCounter , 3,  pItem);
        dblSumm = dblSumm + query.value(6).toDouble();

        /*Сумма для заказчика*/
        pItem = new QTableWidgetItem(locale.toString(query.value(13).toInt()));
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        pItem->setBackground(QBrush(m_ColorMap[5]));
        m_pTasksTableWidget->setItem(iRowCounter , 4,  pItem);
        dblZakazSumm = dblZakazSumm + query.value(13).toDouble();

        /*ГРЗ*/
        pItem = new QTableWidgetItem(query.value(7).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        pItem->setBackground(QBrush(m_ColorMap[6]));
        m_pTasksTableWidget->setItem(iRowCounter , 5,  pItem);

        /*Сотрудник*/
        pItem = new QTableWidgetItem(QString("%1 %2 %3").arg(query.value(8).toString()).arg(query.value(9).toString()).arg(query.value(10).toString()));
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        pItem->setBackground(QBrush(m_ColorMap[7]));
        m_pTasksTableWidget->setItem(iRowCounter , 6,  pItem);

        /*Затраты*/
        pItem = new QTableWidgetItem(locale.toString(query.value(14).toInt()));
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        pItem->setBackground(QBrush(m_ColorMap[8]));
        m_pTasksTableWidget->setItem(iRowCounter , 7,  pItem);
        dblPay = dblPay + query.value(14).toDouble();

        /*Выбрана штрафстоянка - доп. столбцы штрафстоянки*/
        if(m_pTaskTypeComboBox->currentData().toUuid()==QUuid(QString("8078b7ce-e423-49ae-9ce6-17758b852b33")))
        {
            /*Причина задержания*/
            pItem = new QTableWidgetItem(query.value(15).toString());
            pItem->setData(Qt::UserRole , query.value(0));
            pItem->setData(Qt::UserRole +1 , query.value(3));
            pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            pItem->setBackground(QBrush(m_ColorMap[11]));
            m_pTasksTableWidget->setItem(iRowCounter , 8,  pItem);

            /*Задача на штрафстоянку получена от заказчика*/
            if(query.value(16).toBool()) pItem = new QTableWidgetItem("Да");
            else pItem = new QTableWidgetItem("Нет");
            pItem->setData(Qt::UserRole , query.value(0));
            pItem->setData(Qt::UserRole +1 , query.value(3));
            pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            pItem->setBackground(QBrush(m_ColorMap[12]));
            m_pTasksTableWidget->setItem(iRowCounter , 9,  pItem);

        }

        /*Выбран тип Документы*/
        if(m_pTaskTypeComboBox->currentData().toUuid() == QUuid(QString("25695573-f5fe-43fd-93dc-76ee09e461fa")))
        {
            /*Количество документов*/
            pItem = new QTableWidgetItem(query.value(15).toString());
            pItem->setData(Qt::UserRole , query.value(0));
            pItem->setData(Qt::UserRole +1 , query.value(3));
            pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            pItem->setBackground(QBrush(m_ColorMap[9]));
            m_pTasksTableWidget->setItem(iRowCounter , 8,  pItem);

            /*Тип документов*/
            pItem = new QTableWidgetItem(query.value(16).toString());
            pItem->setData(Qt::UserRole , query.value(0));
            pItem->setData(Qt::UserRole +1 , query.value(3));
            pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            pItem->setBackground(QBrush(m_ColorMap[10]));
            m_pTasksTableWidget->setItem(iRowCounter , 9,  pItem);
        }

        /*Чек-бокс*/
        QCheckBox * pCheckBox = new QCheckBox();
        pItem = new QTableWidgetItem();
        pItem->setBackground(QBrush(m_ColorMap[13]));
        m_pTasksTableWidget->setItem(iRowCounter , m_iCheckBoxCol,  pItem);
        pCheckBox->setChecked(true);
        m_pTasksTableWidget->setCellWidget(iRowCounter , m_iCheckBoxCol , pCheckBox);

        iRowCounter++;
    }

    m_pTasksTableWidget->setRowCount(iRowCounter + 1);//+1 для итого

    /*Добавим Итого*/
    m_pTasksTableWidget->setSpan(iRowCounter , 0 , 1 , 3);
    QTableWidgetItem * pItem = new QTableWidgetItem(QString("Итого: "));
    m_pTasksTableWidget->setItem(iRowCounter , 0,  pItem);

    pItem = new QTableWidgetItem(QString(" %1 руб.").arg(locale.toString((int)dblSumm)));
    m_pTasksTableWidget->setItem(iRowCounter , 3,  pItem);

    pItem = new QTableWidgetItem(QString(" %1 руб.").arg(locale.toString((int)dblZakazSumm)));
    m_pTasksTableWidget->setItem(iRowCounter , 4,  pItem);

    pItem = new QTableWidgetItem(QString(" %1 руб.").arg(locale.toString((int)dblPay)));
    m_pTasksTableWidget->setItem(iRowCounter , 7,  pItem);

    QFont font;
    font.setBold(true);
    m_pTasksTableWidget->item(iRowCounter, 0)->setFont(font);

    m_pTasksTableWidget->resizeColumnsToContents();
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
        if(m_pTaskTypeComboBox->currentData().toUuid()==QUuid(QString("8078b7ce-e423-49ae-9ce6-17758b852b33")))
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
            if(m_pPenalParkTaskFilterWidget->m_pOnlyFromZakazchikCheckBox->isChecked())
            {
                strPenParkFilter = QString("  and EXISTS(select 1 from ЗадачиЗаказчикаШС where ПереведенаВЗадачу = Задачи.id) ");
                typeFilterString = typeFilterString.append(strPenParkFilter);
            }
            if(m_pPenalParkTaskFilterWidget->m_pOnlyManualCreatedCheckBox->isChecked())
            {
                strPenParkFilter = QString("  and NOT EXISTS(select 1 from ЗадачиЗаказчикаШС where ПереведенаВЗадачу = Задачи.id) ");
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

    QString FinishFilterString("");
    if(m_pOnlyFinishedCheckBox->isChecked())
    {
        FinishFilterString = " and Задачи.Цена <> 0 ";
    }
    if(m_pOnlyUnfinishedCheckBox->isChecked())
    {
        FinishFilterString = " and Задачи.Цена = 0 ";
    }

    m_filtersStr = QString("and Задачи.\"Дата Время\">'%1' and Задачи.\"Дата Время\"<'%2' %3 %4 %5 %6 %7").arg(m_pFromDateTimeEdit->dateTime().toSecsSinceEpoch()).arg(m_pToDateTimeEdit->dateTime().toSecsSinceEpoch()).arg(typeFilterString).arg(numberFilterString).arg(emplFilterString).arg(CarshsFilterString).arg(FinishFilterString);
    UpdateTasksList();
}


void QTasksWidget::onFinishedStateChanged(int state) {
    if (state == Qt::Checked) {
        // Блокируем сигналы второго чекбокса на время изменения
        m_pOnlyUnfinishedCheckBox->blockSignals(true);
        m_pOnlyUnfinishedCheckBox->setChecked(false);
        m_pOnlyUnfinishedCheckBox->blockSignals(false);
    }
}

void QTasksWidget::onUnfinishedStateChanged(int state) {
    if (state == Qt::Checked) {
        // Блокируем сигналы первого чекбокса на время изменения
        m_pOnlyFinishedCheckBox->blockSignals(true);
        m_pOnlyFinishedCheckBox->setChecked(false);
        m_pOnlyFinishedCheckBox->blockSignals(false);
    }
}
