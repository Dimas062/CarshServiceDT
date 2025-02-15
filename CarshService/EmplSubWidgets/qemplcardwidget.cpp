#include "qemplcardwidget.h"
#include <QVBoxLayout>
#include <QSqlQuery>
#include "pictures/qfullscreenpicdialog.h"
#include "common.h"
#include <QMessageBox>
#include <QDateTime>
#include "service/qselectavailablezakazs.h"

QEmplCardWidget::QEmplCardWidget(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout;
    this->setLayout(pVMainLayout);

    pVMainLayout->addSpacing(5);

    m_pFIOLabel = new QLabel();
    pVMainLayout->addWidget(m_pFIOLabel);

    m_pLabelActive= new QLabel();
    pVMainLayout->addWidget(m_pLabelActive);

    m_pFotoPicLabel = new QPicLabel();
    m_pFotoPicLabel->setStyleSheet("QLabel {"
                                   "border-style: solid;"
                                   "border-width: 1px;"
                                   "border-color: black;"
                                   "border-radius: 7px;"
                                   "}");
    m_pFotoPicLabel->setAlignment(Qt::AlignCenter);
    connect(m_pFotoPicLabel,SIGNAL(clicked()), this , SLOT(OnFotoPictureClik()));
    pVMainLayout->addWidget(m_pFotoPicLabel);


    pVMainLayout->addSpacing(15);

    QPushButton * pZakazsButton = new QPushButton("Доступные заказчики");
    connect(pZakazsButton,SIGNAL(pressed()),this,SLOT(OnZakazsPressed()));
    pVMainLayout->addWidget(pZakazsButton);


    QPushButton * pDriverCardButton = new QPushButton("Права");
    connect(pDriverCardButton,SIGNAL(pressed()),this,SLOT(OnDriverCardPressed()));

    pVMainLayout->addWidget(pDriverCardButton);

    QPushButton * pPassportButton = new QPushButton("Паспорт");
    connect(pPassportButton,SIGNAL(pressed()),this,SLOT(OnPassportPressed()));

    pVMainLayout->addWidget(pPassportButton);

    QPushButton * pSertButton = new QPushButton("Лицензия");
    connect(pSertButton,SIGNAL(pressed()),this,SLOT(OnSertPressed()));

    pVMainLayout->addWidget(pSertButton);

    pVMainLayout->addSpacing(15);

    QPushButton * pRemoveButton = new QPushButton("Удалить");
    pRemoveButton->setIcon(QIcon(":/icons/remove_icon.png"));

    connect(pRemoveButton,SIGNAL(pressed()),this,SLOT(OnRemovePressed()));

    pVMainLayout->addWidget(pRemoveButton);

    m_pActivationButton = new QPushButton("Активировать");
    connect(m_pActivationButton,SIGNAL(pressed()),this,SLOT(OnActivPressed()));

    pVMainLayout->addWidget(m_pActivationButton);
    m_IsActive = false;


}

void QEmplCardWidget::OnZakazsPressed()
{
    QSelectAvailableZakazs dlg(m_strCurUserId);
    dlg.exec();
}

void QEmplCardWidget::OnFotoPictureClik()
{
    QFullScreenPicDialog dlg(&m_FotoImage , this);

    dlg.exec();
}

void QEmplCardWidget::SetActivEmpl(QString strUuid)
{
    m_strCurUserId = strUuid;
    QString strExec=QString("SELECT Пользователи.Фамилия, \"Пользователи\".\"Имя\" , Пользователи.Отчество , Пользователи.Подтвержден , Пользователи.\"Дата регистрации\" , Пользователи.Фото  FROM Пользователи where Пользователи.id = '%1'").arg(m_strCurUserId);

    QSqlQuery query;

    query.exec(strExec);

    if(query.next())
    {
        m_pFIOLabel->setText(QString("<h1> %1 %2 %3</h1>\nДата регистрации: %4").arg(query.value(0).toString()).arg(query.value(1).toString()).arg(query.value(2).toString()).arg(QDateTime::fromSecsSinceEpoch(query.value(4).toInt()).toString("dd.MM.yyyy hh:mm")));

        m_IsActive = query.value(3).toBool();
        if(m_IsActive)
        {
            m_pActivationButton->setText("Деактивировать");
            m_pLabelActive->setText("Учётная запись активирована");
        }
        else
        {
            m_pActivationButton->setText("Активировать");
            m_pLabelActive->setText("Учётная запись не активирована");
        }

        QString strFotoPicture= query.value(5).toString();

        m_FotoImage = Base64ToImage(strFotoPicture);

        m_pFotoPicLabel->SetCurrentImage(&m_FotoImage);
    }
}

void QEmplCardWidget::OnDriverCardPressed()
{
    QString strExec=QString("select Документы.Изображение from Документы, \"Пользователь-Документы\" where Документы.id=\"Пользователь-Документы\".Документ and \"Пользователь-Документы\".Пользователь='%1' and Документы.Тип='0f3d1416-cbf7-481e-8c1c-1e5b819e75f5'").arg(m_strCurUserId);
    QSqlQuery query;
    query.exec(strExec);
    if(query.next())
    {
        QString strPicture= query.value(0).toString();

        QImage Image = Base64ToImage(strPicture);

        QFullScreenPicDialog dlg(&Image , this);

        dlg.exec();
    }
}

void QEmplCardWidget::OnPassportPressed()
{
    QString strExec=QString("select Документы.Изображение from Документы, \"Пользователь-Документы\" where Документы.id=\"Пользователь-Документы\".Документ and \"Пользователь-Документы\".Пользователь='%1' and Документы.Тип='79e23c72-712b-4751-8a2b-4db82a270be3'").arg(m_strCurUserId);

    QSqlQuery query;
    query.exec(strExec);
    if(query.next())
    {
        QString strPicture= query.value(0).toString();

        QImage Image = Base64ToImage(strPicture);

        QFullScreenPicDialog dlg(&Image , this);

        dlg.exec();
    }
}
void QEmplCardWidget::OnSertPressed()
{
    QString strExec=QString("select Документы.Изображение from Документы, \"Пользователь-Документы\" where Документы.id=\"Пользователь-Документы\".Документ and \"Пользователь-Документы\".Пользователь='%1' and Документы.Тип='2e73791e-402d-4f7c-b777-bc3cc51e415b'").arg(m_strCurUserId);
    QSqlQuery query;
    query.exec(strExec);
    if(query.next())
    {
        QString strPicture= query.value(0).toString();

        QImage Image = Base64ToImage(strPicture);

        QFullScreenPicDialog dlg(&Image , this);

        dlg.exec();
    }
}

void QEmplCardWidget::OnActivPressed()
{
    if(m_IsActive)
    {
        QString strExec=QString("update Пользователи set Подтвержден='false' where id='%1'").arg(m_strCurUserId);
        QSqlQuery query;
        query.exec(strExec);
        m_IsActive = false;
        m_pLabelActive->setText("Учётная запись не активна");
        m_pActivationButton->setText("Активировать");
    }
    else
    {
        QString strExec=QString("update Пользователи set Подтвержден='true' where id='%1'").arg(m_strCurUserId);
        QSqlQuery query;
        query.exec(strExec);
        m_IsActive = true;
        m_pLabelActive->setText("Учётная запись активна");
        m_pActivationButton->setText("Деактивировать");
    }
}

void QEmplCardWidget::OnRemovePressed()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Злобин КаршСервис", "Удалить пользователя?",
                                  QMessageBox::Yes|QMessageBox::No);

    if(reply == QMessageBox::Yes)
    {
        QString strExec=QString("update Пользователи set Удалено='true' where id='%1'").arg(m_strCurUserId);
        QSqlQuery query;
        query.exec(strExec);

        //TODO: переход на следующего и т.д. т.к. этого удалили
    }
}
