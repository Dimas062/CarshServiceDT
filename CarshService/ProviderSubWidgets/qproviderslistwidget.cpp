#include "qproviderslistwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPixmap>
#include "../ProviderSubWidgets/qproviderdlg.h"
#include <QSqlQuery>
#include <QListWidgetItem>
#include "common.h"
#include <QTableWidget>
#include <QUuid>

QProvidersListWidget::QProvidersListWidget(QWidget *parent)
    : QWidget{parent}
{    QHBoxLayout * pHMainLayout = new QHBoxLayout;
    this->setLayout(pHMainLayout);

    QVBoxLayout * pVListLayout = new QVBoxLayout;

    m_pProvidersListWidget = new QListWidget();
    connect(m_pProvidersListWidget , SIGNAL(itemDoubleClicked(QListWidgetItem*)) , this , SLOT(providerDoubleClicked(QListWidgetItem*)));

    pVListLayout->addWidget(m_pProvidersListWidget);

    QHBoxLayout * pButtonsHLayout = new QHBoxLayout;

    QPushButton * pAddProviderButton = new QPushButton("Добавить");
    connect(pAddProviderButton,SIGNAL(pressed()),this,SLOT(OnAddProviderPressed()));
    pButtonsHLayout->addWidget(pAddProviderButton);

    QPushButton * pRemoveProviderButton = new QPushButton("Удалить");
    connect(pRemoveProviderButton,SIGNAL(pressed()),this,SLOT(OnRemoveProviderPressed()));
    pButtonsHLayout->addWidget(pRemoveProviderButton);

    pVListLayout->addLayout(pButtonsHLayout);

    pHMainLayout->addLayout(pVListLayout);

    QTabWidget * pTabWidget = new QTabWidget();

    pHMainLayout->addWidget(pTabWidget);

    UpdateProvidersList();
}


void QProvidersListWidget::UpdateProvidersList()
{
    m_pProvidersListWidget->clear();

    QString strExec = QString("select id, Название from Поставщики");

    QSqlQuery query;

    query.exec(strExec);

    while(query.next())
    {
        QListWidgetItem * pCurrItem= new QListWidgetItem();
        pCurrItem->setText(query.value(1).toString());
        pCurrItem->setData(Qt::UserRole + 1 , query.value(0));

        m_pProvidersListWidget->addItem(pCurrItem);
    }
}

void QProvidersListWidget::OnAddProviderPressed()
{
    QProviderDlg dlg;
    dlg.exec();

    UpdateProvidersList();
}
void QProvidersListWidget::OnRemoveProviderPressed()
{

}

void QProvidersListWidget::providerDoubleClicked(QListWidgetItem* item)
{
    QProviderDlg dlg(item->data(Qt::UserRole + 1).toUuid());
    dlg.exec();

    UpdateProvidersList();
}
