#include "CarshService/CarshSubWidgets/qcarshdlg.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QString>
#include <QPixmap>
#include <QImageReader>
#include <QImage>
#include <QMessageBox>
#include <QSqlQuery>
#include <QPixmap>
#include <QPalette>
#include <QColorDialog>
#include <QImage>
#include "common.h"


QCarshDlg::QCarshDlg(QUuid uuidCars):QDialog()
{
    m_uuidSourceCarsh = uuidCars;

    QVBoxLayout * pVMainLayout = new QVBoxLayout;
    this->setLayout(pVMainLayout);

    m_currentRGB=qRgb(255,255,255);
    m_strLogoPath = QString("");

    m_pNameLineText = new QLineText("Название");
    pVMainLayout->addWidget(m_pNameLineText);

    m_pLogoLabel = new QLabel();
    pVMainLayout->addWidget(m_pLogoLabel);

    m_pColorButton = new QPushButton("Цвет");
    connect(m_pColorButton,SIGNAL(pressed()),this,SLOT(OnColorPressed()));
    pVMainLayout->addWidget(m_pColorButton);

    QPushButton * pULButton = new QPushButton("Реквизиты ЮЛ");
    connect(pULButton,SIGNAL(pressed()),this,SLOT(OnULPressed()));
    pVMainLayout->addWidget(pULButton);

    QPushButton * pAddLogoButton = new QPushButton("Установить логотип");
    connect(pAddLogoButton,SIGNAL(pressed()),this,SLOT(OnAddLogoPressed()));
    pVMainLayout->addWidget(pAddLogoButton);

    QPushButton * pApplyButton = new QPushButton("Применить");
    connect(pApplyButton,SIGNAL(pressed()),this,SLOT(OnApplyPressed()));
    pVMainLayout->addWidget(pApplyButton);


    if(m_uuidSourceCarsh!=QUuid()) LoadFromBd();
}

void QCarshDlg::LoadFromBd()
{
    QString strQuery = QString("select Название, Логотип, Цвет , ЮЛ from Заказчики where id='%1'").arg(m_uuidSourceCarsh.toString());
    QSqlQuery query;
    query.exec(strQuery);
    while(query.next())
    {
        m_pNameLineText->setText(query.value(0).toString());

        QString tempStr = query.value(1).toString();
        QImage tmpImg = Base64ToImage(tempStr);
        QPixmap tempPixmap = QPixmap::fromImage(tmpImg);
        m_pLogoLabel->setPixmap(tempPixmap);

        m_currentRGB = QRgb(query.value(2).toInt());

        m_pColorButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(qRed(m_currentRGB)).arg(qGreen(m_currentRGB)).arg(qBlue(m_currentRGB)));

        m_ulDlg.LoadFromBD(query.value(3).toString());
    }
}

void QCarshDlg::OnColorPressed()
{
    QColorDialog dlg(QColor::fromRgb(m_currentRGB));
    if(dlg.exec()==QDialog::Accepted)
    {
        m_currentRGB = dlg.selectedColor().rgb();

        m_pColorButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(qRed(m_currentRGB)).arg(qGreen(m_currentRGB)).arg(qBlue(m_currentRGB)));
    }
}

void QCarshDlg::OnAddLogoPressed()
{
    m_strLogoPath = QFileDialog::getOpenFileName(this , "Выберите логотип" , QString(), "All files (*.*);;JPEG (*.jpg *.jpeg);;BMP (*.bmp)");
    if(m_strLogoPath.length()>2)
    {
        QImageReader imageReader(m_strLogoPath);
        QPixmap logoPixMap = QPixmap::fromImageReader(&imageReader);

        m_pLogoLabel->setPixmap(logoPixMap);
    }
}

void QCarshDlg::OnULPressed()
{
    m_ulDlg.exec();
}

void QCarshDlg::OnApplyPressed()
{
    if(m_uuidSourceCarsh==QUuid())//Новый
    {
        if(m_pNameLineText->getText().length()>2)
            if(m_strLogoPath.length()>2)
            {
                QString strLogo = PictureFileToBase64(m_strLogoPath);

                QUuid carshUuid = QUuid::createUuid();

                QString strUL = m_ulDlg.SaveToBD();

                QString strExec = QString("insert into Заказчики (id , Название , Логотип, Цвет , ЮЛ) values ('%1' , '%2' , '%3' , '%4' , '%5')").arg(carshUuid.toString()).arg(m_pNameLineText->getText()).arg(strLogo).arg(m_currentRGB).arg(strUL);

                QSqlQuery query;

                query.exec(strExec);

                accept();

                return;
            }
    }
    else //Обновляем
    {
        QString strExec;

        if(m_strLogoPath.length()>2)//Выбрали новый логотип
        {
            QString strLogo = PictureFileToBase64(m_strLogoPath);

            strExec = QString("update Заказчики set Название='%1' , Логотип = '%2', Цвет='%3' where id='%4'").arg(m_pNameLineText->getText()).arg(strLogo).arg(m_currentRGB).arg(m_uuidSourceCarsh.toString());
        }
        else
            strExec = QString("update Заказчики set Название='%1' , Цвет='%2' where id='%3'").arg(m_pNameLineText->getText()).arg(m_currentRGB).arg(m_uuidSourceCarsh.toString());

        QSqlQuery query;

        query.exec(strExec);

        m_ulDlg.SaveToBD();

        accept();

        return;
    }
    QMessageBox::information(this , "Предупреждение" , "Для записи оператора введите название и выберите логотип.");
}
