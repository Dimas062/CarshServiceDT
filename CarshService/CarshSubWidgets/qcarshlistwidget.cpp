#include "qcarshlistwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPixmap>
#include "qcarshdlg.h"
#include <QSqlQuery>
#include <QListWidgetItem>
#include "common.h"
#include <QTableWidget>

QCarshListWidget::QCarshListWidget(QWidget *parent)
    : QWidget{parent}
{
    m_strCurrentCarshUuid = "";

    QHBoxLayout * pHMainLayout = new QHBoxLayout;
    this->setLayout(pHMainLayout);

    QVBoxLayout * pVListLayout = new QVBoxLayout;

    m_pCarshsListWidget = new QListWidget();
    m_pCarshsListWidget->setIconSize(QSize(100,30));
    connect(m_pCarshsListWidget , SIGNAL(itemDoubleClicked(QListWidgetItem*)) , this , SLOT(carshDoubleClicked(QListWidgetItem*)));
    connect(m_pCarshsListWidget , SIGNAL(itemClicked(QListWidgetItem*)) , this , SLOT(carshClicked(QListWidgetItem*)));


    pVListLayout->addWidget(m_pCarshsListWidget);

    QHBoxLayout * pButtonsHLayout = new QHBoxLayout;

    QPushButton * pAddCarshButton = new QPushButton("Добавить");
    connect(pAddCarshButton,SIGNAL(pressed()),this,SLOT(OnAddCarshPressed()));
    pButtonsHLayout->addWidget(pAddCarshButton);

    QPushButton * pRemoveCarshButton = new QPushButton("Удалить");
    connect(pRemoveCarshButton,SIGNAL(pressed()),this,SLOT(OnRemoveCarshPressed()));
    pButtonsHLayout->addWidget(pRemoveCarshButton);

    pVListLayout->addLayout(pButtonsHLayout);

    pHMainLayout->addLayout(pVListLayout , 1);

    QVBoxLayout * pVSettingsLayout = new QVBoxLayout;

    m_pWashGoogleTableIdLineText = new QLineText("Id Google-таблицы моек");
    m_pWashGoogleTableIdLineText->SetToolTip("Id Google-таблицы моек, в которую посуточно выгружается\n статистика по выполненным работам каждой точкой");
    pVSettingsLayout->addWidget(m_pWashGoogleTableIdLineText);

    QPushButton * pApplySettingsButton = new QPushButton("Применить");
    connect(pApplySettingsButton,SIGNAL(pressed()),this,SLOT(OnApplySettingsPressed()));
    pVSettingsLayout->addWidget(pApplySettingsButton);

    pHMainLayout->addLayout(pVSettingsLayout , 3);

    UpdateCarshsList();
}

void QCarshListWidget::OnApplySettingsPressed()
{
    QString strExec = QString("update Заказчики set ГуглТаблицаМоек='%1'  where id = '%2'").arg(m_pWashGoogleTableIdLineText->getText()).arg(m_strCurrentCarshUuid);
    QSqlQuery query;
    query.exec(strExec);
}

void QCarshListWidget::UpdateSelectedCarshSettings()
{
    QString strExec = QString("select ГуглТаблицаМоек from Заказчики where id = '%1'").arg(m_strCurrentCarshUuid);
    QSqlQuery query;
    query.exec(strExec);
    while(query.next())
    {
        m_pWashGoogleTableIdLineText->setText(query.value(0).toString());
    }
}

void QCarshListWidget::carshClicked(QListWidgetItem * pItem)
{
    m_strCurrentCarshUuid = pItem->data(Qt::UserRole + 1).toString();
    UpdateSelectedCarshSettings();
}


void QCarshListWidget::UpdateCarshsList()
{
    m_pCarshsListWidget->clear();

    QString strExec = QString("select id, Название, Логотип from Заказчики");

    QSqlQuery query;

    query.exec(strExec);

    while(query.next())
    {
        QString strLogo(query.value(2).toString());

        QImage imgLogo = Base64ToImage(strLogo);

        QPixmap pxmpLogo = QPixmap::fromImage(imgLogo);

        QListWidgetItem * pCurrItem= new QListWidgetItem();
        pCurrItem->setText(query.value(1).toString());
        pCurrItem->setIcon(QIcon(pxmpLogo));
        pCurrItem->setData(Qt::UserRole , QVariant(query.value(2).toUuid()));
        pCurrItem->setData(Qt::UserRole + 1 , query.value(0));

        m_pCarshsListWidget->addItem(pCurrItem);
    }
}

void QCarshListWidget::OnAddCarshPressed()
{
    QCarshDlg dlg;
    dlg.exec();

    UpdateCarshsList();
}
void QCarshListWidget::OnRemoveCarshPressed()
{

}

void QCarshListWidget::carshDoubleClicked(QListWidgetItem* item)
{
    QCarshDlg dlg(item->data(Qt::UserRole + 1).toUuid());
    dlg.exec();

    UpdateCarshsList();
}
