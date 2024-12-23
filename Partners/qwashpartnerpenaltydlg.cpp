#include "qwashpartnerpenaltydlg.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlQuery>
#include <QDateTime>
#include <QImageReader>
#include <QFileDialog>
#include <QMessageBox>
#include "common.h"
#include "BDPatterns.h"

QWashPartnerPenaltyDlg::QWashPartnerPenaltyDlg(QString strIdTask , QString strIdType, bool bNigth)
{
    //пробуем найти эту рекламацию (в этом диалоге не типичная система с вызовом лоад и сейв из БД
    m_strSourcePenId = QString();
    QString execPenId = QString("select id , Количество from \"Отмена Мойки\" where Задача = '%1' and Тип='%2' and  Ночь = '%3' and Удалено=false").arg(strIdTask).arg(strIdType).arg(bool_to_str(bNigth));
    QSqlQuery sourcePenQuery;
    sourcePenQuery.exec(execPenId);
    int iCount = 0;

    m_pPicturesWidget = new QPicturesWidget(nullptr , true , true);
    m_pPicturesWidget->setMinimumHeight(300);

    while(sourcePenQuery.next())
    {

        m_strSourcePenId = sourcePenQuery.value(0).toString();

        iCount = sourcePenQuery.value(1).toInt();

        /*загрузим картинки*/
        QString strPicsExec = QString("select Документы.Изображение from Документы, \"Задача-Документы задач\" where Документы.id=\"Задача-Документы задач\".Документ and \"Задача-Документы задач\".Задача='%1'").arg(m_strSourcePenId);

        QSqlQuery PicsQuery;

        PicsQuery.exec(strPicsExec);

        while(PicsQuery.next())
        {

            QString tmpStr = PicsQuery.value(0).toString();

            QImage tmpImg = Base64ToImage(tmpStr);

            m_pPicturesWidget->AddImage(tmpImg);

        }
    }

    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    m_strTaskId = strIdTask;
    m_strIdType = strIdType;
    m_bNigth = bNigth;

    QLabel * pTopLabel = new QLabel();
    pVMainLayout->addWidget(pTopLabel);
    pTopLabel->setText("<b>Рекламация Мойка<\b>");
    pTopLabel->setStyleSheet("font-size: 20px;");

    QString strNigth;

    if(bNigth) strNigth = " (Ночь)";
    else strNigth = " (День)";


    QString strTypeExec = QString("select Тип from \"Типы задач Мойка\" where id='%1'").arg(strIdType);

    QSqlQuery typeQuery;
    typeQuery.exec(strTypeExec);
    while(typeQuery.next())
    {
        QLabel * pTypeLabel = new QLabel();
        pVMainLayout->addWidget(pTypeLabel);
        pTypeLabel->setText(QString("<b>Задача: %1 %2 <\b>").arg(typeQuery.value(0).toString()).arg(strNigth));
        pTypeLabel->setStyleSheet("font-size: 16px;");
    }

    pVMainLayout->addSpacing(5);

    QHBoxLayout * pHCountLoyout = new QHBoxLayout;

    QLabel * pCountLabel = new QLabel("Количество позиций:");
    pHCountLoyout->addWidget(pCountLabel);
    m_pCountLineEdit = new QLineEdit;
    m_pCountLineEdit->setText(QString("%1").arg(iCount));
    pHCountLoyout->addWidget(m_pCountLineEdit);
    pVMainLayout->addLayout(pHCountLoyout);



    QPushButton * pAddFotoButton = new QPushButton("Добавить фото");
    connect(pAddFotoButton,SIGNAL(pressed()),this,SLOT(OnAddPicPressed()));
    pVMainLayout->addWidget(pAddFotoButton);

    QPushButton * pDelFotoButton = new QPushButton("Удалить фото");
    connect(pDelFotoButton,SIGNAL(pressed()),this,SLOT(OnDelPicPressed()));
    pVMainLayout->addWidget(pDelFotoButton);


    pVMainLayout->addWidget(m_pPicturesWidget);


    QPushButton * pApplyButton = new QPushButton("Применить");
    connect(pApplyButton,SIGNAL(pressed()),this,SLOT(OnApplyPressed()));
    pVMainLayout->addWidget(pApplyButton);
}


void QWashPartnerPenaltyDlg::OnAddPicPressed()
{
    QStringList strPaths = QFileDialog::getOpenFileNames(this , "Выберите фотографии" , QString(), "All files (*.*);;JPEG (*.jpg *.jpeg);;BMP (*.bmp)");
    foreach(QString strPath, strPaths)
    {
        QImageReader imageReader(strPath);
        QImage img = imageReader.read();

        m_pPicturesWidget->AddImage(img);
    }
}

void QWashPartnerPenaltyDlg::OnDelPicPressed()
{
    m_pPicturesWidget->RemovePicture();
}

void QWashPartnerPenaltyDlg::OnApplyPressed()
{
    int iCount = m_pCountLineEdit->text().toInt();
    if(iCount < 1)
    {
        QMessageBox::information(this , "Предупреждение" , "Введите количество");
        return;
    }

    if(m_strSourcePenId == QString()) //Новая рекламация
    {

        QUuid newPen = QUuid::createUuid();

        QString strExec = QString("insert into \"Отмена Мойки\" (id , Задача, Тип, Ночь, Количество) values ('%1' , '%2' , '%3', '%4' , '%5')").arg(newPen.toString()).arg(m_strTaskId).arg(m_strIdType).arg(bool_to_str(m_bNigth)).arg(iCount);
        QSqlQuery query;
        query.exec(strExec);

        /*Затем перебрать и записать картинки*/
        for (int iPicCounter = 0; iPicCounter < m_pPicturesWidget->m_Pictures.size(); ++iPicCounter)
        {
            CreateTaskDocRecord(newPen , ImageToBase64(m_pPicturesWidget->m_Pictures.at(iPicCounter)));
        }
    }
    else //Обновляем существующую рекламацию
    {
        QString strExec = QString("update \"Отмена Мойки\" set Количество = '%1' where id='%2'").arg(iCount).arg(m_strSourcePenId);
        QSqlQuery query;
        query.exec(strExec);

        /*Фото/документы*/
        /*Сначала удалим все старые (включая акт-протокол)*/
        RemoveTaskDocs(QUuid::fromString(m_strSourcePenId));

        /*Затем перебрать и записать картинки*/
        for (int iPicCounter = 0; iPicCounter < m_pPicturesWidget->m_Pictures.size(); ++iPicCounter)
        {
            CreateTaskDocRecord(QUuid::fromString(m_strSourcePenId) , ImageToBase64(m_pPicturesWidget->m_Pictures.at(iPicCounter)));
        }
    }

    accept();

}
