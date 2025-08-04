#include "qtaskswidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QUuid>
#include <QSqlQuery>
#include <QLineEdit>
#include <QDateTime>
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
#include "TasksSubWidgets/qnewtaskdlg.h"
#include "service/qselempldlg.h"
#include <QDebug>

extern QSettings settings;

QTasksWidget::QTasksWidget(QWidget *parent)
    : QWidget{parent}
{
    LoadColorMap();

    m_filtersStr = QString("");
    m_iCheckBoxCol = 9;



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

    m_pDateByEndOfTask = new QCheckBox("Время завершения задачи");
    pFilterHLoyuot2->addWidget(m_pDateByEndOfTask);

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

    QPushButton * pCreateTaskButton = new QPushButton("Создать задачу");
    connect(pCreateTaskButton,SIGNAL(pressed()),this,SLOT(OnCreateTaskPressed()));
    pButtonsHLoyuot->addWidget(pCreateTaskButton);

    QPushButton * pSchetApplyButton = new QPushButton("Счет/Акт");
    connect(pSchetApplyButton,SIGNAL(pressed()),this,SLOT(OnSchetPressed()));
    pButtonsHLoyuot->addWidget(pSchetApplyButton);

    QPushButton * pExcelButton = new QPushButton("Экспорт в Excel");
    connect(pExcelButton,SIGNAL(pressed()),this,SLOT(OnExcelPressed()));
    pButtonsHLoyuot->addWidget(pExcelButton);

    pVMainLayout->addLayout(pButtonsHLoyuot);

    pVMainLayout->addSpacing(5);

    m_pTasksTableWidget = new QTableWidget;
    m_pTasksTableWidget->setColumnCount(10);
    //m_pTasksTableWidget->setColumnHidden(2,true);  //Скрыли зазказчика
    QStringList headers;
    headers << "Дата/время" << "Дата/время завершения"<< "Задача" << "Заказчик"<<"Сумма сотруднику"<<"Сумма заказчику"<<"ГРЗ"<<"Сотрудник"<<"Затраты"<<" ";
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
    m_ColorMap[14] = settings.value("QTasksWidget_Ramki_Col" , m_defaultColor).value<QColor>();
    m_ColorMap[15] = settings.value("QTasksWidget_Plate_Col" , m_defaultColor).value<QColor>();
    m_ColorMap[16] = settings.value("QTasksWidget_DateTime_end" , m_defaultColor).value<QColor>();
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
    settings.setValue("QTasksWidget_Ramki_Col", m_ColorMap[14]);
    settings.setValue("QTasksWidget_Plate_Col", m_ColorMap[15]);
    settings.setValue("QTasksWidget_DateTime_end", m_ColorMap[16]);
}

void QTasksWidget::TaskTypeComboChanged(int )
{

    /*Выбран тип Штрафстоянка*/
    if(m_pTaskTypeComboBox->currentData().toUuid() == QUuid(QString("8078b7ce-e423-49ae-9ce6-17758b852b33")))
    {
        m_pPenalParkTaskFilterWidget->show();
        m_iCheckBoxCol = 11;
        return;
    }
    /*Выбраны все типы или один из типов без доп. панели фильтров*/
    m_pPenalParkTaskFilterWidget->hide();

    /*Выбран тип Документы*/
    if(m_pTaskTypeComboBox->currentData().toUuid() == QUuid(QString("25695573-f5fe-43fd-93dc-76ee09e461fa")))
    {
        m_iCheckBoxCol = 11;

        return;
    }

    /*Выбран тип Номера*/
    if(m_pTaskTypeComboBox->currentData().toUuid() == QUuid(QString("99b4e860-5a7b-42a4-9136-f96252ef4192")))
    {
        m_iCheckBoxCol = 11;
        return;
    }

    m_iCheckBoxCol=9;

}

void QTasksWidget::onHeaderDoubleClicked(int logicalIndex)
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Выберите цвет");
    if (color.isValid()) {

        /*Выбрана штрафстоянка - доп. столбцы штрафстоянки*/
        if(m_pTaskTypeComboBox->currentData().toUuid()==QUuid(QString("8078b7ce-e423-49ae-9ce6-17758b852b33"))&&(logicalIndex==9 || logicalIndex==10 || logicalIndex==11))
        {
            if(logicalIndex == 9) m_ColorMap[11] = color;
            if(logicalIndex == 10) m_ColorMap[12] = color;
            if(logicalIndex == 11) m_ColorMap[13] = color;
            SaveColorMap();
            UpdateTasksList();
            return;
        }
        /*Выбран тип Документы*/
        if(m_pTaskTypeComboBox->currentData().toUuid() == QUuid(QString("25695573-f5fe-43fd-93dc-76ee09e461fa"))&&(logicalIndex==9 || logicalIndex==10 || logicalIndex==11))
        {
            if(logicalIndex == 9) m_ColorMap[9] = color;
            if(logicalIndex == 10) m_ColorMap[10] = color;
            if(logicalIndex == 11) m_ColorMap[13] = color;
            SaveColorMap();
            UpdateTasksList();
            return;
        }
        /*Выбран тип Документы*/
        if(m_pTaskTypeComboBox->currentData().toUuid() == QUuid(QString("99b4e860-5a7b-42a4-9136-f96252ef4192"))&&(logicalIndex==9 || logicalIndex==10 || logicalIndex==11))
        {
            if(logicalIndex == 9) m_ColorMap[14] = color;
            if(logicalIndex == 10) m_ColorMap[15] = color;
            if(logicalIndex == 11) m_ColorMap[13] = color;
            SaveColorMap();
            UpdateTasksList();
            return;
        }


        if(logicalIndex == 9) m_ColorMap[13] = color;
            else if(logicalIndex == 0) m_ColorMap[1] = color;
                else if(logicalIndex == 1) m_ColorMap[16] = color;
                    else m_ColorMap[logicalIndex] = color;

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
        if(dlg.exec() == QDialog::Accepted) UpdateTasksList();
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
        if(dlg.exec() == QDialog::Accepted) UpdateTasksList();
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
        if(dlg.exec() == QDialog::Accepted) UpdateTasksList();
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
        if(dlg.exec() == QDialog::Accepted) UpdateTasksList();
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
        if(dlg.exec() == QDialog::Accepted) UpdateTasksList();
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
        if(dlg.exec() == QDialog::Accepted) UpdateTasksList();
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

        QUuid uuidCurrentLineZakazUL = m_pTasksTableWidget->item(iRowCounter , 2)->data(Qt::UserRole + 5).toUuid();
        //QUuid uuidCurrentLineZakaz   = m_pTasksTableWidget->item(iRowCounter , 1)->data(Qt::UserRole + 6).toUuid();

        if(uuidULZakazIdUL != QUuid() && uuidULZakazIdUL!=uuidCurrentLineZakazUL)
        {
            QMessageBox::information(this , "Счет, акт для заказчика" , "В таблице отмечены позиции для разных заказчиков. Для формирования счета и акта для заказчика оставьте отмеченными только позиции одного заказчика");
            return;
        }
        uuidULZakazIdUL = uuidCurrentLineZakazUL;

        schetItem.dblCount = 1;
        schetItem.strName = m_pTasksTableWidget->item(iRowCounter , 2)->text() + "(" + m_pTasksTableWidget->item(iRowCounter , 6)->text() + ")"; //Тип(ГРЗ)
        schetItem.strUnitMeasure =" шт.";
        schetItem.dblItemPrice = m_pTasksTableWidget->item(iRowCounter , 2)->data(Qt::UserRole +4).toDouble();//Стоимость задачи для заказчика

        vCurrentSchetItems.push_back(schetItem);

        schetItem.dblItemPrice = m_pTasksTableWidget->item(iRowCounter , 2)->data(Qt::UserRole +2).toDouble();//Затраты
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

    QString strTimeBase("\"Дата Время\"");
    if(m_pDateByEndOfTask->isChecked())
        strTimeBase=QString("\"Время выполнения\"");


    //QString strQuery =  QString("SELECT Задачи.id, Задачи.\"Дата Время\", \"Типы задач\".\"Тип\" , \"Типы задач\".id , Задачи.\"Время выполнения\" , Заказчики.Название , Задачи.Цена , %3 , Пользователи.Имя, Пользователи.Фамилия , Пользователи. Отчество ,  Заказчики.id , Заказчики.ЮЛ, ЦеныЗаказчиков.Цена , %4 FROM \"Типы задач\", Задачи, Заказчики, Пользователи, ЦеныЗаказчиков where ЦеныЗаказчиков.Заказчик=Заказчики.id and ЦеныЗаказчиков.ТипЗадачи=Задачи.Тип and Заказчики.id=Задачи.Заказчик and Задачи.Исполнитель=Пользователи.id and Задачи.Тип = \"Типы задач\".id and Задачи.Удалено<> 'true'  %2 order by Задачи.%5 desc").arg(m_filtersStr).arg(NUMBER_BY_TASK).arg(PAY_BY_TASK).arg(strTimeBase);
    QString strQuery = QString(
        "SELECT "
            "t.id, "
            "t.\"Дата Время\", "
            "tt.\"Тип\", "
            "tt.id AS \"Тип id\", "
            "t.\"Время выполнения\", "
            "c.\"Название\" AS \"Заказчик\", "
            "t.\"Цена\", "
            "%1, "  // NUMBER_BY_TASK
            "u.\"Имя\", "
            "u.\"Фамилия\", "
            "u.\"Отчество\", "
            "c.id AS \"Заказчик id\", "
            "c.\"ЮЛ\", "
            "p.\"Цена\" AS \"Цена заказчика\", "
            "%2 "  // PAY_BY_TASK
        "FROM Задачи t "
        "INNER JOIN \"Типы задач\" tt ON t.\"Тип\" = tt.id "
        "INNER JOIN \"Заказчики\" c ON t.\"Заказчик\" = c.id "
        "INNER JOIN \"Пользователи\" u ON t.\"Исполнитель\" = u.id "
        "INNER JOIN \"ЦеныЗаказчиков\" p ON "
            "p.\"Заказчик\" = c.id AND "
            "p.\"ТипЗадачи\" = t.\"Тип\" "
        "WHERE t.\"Удалено\" <> 'true' "
        "%3 "   // m_filtersStr
        "ORDER BY t.%4 DESC"  // Сортировка
    )
    .arg(NUMBER_BY_TASK)      // %1
    .arg(PAY_BY_TASK)         // %2
    .arg(m_filtersStr)        // %3
    .arg(strTimeBase);        // %4

    headers << "Дата/время" << "Дата/время завершения" << "Задача" << "Заказчик"<<"Сумма сотруднику"<<"Сумма заказчику"<<"ГРЗ"<<"Сотрудник"<<"Затраты"<<" ";
    m_pTasksTableWidget->setColumnCount(m_iCheckBoxCol + 1);

    /*Выбрана штрафстоянка - доп. столбцы штрафстоянки*/
    if(m_pTaskTypeComboBox->currentData().toUuid()==QUuid(QString("8078b7ce-e423-49ae-9ce6-17758b852b33")))
    {
        headers.clear();
        headers << "Дата/время" << "Дата/время завершения"<< "Задача" << "Заказчик"<<"Сумма сотруднику"<<"Сумма заказчику"<<"ГРЗ"<<"Сотрудник"<<"Затраты"<<"Причина задержания"<<"От заказчика"<<" ";
        //strQuery =  QString("SELECT Задачи.id, Задачи.\"Дата Время\", \"Типы задач\".\"Тип\" , \"Типы задач\".id , Задачи.\"Время выполнения\" , Заказчики.Название , Задачи.Цена , %2 , Пользователи.Имя, Пользователи.Фамилия , Пользователи. Отчество ,  Заказчики.id , Заказчики.ЮЛ, ЦеныЗаказчиков.Цена,  %3 , \"Причины задержания\".Название , EXISTS(select 1 from ЗадачиЗаказчикаШС where ПереведенаВЗадачу = Задачи.id) FROM \"Типы задач\", Задачи, Заказчики, Пользователи, ЦеныЗаказчиков, \"Расширение задачи ШС\", \"Причины задержания\" where ЦеныЗаказчиков.Заказчик=Заказчики.id and ЦеныЗаказчиков.ТипЗадачи=Задачи.Тип and Заказчики.id=Задачи.Заказчик and Задачи.Исполнитель=Пользователи.id and Задачи.Тип = \"Типы задач\".id and Задачи.Удалено<> 'true' and Задачи.Расширение = \"Расширение задачи ШС\".id and \"Расширение задачи ШС\".\"Причина задержания\"=\"Причины задержания\".id %1 order by Задачи.\"Дата Время\" desc").arg(m_filtersStr).arg(NUMBER_BY_TASK).arg(PAY_BY_TASK);
        strQuery = QString(
            "SELECT "
                "t.id, "
                "t.\"Дата Время\", "
                "tt.\"Тип\", "
                "tt.id AS \"Тип id\", "
                "t.\"Время выполнения\", "
                "c.\"Название\" AS \"Заказчик\", "
                "t.\"Цена\", "
                "%1, "  // NUMBER_BY_TASK
                "u.\"Имя\", "
                "u.\"Фамилия\", "
                "u.\"Отчество\", "
                "c.id AS \"Заказчик id\", "
                "c.\"ЮЛ\", "
                "p.\"Цена\" AS \"Цена заказчика\", "
                "%2, "  // PAY_BY_TASK
                "pr.\"Название\" AS \"Причина\", "
                "EXISTS(SELECT 1 FROM \"ЗадачиЗаказчикаШС\" WHERE \"ПереведенаВЗадачу\" = t.id) AS \"От заказчика\" "
            "FROM Задачи t "
            "INNER JOIN \"Типы задач\" tt ON t.\"Тип\" = tt.id "
            "INNER JOIN \"Заказчики\" c ON t.\"Заказчик\" = c.id "
            "INNER JOIN \"Пользователи\" u ON t.\"Исполнитель\" = u.id "
            "INNER JOIN \"ЦеныЗаказчиков\" p ON p.\"Заказчик\" = c.id AND p.\"ТипЗадачи\" = t.\"Тип\" "
            // Обязательное соединение с расширением ШС
            "INNER JOIN \"Расширение задачи ШС\" rs ON t.\"Расширение\" = rs.id "
            // Необязательное соединение с причинами задержания
            "LEFT JOIN \"Причины задержания\" pr ON rs.\"Причина задержания\" = pr.id "
            "WHERE t.\"Удалено\" <> 'true' "
            "%3 "   // Фильтры (m_filtersStr)
            "ORDER BY t.%4 DESC"
        )
        .arg(NUMBER_BY_TASK)   // %1
        .arg(PAY_BY_TASK)      // %2
        .arg(m_filtersStr)     // %3
        .arg(strTimeBase);     // %4
    }
    /*Выбран тип Документы*/
    if(m_pTaskTypeComboBox->currentData().toUuid() == QUuid(QString("25695573-f5fe-43fd-93dc-76ee09e461fa")))
    {
        headers.clear();
        headers << "Дата/время" << "Дата/время завершения"<< "Задача" << "Заказчик"<<"Сумма сотруднику"<<"Сумма заказчику"<<"ГРЗ"<<"Сотрудник"<<"Затраты"<<"Количество"<<"Тип документа"<<" ";
        //strQuery =  QString("SELECT Задачи.id, Задачи.\"Дата Время\", \"Типы задач\".\"Тип\" , \"Типы задач\".id , Задачи.\"Время выполнения\" , Заказчики.Название , Задачи.Цена , %2 , Пользователи.Имя, Пользователи.Фамилия , Пользователи. Отчество ,  Заказчики.id , Заказчики.ЮЛ, ЦеныЗаказчиков.Цена,  %3 , \"Расширение задачи Документы\".Количество, \"Документы задачи документы\".Документ   FROM \"Типы задач\", Задачи, Заказчики, Пользователи, ЦеныЗаказчиков, \"Расширение задачи Документы\", \"Документы задачи документы\" where ЦеныЗаказчиков.Заказчик=Заказчики.id and ЦеныЗаказчиков.ТипЗадачи=Задачи.Тип and Заказчики.id=Задачи.Заказчик and Задачи.Исполнитель=Пользователи.id and Задачи.Тип = \"Типы задач\".id and Задачи.Удалено<> 'true' and Задачи.Расширение = \"Расширение задачи Документы\".id and \"Расширение задачи Документы\".\"Документ\"=\"Документы задачи документы\".id %1 order by Задачи.\"Дата Время\" desc").arg(m_filtersStr).arg(NUMBER_BY_TASK).arg(PAY_BY_TASK);
        strQuery = QString(
            "SELECT "
                "t.id, "
                "t.\"Дата Время\", "
                "tt.\"Тип\", "
                "tt.id AS \"Тип id\", "
                "t.\"Время выполнения\", "
                "c.\"Название\" AS \"Заказчик\", "
                "t.\"Цена\", "
                "%1, "  // NUMBER_BY_TASK
                "u.\"Имя\", "
                "u.\"Фамилия\", "
                "u.\"Отчество\", "
                "c.id AS \"Заказчик id\", "
                "c.\"ЮЛ\", "
                "p.\"Цена\" AS \"Цена заказчика\", "
                "%2, "  // PAY_BY_TASK
                "de.\"Количество\", "
                "dt.\"Документ\" "
            "FROM Задачи t "
            "INNER JOIN \"Типы задач\" tt ON t.\"Тип\" = tt.id "
            "INNER JOIN \"Заказчики\" c ON t.\"Заказчик\" = c.id "
            "INNER JOIN \"Пользователи\" u ON t.\"Исполнитель\" = u.id "
            "INNER JOIN \"ЦеныЗаказчиков\" p ON p.\"Заказчик\" = c.id AND p.\"ТипЗадачи\" = t.\"Тип\" "
            "LEFT JOIN \"Расширение задачи Документы\" de ON t.\"Расширение\" = de.id "
            "LEFT JOIN \"Документы задачи документы\" dt ON de.\"Документ\" = dt.id "
            "WHERE t.\"Удалено\" <> 'true' "
            "%3 "   // Фильтры (m_filtersStr)
            "ORDER BY t.%4 DESC"
        )
        .arg(NUMBER_BY_TASK)   // %1
        .arg(PAY_BY_TASK)      // %2
        .arg(m_filtersStr)     // %3
        .arg(strTimeBase);     // %4
    }

    //Выбран тип Номера
    if(m_pTaskTypeComboBox->currentData().toUuid() == QUuid(QString("99b4e860-5a7b-42a4-9136-f96252ef4192")))
    {
        headers.clear();
        headers << "Дата/время" << "Дата/время завершения"<< "Задача" << "Заказчик" << "Сумма сотруднику"
                << "Сумма заказчику" << "ГРЗ" << "Сотрудник" << "Затраты"
                << "Кол-во рамок" << "Кол-во пластин" << " ";

        // Формирование запроса
        strQuery = QString(
            "SELECT "
                "t.id, "
                "t.\"Дата Время\", "
                "tt.\"Тип\", "
                "tt.id AS \"Тип id\", "
                "t.\"Время выполнения\", "
                "c.\"Название\" AS \"Заказчик\", "
                "t.\"Цена\", "
                "%1, "  // NUMBER_BY_TASK (госномер)
                "u.\"Имя\", "
                "u.\"Фамилия\", "
                "u.\"Отчество\", "
                "c.id AS \"Заказчик id\", "
                "c.\"ЮЛ\", "
                "p.\"Цена\" AS \"Цена заказчика\", "
                "%2, "  // PAY_BY_TASK (затраты)
                "rn.\"Количество рамок\", "  // Доп. поле: количество рамок
                "rn.\"Количество пластин\" " // Доп. поле: количество пластин
            "FROM Задачи t "
            "INNER JOIN \"Типы задач\" tt ON t.\"Тип\" = tt.id "
            "INNER JOIN \"Заказчики\" c ON t.\"Заказчик\" = c.id "
            "INNER JOIN \"Пользователи\" u ON t.\"Исполнитель\" = u.id "
            "INNER JOIN \"ЦеныЗаказчиков\" p ON "
                "p.\"Заказчик\" = c.id AND "
                "p.\"ТипЗадачи\" = t.\"Тип\" "
            // Соединение с расширением для задач "Номера"
            "LEFT JOIN \"Расширение задачи Номера\" rn ON t.\"Расширение\" = rn.id "
            "WHERE t.\"Удалено\" <> 'true' "
            "%3 "   // Фильтры (m_filtersStr)
            "ORDER BY t.%4 DESC"
        )
        .arg(NUMBER_BY_TASK)   // %1
        .arg(PAY_BY_TASK)      // %2
        .arg(m_filtersStr)     // %3
        .arg(strTimeBase);     // %4
    }

    m_pTasksTableWidget->setHorizontalHeaderLabels(headers);

    QSqlQuery query;

    query.exec(strQuery);

    qDebug()<<"query = "<<strQuery;
    qDebug()<<"m_filtersStr = "<<m_filtersStr;

    if(query.size() < 1) return;

    m_pTasksTableWidget->setRowCount(query.size() + 1);//+1 для итого

    int iRowCounter = 0;
    double dblSumm        = 0;//Итого сотрудникам
    double dblPay         = 0;//Итого затраты
    double dblZakazSumm   = 0;//Итого заказчику
    while(query.next())
    {
        /*Время*/
        QTableWidgetItem * pItem = new QTableWidgetItem(QDateTime::fromSecsSinceEpoch(query.value(1).toInt()).toString("dd.MM.yyyy hh:mm"));
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        pItem->setBackground(QBrush(m_ColorMap[1]));
        m_pTasksTableWidget->setItem(iRowCounter , 0,  pItem);

        /*Время завершения задачи*/
        if(query.value(4).toInt()!=0)
            pItem = new QTableWidgetItem(QDateTime::fromSecsSinceEpoch(query.value(4).toInt()).toString("dd.MM.yyyy hh:mm"));
        else
            pItem = new QTableWidgetItem("  ");
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        pItem->setBackground(QBrush(m_ColorMap[16]));
        m_pTasksTableWidget->setItem(iRowCounter , 1,  pItem);



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
        m_pTasksTableWidget->setItem(iRowCounter , 2,  pItem);

        /*Заказчик*/
        pItem = new QTableWidgetItem(query.value(5).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        pItem->setBackground(QBrush(m_ColorMap[3]));
        m_pTasksTableWidget->setItem(iRowCounter , 3,  pItem);

        /*Сумма сотруднику*/
        //pItem = new QTableWidgetItem(locale.toString(query.value(6).toInt()));
        pItem = new QTableWidgetItem(query.value(6).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        pItem->setBackground(QBrush(m_ColorMap[4]));
        if(query.value(6).toInt() == 0) pItem->setBackground(QBrush(Qt::red));
        m_pTasksTableWidget->setItem(iRowCounter , 4,  pItem);
        dblSumm = dblSumm + query.value(6).toDouble();

        /*Сумма для заказчика*/
        //pItem = new QTableWidgetItem(locale.toString(query.value(13).toInt()));
        pItem = new QTableWidgetItem(query.value(13).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        pItem->setBackground(QBrush(m_ColorMap[5]));
        m_pTasksTableWidget->setItem(iRowCounter , 5,  pItem);
        dblZakazSumm = dblZakazSumm + query.value(13).toDouble();

        /*ГРЗ*/
        pItem = new QTableWidgetItem(query.value(7).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        pItem->setBackground(QBrush(m_ColorMap[6]));
        m_pTasksTableWidget->setItem(iRowCounter , 6,  pItem);

        /*Сотрудник*/
        pItem = new QTableWidgetItem(QString("%1 %2 %3").arg(query.value(8).toString()).arg(query.value(9).toString()).arg(query.value(10).toString()));
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        pItem->setBackground(QBrush(m_ColorMap[7]));
        m_pTasksTableWidget->setItem(iRowCounter , 7,  pItem);

        /*Затраты*/
        //pItem = new QTableWidgetItem(locale.toString(query.value(14).toInt()));
        pItem = new QTableWidgetItem(query.value(14).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setData(Qt::UserRole +1 , query.value(3));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        pItem->setBackground(QBrush(m_ColorMap[8]));
        m_pTasksTableWidget->setItem(iRowCounter , 8,  pItem);
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
            m_pTasksTableWidget->setItem(iRowCounter , 9,  pItem);

            /*Задача на штрафстоянку получена от заказчика*/
            if(query.value(16).toBool()) pItem = new QTableWidgetItem("Да");
            else pItem = new QTableWidgetItem("Нет");
            pItem->setData(Qt::UserRole , query.value(0));
            pItem->setData(Qt::UserRole +1 , query.value(3));
            pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            pItem->setBackground(QBrush(m_ColorMap[12]));
            m_pTasksTableWidget->setItem(iRowCounter , 10,  pItem);

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
            m_pTasksTableWidget->setItem(iRowCounter , 9,  pItem);

            /*Тип документов*/
            pItem = new QTableWidgetItem(query.value(16).toString());
            pItem->setData(Qt::UserRole , query.value(0));
            pItem->setData(Qt::UserRole +1 , query.value(3));
            pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            pItem->setBackground(QBrush(m_ColorMap[10]));
            m_pTasksTableWidget->setItem(iRowCounter , 10,  pItem);
        }

        /*Выбран тип Номера*/
        if(m_pTaskTypeComboBox->currentData().toUuid() == QUuid(QString("99b4e860-5a7b-42a4-9136-f96252ef4192")))
        {
            /*Количество рамок*/
            pItem = new QTableWidgetItem(query.value(15).toString());
            pItem->setData(Qt::UserRole , query.value(0));
            pItem->setData(Qt::UserRole +1 , query.value(3));
            pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            pItem->setBackground(QBrush(m_ColorMap[14]));
            m_pTasksTableWidget->setItem(iRowCounter , 9,  pItem);

            /*Тип пластин*/
            pItem = new QTableWidgetItem(query.value(16).toString());
            pItem->setData(Qt::UserRole , query.value(0));
            pItem->setData(Qt::UserRole +1 , query.value(3));
            pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            pItem->setBackground(QBrush(m_ColorMap[15]));
            m_pTasksTableWidget->setItem(iRowCounter , 10,  pItem);
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
    m_pTasksTableWidget->setSpan(iRowCounter , 0 , 1 , 4);
    QTableWidgetItem * pItem = new QTableWidgetItem(QString("Итого: "));
    m_pTasksTableWidget->setItem(iRowCounter , 0,  pItem);

    pItem = new QTableWidgetItem(QString(" %1 руб.").arg(locale.toString((int)dblSumm)));
    m_pTasksTableWidget->setItem(iRowCounter , 4,  pItem);

    pItem = new QTableWidgetItem(QString(" %1 руб.").arg(locale.toString((int)dblZakazSumm)));
    m_pTasksTableWidget->setItem(iRowCounter , 5,  pItem);

    pItem = new QTableWidgetItem(QString(" %1 руб.").arg(locale.toString((int)dblPay)));
    m_pTasksTableWidget->setItem(iRowCounter , 8,  pItem);

    QFont font;
    font.setBold(true);
    m_pTasksTableWidget->item(iRowCounter, 0)->setFont(font);

    m_pTasksTableWidget->resizeColumnsToContents();
}



void QTasksWidget::OnFilterApplyPressed()
{
    //TODO: В union добавлять все задачи, в которых ест госномер
    QString numberFilterString = "";
    if(!m_pNumberEdit->text().isEmpty())
    {
        numberFilterString = QString(" and t.\"Расширение\" IN ("
            "(SELECT id FROM \"Расширение задачи Возврат в зону\" WHERE \"Госномер\" ILIKE '%%1%') "
            "UNION ALL "
            "(SELECT id FROM \"Расширение задачи Номера\" WHERE \"Госномер\" ILIKE '%%1%') "
            "UNION ALL "
            "(SELECT id FROM \"Расширение задачи Парковка\" WHERE \"Госномер\" ILIKE '%%1%') "
            "UNION ALL "
            "(SELECT id FROM \"Расширение задачи ШС\" WHERE \"Госномер\" ILIKE '%%1%')"
        ")").arg(m_pNumberEdit->text());
    }

    QString typeFilterString;
    QUuid taskTypeUuid = m_pTaskTypeComboBox->currentData().toUuid();

    if(!taskTypeUuid.isNull())
    {
        typeFilterString = QString(" and t.\"Тип\" = '%1' ").arg(taskTypeUuid.toString());

        // Дополнительные фильтры для штрафстоянки
        if(taskTypeUuid == QUuid("{8078b7ce-e423-49ae-9ce6-17758b852b33}"))
        {
            if(m_pPenalParkTaskFilterWidget->m_pGibddCombo->currentData()!=QVariant(QUuid()))
            {
                typeFilterString += QString(" AND EXISTS(SELECT 1 FROM \"Расширение задачи ШС\" rs "
                    "WHERE rs.id = t.\"Расширение\" AND rs.\"Отдел ГАИ\" = '%1')")
                    .arg(m_pPenalParkTaskFilterWidget->m_pGibddCombo->currentData().toUuid().toString());
            }

            if(m_pPenalParkTaskFilterWidget->m_pParkingCombo->currentData()!=QVariant(QUuid()))
            {
                typeFilterString += QString(" AND EXISTS(SELECT 1 FROM \"Расширение задачи ШС\" rs "
                    "WHERE rs.id = t.\"Расширение\" AND rs.\"Штрафстоянка\" = '%1')")
                    .arg(m_pPenalParkTaskFilterWidget->m_pParkingCombo->currentData().toUuid().toString());
            }

            if(m_pPenalParkTaskFilterWidget->m_pReasonCombo->currentData()!=QVariant(QUuid()))
            {
                typeFilterString += QString(" AND EXISTS(SELECT 1 FROM \"Расширение задачи ШС\" rs "
                    "WHERE rs.id = t.\"Расширение\" AND rs.\"Причина задержания\" = '%1')")
                    .arg(m_pPenalParkTaskFilterWidget->m_pReasonCombo->currentData().toUuid().toString());
            }

            if(m_pPenalParkTaskFilterWidget->m_pOnlyFromZakazchikCheckBox->isChecked())
            {
                typeFilterString += " AND EXISTS(SELECT 1 FROM \"ЗадачиЗаказчикаШС\" WHERE \"ПереведенаВЗадачу\" = t.id)";
            }

            if(m_pPenalParkTaskFilterWidget->m_pOnlyManualCreatedCheckBox->isChecked())
            {
                typeFilterString += " AND NOT EXISTS(SELECT 1 FROM \"ЗадачиЗаказчикаШС\" WHERE \"ПереведенаВЗадачу\" = t.id)";
            }
        }
    }

    QString emplFilterString;
    QUuid emplUuid = m_pEmplComboBox->currentData().toUuid();
    if(!emplUuid.isNull())
    {
        emplFilterString = QString(" and t.\"Исполнитель\" = '%1' ").arg(emplUuid.toString());
    }

    QString CarshsFilterString;
    QUuid customerUuid = m_pCarshsComboBox->currentData().toUuid();
    if(!customerUuid.isNull())
    {
        CarshsFilterString = QString(" and c.id = '%1' ").arg(customerUuid.toString());
    }

    QString FinishFilterString;
    if(m_pOnlyFinishedCheckBox->isChecked())
    {
        FinishFilterString = " and t.\"Цена\" <> 0 ";
    }
    else if(m_pOnlyUnfinishedCheckBox->isChecked())
    {
        FinishFilterString = " and t.\"Цена\" = 0 ";
    }

    QString strTimeBase("t.\"Дата Время\"");
    if(m_pDateByEndOfTask->isChecked())
        strTimeBase = "t.\"Время выполнения\"";

    m_filtersStr = QString("and %1 > '%2' and %1 < '%3' %4 %5 %6 %7 %8")
        .arg(strTimeBase)
        .arg(m_pFromDateTimeEdit->dateTime().toSecsSinceEpoch())
        .arg(m_pToDateTimeEdit->dateTime().toSecsSinceEpoch())
        .arg(typeFilterString)
        .arg(numberFilterString)
        .arg(emplFilterString)
        .arg(CarshsFilterString)
        .arg(FinishFilterString);

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

void QTasksWidget::onUnfinishedStateChanged(int state)
{
    if (state == Qt::Checked) {
        // Блокируем сигналы первого чекбокса на время изменения
        m_pOnlyFinishedCheckBox->blockSignals(true);
        m_pOnlyFinishedCheckBox->setChecked(false);
        m_pOnlyFinishedCheckBox->blockSignals(false);
    }
}

void QTasksWidget::OnCreateTaskPressed()
{
    //Выбор сотрудника кому
    QSelEmplDlg dlgSelEmpl;
    if(dlgSelEmpl.exec() == QDialog::Accepted)
    {
        //Назначение задачи
        QNewTaskDlg dlgNewTask(dlgSelEmpl.m_uuidSelectedEmpl);
        dlgNewTask.exec();
    }
}
