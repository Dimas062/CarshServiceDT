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
    QHBoxLayout * pHMainLayout = new QHBoxLayout;
    this->setLayout(pHMainLayout);

    QVBoxLayout * pVListLayout = new QVBoxLayout;

    m_pCarshsListWidget = new QListWidget();
    m_pCarshsListWidget->setIconSize(QSize(100,30));
    connect(m_pCarshsListWidget , SIGNAL(itemDoubleClicked(QListWidgetItem*)) , this , SLOT(carshDoubleClicked(QListWidgetItem*)));

    pVListLayout->addWidget(m_pCarshsListWidget);

    QHBoxLayout * pButtonsHLayout = new QHBoxLayout;

    QPushButton * pAddCarshButton = new QPushButton("Добавить");
    connect(pAddCarshButton,SIGNAL(pressed()),this,SLOT(OnAddCarshPressed()));
    pButtonsHLayout->addWidget(pAddCarshButton);

    QPushButton * pRemoveCarshButton = new QPushButton("Удалить");
    connect(pRemoveCarshButton,SIGNAL(pressed()),this,SLOT(OnRemoveCarshPressed()));
    pButtonsHLayout->addWidget(pRemoveCarshButton);

    pVListLayout->addLayout(pButtonsHLayout);

    pHMainLayout->addLayout(pVListLayout);

    QTabWidget * pTabWidget = new QTabWidget();

    pHMainLayout->addWidget(pTabWidget);

    UpdateCarshsList();
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
