#include "qwashpartnercardwidget.h"
#include <QVBoxLayout>
#include <QString>
#include <QSqlQuery>
#include <QListWidgetItem>
#include "common.h"
#include <QUuid>
#include "service/qselectavailablezakazs.h"
#include "service/qpointeditdlg.h"

extern int iUserType;


QWashPartnerCardWidget::QWashPartnerCardWidget(QWidget *parent)
    : QWidget{parent}
{
    m_strActivPartner = QString("NULL");

    QVBoxLayout * pVMainLayout = new QVBoxLayout;

    m_pTopLabel = new QLabel("<b>Партнер \"Мойка\"</b>");
    m_pTopLabel->setStyleSheet("font-size: 20px;\
                               color: black;");
    m_pTopLabel->setAlignment(Qt::AlignHCenter);
    pVMainLayout->addWidget(m_pTopLabel);

    m_pActivLabel = new QLabel("Учетная запись не активна");
    m_pActivLabel->setStyleSheet("font-size: 16px;\
                               color: black;");
    m_pActivLabel->setAlignment(Qt::AlignHCenter);
    pVMainLayout->addWidget(m_pActivLabel);

    m_bIsAccountActiv = false;

    m_pNameLabel = new QLabel("Название: ");
    m_pNameLabel->setStyleSheet("font-size: 16px;\
                               color: black;");
    pVMainLayout->addWidget(m_pNameLabel);

    m_pPostavshikLabel = new QLabel("Заказчик: ");
    m_pPostavshikLabel->setStyleSheet("font-size: 16px;\
                               color: black;");
    pVMainLayout->addWidget(m_pPostavshikLabel);

    m_pAdressLabel = new QLabel("Адрес: ");
    m_pAdressLabel->setStyleSheet("font-size: 16px;\
                               color: black;");
    pVMainLayout->addWidget(m_pAdressLabel);

    m_pINNLabel = new QLabel("ИНН: ");
    m_pINNLabel->setStyleSheet("font-size: 16px;\
                               color: black;");
    pVMainLayout->addWidget(m_pINNLabel);

    m_pBankLabel = new QLabel("Банк: ");
    m_pBankLabel->setStyleSheet("font-size: 16px;\
                               color: black;");
    pVMainLayout->addWidget(m_pBankLabel);

    m_pScetLabel = new QLabel("счет: ");
    m_pScetLabel->setStyleSheet("font-size: 16px;\
                               color: black;");
    pVMainLayout->addWidget(m_pScetLabel);

    m_pLoginLabel = new QLabel("Логин: ");
    m_pLoginLabel->setStyleSheet("font-size: 16px;\
                                color: black;");
    if(iUserType == CarshService)
        pVMainLayout->addWidget(m_pLoginLabel);

    m_pPasswordLabel = new QLabel("Пароль: ");
    m_pPasswordLabel->setStyleSheet("font-size: 16px;\
                                color: black;");
    if(iUserType == CarshService)
        pVMainLayout->addWidget(m_pPasswordLabel);

    QHBoxLayout * pPostavHLoyout = new QHBoxLayout;
    QLabel * pPostavshikLabel = new QLabel("Поставщик:");
    pPostavshikLabel->setStyleSheet("font-size: 16px;\
                                color: black;");
    pPostavHLoyout->addWidget(pPostavshikLabel);
    
    m_pPostavshikCombo = new QComboBox;
    connect(m_pPostavshikCombo , SIGNAL(currentIndexChanged(int)) , this , SLOT(PostavshikIndexChanget(int)));
    
    pPostavHLoyout->addWidget(m_pPostavshikCombo);
    pVMainLayout->addLayout(pPostavHLoyout);

    QPushButton * pZakazsButton = new QPushButton("Доступные заказчики");
    connect(pZakazsButton,SIGNAL(pressed()),this,SLOT(OnZakazsPressed()));
    pVMainLayout->addWidget(pZakazsButton);

    QPushButton * pULButton = new QPushButton("Реквизиты ЮЛ");
    connect(pULButton,SIGNAL(pressed()),this,SLOT(OnULPressed()));
    pVMainLayout->addWidget(pULButton);

    m_pActivateButton = new QPushButton("Актирвировать");
    if(iUserType == CarshService)
        pVMainLayout->addWidget(m_pActivateButton);
    connect(m_pActivateButton,SIGNAL(pressed()),this,SLOT(OnActivAccountPressed()));

    QLabel * pPointsLabel = new QLabel("<b>Точки</b>");
    pPointsLabel->setStyleSheet("font-size: 16px;\
                               color: black;");
    pPointsLabel->setAlignment(Qt::AlignHCenter);
    pVMainLayout->addWidget(pPointsLabel);

    m_pPointsListWidget = new QListWidget;
    pVMainLayout->addWidget(m_pPointsListWidget);
    connect(m_pPointsListWidget , SIGNAL(itemDoubleClicked(QListWidgetItem*)) , this , SLOT(pointsDoubleClicked(QListWidgetItem*)));


    QHBoxLayout * pPointsButtonLayout = new QHBoxLayout;

    QPushButton * pAddPointButton = new QPushButton("Добавить");
    connect(pAddPointButton,SIGNAL(pressed()),this,SLOT(OnAddPointPressed()));
    pPointsButtonLayout->addWidget(pAddPointButton);

    QPushButton * pRemovePointButton = new QPushButton("Удалить");
    connect(pRemovePointButton,SIGNAL(pressed()),this,SLOT(OnRemovePointPressed()));
    pPointsButtonLayout->addWidget(pRemovePointButton);

    pVMainLayout->addLayout(pPointsButtonLayout);

    this->setLayout(pVMainLayout);
}

void QWashPartnerCardWidget::pointsDoubleClicked(QListWidgetItem* pItem)
{
    QPointEditDlg dlg;
    dlg.LoadFromBD(pItem->data(Qt::UserRole).toString());
    if(dlg.exec()==QDialog::Accepted)
    {
        dlg.SaveUpdateToBD();
    }

    UpdatePointsList();
}

void QWashPartnerCardWidget::OnActivAccountPressed()
{
    if(m_strActivPartner != QString("NULL"))
    {
        if(m_bIsAccountActiv)//Если аккаунт активен - деактивируем
        {
            m_bIsAccountActiv = false;
            QString strExec = QString("update Партнеры set Подтвержден = 'false' where id='%1'").arg(m_strActivPartner);
            QSqlQuery query;
            query.exec(strExec);
            m_pActivLabel->setText("Учетная запись не активна");
            m_pActivateButton->setText("Активировать учетную запись");
        }
        else
        {
            m_bIsAccountActiv = true;
            QString strExec = QString("update Партнеры set Подтвержден = 'true' where id='%1'").arg(m_strActivPartner);
            QSqlQuery query;
            query.exec(strExec);
            m_pActivLabel->setText("Учетная запись активна");
            m_pActivateButton->setText("Деактивировать учетную запись");
        }
    }
}

void QWashPartnerCardWidget::PostavshikIndexChanget(int idx)
{
    QString strPostavId = m_pPostavshikCombo->itemData(idx).toString();

    QString strQuery = QString("update Партнеры set Поставщик='%1' where id='%2'").arg(strPostavId).arg(m_strActivPartner);
    QSqlQuery query;
    query.exec(strQuery);
}

void QWashPartnerCardWidget::OnULPressed()
{
    if(m_strActivPartner==QString("NULL")) return;

    if(m_ulDlg.exec()==QDialog::Accepted)
        m_ulDlg.SaveToBD();
}

void QWashPartnerCardWidget::UpdatePointsList()
{
    m_pPointsListWidget->clear();

    QString strPointsExec = QString("select Название, Адрес , id from \"Точки Партнеров\" where Партнер='%1' and Удалено<>'true'").arg(m_strActivPartner);

    QSqlQuery PointsQuery;
    PointsQuery.exec(strPointsExec);
    while(PointsQuery.next())
    {
        QListWidgetItem * pItem = new QListWidgetItem;
        pItem->setText(QString("%1 (%2)").arg(PointsQuery.value(0).toString()).arg(PointsQuery.value(1).toString()));
        pItem->setData(Qt::UserRole , PointsQuery.value(2).toUuid());
        m_pPointsListWidget->addItem(pItem);
    }
}

void QWashPartnerCardWidget::OnAddPointPressed()
{
    if(m_strActivPartner==QString("NULL")) return;

    QPointEditDlg dlg;
    if(dlg.exec()==QDialog::Accepted)
    {
        dlg.SaveNewToBD(m_strActivPartner);
    }

    UpdatePointsList();
}

void QWashPartnerCardWidget::OnRemovePointPressed()
{
    if(m_strActivPartner==QString("NULL")) return;

    QSqlQuery PointsQuery;
    QString strPointsExec;

    QListWidgetItem * pItem =  m_pPointsListWidget->currentItem();
    if(pItem)
    {
        QString strPointUuid = pItem->data(Qt::UserRole).toString();
        strPointsExec = QString("update \"Точки Партнеров\" set Удалено=true where id='%1'").arg(strPointUuid);
        PointsQuery.exec(strPointsExec);
    }

    UpdatePointsList();
}


void QWashPartnerCardWidget::SetActivPartner(QString strUuid)
{
    m_strActivPartner = strUuid;

    QString strPartnerExec = QString("select ЮЛ.Название, ЮЛ.Адрес, ЮЛ.ИНН, ЮЛ.Банк, ЮЛ.Счет, Партнеры.Поставщик , Партнеры.Подтвержден , Партнеры.Логин , Партнеры.Пароль, ЮЛ.id from ЮЛ, Партнеры where Партнеры.id='%1' and Партнеры.ЮЛ=ЮЛ.id").arg(strUuid);
    QSqlQuery query;
    query.exec(strPartnerExec);
    QString strPostavId;

    if(query.next())
    {
        m_pNameLabel->setText(QString("<b>Название: %1<\b>").arg(query.value(0).toString()));
        m_pPostavshikLabel->setText(QString("<b>Заказчик: %1<\b>").arg(query.value(5).toString()));
        m_pAdressLabel->setText(QString("<b>Адрес: %1<\b>").arg(query.value(1).toString()));
        m_pINNLabel->setText(QString("<b>ИНН: %1<\b>").arg(query.value(2).toString()));
        m_pBankLabel->setText(QString("<b>Банк: %1<\b>").arg(query.value(3).toString()));
        m_pScetLabel->setText(QString("<b>Счет: %1<\b>").arg(query.value(4).toString()));
        m_pLoginLabel->setText(QString("<b>Логин: %1<\b>").arg(query.value(7).toString()));
        m_pPasswordLabel->setText(QString("<b>Пароль: %1<\b>").arg(query.value(8).toString()));
        m_bIsAccountActiv = query.value(6).toBool();
        if(m_bIsAccountActiv)
        {
            m_pActivLabel->setText("Учетная запись активна");
            m_pActivateButton->setText("Деактивировать учетную запись");
        }
        else
        {
            m_pActivLabel->setText("Учетная запись не активна");
            m_pActivateButton->setText("Активировать учетную запись");
        }
        strPostavId = query.value(5).toString();
        m_ulDlg.LoadFromBD(query.value(9).toString());
    }


    UpdatePointsList();
    
    
    m_pPostavshikCombo->clear();
    QString strExec = QString("select Поставщики.id, ЮЛ.Название from ЮЛ, Поставщики where Поставщики.ЮЛ=ЮЛ.id");
    query.exec(strExec);
    while(query.next())
    {
        m_pPostavshikCombo->addItem(query.value(1).toString() , query.value(0).toUuid());
    }

    m_pPostavshikCombo->setCurrentIndex(m_pPostavshikCombo->findData(QVariant(QUuid::fromString(strPostavId))));
}

void QWashPartnerCardWidget::OnZakazsPressed()
{
    QSelectAvailableZakazs dlg(m_strActivPartner);
    dlg.exec();
}
